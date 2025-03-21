//# ArrayError.h: Exception classes thrown by Array and related classes/functions
//# Copyright (C) 1993,1994,1995,1999,2000
//# Associated Universities, Inc. Washington DC, USA.
//# 
//# This library is free software; you can redistribute it and/or modify it
//# under the terms of the GNU Library General Public License as published by
//# the Free Software Foundation; either version 2 of the License, or (at your
//# option) any later version.
//# 
//# This library is distributed in the hope that it will be useful, but WITHOUT
//# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//# License for more details.
//# 
//# You should have received a copy of the GNU Library General Public License
//# along with this library; if not, write to the Free Software Foundation,
//# Inc., 675 Massachusetts Ave, Cambridge, MA 02139, USA.
//# 
//# Correspondence concerning AIPS++ should be addressed as follows:
//#        Internet email: aips2-request@nrao.edu.
//#        Postal address: AIPS++ Project Office
//#                        National Radio Astronomy Observatory
//#                        520 Edgemont Road
//#                        Charlottesville, VA 22903-2475 USA

#ifndef CASA_ARRAYERROR_2_H
#define CASA_ARRAYERROR_2_H

//# Includes
#include <stdexcept>
#include <string>

#include "IPosition.h"

namespace casacore { //# NAMESPACE CASACORE - BEGIN

// <summary> The base class for all Array exception classes. </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
//
// ArrayError is the base class for all the Array-specific exception classes,
// i.e. if it is caught you will catch (through inheritance) all Array-specific
// exceptions. Note that (presently, anyway) the Array classes will throw
// a few non-Array exceptions.
// <srcblock>
// try {
//    // Some lines, functions, ...
// } catch (ArrayError x) {
//    // Array specific errors
// } catch (std::exception x) {
//    // All other errors caught here.
// }
// </srcblock>
//
//# There are too many Array related error classes. Some should be deleted.

class ArrayError : public std::runtime_error
{
public:
  // Initialize with the message "ArrayError"
  ArrayError();
  // Initialize with the supplied message.
  ArrayError(const char *m);
  // Initialize with the supplied message.
  ArrayError(const std::string& m);
  ~ArrayError() noexcept;
};


// <summary> An error thrown when an index is out of range </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
//
// The ArrayIndexError class, which is derived from ArrayError, is intended
// to be thrown when an index is out-of-bounds. It contains within it
// the offending index, as well as the shape of the array which
// is being indexed. This should be multiply-derived from
// indexError<T> defined in Error.h.
class ArrayIndexError : public ArrayError
{
public:
    // Initialize with the message "ArrayIndexError".
    ArrayIndexError();
    // Initialize with the supplied message, the index and shape are null.
    ArrayIndexError(const char *m);
    // Initialize with the supplied message, the index and shape are null.
    ArrayIndexError(const std::string &m);
    // Initialize with a given out-of-bounds index, as well as the shape
    // of the array and a supplied message.
    ArrayIndexError(const IPosition &index, const IPosition &shape, 
		    const char *m="ArrayIndexError");
    ~ArrayIndexError() noexcept;
    // The out-of-bounds index.
    IPosition index() const;
    // The shape of the violated array.
    IPosition shape() const;
private:
    //# index, offset, length
    IPosition i,l;
};


// <summary> An error thrown when two arrays do not conform </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
//
// The ArrayConformanceError class is the base class for all errors thrown
// because two arrays are not conformant. See also the ArrayShapeError and
// ArrayNDimError classes which are derived from it. This error, or one derived
// from it, os normally thrown from a binary operation (arithmetic, logical,
// assignment, etc).
class ArrayConformanceError : public ArrayError
{
public:
    // Initialize the message with "ArrayConformanceError".
    ArrayConformanceError();
    // Initialize with a supplied message.
    ArrayConformanceError(const char *m);
    // Initialize with a supplied message.
    ArrayConformanceError(const std::string& m);
    ~ArrayConformanceError() noexcept;
};


// <summary> Thrown when two arrays have different dimensionality </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
//
// An ArrayNDimError is derived from ArrayConformanceError. It is thrown when
// two arrays are non-conformant by virtue of having different dimensionality.
// It holds within it the two dimensions.
class ArrayNDimError : public ArrayConformanceError
{
public:
    // Define the two (presumably different) messages and optionally
    // supply a message.
    ArrayNDimError(int dim1, int dim2, const char *m="ArrayNDimError");
    ArrayNDimError(int dim1, int dim2, const std::string& m);
    ~ArrayNDimError() noexcept;
    // Return the stored dimensions. NB modifies arguments.
    void ndims(int &dim1, int &dim2) const; // modifies arguments
private:
    int r1, r2;
};


// <summary> An error thrown when two arrays have different shapes </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
//
// An ArrayShapeError is derived from ArrayConformanceError. It is thrown when
// two arrays are non-conformant by virtue of having different shapes.
// It holds within it the two different two shapes.
class ArrayShapeError : public ArrayConformanceError
{
public:
    // Define an ArrayShapeError with the two (presumably different) shapes
    // and an optional supplied message.
    ArrayShapeError(const IPosition &shape1, const IPosition &shape2,
		     const char *m="ArrayShapeError");
    ~ArrayShapeError() noexcept;
    // Get back the stored shapes. NB modifies arguments.
    void shapes(IPosition &, IPosition &) const;  // modifies arguments
private:
    IPosition sh1, sh2;
};


// <summary> An error thrown by an ArrayIterator </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
//
// An ArrayIteratorError is thrown by an array iterator or related class
// (e.g. VectorIterator).
class ArrayIteratorError : public ArrayError
{
public:    
    // Initialize with the message "ArrayIteratorError.
    ArrayIteratorError();
    // Initialize with the supplied message
    ArrayIteratorError(const char *m);
    // Initialize with the supplied message
    ArrayIteratorError(const std::string &m);
    ~ArrayIteratorError() noexcept;
};


// <summary> An error thrown by an Slicer member function </summary>
// <reviewed reviewer="UNKNOWN" date="before2004/08/25" tests="" demos="">
// </reviewed>
//
// An ArraySlicerError is thrown by an  Slicer member function.
class ArraySlicerError : public ArrayError
{
public:    
    // Initialize with the message "Slicer error."
    ArraySlicerError();
    // Initialize with ArraySlicerError plus the supplied message
    ArraySlicerError(const std::string &m);
    ~ArraySlicerError() noexcept;
};


} //# NAMESPACE CASACORE - END

#endif
