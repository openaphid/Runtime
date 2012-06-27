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

package org.openaphid.internal;

import java.net.URL;

import org.openaphid.gl.AphidActivity;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.os.Handler;

public class AppDelegate {
	private static boolean develop_mode = true;

	private static URL base_url = null;

	private static Context application_context;

	private static AphidActivity aphid_activity;

	private static AssetsBundle game_bundle;

	private static Handler shared_handler = null;

	private static double frame_interval = 1.0 / 60;

	public static Handler getHandler() {
		return shared_handler;
	}

	public static void initialize(AphidActivity activity, URL baseURL, String gameBundleName, boolean developMode) {
		aphid_activity = activity;
		application_context = activity.getApplicationContext();
		shared_handler = new Handler();

		base_url = baseURL;
		develop_mode = developMode;
		game_bundle = new AssetsBundle(gameBundleName);
	}

	public static void onAphidActivityDestroyed(AphidActivity activity) {
		aphid_activity = null;
	}

	public static void initializeNativeEngine() {
		nativeInitialize(develop_mode, base_url != null ? base_url.toExternalForm() : null);
	}

	public static Context getApplicationContext() {
		return application_context;
	}

	public static Resources getResources() {
		return application_context.getResources();
	}

	public static AssetManager getAssetManager() {
		return application_context.getResources().getAssets();
	}

	public static AssetsBundle getGameBundle() {
		return game_bundle;
	}

	public static boolean isDevelopMode() {
		return develop_mode;
	}

	public static URL getBaseURL() {
		return base_url;
	}

	public static double getFrameInterval() {
		return frame_interval;
	}

	public static void setFrameInterval(double frame_interval) {
		AppDelegate.frame_interval = frame_interval;
	}

	public static void destroyAphidActivity() {
		if (shared_handler != null) {
			shared_handler.post(new Runnable() {
				public void run() {
					if (aphid_activity != null)
						aphid_activity.finish();
				}
			});
		}
	}

	private static native void nativeInitialize(boolean mode, String url);
}
