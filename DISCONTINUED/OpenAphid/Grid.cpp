/*
Copyright 2012 Aphid Mobile

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
 
   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

 */

#include <config.h>
#include "Grid.h"

#include "Director.h"
#include "G2DMacros.h"
#include "glu.h"
#include "Node.h"
#include "Camera.h"

namespace Aphid {
	///-------------------------------------------------------------------------------------------------------------------
	/// GridBase
	GridBase::GridBase(const GridSize& size, PassRefPtr<Texture2D> texture, bool flipped)
	{
		LEAK_DETECT_INC("GridBase");
		
		init(size, texture, flipped);
	}
	
	GridBase::GridBase(const GridSize& size)
	{
		LEAK_DETECT_INC("GridBase");
		
		Director* director = Director::sharedDirector();
		Size s = director->winSizeInPixels();
		
		unsigned long POTWide = ccNextPOT(s.width);
		unsigned long POTHigh = ccNextPOT(s.height);
		
/*
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED		
		CCTexture2DPixelFormat format = director->pixelFormat();
#else
		CCTexture2DPixelFormat format = kCCTexture2DPixelFormat_RGBA8888;
#endif
*/
		CCTexture2DPixelFormat format = director->pixelFormat();
		
		void *data = calloc((int)(POTWide * POTHigh * 4), 1);
		if( ! data ) {
			oa_warn("CCGrid: not enough memory");
			CRASH();
		}
		
		RefPtr<Texture2D> texture = Texture2D::create(data, format, POTWide, POTHigh, s);
		OA_FREE(data);
		
		init(size, texture, false);
	}
	
	GridBase::~GridBase()
	{
		LEAK_DETECT_DEC("GridBase");
		
		if (Director::sharedDirector())
		setActive(false);
	}
	
	void GridBase::init(const GridSize &size, PassRefPtr<Aphid::Texture2D> texture, bool flipped)
	{
		m_active = false;
		m_reuseGrid = 0;
		m_gridSize = size;
		m_texture = texture;
		m_textureFlipped = flipped;
		
		Size texSize = m_texture->contentSizeInPixels();
		m_step.x = texSize.width / m_gridSize.x;
		m_step.y = texSize.height / m_gridSize.y;
		
		m_grabber = Grabber::create();
		m_grabber->grab(m_texture.get());
		
		//calculateVertexPoints();
	}
		
	void GridBase::setActive(bool b)
	{
		m_active = b;
		if (!m_active) {
			Director* director = Director::sharedDirector();
			director->setProjection(director->projection());
		}
	}
	
	void GridBase::setTextureFlipped(bool b)
	{
		if (m_textureFlipped != b) {
			m_textureFlipped = b;
			calculateVertexPoints();
		}
	}
	
	void GridBase::applyLandscape()
	{
		Director* director = Director::sharedDirector();
		
		Size winSize = director->displaySizeInPixels();
		float w = winSize.width / 2;
		float h = winSize.height / 2;
		
		ccDeviceOrientation orientation  = director->deviceOrientation();
		
		switch (orientation) {
			case kCCDeviceOrientationLandscapeLeft:
				glTranslatef(w,h,0);
				glRotatef(-90,0,0,1);
				glTranslatef(-h,-w,0);
				break;
			case kCCDeviceOrientationLandscapeRight:
				glTranslatef(w,h,0);
				glRotatef(90,0,0,1);
				glTranslatef(-h,-w,0);
				break;
			case kCCDeviceOrientationPortraitUpsideDown:
				glTranslatef(w,h,0);
				glRotatef(180,0,0,1);
				glTranslatef(-w,-h,0);
				break;
			default:
				break;
		}
	}
	
	void GridBase::set2DProjection()
	{
		Size winSize = Director::sharedDirector()->winSizeInPixels();
		
		glLoadIdentity();
		glViewport(0, 0, winSize.width, winSize.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		ccglOrtho(0, winSize.width, 0, winSize.height, -1024, 1024);
		glMatrixMode(GL_MODELVIEW);
	}
	
	// This routine can be merged with Director
	void GridBase::set3DProjection()
	{
		Director* director = Director::sharedDirector();
		
		Size winSize = director->displaySizeInPixels();
		
		glViewport(0, 0, winSize.width, winSize.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, (GLfloat)winSize.width/winSize.height, 0.5f, 1500.0f);
		
		glMatrixMode(GL_MODELVIEW);	
		glLoadIdentity();
		gluLookAt(winSize.width/2, winSize.height/2, director->zEye(),
							winSize.width/2, winSize.height/2, 0,
							0.0f, 1.0f, 0.0f
							);
	}
	
	void GridBase::beforeDraw()
	{
		set2DProjection();
		m_grabber->beforeRender(m_texture.get());
	}
	
	void GridBase::afterDraw(Aphid::Node *target)
	{
		m_grabber->afterRender(m_texture.get());
		
		set3DProjection();
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
		applyLandscape();
#endif
		
		if(target->camera()->isDirty()) {			
			Point offset = target->anchorInPixels();
			
			//
			// TODO: Camera should be applied in the AnchorPoint
			//
			ccglTranslate(offset.x, offset.y, 0);
			target->camera()->locate();
			ccglTranslate(-offset.x, -offset.y, 0);
		}
		
		glBindTexture(GL_TEXTURE_2D, m_texture->name());
		
		blit();
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// Grid3D
	Grid3D::Grid3D(const GridSize& size, PassRefPtr<Texture2D> texture, bool flipped)
	: Base(size, texture, flipped)
	{
		LEAK_DETECT_INC("Grid3D");
		
		calculateVertexPoints();
	}
	
	Grid3D::Grid3D(const GridSize& size)
	: Base(size)
	{
		LEAK_DETECT_INC("Grid3D");
		calculateVertexPoints();
	}
	
	Grid3D::~Grid3D()
	{
		OA_FREE(m_texCoordinates);
		OA_FREE(m_vertices);
		OA_FREE(m_indices);
		OA_FREE(m_originalVertices);
		
		LEAK_DETECT_DEC("Grid3D");
	}
	
	void Grid3D::blit()
	{
		int n = m_gridSize.x * m_gridSize.y;
		
		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Unneeded states: GL_COLOR_ARRAY
		glDisableClientState(GL_COLOR_ARRAY);	
		
		glVertexPointer(3, GL_FLOAT, 0, m_vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, m_texCoordinates);
		glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, m_indices);
		
		// restore GL default state
		glEnableClientState(GL_COLOR_ARRAY);
	}
	
	void Grid3D::calculateVertexPoints()
	{
		float width = (float)m_texture->pixelWidth();
		float height = (float)m_texture->pixelHeight();
		float imageH = m_texture->contentSizeInPixels().height;
		
		int x, y, i;
		
		m_vertices = malloc((m_gridSize.x+1) * (m_gridSize.y+1) * sizeof(ccVertex3F));
		m_originalVertices = malloc((m_gridSize.x+1) * (m_gridSize.y + 1) * sizeof(ccVertex3F));
		m_texCoordinates = malloc((m_gridSize.x+1) * (m_gridSize.y + 1) * sizeof(Point));
		m_indices = (GLushort*)malloc(m_gridSize.x * m_gridSize.y * sizeof(GLushort) * 6);
		
		float *vertArray = (float*)m_vertices;
		float *texArray = (float*)m_texCoordinates;
		GLushort *idxArray = (GLushort *)m_indices;
		
		for( x = 0; x < m_gridSize.x; x++ )
		{
			for( y = 0; y < m_gridSize.y; y++ )
			{
				int idx = (y * m_gridSize.x) + x;
				
				float x1 = x * m_step.x;
				float x2 = x1 + m_step.x;
				float y1 = y * m_step.y;
				float y2 = y1 + m_step.y;
				
				GLushort a = x * (m_gridSize.y+1) + y;
				GLushort b = (x+1) * (m_gridSize.y+1) + y;
				GLushort c = (x+1) * (m_gridSize.y+1) + (y+1);
				GLushort d = x * (m_gridSize.y+1) + (y+1);
				
				GLushort	tempidx[6] = { a, b, d, b, c, d };
				
				memcpy(&idxArray[6*idx], tempidx, 6 * sizeof(GLushort));
				
				int l1[4] = { a*3, b*3, c*3, d*3 };
				ccVertex3F	e = {x1,y1,0};
				ccVertex3F	f = {x2,y1,0};
				ccVertex3F	g = {x2,y2,0};
				ccVertex3F	h = {x1,y2,0};
				
				ccVertex3F l2[4] = { e, f, g, h };
				
				int tex1[4] = { a*2, b*2, c*2, d*2 };
				Point tex2[4] = {PointMake(x1, y1), PointMake(x2, y1), PointMake(x2, y2), PointMake(x1, y2)};
				
				for( i = 0; i < 4; i++ )
				{
					vertArray[ l1[i] ] = l2[i].x;
					vertArray[ l1[i] + 1 ] = l2[i].y;
					vertArray[ l1[i] + 2 ] = l2[i].z;
					
					texArray[ tex1[i] ] = tex2[i].x / width;
					if(m_textureFlipped)
						texArray[ tex1[i] + 1 ] = (imageH - tex2[i].y) / height;
					else
						texArray[ tex1[i] + 1 ] = tex2[i].y / height;
				}
			}
		}
		
		memcpy(m_originalVertices, m_vertices, (m_gridSize.x + 1) * (m_gridSize.y + 1) * sizeof(ccVertex3F));
	}
	
	ccVertex3F Grid3D::vertex(const GridSize& pos)
	{
		int index = (pos.x * (m_gridSize.y+1) + pos.y) * 3;
		float *vertArray = (float *)m_vertices;
		
		ccVertex3F	vert = {vertArray[index], vertArray[index+1], vertArray[index+2]};
		
		return vert;
	}
	
	ccVertex3F Grid3D::originalVertex(const GridSize& pos)
	{
		int index = (pos.x * (m_gridSize.y+1) + pos.y) * 3;
		float *vertArray = (float *)m_originalVertices;
		
		ccVertex3F	vert = {vertArray[index], vertArray[index+1], vertArray[index+2]};
		
		return vert;
	}
	
	void Grid3D::setVertex(const GridSize& pos, const ccVertex3F& vertex)
	{
		int index = (pos.x * (m_gridSize.y+1) + pos.y) * 3;
		float *vertArray = (float *)m_vertices;
		vertArray[index] = vertex.x;
		vertArray[index+1] = vertex.y;
		vertArray[index+2] = vertex.z;
	}
	
	void Grid3D::reuse()
	{
		if (m_reuseGrid > 0) {
			memcpy(m_originalVertices, m_vertices, (m_gridSize.x+1) * (m_gridSize.y+1)*sizeof(ccVertex3F));
			m_reuseGrid--;
		}
	}
	
	///-------------------------------------------------------------------------------------------------------------------
	/// TiledGrid3D
	TiledGrid3D::TiledGrid3D(const GridSize& size, PassRefPtr<Texture2D> texture, bool flipped)
	: Base(size, texture, flipped)
	{
		LEAK_DETECT_INC("TiledGrid3D");
		
		calculateVertexPoints();
	}
	
	TiledGrid3D::TiledGrid3D(const GridSize& size)
	: Base(size)
	{
		LEAK_DETECT_INC("TiledGrid3D");
		calculateVertexPoints();
	}
	
	TiledGrid3D::~TiledGrid3D()
	{
		OA_FREE(m_texCoordinates);
		OA_FREE(m_vertices);
		OA_FREE(m_indices);
		OA_FREE(m_originalVertices);
		
		LEAK_DETECT_DEC("TiledGrid3D");
	}
	
	void TiledGrid3D::blit()
	{
		int n = m_gridSize.x * m_gridSize.y;
		
		// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_TEXTURE_COORD_ARRAY
		// Unneeded states: GL_COLOR_ARRAY
		glDisableClientState(GL_COLOR_ARRAY);	
		
		glVertexPointer(3, GL_FLOAT, 0, m_vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, m_texCoordinates);
		glDrawElements(GL_TRIANGLES, (GLsizei) n*6, GL_UNSIGNED_SHORT, m_indices);
		
		// restore default GL state
		glEnableClientState(GL_COLOR_ARRAY);
	}
	
	void TiledGrid3D::calculateVertexPoints()
	{
		float width = (float)m_texture->pixelWidth();
		float height = (float)m_texture->pixelHeight();
		float imageH = m_texture->contentSizeInPixels().height;
		
		int numQuads = m_gridSize.x * m_gridSize.y;
		
		m_vertices = malloc(numQuads * 12 * sizeof(GLfloat));
		m_originalVertices = malloc(numQuads * 12 * sizeof(GLfloat));
		m_texCoordinates = malloc(numQuads * 8 * sizeof(GLfloat));
		m_indices = (GLushort*)malloc(numQuads * 6 * sizeof(GLushort));
		
		float *vertArray = (float*)m_vertices;
		float *texArray = (float*)m_texCoordinates;
		GLushort *idxArray = (GLushort *)m_indices;
		
		int x, y;
		
		for(x = 0; x < m_gridSize.x; x++)	{
			for(y = 0; y < m_gridSize.y; y++)	{
				float x1 = x * m_step.x;
				float x2 = x1 + m_step.x;
				float y1 = y * m_step.y;
				float y2 = y1 + m_step.y;
				
				*vertArray++ = x1;
				*vertArray++ = y1;
				*vertArray++ = 0;
				*vertArray++ = x2;
				*vertArray++ = y1;
				*vertArray++ = 0;
				*vertArray++ = x1;
				*vertArray++ = y2;
				*vertArray++ = 0;
				*vertArray++ = x2;
				*vertArray++ = y2;
				*vertArray++ = 0;
				
				float newY1 = y1;
				float newY2 = y2;
				
				if( m_textureFlipped ) {
					newY1 = imageH - y1;
					newY2 = imageH - y2;
				}
				
				*texArray++ = x1 / width;
				*texArray++ = newY1 / height;
				*texArray++ = x2 / width;
				*texArray++ = newY1 / height;
				*texArray++ = x1 / width;
				*texArray++ = newY2 / height;
				*texArray++ = x2 / width;
				*texArray++ = newY2 / height;
			}
		}
		
		for( x = 0; x < numQuads; x++) {
			idxArray[x*6+0] = x*4+0;
			idxArray[x*6+1] = x*4+1;
			idxArray[x*6+2] = x*4+2;
			
			idxArray[x*6+3] = x*4+1;
			idxArray[x*6+4] = x*4+2;
			idxArray[x*6+5] = x*4+3;
		}
		
		memcpy(m_originalVertices, m_vertices, numQuads*12*sizeof(GLfloat));
	}
	
	void TiledGrid3D::setTile(const GridSize &pos, const ccQuad3 &coords)
	{
		int idx = (m_gridSize.y * pos.x + pos.y) * 4 * 3;
		float *vertArray = (float*)m_vertices;
		memcpy(&vertArray[idx], &coords, sizeof(ccQuad3));
	}
	
	ccQuad3 TiledGrid3D::originalTile(const GridSize &pos)
	{
		int idx = (m_gridSize.y * pos.x + pos.y) * 4 * 3;
		float *vertArray = (float*)m_originalVertices;
		
		ccQuad3 ret;
		memcpy(&ret, &vertArray[idx], sizeof(ccQuad3));
		
		return ret;
	}
	
	ccQuad3 TiledGrid3D::tile(const GridSize &pos)
	{
		int idx = (m_gridSize.y * pos.x + pos.y) * 4 * 3;
		float *vertArray = (float*)m_vertices;
		
		ccQuad3 ret;
		memcpy(&ret, &vertArray[idx], sizeof(ccQuad3));
		
		return ret;
	}
	
	void TiledGrid3D::reuse()
	{
		if (m_reuseGrid > 0) {
			int numQuads = m_gridSize.x * m_gridSize.y;
			
			memcpy(m_originalVertices, m_vertices, numQuads * 12 * sizeof(GLfloat));
			m_reuseGrid--;
		}
	}	
}
