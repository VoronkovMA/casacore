//# MSPolColumns.h: provides easy access to MSPolarization columns
//# Copyright (C) 1999,2000
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

#ifndef MS_MSPOLCOLUMNS_H
#define MS_MSPOLCOLUMNS_H

#include <casacore/casa/aips.h>
#include <casacore/casa/Arrays/ArrayFwd.h>
#include <casacore/tables/Tables/ArrayColumn.h>
#include <casacore/tables/Tables/ScalarColumn.h>
#include <casacore/measures/Measures/Stokes.h>

namespace casacore { //# NAMESPACE CASACORE - BEGIN

class MSPolarization;

// <summary>
// A class to provide easy access to MSPolarization columns
// </summary>

// <use visibility=export>

// <reviewed reviewer="Bob Garwood" date="1997/02/01" tests="" demos="">
// </reviewed>

// <prerequisite>
//   <li> MSPolarization
//   <li> ArrayColumn
//   <li> ScalarColumn
// </prerequisite>
//
// <etymology>
// MSPolarizationColumns stands for MeasurementSet Polarization Table columns.
// </etymology>
//
// <synopsis>
// This class provides access to the columns in the MSPolarization Table,
// it does the declaration of all the Scalar and ArrayColumns with the
// correct types, so the application programmer doesn't have to
// worry about getting those right. There is an access function
// for every predefined column. Access to non-predefined columns will still
// have to be done with explicit declarations.
// See <linkto class=MSColumns> MSColumns</linkto> for an example.
// </synopsis>
//
// <motivation>
// See <linkto class=MSColumns> MSColumns</linkto> for the motivation.
// </motivation>

class MSPolarizationColumns
{
public:
  // Create a columns object that accesses the data in the specified Table
  MSPolarizationColumns(const MSPolarization& msPolarization);
  
  // The destructor does nothing special
  ~MSPolarizationColumns();
  
  // Access to required columns
  // <group>
  ArrayColumn<Int>& corrProduct() {return corrProduct_p;}
  ArrayColumn<Int>& corrType() {return corrType_p;}
  ScalarColumn<Bool>& flagRow() {return flagRow_p;}
  ScalarColumn<Int>& numCorr() {return numCorr_p;}
  // </group>
  
  // Const access to required columns
  // <group>
  const ArrayColumn<Int>& corrProduct() const {return corrProduct_p;}
  const ArrayColumn<Int>& corrType() const {return corrType_p;}
  const ScalarColumn<Bool>& flagRow() const {return flagRow_p;}
  const ScalarColumn<Int>& numCorr() const {return numCorr_p;}
  // </group>

  // Convenience function that returns the number of rows in any of the columns
  rownr_t nrow() const {return corrProduct_p.nrow();}

  // returns the last row that contains the an entry in the CORR_TYPE column
  // that matches, in length and value, the supplied corrType Vector.  Returns
  // -1 if no match could be found. Flagged rows can never match. If tryRow is
  // non-negative, then that row is tested to see if it matches before any
  // others are tested. Setting tryRow to a positive value greater than the
  // table length will throw an exception (AipsError), when compiled in debug
  // mode.
  Int64 match(const Vector<Stokes::StokesTypes>& polType, Int64 tryRow=-1);

protected:
  //# default constructor creates a object that is not usable. Use the attach
  //# function correct this.
  MSPolarizationColumns();

  //# attach this object to the supplied table.
  void attach(const MSPolarization& msPolarization);

private:
  //# Make the assignment operator and the copy constructor private to prevent
  //# any compiler generated one from being used.
  MSPolarizationColumns(const MSPolarizationColumns&);
  MSPolarizationColumns& operator=(const MSPolarizationColumns&);

  //# Functions which check the supplied values against the relevant column and
  //# the specified row.
  Bool matchCorrType(rownr_t row, const Vector<Int>& polType) const;
  Bool matchCorrProduct(rownr_t row, const Matrix<Int>& polProduct) const;

  //# required columns
  ArrayColumn<Int> corrProduct_p;
  ArrayColumn<Int> corrType_p;
  ScalarColumn<Bool> flagRow_p;
  ScalarColumn<Int> numCorr_p;
};

//# Define the RO version for backward compatibility.
typedef MSPolarizationColumns ROMSPolarizationColumns;

} //# NAMESPACE CASACORE - END

#endif
