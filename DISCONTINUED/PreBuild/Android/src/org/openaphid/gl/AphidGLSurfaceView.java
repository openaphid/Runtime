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

import java.util.concurrent.Callable;

import org.openaphid.internal.AppDelegate;
import org.openaphid.internal.utils.AphidLog;
import org.openaphid.internal.utils.UI;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

//TODO: enhance it to reload texture only when the gl context is invalid
public class AphidGLSurfaceView extends GLSurfaceView {

	public static final int EventFlagTouchStart = 1 << 1;
	public static final int EventFlagTouchMove = 1 << 2;
	public static final int EventFlagTouchEnd = 1 << 3;
	public static final int EventFlagTouchCancel = 1 << 4;

	private AphidRenderer renderer = null;

	private int[] location = { 0, 0 };
	
	private int previousSingleTouchID = -1;

	public AphidGLSurfaceView(Context context) {
		super(context);

		//setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR);

		//setEGLConfigChooser(8, 8, 8, 8, 16, 0);
		//setZOrderOnTop(true);
		//getHolder().setFormat(PixelFormat.TRANSLUCENT);
		setEGLConfigChooser(false);
		renderer = new AphidRenderer();
		setRenderer(renderer);

		setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
	}

	public AphidRenderer getAphidRenderer() {
		return renderer;
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event) {
		if (AppDelegate.isMultitouchEnabled()) {
			if (handleMultiTouchEvent(event))
				return true;
		} else {
			if (handleSingleTouchEvent(event))
				return true;
		}
		return super.onTouchEvent(event);
	}

	private boolean handleSingleTouchEvent(MotionEvent event) {
		final int phase;
		int index = 0;
		switch (event.getActionMasked()) {
			case MotionEvent.ACTION_DOWN:
				phase = EventFlagTouchStart;
				previousSingleTouchID = event.getPointerId(0);
				index = 0;
				break;
			case MotionEvent.ACTION_MOVE:
				phase = EventFlagTouchMove;
				index = event.findPointerIndex(previousSingleTouchID);
				break;
			case MotionEvent.ACTION_UP:
				phase = EventFlagTouchEnd;
				index = event.findPointerIndex(previousSingleTouchID);
				previousSingleTouchID = -1;
				break;
			case MotionEvent.ACTION_CANCEL:
				phase = EventFlagTouchCancel;
				index = event.findPointerIndex(previousSingleTouchID);
				previousSingleTouchID = -1;
				break;
			case MotionEvent.ACTION_POINTER_UP:
				index = event.getActionIndex();
				if (event.getPointerId(index) == previousSingleTouchID) {
					phase = EventFlagTouchEnd;
					previousSingleTouchID = -1;
				} else 
					phase = -1;
				break;
			default:
				AphidLog.d("Unknown action of MotionEvent: %d", event.getAction());
				phase = -1;
				break;
		}

		if (phase >= 0 && index >= 0) {
			getLocationOnScreen(location);
			AphidTouchEvent touch = AphidTouchEvent.createSingleTouchEvent(location, event, phase, index);
			renderer.handleTouchEvent(touch);
			return true;
		}

		return false;
	}
	
	private boolean handleMultiTouchEvent(MotionEvent event) {
		final int phase;
		
		switch(event.getActionMasked()) {
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN:
				phase = EventFlagTouchStart;
				break;
			case MotionEvent.ACTION_MOVE:
				phase = EventFlagTouchMove;
				break;
			case MotionEvent.ACTION_CANCEL:
				phase = EventFlagTouchCancel;
				break;
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP:
				phase = EventFlagTouchEnd;
				break;
			default:
				AphidLog.d("Unknown multitouch action of MotionEvent: %d", event.getAction());
				phase = -1;
				break;
		}
		
		if (phase >= 0) {
			getLocationOnScreen(location);
			AphidTouchEvent touchEvent = AphidTouchEvent.createMultiTouchEvent(location, event, phase);
			renderer.handleTouchEvent(touchEvent);
			return true;
		}
		return false;
	}

	@Override
	public void onPause() {
		AphidLog.i("surface view, onPause");
		super.onPause();
	}

	@Override
	public void onResume() {
		AphidLog.i("surface view onResume");
		super.onResume();
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		AphidLog.i("surface view, surface destroyed, thread id %s, isMainThread %s", Thread.currentThread().getId(),
				UI.isMainThread());
		renderer.call(new Callable<Void>() {
			public Void call() {
				renderer.onSurfaceDestroyed();
				return null;
			}
		});
		super.surfaceDestroyed(holder);
	}
}
