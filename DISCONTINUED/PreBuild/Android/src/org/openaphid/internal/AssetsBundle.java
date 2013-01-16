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

import java.io.InputStream;
import java.net.URL;

import org.openaphid.internal.utils.*;

import android.graphics.Bitmap;

public class AssetsBundle {
	private String bundleName = null;

	public AssetsBundle(String bundleName) {
		this.bundleName = bundleName;
	}

	public void setBundleName(String bundleName) {
		this.bundleName = bundleName;
	}

	public String normalize(String path) {
		path = bundleName == null ? path : bundleName + "/" + path;
		int index = -1;
		int pathlen = path.length();
		int size = 0;
		if (pathlen > 0 && path.charAt(0) != '/') {
			size++;
		}
		while ((index = path.indexOf('/', index + 1)) != -1) {
			if (index + 1 < pathlen && path.charAt(index + 1) != '/') {
				size++;
			}
		}

		String[] seglist = new String[size];
		boolean[] include = new boolean[size];

		int current = 0;
		int index2 = 0;
		index = (pathlen > 0 && path.charAt(0) == '/') ? 1 : 0;
		while ((index2 = path.indexOf('/', index + 1)) != -1) {
			seglist[current++] = path.substring(index, index2);
			index = index2 + 1;
		}

		if (current < size) {
			seglist[current] = path.substring(index);
		}

		for (int i = 0; i < size; i++) {
			include[i] = true;
			if (seglist[i].equals("..")) {
				int remove = i - 1;
				while (remove > -1 && !include[remove]) {
					remove--;
				}
				if (remove > -1 && !seglist[remove].equals("..")) { //$NON-NLS-1$
					include[remove] = false;
					include[i] = false;
				}
			} else if (seglist[i].equals(".")) { //$NON-NLS-1$
				include[i] = false;
			}
		}

		StringBuilder newpath = new StringBuilder();
		if (path.startsWith("/")) { //$NON-NLS-1$
			newpath.append('/');
		}

		for (int i = 0; i < seglist.length; i++) {
			if (include[i]) {
				newpath.append(seglist[i]);
				newpath.append('/');
			}
		}

		if (!path.endsWith("/") && seglist.length > 0 //$NON-NLS-1$
				&& include[seglist.length - 1]) {
			newpath.deleteCharAt(newpath.length() - 1);
		}

		String result = newpath.toString();

		index = result.indexOf(':');
		index2 = result.indexOf('/');
		if (index != -1 && (index < index2 || index2 == -1)) {
			newpath.insert(0, "./"); //$NON-NLS-1$
			result = newpath.toString();
		}
		return result;
	}

	public InputStream openInput(String name) {
		try {
			String normalized = normalize(name);
			return AppDelegate.getAssetManager().open(normalized);
		} catch (Exception e) {
			AphidLog.e(e, "Failed to open input stream for %s in bundle %s", name, bundleName);
			return null;
		}
	}

	@AphidNativeExposed
	public String loadString(String name) {
		String string = null;
		if (AppDelegate.isDeveloperMode() && AppDelegate.getBaseURL() != null) {
			URL url = Net.newURL(name);
			AphidLog.i("(develop mode) loading string '%s' from remote: %s", name, url);
			string = IO.readString(Net.openInput(url));
		} else
			string = IO.readString(openInput(name));

		if (string == null)
			Diagnostic.warn(AphidLog.format("Failed to load string for '%s'", name));

		return string;
	}

	@AphidNativeExposed
	public byte[] loadData(String name, boolean supressLogging) {
		byte[] data = null;
		if (AppDelegate.isDeveloperMode() && AppDelegate.getBaseURL() != null) {
			URL url = Net.newURL(name);
			AphidLog.i("(develop mode) loading data '%s' from remote: %s", name, url);
			data = IO.readData(Net.openInput(url));
		} else
			data = IO.readData(openInput(name));

		if (data == null && !supressLogging)
			Diagnostic.warn(AphidLog.format("Failed to load data for '%s'", name));
		return data;
	}

	@AphidNativeExposed
	public Bitmap loadBitmap(String name) {
		Bitmap bm = null;
		if (AppDelegate.isDeveloperMode() && AppDelegate.getBaseURL() != null) {
			URL url = Net.newURL(name);
			AphidLog.i("(develop mode) loading bitmap '%s' from remote: %s", name, url);
			bm = IO.readBitmap(Net.openInput(url));
		} else
			bm = IO.readBitmap(openInput(name));

		if (bm == null)
			Diagnostic.warn(AphidLog.format("Failed to load bitmap for '%s'", name));
		else if (bm.getConfig() == null) {
			Bitmap bmFix = bm.copy(bm.hasAlpha() ? Bitmap.Config.ARGB_8888 : Bitmap.Config.RGB_565, false);
			bm.recycle();
			bm = bmFix;
		}

		return bm;
	}
}
