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

package org.openaphid.internal.utils;

import java.io.IOException;
import java.io.InputStream;
import java.net.*;
import java.util.zip.GZIPInputStream;
import java.util.zip.InflaterInputStream;

import org.openaphid.internal.AppDelegate;

public class Net {
	private static final int READ_TIMEOUT = 45 * 1000;
	private static final int CONNECT_TIMEOUT = 15 * 1000;

	private Net() {
	}

	public static URL newURL(String urlString) {
		try {
			return new URL(AppDelegate.getBaseURL(), urlString);
		} catch (MalformedURLException e) {
			AphidLog.e(e, "Failed to create URL from %s", urlString);
			return null;
		}
	}

	public static InputStream openInput(URL url) {
		if (url == null)
			return null;

		try {
			HttpURLConnection con = (HttpURLConnection) url.openConnection();
			con.setConnectTimeout(CONNECT_TIMEOUT);
			con.setReadTimeout(READ_TIMEOUT);
			con.setInstanceFollowRedirects(true);
			con.setRequestProperty("Accept-Encoding", "gzip");
			con.connect();
			if (con.getResponseCode() == HttpURLConnection.HTTP_OK) {
				return openInput(con);
			} else
				AphidLog.w("inputstream error code %d", con.getResponseCode());
		} catch (Exception e) {
			AphidLog.w("failed to open inputstream, %s", e);
		}

		return null;
	}

	public static InputStream openInput(URLConnection con) {
		try {
			InputStream input = con.getInputStream();
			String encodings = con.getContentEncoding();

			if (encodings != null) {
				encodings = encodings.toLowerCase();
				if (encodings.contains("gzip"))
					return new GZIPInputStream(input);
				else if (encodings.contains("deflate"))
					return new InflaterInputStream(input);
			}

			return input;
		} catch (IOException e) {
			AphidLog.e(e, "Failed to open inputstream from URLConnection %s", con);
			return null;
		}
	}
}
