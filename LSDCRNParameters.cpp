//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// LSDCRNParameters.hpp
//
// Land Surface Dynamics Cosmogenic Radionuclide Parameters Object
//
// This keeps track of paramters used to calculate the evolution of 
// in situ cosmogenic nuclides. 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// An object within the University
//  of Edinburgh Land Surface Dynamics group topographic toolbox
//  for calculating concntration of environmental tracers, CRNs, TCN, fallout
//  nuclides
//
// Developed by:
//  Simon M. Mudd
//  Martin D. Hurst
//  David T. Milodowski
//  Stuart W.D. Grieve
//  Declan A. Valters
//  Fiona Clubb
//
// Copyright (C) 2013 Simon M. Mudd 2013
//
// Developer can be contacted by simon.m.mudd _at_ ed.ac.uk
//
//    Simon Mudd                                                    
//    University of Edinburgh
//    School of GeoSciences
//    Drummond Street
//    Edinburgh, EH8 9XP
//    Scotland
//    United Kingdom
//
// This program is free software;
// you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation;
// either version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY;
// without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the
// GNU General Public License along with this program;
// if not, write to:
// Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301
// USA
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include <fstream>
#include <math.h>
#include <iostream>
#include <vector>
#include "LSDStatsTools.hpp"
#include "LSDCRNParameters.hpp"
using namespace std;

#ifndef LSDCRNParameters_CPP
#define LSDCRNParameters_CPP

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// the LSDCRNParameters object
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// function to set CRN parameters
void LSDCRNParameters::create()
{
  S_t = 1;

  // from Vermeesh 2007
  lambda_10Be = 456e-9;		// in yr-1
  lambda_26Al = 980e-9;		// in yr-1
  lambda_14C = 121e-6;		// in yr-1
  lambda_36Cl = 230e-8;		// in yr-1

  // from Vermeesh 2007
  P0_10Be = 5.11;					// in a/g/yr
  P0_26Al = 30.31;				// in a/g/yr
  P0_14C = 5.86;					// in a/g/yr
  P0_36Cl = 55.45;				// in a/g/yr
  P0_21Ne = 20.29;				// in a/g/yr
  P0_3He = 97.40;					// in a/g/yr

  // in g/cm^2
  Gamma[0] = 160;
  Gamma[1] = 738.6;
  Gamma[2] = 2688;
  Gamma[3] = 4360;

  // dimensionless
  F_10Be[0] = 0.9724;
  F_10Be[1] = 0.0186;
  F_10Be[2] = 0.004;
  F_10Be[3] = 0.005;

  // dimensionless
  F_26Al[0] = 0.9655;
  F_26Al[1] = 0.0233;
  F_26Al[2] = 0.005;
  F_26Al[3] = 0.0062;

  // dimensionless
  F_14C[0] = 0.83;
  F_14C[1] = 0.0691;
  F_14C[2] = 0.0809;
  F_14C[3] = 0.02;

  // dimensionless
  F_36Cl[0] = 0.903;
  F_36Cl[1] = 0.0447;
  F_36Cl[2] = 0.05023;
  F_36Cl[3] = 0.0;
}

// this function gets the parameters used to convert elevation to 
// pressure
void LSDCRNParameters::load_parameters_for_atmospheric_scaling(string path_to_data)
{
  // first load the levels
  levels.push_back(1000);
  levels.push_back(925);
  levels.push_back(850);
  levels.push_back(700);
  levels.push_back(600);
  levels.push_back(500);
  levels.push_back(400);
  levels.push_back(300);
  
  // the dimensions of the data
  int n_levels = 8;
  int NRows = 73;
  int NCols = 145;
  
  // now load the mean sea level pressure
  string filename = meanslp.bin;
  filename = path_to_data+filename;
  cout << "Loading mean sea level, file is: " << endl << filename << endl;
  Array2D<double> new_slp(NRows,NCols,0.0);
  ifstream ifs_data(string_filename.c_str(), ios::in | ios::binary);
  if( ifs_data.fail() )
  {
    cout << "\nFATAL ERROR: the data file \"" << string_filename
         << "\" doesn't exist" << endl;
    exit(EXIT_FAILURE);
  }  
  

  double temp;
  cout << "The size of a double is: " << sizeof(temp) << endl;
  for (int i=0; i<NRows; ++i)
  {
    for (int j=0; j<NCols; ++j)
    {
      ifs_data.read(reinterpret_cast<char*>(&temp), sizeof(temp));
      new_slp[i][j] = temp;
    }
  }
  ifs_data.close();
  
  cout << "10,20: " << new_slp[10][20] << endl;
  cout << "70,120: " << new_slp[70][120] << endl;
  
 
  

}



// This sets the parameters to those used by Grange (need reference year!)
void LSDCRNParameters::set_Granger_parameters()
{
  S_t = 1;

  // from Vermeesh 2007
  lambda_10Be = 456e-9;		// in yr-1
  lambda_26Al = 980e-9;		// in yr-1
  lambda_14C = 121e-6;		// in yr-1
  lambda_36Cl = 230e-8;		// in yr-1

  // from Vermeesh 2007
  P0_10Be = 5.11;					// in a/g/yr
  P0_26Al = 30.31;				// in a/g/yr
  P0_14C = 5.86;					// in a/g/yr
  P0_36Cl = 55.45;				// in a/g/yr
  P0_21Ne = 20.29;				// in a/g/yr
  P0_3He = 97.40;					// in a/g/yr

  // in g/cm^2
  Gamma[0] = 160;
  Gamma[1] = 738.6;
  Gamma[2] = 2688;
  Gamma[3] = 4360;

  // dimensionless
  F_10Be[0] = 0.9724;
  F_10Be[1] = 0.0186;
  F_10Be[2] = 0.004;
  F_10Be[3] = 0.005;

  // dimensionless
  F_26Al[0] = 0.9655;
  F_26Al[1] = 0.0233;
  F_26Al[2] = 0.005;
  F_26Al[3] = 0.0062;

  // dimensionless
  F_14C[0] = 0.83;
  F_14C[1] = 0.0691;
  F_14C[2] = 0.0809;
  F_14C[3] = 0.02;

  // dimensionless
  F_36Cl[0] = 0.903;
  F_36Cl[1] = 0.0447;
  F_36Cl[2] = 0.05023;
  F_36Cl[3] = 0.0;
}

// function to set CRN parameters
// based on the Vermeesch approximation of the Schaller et al (2000)
// formulation
void LSDCRNParameters::set_Schaller_parameters()
{
  S_t =1;

  // from Vermeesh 2007
  lambda_10Be = 456e-9;		// in yr-1
  lambda_26Al = 980e-9;		// in yr-1
  lambda_14C = 121e-6;		// in yr-1
  lambda_36Cl = 230e-8;		// in yr-1

  // from Vermeesh 2007
  P0_10Be = 5.11;					// in a/g/yr
  P0_26Al = 30.31;				// in a/g/yr
  P0_14C = 5.86;					// in a/g/yr
  P0_36Cl = 55.45;				// in a/g/yr
  P0_21Ne = 20.29;				// in a/g/yr
  P0_3He = 97.40;					// in a/g/yr

  // in g/cm^2
  Gamma[0] = 160;
  Gamma[1] = 738.6;
  Gamma[2] = 2688;
  Gamma[3] = 4360;

  // dimensionless
  F_10Be[0] = 0.964;
  F_10Be[1] = 0.0266;
  F_10Be[2] = -0.0074;
  F_10Be[3] = 0.0168;

  // dimensionless
  F_26Al[0] = 0.9575;
  F_26Al[1] = 0.0315;
  F_26Al[2] = -0.009;
  F_26Al[3] = 0.02;

  // dimensionless
  F_14C[0] = 0.83;
  F_14C[1] = 0.1363;
  F_14C[2] = 0.0137;
  F_14C[3] = 0.02;

  // dimensionless
  F_36Cl[0] = 0.903;
  F_36Cl[1] = 0.0793;
  F_36Cl[2] = 0.0177;
  F_36Cl[3] = 0.0;
}

// this forces a neutron only calculation
void LSDCRNParameters::set_Neutron_only_parameters()
{
  S_t =1;

  // from Vermeesh 2007
  lambda_10Be = 456e-9;		// in yr-1
  lambda_26Al = 980e-9;		// in yr-1
  lambda_14C = 121e-6;		// in yr-1
  lambda_36Cl = 230e-8;		// in yr-1

  // from Vermeesh 2007
  P0_10Be = 5.11;					// in a/g/yr
  P0_26Al = 30.31;				// in a/g/yr
  P0_14C = 5.86;					// in a/g/yr
  P0_36Cl = 55.45;				// in a/g/yr
  P0_21Ne = 20.29;				// in a/g/yr
  P0_3He = 97.40;					// in a/g/yr

  // in g/cm^2
  Gamma[0] = 160;
  Gamma[1] = 738.6;
  Gamma[2] = 2688;
  Gamma[3] = 4360;

  // dimensionless
  F_10Be[0] = 1;
  F_10Be[1] = 0;
  F_10Be[2] = 0;
  F_10Be[3] = 0;

  // dimensionless
  F_26Al[0] = 1;
  F_26Al[1] = 0;
  F_26Al[2] = 0;
  F_26Al[3] = 0;

  // dimensionless
  F_14C[0] = 1;
  F_14C[1] = 0;
  F_14C[2] = 0;
  F_14C[3] = 0;

  // dimensionless
  F_36Cl[0] = 1;
  F_36Cl[1] = 0;
  F_36Cl[2] = 0;
  F_36Cl[3] = 0;
}


// this function takes a single scaling factor for
// elevation scaling, self shielding, snow shielding,
// and latitude scaling and produces scaling factors
// for each production mechamism.
// the scaling follows the approach of vermeesch 2008
// it uses a 'virstual' shielding depth to calcualte
// the updated scaling factors
void LSDCRNParameters::scale_F_values(double single_scaling)
{
  double tol = 1e-7;
  double x = 0;
  double new_x = 0;
  double test_scaling = 1e8;
  double dx =-10;

  // first do 10Be
  if (single_scaling > 1)
  {
    dx = -10;
  }
  else if (single_scaling < 1)
  {
    dx = 10;
  }
  else if (single_scaling == 1)
  {
    dx = 10;
    test_scaling = 1;
  }

	while (fabs(test_scaling - single_scaling) > tol)
	//for (int i = 0; i< 100; i++)
	{
		x = new_x;
		new_x = x+dx;

		// calcualte the new scaling
		test_scaling = exp(-new_x/Gamma[0])*F_10Be[0]+
					   exp(-new_x/Gamma[1])*F_10Be[1]+
					   exp(-new_x/Gamma[2])*F_10Be[2]+
					   exp(-new_x/Gamma[3])*F_10Be[3];

		// if you have overshot, halve dx and reset new_x
		if (single_scaling > 1)
		{
			if (test_scaling > single_scaling)
			{
				dx = 0.5*dx;
				new_x = x;
			}
		}
		else if (single_scaling < 1)
		{
			if (test_scaling < single_scaling)
			{
				dx = 0.5*dx;
				new_x = x;
			}
		}
	}

	// now reset the F_values
	F_10Be[0] = exp(-new_x/Gamma[0])*F_10Be[0];
	F_10Be[1] = exp(-new_x/Gamma[1])*F_10Be[1];
	F_10Be[2] = exp(-new_x/Gamma[2])*F_10Be[2];
	F_10Be[3] = exp(-new_x/Gamma[3])*F_10Be[3];

	//cout << "FINISHED 10Be x is: " << x << " and test_scaling is: " << test_scaling << endl;
	//cout << F_10Be[0] << endl << F_10Be[1] << endl << F_10Be[2] << endl << F_10Be[3] << endl;

	// now do 26Al
	x = 0;
	new_x = 0;
	test_scaling = 1e8;
	dx =-10;
	if (single_scaling > 1)
	{
		dx = -10;
	}
	else if (single_scaling < 1)
	{
		dx = 10;
	}
	else if (single_scaling == 1)
	{
		dx = 10;
		test_scaling = 1;
	}

	while (fabs(test_scaling - single_scaling) > tol)
	//for (int i = 0; i< 100; i++)
	{
		x = new_x;
		new_x = x+dx;

		// calcualte the new scaling
		test_scaling = exp(-new_x/Gamma[0])*F_26Al[0]+
					   exp(-new_x/Gamma[1])*F_26Al[1]+
					   exp(-new_x/Gamma[2])*F_26Al[2]+
					   exp(-new_x/Gamma[3])*F_26Al[3];

		// if you have overshot, halve dx and reset new_x
		if (single_scaling > 1)
		{
			if (test_scaling > single_scaling)
			{
				dx = 0.5*dx;
				new_x = x;
			}
		}
		else if (single_scaling < 1)
		{
			if (test_scaling < single_scaling)
			{
				dx = 0.5*dx;
				new_x = x;
			}
		}
	}

	// now reset the F_values
	F_26Al[0] = exp(-new_x/Gamma[0])*F_26Al[0];
	F_26Al[1] = exp(-new_x/Gamma[1])*F_26Al[1];
	F_26Al[2] = exp(-new_x/Gamma[2])*F_26Al[2];
	F_26Al[3] = exp(-new_x/Gamma[3])*F_26Al[3];

	//cout << "FINISHED 26Al x is: " << x << " and test_scaling is: " << test_scaling << endl;
	//cout << F_26Al[0] << endl << F_26Al[1] << endl << F_26Al[2] << endl << F_26Al[3] << endl;

	// now do 36Cl
	x = 0;
	new_x = 0;
	test_scaling = 1e8;
	dx =-10;
	if (single_scaling > 1)
	{
		dx = -10;
	}
	else if (single_scaling < 1)
	{
		dx = 10;
	}
	else if (single_scaling == 1)
	{
		dx = 10;
		test_scaling = 1;
	}

	while (fabs(test_scaling - single_scaling) > tol)
	//for (int i = 0; i< 100; i++)
	{
		x = new_x;
		new_x = x+dx;

		// calcualte the new scaling
		test_scaling = exp(-new_x/Gamma[0])*F_36Cl[0]+
					   exp(-new_x/Gamma[1])*F_36Cl[1]+
					   exp(-new_x/Gamma[2])*F_36Cl[2]+
					   exp(-new_x/Gamma[3])*F_36Cl[3];

		// if you have overshot, halve dx and reset new_x
		if (single_scaling > 1)
		{
			if (test_scaling > single_scaling)
			{
				dx = 0.5*dx;
				new_x = x;
			}
		}
		else if (single_scaling < 1)
		{
			if (test_scaling < single_scaling)
			{
				dx = 0.5*dx;
				new_x = x;
			}
		}
	}

	// now reset the F_values
	F_36Cl[0] = exp(-new_x/Gamma[0])*F_36Cl[0];
	F_36Cl[1] = exp(-new_x/Gamma[1])*F_36Cl[1];
	F_36Cl[2] = exp(-new_x/Gamma[2])*F_36Cl[2];
	F_36Cl[3] = exp(-new_x/Gamma[3])*F_36Cl[3];

	//cout << "FINISHED 36Cl x is: " << x << " and test_scaling is: " << test_scaling << endl;
	//cout << F_36Cl[0] << endl << F_36Cl[1] << endl << F_36Cl[2] << endl << F_36Cl[3] << endl;

	// now do 14C
	x = 0;
	new_x = 0;
	test_scaling = 1e8;
	dx =-10;
	if (single_scaling > 1)
	{
		dx = -10;
	}
	else if (single_scaling < 1)
	{
		dx = 10;
	}
	else if (single_scaling == 1)
	{
		dx = 10;
		test_scaling = 1;
	}

	while (fabs(test_scaling - single_scaling) > tol)
	//for (int i = 0; i< 100; i++)
	{
		x = new_x;
		new_x = x+dx;

		// calcualte the new scaling
		test_scaling = exp(-new_x/Gamma[0])*F_14C[0]+
					   exp(-new_x/Gamma[1])*F_14C[1]+
					   exp(-new_x/Gamma[2])*F_14C[2]+
					   exp(-new_x/Gamma[3])*F_14C[3];

		// if you have overshot, halve dx and reset new_x
		if (single_scaling > 1)
		{
			if (test_scaling > single_scaling)
			{
				dx = 0.5*dx;
				new_x = x;
			}
		}
		else if (single_scaling < 1)
		{
			if (test_scaling < single_scaling)
			{
				dx = 0.5*dx;
				new_x = x;
			}
		}
	}

	// now reset the F_values
	F_14C[0] = exp(-new_x/Gamma[0])*F_14C[0];
	F_14C[1] = exp(-new_x/Gamma[1])*F_14C[1];
	F_14C[2] = exp(-new_x/Gamma[2])*F_14C[2];
	F_14C[3] = exp(-new_x/Gamma[3])*F_14C[3];

	//cout << "FINISHED 14C x is: " << x << " and test_scaling is: " << test_scaling << endl;
	//cout << F_14C[0] << endl << F_14C[1] << endl << F_14C[2] << endl << F_14C[3] << endl;
}

#endif
