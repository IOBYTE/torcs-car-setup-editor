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

extern void CalcCV (void);
extern GLUI_Listbox *loadEngine_List;
extern int currentEngine;
vector<string> engineList;

void loadEngine( int k )
{
    std::string engineName = "";
    std::string filePath = "data/enginesCV.txt";
    engineName = engineList.at(currentEngine);
    
    std::string buffer = "";
    
    ifstream f;  //fichero de salid
   
    f.open(filePath.c_str());  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "Error opening the " << filePath << " file." << endl;
    else
    {
        int findedEngine = -1;
        while (!f.eof()!=0)
        {
            getline(f, buffer);
            //f.ignore(); // igonere the '\n'
            findedEngine = buffer.find( engineName );
             //cout << "searching  " << currentEngine << " - " << engineName << endl;
            if (findedEngine >= 0)
            {    
                int i; 
                f >> buffer; //#engineparams
                for (i=0; i<5; i++) {f >> engineparams[i];}
                f >> buffer; //#rpmValue
                for (i=0; i<21; i++) {f >> rpmValue[i];}
                f >> buffer; //#tqValue
                for (i=0; i<21; i++) {f >> tqValue[i];}
                CalcCV(); // after reading the tq we calculate the cv to update the values of cv
                f >> buffer; //#turboS
                f >> turboS;
                f >> buffer; //#turbo
                for (i=0; i<3; i++) {f >> turbo[i];}
                GLUI_Master.sync_live_all();
                cout << "Engine Loaded " << currentEngine << " - " << engineList.at(currentEngine)<< endl;
            }        
        }
    f.close();    
    }        
}

void loadEngineList ( void )
{
    std::string engineName = "";
    std::string filePath = "data/enginesCV.txt";
    int engineItemStart = -1;
    int engineItemEnd = -1;
    
    std::string buffer = "";
    
    ifstream f;  //fichero de salid
   
    f.open(filePath.c_str());  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "Error opening the " << filePath << " file." << endl;
    else
    {
        int k = 0;
        while (!f.eof()!=0)
        {
            getline(f, buffer);
    
            engineItemStart = buffer.find("[");
            engineItemEnd = buffer.find("]");
    
            if (engineItemStart >= 0 && engineItemEnd >= 0)
            {
            engineList.push_back(buffer);
            engineName.assign(buffer,engineItemStart+1,engineItemEnd-engineItemStart-1);
            loadEngine_List->add_item( k, engineName.c_str() );
            GLUI_Master.sync_live_all();
            k++;
            }
        }
    }            
}        
