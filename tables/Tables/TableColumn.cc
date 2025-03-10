//# TableColumn.cc: Const access to a table column
//# Copyright (C) 1994,1995,1996,1997,1999,2001,2002
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

#include <casacore/tables/Tables/TableColumn.h>
#include <casacore/tables/Tables/Table.h>
#include <casacore/tables/Tables/TableError.h>
#include <casacore/casa/Arrays/Array.h>
#include <casacore/casa/Containers/ValueHolder.h>


namespace casacore { //# NAMESPACE CASACORE - BEGIN

TableColumn::TableColumn ()
: baseTabPtr_p     (0),
  baseColPtr_p     (0),
  colCachePtr_p    (0),
  canChangeShape_p (False),
  isColWritable_p  (False)
{}

TableColumn::TableColumn (const Table& tab, const String& columnName)
: baseColPtr_p(0)
{
    //# Get base table and base column.
    baseTabPtr_p = tab.baseTablePtr();
    if (baseTabPtr_p == 0) {
	throw (TableInvOper ("TableColumn: no table in Table object"));
    }
    baseColPtr_p  = baseTabPtr_p->getColumn (columnName);
    colCachePtr_p = &(baseColPtr_p->columnCache());
    canChangeShape_p = baseColPtr_p->canChangeShape();
    isColWritable_p  = baseColPtr_p->isWritable();
}

TableColumn::TableColumn (const Table& tab, uInt columnIndex)
: baseColPtr_p(0)
{
    //# Get base table and base column.
    baseTabPtr_p = tab.baseTablePtr();
    if (baseTabPtr_p == 0) {
	throw (TableInvOper ("TableColumn: no table in Table object"));
    }
    baseColPtr_p  = baseTabPtr_p->getColumn (columnIndex);
    colCachePtr_p = &(baseColPtr_p->columnCache());
    canChangeShape_p = baseColPtr_p->canChangeShape();
    isColWritable_p  = baseColPtr_p->isWritable();
}

TableColumn::TableColumn (const TableColumn& that)
: baseTabPtr_p     (that.baseTabPtr_p),
  baseColPtr_p     (that.baseColPtr_p),
  colCachePtr_p    (that.colCachePtr_p),
  canChangeShape_p (that.canChangeShape_p),
  isColWritable_p  (that.isColWritable_p)
{}

TableColumn* TableColumn::clone() const
{
    return new TableColumn (*this);
}

TableColumn& TableColumn::operator= (const TableColumn& that)
{
    reference (that);
    return *this;
}

void TableColumn::reference (const TableColumn& that)
{
    baseTabPtr_p     = that.baseTabPtr_p;
    baseColPtr_p     = that.baseColPtr_p;
    colCachePtr_p    = that.colCachePtr_p;
    canChangeShape_p = that.canChangeShape_p;
    isColWritable_p  = that.isColWritable_p;
}

TableColumn::~TableColumn()
{}


void TableColumn::throwIfNull() const
{
    if (isNull()) {
	throw (TableInvOper ("TableColumn is null"));
    }
}


TableRecord& TableColumn::rwKeywordSet()
{
    if (! baseTabPtr_p->isWritable()) {
	throw (TableError ("TableColumn::rwKeywordSet cannot be used: table "
			   + baseTabPtr_p->tableName() + " is not writable"));
    }
    return baseColPtr_p->rwKeywordSet();
}


const ColumnDesc& TableColumn::columnDesc() const
    { return baseColPtr_p->columnDesc(); }

Table TableColumn::table() const
    { return Table (baseTabPtr_p); }


Bool TableColumn::asBool (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    Bool value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
uChar TableColumn::asuChar (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    uChar value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
Short TableColumn::asShort (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    Short value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
uShort TableColumn::asuShort (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    uShort value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
Int TableColumn::asInt (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    Int value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
uInt TableColumn::asuInt (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    uInt value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
Int64 TableColumn::asInt64 (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    Int64 value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
float TableColumn::asfloat (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    float value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
double TableColumn::asdouble (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    double value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
Complex TableColumn::asComplex (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    Complex value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
DComplex TableColumn::asDComplex (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    DComplex value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}
String TableColumn::asString (rownr_t rownr) const
{
    TABLECOLUMNCHECKROW(rownr); 
    String value;
    baseColPtr_p->getScalar (rownr, value);
    return value;
}


void TableColumn::put (rownr_t thisRownr, const TableColumn& that,
		       rownr_t thatRownr, Bool preserveTileShape)
{
  TABLECOLUMNCHECKROW(thisRownr);
  checkWritable();
  if (columnDesc().isScalar()) {
    switch (columnDesc().dataType()) {
    case TpBool:
      putScalar (thisRownr, that.asBool (thatRownr));
      break;
    case TpUChar:
      putScalar (thisRownr, that.asuChar (thatRownr));
      break;
    case TpShort:
      putScalar (thisRownr, that.asShort (thatRownr));
      break;
    case TpUShort:
      putScalar (thisRownr, that.asuShort (thatRownr));
      break;
    case TpInt:
      putScalar (thisRownr, that.asInt (thatRownr));
      break;
    case TpUInt:
      putScalar (thisRownr, that.asuInt (thatRownr));
      break;
    case TpInt64:
      putScalar (thisRownr, that.asInt64 (thatRownr));
      break;
    case TpFloat:
      putScalar (thisRownr, that.asfloat (thatRownr));
      break;
    case TpDouble:
      putScalar (thisRownr, that.asdouble (thatRownr));
      break;
    case TpComplex:
      putScalar (thisRownr, that.asComplex (thatRownr));
      break;
    case TpDComplex:
      putScalar (thisRownr, that.asDComplex (thatRownr));
      break;
    case TpString:
      putScalar (thisRownr, that.asString (thatRownr));
      break;
    default:
      throw (TableInvDT ("TableColumn::put; invalid type promotion"));
    }
  }else{
    if (! columnDesc().isArray()) {
      throw (TableInvDT ("TableColumn::put; no scalar or array"));
    }
    if (! that.columnDesc().isArray()) {
      throw (TableInvDT ("TableColumn::put; array types mismatch"));
    }
    if (that.isDefined (thatRownr)) {
      //#// If not defined, the this-value should be unset (if there is one).
      //#// However, this requires an undefine function, which is not there yet.
      //# Get the shape and define it for non-FixedShape arrays.
      //# Then get the data and put it depending on the type.
      IPosition shape = that.shape (thatRownr);
      if (preserveTileShape) {
        IPosition tileShape = that.tileShape (thatRownr);
        if (tileShape.empty()) {
          baseColPtr_p->setShape (thisRownr, shape);
        } else {
          baseColPtr_p->setShape (thisRownr, shape, tileShape);
        }
      } else if ((columnDesc().options() & ColumnDesc::FixedShape)
                 != ColumnDesc::FixedShape) {
        baseColPtr_p->setShape (thisRownr, shape);
      }
      ValueHolder vh;
      switch (that.columnDesc().dataType()) {
      case TpBool:
        {
          Array<Bool> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpUChar:
        {
          Array<uChar> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpShort:
        {
          Array<Short> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpUShort:
        {
          Array<uShort> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpInt:
        {
          Array<Int> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpUInt:
        {
          Array<uInt> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpInt64:
        {
          Array<Int64> array(shape);
          baseColPtr(that)->get (thatRownr, &array);
          vh = ValueHolder (array);
        }
        break;
      case TpFloat:
        {
          Array<float> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpDouble:
        {
          Array<double> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpComplex:
        {
          Array<Complex> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpDComplex:
        {
          Array<DComplex> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      case TpString:
        {
          Array<String> array(shape);
          baseColPtr(that)->getArray (thatRownr, array);
          vh = ValueHolder (array);
        }
        break;
      default:
        throw (TableInvDT ("TableColumn::put of that column"));
      }
      switch (columnDesc().dataType()) {
      case TpBool:
        {
          Array<Bool> arr (vh.asArrayBool());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpUChar:
        {
          Array<uChar> arr (vh.asArrayuChar());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpShort:
        {
          Array<Short> arr (vh.asArrayShort());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpUShort:
        {
          Array<uShort> arr (vh.asArrayuShort());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpInt:
        {
          Array<Int> arr (vh.asArrayInt());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpUInt:
        {
          Array<uInt> arr (vh.asArrayuInt());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpInt64:
        {
          Array<Int64> arr (vh.asArrayInt64());
          baseColPtr_p->put (thisRownr, &arr);
        }
        break;
      case TpFloat:
        {
          Array<Float> arr (vh.asArrayFloat());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpDouble:
        {
          Array<Double> arr (vh.asArrayDouble());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpComplex:
        {
          Array<Complex> arr (vh.asArrayComplex());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpDComplex:
        {
          Array<DComplex> arr (vh.asArrayDComplex());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      case TpString:
        {
          Array<String> arr (vh.asArrayString());
          baseColPtr_p->putArray (thisRownr, arr);
        }
        break;
      default:
        throw (TableInvDT ("TableColumn::put of this column"));
      }
    }
  }
}


//#// Currently this is a very dumb implementation.
//# It should check if types are equal and take advantage of that.
void TableColumn::putColumn (const TableColumn& that)
{
    checkWritable();
    rownr_t nrrow = nrow();
    if (nrrow != that.nrow()) {
	throw (TableConformanceError ("TableColumn::putColumn"));
    }
    for (rownr_t i=0; i<nrrow; i++) {
	put (i, that, i);
    }
}

void TableColumn::throwNotWritable() const
{
  throw TableError ("Column " + columnDesc().name() + " in table " +
                    baseTabPtr_p->tableName() + " is not writable");
}

Bool TableColumn::hasContent (rownr_t rownr) const
{
  Bool retval = !isNull() && isDefined(rownr);
  if (retval  &&  columnDesc().isArray()) {
    // The first cell seems to have something, but check for
    // degenerate Arrays.
    IPosition shp(shape(rownr));
    if (shp.empty()) {
      retval = False;
    } else {
      for (uInt i=0; i<shp.size(); ++i){
        if (shp[i] == 0) {
          retval = False;
          break;
        }
      }
    }
  }
  return retval;
}

} //# NAMESPACE CASACORE - END

