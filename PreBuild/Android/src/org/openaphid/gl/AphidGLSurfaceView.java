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

	private boolean multipleTouchEnabled = false;

	private int[] location = { 0, 0 };

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

	public boolean isMultipleTouchEnabled() {
		return multipleTouchEnabled;
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event) {

		final int phase;

		switch (event.getActionMasked()) {
			case MotionEvent.ACTION_DOWN:
				phase = EventFlagTouchStart;
				break;
			case MotionEvent.ACTION_MOVE:
				phase = EventFlagTouchMove;
				break;
			case MotionEvent.ACTION_UP:
				phase = EventFlagTouchEnd;
				break;
			case MotionEvent.ACTION_CANCEL:
				phase = EventFlagTouchCancel;
				break;
			default:
				//TODO: handle multitouch actions
				AphidLog.d("Unknown action of MotionEvent: %d", event.getAction());
				phase = -1;
				break;
		}

		if (phase >= 0) {
			int eventHash = System.identityHashCode(event);
			long eventTime = event.getEventTime();
			getLocationOnScreen(location);
			AphidTouch touch = AphidTouch.create(location, event, eventHash, eventTime, phase, 0);//TODO handle multiple touches			
			renderer.handleSingleTouch(touch);
			return true;
		}

		return super.onTouchEvent(event);
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
