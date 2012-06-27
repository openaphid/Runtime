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

public class AphidTouchQueue {
	private AphidTouch head, tail;

	public synchronized void queueTouch(AphidTouch touch) {
		if (tail != null) {
			tail.next = touch;
			tail = touch;
			touch.next = null;
		} else {
			head = tail = touch;
		}
	}

	public synchronized void processAllTouches(AphidRenderer renderer) {
		AphidTouch t = head;
		while (t != null) {
			renderer.nativeOnSingleTouch(t.eventHash, t.eventTime, t.eventPhase, t);
			AphidTouch next = t.next;
			t.recycle();
			t = next;
		}

		head = null;
		tail = null;
	}
}
