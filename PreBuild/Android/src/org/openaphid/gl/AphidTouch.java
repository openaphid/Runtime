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

package org.openaphid.gl;

import android.view.MotionEvent;

public class AphidTouch {
	public int identifier;
	public float screenX, screenY;
	public float clientX, clientY;
	public long timestamp;

	public int eventHash;
	public long eventTime;
	public int eventPhase;

	public AphidTouch next;

	private static final Object pool_lock = new Object();
	private static AphidTouch pooled_touch;
	private static int pool_size = 0;
	private static final int MAX_POOL_SIZE = 10;

	private AphidTouch() {
	}

	public static AphidTouch create(int[] viewLocation, MotionEvent event, int eventHash, long eventTime, int phase,
			int index) {
		AphidTouch ret = null;

		synchronized (pool_lock) {
			ret = pooled_touch;
			if (pooled_touch != null) {
				pooled_touch = pooled_touch.next;
				pool_size--;
			}
		}

		if (ret == null)
			ret = new AphidTouch();

		ret.next = null;

		ret.eventHash = eventHash;
		ret.eventTime = eventTime;
		ret.eventPhase = phase;

		ret.identifier = event.getPointerId(index) + 1; //+ 1 to avoid zero identifier

		ret.clientX = event.getX(index);
		ret.clientY = event.getY(index);
		ret.screenX = ret.clientX + viewLocation[0];
		ret.screenY = ret.clientY + viewLocation[1];
		ret.timestamp = event.getEventTime();

		return ret;
	}

	public void recycle() {
		synchronized (pool_lock) {
			if (pool_size < MAX_POOL_SIZE) {
				next = pooled_touch;
				pooled_touch = this;
				pool_size++;
			} else
				next = null;
		}
	}

	public int getIdentifier() {
		return identifier;
	}

	public float getScreenX() {
		return screenX;
	}

	public float getScreenY() {
		return screenY;
	}

	public float getClientX() {
		return clientX;
	}

	public float getClientY() {
		return clientY;
	}

	public long getTimestamp() {
		return timestamp;
	}

	public int getEventHash() {
		return eventHash;
	}

	public long getEventTime() {
		return eventTime;
	}

	public int getEventPhase() {
		return eventPhase;
	}
}
