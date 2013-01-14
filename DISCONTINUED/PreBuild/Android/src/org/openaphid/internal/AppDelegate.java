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
import org.openaphid.internal.utils.AphidNativeExposed;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.os.Handler;

@AphidNativeExposed
public class AppDelegate {
	public static final String VERSION_STRING = "OpenAphid Android v0.1.5";
	
	private static boolean developer_mode = true;

	private static URL base_url = null;

	private static Context application_context;

	private static AphidActivity aphid_activity;

	private static AssetsBundle game_bundle;

	private static Handler shared_handler = null;

	private static double frame_interval = 1.0 / 60;
	
	private static boolean multitouch_enabled = false;

	public static Handler getHandler() {
		return shared_handler;
	}

	public static void initialize(AphidActivity activity, URL baseURL, String gameBundleName, boolean developerMode) {
		aphid_activity = activity;
		application_context = activity.getApplicationContext();
		shared_handler = new Handler();

		base_url = baseURL;
		developer_mode = developerMode;
		game_bundle = new AssetsBundle(gameBundleName);
	}

	public static void onAphidActivityDestroyed(AphidActivity activity) {
		aphid_activity = null;
	}

	public static void initializeNativeEngine() {
		nativeInitialize(developer_mode, base_url != null ? base_url.toExternalForm() : null);
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

	@AphidNativeExposed
	public static AssetsBundle getGameBundle() {
		return game_bundle;
	}

	public static boolean isDeveloperMode() {
		return developer_mode;
	}

	public static URL getBaseURL() {
		return base_url;
	}

	public static double getFrameInterval() {
		return frame_interval;
	}

	@AphidNativeExposed
	public static void setFrameInterval(double frame_interval) {
		AppDelegate.frame_interval = frame_interval;
	}
	
	@AphidNativeExposed
	public static boolean isMultitouchEnabled() {
		return multitouch_enabled;
	}
	
	@AphidNativeExposed
	public static void setMultitouchEnabled(boolean multipleTouchEnabled) {
		multitouch_enabled = multipleTouchEnabled;
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
