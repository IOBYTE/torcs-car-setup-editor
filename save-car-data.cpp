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

extern std::string text1;
extern int   main_window;
extern CarData cardata;
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
           strcat(fichero2,cardata.carname.c_str());
           strcat(fichero2,"/");
           makedir(fichero2);
           strcat(fichero2,cardata.carname.c_str());
           strcat(fichero2,".txt");
           break;
       case 2:
           fichero2[0]='\0';
           strcat(fichero2,"car-setups/");
           strcat(fichero2,cardata.carname.c_str());
           strcat(fichero2,"/");
           makedir(fichero2);
           strcat(fichero2,cardata.carname.c_str());
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
            f << cardata.carname <<  " car-data-created-by-Torcs-Car-Setup-Editor" << endl;
            break;
       case 2:
            f << cardata.carname << " for track "<< trackname <<  " car-data-created-by-Torcs-Car-Setup-Editor" << endl;
            break;
    }
    f.close();
    }
    printValf("#TorcsCarSetupEditorVersion",TCSE_version);
    printStr("#carname",cardata.carname);
    printValf("#inertia", cardata.engine.inertia);
    printValf("#revsMaxi", cardata.engine.revsMaxi);
    printValf("#revsLimiter", cardata.engine.revsLimiter);
    printValf("#tickover", cardata.engine.tickover);
    printValf("#fuelConsFactor", cardata.engine.fuelConsFactor);
    printValf("#engineCapacity", cardata.engine.capacity);
    printStr("#engineCapacityUnits", "l");
    printVali("#engineCylinders", cardata.engine.cylinders);
    printStr("#engineShape", cardata.engine.shape[cardata.engine.curr_shape]);
    printStr("#enginePosition", cardata.engine.position[cardata.engine.curr_position]);
    printValf("#brakeLinearCoefficient", cardata.engine.brakeLinearCoefficient);
    printValf("#brakeCoefficient", cardata.engine.brakeCoefficient);
    printVal("#rpmValue",cardata.engine.rpmValue,21);
    printVal("#tqValue",cardata.engine.tqValue,21);
    printVali("#turboS",cardata.engine.turboS);
    printVal("#turbo",cardata.engine.turbo,3);
    printVali("#numberOfGears",cardata.numberOfGears);
    printValf("#gearbox_shift_time",cardata.gearbox_shift_time);
    printVal("#gearboxratio",cardata.gearboxratio,8);
    printVal("#gearboxinertia",cardata.gearboxinertia,9);
    printVal("#gearboxefficiency",cardata.gearboxefficiency,8);
    printVal("#antirollbar1",cardata.antirollbar1,3);
    printVal("#antirollbar2",cardata.antirollbar2,3);
    printVal("#suspension1",cardata.suspension1,8);
    printVal("#suspension2",cardata.suspension2,8);
    printVal("#suspension3",cardata.suspension3,8);
    printVal("#suspension4",cardata.suspension4,8);
    printVal("#aerodynamics",cardata.aerodynamics,4);
    printVal("#frontwing",cardata.frontwing,4);
    printVal("#rearwing",cardata.rearwing,4);
    printVal("#massdata",cardata.massdata,8);
    printVal("#brakesystem",cardata.brakesystem,2);
    printVal("#brake1",cardata.brake1,4);
    printVal("#brake2",cardata.brake2,4);
    printVal("#brake3",cardata.brake3,4);
    printVal("#brake4",cardata.brake4,4);
    printVal("#wheel1",cardata.wheel1,12);
    printVal("#wheel2",cardata.wheel2,12);
    printVal("#wheel3",cardata.wheel3,12);
    printVal("#wheel4",cardata.wheel4,12);
    printVal("#steer",cardata.steer,2);
    printVali("#curr_drivetrain_type",cardata.curr_drivetrain_type);
    printValf("#drivetrain",cardata.drivetrain);
    printVali("#curr_frontdifferential_type",cardata.curr_frontdifferential_type);
    printVal("#frontifferential",cardata.frontdifferential,5);
    printVali("#curr_reardifferential_type",cardata.curr_reardifferential_type);
    printVal("#reardifferential",cardata.reardifferential,5);
    printVali("#curr_centraldifferential_type",cardata.curr_centraldifferential_type);
    printVal("#centraldifferential",cardata.centraldifferential,5);
    printVal("#frontaxle",cardata.frontaxle,3);
    printVal("#rearaxle",cardata.rearaxle,3);
    printStr("#enginesample",cardata.enginesample);
    printValf("#rmpscale",cardata.rmpscale);
    printStr("#wheelTexture",cardata.wheelTexture);
    printStr("#shadowTexture",cardata.shadowTexture);
    printStr("#tachometerTexture",cardata.tachometerTexture);
    printStr("#speedometerTexture",cardata.speedometerTexture);
    printStr("#graphicenv",cardata.graphicenv);
    printVi("#tachometerMinMax",cardata.tachometerMinMax,2);
    printVi("#speedometerMinMax",cardata.speedometerMinMax,2);
    printStr("#carRange1",cardata.carRange1);
    printStr("#carRange2",cardata.carRange2);
    printStr("#carRange3",cardata.carRange3);
    printStr("#carRange4",cardata.carRange4);
    printStr("#carRange5",cardata.carRange5);

    printVali("#carRange1data",cardata.carRange1dataA);
    printVali("#carRange1data",cardata.carRange1dataB);
    printVali("#carRange1data",cardata.carRange1dataC);

    printVali("#carRange2data",cardata.carRange2dataA);
    printVali("#carRange2data",cardata.carRange2dataB);
    printVali("#carRange2data",cardata.carRange2dataC);

    printVali("#carRange3data",cardata.carRange3dataA);
    printVali("#carRange3data",cardata.carRange3dataB);
    printVali("#carRange3data",cardata.carRange3dataC);

    printVali("#carRange4data",cardata.carRange4dataA);
    printVali("#carRange4data",cardata.carRange4dataB);
    printVali("#carRange4data",cardata.carRange4dataC);

    printVali("#carRange5data",cardata.carRange5dataA);
    printVali("#carRange5data",cardata.carRange5dataB);
    printVali("#carRange5data",cardata.carRange5dataC);

    printVal("#bonnetPosition",cardata.bonnetPosition,3);
    printVal("#driverPosition",cardata.driverPosition,3);
    printStr("#carCategory",cardata.carCategory);
    printVal("#carBodyDimensions",cardata.carBodyDimensions,3);
    printVal("#carOverallDimensions",cardata.carOverallDimensions,2);

    printVal("#head1a",cardata.head1a,4);
    printVal("#head1b",cardata.head1b,4);
    printVal("#head2a",cardata.head2a,4);
    printVal("#head2b",cardata.head2b,4);
    printVal("#reara",cardata.reara,4);
    printVal("#rearb",cardata.rearb,4);
    printVal("#brakea",cardata.brakea,4);
    printVal("#brakeb",cardata.brakeb,4);
    printVal("#rearc",cardata.rearc,4);
    printVal("#reard",cardata.reard,4);
    printVal("#brakec",cardata.brakec,4);
    printVal("#braked",cardata.braked,4);
    printVal("#brake2a",cardata.brake2a,4);
    printVal("#brake2b",cardata.brake2b,4);

    printVali("#head1aS",cardata.head1aS);
    printVali("#head1bS",cardata.head1bS);
    printVali("#head2aS",cardata.head2aS);
    printVali("#head2bS",cardata.head2bS);
    printVali("#rearaS",cardata.rearaS);
    printVali("#rearbS",cardata.rearbS);
    printVali("#brakeaS",cardata.brakeaS);
    printVali("#brakebS",cardata.brakebS);
    printVali("#rearcS",cardata.rearcS);
    printVali("#reardS",cardata.reardS);
    printVali("#brakecS",cardata.brakecS);
    printVali("#brakedS",cardata.brakedS);
    printVali("#brake2aS",cardata.brake2aS);
    printVali("#brake2bS",cardata.brake2bS);

    printVali("#flameS",cardata.flameS);
    printValf("#flamePower",cardata.flamePower);
    printVal("#flame1",cardata.flame1,3);
    printVal("#flame2",cardata.flame2,3);

    printStr("#model", cardata.steeringWheel.model);
    printStr("#hiResModel", cardata.steeringWheel.hiResModel);
    printValf("#xpos", cardata.steeringWheel.xpos);
    printValf("#ypos", cardata.steeringWheel.ypos);
    printValf("#zpos", cardata.steeringWheel.zpos);
    printValf("#angle", cardata.steeringWheel.angle);
    printValf("#movt", cardata.steeringWheel.movt);

    text1 =  "Saved: ";
    text1 += fichero2;
    glutSetWindow(main_window);
    glutPostRedisplay();

    cout << "File saved: " << cardata.carname << ".txt\n" << endl;

}
