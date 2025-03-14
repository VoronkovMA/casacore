//# StIndArray.cc: Read/write indirect arrays
//# Copyright (C) 1994,1995,1996,1997,1999,2001
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

#include <casacore/tables/DataMan/StIndArray.h>
#include <casacore/tables/DataMan/StArrayFile.h>
#include <casacore/casa/Arrays/Slicer.h>
#include <casacore/casa/Arrays/IPosition.h>
#include <casacore/casa/Arrays/Array.h>
#include <casacore/casa/Arrays/ArrayPosIter.h>
#include <casacore/casa/Utilities/DataType.h>
#include <casacore/casa/BasicSL/Complex.h>
#include <casacore/casa/BasicSL/String.h>
#include <casacore/tables/DataMan/DataManError.h>


namespace casacore { //# NAMESPACE CASACORE - BEGIN

StIndArray::StIndArray (Int64 fileOffset)
: fileOffset_p (fileOffset),
  arrOffset_p  (0)
{}

StIndArray::StIndArray (const StIndArray& that)
: fileOffset_p (that.fileOffset_p),
  arrOffset_p  (that.arrOffset_p),
  shape_p      (that.shape_p)
{}

StIndArray::~StIndArray()
{}

StIndArray& StIndArray::operator= (const StIndArray& that)
{
    if (this != &that) {
	fileOffset_p = that.fileOffset_p;
	arrOffset_p  = that.arrOffset_p;
	shape_p.resize (that.shape_p.nelements());
	shape_p      = that.shape_p;
    }
    return *this;
}

void StIndArray::getShape (StManArrayFile& ios)
{
    if (arrOffset_p == 0) {
	arrOffset_p = ios.getShape (fileOffset_p, shape_p);
    }
}

uInt StIndArray::refCount (StManArrayFile& ios)
{
    return ios.getRefCount (fileOffset_p);
}

void StIndArray::incrementRefCount (StManArrayFile& ios)
{
    uInt refCount = ios.getRefCount (fileOffset_p);
    refCount++;
    ios.putRefCount (refCount, fileOffset_p);
#ifdef AIPS_TRACE
    cout << "   incr refcount to " << refCount << " at "<<fileOffset_p<<endl;
#endif
}

void StIndArray::decrementRefCount (StManArrayFile& ios)
{
    uInt refCount = ios.getRefCount (fileOffset_p);
    refCount--;
    ios.putRefCount (refCount, fileOffset_p);
#ifdef AIPS_TRACE
    cout << "   decr refcount to " << refCount << " at "<<fileOffset_p<<endl;
#endif
}

Bool StIndArray::setShape (StManArrayFile& ios, int dataType,
			   const IPosition& shape)
{
    // Return immediately if the shape is defined and is the same.
    if (arrOffset_p != 0  &&  shape_p.isEqual (shape)) {
	return False;
    }
    // Set the shape.
    shape_p.resize (shape.nelements());
    shape_p = shape;
    // Store the shape in the file and allocate storage for the array.
    switch (dataType) {
    case TpBool:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<Bool*>(0));
	break;
    case TpUChar:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<uChar*>(0));
	break;
    case TpShort:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<Short*>(0));
	break;
    case TpUShort:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<uShort*>(0));
	break;
    case TpInt:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<Int*>(0));
	break;
    case TpUInt:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<uInt*>(0));
	break;
    case TpInt64:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<Int64*>(0));
	break;
    case TpFloat:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<float*>(0));
	break;
    case TpDouble:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<double*>(0));
	break;
    case TpComplex:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<Complex*>(0));
	break;
    case TpDComplex:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<DComplex*>(0));
	break;
    case TpString:
	arrOffset_p = ios.putShape (shape_p, fileOffset_p, static_cast<String*>(0));
	break;
    }
    return True;
}


void StIndArray::copyData (StManArrayFile& ios, int dataType,
			   const StIndArray& other)

{
    // Check if both shape are equal.
    if (! shape_p.isEqual (other.shape_p)) {
	throw (DataManInternalError
	         ("StManIndArray::copyData shapes not conforming"));
    }
    switch (dataType) {
    case TpBool:
	ios.copyArrayBool (fileOffset_p + arrOffset_p,
			   other.fileOffset_p + other.arrOffset_p,
			   shape_p.product());
	break;
    case TpUChar:
	ios.copyArrayuChar (fileOffset_p + arrOffset_p,
			    other.fileOffset_p + other.arrOffset_p,
			    shape_p.product());
	break;
    case TpShort:
	ios.copyArrayShort (fileOffset_p + arrOffset_p,
			    other.fileOffset_p + other.arrOffset_p,
			    shape_p.product());
	break;
    case TpUShort:
	ios.copyArrayuShort (fileOffset_p + arrOffset_p,
			     other.fileOffset_p + other.arrOffset_p,
			     shape_p.product());
	break;
    case TpInt:
	ios.copyArrayInt (fileOffset_p + arrOffset_p,
			  other.fileOffset_p + other.arrOffset_p,
			  shape_p.product());
	break;
    case TpUInt:
	ios.copyArrayuInt (fileOffset_p + arrOffset_p,
			   other.fileOffset_p + other.arrOffset_p,
			   shape_p.product());
	break;
    case TpInt64:
	ios.copyArrayInt64 (fileOffset_p + arrOffset_p,
                            other.fileOffset_p + other.arrOffset_p,
                            shape_p.product());
	break;
    case TpFloat:
	ios.copyArrayFloat (fileOffset_p + arrOffset_p,
			    other.fileOffset_p + other.arrOffset_p,
			    shape_p.product());
	break;
    case TpDouble:
	ios.copyArrayDouble (fileOffset_p + arrOffset_p,
			     other.fileOffset_p + other.arrOffset_p,
			     shape_p.product());
	break;
    case TpComplex:
	ios.copyArrayComplex (fileOffset_p + arrOffset_p,
			      other.fileOffset_p + other.arrOffset_p,
			      shape_p.product());
	break;
    case TpDComplex:
	ios.copyArrayDComplex (fileOffset_p + arrOffset_p,
			       other.fileOffset_p + other.arrOffset_p,
			       shape_p.product());
	break;
    case TpString:
	ios.copyArrayString (fileOffset_p + arrOffset_p,
			     other.fileOffset_p + other.arrOffset_p,
			     shape_p.product());
	break;
    default:
      throw DataManInternalError("Unhandled data type in StIndArray::copyData");
    }
}


void StIndArray::checkShape (const IPosition& userArrayShape,
			     const IPosition& tableArrayShape) const
{
    if (! userArrayShape.isEqual (tableArrayShape)) {
	throw (DataManInternalError
                 ("StManIndArray::get/put shapes not conforming"));
    }
}

void StIndArray::getArrayV (StManArrayFile& ios, ArrayBase& arr,
                            DataType dtype)
{
    checkShape (arr.shape(), shape_p);
    Bool deleteIt;
    void* value = arr.getVStorage (deleteIt);
    switch (dtype) {
    case TpBool:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<Bool*>(value));
      break;
    case TpUChar:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<uChar*>(value));
      break;
    case TpShort:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<Short*>(value));
      break;
    case TpUShort:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<uShort*>(value));
      break;
    case TpInt:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<Int*>(value));
      break;
    case TpUInt:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<uInt*>(value));
      break;
    case TpInt64:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<Int64*>(value));
      break;
    case TpFloat:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<float*>(value));
      break;
    case TpDouble:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<double*>(value));
      break;
    case TpComplex:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<Complex*>(value));
      break;
    case TpDComplex:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<DComplex*>(value));
      break;
    case TpString:
      ios.get (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<String*>(value));
      break;
    default:
      const void* cvalue = value;
      arr.freeVStorage (cvalue, deleteIt);
      throw DataManInternalError("Unhandled data type in StIndArray::getArrayV");
      break;
    }
    arr.putVStorage (value, deleteIt);
}

void StIndArray::putArrayV (StManArrayFile& ios, const ArrayBase& arr,
                            DataType dtype)
{
    checkShape (arr.shape(), shape_p);
    Bool deleteIt;
    const void* value = arr.getVStorage (deleteIt);
    switch (dtype) {
    case TpBool:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const Bool*>(value));
      break;
    case TpUChar:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const uChar*>(value));
      break;
    case TpShort:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const Short*>(value));
      break;
    case TpUShort:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const uShort*>(value));
      break;
    case TpInt:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const Int*>(value));
      break;
    case TpUInt:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const uInt*>(value));
      break;
    case TpInt64:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const Int64*>(value));
      break;
    case TpFloat:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const float*>(value));
      break;
    case TpDouble:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const double*>(value));
      break;
    case TpComplex:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const Complex*>(value));
      break;
    case TpDComplex:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const DComplex*>(value));
      break;
    case TpString:
      ios.put (fileOffset_p + arrOffset_p, 0, shape_p.product(),
               static_cast<const String*>(value));
      break;
    default:
      arr.freeVStorage (value, deleteIt);
      throw DataManInternalError("Unhandled data type in StIndArray::putArrayV");
      break;
    }
    arr.freeVStorage (value, deleteIt);
}

void StIndArray::getSliceV (StManArrayFile& ios, const Slicer& ns,
                            ArrayBase& arr, DataType dtype)
{
    Bool deleteIt;
    void* value = arr.getVStorage (deleteIt);
    switch (dtype) {
    case TpBool:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecBoolV);
      break;
    case TpUChar:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecuCharV);
      break;
    case TpShort:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecShortV);
      break;
    case TpUShort:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecuShortV);
      break;
    case TpInt:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecIntV);
      break;
    case TpUInt:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecuIntV);
      break;
    case TpInt64:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecInt64V);
      break;
    case TpFloat:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecfloatV);
      break;
    case TpDouble:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecdoubleV);
      break;
    case TpComplex:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecComplexV);
      break;
    case TpDComplex:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecDComplexV);
      break;
    case TpString:
      getSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::getVecStringV);
      break;
    default:
      const void* cvalue = value;
      arr.freeVStorage (cvalue, deleteIt);
      throw DataManInternalError("Unhandled data type in StIndArray::getSliceV");
      break;
    }
    arr.putVStorage (value, deleteIt);
}

void StIndArray::putSliceV (StManArrayFile& ios, const Slicer& ns,
                            const ArrayBase& arr, DataType dtype)
{
    Bool deleteIt;
    const void* value = arr.getVStorage (deleteIt);
    switch (dtype) {
    case TpBool:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecBoolV);
      break;
    case TpUChar:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecuCharV);
      break;
    case TpShort:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecShortV);
      break;
    case TpUShort:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecuShortV);
      break;
    case TpInt:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecIntV);
      break;
    case TpUInt:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecuIntV);
      break;
    case TpInt64:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecInt64V);
      break;
    case TpFloat:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecfloatV);
      break;
    case TpDouble:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecdoubleV);
      break;
    case TpComplex:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecComplexV);
      break;
    case TpDComplex:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecDComplexV);
      break;
    case TpString:
      putSliceData (ios, ns, value, arr.shape(),
                    &StIndArray::putVecStringV);
      break;
    default:
      arr.freeVStorage (value, deleteIt);
      throw DataManInternalError("Unhandled data type in StIndArray::putSliceV");
      break;
    }
    arr.freeVStorage (value, deleteIt);
}

void StIndArray::getVecfloatV (StManArrayFile& ios,
			       Int64 fileOffset,
			       uInt64 start, uInt64 leng, uInt64 inc,
			       uInt64 valInx, void* value)
{
    float* valp = (float*)value + valInx;
    if (inc == 1) {
	ios.get (fileOffset, start, leng, valp);
    }else{
	while (leng-- > 0) {
	    ios.get (fileOffset, start, 1, valp++);
	    start += inc;
	}
    }
}
void StIndArray::getSliceData (StManArrayFile& ios, const Slicer& ns,
      void* value, const IPosition& userShape,
      void (*getVec) (StManArrayFile&, Int64, uInt64, uInt64, uInt64, uInt64, void*))
{
    //# Check if the shape of the slice and user array match.
    uInt ndim = ns.ndim();
    IPosition blc(ndim), trc(ndim), inc(ndim), shape(ndim);
    shape = ns.inferShapeFromSource (shape_p, blc,trc,inc);
    checkShape (userShape, shape);
    //# We'll get a vector at the time; get its length.
    //# Get the offset of the array in the file.
    //# If the array is 1-dim, we can just use the vector get.
    uInt64 leng = shape(0);
    Int64 fileOffset = fileOffset_p + arrOffset_p;
    if (ndim == 1) {
	getVec (ios, fileOffset, blc(0), leng, inc(0), 0, value);
    }else{
	//# Loop through the slice a vector at a time.
	ArrayPositionIterator iter(shape, 1);
	IPosition pos(ndim);
	uInt64 count=0;
	while (! iter.pastEnd()) {
	    //# Get the iterator position in the slice and transform
	    //# that to the file-offset for the corresponding part in
	    //# the table array.
	    pos = iter.pos();
	    uInt64 offset = 0;
	    for (uInt i=ndim-1; i>0; i--) {
		offset += blc(i) + pos(i) * inc(i);
		offset *= shape_p(i-1);
	    }
	    offset += blc(0);
	    getVec (ios, fileOffset,
		    offset, leng, inc(0), count, value);
	    count += leng;
	    iter.next();
	}
    }
}

void StIndArray::putVecfloatV (StManArrayFile& ios,
			       Int64 fileOffset,
			       uInt64 start, uInt64 leng, uInt64 inc,
			       uInt64 valInx, const void* value)
{
    float* valp = (float*)value + valInx;
    if (inc == 1) {
	ios.put (fileOffset, start, leng, valp);
    }else{
	while (leng-- > 0) {
	    ios.put (fileOffset, start, 1, valp++);
	    start += inc;
	}
    }
}
//# putSliceData works similar to getSliceData.
void StIndArray::putSliceData (StManArrayFile& ios, const Slicer& ns,
	const void* value, const IPosition& userShape,
        void (*putVec) (StManArrayFile&, Int64, uInt64, uInt64, uInt64, uInt64,
			const void*))
{
    uInt ndim = ns.ndim();
    IPosition blc(ndim), trc(ndim), inc(ndim), shape(ndim);
    shape = ns.inferShapeFromSource (shape_p, blc,trc,inc);
    checkShape (userShape, shape);
    uInt64 leng = shape(0);
    Int64 fileOffset = fileOffset_p + arrOffset_p;
    if (ndim == 1) {
	putVec (ios, fileOffset, blc(0), leng, inc(0), 0, value);
    }else{
	ArrayPositionIterator iter(shape, 1);
	IPosition pos(ndim);
	uInt64 count=0;
	while (! iter.pastEnd()) {
	    pos = iter.pos();
	    uInt64 offset = 0;
	    for (uInt i=ndim-1; i>0; i--) {
		offset += blc(i) + pos(i) * inc(i);
		offset *= shape_p(i-1);
	    }
	    offset += blc(0);
	    putVec (ios, fileOffset,
		    offset, leng, inc(0), count, value);
	    count += leng;
	    iter.next();
	}
    }
}
    

#define STINDARRAY_GETPUT(T,NM) \
void StIndArray::aips_name2(getVec,NM) \
                  (StManArrayFile& ios, Int64 fileOffset, \
		   uInt64 start, uInt64 leng, uInt64 inc, uInt64 valInx, \
                   void* value) \
{ \
    T* valp = (T*)value + valInx; \
    if (inc == 1) { \
	ios.get (fileOffset, start, leng, valp); \
    }else{ \
	while (leng-- > 0) { \
	    ios.get (fileOffset, start, 1, valp++); \
	    start += inc; \
	} \
    } \
} \
void StIndArray::aips_name2(putVec,NM) \
                  (StManArrayFile& ios, Int64 fileOffset, \
		   uInt64 start, uInt64 leng, uInt64 inc, uInt64 valInx, \
                   const void* value) \
{ \
    T* valp = (T*)value + valInx; \
    if (inc == 1) { \
	ios.put (fileOffset, start, leng, valp); \
    }else{ \
	while (leng-- > 0) { \
	    ios.put (fileOffset, start, 1, valp++); \
	    start += inc; \
	} \
    } \
}

STINDARRAY_GETPUT(Bool,BoolV)
STINDARRAY_GETPUT(uChar,uCharV)
STINDARRAY_GETPUT(Short,ShortV)
STINDARRAY_GETPUT(uShort,uShortV)
STINDARRAY_GETPUT(Int,IntV)
STINDARRAY_GETPUT(uInt,uIntV)
STINDARRAY_GETPUT(Int64,Int64V)
//#//STINDARRAY_GETPUT(float,floatV)   // done explicitly for debug purposes
STINDARRAY_GETPUT(double,doubleV)
STINDARRAY_GETPUT(Complex,ComplexV)
STINDARRAY_GETPUT(DComplex,DComplexV)
STINDARRAY_GETPUT(String,StringV)

} //# NAMESPACE CASACORE - END

