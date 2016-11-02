//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// LSDParameterParser.cpp
//
// Land Surface Dynamics Parameter Parser Object
//
// An object for keeping track of parameters
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
// Copyright (C) 2016 Simon M. Mudd 2016
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
#include <map>
#include "TNT/tnt.h"
#include "LSDStatsTools.hpp"
#include "LSDParameterParser.hpp"
using namespace std;
using namespace TNT;

#ifndef LSDParameterParser_CPP
#define LSDParameterParser_CPP


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Create functions
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDParameterParser::create()
{
  cout << "I have created an empty parameter parser object. " << endl;
  cout << "Surely you want to give it a filename?" << endl;
}

// THis just creates using a path and a filenmate
void LSDParameterParser::create(string PathName, string FileName)
{

  // Make sure the path has an extension
  PathName = FixPath(PathName);
  string FullName = PathName+FileName;
  create(FullName);
}


void LSDParameterParser::create(string FullName)
{
  ifstream file_info_in;
  file_info_in.open(FullName.c_str());
  
  // check if the parameter file exists
  if( file_info_in.fail() )
  {
    cout << "\nFATAL ERROR: The parameter file \"" << FullName
         << "\" doesn't exist" << endl;
    exit(EXIT_FAILURE);
  }
  
  // now ingest the parameters
  ingest_data(FullName);
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Gets a line of the parameter file. Has a long buffer so you can add long path 
// names. 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDParameterParser::LSDPP_parse_line(ifstream &infile, string &parameter, string &value)
{
  char c;
  char buff[1024];
  int pos = 0;
  int word = 0;

  while ( infile.get(c) )
  {
    if (pos >= 1024)
    {
      cout << "Buffer overrun, word too long in parameter line: " << endl;
      string line;
      getline(infile, line);
      cout << "\t" << buff << " ! \n" << line << endl;
      exit(1);
    }
    // preceeding whitespace
    if (c == '#')
    {
      if (word == 0)
      {
        parameter = "NULL";
        value = "NULL";
      }
      if (word == 1)
        value = "NULL";
      word = 2;
    }

    if ((c == ' ' || c == '\t') && pos == 0)
      continue;
    else if ( (c == ':' && word == 0) || ( (c == ' ' || c == '\n' || c == '\t') && word == 1))
    {
      while (buff[pos-1] == ' ' || buff[pos-1] == '\t')
        --pos;    // Trailing whitespace
      buff[pos] = '\0';  // Append Null char
      if (word == 0)
        parameter = buff;  // Assign buffer contents
      else if (word == 1)
        value = buff;
      ++word;
      pos = 0;    // Rewind buffer
    }
    else if ( c == '\n' && word == 0 )
    {
      parameter = "NULL";
      buff[pos] = '\0';
      value = buff;
      ++word;
    }
    else if (word < 2)
    {
      buff[pos] = c;
      ++pos;
    }

    if (c == '\n')
      break;
  }
  if (word == 0)
  {
    parameter = "NULL";
    value = "NULL";
  }
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// This function gets all the data from a parameter file
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void LSDParameterParser::ingest_data(string FullName)
{

  ifstream infile;
  infile.open(FullName.c_str());
  string parameter, value, lower, lower_val;
  string bc;

  cout << "Parameter filename is: " << FullName << endl;


  // now ingest parameters
  while (infile.good())
  {
    LSDPP_parse_line(infile, parameter, value);
    lower = parameter;
    if (parameter == "NULL")
      continue;
    for (unsigned int i=0; i<parameter.length(); ++i)
    {
      lower[i] = tolower(parameter[i]);
    }

    cout << "parameter is: " << lower << " and value is: " << value << endl;

    // get rid of control characters
    value = RemoveControlCharactersFromEndOfString(value);

    if (lower == "dem read extension")
    {
      dem_read_extension = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      dem_read_extension = RemoveControlCharactersFromEndOfString(dem_read_extension);
    }
    else if (lower == "dem write extension")
    {
      dem_write_extension = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      dem_write_extension = RemoveControlCharactersFromEndOfString(dem_write_extension);
    }
    else if (lower == "write path")
    {
      write_path = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      write_path = RemoveControlCharactersFromEndOfString(write_path);
    }
    else if (lower == "write fname")
    {
      write_fname = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      write_fname = RemoveControlCharactersFromEndOfString(write_fname);
      //cout << "Got the write name, it is: "  << write_fname << endl;
    }
    else if (lower == "read path")
    {
      read_path = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      read_path = RemoveControlCharactersFromEndOfString(read_path);
      //cout << "Got the write name, it is: "  << write_fname << endl;
    }
    else if (lower == "read fname")
    {
      read_fname = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      read_fname = RemoveControlCharactersFromEndOfString(read_fname);
      //cout << "Got the read name, it is: " << read_fname << endl;
    }
    else if (lower == "channel heads fname")
    {
      CHeads_file = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      CHeads_file = RemoveControlCharactersFromEndOfString(CHeads_file);
      //cout << "Got the channel heads name, it is: " << CHeads_file << endl;
    }
    
    
    //=-=-=-=-=-=--=-=-=-=-
    // paramters for fill
    //-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "min_slope_for_fill")
    {
      float_parameters["min_slope_for_fill"] = atof(value.c_str());
    }
    else if (lower == "fill_method")
    {
      method_map["fill_method"] = value;
      method_map["fill_method"] = RemoveControlCharactersFromEndOfString(method_map["fill_method"]);
    }

    //=-=-=-=-=-=--=-=-=-=-
    // paramters for hillshade
    //-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "hs_altitude")
    {
      float_parameters["hs_altitude"] = atof(value.c_str());
    }
    else if (lower == "hs_azimuth")
    {
      float_parameters["hs_azimuth"] = atof(value.c_str());
    }
    else if (lower == "hs_z_factor")
    {
      float_parameters["hs_z_factor"] = atof(value.c_str());
    }
    else if (lower == "hs_use_fill")
    {
      //cout << "Use hs bool: " << value << endl;
      bool temp_bool = (value == "true") ? true : false;
      //cout << "Temp bool: " << temp_bool << endl;
      //bool tbool = true;
      //bool fbool = false;
      //cout << "True is " << tbool << " and false is: " << fbool << endl;
      analyses_switches["hs_use_fill"] = temp_bool;
      cout << "You have set use of the fill raster for the hillshade to " 
           << analyses_switches["hs_use_fill"] << endl;
    }


    //=-=-=-=-=-=--=-=-=-=-
    // parameters for flow info
    //-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "boundary conditions")
    {
      // get the boundary value in lowercase
      lower_val = value;
      for (unsigned int i=0; i<value.length(); ++i)
      {
        lower_val[i] = tolower(value[i]);
      }
      vector<string> temp_bc(4);
      bc = lower_val;

      // now loop through collecting boundary conditions
      for (int i = 0; i<4; i++)
      {
        string this_bc = bc.substr(i,1);
        //cout << "Component " << i << " of the bc string: " << this_bc << endl;
        if (this_bc.find("p") != 0 && this_bc.find("b") != 0 && this_bc.find("n") != 0)
        {
          cout << "boundary condition not periodic, baselevel or noflux!" << endl;
          cout << "defaulting to no flux" << endl;
          temp_bc[i] = "n";
        }
        else
        {
          temp_bc[i] = this_bc;
        }
      }
      boundary_conditions = temp_bc;
    }

    //=-=-=-=-=-=--=-=-=-=-
    // parameters for chi
    //-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "nodeindex fname for chi map")
    {
      support_file_names["nodeindex_fname_for_chi_map"] = atof(value.c_str());
    }
    else if (lower == "a_0")
    {
      float_parameters["A_0"] = atof(value.c_str());
    }
    else if (lower == "m_over_n")
    {
      float_parameters["m_over_n"] = atof(value.c_str());
    }
    else if (lower == "threshold_area_for_chi")
    {
      float_parameters["threshold_area_for_chi"] = atof(value.c_str());
    }
    else if (lower == "threshold_pixels_for_chi")
    {
      int_parameters["threshold_pixels_for_chi"] = atoi(value.c_str());
    }

    //=-=-=-=-=-=--=-=-=-=-
    // parameters for chi segment fitting
    //-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "n_iterations")
    {
      int_parameters["n_iterations"] = atoi(value.c_str());
    }
    else if (lower == "skip")
    {
      int_parameters["skip"] = atoi(value.c_str());
    }
    else if (lower == "minimum_segment_length")
    {
      int_parameters["minimum_segment_length"] = atoi(value.c_str());
    }
    else if (lower == "target_nodes")
    {
      int_parameters["target_nodes"] = atoi(value.c_str());
    }
    else if (lower == "sigma")
    {
      float_parameters["sigma"] = atof(value.c_str());
    }



    //=-=-=-=-=-=--=-=-=-=-
    // parameters for polyfit
    //-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "polyfit_window_radius")
    {
      float_parameters["polyfit_window_radius"] = atof(value.c_str());
      cout << "Your polyfit window radius is: "  <<  float_parameters["polyfit_window_radius"] << endl;
    }
    else if (lower == "slope_method")
    {
      method_map["slope_method"] = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      method_map["slope_method"] = RemoveControlCharactersFromEndOfString(method_map["slope_method"]);
      cout << "Your slope method is: "  <<  method_map["slope_method"] << endl;
    }


    //=-=-=-=-=-=-=-=-=-=-=-=-
    // parameters for drainage area extraction
    //=-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "drainage_area_method")
    {
      method_map["drainage_area_method"] = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      method_map["drainage_area_method"] = RemoveControlCharactersFromEndOfString(method_map["drainage_area_method"]);
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-
    // parameters for single thread channel extraction
    //=-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "single_thread_channel_method")
    {
      method_map["single_thread_channel_method"] = value;
      // get rid of any control characters from the end (if param file was made in DOS)
      method_map["single_thread_channel_method"] =
      RemoveControlCharactersFromEndOfString(method_map["single_thread_channel_method"]);
    }

    //=-=-=-=-=-=--=-=-=-=-
    // parameters for area threshold channel network
    //-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "pixel_threshold_for_channel_net")
    {
      float_parameters["pixel_threshold_for_channel_net"] = atof(value.c_str());
    }

    //=-=-=-=-=-=--=-=-=-=-
    // parameters for landscape properties
    //-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "root_cohesion")
    {
      float_parameters["root_cohesion"] = atof(value.c_str());
      cout << "Your root_cohesion is: "  <<  float_parameters["root_cohesion"] << endl;
    }
    else if (lower == "soil_density")
    {
      float_parameters["soil_density"] = atof(value.c_str());
      cout << "Your soil_density is: "  <<  float_parameters["soil_density"] << endl;
    }
    else if (lower == "hydraulic_conductivity")
    {
      float_parameters["hydraulic_conductivity"] = atof(value.c_str());
      cout << "Your hydraulic_conductivity is: "  <<  float_parameters["hydraulic_conductivity"] << endl;
    }
    else if (lower == "soil_thickness")
    {
      float_parameters["soil_thickness"] = atof(value.c_str());
      cout << "Your soil_thickness is: "  <<  float_parameters["soil_thickness"] << endl;
    }
    else if (lower == "tan_phi")
    {
      float_parameters["tan_phi"] = atof(value.c_str());
      cout << "Your tan_phi is: "  <<  float_parameters["tan_phi"] << endl;
    }

    //=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
    // parameters for nodata hole filling
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "nodata_hole_filling_window_width")
    {
      float_parameters["nodata_hole_filling_window_width"] = atof(value.c_str());
      cout << "Your hole_filling_window is: "  
           <<  float_parameters["nodata_hole_filling_window_width"] <<  " pixels" << endl;
    }
    
    //=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=
    // parameters for masking
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    else if (lower == "curvature_mask_threshold")
    {
      float_parameters["curvature_mask_threshold"] = atof(value.c_str());
      cout << "Your curvature_mask_threshold is: "  
           <<  float_parameters["curvature_mask_threshold"] <<  " 1/m" << endl;
    }
    else if (lower == "curvature_mask_nodataisbelowthreshold")
    {
      int_parameters["curvature_mask_nodataisbelowthreshold"] = atoi(value.c_str());
      cout << "Your curvature_mask_nodataisbelowthreshold is: "  
           <<  int_parameters["curvature_mask_nodataisbelowthreshold"] <<  "; anything other than 0 means true." << endl;
    }
    else if (lower == "mask_threshold")
    {
      float_parameters["mask_threshold"] = atof(value.c_str());
      cout << "Mask_threshold is: "  
           <<  float_parameters["mask_threshold"] <<  " (dimensions depend on raster)" << endl;
    }
    else if (lower == "mask_nodataisbelowthreshold")
    {
      int_parameters["mask_nodataisbelowthreshold"] = atoi(value.c_str());
      cout << "Your mask_nodataisbelowthreshold is: "  
           <<  int_parameters["mask_nodataisbelowthreshold"] <<  "; anything other than 0 means true." << endl;
    }


    //=-=-=-=-=-=--=-=-=-=-
    // what to write
    //-=-=-=-=-=-=-=-=-=-=-=-
    else if (lower == "write fill")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_fill"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_fill"] = temp_bool;
    }
    else if (lower == "write trimmed and nodata filled")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_trim_ndfill"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_trimmed_hole_filled"] = temp_bool;
    }
    else if (lower == "write hillshade")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_hillshade"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_hillshade"] = temp_bool;
    }
    else if (lower == "write mask threshold")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_mask_threshold"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_mask_threshold"] = temp_bool;
    }
    else if (lower == "write slope")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_slope"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_fill"] = temp_bool;
      raster_switches["need_slope"] = temp_bool;
    }
    else if (lower == "write curvature")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_curvature"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_curvature"] = temp_bool;
    }
    else if (lower == "write curvature mask threshold")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_curvature_mask_threshold"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_curvature"] = temp_bool;
      raster_switches["need_curvature_mask_threshold"] = temp_bool;
    }
    else if (lower == "write planform curvature")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_planform_curvature"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_planform_curvature"] = temp_bool;
    }
    else if (lower == "write tangential curvature")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_tangential_curvature"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_tangential_curvature"] = temp_bool;
    }
    else if (lower == "write profile curvature")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_profile_curvature"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_profile_curvature"] = temp_bool;
    }
    else if (lower == "write aspect")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_aspect"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_aspect"] = temp_bool;
    }
    else if (lower == "write topographic classification")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_topographic_classification"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_topographic_classification"] = temp_bool;
    }
    else if (lower == "write drainage area")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_drainage_area"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_fill"] = temp_bool;
      raster_switches["need_drainage_area"] = temp_bool;
    }
    else if (lower == "write channel net")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_channel_net"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_fill"] = temp_bool;
      raster_switches["need_flowinfo"] = temp_bool;
      raster_switches["need_ContributingPixels"] = temp_bool;
      raster_switches["need_JunctionNetwork"] = temp_bool;
      raster_switches["need_sources"] = temp_bool;
      raster_switches["need_SOArray"] = temp_bool;
      raster_switches["need_JunctionIndex"] = temp_bool;
    }
    else if (lower == "write nodeindex")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_nodeindex"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_fill"] = temp_bool;
      raster_switches["need_flowinfo"] = temp_bool;
      raster_switches["need_nodeindex"] = temp_bool;
    }
    else if (lower == "write single thread channel")
    {
      bool temp_bool =  (value == "true") ? true : false;
      analyses_switches["write_single_thread_channel"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_fill"] = temp_bool;
      raster_switches["need_flowinfo"] = temp_bool;
      raster_switches["need_flow_distance"] = temp_bool;
      raster_switches["need_drainage_area"] = temp_bool;
    }
    else if (lower == "write chi map")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_chi_map"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_fill"] = temp_bool;
      raster_switches["need_flowinfo"] = temp_bool;
      raster_switches["need_chi_map"] = temp_bool;
    }
    else if (lower == "write factor of safety at saturation")
    {
      bool temp_bool = (value == "true") ? true : false;
      analyses_switches["write_FS_sat"] = temp_bool;
      raster_switches["need_base_raster"] = temp_bool;
      raster_switches["need_fill"] = temp_bool;
      raster_switches["need_slope"] = temp_bool;
      raster_switches["need_slope_angle"] = temp_bool;
      raster_switches["need_FS_sat"] = temp_bool;
    }
    else
    {
      cout << "Line " << __LINE__ << ": No parameter '"
           << parameter << "' expected.\n\t> Check spelling." << endl;
    }

    //cout << "Got " << lower << " and value is: " << value << endl;

  }
  infile.close();

}
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// These two functions takes a map of defualt parameters and returns the parameters for the
// current implementation
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
map<string,float> LSDParameterParser::set_float_parameters(map<string,float> float_default_map)
{
  // the idea is to look through the default map, getting the keys, and then
  // looking for the keys in the parameter maps
  vector<string> these_keys = extract_keys(float_default_map);
  
  int n_keys = int(these_keys.size());
  for(int i = 0; i<n_keys; i++)
  {
    cout << "Key is: " << these_keys[i] << endl;
  }
  
  
  map<string,float> these_float_parameters;
  
  return these_float_parameters;
  

}

map<string,int> LSDParameterParser::set_int_parameters(map<string,int> int_default_map)
{
  // the idea is to look through the default map, getting the keys, and then
  // looking for the keys in the parameter maps
  vector<string> these_keys = extract_keys(int_default_map);
  
  int n_keys = int(these_keys.size());
  for(int i = 0; i<n_keys; i++)
  {
    cout << "Key is: " << these_keys[i] << endl;
  }

  map<string,int> these_int_parameters;
  
  return these_int_parameters;

}


#endif