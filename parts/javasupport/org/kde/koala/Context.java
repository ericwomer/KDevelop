/***************************************************************************
                            Context.java -  description
                             -------------------
    begin                : Mon May 21 09:39:20 2001
    copyright            : (C) 2000-2001 Lost Highway Ltd.
    email                : Richard_Dale@tipitina.demon.co.uk
    generated by         : duke@tipitina on Mon May 21 09:39:20 2001, using kdoc parser/custom Java conversion backend.
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
***************************************************************************/

package org.kde.koala;

import org.kde.qt.*;
import java.util.*;
import java.lang.Error;

/**
*/
public class  Context implements QtSupport {
	private long _qt;
	private boolean _allocatedInJavaWorld = true;

	protected Context(Class dummy){}

	private native void newContext( String type);
	public Context( String type) {
		newContext( type);
	}
	protected native void finalize() throws InternalError;
	public native boolean hasType( String type);
}
