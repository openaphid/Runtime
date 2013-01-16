package org.openaphid.internal;

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.*;
import java.util.concurrent.Callable;

import org.openaphid.bind.AphidJSFunction;
import org.openaphid.internal.utils.*;

public class AphidJSBinder {
	private static final Object ERROR_RETURN = new Object();

	private String bindingName;
	private Object boundObject;

	private HashMap<String, MethodMeta> methodMap = new HashMap<String, MethodMeta>();

	@SuppressWarnings("rawtypes")
	static HashMap<Class, Character> TYPE_SIGNATURES;
	@SuppressWarnings("rawtypes")
	static HashMap<Class, Character> EXT_TYPE_SIGNATURES;

	static {
		TYPE_SIGNATURES = new HashMap<Class, Character>();
		EXT_TYPE_SIGNATURES = new HashMap<Class, Character>();

		TYPE_SIGNATURES.put(boolean.class, 'Z');
		EXT_TYPE_SIGNATURES.put(Boolean.class, 'Z');

		TYPE_SIGNATURES.put(byte.class, 'B');
		EXT_TYPE_SIGNATURES.put(Byte.class, 'B');

		TYPE_SIGNATURES.put(char.class, 'C');
		EXT_TYPE_SIGNATURES.put(Character.class, 'C');

		TYPE_SIGNATURES.put(short.class, 'S');
		EXT_TYPE_SIGNATURES.put(Short.class, 'S');

		TYPE_SIGNATURES.put(int.class, 'I');
		EXT_TYPE_SIGNATURES.put(Integer.class, 'I');

		TYPE_SIGNATURES.put(long.class, 'J');
		EXT_TYPE_SIGNATURES.put(Long.class, 'J');

		TYPE_SIGNATURES.put(float.class, 'F');
		EXT_TYPE_SIGNATURES.put(Float.class, 'F');

		TYPE_SIGNATURES.put(double.class, 'D');
		EXT_TYPE_SIGNATURES.put(Double.class, 'D');

		TYPE_SIGNATURES.put(void.class, 'v');
		TYPE_SIGNATURES.put(String.class, 's');
		TYPE_SIGNATURES.put(List.class, 'l');
		TYPE_SIGNATURES.put(Map.class, 'm');

		EXT_TYPE_SIGNATURES.putAll(TYPE_SIGNATURES);
	}

	private static final class MethodMeta {
		public Method method;
		public String name;
		public boolean isInMainThread = false;

		public Class<?> returnType;
		public Class<?>[] parameterTypes;
		public char[] parameterTypeSignature;
		public char returnTypeSignature;

		public MethodMeta(Method method, AphidJSFunction annotation) {
			this.method = method;
			name = annotation.name();
			if (name == null || name.length() == 0)
				name = method.getName();
			isInMainThread = annotation.isInUIThread();
			returnType = method.getReturnType();
			parameterTypes = method.getParameterTypes();
			parameterTypeSignature = new char[parameterTypes.length];
			for (int i = 0; i < parameterTypes.length; i++) {
				parameterTypeSignature[i] = getParameterTypeSignature(parameterTypes[i]);
			}
			returnTypeSignature = getParameterTypeSignature(returnType);
		}

		public static char getParameterTypeSignature(Class<?> type) {
			Character c = TYPE_SIGNATURES.get(type);
			if (c != null)
				return c;
			else {
				AphidLog.w("Can't find signature char for " + type);
				return '0';
			}
		}

		public boolean validate() {
			if (!Modifier.isPublic(method.getModifiers())) {
				Diagnostic.error(AphidLog.format("Invalid visibility of binding method: '%s'", name));
				return false;
			}

			if (!isValidReturnType(returnType)) {
				Diagnostic.error(AphidLog.format("Invalid return type '%s' for binding: '%s'", returnType, name));
				return false;
			}

			for (int i = 0; i < parameterTypes.length; i++) {
				Class<?> parameterType = parameterTypes[i];
				if (!isValidParameterType(parameterType)) {
					Diagnostic.error(AphidLog.format("Invalid parameter type '%s' at %dth for binding '%s'", parameterType, i,
							name));
					return false;
				}
			}

			return true;
		}

		public static boolean isValidParameterType(Class<?> type) {
			return type != void.class && TYPE_SIGNATURES.containsKey(type);
		}

		public static boolean isValidReturnType(Class<?> type) {
			return TYPE_SIGNATURES.containsKey(type);
		}
	}

	private AphidJSBinder(String bindingName, Object boundObject) {
		this.bindingName = bindingName;
		this.boundObject = boundObject;
	}

	public void initialize() {
		Method[] methods = boundObject.getClass().getMethods();

		for (int i = 0; i < methods.length; i++) {
			Method method = methods[i];
			AphidJSFunction annotation = method.getAnnotation(AphidJSFunction.class);
			if (annotation == null)
				continue;
			
			MethodMeta meta = new MethodMeta(method, annotation);
			if (methodMap.containsKey(meta.name))
				Diagnostic.warn(AphidLog.format("Found duplicated binding name: %s; Method overloading is not supported in %s, the old one will be replaced",
						meta.name, AppDelegate.VERSION_STRING));
			if (!AppDelegate.isDeveloperMode() || meta.validate()) {
				methodMap.put(meta.name, meta);
				try {
					meta.method.setAccessible(true);
				} catch (SecurityException e) {
					AphidLog.w("Failed to pre-enable accessible for %s", meta.method);
				}
			}
		}
	}

	@AphidNativeExposed
	public static char getObjectTypeSignature(Object o) {
		if (o == null)
			return '0';
		Character c = EXT_TYPE_SIGNATURES.get(o.getClass());
		if (c != null)
			return c;
		if (o instanceof List)
			return 'l';
		else if (o instanceof Map)
			return 'm';

		AphidLog.w("Unrecognized class: " + o.getClass());
		return '0';
	}

	@AphidNativeExposed
	public static Object getErrorReturnObject() {
		return ERROR_RETURN;
	}

	public String getBindingName() {
		return bindingName;
	}

	public Object getBoundObject() {
		return boundObject;
	}

	@AphidNativeExposed
	public boolean respondsToFunction(String name) {
		return methodMap.containsKey(name);
	}

	@AphidNativeExposed
	public String[] listFunctionNames() {
		return methodMap.keySet().toArray(new String[0]);
	}

	@AphidNativeExposed
	public char[] getParameterTypeSignature(String name) {
		MethodMeta meta = methodMap.get(name);
		return meta == null ? new char[0] : meta.parameterTypeSignature;
	}

	@AphidNativeExposed
	public char getReturnTypeSignature(String name) {
		MethodMeta meta = methodMap.get(name);
		return meta == null ? '0' : meta.returnTypeSignature;
	}

	@AphidNativeExposed
	public Object invokeMethod(String name, final Object[] args) {
		final MethodMeta meta = methodMap.get(name);
		if (meta != null) {
			if (meta.isInMainThread) {
				try {
					return UI.callInMainThread(new Callable<Object>() {
						public Object call() {
							return doInvokeMethod(meta, args);
						}
					});
				} catch (Exception e) {
					AphidLog.e(e, "Failed in invokeVoidMethod, %s", name);
				}
			} else {
				return doInvokeMethod(meta, args);
			}
		} else
			AphidLog.w("Can't find binding method %s", name);

		return ERROR_RETURN;
	}

	private Object doInvokeMethod(MethodMeta meta, Object[] args) {
		try {
			return meta.method.invoke(boundObject, args);
		} catch (Exception e) {
			AphidLog.e(e, "Failed doIn invokeMethod, %s", meta.name);
			return ERROR_RETURN;
		}
	}

	public static AphidJSBinder createBinder(String name, Object boundObject) {
		AphidJSBinder binder = new AphidJSBinder(name, boundObject);
		binder.initialize();
		return binder;
	}

	@AphidNativeExposed
	public static Boolean toBoolean(boolean b) {
		return Boolean.valueOf(b);
	}

	@AphidNativeExposed
	public static Byte toByte(int i) {
		return Byte.valueOf((byte) i);
	}

	@AphidNativeExposed
	public static Character toChar(int i) {
		return Character.valueOf((char) i);
	}

	@AphidNativeExposed
	public static Character toChar(String s) {
		return Character.valueOf(s.charAt(0));
	}

	@AphidNativeExposed
	public static Short toShort(int i) {
		return Short.valueOf((short) i);
	}

	@AphidNativeExposed
	public static Integer toInteger(int i) {
		return Integer.valueOf(i);
	}

	@AphidNativeExposed
	public static Long toLong(int i) {
		return Long.valueOf(i);
	}

	@AphidNativeExposed
	public static Float toFloat(float f) {
		return Float.valueOf(f);
	}

	@AphidNativeExposed
	public static Double toDouble(double d) {
		return Double.valueOf(d);
	}

	@AphidNativeExposed
	public static boolean getBoolean(Boolean b) {
		return b.booleanValue();
	}

	@AphidNativeExposed
	public static int getInt(Number n) {
		return n.intValue();
	}

	@AphidNativeExposed
	public static float getFloat(Number n) {
		return n.floatValue();
	}

	@AphidNativeExposed
	public static double getDouble(Number n) {
		return n.doubleValue();
	}

	@AphidNativeExposed
	@SuppressWarnings("rawtypes")
	public static Iterator getKeyIterator(Map map) {
		return map.keySet().iterator();
	}

}
