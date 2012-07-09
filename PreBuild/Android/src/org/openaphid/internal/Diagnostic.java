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

import org.openaphid.internal.utils.*;

import android.graphics.Color;
import android.widget.TextView;
import android.widget.Toast;

@AphidNativeExposed
public class Diagnostic {
	private Diagnostic() {
	}

	public static final int INFO_COLOR = Color.TRANSPARENT;

	public static void error(String message) {
		error(message, true);
	}

	@AphidNativeExposed
	public static void error(String message, boolean logInConsole) {
		if (message != null) {
			if (logInConsole)
				AphidLog.e("(OpenAphid Diagnostic) Error, " + message);
			if (AppDelegate.isDeveloperMode())
				displayNotification(Color.RED, message, false);
		}
	}

	public static void warn(String message) {
		warn(message, true);
	}

	@AphidNativeExposed
	public static void warn(String message, boolean logInConsole) {
		if (message != null) {
			if (logInConsole)
				AphidLog.w("(OpenAphid Diagnostic) Warning, " + message);
			if (AppDelegate.isDeveloperMode())
				displayNotification(Color.BLUE, message, false);
		}
	}

	public static void info(String message) {
		info(message, true);
	}

	@AphidNativeExposed
	public static void info(String message, boolean logInConsole) {
		if (message != null) {
			if (logInConsole)
				AphidLog.i("(OpenAphid Diagnostic) Info, " + message);
			if (AppDelegate.isDeveloperMode())
				displayNotification(INFO_COLOR, message, true);
		}
	}

	public static void displayNotification(final int color, final String message, final boolean shortTimeout) {
		if (UI.isMainThread())
			doDisplayNotification(color, message, shortTimeout);
		else
			AppDelegate.getHandler().post(new Runnable() {
				public void run() {
					doDisplayNotification(color, message, shortTimeout);
				}
			});
	}

	public static void doDisplayNotification(int color, String message, boolean shortTimeout) {
		if (color == INFO_COLOR)
			Toast.makeText(AppDelegate.getApplicationContext(), message,
					shortTimeout ? Toast.LENGTH_SHORT : Toast.LENGTH_LONG).show();
		else {
			TextView tv = new TextView(AppDelegate.getApplicationContext());
			tv.setText(message);
			tv.setBackgroundColor(color);

			Toast toast = new Toast(AppDelegate.getApplicationContext());
			toast.setView(tv);
			toast.setDuration(shortTimeout ? Toast.LENGTH_SHORT : Toast.LENGTH_LONG);
			toast.show();
		}
	}
}
