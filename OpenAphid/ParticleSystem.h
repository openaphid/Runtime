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

#ifndef OpenAphid_ParticleSystem_h
#define OpenAphid_ParticleSystem_h

#include "Node.h"
#include <AtomicString.h>
#include "AtomicStringHash.h"
#include <StringHash.h>

namespace Aphid {
	enum {
		/** The Particle emitter lives forever */
		kCCParticleDurationInfinity = -1,
		
		/** The starting size of the particle is equal to the ending size */
		kCCParticleStartSizeEqualToEndSize = -1,
		
		/** The starting radius of the particle is equal to the ending radius */
		kCCParticleStartRadiusEqualToEndRadius = -1,
		
		// backward compatible
		kParticleStartSizeEqualToEndSize = kCCParticleStartSizeEqualToEndSize,
		kParticleDurationInfinity = kCCParticleDurationInfinity,
	};
	
	//* @enum
	enum {
		/** Gravity mode (A mode) */
		kCCParticleModeGravity,
		
		/** Radius mode (B mode) */
		kCCParticleModeRadius,	
	};
	
	
	/** @typedef tCCPositionType
	 possible types of particle positions
	 */
	typedef enum {
		/** Living particles are attached to the world and are unaffected by emitter repositioning. */
		kCCPositionTypeFree,
		
		/** Living particles are attached to the world but will follow the emitter repositioning.
		 Use case: Attach an emitter to an sprite, and you want that the emitter follows the sprite.
		 */
		kCCPositionTypeRelative,
		
		/** Living particles are attached to the emitter and are translated along with it. */
		kCCPositionTypeGrouped,
	}tCCPositionType;
	
	// backward compatible
	enum {
		kPositionTypeFree = kCCPositionTypeFree,
		kPositionTypeGrouped = kCCPositionTypeGrouped,
	}; 
	
	/** @struct tCCParticle
	 Structure that contains the values of each particle
	 */
	typedef struct sCCParticle {
		Point		pos;
		Point		startPos;
		
		ccColor4F	color;
		ccColor4F	deltaColor;
		
		float		size;
		float		deltaSize;
		
		float		rotation;
		float		deltaRotation;
		
		ccTime		timeToLive;
		
		union {
			// Mode A: gravity, direction, radial accel, tangential accel
			struct {
				CGPoint		dir;
				float		radialAccel;
				float		tangentialAccel;
			} A;
			
			// Mode B: radius mode
			struct {
				float		angle;
				float		degreesPerSecond;
				float		radius;
				float		deltaRadius;
			} B;
		} mode;
		
	}tCCParticle;
	
	class ParticleConfig : public Noncopyable {
		typedef ATF::HashMap<AtomicString, double, CaseFoldingHash > NumericMap;
		typedef ATF::HashMap<AtomicString, String, CaseFoldingHash > StringMap;
	public:
		typedef ATF::HashSet<AtomicString, CaseFoldingHash> KeysSet;
#define DECLARE_CONFIG(x) static const AtomicString x
		DECLARE_CONFIG(maxParticles);
		DECLARE_CONFIG(angle);
		DECLARE_CONFIG(angleVariance);
		DECLARE_CONFIG(duration);
		DECLARE_CONFIG(blendFuncSource);
		DECLARE_CONFIG(blendFuncDestination);
		DECLARE_CONFIG(startColorRed);
		DECLARE_CONFIG(startColorGreen);
		DECLARE_CONFIG(startColorBlue);
		DECLARE_CONFIG(startColorAlpha);
		DECLARE_CONFIG(startColorVarianceRed);
		DECLARE_CONFIG(startColorVarianceGreen);
		DECLARE_CONFIG(startColorVarianceBlue);
		DECLARE_CONFIG(startColorVarianceAlpha);
		DECLARE_CONFIG(finishColorRed);
		DECLARE_CONFIG(finishColorGreen);
		DECLARE_CONFIG(finishColorBlue);
		DECLARE_CONFIG(finishColorAlpha);
		DECLARE_CONFIG(finishColorVarianceRed);
		DECLARE_CONFIG(finishColorVarianceGreen);
		DECLARE_CONFIG(finishColorVarianceBlue);
		DECLARE_CONFIG(finishColorVarianceAlpha);
		DECLARE_CONFIG(startParticleSize);
		DECLARE_CONFIG(startParticleSizeVariance);
		DECLARE_CONFIG(finishParticleSize);
		DECLARE_CONFIG(finishParticleSizeVariance);
		DECLARE_CONFIG(sourcePositionx);
		DECLARE_CONFIG(sourcePositiony);
		DECLARE_CONFIG(sourcePositionVariancex);
		DECLARE_CONFIG(sourcePositionVariancey);
		DECLARE_CONFIG(rotationStart);
		DECLARE_CONFIG(rotationStartVariance);
		DECLARE_CONFIG(rotationEnd);
		DECLARE_CONFIG(rotationEndVariance);
		DECLARE_CONFIG(emitterType);
		DECLARE_CONFIG(gravityx);
		DECLARE_CONFIG(gravityy);
		DECLARE_CONFIG(speed);
		DECLARE_CONFIG(speedVariance);
		DECLARE_CONFIG(radialAcceleration);
		DECLARE_CONFIG(radialAccelVariance);
		DECLARE_CONFIG(tangentialAcceleration);
		DECLARE_CONFIG(tangentialAccelVariance);
		DECLARE_CONFIG(maxRadius);
		DECLARE_CONFIG(maxRadiusVariance);
		DECLARE_CONFIG(minRadius);
		DECLARE_CONFIG(rotatePerSecond);
		DECLARE_CONFIG(rotatePerSecondVariance);
		DECLARE_CONFIG(particleLifespan);
		DECLARE_CONFIG(particleLifespanVariance);
		DECLARE_CONFIG(textureFileName);
		DECLARE_CONFIG(textureImageData);
#undef DECLARE_CONFIG
		
	public:
		virtual ~ParticleConfig() {}
		ParticleConfig(){}
		ParticleConfig(AJ::ExecState* exec, AJ::AJObject* o);
		
		void setNumber(const AtomicString& key, double value)
		{
			m_numbers.add(key, value);
		}
		
		void setString(const AtomicString& key, const String& value)
		{
			m_strings.add(key, value);
		}
		
		float floatValue(const AtomicString& key, float defaultValue=0.0f) const
		{
			NumericMap::const_iterator it = m_numbers.find(key);
			return it == m_numbers.end() ? defaultValue : (float)(it->second);
		}
		
		int intValue(const AtomicString& key, int defaultValue=0) const
		{
			NumericMap::const_iterator it = m_numbers.find(key);
			return it == m_numbers.end() ? defaultValue : (int)(it->second);
		}
		
		RGBA colorValue(const AtomicString& key, RGBA defaultColor=WebColors::black) const
		{
			NumericMap::const_iterator it = m_numbers.find(key);
			return it == m_numbers.end() ? defaultColor : (RGBA)(it->second);
		}
		
		String stringValue(const AtomicString& key, const char* defaultValue=0) const
		{
			StringMap::const_iterator it = m_strings.find(key);
			return it == m_strings.end() ? defaultValue : it->second;
		}
		
	protected:
		static KeysSet s_supported_keys;
		static void fillKeys();
		
		NumericMap m_numbers;
		StringMap m_strings;
	};
	
	class ParticleSystem : public Node, public TextureProtocol {
		typedef Node Base;
		typedef ParticleSystem Self;
	public:
		virtual ~ParticleSystem();
		
		static PassRefPtr<ParticleSystem> create(const ParticleConfig& config)
		{
			ParticleSystem* ret = new ParticleSystem();
			ret->config(config);
			return adoptRef(ret);
		}
		
		//InheritanceWrapper
		virtual const AJ::ClassInfo* wrapperClassInfo() const;
		virtual AJ::AJObject* createWrapper(AJ::ExecState* exec, AJOAGlobalObject* globalObject);
		virtual void markObjects(AJ::MarkStack& markStack, unsigned markID);
		
		virtual Texture2D* texture() const {return m_texture.get();}
		virtual void setTexture(PassRefPtr<Texture2D> texture);
		
		virtual void update(TimeSec dt);
		
		virtual void config(const ParticleConfig& config);
		
		void setBlendAdditive(bool additive);
		bool blendAdditive() const;
		
		void setTangentialAccel(float t);
		float tangentialAccel() const;
		
		void setTangentialAccelVar(float t);
		float tangentialAccelVar() const;
		
		void setRadialAccel(float t);
		float radialAccel() const;
		
		void setRadialAccelVar(float t);
		float radialAccelVar() const;
		
		void setGravity(const Point& g);
		Point gravity() const;
		
		void setSpeed(float speed);
		float speed() const;
		
		void setSpeedVar(float speedVar);
		float speedVar() const;
		
		void setStartRadius(float startRadius);
		float startRadius() const;
		
		void setStartRadiusVar(float startRadiusVar);
		float startRadiusVar() const;
		
		void setEndRadius(float endRadius);
		float endRadius() const;
		
		void setEndRadiusVar(float endRadiusVar);
		float endRadiusVar() const;
		
		void setRotatePerSecond(float degrees);
		float rotatePerSecond() const;
		
		void setRotatePerSecondVar(float degrees);
		float rotatePerSecondVar() const;
		
		Point positionVariance() const {return m_posVar;}
		void setPositionVariance(const Point& p) {m_posVar = p;}
		
		float life() const {return m_life;}
		void setLife(float f) {m_life = f;}
		
		float lifeVariance() const {return m_lifeVar;}
		void setLifeVariance(float f) {m_lifeVar = f;}		
		
	protected:
		ParticleSystem();
		
		virtual void updateQuadWithParticle(tCCParticle* , const Point&) {}
		virtual void postStep() {}
		
		bool addParticle();
		bool isFull() {return m_particleCount == m_totalParticles;}
		void initParticle(tCCParticle* particle);
		void stopSystem();
		void resetSystem();
		virtual void clearResources();
		
		// is the particle system active ?
		bool m_active;
		// duration in seconds of the system. -1 is infinity
		float m_duration;
		// time elapsed since the start of the system (in seconds)
		float m_elapsed;
		
		// position is from "superclass" CocosNode
		Point m_sourcePosition;
		// Position variance
		Point m_posVar;
		
		// The angle (direction) of the particles measured in degrees
		float m_angle;
		// Angle variance measured in degrees;
		float m_angleVar;
		
		// Different modes
		
		size_t m_emitterMode;
		union {
			// Mode A:Gravity + Tangential Accel + Radial Accel
			struct {
				// gravity of the particles
				CGPoint gravity;
				
				// The speed the particles will have.
				float speed;
				// The speed variance
				float speedVar;
				
				// Tangential acceleration
				float tangentialAccel;
				// Tangential acceleration variance
				float tangentialAccelVar;
				
				// Radial acceleration
				float radialAccel;
				// Radial acceleration variance
				float radialAccelVar;
			} A;
			
			// Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
			struct {
				
				// The starting radius of the particles
				float startRadius;
				// The starting radius variance of the particles
				float startRadiusVar;
				// The ending radius of the particles
				float endRadius;
				// The ending radius variance of the particles
				float endRadiusVar;			
				// Number of degress to rotate a particle around the source pos per second
				float rotatePerSecond;
				// Variance in degrees for rotatePerSecond
				float rotatePerSecondVar;
			} B;
		} m_mode;
		
		// start ize of the particles
		float m_startSize;
		// start Size variance
		float m_startSizeVar;
		// End size of the particle
		float m_endSize;
		// end size of variance
		float m_endSizeVar;
		
		// How many seconds will the particle live
		float m_life;
		// Life variance
		float m_lifeVar;
		
		// Start color of the particles
		ccColor4F m_startColor;
		// Start color variance
		ccColor4F m_startColorVar;
		// End color of the particles
		ccColor4F m_endColor;
		// End color variance
		ccColor4F m_endColorVar;
		
		// start angle of the particles
		float m_startSpin;
		// start angle variance
		float m_startSpinVar;
		// End angle of the particle
		float m_endSpin;
		// end angle ariance
		float m_endSpinVar;
		
		
		// Array of particles
		tCCParticle* m_particles;
		// Maximum particles
		size_t m_totalParticles;
		// Count of active particles
		size_t m_particleCount;
		
		// How many particles can be emitted per second
		float m_emissionRate;
		float m_emitCounter;
		
		// Texture of the particles
		RefPtr<Texture2D> m_texture;
		// blend function
		ccBlendFunc	m_blendFunc;
		
		// movment type: free or grouped
		tCCPositionType	m_positionType;
		
		// Whether or not the node will be auto-removed when there are not particles
		bool m_autoRemoveOnFinish;
		
		//  particle idx
		size_t m_particleIdx;
	};
	
	class ParticleSystemPoint : public ParticleSystem {
		typedef ParticleSystem Base;
		typedef ParticleSystemPoint Self;
	public:
		virtual ~ParticleSystemPoint();
		
		static PassRefPtr<Self> create(const ParticleConfig& config)
		{
			Self* ret = new Self();
			ret->config(config);
			return adoptRef(ret);
		}
		
		virtual void config(const ParticleConfig& config);
		virtual void postStep();
		virtual void draw();
		
	protected:
		ParticleSystemPoint();
		
		virtual void updateQuadWithParticle(tCCParticle *particle, const Point &pos);
		virtual void clearResources();
		
		ccPointSprite* m_vertices;
#if CC_USES_VBO
		GLuint	m_verticesID;
#endif
	};
	
	///-------------------------------------------------------------------------------------------------------------------
	class ParticleSystemQuad : public ParticleSystem {
		typedef ParticleSystem Base;
		typedef ParticleSystemQuad Self;
	public:
		virtual ~ParticleSystemQuad();
		
		static PassRefPtr<Self> create(const ParticleConfig& config)
		{
			Self* ret = new Self();
			ret->config(config);
			return adoptRef(ret);
		}
		
		virtual void config(const ParticleConfig& config);
		
		virtual void draw();
		virtual void setTexture(PassRefPtr<Texture2D> texture);
		void setTexture(PassRefPtr<Texture2D> texture, const Rect& rect);
	protected:
		ParticleSystemQuad();
		
		void initTexCoordsWithRect(const Rect& rect);
		virtual void clearResources();
		void initIndices();
		virtual void updateQuadWithParticle(tCCParticle *particle, const Point &pos);
		virtual void postStep();
		
		ccV2F_C4B_T2F_Quad* m_quads;		// quads to be rendered
		GLushort* m_indices;		// indices
#if CC_USES_VBO
		GLuint m_quadsID;		// VBO id
#endif

	};
}

#endif
