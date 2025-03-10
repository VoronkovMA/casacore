//# ScaColData.h: Access to a table column containing scalars
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

#ifndef TABLES_SCACOLDATA_H
#define TABLES_SCACOLDATA_H

//# Includes
#include <casacore/casa/aips.h>
#include <casacore/casa/Arrays/ArrayFwd.h>
#include <casacore/tables/Tables/PlainColumn.h>

namespace casacore { //# NAMESPACE CASACORE - BEGIN

//# Forward Declarations
class ColumnSet;
template<class T> class ScalarColumnDesc;
class AipsIO;

// <summary>
// Access to a table column containing scalars
// </summary>

// <use visibility=local>

// <reviewed reviewer="Gareth Hunt" date="94Nov17" tests="">
// </reviewed>

// <prerequisite>
//# Classes you should understand before using this one.
//   <li> PlainColumn
//   <li> ScalarColumnDesc
//   <li> Table
// </prerequisite>

// <etymology>
// ScalarColumnData represents a table column containing scalars.
// </etymology>

// <synopsis> 
// The class ScalarColumnData is derived from PlainColumn.
// It implements the virtual functions accessing a table column
// containing scalars with an arbitrary data type.
//
// It is possible to access an scalar an individual cell (i.e. table row)
// or in the entire column.
//
// The main task of this class is to communicate with the data manager
// column object. This consists of:
// <ul>
//  <li> Binding itself to a data manager.
//  <li> Letting the data manager create its column object.
//  <li> Closing the data manager column object (in putFileDerived).
//  <li> Reconstructing the data manager object for an existing table
//         (in getFileDerived).
//  <li> Transferring get/put calls to the data manager column object.
// </ul>
//
// The class is hidden from the user by the envelope class ScalarColumn.
// It used directly, it should be done with care. It assumes that the
// arrays in the various get and put functions have the correct length.
// ScalarColumn does that check.
// </synopsis>

// <todo asof="$DATE:$">
//# A List of bugs, limitations, extensions or planned refinements.
// </todo>


template<class T>
class ScalarColumnData : public PlainColumn
{
public:

    // Construct a scalar column object from the given description
    // in the given column set.
    // This constructor is used by ScalarColumnDesc::makeColumn.
    ScalarColumnData (const ScalarColumnDesc<T>*, ColumnSet*);

    ~ScalarColumnData();

    // Initialize the rows from startRownr till endRownr (inclusive)
    // with the default value defined in the column description.
    virtual void initialize (rownr_t startRownr, rownr_t endRownr);

    // Test if the given cell contains a defined value.
    virtual Bool isDefined (rownr_t rownr) const;

    // Get the value from a particular cell.
    virtual void get (rownr_t rownr, void*) const;

    // Get the array of all values in the column.
    // The length of the buffer pointed to by dataPtr must match
    // the actual length. This is checked by ScalarColumn.
    virtual void getScalarColumn (ArrayBase& dataPtr) const;

    // Get the array of some values in the column (on behalf of RefColumn).
    // The length of the buffer pointed to by dataPtr must match
    // the actual length. This is checked by ScalarColumn.
    virtual void getScalarColumnCells (const RefRows& rownrs,
                                       ArrayBase& dataPtr) const;

    // Put the value in a particular cell.
    // The length of the buffer pointed to by dataPtr must match
    // the actual length. This is checked by ScalarColumn.
    virtual void put (rownr_t rownr, const void* dataPtr);

    // Put the array of all values in the column.
    // The length of the buffer pointed to by dataPtr must match
    // the actual length. This is checked by ScalarColumn.
    virtual void putScalarColumn (const ArrayBase& dataPtr);

    // Put the array of some values in the column (on behalf on RefColumn).
    // The length of the buffer pointed to by dataPtr must match
    // the actual length. This is checked by ScalarColumn.
    virtual void putScalarColumnCells (const RefRows& rownrs,
                                       const ArrayBase& dataPtr);

    // Add the sort key to the Sort object on behalf of the Table sort function.
    // <group>
    virtual void makeSortKey (Sort&, std::shared_ptr<BaseCompare>& cmpFunc, Int order,
                              std::shared_ptr<ArrayBase>& dataSave);
    // Do it only for the given row numbers.
    void makeRefSortKey (Sort&, std::shared_ptr<BaseCompare>& cmpFunc, Int order,
			 const Vector<rownr_t>& rownrs,
                         std::shared_ptr<ArrayBase>& dataSave);
    // </group>

    // Allocate value buffers for the table iterator.
    // Also get a comparison object if undefined.
    // The function freeIterBuf must be called to free the buffers.
    void allocIterBuf (void*& lastVal, void*& curVal,
		       std::shared_ptr<BaseCompare>& cmpObj);

    // Free the value buffers allocated by allocIterBuf.
    void freeIterBuf (void*& lastVal, void*& curVal);

    // Create a data manager column object for this column.
    void createDataManagerColumn();


private:
    // Pointer to column description.
    const ScalarColumnDesc<T>* scaDescPtr_p;
    // Undefined value can exist?
    Bool undefFlag_p;
    // Undefined value.
    T undefVal_p;
    

    // Copy constructor cannot be used.
    ScalarColumnData (const ScalarColumnData<T>&);

    // Assignment cannot be used.
    ScalarColumnData<T>& operator= (const ScalarColumnData<T>&);

    // Write the column data.
    // The control information is written into the given AipsIO object,
    // while the data is written/flushed by the data manager.
    void putFileDerived (AipsIO&);

    // Read the column data back.
    // The control information is read from the given AipsIO object.
    // This is used to bind the column to the appropriate data manager.
    // Thereafter the data manager gets opened.
    void getFileDerived (AipsIO&, const ColumnSet&);

    // Fill in the sort key on behalf of the Table sort function.
    // The pointer to the data (which can be allocated on the heap)
    // is stored in dataPtr. This is used by freeSortKey to release it.
    // It checks if a compare function is given when needed.
    // <thrown>
    //   <li> TableInvSort
    // </thrown>
    void fillSortKey (const Vector<T>* dataPtr, Sort&,
		      std::shared_ptr<BaseCompare>& cmpObj, Int order);
};



} //# NAMESPACE CASACORE - END

#ifndef CASACORE_NO_AUTO_TEMPLATES
#include <casacore/tables/Tables/ScaColData.tcc>
#endif //# CASACORE_NO_AUTO_TEMPLATES
#endif
