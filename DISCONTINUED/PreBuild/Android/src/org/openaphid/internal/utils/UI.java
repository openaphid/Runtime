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

import java.util.HashMap;
import java.util.concurrent.Callable;
import java.util.concurrent.FutureTask;

import org.openaphid.internal.AppDelegate;

import android.graphics.*;
import android.os.Looper;
import android.text.*;
import android.text.TextUtils.TruncateAt;
import android.util.FloatMath;

public class UI {
	public static final int ALIGNMENT_LEFT = 0;
	public static final int ALIGNMENT_CENTER = 1;
	public static final int ALIGNMENT_RIGHT = 2;

	public static final int LINE_BREAK_WORD_WRAP = 0;
	public static final int LINE_BREAK_CHARACTER_WRAP = 1;
	public static final int LINE_BREAK_CLIP = 2;
	public static final int LINE_BREAK_HEAD_TRUNCATION = 3;
	public static final int LINE_BREAK_TAIL_TRUNCATION = 4;
	public static final int LINE_BREAK_MIDDLE_TRUNCATION = 5;

	private static final HashMap<String, Typeface> typeface_cache = new HashMap<String, Typeface>();

	public static boolean isMainThread() {
		return Looper.myLooper() == Looper.getMainLooper();
	}
	
	public static void assertInMainThread() {
		if (!isMainThread())
			throw new RuntimeException("Main thread assertion failed");
	}
	
	public static <T> T callInMainThread(Callable<T> call) throws Exception {
		if (isMainThread())
			return call.call();
		else {
			FutureTask<T> task = new FutureTask<T>(call);
			AppDelegate.getHandler().post(task);
			return task.get();
		}
	}

	public static Typeface getTypeface(String fontname) {
		Typeface typeface = typeface_cache.get(fontname);
		if (typeface == null) {
			typeface = Typeface.create(fontname, Typeface.NORMAL);
			typeface_cache.put(fontname, typeface);
		}
		return typeface;
	}

	private static final TextPaint shared_text_paint = new TextPaint();

	@AphidNativeExposed
	public static Bitmap stringToBitmap(String text, String fontName, float fontSize, int width, int height,
			int alignment, int lineBreakMode) {

		shared_text_paint.setAntiAlias(true);

		if (fontName != null && fontName.length() > 0)
			shared_text_paint.setTypeface(getTypeface(fontName));
		else
			shared_text_paint.setTypeface(null);

		shared_text_paint.setTextSize(fontSize);
		shared_text_paint.setColor(Color.WHITE);

		Layout.Alignment layoutAlignment = Layout.Alignment.ALIGN_NORMAL;

		switch (alignment) {
			case ALIGNMENT_LEFT:
				layoutAlignment = Layout.Alignment.ALIGN_NORMAL;
				break;
			case ALIGNMENT_CENTER:
				layoutAlignment = Layout.Alignment.ALIGN_CENTER;
				break;
			case ALIGNMENT_RIGHT:
				layoutAlignment = Layout.Alignment.ALIGN_OPPOSITE;
				break;
			default:
				layoutAlignment = Layout.Alignment.ALIGN_NORMAL;
				break;
		}

		TruncateAt truncateAt = null;
		switch (lineBreakMode) {//TODO: StaticLayout doesn't support some line break modes, we need to implements our own layout in future
			case LINE_BREAK_WORD_WRAP:
				break;
			case LINE_BREAK_CHARACTER_WRAP:
				break;
			case LINE_BREAK_CLIP:
				break;
			case LINE_BREAK_HEAD_TRUNCATION:
				truncateAt = TruncateAt.START;
				break;
			case LINE_BREAK_TAIL_TRUNCATION:
				truncateAt = TruncateAt.END;
				break;
			case LINE_BREAK_MIDDLE_TRUNCATION:
				//truncateAt = TruncateAt.MARQUEE;
				break;

			default:
				break;
		}

		int measuredWidth = width == 0 ? (int) FloatMath.ceil(shared_text_paint.measureText(text)) : width;

		//TODO: find a way to reuse layout
		StaticLayout layout = new StaticLayout(text, 0, text.length(), shared_text_paint, measuredWidth, layoutAlignment,
				1, 0, false, truncateAt, measuredWidth);

		int layoutWidth = layout.getWidth();
		int layoutHeight = layout.getHeight();

		if (layoutWidth > 0 && (height > 0 || layoutHeight > 0)) {
			Bitmap bm = Bitmap.createBitmap(layout.getWidth(), height <= 0 ? layoutHeight : height, Bitmap.Config.ARGB_8888);
			Canvas canvas = new Canvas(bm);
			//debug option: canvas.drawColor(Color.RED);
			layout.draw(canvas);

			return bm;
		}

		return null;
	}
}
