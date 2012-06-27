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
#include "ParticleSystem.h"
#include "G2DMacros.h"
#include "Diagnostic.h"
#include "AJParticleSystem.h"
#include "AJTexture2D.h"
#include <algorithm>

namespace Aphid {
  using namespace AJ;

#define IMPLEMENT_CONFIG(x) const AtomicString ParticleConfig::x = #x
  IMPLEMENT_CONFIG(maxParticles);
  IMPLEMENT_CONFIG(angle);
  IMPLEMENT_CONFIG(angleVariance);
  IMPLEMENT_CONFIG(duration);
  IMPLEMENT_CONFIG(blendFuncSource);
  IMPLEMENT_CONFIG(blendFuncDestination);
  IMPLEMENT_CONFIG(startColorRed);
  IMPLEMENT_CONFIG(startColorGreen);
  IMPLEMENT_CONFIG(startColorBlue);
  IMPLEMENT_CONFIG(startColorAlpha);
  IMPLEMENT_CONFIG(startColorVarianceRed);
  IMPLEMENT_CONFIG(startColorVarianceGreen);
  IMPLEMENT_CONFIG(startColorVarianceBlue);
  IMPLEMENT_CONFIG(startColorVarianceAlpha);
  IMPLEMENT_CONFIG(finishColorRed);
  IMPLEMENT_CONFIG(finishColorGreen);
  IMPLEMENT_CONFIG(finishColorBlue);
  IMPLEMENT_CONFIG(finishColorAlpha);
  IMPLEMENT_CONFIG(finishColorVarianceRed);
  IMPLEMENT_CONFIG(finishColorVarianceGreen);
  IMPLEMENT_CONFIG(finishColorVarianceBlue);
  IMPLEMENT_CONFIG(finishColorVarianceAlpha);
  IMPLEMENT_CONFIG(startParticleSize);
  IMPLEMENT_CONFIG(startParticleSizeVariance);
  IMPLEMENT_CONFIG(finishParticleSize);
  IMPLEMENT_CONFIG(finishParticleSizeVariance);
  IMPLEMENT_CONFIG(sourcePositionx);
  IMPLEMENT_CONFIG(sourcePositiony);
  IMPLEMENT_CONFIG(sourcePositionVariancex);
  IMPLEMENT_CONFIG(sourcePositionVariancey);
  IMPLEMENT_CONFIG(rotationStart);
  IMPLEMENT_CONFIG(rotationStartVariance);
  IMPLEMENT_CONFIG(rotationEnd);
  IMPLEMENT_CONFIG(rotationEndVariance);
  IMPLEMENT_CONFIG(emitterType);
  IMPLEMENT_CONFIG(gravityx);
  IMPLEMENT_CONFIG(gravityy);
  IMPLEMENT_CONFIG(speed);
  IMPLEMENT_CONFIG(speedVariance);
  IMPLEMENT_CONFIG(radialAcceleration);
  IMPLEMENT_CONFIG(radialAccelVariance);
  IMPLEMENT_CONFIG(tangentialAcceleration);
  IMPLEMENT_CONFIG(tangentialAccelVariance);
  IMPLEMENT_CONFIG(maxRadius);
  IMPLEMENT_CONFIG(maxRadiusVariance);
  IMPLEMENT_CONFIG(minRadius);
  IMPLEMENT_CONFIG(rotatePerSecond);
  IMPLEMENT_CONFIG(rotatePerSecondVariance);
  IMPLEMENT_CONFIG(particleLifespan);
  IMPLEMENT_CONFIG(particleLifespanVariance);
  IMPLEMENT_CONFIG(textureFileName);
  IMPLEMENT_CONFIG(textureImageData);
#undef IMPLEMENT_CONFIG

  ParticleConfig::KeysSet ParticleConfig::s_supported_keys;

  void ParticleConfig::fillKeys()
  {
	  if (s_supported_keys.size() == 0) {
		  s_supported_keys.add(ParticleConfig::angle);
		  s_supported_keys.add(ParticleConfig::angleVariance);
		  s_supported_keys.add(ParticleConfig::blendFuncDestination);
		  s_supported_keys.add(ParticleConfig::blendFuncSource);
		  s_supported_keys.add(ParticleConfig::duration);
		  s_supported_keys.add(ParticleConfig::emitterType);
		  s_supported_keys.add(ParticleConfig::finishColorAlpha);
		  s_supported_keys.add(ParticleConfig::finishColorBlue);
		  s_supported_keys.add(ParticleConfig::finishColorGreen);
		  s_supported_keys.add(ParticleConfig::finishColorRed);
		  s_supported_keys.add(ParticleConfig::finishColorVarianceAlpha);
		  s_supported_keys.add(ParticleConfig::finishColorVarianceBlue);
		  s_supported_keys.add(ParticleConfig::finishColorVarianceGreen);
		  s_supported_keys.add(ParticleConfig::finishColorVarianceRed);
		  s_supported_keys.add(ParticleConfig::finishParticleSize);
		  s_supported_keys.add(ParticleConfig::finishParticleSizeVariance);
		  s_supported_keys.add(ParticleConfig::gravityx);
		  s_supported_keys.add(ParticleConfig::gravityy);
		  s_supported_keys.add(ParticleConfig::maxParticles);
		  s_supported_keys.add(ParticleConfig::maxRadius);
		  s_supported_keys.add(ParticleConfig::maxRadiusVariance);
		  s_supported_keys.add(ParticleConfig::minRadius);
		  s_supported_keys.add(ParticleConfig::particleLifespan);
		  s_supported_keys.add(ParticleConfig::particleLifespanVariance);
		  s_supported_keys.add(ParticleConfig::radialAccelVariance);
		  s_supported_keys.add(ParticleConfig::radialAcceleration);
		  s_supported_keys.add(ParticleConfig::rotatePerSecond);
		  s_supported_keys.add(ParticleConfig::rotatePerSecondVariance);
		  s_supported_keys.add(ParticleConfig::rotationEnd);
		  s_supported_keys.add(ParticleConfig::rotationEndVariance);
		  s_supported_keys.add(ParticleConfig::rotationStart);
		  s_supported_keys.add(ParticleConfig::rotationStartVariance);
		  s_supported_keys.add(ParticleConfig::sourcePositionVariancex);
		  s_supported_keys.add(ParticleConfig::sourcePositionVariancey);
		  s_supported_keys.add(ParticleConfig::sourcePositionx);
		  s_supported_keys.add(ParticleConfig::sourcePositiony);
		  s_supported_keys.add(ParticleConfig::speed);
		  s_supported_keys.add(ParticleConfig::speedVariance);
		  s_supported_keys.add(ParticleConfig::startColorAlpha);
		  s_supported_keys.add(ParticleConfig::startColorBlue);
		  s_supported_keys.add(ParticleConfig::startColorGreen);
		  s_supported_keys.add(ParticleConfig::startColorRed);
		  s_supported_keys.add(ParticleConfig::startColorVarianceAlpha);
		  s_supported_keys.add(ParticleConfig::startColorVarianceBlue);
		  s_supported_keys.add(ParticleConfig::startColorVarianceGreen);
		  s_supported_keys.add(ParticleConfig::startColorVarianceRed);
		  s_supported_keys.add(ParticleConfig::startParticleSize);
		  s_supported_keys.add(ParticleConfig::startParticleSizeVariance);
		  s_supported_keys.add(ParticleConfig::tangentialAccelVariance);
		  s_supported_keys.add(ParticleConfig::tangentialAcceleration);
		  s_supported_keys.add(ParticleConfig::textureFileName);
		  s_supported_keys.add(ParticleConfig::textureImageData);
	  }
  }

  ParticleConfig::ParticleConfig(AJ::ExecState *exec, AJ::AJObject *o)
  {
	  if (!o || !exec)
		  return;

	  fillKeys();

	  ///TODO: below code block adds 10K to binary size, why?
	  PropertyNameArray properties(exec->lexicalGlobalObject()->globalData());
	  o->getPropertyNames(exec, properties);
	  size_t len = properties.size();
	  for (size_t i = 0; i < len; i++) {
		  const Identifier& identifier = properties[i];
		  AJValue value = o->get(exec, identifier);
		  if (s_supported_keys.contains(identifier.ascii())) {
			  if (value.isString())
				  setString(identifier.ascii(), toString(value.toString(exec)));
			  else if (value.isNumber())
				  setNumber(identifier.ascii(), value.toNumber(exec));
		  } else
			  Diagnostic::warn(String::format("Incorrect config key: %s", identifier.ascii()));
	  }
  }

  ///-------------------------------------------------------------------------------------------------------------------
  ParticleSystem::ParticleSystem()
  : m_active(true)
  , m_duration(-1)
  , m_elapsed(0)
  , m_sourcePosition(PointZero)
  , m_posVar(PointZero)
  , m_angle(0)
  , m_angleVar(0)
  , m_emitterMode(kCCParticleModeGravity)
  , m_startSize(0)
  , m_startSizeVar(0)
  , m_endSize(0)
  , m_endSizeVar(0)
  , m_life(0)
  , m_lifeVar(0)
  , m_startColor(toColor4F(WebColors::white))
  , m_startColorVar(toColor4F(WebColors::white))
  , m_endColor(toColor4F(WebColors::white))
  , m_endColorVar(toColor4F(WebColors::white))
  , m_startSpin(0.0f)
  , m_startSpinVar(0.0f)
  , m_endSpin(0.0f)
  , m_endSpinVar(0.0f)
  , m_particles(0)
  , m_totalParticles(0)
  , m_particleCount(0)
  , m_emissionRate(0)
  , m_emitCounter(0)
  , m_positionType(kCCPositionTypeFree)
  , m_autoRemoveOnFinish(false)
  , m_particleIdx(0)
  {
	  LEAK_DETECT_INC("ParticleSystem");
	  m_blendFunc = (ccBlendFunc) {CC_BLEND_SRC, CC_BLEND_DST};
  }

  ParticleSystem::~ParticleSystem()
  {
	  LEAK_DETECT_DEC("ParticleSystem");
	  clearResources();
  }

  void ParticleSystem::clearResources()
  {
	  OA_FREE(m_particles);
  }

  void ParticleSystem::config(const ParticleConfig &cfg)
  {
	  m_totalParticles = cfg.intValue(ParticleConfig::maxParticles);

	  clearResources();

	  if (m_totalParticles == 0) {
		  m_active = false;
		  oa_error("Invalid particle config");
		  unscheduleUpdate();
		  return;
	  }

	  m_particles = (tCCParticle *) calloc(m_totalParticles, sizeof(tCCParticle));

	  RELEASE_ASSERT(m_particles);

	  m_active = true;

	  // default blend function
	  m_blendFunc = (ccBlendFunc) {CC_BLEND_SRC, CC_BLEND_DST};

	  // default movement type;
	  m_positionType = kCCPositionTypeFree;

	  // by default be in mode A:
	  m_emitterMode = kCCParticleModeGravity;

	  m_autoRemoveOnFinish = false;

	  // angle
	  m_angle = cfg.floatValue(ParticleConfig::angle);
	  m_angleVar = cfg.floatValue(ParticleConfig::angleVariance);

	  // duration
	  m_duration = cfg.floatValue(ParticleConfig::duration, kParticleDurationInfinity);

	  // blend function
	  m_blendFunc.src = cfg.intValue(ParticleConfig::blendFuncSource, CC_BLEND_SRC);
	  m_blendFunc.dst = cfg.intValue(ParticleConfig::blendFuncDestination, CC_BLEND_DST);

	  // color
	  float r, g, b, a;

	  r = cfg.floatValue(ParticleConfig::startColorRed);
	  g = cfg.floatValue(ParticleConfig::startColorGreen);
	  b = cfg.floatValue(ParticleConfig::startColorBlue);
	  a = cfg.floatValue(ParticleConfig::startColorAlpha);
	  m_startColor = (ccColor4F) {r, g, b, a};

	  r = cfg.floatValue(ParticleConfig::startColorVarianceRed);
	  g = cfg.floatValue(ParticleConfig::startColorVarianceGreen);
	  b = cfg.floatValue(ParticleConfig::startColorVarianceBlue);
	  a = cfg.floatValue(ParticleConfig::startColorVarianceAlpha);
	  m_startColorVar = (ccColor4F) {r, g, b, a};

	  r = cfg.floatValue(ParticleConfig::finishColorRed);
	  g = cfg.floatValue(ParticleConfig::finishColorGreen);
	  b = cfg.floatValue(ParticleConfig::finishColorBlue);
	  a = cfg.floatValue(ParticleConfig::finishColorAlpha);
	  m_endColor = (ccColor4F) {r, g, b, a};

	  r = cfg.floatValue(ParticleConfig::finishColorVarianceRed);
	  g = cfg.floatValue(ParticleConfig::finishColorVarianceGreen);
	  b = cfg.floatValue(ParticleConfig::finishColorVarianceBlue);
	  a = cfg.floatValue(ParticleConfig::finishColorVarianceAlpha);
	  m_endColorVar = (ccColor4F) {r, g, b, a};

	  // particle size
	  m_startSize = cfg.floatValue(ParticleConfig::startParticleSize);
	  m_startSizeVar = cfg.floatValue(ParticleConfig::startParticleSizeVariance);
	  m_endSize = cfg.floatValue(ParticleConfig::finishParticleSize);
	  m_endSizeVar = cfg.floatValue(ParticleConfig::finishParticleSizeVariance);


	  // position
	  float x = cfg.floatValue(ParticleConfig::sourcePositionx);
	  float y = cfg.floatValue(ParticleConfig::sourcePositiony);
	  setPosition(PointMake(x, y));
	  m_posVar.x = cfg.floatValue(ParticleConfig::sourcePositionVariancex);
	  m_posVar.y = cfg.floatValue(ParticleConfig::sourcePositionVariancey);


	  // Spinning
	  m_startSpin = cfg.floatValue(ParticleConfig::rotationStart);
	  m_startSpinVar = cfg.floatValue(ParticleConfig::rotationStartVariance);
	  m_endSpin = cfg.floatValue(ParticleConfig::rotationEnd);
	  m_endSpinVar = cfg.floatValue(ParticleConfig::rotationEndVariance);

	  m_emitterMode = cfg.intValue(ParticleConfig::emitterType);

	  // Mode A: Gravity + tangential accel + radial accel
	  if (m_emitterMode == kCCParticleModeGravity) {
		  // gravity
		  m_mode.A.gravity.x = cfg.floatValue(ParticleConfig::gravityx);
		  m_mode.A.gravity.y = cfg.floatValue(ParticleConfig::gravityy);

		  //
		  // speed
		  m_mode.A.speed = cfg.floatValue(ParticleConfig::speed);
		  m_mode.A.speedVar = cfg.floatValue(ParticleConfig::speedVariance);

		  // radial acceleration
		  String tmp = cfg.stringValue(ParticleConfig::radialAcceleration);
		  m_mode.A.radialAccel = tmp.isEmpty() ? 0 : tmp.toFloat();

		  tmp = cfg.stringValue(ParticleConfig::radialAccelVariance);
		  m_mode.A.radialAccelVar = tmp.isEmpty() ? 0 : tmp.toFloat();

		  // tangential acceleration
		  tmp = cfg.stringValue(ParticleConfig::tangentialAcceleration);
		  m_mode.A.tangentialAccel = tmp.isEmpty() ? 0 : tmp.toFloat();

		  tmp = cfg.stringValue(ParticleConfig::tangentialAccelVariance);
		  m_mode.A.tangentialAccelVar = tmp.isEmpty() ? 0 : tmp.toFloat();
	  }


		  // or Mode B: radius movement
	  else if (m_emitterMode == kCCParticleModeRadius) {
		  float maxRadius = cfg.floatValue(ParticleConfig::maxRadius);
		  float maxRadiusVar = cfg.floatValue(ParticleConfig::maxRadiusVariance);
		  float minRadius = cfg.floatValue(ParticleConfig::minRadius);

		  m_mode.B.startRadius = maxRadius;
		  m_mode.B.startRadiusVar = maxRadiusVar;
		  m_mode.B.endRadius = minRadius;
		  m_mode.B.endRadiusVar = 0;
		  m_mode.B.rotatePerSecond = cfg.floatValue(ParticleConfig::rotatePerSecond);
		  m_mode.B.rotatePerSecondVar = cfg.floatValue(ParticleConfig::rotatePerSecondVariance);

	  } else {
		  oa_error("Invalid emitter type in config");
		  m_active = false;
		  return;
	  }

	  // life span
	  m_life = cfg.floatValue(ParticleConfig::particleLifespan);
	  m_lifeVar = cfg.floatValue(ParticleConfig::particleLifespanVariance);

	  // emission Rate
	  m_emissionRate = m_totalParticles / m_life;

	  // texture
	  // Try to get the texture from the cache
	  String textureName = cfg.stringValue(ParticleConfig::textureFileName);

	  if (!textureName.isEmpty()) {
		  m_texture = Texture2D::create(textureName.utf8().data()); ///TODO: use texture cache
	  } else {
		  ///TODO: handle base64 data
		  /*
						 NSString *textureData = [dictionary valueForKey:@"textureImageData"];
						 NSAssert( textureData, @"CCParticleSystem: Couldn't load texture");

						 // if it fails, try to get it from the base64-gzipped data
						 unsigned char *buffer = NULL;
						 int len = base64Decode((unsigned char*)[textureData UTF8String], (unsigned int)[textureData length], &buffer);
						 NSAssert( buffer != NULL, @"CCParticleSystem: error decoding textureImageData");

						 unsigned char *deflated = NULL;
						 NSUInteger deflatedLen = ccInflateMemory(buffer, len, &deflated);
						 free( buffer );

						 NSAssert( deflated != NULL, @"CCParticleSystem: error ungzipping textureImageData");
						 NSData *data = [[NSData alloc] initWithBytes:deflated length:deflatedLen];

						 #ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
						 UIImage *image = [[UIImage alloc] initWithData:data];
						 #elif defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
						 NSBitmapImageRep *image = [[NSBitmapImageRep alloc] initWithData:data];
						 #endif

						 free(deflated); deflated = NULL;

						 self.texture = [[CCTextureCache sharedTextureCache] addCGImage:[image CGImage] forKey:textureName];
						 [data release];
						 [image release];
						 */
	  }

	  //ASSERT(m_texture);

	  scheduleUpdate();
  }

  //InheritanceWrapper
  const AJ::ClassInfo *ParticleSystem::wrapperClassInfo() const
  {
	  return &AJParticleSystem::s_info;
  }

  AJ::AJObject *ParticleSystem::createWrapper(AJ::ExecState *exec, AJOAGlobalObject *globalObject)
  {
	  return createAJOAWrapper<AJParticleSystem>(exec, globalObject, this);
  }

  void ParticleSystem::markObjects(AJ::MarkStack &markStack, unsigned int markID)
  {
	  if (isMarked(markID))
		  return;

	  Base::markObjects(markStack, markID);
	  if (m_texture)
		  m_texture->markObjects(markStack, markID);
  }

  bool ParticleSystem::addParticle()
  {
	  if (this->isFull())
		  return false;

	  tCCParticle *particle = &m_particles[m_particleCount];

	  initParticle(particle);
	  m_particleCount++;

	  return true;
  }

  void ParticleSystem::initParticle(tCCParticle *particle)
  {
	  // timeToLive
	  // no negative life. prevent division by 0
	  particle->timeToLive = m_life + m_lifeVar * CCRANDOM_MINUS1_1();
	  particle->timeToLive = std::max(0.0f, particle->timeToLive);

	  // position
	  particle->pos.x = m_sourcePosition.x + m_posVar.x * CCRANDOM_MINUS1_1();
	  particle->pos.x *= G2D_CONTENT_SCALE_FACTOR();
	  particle->pos.y = m_sourcePosition.y + m_posVar.y * CCRANDOM_MINUS1_1();
	  particle->pos.y *= G2D_CONTENT_SCALE_FACTOR();

	  // Color
	  ccColor4F start;
	  start.r = clampf(m_startColor.r + m_startColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
	  start.g = clampf(m_startColor.g + m_startColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
	  start.b = clampf(m_startColor.b + m_startColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
	  start.a = clampf(m_startColor.a + m_startColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);

	  ccColor4F end;
	  end.r = clampf(m_endColor.r + m_endColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
	  end.g = clampf(m_endColor.g + m_endColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
	  end.b = clampf(m_endColor.b + m_endColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
	  end.a = clampf(m_endColor.a + m_endColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);

	  particle->color = start;
	  particle->deltaColor.r = (end.r - start.r) / particle->timeToLive;
	  particle->deltaColor.g = (end.g - start.g) / particle->timeToLive;
	  particle->deltaColor.b = (end.b - start.b) / particle->timeToLive;
	  particle->deltaColor.a = (end.a - start.a) / particle->timeToLive;

	  // size
	  float startS = m_startSize + m_startSizeVar * CCRANDOM_MINUS1_1();
	  startS = std::max(0.0f, startS); // No negative value
	  startS *= G2D_CONTENT_SCALE_FACTOR();

	  particle->size = startS;
	  if (m_endSize == kCCParticleStartSizeEqualToEndSize)
		  particle->deltaSize = 0;
	  else {
		  float endS = m_endSize + m_endSizeVar * CCRANDOM_MINUS1_1();
		  endS = std::max(0.0f, endS);  // No negative values
		  endS *= G2D_CONTENT_SCALE_FACTOR();
		  particle->deltaSize = (endS - startS) / particle->timeToLive;
	  }

	  // rotation
	  float startA = m_startSpin + m_startSpinVar * CCRANDOM_MINUS1_1();
	  float endA = m_endSpin + m_endSpinVar * CCRANDOM_MINUS1_1();
	  particle->rotation = startA;
	  particle->deltaRotation = (endA - startA) / particle->timeToLive;

	  // position
	  if (m_positionType == kCCPositionTypeFree) {
		  Point p = convertToWorldSpace(PointZero);
		  particle->startPos = g2d_mult(p, G2D_CONTENT_SCALE_FACTOR());
	  }
	  else if (m_positionType == kCCPositionTypeRelative) {
		  particle->startPos = g2d_mult(m_position, G2D_CONTENT_SCALE_FACTOR());
	  }

	  // direction
	  float a = CC_DEGREES_TO_RADIANS( m_angle + m_angleVar * CCRANDOM_MINUS1_1() );

	  // Mode Gravity: A
	  if (m_emitterMode == kCCParticleModeGravity) {

		  Point v = {cosf(a), sinf(a)};
		  float s = m_mode.A.speed + m_mode.A.speedVar * CCRANDOM_MINUS1_1();
		  s *= G2D_CONTENT_SCALE_FACTOR();

		  // direction
		  particle->mode.A.dir = g2d_mult(v, s);

		  // radial accel
		  particle->mode.A.radialAccel = m_mode.A.radialAccel + m_mode.A.radialAccelVar * CCRANDOM_MINUS1_1();
		  particle->mode.A.radialAccel *= G2D_CONTENT_SCALE_FACTOR();

		  // tangential accel
		  particle->mode.A.tangentialAccel = m_mode.A.tangentialAccel + m_mode.A.tangentialAccelVar * CCRANDOM_MINUS1_1();
		  particle->mode.A.tangentialAccel *= G2D_CONTENT_SCALE_FACTOR();
	  }
		  // Mode Radius: B
	  else {
		  // Set the default diameter of the particle from the source position
		  float startRadius = m_mode.B.startRadius + m_mode.B.startRadiusVar * CCRANDOM_MINUS1_1();
		  float endRadius = m_mode.B.endRadius + m_mode.B.endRadiusVar * CCRANDOM_MINUS1_1();

		  startRadius *= G2D_CONTENT_SCALE_FACTOR();
		  endRadius *= G2D_CONTENT_SCALE_FACTOR();

		  particle->mode.B.radius = startRadius;

		  if (m_mode.B.endRadius == kCCParticleStartRadiusEqualToEndRadius)
			  particle->mode.B.deltaRadius = 0;
		  else
			  particle->mode.B.deltaRadius = (endRadius - startRadius) / particle->timeToLive;

		  particle->mode.B.angle = a;
		  particle->mode.B.degreesPerSecond = CC_DEGREES_TO_RADIANS(m_mode.B.rotatePerSecond + m_mode.B.rotatePerSecondVar * CCRANDOM_MINUS1_1());
	  }
  }

  void ParticleSystem::stopSystem()
  {
	  m_active = false;
	  m_elapsed = m_duration;
	  m_emitCounter = 0;
  }

  void ParticleSystem::resetSystem()
  {
	  m_active = true;
	  m_elapsed = 0;
	  for (m_particleIdx = 0; m_particleIdx < m_particleCount; ++m_particleIdx) {
		  tCCParticle *p = &m_particles[m_particleIdx];
		  p->timeToLive = 0;
	  }
  }

  void ParticleSystem::update(TimeSec dt)
  {
	  if (m_active && m_emissionRate) {
		  float rate = 1.0f / m_emissionRate;
		  m_emitCounter += dt;
		  while (m_particleCount < m_totalParticles && m_emitCounter > rate) {
			  addParticle();
			  m_emitCounter -= rate;
		  }

		  m_elapsed += dt;
		  if (m_duration != -1 && m_duration < m_elapsed)
			  stopSystem();
	  }

	  m_particleIdx = 0;

	  Point currentPosition = PointZero;
	  if (m_positionType == kCCPositionTypeFree) {
		  currentPosition = convertToWorldSpace(PointZero);
		  currentPosition.x *= G2D_CONTENT_SCALE_FACTOR();
		  currentPosition.y *= G2D_CONTENT_SCALE_FACTOR();
	  }
	  else if (m_positionType == kCCPositionTypeRelative) {
		  currentPosition = m_position;
		  currentPosition.x *= G2D_CONTENT_SCALE_FACTOR();
		  currentPosition.y *= G2D_CONTENT_SCALE_FACTOR();
	  }

	  while (m_particleIdx < m_particleCount) {
		  tCCParticle *p = &m_particles[m_particleIdx];

		  // life
		  p->timeToLive -= dt;

		  if (p->timeToLive > 0) {

			  // Mode A: gravity, direction, tangential accel & radial accel
			  if (m_emitterMode == kCCParticleModeGravity) {
				  Point tmp, radial, tangential;

				  radial = PointZero;
				  // radial acceleration
				  if (p->pos.x || p->pos.y)
					  radial = PointNormalize(p->pos);

				  tangential = radial;
				  radial = g2d_mult(radial, p->mode.A.radialAccel);

				  // tangential acceleration
				  float newy = tangential.x;
				  tangential.x = -tangential.y;
				  tangential.y = newy;
				  tangential = g2d_mult(tangential, p->mode.A.tangentialAccel);

				  // (gravity + radial + tangential) * dt
				  tmp = g2d_add(g2d_add(radial, tangential), m_mode.A.gravity);
				  tmp = g2d_mult(tmp, dt);
				  p->mode.A.dir = g2d_add(p->mode.A.dir, tmp);
				  tmp = g2d_mult(p->mode.A.dir, dt);
				  p->pos = g2d_add(p->pos, tmp);
			  }

				  // Mode B: radius movement
			  else {
				  // Update the angle and radius of the particle.
				  p->mode.B.angle += p->mode.B.degreesPerSecond * dt;
				  p->mode.B.radius += p->mode.B.deltaRadius * dt;

				  p->pos.x = -cosf(p->mode.B.angle) * p->mode.B.radius;
				  p->pos.y = -sinf(p->mode.B.angle) * p->mode.B.radius;
			  }

			  // color
			  p->color.r += (p->deltaColor.r * dt);
			  p->color.g += (p->deltaColor.g * dt);
			  p->color.b += (p->deltaColor.b * dt);
			  p->color.a += (p->deltaColor.a * dt);

			  // size
			  p->size += (p->deltaSize * dt);
			  p->size = std::max(0.0f, p->size);

			  // angle
			  p->rotation += (p->deltaRotation * dt);

			  //
			  // update values in quad
			  //

			  Point newPos;

			  if (m_positionType == kCCPositionTypeFree || m_positionType == kCCPositionTypeRelative) {
				  Point diff = g2d_sub(currentPosition, p->startPos);
				  newPos = g2d_sub(p->pos, diff);

			  } else
				  newPos = p->pos;

			  updateQuadWithParticle(p, newPos);

			  // update particle counter
			  m_particleIdx++;

		  } else {
			  // life < 0
			  if (m_particleIdx != m_particleCount - 1)
				  m_particles[m_particleIdx] = m_particles[m_particleCount - 1];
			  m_particleCount--;

			  if (m_particleCount == 0 && m_autoRemoveOnFinish) {
				  unscheduleUpdate();
				  removeFromParentAndCleanup(true);
				  return;
			  }
		  }
	  }

#ifdef CC_USES_VBO
	  postStep();
#endif
	  Base::update(dt);
  }

  void ParticleSystem::setTexture(PassRefPtr<Aphid::Texture2D> texture)
  {
	  m_texture = texture;
	  if (m_texture && !m_texture->hasPremultipliedAlpha() &&
		  (m_blendFunc.src == CC_BLEND_SRC && m_blendFunc.dst == CC_BLEND_DST)) {
		  m_blendFunc.src = GL_SRC_ALPHA;
		  m_blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
	  }
  }

  void ParticleSystem::setBlendAdditive(bool additive)
  {
	  if (additive) {
		  m_blendFunc.src = GL_SRC_ALPHA;
		  m_blendFunc.dst = GL_ONE;

	  } else {

		  if (m_texture && !m_texture->hasPremultipliedAlpha()) {
			  m_blendFunc.src = GL_SRC_ALPHA;
			  m_blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
		  } else {
			  m_blendFunc.src = CC_BLEND_SRC;
			  m_blendFunc.dst = CC_BLEND_DST;
		  }
	  }
  }

  bool ParticleSystem::blendAdditive() const
  {
	  return (m_blendFunc.src == GL_SRC_ALPHA && m_blendFunc.dst == GL_ONE);
  }

  void ParticleSystem::setTangentialAccel(float t)
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  m_mode.A.tangentialAccel = t;
  }

  float ParticleSystem::tangentialAccel() const
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  return m_mode.A.tangentialAccel;
	  return 0;
  }

  void ParticleSystem::setTangentialAccelVar(float t)
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  m_mode.A.tangentialAccelVar = t;
  }

  float ParticleSystem::tangentialAccelVar() const
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  return m_mode.A.tangentialAccelVar;
	  return 0;
  }

  void ParticleSystem::setRadialAccel(float t)
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  m_mode.A.radialAccel = t;
  }

  float ParticleSystem::radialAccel() const
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  return m_mode.A.radialAccel;
	  return 0;
  }

  void ParticleSystem::setRadialAccelVar(float t)
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  m_mode.A.radialAccelVar = t;
  }

  float ParticleSystem::radialAccelVar() const
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  return m_mode.A.radialAccelVar;
	  return 0;
  }

  void ParticleSystem::setGravity(const Point& g)
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  m_mode.A.gravity = g;
  }

  Point ParticleSystem::gravity() const
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  return m_mode.A.gravity;
	  return PointZero;
  }

  void ParticleSystem::setSpeed(float speed)
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  m_mode.A.speed = speed;
  }

  float ParticleSystem::speed() const
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  return m_mode.A.speed;
	  return 0;
  }

  void ParticleSystem::setSpeedVar(float speedVar)
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  m_mode.A.speedVar = speedVar;
  }

  float ParticleSystem::speedVar() const
  {
	  if (m_emitterMode == kCCParticleModeGravity)
		  return m_mode.A.speedVar;
	  return 0;
  }

#pragma mark ParticleSystem - Properties of Radius Mode

  void ParticleSystem::setStartRadius(float startRadius)
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  m_mode.B.startRadius = startRadius;
  }

  float ParticleSystem::startRadius() const
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  return m_mode.B.startRadius;
	  return 0;
  }

  void ParticleSystem::setStartRadiusVar(float startRadiusVar)
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  m_mode.B.startRadiusVar = startRadiusVar;
  }

  float ParticleSystem::startRadiusVar() const
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  return m_mode.B.startRadiusVar;
	  return 0;
  }

  void ParticleSystem::setEndRadius(float endRadius)
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  m_mode.B.endRadius = endRadius;
  }

  float ParticleSystem::endRadius() const
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  return m_mode.B.endRadius;
	  return 0;
  }

  void ParticleSystem::setEndRadiusVar(float endRadiusVar)
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  m_mode.B.endRadiusVar = endRadiusVar;
  }

  float ParticleSystem::endRadiusVar() const
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  return m_mode.B.endRadiusVar;
	  return 0;
  }

  void ParticleSystem::setRotatePerSecond(float degrees)
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  m_mode.B.rotatePerSecond = degrees;
  }

  float ParticleSystem::rotatePerSecond() const
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  return m_mode.B.rotatePerSecond;
	  return 0;
  }

  void ParticleSystem::setRotatePerSecondVar(float degrees)
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  m_mode.B.rotatePerSecondVar = degrees;
  }

  float ParticleSystem::rotatePerSecondVar() const
  {
	  if (m_emitterMode == kCCParticleModeRadius)
		  return m_mode.B.rotatePerSecondVar;
	  return 0;
  }

  ///-------------------------------------------------------------------------------------------------------------------
  /// ParticleSystemPoint
  ParticleSystemPoint::ParticleSystemPoint()
  : m_vertices(0)
  , m_verticesID(0)
  {
	  LEAK_DETECT_INC("ParticleSystemPoint");
  }

  ParticleSystemPoint::~ParticleSystemPoint()
  {
	  LEAK_DETECT_DEC("ParticleSystemPoint");
	  clearResources();
  }

  void ParticleSystemPoint::clearResources()
  {
	  Base::clearResources();
	  OA_FREE(m_vertices);
#if CC_USES_VBO
		glDeleteBuffers(1, &m_verticesID);
#endif
  }

  void ParticleSystemPoint::config(const Aphid::ParticleConfig &config)
  {
	  Base::config(config);

	  if (m_active) {
		  m_vertices = (ccPointSprite *) malloc(sizeof(ccPointSprite) * m_totalParticles);
		  RELEASE_ASSERT(m_vertices);
#if CC_USES_VBO
			glGenBuffers(1, &m_verticesID);
			
			// initial binding
			glBindBuffer(GL_ARRAY_BUFFER, m_verticesID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(ccPointSprite)*m_totalParticles, m_vertices, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
	  }
  }

  void ParticleSystemPoint::updateQuadWithParticle(tCCParticle *p, const Point &newPos)
  {
	  m_vertices[m_particleIdx].pos = (ccVertex2F) {newPos.x, newPos.y};
	  m_vertices[m_particleIdx].size = p->size;
	  ccColor4B color = {p->color.r * 255, p->color.g * 255, p->color.b * 255, p->color.a * 255};
	  m_vertices[m_particleIdx].color = color;
  }

  void ParticleSystemPoint::postStep()
  {
#if CC_USES_VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_verticesID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ccPointSprite)*m_particleCount, m_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
  }

  void ParticleSystemPoint::draw()
  {
	  Base::draw();

	  if (m_particleIdx == 0 || !m_texture)
		  return;

	  // Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY
	  // Unneeded states: GL_TEXTURE_COORD_ARRAY
	  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	  glBindTexture(GL_TEXTURE_2D, m_texture->name());

	  glEnable(GL_POINT_SPRITE_OES);
	  glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);

#define kPointSize sizeof(m_vertices[0])

#if CC_USES_VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_verticesID);
		
		glVertexPointer(2,GL_FLOAT, kPointSize, 0);
		
		glColorPointer(4, GL_UNSIGNED_BYTE, kPointSize, (GLvoid*) offsetof(ccPointSprite, color) );
		
		glEnableClientState(GL_POINT_SIZE_ARRAY_OES);
		glPointSizePointerOES(GL_FLOAT, kPointSize, (GLvoid*) offsetof(ccPointSprite, size) );
#else // Uses Vertex Array List
	  int offset = (int) vertices;
	  glVertexPointer(2, GL_FLOAT, kPointSize, (GLvoid *) offset);

	  int diff = offsetof(ccPointSprite, color);
	  glColorPointer(4, GL_UNSIGNED_BYTE, kPointSize, (GLvoid *) (offset + diff));

	  glEnableClientState(GL_POINT_SIZE_ARRAY_OES);
	  diff = offsetof(ccPointSprite, size);
	  glPointSizePointerOES(GL_FLOAT, kPointSize, (GLvoid *) (offset + diff));
#endif

	  bool newBlend = m_blendFunc.src != CC_BLEND_SRC || m_blendFunc.dst != CC_BLEND_DST;
	  if (newBlend)
		  glBlendFunc(m_blendFunc.src, m_blendFunc.dst);


	  glDrawArrays(GL_POINTS, 0, m_particleIdx);

	  // restore blend state
	  if (newBlend)
		  glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);


#if CC_USES_VBO
		// unbind VBO buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

	  glDisableClientState(GL_POINT_SIZE_ARRAY_OES);
	  glDisable(GL_POINT_SPRITE_OES);

	  // restore GL default state
	  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  ///-------------------------------------------------------------------------------------------------------------------
  ParticleSystemQuad::ParticleSystemQuad()
  : m_indices(0)
  , m_quads(0)
  , m_quadsID(0)
  {
	  LEAK_DETECT_INC("ParticleSystemQuad");
  }

  ParticleSystemQuad::~ParticleSystemQuad()
  {
	  LEAK_DETECT_DEC("ParticleSystemQuad");
  }

  void ParticleSystemQuad::clearResources()
  {
	  Base::clearResources();
	  OA_FREE(m_quads);
	  OA_FREE(m_indices);
#if CC_USES_VBO
		glDeleteBuffers(1, &m_quadsID);
#endif
  }

  void ParticleSystemQuad::config(const Aphid::ParticleConfig &config)
  {
	  Base::config(config);

	  if (m_active) {
		  m_quads = (ccV2F_C4B_T2F_Quad *) calloc(sizeof(m_quads[0]) * m_totalParticles, 1);
		  m_indices = (GLushort *) calloc(sizeof(m_indices[0]) * m_totalParticles * 6, 1);

		  RELEASE_ASSERT(m_quads && m_indices);

		  initTexCoordsWithRect(RectMake(0, 0, m_texture ? m_texture->pixelWidth() : 0, m_texture ? m_texture->pixelHeight() : 0));

		  initIndices();

#if CC_USES_VBO
			// create the VBO buffer
			glGenBuffers(1, &m_quadsID);
			
			// initial binding
			glBindBuffer(GL_ARRAY_BUFFER, m_quadsID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_quads[0])*m_totalParticles, m_quads,GL_DYNAMIC_DRAW);	
			glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif

	  }
  }

  void ParticleSystemQuad::initTexCoordsWithRect(const Rect& pointRect)
  {
	  Rect rect = RectMake(
	  pointRect.origin.x * G2D_CONTENT_SCALE_FACTOR(),
	  pointRect.origin.y * G2D_CONTENT_SCALE_FACTOR(),
	  pointRect.size.width * G2D_CONTENT_SCALE_FACTOR(),
	  pointRect.size.height * G2D_CONTENT_SCALE_FACTOR() );

	  GLfloat wide = m_texture ? m_texture->pixelWidth() : 0;
	  GLfloat high = m_texture ? m_texture->pixelHeight() : 0;

#if CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL
		GLfloat left = (rect.origin.x*2+1) / (wide*2);
		GLfloat bottom = (rect.origin.y*2+1) / (high*2);
		GLfloat right = left + (rect.size.width*2-2) / (wide*2);
		GLfloat top = bottom + (rect.size.height*2-2) / (high*2);
#else
	  GLfloat left = rect.origin.x / wide;
	  GLfloat bottom = rect.origin.y / high;
	  GLfloat right = left + rect.size.width / wide;
	  GLfloat top = bottom + rect.size.height / high;
#endif // ! CC_FIX_ARTIFACTS_BY_STRECHING_TEXEL

	  // Important. Texture in cocos2d are inverted, so the Y component should be inverted
	  CC_SWAP( top, bottom);

	  for (size_t i = 0; i < m_totalParticles; i++) {
		  // bottom-left vertex:
		  m_quads[i].bl.texCoords.u = left;
		  m_quads[i].bl.texCoords.v = bottom;
		  // bottom-right vertex:
		  m_quads[i].br.texCoords.u = right;
		  m_quads[i].br.texCoords.v = bottom;
		  // top-left vertex:
		  m_quads[i].tl.texCoords.u = left;
		  m_quads[i].tl.texCoords.v = top;
		  // top-right vertex:
		  m_quads[i].tr.texCoords.u = right;
		  m_quads[i].tr.texCoords.v = top;
	  }
  }

  void ParticleSystemQuad::initIndices()
  {
	  for (size_t i = 0; i < m_totalParticles; i++) {
		  const size_t i6 = i * 6;
		  const size_t i4 = i * 4;
		  m_indices[i6 + 0] = (GLushort) i4 + 0;
		  m_indices[i6 + 1] = (GLushort) i4 + 1;
		  m_indices[i6 + 2] = (GLushort) i4 + 2;

		  m_indices[i6 + 5] = (GLushort) i4 + 1;
		  m_indices[i6 + 4] = (GLushort) i4 + 2;
		  m_indices[i6 + 3] = (GLushort) i4 + 3;
	  }
  }

  void ParticleSystemQuad::updateQuadWithParticle(tCCParticle *p, const Point &newPos)
  {
	  ccV2F_C4B_T2F_Quad *quad = &(m_quads[m_particleIdx]);

	  ccColor4B color = {p->color.r * 255, p->color.g * 255, p->color.b * 255, p->color.a * 255};
	  quad->bl.colors = color;
	  quad->br.colors = color;
	  quad->tl.colors = color;
	  quad->tr.colors = color;

	  // vertices
	  GLfloat size_2 = p->size / 2;
	  if (p->rotation) {
		  GLfloat x1 = -size_2;
		  GLfloat y1 = -size_2;

		  GLfloat x2 = size_2;
		  GLfloat y2 = size_2;
		  GLfloat x = newPos.x;
		  GLfloat y = newPos.y;

		  GLfloat r = (GLfloat) -CC_DEGREES_TO_RADIANS(p->rotation);
		  GLfloat cr = cosf(r);
		  GLfloat sr = sinf(r);
		  GLfloat ax = x1 * cr - y1 * sr + x;
		  GLfloat ay = x1 * sr + y1 * cr + y;
		  GLfloat bx = x2 * cr - y1 * sr + x;
		  GLfloat by = x2 * sr + y1 * cr + y;
		  GLfloat cx = x2 * cr - y2 * sr + x;
		  GLfloat cy = x2 * sr + y2 * cr + y;
		  GLfloat dx = x1 * cr - y2 * sr + x;
		  GLfloat dy = x1 * sr + y2 * cr + y;

		  // bottom-left
		  quad->bl.vertices.x = ax;
		  quad->bl.vertices.y = ay;

		  // bottom-right vertex:
		  quad->br.vertices.x = bx;
		  quad->br.vertices.y = by;

		  // top-left vertex:
		  quad->tl.vertices.x = dx;
		  quad->tl.vertices.y = dy;

		  // top-right vertex:
		  quad->tr.vertices.x = cx;
		  quad->tr.vertices.y = cy;
	  } else {
		  // bottom-left vertex:
		  quad->bl.vertices.x = newPos.x - size_2;
		  quad->bl.vertices.y = newPos.y - size_2;

		  // bottom-right vertex:
		  quad->br.vertices.x = newPos.x + size_2;
		  quad->br.vertices.y = newPos.y - size_2;

		  // top-left vertex:
		  quad->tl.vertices.x = newPos.x - size_2;
		  quad->tl.vertices.y = newPos.y + size_2;

		  // top-right vertex:
		  quad->tr.vertices.x = newPos.x + size_2;
		  quad->tr.vertices.y = newPos.y + size_2;
	  }
  }

  void ParticleSystemQuad::postStep()
  {
#if CC_USES_VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_quadsID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_quads[0])*m_particleCount, m_quads);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
  }

  void ParticleSystemQuad::draw()
  {
	  Base::draw();
	  if (!m_texture)
		  return;
	  // Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Needed states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	  // Unneeded states: -

	  glBindTexture(GL_TEXTURE_2D, m_texture->name());

#define kQuadSize sizeof(m_quads[0].bl)

#if CC_USES_VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_quadsID);
		
		glVertexPointer(2,GL_FLOAT, kQuadSize, 0);
		
		glColorPointer(4, GL_UNSIGNED_BYTE, kQuadSize, (GLvoid*) offsetof(ccV2F_C4B_T2F,colors) );
		
		glTexCoordPointer(2, GL_FLOAT, kQuadSize, (GLvoid*) offsetof(ccV2F_C4B_T2F,texCoords) );
#else // vertex array list

	  unsigned offset = (unsigned) quads_;

	  // vertex
	  unsigned diff = offsetof( ccV2F_C4B_T2F, vertices);
	  glVertexPointer(2, GL_FLOAT, kQuadSize, (GLvoid *) (offset + diff));

	  // color
	  diff = offsetof( ccV2F_C4B_T2F, colors);
	  glColorPointer(4, GL_UNSIGNED_BYTE, kQuadSize, (GLvoid *) (offset + diff));

	  // tex coords
	  diff = offsetof( ccV2F_C4B_T2F, texCoords);
	  glTexCoordPointer(2, GL_FLOAT, kQuadSize, (GLvoid *) (offset + diff));

#endif // ! CC_USES_VBO

	  bool newBlend = m_blendFunc.src != CC_BLEND_SRC || m_blendFunc.dst != CC_BLEND_DST;
	  if (newBlend)
		  glBlendFunc(m_blendFunc.src, m_blendFunc.dst);

	  ASSERT(m_particleIdx == m_particleCount);
	  glDrawElements(GL_TRIANGLES, (GLsizei) m_particleIdx * 6, GL_UNSIGNED_SHORT, m_indices);

	  // restore blend state
	  if (newBlend)
		  glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

#if CC_USES_VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
  }

  void ParticleSystemQuad::setTexture(PassRefPtr<Aphid::Texture2D> texture)
  {
	  Size s = texture ? texture->contentSize() : SizeZero;
	  setTexture(texture, RectMake(0, 0, s.width, s.height));
  }

  void ParticleSystemQuad::setTexture(PassRefPtr<Texture2D> texture, const Rect& rect)
  {
	  Base::setTexture(texture);
	  initTexCoordsWithRect(rect);
  }
}