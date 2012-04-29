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
#include "ActionTiledGrid.h"
#include "ccTypes.h"
#include "G2DMacros.h"
#import "Node.h"

namespace Aphid {
  static void shuffle(int *array, size_t len)
  {
	  for (int i = len - 1; i >= 0; i--) {
		  int j = rand() % (i + 1);
		  int v = array[i];
		  array[i] = array[j];
		  array[j] = v;
	  }
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// ShakyTiles3DAction
  ShakyTiles3DAction::ShakyTiles3DAction(int range, bool shakeZ, const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  , m_randRange(range)
  , m_shakeZ(shakeZ)
  {
	  LEAK_DETECT_INC("ShakyTiles3DAction");
  }
	
  ShakyTiles3DAction::~ShakyTiles3DAction()
  {
	  LEAK_DETECT_DEC("ShakyTiles3DAction");
  }
	
  void ShakyTiles3DAction::update(TimeSec time)
  {
		UNUSED_PARAM(time);
		
	  int i, j;
		
	  for (i = 0; i < m_gridSize.x; i++) {
		  for (j = 0; j < m_gridSize.y; j++) {
			  ccQuad3 coords = originalTile(ccg(i, j));
				
			  // X
			  coords.bl.x += (rand() % (m_randRange * 2)) - m_randRange;
			  coords.br.x += (rand() % (m_randRange * 2)) - m_randRange;
			  coords.tl.x += (rand() % (m_randRange * 2)) - m_randRange;
			  coords.tr.x += (rand() % (m_randRange * 2)) - m_randRange;
				
			  // Y
			  coords.bl.y += (rand() % (m_randRange * 2)) - m_randRange;
			  coords.br.y += (rand() % (m_randRange * 2)) - m_randRange;
			  coords.tl.y += (rand() % (m_randRange * 2)) - m_randRange;
			  coords.tr.y += (rand() % (m_randRange * 2)) - m_randRange;
				
			  if (m_shakeZ) {
				  coords.bl.z += (rand() % (m_randRange * 2)) - m_randRange;
				  coords.br.z += (rand() % (m_randRange * 2)) - m_randRange;
				  coords.tl.z += (rand() % (m_randRange * 2)) - m_randRange;
				  coords.tr.z += (rand() % (m_randRange * 2)) - m_randRange;
			  }
				
			  setTile(ccg(i, j), coords);
		  }
	  }
  }

  ShakyTiles3DAction* ShakyTiles3DAction::doCreateClone()
  {
  	return new ShakyTiles3DAction(m_randRange, m_shakeZ, m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// ShatteredTiles3DAction
  ShatteredTiles3DAction::ShatteredTiles3DAction(int range, bool shatterZ, const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  , m_randRange(range)
  , m_shatterZ(shatterZ)
  , m_once(false)
  {
	  LEAK_DETECT_INC("ShatteredTiles3DAction");
  }
	
  ShatteredTiles3DAction::~ShatteredTiles3DAction()
  {
	  LEAK_DETECT_DEC("ShatteredTiles3DAction");
  }
	
  void ShatteredTiles3DAction::update(TimeSec time)
  {
		UNUSED_PARAM(time);
		
	  int i, j;
		
	  if (!m_once) {
		  for (i = 0; i < m_gridSize.x; i++) {
			  for (j = 0; j < m_gridSize.y; j++) {
				  ccQuad3 coords = originalTile(ccg(i, j));
					
				  // X
				  coords.bl.x += (rand() % (m_randRange * 2)) - m_randRange;
				  coords.br.x += (rand() % (m_randRange * 2)) - m_randRange;
				  coords.tl.x += (rand() % (m_randRange * 2)) - m_randRange;
				  coords.tr.x += (rand() % (m_randRange * 2)) - m_randRange;
					
				  // Y
				  coords.bl.y += (rand() % (m_randRange * 2)) - m_randRange;
				  coords.br.y += (rand() % (m_randRange * 2)) - m_randRange;
				  coords.tl.y += (rand() % (m_randRange * 2)) - m_randRange;
				  coords.tr.y += (rand() % (m_randRange * 2)) - m_randRange;
					
				  if (m_shatterZ) {
					  coords.bl.z += (rand() % (m_randRange * 2)) - m_randRange;
					  coords.br.z += (rand() % (m_randRange * 2)) - m_randRange;
					  coords.tl.z += (rand() % (m_randRange * 2)) - m_randRange;
					  coords.tr.z += (rand() % (m_randRange * 2)) - m_randRange;
				  }
					
				  setTile(ccg(i, j), coords);
			  }
		  }
			
		  m_once = true;
	  }
  }

  ShatteredTiles3DAction* ShatteredTiles3DAction::doCreateClone()
  {
  	return new ShatteredTiles3DAction(m_randRange, m_shatterZ, m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// ShuffleTilesAction
  ShuffleTilesAction::ShuffleTilesAction(int seed, const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  , m_seed(seed)
  , m_tilesOrder(0)
  , m_tiles(0)
  , m_tilesCount(0)
  {
	  LEAK_DETECT_INC("ShuffleTilesAction");
  }
	
  ShuffleTilesAction::~ShuffleTilesAction()
  {
	  LEAK_DETECT_DEC("ShuffleTilesAction");
	  OA_FREE(m_tilesOrder);
	  OA_FREE(m_tiles);
  }
	
  GridSize ShuffleTilesAction::getDelta(const GridSize& pos)
  {
	  Point pos2;
		
	  int idx = pos.x * m_gridSize.y + pos.y;
		
	  pos2.x = m_tilesOrder[idx] / m_gridSize.y;
	  pos2.y = m_tilesOrder[idx] % m_gridSize.y;
		
	  return ccg(pos2.x - pos.x, pos2.y - pos.y);
  }
	
  void ShuffleTilesAction::placeTile(const GridSize& pos, const Tile& t)
  {
	  ccQuad3 coords = originalTile(pos);
		
	  Point step = m_target->grid()->step();
	  coords.bl.x += (int) (t.position.x * step.x);
	  coords.bl.y += (int) (t.position.y * step.y);
		
	  coords.br.x += (int) (t.position.x * step.x);
	  coords.br.y += (int) (t.position.y * step.y);
		
	  coords.tl.x += (int) (t.position.x * step.x);
	  coords.tl.y += (int) (t.position.y * step.y);
		
	  coords.tr.x += (int) (t.position.x * step.x);
	  coords.tr.y += (int) (t.position.y * step.y);
		
	  setTile(pos, coords);
  }
	
  void ShuffleTilesAction::startWithTarget(Node *target)
  {
	  Base::startWithTarget(target);
	  if (m_seed != -1)
		  srand(m_seed);
		
	  m_tilesCount = m_gridSize.x * m_gridSize.y;
	  m_tilesOrder = (int *) malloc(m_tilesCount * sizeof(int));
	  int i, j;
		
	  for (i = 0; i < m_tilesCount; i++)
		  m_tilesOrder[i] = i;
		
	  shuffle(m_tilesOrder, m_tilesCount);
		
	  m_tiles = malloc(m_tilesCount * sizeof(Tile));
	  Tile *tileArray = (Tile *) m_tiles;
		
	  for (i = 0; i < m_gridSize.x; i++) {
		  for (j = 0; j < m_gridSize.y; j++) {
			  tileArray->position = PointMake(i, j);
			  tileArray->startPosition = PointMake(i, j);
			  tileArray->delta = getDelta(ccg(i, j));
			  tileArray++;
		  }
	  }
  }
	
  void ShuffleTilesAction::update(TimeSec time)
  {
	  int i, j;
		
	  Tile *tileArray = (Tile *) m_tiles;
		
	  for (i = 0; i < m_gridSize.x; i++) {
		  for (j = 0; j < m_gridSize.y; j++) {
			  tileArray->position = g2d_mult(PointMake(tileArray->delta.x, tileArray->delta.y), time);
			  placeTile(ccg(i, j), *tileArray);
			  tileArray++;
		  }
	  }
  }

  ShuffleTilesAction* ShuffleTilesAction::doCreateClone()
  {
  	return new ShuffleTilesAction(m_seed, m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FadeOutTilesBaseAction
  float FadeOutTilesBaseAction::testFunc(const GridSize& pos, TimeSec time)
  {
	  Point n = g2d_mult(PointMake(m_gridSize.x, m_gridSize.y), time);
	  if ((n.x + n.y) == 0.0f)
		  return 1.0f;
		
	  return powf((pos.x + pos.y) / (n.x + n.y), 6);
  }
	
  void FadeOutTilesBaseAction::transformTile(const GridSize& pos, float distance)
  {
	  ccQuad3 coords = originalTile(pos);
	  Point step = m_target->grid()->step();
		
	  coords.bl.x += (step.x / 2) * (1.0f - distance);
	  coords.bl.y += (step.y / 2) * (1.0f - distance);
		
	  coords.br.x -= (step.x / 2) * (1.0f - distance);
	  coords.br.y += (step.y / 2) * (1.0f - distance);
		
	  coords.tl.x += (step.x / 2) * (1.0f - distance);
	  coords.tl.y -= (step.y / 2) * (1.0f - distance);
		
	  coords.tr.x -= (step.x / 2) * (1.0f - distance);
	  coords.tr.y -= (step.y / 2) * (1.0f - distance);
		
	  setTile(pos, coords);
  }
	
  void FadeOutTilesBaseAction::update(TimeSec time)
  {
	  int i, j;
		
	  for (i = 0; i < m_gridSize.x; i++) {
		  for (j = 0; j < m_gridSize.y; j++) {
			  float distance = testFunc(ccg(i, j), time);
			  if (distance == 0)
				  turnOffTile(ccg(i, j));
			  else if (distance < 1)
				  transformTile(ccg(i, j), distance);
			  else
				  turnOnTile(ccg(i, j));
		  }
	  }
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FadeOutTRTilesAction
  FadeOutTRTilesAction::FadeOutTRTilesAction(const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  {
	  LEAK_DETECT_INC("FadeOutTRTilesAction");
  }
	
  FadeOutTRTilesAction::~FadeOutTRTilesAction()
  {
	  LEAK_DETECT_DEC("FadeOutTRTilesAction");
  }

  FadeOutTRTilesAction* FadeOutTRTilesAction::doCreateClone()
  {
  	return new FadeOutTRTilesAction(m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FadeOutBLTilesAction
  FadeOutBLTilesAction::FadeOutBLTilesAction(const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  {
	  LEAK_DETECT_INC("FadeOutBLTilesAction");
  }
	
  FadeOutBLTilesAction::~FadeOutBLTilesAction()
  {
	  LEAK_DETECT_DEC("FadeOutBLTilesAction");
  }
	
  float FadeOutBLTilesAction::testFunc(const GridSize& pos, TimeSec time)
  {
	  Point n = g2d_mult(PointMake(m_gridSize.x, m_gridSize.y), 1.0f - time);
	  if ((pos.x + pos.y) == 0.0f)
		  return 1.0f;
		
	  return powf((n.x + n.y) / (pos.x + pos.y), 6);
  }

  FadeOutBLTilesAction* FadeOutBLTilesAction::doCreateClone()
  {
  	return new FadeOutBLTilesAction(m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FadeOutUpTilesAction
  FadeOutUpTilesAction::FadeOutUpTilesAction(const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  {
	  LEAK_DETECT_INC("FadeOutUpTilesAction");
  }
	
  FadeOutUpTilesAction::~FadeOutUpTilesAction()
  {
	  LEAK_DETECT_DEC("FadeOutUpTilesAction");
  }
	
  float FadeOutUpTilesAction::testFunc(const GridSize& pos, TimeSec time)
  {
	  Point n = g2d_mult(PointMake(m_gridSize.x, m_gridSize.y), time);
	  if (n.y == 0.0f)
		  return 1.0f;
		
	  return powf(pos.y / n.y, 6);
  }
	
  void FadeOutUpTilesAction::transformTile(const GridSize& pos, float distance)
  {
	  ccQuad3 coords = originalTile(pos);
	  Point step = m_target->grid()->step();
		
	  coords.bl.y += (step.y / 2) * (1.0f - distance);
	  coords.br.y += (step.y / 2) * (1.0f - distance);
	  coords.tl.y -= (step.y / 2) * (1.0f - distance);
	  coords.tr.y -= (step.y / 2) * (1.0f - distance);
		
	  setTile(pos, coords);
  }

  FadeOutUpTilesAction* FadeOutUpTilesAction::doCreateClone()
  {
  	return new FadeOutUpTilesAction(m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// FadeOutDownTilesAction
  FadeOutDownTilesAction::FadeOutDownTilesAction(const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  {
	  LEAK_DETECT_INC("FadeOutDownTilesAction");
  }
	
  FadeOutDownTilesAction::~FadeOutDownTilesAction()
  {
	  LEAK_DETECT_DEC("FadeOutDownTilesAction");
  }
	
  float FadeOutDownTilesAction::testFunc(const GridSize& pos, TimeSec time)
  {
	  Point n = g2d_mult(PointMake(m_gridSize.x, m_gridSize.y), 1.0f - time);
	  if (pos.y == 0)
		  return 1.0f;
		
	  return powf(n.y / pos.y, 6);
  }

  FadeOutDownTilesAction* FadeOutDownTilesAction::doCreateClone()
  {
  	return new FadeOutDownTilesAction(m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// TurnOffTilesAction
  TurnOffTilesAction::TurnOffTilesAction(int seed, const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  , m_seed(seed)
  , m_tilesOrder(0)
  {
	  LEAK_DETECT_INC("TurnOffTilesAction");
  }
	
  TurnOffTilesAction::~TurnOffTilesAction()
  {
	  LEAK_DETECT_DEC("TurnOffTilesAction");
	  OA_FREE(m_tilesOrder);
  }
	
  void TurnOffTilesAction::startWithTarget(Node *target)
  {
	  int i;
		
	  Base::startWithTarget(target);
		
	  if (m_seed != -1)
		  srand(m_seed);
		
	  m_tilesCount = m_gridSize.x * m_gridSize.y;
	  m_tilesOrder = (int *) malloc(m_tilesCount * sizeof(int));
		
	  for (i = 0; i < m_tilesCount; i++)
		  m_tilesOrder[i] = i;
		
	  shuffle(m_tilesOrder, m_tilesCount);
  }
	
  void TurnOffTilesAction::update(TimeSec time)
  {
	  int i, l, t;
		
	  l = (int) (time * (float) m_tilesCount);
		
	  for (i = 0; i < m_tilesCount; i++) {
		  t = m_tilesOrder[i];
		  GridSize tilePos = ccg(t / m_gridSize.y, t % m_gridSize.y);
			
		  if (i < l)
			  turnOffTile(tilePos);
		  else
			  turnOnTile(tilePos);
	  }
  }

  TurnOffTilesAction* TurnOffTilesAction::doCreateClone()
  {
  	return new TurnOffTilesAction(m_seed, m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// WavesTiles3DAction
  WavesTiles3DAction::WavesTiles3DAction(int waves, float amplitude, const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  , m_waves(waves)
  , m_amplitude(amplitude)
  {
	  LEAK_DETECT_INC("WavesTiles3DAction");
  }
	
  WavesTiles3DAction::~WavesTiles3DAction()
  {
	  LEAK_DETECT_DEC("WavesTiles3DAction");
  }
	
  void WavesTiles3DAction::update(TimeSec time)
  {
	  int i, j;
		
	  for (i = 0; i < m_gridSize.x; i++) {
		  for (j = 0; j < m_gridSize.y; j++) {
			  ccQuad3 coords = originalTile(ccg(i, j));
				
			  coords.bl.z = (sinf(time * (float) M_PI * m_waves * 2 + (coords.bl.y + coords.bl.x) * .01f) * m_amplitude * m_amplitudeRate);
			  coords.br.z = coords.bl.z;
			  coords.tl.z = coords.bl.z;
			  coords.tr.z = coords.bl.z;
				
			  setTile(ccg(i, j), coords);
		  }
	  }
  }

  WavesTiles3DAction* WavesTiles3DAction::doCreateClone()
  {
  	return new WavesTiles3DAction(m_waves, m_amplitude, m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// JumpTiles3DAction
  JumpTiles3DAction::JumpTiles3DAction(int jumps, float amplitude, const GridSize& gridSize, TimeSec duration)
  : Base(gridSize, duration)
  , m_jumps(jumps)
  , m_amplitude(amplitude)
  {
	  LEAK_DETECT_INC("JumpTiles3DAction");
  }
	
  JumpTiles3DAction::~JumpTiles3DAction()
  {
	  LEAK_DETECT_DEC("JumpTiles3DAction");
  }
	
  void JumpTiles3DAction::update(TimeSec time)
  {
	  int i, j;
		
	  float sinz = (sinf((float) M_PI * time * m_jumps * 2) * m_amplitude * m_amplitudeRate);
	  float sinz2 = (sinf((float) M_PI * (time * m_jumps * 2 + 1)) * m_amplitude * m_amplitudeRate);
		
	  for (i = 0; i < m_gridSize.x; i++) {
		  for (j = 0; j < m_gridSize.y; j++) {
			  ccQuad3 coords = originalTile(ccg(i, j));
				
			  if (((i + j) % 2) == 0) {
				  coords.bl.z += sinz;
				  coords.br.z += sinz;
				  coords.tl.z += sinz;
				  coords.tr.z += sinz;
			  }
			  else {
				  coords.bl.z += sinz2;
				  coords.br.z += sinz2;
				  coords.tl.z += sinz2;
				  coords.tr.z += sinz2;
			  }
				
			  setTile(ccg(i, j), coords);
		  }
	  }
  }

  JumpTiles3DAction* JumpTiles3DAction::doCreateClone()
  {
  	return new JumpTiles3DAction(m_jumps, m_amplitude, m_gridSize, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// SplitRowsAction
  SplitRowsAction::SplitRowsAction(int rows, TimeSec duration)
  : Base(ccg(1, rows), duration)
  , m_rows(rows)
  {
	  LEAK_DETECT_INC("SplitRowsAction");
  }
	
  SplitRowsAction::~SplitRowsAction()
  {
	  LEAK_DETECT_DEC("SplitRowsAction");
  }
	
  void SplitRowsAction::startWithTarget(Node *target)
  {
	  Base::startWithTarget(target);
	  m_winSize = Director::sharedDirector()->winSizeInPixels();
  }
	
  void SplitRowsAction::update(TimeSec time)
  {
	  int j;
		
	  for (j = 0; j < m_gridSize.y; j++) {
		  ccQuad3 coords = originalTile(ccg(0, j));
		  float direction = 1;
			
		  if ((j % 2) == 0)
			  direction = -1;
			
		  coords.bl.x += direction * m_winSize.width * time;
		  coords.br.x += direction * m_winSize.width * time;
		  coords.tl.x += direction * m_winSize.width * time;
		  coords.tr.x += direction * m_winSize.width * time;
			
		  setTile(ccg(0, j), coords);
	  }
  }

  SplitRowsAction* SplitRowsAction::doCreateClone()
  {
  	return new SplitRowsAction(m_rows, m_duration);
  }
	
  ///-------------------------------------------------------------------------------------------------------------------
  /// SplitColumnsAction
  SplitColumnsAction::SplitColumnsAction(int cols, TimeSec duration)
  : Base(ccg(cols, 1), duration)
  , m_cols(cols)
  {
	  LEAK_DETECT_INC("SplitColumnsAction");
  }
	
  SplitColumnsAction::~SplitColumnsAction()
  {
	  LEAK_DETECT_DEC("SplitColumnsAction");
  }
	
  void SplitColumnsAction::startWithTarget(Node *target)
  {
	  Base::startWithTarget(target);
	  m_winSize = Director::sharedDirector()->winSizeInPixels();
  }
	
  void SplitColumnsAction::update(TimeSec time)
  {
	  int i;
		
	  for (i = 0; i < m_gridSize.x; i++) {
		  ccQuad3 coords = originalTile(ccg(i, 0));
		  float direction = 1;
			
		  if ((i % 2) == 0)
			  direction = -1;
			
		  coords.bl.y += direction * m_winSize.height * time;
		  coords.br.y += direction * m_winSize.height * time;
		  coords.tl.y += direction * m_winSize.height * time;
		  coords.tr.y += direction * m_winSize.height * time;
			
		  setTile(ccg(i, 0), coords);
	  }
  }

  SplitColumnsAction* SplitColumnsAction::doCreateClone()
  {
  	return new SplitColumnsAction(m_cols, m_duration);
  }
}