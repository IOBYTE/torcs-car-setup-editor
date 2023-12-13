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

#include <string.h>
#include <GL/glui.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "cardata.h"

using namespace std;

extern GLUI_CommandLine *cmd_line; /* command line */
extern float cvmaxNew;
extern float tqmaxNew;
extern float gearbox_speed[8];
extern float gearRed100;
extern int simetricaledityn1;
extern int simetricaledityn2;
extern int simetricaledityn3;
extern std::string trackname;
extern int fullRpmScale;
extern int ac_acc_autogenerate;
extern float CVW;

void savexml( int i ); // prototipo de la funcion
void importxml( int k);
void opencardata( int i );
void savecardata( int i );
void helpSelectList(int control);
void loadTrackList(void);
void updateTrackName (int i);
void loadDriversList(void);
void updateDriverName ( void );
void loadCarsList(void);
void updateCarName ( void );
void CalcCV (void);
void CalcTQ (void);
void CalcCVTQmax ( void );
void gearMaxMin( int i );
void CalcRPM ( int i );
float interpolationTQCV (float vectx[50], float vecty[50], float vectx0);
void helpSelectList(int control);
void loadEngine(int k);
void loadEngineList ( void );
void warningMsg(int warningn);
void fileBrowserWindow(int file);
int mainAC3Dloader( int ac3d ); //AC3Dloader window
void gearboxRatioSpeed ( int k );
void gearboxRatioCalcAll ( int k );
void numberOfGearsDisable   ( void );
void scaleAllEnginePoints ( int k );
void modifyShapeOfEnginePoints ( int k );
float CalcMaxEngine(float vector[100], float vector2[100],int init, int tamv, float &revs, int real);
int calcEnginePointsLesRL (void);
void setRevsLimiterLimits ( void );
void create3dFilesNames ( int k );
void activateDeativateGraphicRanges ( int k );
void gearsAutoinertiaEff ( int k );
void cmd_line_cb (int k);
void optimalShiftPoint (int k);
int listDir (std::string dirPath, vector<string> &directori );

void cmd_line_cb (int k)
{
    std::string val="";
    int pos = -1;
    float buffer = 0.0;
    int bufferInt =0;
    std::string bufferStr = "";
    
    std::string text = cmd_line->get_text();
    if (text =="exit" || text == "quit")
      exit(0);
    else if (text.find(val="gears=")!=-1){
        pos = -1;
        bufferStr = "";
        bufferInt = 0;
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        bufferInt = atoi( bufferStr.c_str() );
        if (bufferInt <= 0) bufferInt = 1;
        if (bufferInt > 7) bufferInt = 7;
        numberOfGears=bufferInt;
        numberOfGearsDisable();
    }
    else if (text.find(val="cv=")!=-1){
        pos = -1;
        bufferStr = "";
        buffer = 0.0;
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        buffer = atof( bufferStr.c_str() );
        if (buffer <= 0.0) buffer = 50.0;
        cvmaxNew=buffer;
        scaleAllEnginePoints ( 1 );
    }

    else if (text.find(val="tq=")!=-1){
        pos = -1;
        bufferStr = "";
        buffer = 0.0;
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        buffer = atof( bufferStr.c_str() );
        if (buffer <= 0.0) buffer = 50.0;
        tqmaxNew=buffer;
        scaleAllEnginePoints ( 2 );
    }        

    else if (text.find(val="vmax=")!=-1){
        pos = -1;
        bufferStr = "";
        buffer = 0.0;
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        buffer = atof( bufferStr.c_str() );
        if (buffer <= 0.0) buffer = 150.0;
        gearbox_speed[numberOfGears]=buffer;
        gearboxRatioSpeed ( 0 );
        if (text.find(val="r="))
        {
            pos = -1;
            bufferStr = "";
            buffer = 0.0;
            pos = text.find(val);
            bufferStr.assign(text,pos+val.length(),text.length()-pos);
            buffer = atof( bufferStr.c_str() );
            if (buffer <= 0.0) buffer = 5.0;
            gearRed100=buffer;
        }            
        gearboxRatioCalcAll ( 0 );
    }

    else if (text.find(val="rl=")!=-1){
        pos = -1;
        bufferStr = "";
        buffer = 0.0;
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        buffer = atof( bufferStr.c_str() );
        if (buffer <= 0.0) buffer = 50.0;
        engineparams[2]=buffer;
        CalcCVTQmax();
    }     

    else if (text.find(val="simmetrical")!=-1){
        simetricaledityn1 = 1;
        simetricaledityn2 = 1;
        simetricaledityn3 = 1;
    }

    else if (text.find(val="carname=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        carname=bufferStr;
    }
    else if (text.find(val="carnamef=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        fullCarName=bufferStr;
    } 
    else if (text.find(val="autor=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        autorName=bufferStr;
    } 
    else if (text.find(val="autore=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        autorEmail=bufferStr;
    }
    else if (text.find(val="carcat=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        carCategory=bufferStr;
    }
    else if (text=="open"){
        opencardata( 1 );
    } 
    else if (text.find(val="open=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        carname=bufferStr;
        opencardata( 1 );
    }    
    else if (text=="opent"){
        opencardata( 2 );
    }
    else if (text.find(val="opent=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        carname=bufferStr;
        opencardata( 2 );
    }
    else if (text.find(val="openct=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        trackname=bufferStr;
        opencardata( 2 );
    } 
    else if (text.find(val="track=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        trackname=bufferStr;
    }
    else if (text=="save"){
        savecardata( 1 );
    }                
    else if (text.find(val="save=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        carname=bufferStr;
        savecardata( 1 );
    }
    else if (text=="savet"){
        savecardata( 2 );
    } 
    else if (text.find(val="savet=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        carname=bufferStr;
        savecardata( 2 );
    }
    else if (text=="xml"){
        savexml( 1 );
    } 
    else if (text.find(val="xml=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        carname=bufferStr;
        savexml( 1 );
    }
    else if (text=="import"){
        importxml( 0 );
    } 
    else if (text.find(val="import=")!=-1){
        pos = -1;
        bufferStr = "";
        pos = text.find(val);
        bufferStr.assign(text,pos+val.length(),text.length()-pos);
        carname=bufferStr;
        importxml( 0 );
    }
    else if (text=="rpmscale=full"){
        fullRpmScale = 1;
    }
    else if (text=="rpmscale=auto"){
        fullRpmScale = 0;
    }
    else if (text=="3dfilesac=auto"){
        ac_acc_autogenerate = 0;
        create3dFilesNames (0);
    }
    else if (text=="3dfilesacc=auto"){
        ac_acc_autogenerate = 1;
        create3dFilesNames (0);
    }
    else if (text=="punit=cv"){
        CVW = 735.49875;
        CalcCV();
    }
    else if (text=="punit=hp"){
        CVW = 745.69872;
        CalcCV();
    }
    else if (text=="osp"){
        optimalShiftPoint (0);
    } 
    else if (text=="dir"){
        /* list the actual directory */
        vector<string> directory;
        directory.reserve(10);
        listDir("../cars/", directory );
    }
    GLUI_Master.sync_live_all();     
}  
