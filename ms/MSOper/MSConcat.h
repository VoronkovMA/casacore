//# MSConcat.h: A class for concatenating MeasurementSets.
//# Copyright (C) 2000,2002,2003
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

#ifndef MS_MSCONCAT_H
#define MS_MSCONCAT_H

#include <casacore/casa/aips.h>
#include <casacore/ms/MeasurementSets/MSColumns.h>
#include <casacore/ms/MeasurementSets/MeasurementSet.h>
#include <casacore/casa/Arrays/IPosition.h>

namespace casacore { //# NAMESPACE CASACORE - BEGIN

class TableDesc;
class MSMainColumns;
class MSDataDescColumns;
class MSSpWindowColumns;
class MSPolarizationColumns;
class MSAntenna;
class MSDataDescription;
class MSFeed;
class MSField;
class MSPolarization;
class MSSpectralWindow;
template <class T> class Block;

// <summary>A class with functions for concatenating MeasurementSets</summary>

// <use visibility=export>

// <reviewed reviewer="" date="yyyy/mm/dd" tests="" demos="">
// </reviewed>

// <prerequisite>
// </prerequisite>
//
// <etymology>
// </etymology>
//
// <synopsis>
// </synopsis>
//
// <example>
// </example>
//
// <motivation>
// </motivation>
//
// <todo asof="yyyy/mm/dd">
// </todo>

class MSConcat: public MSColumns
{
public:
  MSConcat(MeasurementSet& ms);

  void virtualconcat(MeasurementSet& otherMS, 
		     const Bool checkShapeAndCateg=True,
		     const String& obsidAndProcAndScanTableName="");

  void concatenate(const MeasurementSet& otherMS,
		   const uInt handling=0,   //# 0 (default): complete concat of all tables
                                            //# 1 : don't concatenate the MAIN table
                                            //# 2 : don't concatenate the POINTING table
                                            //# 3 : neither concat MAIN nor POINTING table
                   const String& destMSName=""); //# support for virtual concat

  void setTolerance(Quantum<Double>& freqTol, Quantum<Double>& dirTol); 
  void setWeightScale(const Float weightScale); 
  void setRespectForFieldName(const Bool respectFieldName); //# If True, fields of same direction are not merged
                                                            //# if their name is different

private:
  MSConcat();
  static IPosition isFixedShape(const TableDesc& td);
  static IPosition getShape(const MSDataDescColumns& ddCols, 
			    const MSSpWindowColumns& spwCols, 
			    const MSPolarizationColumns& polCols, 
			    uInt whichShape);
  void checkShape(const IPosition& otherShape) const;
  void checkCategories(const MSMainColumns& otherCols) const;
  Bool checkEphIdInField(const MSFieldColumns& otherFldCol) const;
  Bool copyPointing(const MSPointing& otherPoint, const Block<uInt>& newAntIndices);
  Bool copyPointingB(MSPointing& otherPoint, const Block<uInt>& newAntIndices);
  Bool copySysCal(const MSSysCal& otherSysCal, const Block<uInt>& newAndIndices);
  Bool copyWeather(const MSWeather& otherWeather, const Block<uInt>& newAndIndices);
  Bool copyGainCurve(const MeasurementSet& otherMS, const Block<uInt>& newAndIndices);
  Bool copyPhaseCal(const MeasurementSet& otherMS, const Block<uInt>& newAndIndices);
  Int copyObservation(const MSObservation& otherObs, const Bool remRedunObsId=True);
                             //# by default remove redundant observation table rows
  Int copyProcessor(const MSProcessor& otherObs, const Bool remRedunProcId=True);
                             //# by default remove redundant processor table rows
  Block<uInt> copyAntennaAndFeed(const MSAntenna& otherAnt,
				 const MSFeed& otherFeed);
  Block<uInt> copyState(const MSState& otherState);
  Block<uInt> copyField(const MeasurementSet& otherms);
  Block<uInt> copySpwAndPol(const MSSpectralWindow& otherSpw,
			    const MSPolarization& otherPol,
			    const MSDataDescription& otherDD);
  Bool copySource(const MeasurementSet& otherms);
  Bool updateSource();
  Bool updateSource2();
  Bool sourceRowsEquivalent(const MSSourceColumns& sourceCol, 
			    const rownr_t& rowi, const rownr_t& rowj,
			    const Bool dontTestDirection=False,
			    const Bool dontTestTransAndRest=False);

  Bool obsRowsEquivalent(const MSObservationColumns& obsCol, 
			 const rownr_t& rowi, const rownr_t& rowj);

  Bool procRowsEquivalent(const MSProcessorColumns& procCol, 
			 const uInt& rowi, const uInt& rowj);


  void updateModelDataKeywords(MeasurementSet& ms);

  MeasurementSet itsMS;
  IPosition itsFixedShape;
  Quantum<Double> itsFreqTol;
  Quantum<Double> itsDirTol;
  Float itsWeightScale;
  Bool itsRespectForFieldName;
  Vector<Bool> itsChanReversed;
  std::map <Int, Int> newSourceIndex_p;
  std::map <Int, Int> newSourceIndex2_p;
  std::map <Int, Int> newSPWIndex_p;
  std::map <Int, Int> newObsIndexA_p;
  std::map <Int, Int> newObsIndexB_p;
  std::map <Int, Int> otherObsIdsWithCounterpart_p;
  std::map <Int, Int> newProcIndexA_p;
  std::map <Int, Int> newProcIndexB_p;
  std::map <Int, Int> solSystObjects_p;

  Bool doSource_p;
  Bool doSource2_p;
  Bool doSPW_p;
  Bool doObsA_p;
  Bool doObsB_p;
  Bool doProcA_p;
  Bool doProcB_p;

};

template<class T>
Bool areEQ(const ScalarColumn<T>& col, rownr_t row_i, rownr_t row_j) 
{
  T value_i, value_j;
  col.get(row_i, value_i);
  col.get(row_j, value_j);
  return (value_i == value_j);
}

template<class T>
Bool areEQ(const ArrayColumn<T>& col, rownr_t row_i, rownr_t row_j) 
{
  Bool rval(False);
  Array<T> arr_i;
  Array<T> arr_j;
  
  col.get(row_i, arr_i, True);
  col.get(row_j, arr_j, True);
  size_t ni = arr_i.nelements();
  size_t nj = arr_j.nelements();
  if( (ni==0 && nj==0) ||    // no data is regarded as equal
      allEQ(arr_i, arr_j)){
    rval = True;
  }
  return rval;
}

inline Int getMapValue (const std::map<Int,Int>& m, Int k)
{
  auto iter = m.find(k);
  return (iter == m.end()  ?  -1 : iter->second);
}
  

} //# NAMESPACE CASACORE - END

#endif



