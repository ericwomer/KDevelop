/***************************************************************************
                            ParsedClassContainer.java -  description
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

 This class is a container that also can hold classes.
 It has a range of functions to work with the classes it holds.

 @author Jonas Nordin
 
 @short 
 This class is a container that also can hold classes.
*/
public class ParsedClassContainer extends ParsedContainer  {
	protected ParsedClassContainer(Class dummy){super((Class) null);}

	private native void newParsedClassContainer();
	public ParsedClassContainer() {
		super((Class) null);
		newParsedClassContainer();
	}
	protected native void finalize() throws InternalError;

/** 
 Tells if a class exists in the store. 

*/
	public native boolean hasClass( String aName);

/** 
 Fetches a class from the store by using its' name. 

*/
	public native ParsedClass getClassByName( String aName);

/** 
 Gets all classnames in sorted order.

*/
	public native ArrayList getSortedClassNameList( boolean useFullPath);

/** 
 Removes all items in the store with references to the file. 

*/
	public native void removeWithReferences( String aFile);

/** 
 Adds a classdefintion. 

*/
	public native void addClass( ParsedClass aClass);

/** 
 Removes a class from the store. 

*/
	public native void removeClass( String aName);

/**  Clears the internal state. 
*/
	public native void clear( boolean bAutodel);
	public native void clear();

/**  Return a list of classes
*/
	public native ArrayList classList();
}
