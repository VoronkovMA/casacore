//# UnitMap7.cc: Unit map custom units initialisation part 3
//# Copyright (C) 2001,2002,2003
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

//# Includes

#include <casacore/casa/Quanta/UnitMap.h>

namespace casacore { //# NAMESPACE CASACORE - BEGIN

// Initialise the maps
void UnitMap::initUMCust3 (UMaps& maps) {
  map<String, UnitName>& mapCust = maps.mapCust;
  mapCust.insert(map<String, UnitName>::value_type
			   ("fl_oz", 	UnitName("fl_oz",
						 UnitVal(277.4193*2.54*2.54*2.54/5/4/2/4, "cm3", &maps),
						 "fluid ounce (Imp)")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("USfl_oz", 	UnitName("USfl_oz",
						 UnitVal(231*2.54*2.54*2.54/4/4/2/4, "cm3", &maps),
						 "fluid ounce (US)")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("ft",   	UnitName("ft",
						 UnitVal(12*2.54e-2, "m", &maps),
						 "foot")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("fur",   	UnitName("fur",
						 UnitVal(220*3*12*2.54, "cm", &maps),
						 "furlong")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("Gal",   	UnitName("Gal",
						 UnitVal(1., "cm/s2", &maps),
						 "gal")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("gal",   	UnitName("gal",
						 UnitVal(277.4193*2.54*2.54*2.54, "cm3", &maps),
						 "gallon (Imp)")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("USgal", 	UnitName("USgal",
						 UnitVal(231*2.54*2.54*2.54, "cm3", &maps),
						 "gallon (US)")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("G",   	UnitName("G",
						 UnitVal(1.0e-4, "T", &maps),
						 "gauss")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("Gb",   	UnitName("Gb",
						 UnitVal(10.0/(4.0 * C::pi), "A", &maps),
						 "gilbert")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("ha",   	UnitName("ha",
						 UnitVal(1.,"hm2", &maps),
						 "hectare")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("hp",   	UnitName("hp",
						 UnitVal(745.7, "W", &maps),
						 "horsepower")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("cwt",   	UnitName("cwt",
						 UnitVal(4*2*14*0.45359237, "kg", &maps),
						 "hundredweight")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("in",   	UnitName("in",
						 UnitVal(2.54, "cm", &maps),
						 "inch")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("kn",   	UnitName("kn",
						 UnitVal(6080*12*2.54, "cm/h", &maps),
						 "knot (Imp)")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("ly",   	UnitName("ly",
						 UnitVal(9.46073047e+15, "m", &maps),
						 "light year")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("Mx",   	UnitName("Mx",
						 UnitVal(1.0e-8, "Wb", &maps),
						 "maxwell")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("mile",  	UnitName("mile",
						 UnitVal(5280*12*2.54e-2, "m", &maps),
						 "mile")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("n_mile", 	UnitName("n_mile",
						 UnitVal(6080*12*2.54, "cm", &maps),
						 "nautical mile (Imp)")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("Oe",   	UnitName("Oe",
						 UnitVal(1000.0/(4.0*C::pi), "A/m", &maps),
						 "oersted")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("oz",   	UnitName("oz",
						 UnitVal(1./16.*0.45359237, "kg", &maps),
						 "ounce (avoirdupois)")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("lb",   	UnitName("lb",
						 UnitVal(0.45359237, "kg", &maps),
						 "pound (avoirdupois)")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("R",   	UnitName("R",
						 UnitVal(2.58e-4, "C/kg", &maps),
						 "mile")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("sb",   	UnitName("sb",
						 UnitVal(1e4, "cd/m2", &maps),
						 "stilb")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("St",   	UnitName("St",
						 UnitVal(1., "cm2/s", &maps),
						 "stokes")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("Torr",  	UnitName("Torr",
						 UnitVal((1.0/760.0)*1.01325e+5, "Pa", &maps),
						 "torr")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("yd",   	UnitName("yd",
						 UnitVal(3*12*2.54, "cm", &maps),
						 "yard")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("yr",   	UnitName("yr",
						 UnitVal(24.*3600.*365.25, "s", &maps),
						 "year")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("adu",	UnitName("adu",
						 UnitVal(1.,UnitDim::Dnon),
						 "dimensionless ADC unit")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("beam",	UnitName("beam",
						 UnitVal(1.,UnitDim::Dnon),
						 "undefined beam area")));
  mapCust.insert(map<String, UnitName>::value_type
                           ("count", 	UnitName("count",
                                                 UnitVal(1.,UnitDim::Dnon),
                                                 "count")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("pixel",	UnitName("pixel",
						 UnitVal(1.,UnitDim::Dnon),
						 "pixel")));
  mapCust.insert(map<String, UnitName>::value_type
			   ("lambda",	UnitName("lambda",
						 UnitVal(1.,UnitDim::Dnon),
						 "lambda")));
}

} //# NAMESPACE CASACORE - END

