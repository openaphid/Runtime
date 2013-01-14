package org.openaphid.bind;

import java.lang.annotation.*;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
@Inherited
public @interface AphidJSFunction {
	public static final String DEFAULT_NAME = "";
	
	String name() default DEFAULT_NAME;
	
	boolean isInUIThread() default true;
}
