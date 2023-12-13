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

using namespace std;

#include "cardata.h"

extern int ac_acc_autogenerate;

extern GLUI_EditText *carRangeEditText[5];
extern GLUI_Spinner *carRangeSpinner[5];
extern GLUI_Checkbox *carRangeCheckbox[5];

void create3dFilesNames ( int k )
{
    std::string fileType="";
    
    if (ac_acc_autogenerate == 0)
        fileType = ".ac";
    if (ac_acc_autogenerate == 1)
        fileType = ".acc";
    
    graphicenv = carname+fileType;
    carRange1= carname+fileType;
    carRange2= carname+"-lod1"+fileType;
    carRange3= carname+"-lod2"+fileType;
    carRange4= carname+"-lod3"+fileType;
    carRange5= carname+"-lod4"+fileType;
    
    GLUI_Master.sync_live_all();
}

void activateDeativateGraphicRanges ( int k )
{
    //cout << carRange1dataC <<"-"<< carRange2dataC <<"-"<< carRange3dataC<<"-"<< carRange4dataC << "-"<< carRange5dataC << endl;  
switch (k)
{    
 case 1:
    carRange1dataC = 1;
    carRange2dataC = 0;
    carRange3dataC = 0;
    carRange4dataC = 0;
    carRange5dataC = 0;
 break;
 
 case 2:
    carRange1dataC = 1;
    //carRange2dataC = 1;
    carRange3dataC = 0;
    carRange4dataC = 0;
    carRange5dataC = 0;
 break;

 case 3:
    carRange1dataC = 1;
    carRange2dataC = 1;
    //carRange3dataC = 1;
    carRange4dataC = 0;
    carRange5dataC = 0;
 break;
 
 case 4:
    carRange1dataC = 1;
    carRange2dataC = 1;
    carRange3dataC = 1;
    //carRange4dataC = 1;
    carRange5dataC = 0;
 break;
 
 case 5:
    carRange1dataC = 1;
    carRange2dataC = 1;
    carRange3dataC = 1;
    carRange4dataC = 1;
    //carRange5dataC = 1;
 break;    
    GLUI_Master.sync_live_all();
  
}
    
    if (carRange1dataC==0)
    {
        carRangeEditText[0]->disable();
        carRangeSpinner[0]->disable();
        carRangeCheckbox[0]->disable();
    }
    else
    {    
        carRangeEditText[0]->enable();
        carRangeSpinner[0]->enable();
        carRangeCheckbox[0]->enable();
    }
    

    if (carRange2dataC==0)
    {
        carRangeEditText[1]->disable();
        carRangeSpinner[1]->disable();
        carRangeCheckbox[1]->disable();
    }
    else
    {    
        carRangeEditText[1]->enable();
        carRangeSpinner[1]->enable();
        carRangeCheckbox[1]->enable();
    } 


    if (carRange3dataC==0)
    {
        carRangeEditText[2]->disable();
        carRangeSpinner[2]->disable();
        carRangeCheckbox[2]->disable();
    }
    else
    {    
        carRangeEditText[2]->enable();
        carRangeSpinner[2]->enable();
        carRangeCheckbox[2]->enable();
    } 


    if (carRange4dataC==0)
    {
        carRangeEditText[3]->disable();
        carRangeSpinner[3]->disable();
        carRangeCheckbox[3]->disable();
    }
    else
    {    
        carRangeEditText[3]->enable();
        carRangeSpinner[3]->enable();
        carRangeCheckbox[3]->enable();
    } 

    if (carRange5dataC==0)
    {
        carRangeEditText[4]->disable();
        carRangeSpinner[4]->disable();
        carRangeCheckbox[4]->disable();
    }
    else
    {    
        carRangeEditText[4]->enable();
        carRangeSpinner[4]->enable();
        carRangeCheckbox[4]->enable();
    } 

   
    GLUI_Master.sync_live_all();
}    
