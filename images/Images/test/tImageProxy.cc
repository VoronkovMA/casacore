 //# tImageProxy.cc
//# Copyright (C) 1998,1999,2000,2002
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

#include <casacore/images/Images/ImageProxy.h>

#include <casacore/images/Images/ImageInfo.h>

#include <casacore/casa/namespace.h>

int main() {
    try {
        ImageProxy proxy("imagetestimage.fits", "", vector<ImageProxy>());
        ImageInfo ii = proxy.imageInfoObject();
        AlwaysAssert(ii.getBeamSet().hasSingleBeam(), AipsError);
        AlwaysAssert(proxy.type() == TpFloat, AipsError);
        AlwaysAssert(proxy.coordSysObject().nWorldAxes() == 2, AipsError);
    }
    catch (const std::exception& x) {
        cout << "Caught error " << x.what() << endl;
        return 1;
    } 
    cout << "OK" << endl;
    return 0;
}
