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

import java.util.Iterator;
import java.util.LinkedList;
import java.util.concurrent.Callable;
import java.util.concurrent.FutureTask;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import org.openaphid.internal.AphidJSBinder;
import org.openaphid.internal.AppDelegate;
import org.openaphid.internal.utils.AphidLog;

import android.opengl.GLSurfaceView;

public class AphidRenderer implements GLSurfaceView.Renderer {

	private final LinkedList<Runnable> eventQueue = new LinkedList<Runnable>();

	private final AphidTouchEventQueue touchQueue = new AphidTouchEventQueue();

	private boolean surfaceAlive = false;

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		AppDelegate.initializeNativeEngine();

		nativeOnSurfaceCreated();

		surfaceAlive = true;
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		nativeOnSurfaceChanged(width, height);
	}

	public void onSurfaceDestroyed() {
		nativeOnSurfaceDestroyed();
		AppDelegate.destroyAphidActivity();
		AphidLog.i("GL surface is destroyed");
	}

	private long lastTime = 0;

	@Override
	public void onDrawFrame(GL10 gl) {
		if (lastTime == 0)
			lastTime = System.currentTimeMillis();

		synchronized (eventQueue) {
			if (!eventQueue.isEmpty()) {
				Iterator<Runnable> iterator = eventQueue.iterator();
				while (iterator.hasNext()) {
					Runnable r = iterator.next();
					r.run();
				}
				eventQueue.clear();
			}
		}

		if (surfaceAlive) {

			touchQueue.processAllTouchesInGLThread(this);

			nativeOnDrawFrame();

			long interval = System.currentTimeMillis() - lastTime;
			long wait = (long) (AppDelegate.getFrameInterval() * 1000 - interval);
			if (wait > 0) {
				try {
					Thread.sleep(wait);
				} catch (InterruptedException e) {
					AphidLog.w("Failed to sleep %dms", wait);
				}
			}

			lastTime = System.currentTimeMillis();
		}
	}

	public <V> V call(Callable<V> callable) {
		FutureTask<V> task = new FutureTask<V>(callable);
		synchronized (eventQueue) {
			post(task);
		}

		try {
			return task.get();
		} catch (Exception e) {
			AphidLog.e(e, "Failed to call a callable in GL thread");
			return null;
		}
	}

	public void post(Runnable runnable) {
		synchronized (eventQueue) {
			eventQueue.add(runnable);
		}
	}

	public void handleTouchEvent(AphidTouchEvent touch) {
		touchQueue.queueTouchInMainThread(touch);
	}
	
	public void setScriptBinding(String name, Object bindingObject, final boolean androidOnly) {
		final AphidJSBinder binder = AphidJSBinder.createBinder(name, bindingObject);
		if (name != null && bindingObject != null) {
			post(new Runnable() {
				public void run() {
					nativeBindJavaObject(binder.getBindingName(), binder, androidOnly);
				}
			});
		}
	}

	public void evaluateScriptFile(final String filename) {
		post(new Runnable() {
			public void run() {
				if (!nativeEvaluateScriptFile(filename))
					AphidLog.e("Failed to evaluate JavaScript file: %s", filename);
			}
		});
	}

	private native void nativeOnSurfaceCreated();

	private native void nativeOnSurfaceChanged(int width, int height);

	private native void nativeOnDrawFrame();

	private native void nativeOnSurfaceDestroyed();

	native void nativeOnTouch(AphidTouchEvent touch);

	private native boolean nativeEvaluateScriptFile(String filename);
	
	private native void nativeBindJavaObject(String name, AphidJSBinder binder, boolean androidOnly);
}
