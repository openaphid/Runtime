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

#ifndef OpenAphid_Grid_h
#define OpenAphid_Grid_h

#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"

#include "ccTypes.h"
#include "Texture2D.h"

namespace Aphid {
	class Node;
  ///-------------------------------------------------------------------------------------------------------------------
	class GridBase : public RefCounted<GridBase> {
	public:
		virtual ~GridBase();
		
		void beforeDraw();
		void afterDraw(Node* target);
		
		virtual void blit() = 0;
		virtual void reuse() = 0;
		virtual void calculateVertexPoints() = 0;
		
		bool active() const {return m_active;}
		void setActive(bool b);
		
		bool isTextureFlipped() const {return m_textureFlipped;}
		void setTextureFlipped(bool b);
		
		int reuseGrid() const {return m_reuseGrid;}
		void setReuseGrid(int times) {m_reuseGrid = times;}

		Point step() const {return m_step;}
		void setStep(const Point& p) {m_step = p;}
		
		GridSize gridSize() const {return m_gridSize;}
		
	protected:
		GridBase(const GridSize& size);
		GridBase(const GridSize& size, PassRefPtr<Texture2D> texture, bool flipped);
		
		void init(const GridSize& size, PassRefPtr<Texture2D> texture, bool flipped);
		void applyLandscape();
		void set2DProjection();
		void set3DProjection();
		
		bool m_active;
		int m_reuseGrid;
		GridSize m_gridSize;
		RefPtr<Texture2D> m_texture;
		Point m_step;
		RefPtr<Grabber> m_grabber;
		bool m_textureFlipped;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class Grid3D : public GridBase {
		typedef GridBase Base;
		typedef Grid3D Self;
	public:
		virtual ~Grid3D();
		
		static PassRefPtr<Self> create(const GridSize& size)
		{
			return adoptRef(new Self(size));
		}
		
		static PassRefPtr<Self> create(const GridSize& size, PassRefPtr<Texture2D> texture, bool flipped)
		{
			return adoptRef(new Self(size, texture, flipped));
		}
		
		virtual void blit();
		virtual void reuse();
		virtual void calculateVertexPoints();
		
		ccVertex3F vertex(const GridSize& pos);
		ccVertex3F originalVertex(const GridSize& pos);
		void setVertex(const GridSize& pos, const ccVertex3F& vertex);
		
	protected:
		Grid3D(const GridSize& size);
		Grid3D(const GridSize& size, PassRefPtr<Texture2D> texture, bool flipped);	
		
		GLvoid* m_texCoordinates;
		GLvoid* m_vertices;
		GLvoid* m_originalVertices;
		GLushort* m_indices;
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class TiledGrid3D : public GridBase {
		typedef GridBase Base;
		typedef TiledGrid3D Self;
	public:
		virtual ~TiledGrid3D();
		
		static PassRefPtr<Self> create(const GridSize& size)
		{
			return adoptRef(new Self(size));
		}
		
		static PassRefPtr<Self> create(const GridSize& size, PassRefPtr<Texture2D> texture, bool flipped)
		{
			return adoptRef(new Self(size, texture, flipped));
		}
		
		virtual void blit();
		virtual void reuse();
		virtual void calculateVertexPoints();
		
		ccQuad3 tile(const GridSize& pos);
		ccQuad3 originalTile(const GridSize& pos);
		void setTile(const GridSize& pos, const ccQuad3& coords);
		
	protected:
		TiledGrid3D(const GridSize& size);
		TiledGrid3D(const GridSize& size, PassRefPtr<Texture2D> texture, bool flipped);		
		
		GLvoid* m_texCoordinates;
		GLvoid* m_vertices;
		GLvoid* m_originalVertices;
		GLushort* m_indices;
	};
}

#endif
