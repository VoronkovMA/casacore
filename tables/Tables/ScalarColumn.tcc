//# ScalarColumn.cc: Access to a scalar table column with arbitrary data type
//# Copyright (C) 1994,1995,1996,1997,1998,1999
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

#ifndef TABLES_SCALARCOLUMN_TCC
#define TABLES_SCALARCOLUMN_TCC

#include <casacore/tables/Tables/ScalarColumn.h>
#include <casacore/tables/Tables/Table.h>
#include <casacore/tables/Tables/BaseColumn.h>
#include <casacore/tables/Tables/RefRows.h>
#include <casacore/casa/Arrays/Vector.h>
#include <casacore/casa/Arrays/Slicer.h>
#include <casacore/casa/Utilities/ValTypeId.h>
#include <casacore/casa/BasicSL/String.h>
#include <casacore/tables/Tables/TableError.h>


namespace casacore { //# NAMESPACE CASACORE - BEGIN

template<class T>
ScalarColumn<T>::ScalarColumn()
: TableColumn()
{}

template<class T>
ScalarColumn<T>::ScalarColumn (const Table& tab,
                               const String& columnName)
: TableColumn (tab, columnName)
{
    checkDataType();
}

template<class T>
ScalarColumn<T>::ScalarColumn (const TableColumn& column)
: TableColumn (column)
{
    checkDataType();
}

template<class T>
ScalarColumn<T>::ScalarColumn (const ScalarColumn<T>& that)
: TableColumn (that)
{}

template<class T>
TableColumn* ScalarColumn<T>::clone() const
{
    return new ScalarColumn<T> (*this);
}

template<class T>
ScalarColumn<T>& ScalarColumn<T>::operator= (const ScalarColumn<T>& that)
{
  reference (that);
  return (*this);
}

template<class T>
void ScalarColumn<T>::reference (const ScalarColumn<T>& that)
{
    if (this != &that) {
        TableColumn::reference (that);
    }
}

template<class T>
ScalarColumn<T>::~ScalarColumn()
{}

template<class T>
void ScalarColumn<T>::checkDataType() const
{
    //# Check if the data type matches.
    const ColumnDesc& cd = baseColPtr_p->columnDesc();
    DataType dtype = cd.dataType();
    if (dtype != ValType::getType(static_cast<T*>(0))  ||  !cd.isScalar()) {
	throw (TableInvDT (" in ScalarColumn ctor for column " + cd.name()));
    }
    if (dtype == TpOther) {
	if (cd.dataTypeId() != valDataTypeId(static_cast<T*>(0))) {
	    throw (TableInvDT (" in ScalarColumn ctor for column "
			       + cd.name() + "; using data type id "
			       + valDataTypeId(static_cast<T*>(0))
			       + ", expected " + cd.dataTypeId()));
	}
    }
}


template<class T>
Vector<T> ScalarColumn<T>::getColumn() const
{
    Vector<T> vec;
    getColumn (vec);
    return vec;
}

template<class T>
void ScalarColumn<T>::getColumn (Vector<T>& vec, Bool resize) const
{
    rownr_t nrrow = nrow();
    //# Resize the vector if empty; otherwise check its length.
    if (vec.nelements() != nrrow) {
	if (resize  ||  vec.nelements() == 0) {
	    vec.resize (nrrow);
	}else{
	    throw (TableConformanceError("ScalarColumn::getColumn"));
	}
    }
    // Get the column.
    baseColPtr_p->getScalarColumn (vec);
}


template<class T>
Vector<T> ScalarColumn<T>::getColumnRange (const Slicer& rowRange) const
{
    Vector<T> vec;
    getColumnRange (rowRange, vec);
    return vec;
}

template<class T>
void ScalarColumn<T>::getColumnRange (const Slicer& rowRange,
                                      Vector<T>& vec, Bool resize) const
{
    rownr_t nrrow = nrow();
    IPosition shp, blc, trc, inc;
    shp = rowRange.inferShapeFromSource (IPosition(1,nrrow), blc, trc, inc);
    //# When the entire column is accessed, use that function.
    if (blc(0) == 0  &&  shp(0) == Int(nrrow)  &&  inc(0) == 1) {
	getColumn (vec, resize);
    } else {
	getColumnCells (RefRows(blc(0), trc(0), inc(0)), vec, resize);
    }
}

template<class T>
Vector<T> ScalarColumn<T>::getColumnCells (const RefRows& rownrs) const
{
    Vector<T> vec;
    getColumnCells (rownrs, vec);
    return vec;
}

template<class T>
void ScalarColumn<T>::getColumnCells (const RefRows& rownrs,
                                      Vector<T>& vec, Bool resize) const
{
    //# Resize the vector if needed; otherwise check its length.
    rownr_t nrrow = rownrs.nrow();
    if (vec.nelements() != nrrow) {
	if (resize  ||  vec.nelements() == 0) {
	    vec.resize (nrrow);
	}else{
	    throw (TableConformanceError("ScalarColumn::getColumnCells"));
	}
    }
    baseColPtr_p->getScalarColumnCells (rownrs, vec);
}


template<class T>
void ScalarColumn<T>::put (rownr_t thisRownr, const ScalarColumn<T>& that,
			   rownr_t thatRownr)
{
    put (thisRownr, that(thatRownr));
}

template<class T>
void ScalarColumn<T>::put (rownr_t thisRownr, const TableColumn& that,
			   rownr_t thatRownr, Bool)
{
    T value;
    that.getScalarValue (thatRownr, &value, columnDesc().dataTypeId());
    put (thisRownr, value);
}

template<class T>
void ScalarColumn<T>::putColumn (const Vector<T>& vec)
{
    checkWritable();
    rownr_t nrrow = nrow();
    //# Check the vector length.
    if (vec.nelements() != nrrow) {
	throw (TableConformanceError("ScalarColumn::putColumn(Vector&)"));
    }
    // Put the column.
    baseColPtr_p->putScalarColumn (vec);
}

template<class T>
void ScalarColumn<T>::putColumnRange (const Slicer& rowRange,
				      const Vector<T>& vec)
{
    rownr_t nrrow = nrow();
    IPosition shp, blc, trc, inc;
    shp = rowRange.inferShapeFromSource (IPosition(1,nrrow), blc, trc, inc);
    //# When the entire column is accessed, use that function.
    if (blc(0) == 0  &&  shp(0) == Int(nrrow)  &&  inc(0) == 1) {
	putColumn (vec);
    } else {
	putColumnCells (RefRows(blc(0), trc(0), inc(0)), vec);
    }
}

template<class T>
void ScalarColumn<T>::putColumnCells (const RefRows& rownrs,
				      const Vector<T>& vec)
{
    checkWritable();
    //# Check the vector length.
    rownr_t nrrow = rownrs.nrow();
    if (vec.nelements() != nrrow) {
	throw (TableConformanceError("ScalarColumn::putColumnCells"));
    }
    baseColPtr_p->putScalarColumnCells (rownrs, vec);
}


//#// This is a very simple implementation.
//#// Ultimately this must be done more directly via the data manager.
template<class T>
void ScalarColumn<T>::fillColumn (const T& value)
{
    rownr_t nrrow = nrow();
    for (rownr_t i=0; i<nrrow; i++) {
	put (i, value);
    }
}

template<class T>
void ScalarColumn<T>::putColumn (const ScalarColumn<T>& that)
{
    //# Check the column lengths.
    rownr_t nrrow = nrow();
    if (nrrow != that.nrow()) {
	throw (TableConformanceError ("ScalarColumn<T>::putColumn"));
    }
    for (rownr_t i=0; i<nrrow; i++) {
	put (i, that, i);
    }
}

} //# NAMESPACE CASACORE - END

#endif
