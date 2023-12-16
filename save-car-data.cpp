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

#include <GL/glui.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include "cardata.h"
#include "portability.h"

using namespace std;

extern std::string trackname;
extern float TCSE_version;

char fichero2[150];

void printVal (std::string section, float vect[100], int imax)
{
    ofstream f;
    f.open(fichero2, ios::app);
    if(!f)
        cout << "Error obrint el fitxer" << endl;
    else
    {
    int i;
    f << section << endl;
    for (i=0; i<imax; i++)
    {
    f << vect[i] << " ";
    }
    f << endl;
    f.close();
    }         
}

void printVi (std::string section, int vect[100], int imax)
{
    ofstream f;
    f.open(fichero2, ios::app);
    if(!f)
        cout << "Error obrint el fitxer" << endl;
    else
    {
    int i;
    f << section << endl;
    for (i=0; i<imax; i++)
    {
    f << vect[i] << " ";
    }
    f.close();
    }         
}

void printVali (std::string section, int value)
{
    ofstream f;
    f.open(fichero2, ios::app);
    if(!f)
        cout << "Error obrint el fitxer" << endl;
    else
    {
    int i;
    f << section << endl;
    f << value << endl;
    f.close();
    }         
} 

void printValf (std::string section, float value)
{
    ofstream f;
    f.open(fichero2, ios::app);
    if(!f)
        cout << "Error obrint el fitxer" << endl;
    else
    {
    int i;
    f << section << endl;
    f << value << endl;
    f.close();
    }         
} 

void printChar (std::string section, char vect[100])
{
    ofstream f;
    f.open(fichero2, ios::app);
    if(!f)
        cout << "Error obrint el fitxer" << endl;
    else
    {
    int i;
    f << section << endl;
    f << vect << endl;
    f.close();
    }         
}         



void printStr (std::string section, std::string mystring)
{
    ofstream f;
    f.open(fichero2, ios::app);
    if(!f)
        cout << "Error obrint el fitxer" << endl;
    else
    {
    int i;
    f << section << endl;
    f << mystring << endl;
    f.close();
    }         
}


void savecardata( int i )
{
    switch ( i )
    {
       case 1:
           fichero2[0]='\0';
           strcat(fichero2,"car-setups/");
           strcat(fichero2,carname.c_str());
           strcat(fichero2,"/");
           makedir(fichero2);
           strcat(fichero2,carname.c_str());
           strcat(fichero2,".txt");
           break;
       case 2:
           fichero2[0]='\0';
           strcat(fichero2,"car-setups/");
           strcat(fichero2,carname.c_str());
           strcat(fichero2,"/");
           makedir(fichero2);
           strcat(fichero2,carname.c_str());
           strcat(fichero2,"_");
           strcat(fichero2,trackname.c_str());
           strcat(fichero2,".txt");
           break;
    }    
    
    ofstream f;  //fichero de salid

    f.open(fichero2);  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "Error obrint el fitxer" << endl;
    else
    {          
    switch ( i )
    {
       case 1:
            f << carname <<  " car-data-created-by-Torcs-Car-Setup-Editor" << endl;
            break;
       case 2:
            f << carname << " for track "<< trackname <<  " car-data-created-by-Torcs-Car-Setup-Editor" << endl;
            break;
    }
    f.close();
    }
    printValf("#TorcsCarSetupEditorVersion",TCSE_version);
    printStr("#carname",carname);  
    printVal("#engineparams",engineparams,5);
    printVali("engineCapacity", engineCapacity);
    printStr("#engineCapacityUnits", engine_capacity_units[curr_engine_capacity_units]);
    printValf("#engineCylinders", engineCylinders);
    printStr("#engineShape", engine_shape[curr_engine_shape]);
    printStr("#enginePosition", engine_position[curr_engine_position]);	 
    printVal("#rpmValue",rpmValue,21);
    printVal("#tqValue",tqValue,21);
    printVali("#turboS",turboS);
    printVal("#turbo",turbo,3);
    printVali("#numberOfGears",numberOfGears);
    printValf("#gearbox_shift_time",gearbox_shift_time);
    printVal("#gearboxratio",gearboxratio,8);
    printVal("#gearboxinertia",gearboxinertia,9);
    printVal("#gearboxefficiency",gearboxefficiency,8); 
    printVal("#antirollbar1",antirollbar1,3);
    printVal("#antirollbar2",antirollbar2,3);
    printVal("#suspension1",suspension1,8);
    printVal("#suspension2",suspension2,8);
    printVal("#suspension3",suspension3,8);
    printVal("#suspension4",suspension4,8);
    printVal("#aerodynamics",aerodynamics,4);
    printVal("#frontwing",frontwing,4);
    printVal("#rearwing",rearwing,4);
    printVal("#massdata",massdata,8);
    printVal("#brakesystem",brakesystem,2);
    printVal("#brake1",brake1,4);
    printVal("#brake2",brake2,4);
    printVal("#brake3",brake3,4);
    printVal("#brake4",brake4,4);
    printVal("#wheel1",wheel1,12);
    printVal("#wheel2",wheel2,12);
    printVal("#wheel3",wheel3,12);
    printVal("#wheel4",wheel4,12);
    printVal("#steer",steer,2);
    printVali("#curr_drivetrain_type",curr_drivetrain_type);
    printValf("#drivetrain",drivetrain);
    printVali("#curr_frontdifferential_type",curr_frontdifferential_type);
    printVal("#frontifferential",frontdifferential,5);
    printVali("#curr_reardifferential_type",curr_reardifferential_type);
    printVal("#reardifferential",reardifferential,5);
    printVali("#curr_centraldifferential_type",curr_centraldifferential_type);
    printVal("#centraldifferential",centraldifferential,5);
    printVal("#frontaxle",frontaxle,3);
    printVal("#rearaxle",rearaxle,3);
    printStr("#enginesample",enginesample);
    printValf("#rmpscale",rmpscale);
    printStr("#wheelTexture",wheelTexture);
    printStr("#shadowTexture",shadowTexture);
    printStr("#tachometerTexture",tachometerTexture);
    printStr("#speedometerTexture",speedometerTexture);
    printStr("#graphicenv",graphicenv);
    printVi("#tachometerMinMax",tachometerMinMax,2);
    printVi("#speedometerMinMax",speedometerMinMax,2);
    printStr("#carRange1",carRange1);
    printStr("#carRange2",carRange2);
    printStr("#carRange3",carRange3);
    printStr("#carRange4",carRange4);
    printStr("#carRange5",carRange5);
    
    printVali("#carRange1data",carRange1dataA);
    printVali("#carRange1data",carRange1dataB);
    printVali("#carRange1data",carRange1dataC);
        
    printVali("#carRange2data",carRange2dataA);
    printVali("#carRange2data",carRange2dataB);
    printVali("#carRange2data",carRange2dataC);
    
    printVali("#carRange3data",carRange3dataA);
    printVali("#carRange3data",carRange3dataB);
    printVali("#carRange3data",carRange3dataC);
    
    printVali("#carRange4data",carRange4dataA);
    printVali("#carRange4data",carRange4dataB);
    printVali("#carRange4data",carRange4dataC);
    
    printVali("#carRange5data",carRange5dataA);
    printVali("#carRange5data",carRange5dataB);
    printVali("#carRange5data",carRange5dataC);
    
    printVal("#bonnetPosition",bonnetPosition,3);
    printVal("#driverPosition",driverPosition,3);
    printStr("#carCategory",carCategory);
    printVal("#carBodyDimensions",carBodyDimensions,3);
    printVal("#carOverallDimensions",carOverallDimensions,2);

    printVal("#head1a",head1a,4);
    printVal("#head1b",head1b,4);
    printVal("#head2a",head2a,4);
    printVal("#head2b",head2b,4);
    printVal("#reara",reara,4);
    printVal("#rearb",rearb,4);
    printVal("#brakea",brakea,4);
    printVal("#brakeb",brakeb,4);
    printVal("#rearc",rearc,4);
    printVal("#reard",reard,4);
    printVal("#brakec",brakec,4);
    printVal("#braked",braked,4);
    printVal("#brake2a",brake2a,4);
    printVal("#brake2b",brake2b,4);

    printVali("#head1aS",head1aS);
    printVali("#head1bS",head1bS);
    printVali("#head2aS",head2aS);
    printVali("#head2bS",head2bS);
    printVali("#rearaS",rearaS);
    printVali("#rearbS",rearbS);
    printVali("#brakeaS",brakeaS);
    printVali("#brakebS",brakebS);
    printVali("#rearcS",rearcS);
    printVali("#reardS",reardS);
    printVali("#brakecS",brakecS);
    printVali("#brakedS",brakedS);
    printVali("#brake2aS",brake2aS);
    printVali("#brake2bS",brake2bS);
    
    printVali("#flameS",flameS);
    printValf("#flamePower",flamePower);
    printVal("#flame1",flame1,3);
    printVal("#flame2",flame2,3);


    text1 =  "Saved: ";
    text1 += fichero2;
    glutSetWindow(main_window);
    glutPostRedisplay();
    
    cout << "File saved: " << carname << ".txt\n" << endl;
     
}    
