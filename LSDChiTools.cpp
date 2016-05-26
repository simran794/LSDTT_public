//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDChiTools
// Land Surface Dynamics ChiTools object
//
// An object within the University
//  of Edinburgh Land Surface Dynamics group topographic toolbox
//  for performing various analyses in chi space
//
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


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// LSDChiTools.cpp
// LSDChiTools object
// LSD stands for Land Surface Dynamics
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// This object is written by
// Simon M. Mudd, University of Edinburgh
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-----------------------------------------------------------------
//DOCUMENTATION URL: http://www.geos.ed.ac.uk/~s0675405/LSD_Docs/
//-----------------------------------------------------------------



#ifndef LSDChiTools_CPP
#define LSDChiTools_CPP

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "TNT/tnt.h"
#include "LSDFlowInfo.hpp"
#include "LSDRaster.hpp"
#include "LSDChannel.hpp"
#include "LSDJunctionNetwork.hpp"
#include "LSDIndexChannel.hpp"
#include "LSDStatsTools.hpp"
#include "LSDShapeTools.hpp"
#include "LSDChiTools.hpp"
#include "LSDChiNetwork.hpp"
using namespace std;
using namespace TNT;

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Creates an LSDChiTools from an LSDRaster 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::create(LSDRaster& ThisRaster)
{
  NRows = ThisRaster.get_NRows();
  NCols = ThisRaster.get_NCols();
  XMinimum = ThisRaster.get_XMinimum();
  YMinimum = ThisRaster.get_YMinimum();
  DataResolution = ThisRaster.get_DataResolution();
  NoDataValue = ThisRaster.get_NoDataValue();
  GeoReferencingStrings = ThisRaster.get_GeoReferencingStrings();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Creates an LSDChiTools from an LSDRaster 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::create(LSDIndexRaster& ThisRaster)
{
  NRows = ThisRaster.get_NRows();
  NCols = ThisRaster.get_NCols();
  XMinimum = ThisRaster.get_XMinimum();
  YMinimum = ThisRaster.get_YMinimum();
  DataResolution = ThisRaster.get_DataResolution();
  NoDataValue = ThisRaster.get_NoDataValue();
  GeoReferencingStrings = ThisRaster.get_GeoReferencingStrings();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Creates an LSDChiTools from an LSDFlowInfo
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::create(LSDFlowInfo& ThisFI)
{
  NRows = ThisFI.get_NRows();
  NCols = ThisFI.get_NCols();
  XMinimum = ThisFI.get_XMinimum();
  YMinimum = ThisFI.get_YMinimum();
  DataResolution = ThisFI.get_DataResolution();
  NoDataValue = ThisFI.get_NoDataValue();
  GeoReferencingStrings = ThisFI.get_GeoReferencingStrings();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Creates an LSDChiTools from an LSDFlowInfo
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::create(LSDJunctionNetwork& ThisJN)
{
  NRows = ThisJN.get_NRows();
  NCols = ThisJN.get_NCols();
  XMinimum = ThisJN.get_XMinimum();
  YMinimum = ThisJN.get_YMinimum();
  DataResolution = ThisJN.get_DataResolution();
  NoDataValue = ThisJN.get_NoDataValue();
  GeoReferencingStrings = ThisJN.get_GeoReferencingStrings();
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function returns the x and y location of a row and column
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_x_and_y_locations(int row, int col, double& x_loc, double& y_loc)
{
  
  x_loc = XMinimum + float(col)*DataResolution + 0.5*DataResolution;
    
  // Slightly different logic for y because the DEM starts from the top corner
  y_loc = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function returns the x and y location of a row and column
// Same as above but with floats
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_x_and_y_locations(int row, int col, float& x_loc, float& y_loc)
{
  
  x_loc = XMinimum + float(col)*DataResolution + 0.5*DataResolution;
    
  // Slightly different logic for y because the DEM starts from the top corner
  y_loc = YMinimum + float(NRows-row)*DataResolution - 0.5*DataResolution;
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// Function to convert a node position with a row and column to a lat
// and long coordinate
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_lat_and_long_locations(int row, int col, double& lat, 
                   double& longitude, LSDCoordinateConverterLLandUTM Converter)
{
  // get the x and y locations of the node
  double x_loc,y_loc;
  get_x_and_y_locations(row, col, x_loc, y_loc);
  
  // get the UTM zone of the node
  int UTM_zone;
  bool is_North;
  get_UTM_information(UTM_zone, is_North);
  //cout << endl << endl << "Line 1034, UTM zone is: " << UTM_zone << endl;
  
  
  if(UTM_zone == NoDataValue)
  {
    lat = NoDataValue;
    longitude = NoDataValue;
  }
  else
  {
    // set the default ellipsoid to WGS84
    int eId = 22;
  
    double xld = double(x_loc);
    double yld = double(y_loc);
  
    // use the converter to convert to lat and long
    double Lat,Long;
    Converter.UTMtoLL(eId, yld, xld, UTM_zone, is_North, Lat, Long);
          
  
    lat = Lat;
    longitude = Long;
  }
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//
// This function gets the UTM zone
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::get_UTM_information(int& UTM_zone, bool& is_North)
{

  // set up strings and iterators
  map<string,string>::iterator iter;

  //check to see if there is already a map info string
  string mi_key = "ENVI_map_info";
  iter = GeoReferencingStrings.find(mi_key);
  if (iter != GeoReferencingStrings.end() )
  {
    string info_str = GeoReferencingStrings[mi_key] ;

    // now parse the string
    vector<string> mapinfo_strings;
    istringstream iss(info_str);
    while( iss.good() )
    {
      string substr;
      getline( iss, substr, ',' );
      mapinfo_strings.push_back( substr );
    }
    UTM_zone = atoi(mapinfo_strings[7].c_str());
    //cout << "Line 1041, UTM zone: " << UTM_zone << endl;
    //cout << "LINE 1042 LSDRaster, N or S: " << mapinfo_strings[7] << endl;
    
    // find if the zone is in the north
    string n_str = "n";
    string N_str = "N";
    is_North = false;
    size_t found = mapinfo_strings[8].find(N_str);
    if (found!=std::string::npos)
    {
      is_North = true;
    }
    found = mapinfo_strings[8].find(n_str);
    if (found!=std::string::npos)
    {
      is_North = true;
    }
    //cout << "is_North is: " << is_North << endl;
        
  }
  else
  {
    UTM_zone = NoDataValue;
    is_North = false;
  }
  
}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-



//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This prints a chi map to csv with an area threshold in m^2
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::chi_map_to_csv(LSDFlowInfo& FlowInfo, string chi_map_fname, 
                                 float A_0, float m_over_n, float area_threshold)
{
  
  ofstream chi_map_csv_out;
  chi_map_csv_out.open(chi_map_fname.c_str());
  
  chi_map_csv_out.precision(9);
  
  float chi_coord;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;
  
  chi_map_csv_out << "latitude,longitude,chi" << endl;
  
  LSDRaster Chi = FlowInfo.get_upslope_chi_from_all_baselevel_nodes(m_over_n, A_0, area_threshold);
  
  float NDV = Chi.get_NoDataValue();

  for(int row = 0; row<NRows; row++)
  {
    for(int col = 0; col<NCols; col++)
    {
      chi_coord =  Chi.get_data_element(row,col);
      
      if (chi_coord != NDV)
      {
        get_lat_and_long_locations(row, col, latitude, longitude, Converter);
        chi_map_csv_out << latitude << "," << longitude  << "," << chi_coord << endl;
      }
    }
  }
  
  chi_map_csv_out.close();

}
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This function is for calculating a bonehead version of the chi slope
// and the chi intercept
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void LSDChiTools::chi_map_automator(LSDFlowInfo& FlowInfo, 
                                    vector<int> source_nodes,
                                    vector<int> outlet_nodes,
                                    LSDRaster& Elevation, LSDRaster& FlowDistance, 
                                    LSDRaster& DrainageArea, 
                                    float A_0, float m_over_n,
                                    int target_nodes, 
                                    int n_iterations, int skip,
                                    int minimum_segment_length, float sigma,
                                    string filename)
{
  
  // open the data file
  ofstream  chi_data_out;
  chi_data_out.open(filename.c_str());
  chi_data_out << "latitude,longitude,chi,elevation,m_chi,b_chi" << endl;
  
  // create the visited array
  //int not_visited = 0;
  //LSDIndexRaster VisitedRaster(NRows,NCols, XMinimum, YMinimum, DataResolution, NoDataValue, GeoReferencingStrings,not_visited);
  
  
  // These elements access the chi data
  vector< vector<float> > chi_m_means;
  vector< vector<float> > chi_b_means;
  vector< vector<float> > chi_coordinates;
  vector< vector<int> > chi_node_indices;
  
  // these are for the individual channels
  vector<float> these_chi_m_means;
  vector<float> these_chi_b_means;
  vector<float> these_chi_coordinates;
  vector<int> these_chi_node_indices;
  
  // these are for extracting element-wise data from the channel profiles. 
  int this_node, row,col;
  double latitude,longitude;
  LSDCoordinateConverterLLandUTM Converter;
  
  // these are maps that will store the data
  map<int,float> m_means_map;
  map<int,float> b_means_map;
  map<int,float> chi_coord_map;
  
  // the iterator for the map
  map<int,float>::iterator iter;

  float this_m_mean;
  float this_b_mean;
  float this_chi_coord;
  float this_elevation;

  // get the number of channels
  int n_channels = int(source_nodes.size());
  
  for(int chan = 0; chan<n_channels; chan++)
  {
    cout << "Sampling channel " << chan+1 << " of " << n_channels << endl;
    
    // get this particualr channel (it is a chi network with only one channel)
    LSDChiNetwork ThisChiChannel(FlowInfo, source_nodes[chan], outlet_nodes[chan], 
                                Elevation, FlowDistance, DrainageArea);
    
    // split the channel
    cout << "Splitting channels" << endl;
    ThisChiChannel.split_all_channels(A_0, m_over_n, n_iterations, skip, target_nodes, minimum_segment_length, sigma);
    
    // monte carlo sample all channels
    cout << "Entering the monte carlo sampling" << endl;
    ThisChiChannel.monte_carlo_sample_river_network_for_best_fit_after_breaks(A_0, m_over_n, n_iterations, skip, minimum_segment_length, sigma);
  
    // okay the ChiNetwork has all the data about the m vales at this stage. 
    // Get these vales and print them to a raster
    chi_m_means = ThisChiChannel.get_m_means();
    chi_b_means = ThisChiChannel.get_b_means();
    chi_coordinates = ThisChiChannel.get_chis();
    chi_node_indices = ThisChiChannel.get_node_indices();
    
    
    
    
    // now get the number of channels. This should be 1!
    int n_channels = int(chi_m_means.size());
    if (n_channels != 1)
    {
      cout << "Whoa there, I am trying to make a chi map but something seems to have gone wrong with the channel extraction."  << endl;
      cout << "I should only have one channel per look but I have " << n_channels << " channels." << endl;
    }
    
    // now get the m_means out
    these_chi_m_means = chi_m_means[0];
    these_chi_b_means = chi_b_means[0];
    these_chi_coordinates = chi_coordinates[0];
    these_chi_node_indices = chi_node_indices[0];
    
    cout << "I have " << these_chi_m_means.size() << " nodes." << endl;
    
    int n_nodes_in_channel = int(these_chi_m_means.size());
    for (int node = 0; node< n_nodes_in_channel; node++)
    {
      
      this_node =  these_chi_node_indices[node];
      //cout << "This node is " << this_node << endl;
      
      // only take the nodes that have not been found
      if (m_means_map.find(this_node) == m_means_map.end() )
      {
        //cout << "This is a new node; " << this_node << endl;
        m_means_map[this_node] = these_chi_m_means[node];
        b_means_map[this_node] = these_chi_b_means[node];
        chi_coord_map[this_node] = these_chi_coordinates[node];
      }
      else
      {
        cout << "I already have node: " << this_node << endl;
      }
    }
  }
  
  // now print the data to a file
  cout << "Now I am going to loop through the nodes." << endl;
  for (iter = m_means_map.begin(); iter!= m_means_map.end(); iter++)
  {
    
    this_node = iter->first;
    this_m_mean = m_means_map[this_node];
    this_b_mean = b_means_map[this_node];
    this_chi_coord = chi_coord_map[this_node];
    
    FlowInfo.retrieve_current_row_and_col(this_node,row,col);
    this_elevation = Elevation.get_data_element(row,col);
    get_lat_and_long_locations(row, col, latitude, longitude, Converter); 
    
    chi_data_out << latitude << ","
                 << longitude << ","
                 << this_chi_coord << ","
                 << this_elevation << ","
                 << this_m_mean << ","
                 << this_b_mean << endl;
    
  }
  chi_data_out.close();
}




#endif