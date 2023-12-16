/*  Torcs Car Setup Editor. A visual car setup editor for torcs build using glui
    Copyright (C) 2007  Vicente Marti

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

#include <iostream>
#include <fstream>
#include <GL/glui.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include "cardata.h"

using namespace std;

int listDir (std::string dirPath, vector<string> &directori );

extern vector<string> tracksRoad;
extern vector<string> tracksOval;
extern vector<string> tracksDirt;
extern vector<string> tracksGprix;
extern int tracksRoadVal;
extern int tracksOvalVal;
extern int tracksDirtVal;
extern int tracksGprixVal;

extern vector<string> carsList;
extern int carsListVal;
 
extern int trackVal;
extern std::string trackname;
//extern char *tracks[100];
extern std::string track1;
extern char track2[100];
extern char track3[100];

extern vector<string> driversList;
extern std::string driverName;
extern int driverVal;

void loadTrackList(void)
{
    string trackName;
    
     /* -- open tracks Road -- */   
    tracksRoad.reserve(30);
    ifstream f;  //fichero de salid
    f.open("data/tracksRoad.txt");  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "The file tracks.txt is not correct" << endl;
    else
    {
    int i;
    while (!f.eof()!=0)
        {
        f >> trackName;
        tracksRoad.push_back(trackName);
        i++;
        }       
    }
    f.close();
    
    /* -- open tracks Oval -- */
    tracksOval.reserve(30);
    ifstream f2;  //fichero de salid
    f2.open("data/tracksOval.txt");  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "The file tracksRoad.txt is not correct" << endl;
    else
    {
    int i;
    while (!f2.eof()!=0)
        {
        f2 >> trackName;
        tracksOval.push_back(trackName);
        i++;
        }       
    }
    f2.close();
    
    /* -- open tracks Dirt -- */
    tracksOval.reserve(30);
    ifstream f3;
    f3.open("data/tracksDirt.txt");
    if(!f)
        cout << "The file tracksRoad.txt is not correct" << endl;
    else
    {
    int i;
    while (!f3.eof()!=0)
        {
        f3 >> trackName;
        tracksDirt.push_back(trackName);
        i++;
        }       
    }
    f3.close();
    
    /* -- open tracks Gprix -- */
    tracksGprix.reserve(30);
    ifstream f4;
    f4.open("data/tracksDirt.txt");
    if(!f)
        cout << "The file tracksRoad.txt is not correct" << endl;
    else
    {
    int i;
    while (!f4.eof()!=0)
        {
        f4 >> trackName;
        tracksGprix.push_back(trackName);
        i++;
        }       
    }
    f4.close();
}

void updateTrackName (int i) /* updates the track name from list --> text box*/
{
     switch ( i )
     {
     case 1: trackname = tracksRoad[tracksRoadVal]; break;
     case 2: trackname = tracksOval[tracksOvalVal]; break;
     case 3: trackname = tracksDirt[tracksDirtVal]; break;
     case 4: trackname = tracksGprix[tracksGprixVal]; break;
     }    
          
     GLUI_Master.sync_live_all();
}

void loadCarsList(void)
{
    string carsName;
      
    carsList.reserve(30);
    ifstream f;  //fichero de salid
    f.open("data/carsList.txt");  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "The file carsList.txt is not correct" << endl;
    else
    {
    int i;
    while (!f.eof()!=0)
        {
        f >> carsName;
        carsList.push_back(carsName);
        i++;
        }       
    }
    f.close();
    /* load the carnames of the torcs/cars/ folder */
    cout << "Loading the ../cars/ carsList" << endl;
    listDir("../cars/", carsList );
}
void updateCarName ( void )
{
     carname = carsList[carsListVal];
     GLUI_Master.sync_live_all();
}

void loadDriversList(void)
{
    string driverName;
      
    driversList.reserve(30);
    ifstream f;  //fichero de salid
    f.open("data/dirversList.txt");  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "The file dirversList.txt is not correct" << endl;
    else
    {
    int i;
    while (!f.eof()!=0)
        {
        f >> driverName;
        driversList.push_back(driverName);
        i++;
        }       
    }
    f.close();
}
void updateDriverName ( void )
{
     driverName = driversList[driverVal];
     GLUI_Master.sync_live_all();
}
