//# ArrColData.h: Access to a table column containing arrays
//# Copyright (C) 1994,1995,1996,1998,1999
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

#ifndef TABLES_ARRCOLDATA_H
#define TABLES_ARRCOLDATA_H

//# Includes
#include <casacore/casa/aips.h>
#include <casacore/tables/Tables/PlainColumn.h>
#include <casacore/casa/Arrays/IPosition.h>

namespace casacore { //# NAMESPACE CASACORE - BEGIN

//# Forward Declarations
class ColumnSet;
class ArrayColumnDescBase;
class AipsIO;


// <summary>
// Access to a table column containing arrays
// </summary>

// <use visibility=local>

// <reviewed reviewer="Gareth Hunt" date="94Nov17" tests="">
// </reviewed>

// <prerequisite>
//# Classes you should understand before using this one.
//   <li> PlainColumn
//   <li> ArrayColumnDesc
//   <li> Table
// </prerequisite>

// <etymology>
// ArrayColumnData represents a table column containing array data.
// </etymology>

// <synopsis> 
// The class ArrayColumnData is derived from PlainColumn.
// It implements the virtual functions accessing a table column
// containing arrays with an arbitrary data type.
// Both direct and indirect arrays are supported.
//
// It is possible to access an array or a subsection of it in an
// individual cell (i.e. table row) or in the entire column.
// The functions accessing the entire column are implemented by
// looping over the individual cells.
//
// The main task of this class is to communicate with the data manager
// column object. This consists of:
// <ul>
//  <li> Binding itself to a data manager.
//  <li> Letting the data manager create its column object and
//         setting the shape for direct arrays.
//  <li> Closing the data manager column object (in putFileDerived).
//  <li> Reconstructing the data manager object for an existing table
//         (in getFileDerived).
//  <li> Transferring get/put calls to the data manager column object.
// </ul>
//
// The class is hidden from the user by the envelope class ArrayColumn.
// It used directly, it should be done with care. It assumes that the
// arrays in the various get and put functions have the correct length.
// ArrayColumn does that check.
// </synopsis> 

// <todo asof="$DATE:$">
//# A List of bugs, limitations, extensions or planned refinements.
//   <li> support tiling
// </todo>


class ArrayColumnData : public PlainColumn
{
public:

    // Construct an array column object from the given description
    // in the given column set.
    // This constructor is used by ArrayColumnDescBase::makeColumn.
    ArrayColumnData (const ArrayColumnDescBase*, ColumnSet*);

    ~ArrayColumnData();

    // Copy constructor cannot be used.
    ArrayColumnData (const ArrayColumnData&) = delete;

    // Assignment cannot be used.
    ArrayColumnData& operator= (const ArrayColumnData&) = delete;

    // Ask the data manager if the shape of an existing array can be changed.
    virtual Bool canChangeShape() const;

    // Initialize the rows from startRownr till endRownr (inclusive)
    // with the default value defined in the column description (if defined).
    void initialize (rownr_t startRownr, rownr_t endRownr);

    // Get the global #dimensions of an array (ie. for all rows).
    uInt ndimColumn() const;

    // Get the global shape of an array (ie. for all rows).
    IPosition shapeColumn() const;

    // Set shape of all arrays in the column.
    // It can only be used for direct arrays.
    void setShapeColumn (const IPosition& shape);

    // Get the #dimensions of an array in a particular cell.
    // If the cell does not contain an array, 0 is returned.
    uInt ndim (rownr_t rownr) const;

    // Get the shape of an array in a particular cell.
    // If the cell does not contain an array, an empty IPosition is returned.
    IPosition shape(rownr_t rownr) const;

    // Get the tile shape of an array in a particular cell.
    // If the cell does not contain an array, an empty IPosition is returned.
    IPosition tileShape(rownr_t rownr) const;

    // Set dimensions of array in a particular cell.
    // <group>
    void setShape (rownr_t rownr, const IPosition& shape);
    // The shape of tiles in the array can also be defined.
    void setShape (rownr_t rownr, const IPosition& shape,
		   const IPosition& tileShape);
    // </group>

    // Test if the given cell contains an array.
    Bool isDefined (rownr_t rownr) const;

    // Get the array from a particular cell.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void getArray (rownr_t rownr, ArrayBase& arrayPtr) const;

    // Get a slice of an N-dimensional array in a particular cell.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void getSlice (rownr_t rownr, const Slicer&, ArrayBase& arrayPtr) const;

    // Get the array of all values in a column.
    // If the column contains n-dim arrays, the resulting array is (n+1)-dim.
    // The arrays in the column have to have the same shape in all cells.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void getArrayColumn (ArrayBase& arrayPtr) const;

    // Get the array of some values in a column.
    // If the column contains n-dim arrays, the resulting array is (n+1)-dim.
    // The arrays in the column have to have the same shape in all cells.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void getArrayColumnCells (const RefRows& rownrs, ArrayBase& arrayPtr) const;

    // Get subsections from all arrays in the column.
    // If the column contains n-dim arrays, the resulting array is (n+1)-dim.
    // The arrays in the column have to have the same shape in all cells.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void getColumnSlice (const Slicer&, ArrayBase& arrayPtr) const;

    // Get subsections from some arrays in the column.
    // If the column contains n-dim arrays, the resulting array is (n+1)-dim.
    // The arrays in the column have to have the same shape in all cells.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void getColumnSliceCells (const RefRows& rownrs, const Slicer&,
			      ArrayBase& arrayPtr) const;

    // Put the value in a particular cell.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void putArray (rownr_t rownr, const ArrayBase& arrayPtr);

    // Put a slice of an N-dimensional array in a particular cell.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void putSlice (rownr_t rownr, const Slicer&, const ArrayBase& arrayPtr);

    // Put the array of all values in the column.
    // If the column contains n-dim arrays, the source array is (n+1)-dim.
    // The arrays in the column have to have the same shape in all cells.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void putArrayColumn (const ArrayBase& arrayPtr);

    // Put the array of some values in the column.
    // If the column contains n-dim arrays, the source array is (n+1)-dim.
    // The arrays in the column have to have the same shape in all cells.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void putArrayColumnCells (const RefRows& rownrs, const ArrayBase& arrayPtr);

    // Put into subsections of all table arrays in the column.
    // If the column contains n-dim arrays, the source array is (n+1)-dim.
    // The arrays in the column have to have the same shape in all cells.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void putColumnSlice (const Slicer&, const ArrayBase& arrayPtr);

    // Put into subsections of some table arrays in the column.
    // If the column contains n-dim arrays, the source array is (n+1)-dim.
    // The arrays in the column have to have the same shape in all cells.
    // The length of the array given by ArrayBase must match
    // the actual length. This is checked by ArrayColumn.
    void putColumnSliceCells (const RefRows& rownrs, const Slicer&,
			      const ArrayBase& arrayPtr);

    // Create a data manager column object for this column.
    void createDataManagerColumn();


private:
    // Is the shape for all arrays in the columns defined.
    Bool shapeColDef_p;
    // Shape for all arrays in the column.
    IPosition shapeCol_p;
    // Does the length of a string has to be checked?
    Bool checkValueLength_p;
    

    // Check if the shape of an array can be set and if it is set
    // correctly (i.e. if matching possible #dim in column description).
    void checkShape (const IPosition& shape) const;

    // Write the column data.
    // The control information is written into the given AipsIO object,
    // while the data is written/flushed by the data manager.
    void putFileDerived (AipsIO&);

    // Read the column data back.
    // The control information is read from the given AipsIO object.
    // This is used to bind the column to the appropriate data manager.
    // Thereafter the data manager gets opened.
    void getFileDerived (AipsIO&, const ColumnSet&);
};




} //# NAMESPACE CASACORE - END

#endif
