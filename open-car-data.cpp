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
#include <GL/glut.h>

#include "cardata.h"

using namespace std;

char fichero3[100];
std::string textdata;
std::string bufer;

extern CarData cardata;
extern std::string text1;
//extern int   main_window;

extern std::string trackname;
extern void CalcCV(void);
extern float TCSE_version;

extern GLUI_Listbox *list_engine_capacity_units;
extern GLUI_Listbox *list_engine_shape;
extern GLUI_Listbox *list_engine_position;

void warningMsg(int warningn);

void gearboxRatioSpeed ( int k );

void numberOfGearsDisable   ( void );

void activateDeativateGraphicRanges ( int k );

void opencardata( int j )
{
        switch ( j )
    {
       case 1:
           fichero3[0]='\0';
           strcat(fichero3,"car-setups/");
           strcat(fichero3,cardata.carname.c_str());
           strcat(fichero3,"/");
           strcat(fichero3,cardata.carname.c_str());
           strcat(fichero3,".txt");
           break;
       case 2:
           fichero3[0]='\0';
           strcat(fichero3,"car-setups/");
           strcat(fichero3,cardata.carname.c_str());
           strcat(fichero3,"/");
           strcat(fichero3,cardata.carname.c_str());
           strcat(fichero3,"_");
           strcat(fichero3,trackname.c_str());
           strcat(fichero3,".txt");
           break;
    }

    ifstream f;  //fichero de salid
    int i;
    float tcseFileVersion;
    f.open(fichero3);  //opening the file or creating it if it does not exist
    if(!f)
        {
        cout << "Error opening the file " << fichero3 << endl;
        warningMsg(1);
        }
    else
    { //operacions en el fitxer
    getline(f, bufer);
    f >> textdata; //#TcseVersion
    f >> tcseFileVersion; //#TcseVersion
    if (tcseFileVersion != TCSE_version)
    {
        warningMsg(3);
    }
    f >> textdata; //#carname
    f >> cardata.carname;
    f >> textdata; //#engineparams
    f >> cardata.engine.inertia;
    f >> cardata.engine.revsMaxi;
    f >> cardata.engine.revsLimiter;
    f >> cardata.engine.tickover;
    f >> cardata.engine.fuelConsFactor;
    f >> textdata; //#rpmValue or #engineCapacity
    if (textdata == "engineCapacity")
    {
	f >> cardata.engine.capacity;
	f >> textdata; // #engineCapacityUnits
	string engineCapacityUnits;
	f >> engineCapacityUnits;
	for (int i = 0; i < 3; i++)
	{
            if (engineCapacityUnits == cardata.engine.capacity_units[i])
	    {
		cardata.engine.curr_capacity_units = i;
		list_engine_capacity_units->set_int_val(cardata.engine.curr_capacity_units);
		break;
	    }
	}
	f >> textdata; // #engineCylinders
	f >> cardata.engine.cylinders;
	f >> textdata; // #engineShape
	string engineShape;
	f >> engineShape;
	for (int i = 0; i < 4; i++)
	{
            if (engineShape == cardata.engine.shape[i])
	    {
		cardata.engine.curr_shape = i;
		list_engine_shape->set_int_val(cardata.engine.curr_shape);
		break;
	    }
	}
	f >> textdata; // #enginePosition
	string enginePosition;
	f >> enginePosition;
	for (int i = 0; i < 5; i++)
	{
            if (enginePosition == cardata.engine.position[i])
	    {
		cardata.engine.curr_position = i;
		list_engine_position->set_int_val(cardata.engine.curr_position);
		break;
	    }
	}
	f >> textdata; //#rpmValue or #brakeLinearCoefficient
    }
    if (textdata == "brakeLinearCoefficient")
    {
        f >> cardata.engine.brakeLinearCoefficient;
        f >> textdata; //#"brakeCoefficient"
        f >> cardata.engine.brakeCoefficient;
        f >> textdata; //#rpmValue
    }
    for (i=0; i<21; i++) {f >> cardata.engine.rpmValue[i];}
    f >> textdata; //#tqValue
    for (i=0; i<21; i++) {f >> cardata.engine.tqValue[i];}
    CalcCV(); // after reading the tq we calculate the cv to update the values of cv
    f >> textdata; //#turboS
    f >> cardata.engine.turboS;
    f >> textdata; //#turbo
    for (i=0; i<3; i++) {f >> cardata.engine.turbo[i];}
    f >> textdata; //#numberOfGears
    f >> cardata.numberOfGears;
    numberOfGearsDisable( );
    f >> textdata; //#gearbox_shift_time
    f >> cardata.gearbox_shift_time;
    f >> textdata; //#gearboxratio
    for (i=0; i<8; i++) {f >> cardata.gearboxratio[i];}
    f >> textdata; //#gearboxinertia
    for (i=0; i<9; i++) {f >> cardata.gearboxinertia[i];}
    f >> textdata; //#gearboxefficiency
    for (i=0; i<8; i++) {f >> cardata.gearboxefficiency[i];}
    f >> textdata; //#antirollbar1
    for (i=0; i<3; i++) {f >> cardata.antirollbar1[i];}
    f >> textdata; //#antirollbar2
    for (i=0; i<3; i++) {f >> cardata.antirollbar2[i];}
    f >> textdata; //#suspension1
    for (i=0; i<8; i++) {f >> cardata.suspension1[i];}
    f >> textdata; //#suspension2
    for (i=0; i<8; i++) {f >> cardata.suspension2[i];}
    f >> textdata; //#suspension3
    for (i=0; i<8; i++) {f >> cardata.suspension3[i];}
    f >> textdata; //#suspension4
    for (i=0; i<8; i++) {f >> cardata.suspension4[i];}
    f >> textdata; //#aerodynamics
    for (i=0; i<4; i++) {f >> cardata.aerodynamics[i];}
    f >> textdata; //#frontwing
    for (i=0; i<4; i++) {f >> cardata.frontwing[i];}
    f >> textdata; //#rearwing
    for (i=0; i<4; i++) {f >> cardata.rearwing[i];}
    f >> textdata; //#massdata
    for (i=0; i<8; i++) {f >> cardata.massdata[i];}
    f >> textdata; //#brakesystem
    for (i=0; i<2; i++) {f >> cardata.brakesystem[i];}
    f >> textdata; //#brake1
    for (i=0; i<4; i++) {f >> cardata.brake1[i];}
    f >> textdata; //#brake2
    for (i=0; i<4; i++) {f >> cardata.brake2[i];}
    f >> textdata; //#brake3
    for (i=0; i<4; i++) {f >> cardata.brake3[i];}
    f >> textdata; //#brake4
    for (i=0; i<4; i++) {f >> cardata.brake4[i];}
    f >> textdata; //#wheel1
    for (i=0; i<12; i++) {f >> cardata.wheel1[i];}
    f >> textdata; //#wheel2
    for (i=0; i<12; i++) {f >> cardata.wheel2[i];}
    f >> textdata; //#wheel3
    for (i=0; i<12; i++) {f >> cardata.wheel3[i];}
    f >> textdata; //#wheel4
    for (i=0; i<12; i++) {f >> cardata.wheel4[i];}
    f >> textdata; //#steer
    for (i=0; i<2; i++) {f >> cardata.steer[i];}
    f >> textdata; //#curr_drivetrain_type
    f >> cardata.curr_drivetrain_type;
    f >> textdata; //#drivetrain
    f >> cardata.drivetrain;
    f >> textdata; //#curr_frontdifferential_type
    f >> cardata.curr_frontdifferential_type;
    f >> textdata; //#frontdifferential
    for (i=0; i<5; i++) {f >> cardata.frontdifferential[i];}
    f >> textdata; //#curr_frontdifferential_type
    f >> cardata.curr_frontdifferential_type;
    f >> textdata; //#reardifferential
    for (i=0; i<5; i++) {f >> cardata.reardifferential[i];}
    f >> textdata; //#curr_centraldifferential_type
    f >> cardata.curr_centraldifferential_type;
    f >> textdata; //#centraldifferential
    for (i=0; i<5; i++) {f >> cardata.centraldifferential[i];}
    f >> textdata; //#frontaxle
    for (i=0; i<3; i++) {f >> cardata.frontaxle[i];}
    f >> textdata; //#rearaxle
    for (i=0; i<3; i++) {f >> cardata.rearaxle[i];}
    f >> textdata; //#enginesample
    f >> cardata.enginesample;
    f >> textdata; //#rmpscale
    f >> cardata.rmpscale;
    f >> textdata; //#wheelTexture
    f >> cardata.wheelTexture;
    f >> textdata; //#shadowTexture
    f >> cardata.shadowTexture;
    f >> textdata; //#tachometerTexture
    f >> cardata.tachometerTexture;
    f >> textdata; //#speedometerTexture
    f >> cardata.speedometerTexture;
    f >> textdata; //#graphicenv
    f >> cardata.graphicenv;
    f >> textdata; //#tachometerMinMax
    for (i=0; i<2; i++) {f >> cardata.tachometerMinMax[i];}
    f >> textdata; //#speedometerMinMax
    for (i=0; i<2; i++) {f >> cardata.speedometerMinMax[i];}
    f >> textdata; //#carRange1
    f >> cardata.carRange1;
    f >> textdata; //#carRange2
    f >> cardata.carRange2;
    f >> textdata; //#carRange3
    f >> cardata.carRange3;
    f >> textdata; //#carRange4
    f >> cardata.carRange4;
    f >> textdata; //#carRange5
    f >> cardata.carRange5;
    f >> textdata; //#carRange1data
    f >> cardata.carRange1dataA;
    f >> textdata; //#carRange1data
    f >> cardata.carRange1dataB;
    f >> textdata; //#carRange1data
    f >> cardata.carRange1dataC;
    f >> textdata; //#carRange2data
    f >> cardata.carRange2dataA;
    f >> textdata; //#carRange2data
    f >> cardata.carRange2dataB;
    f >> textdata; //#carRange2data
    f >> cardata.carRange2dataC;
    f >> textdata; //#carRange3data
    f >> cardata.carRange3dataA;
    f >> textdata; //#carRange3data
    f >> cardata.carRange3dataB;
    f >> textdata; //#carRange3data
    f >> cardata.carRange3dataC;
    f >> textdata; //#carRange4data
    f >> cardata.carRange4dataA;
    f >> textdata; //#carRange4data
    f >> cardata.carRange4dataB;
    f >> textdata; //#carRange4data
    f >> cardata.carRange4dataC;
    f >> textdata; //#carRange5data
    f >> cardata.carRange5dataA;
    f >> textdata; //#carRange5data
    f >> cardata.carRange5dataB;
    f >> textdata; //#carRange5data
    f >> cardata.carRange5dataC;

    activateDeativateGraphicRanges ( 0 );

    f >> textdata; //#bonnetPosition
    for (i=0; i<3; i++) {f >> cardata.bonnetPosition[i];}
    f >> textdata; //#driverPosition
    for (i=0; i<3; i++) {f >> cardata.driverPosition[i];}
    f >> textdata; //#carCategory
    f >> cardata.carCategory;
    f >> textdata; //#carBodyDimensions
    for (i=0; i<3; i++) {f >> cardata.carBodyDimensions[i];}
    f >> textdata; //#carOverallDimensions
    for (i=0; i<2; i++) {f >> cardata.carOverallDimensions[i];}

    f >> textdata; //#head1a
    for (i=0; i<4; i++) {f >> cardata.head1a[i];}
    f >> textdata; //#head1b
    for (i=0; i<4; i++) {f >> cardata.head1b[i];}
    f >> textdata; //#head2a
    for (i=0; i<4; i++) {f >> cardata.head2a[i];}
    f >> textdata; //#head2b
    for (i=0; i<4; i++) {f >> cardata.head2b[i];}
    f >> textdata; //#reara
    for (i=0; i<4; i++) {f >> cardata.reara[i];}
    f >> textdata; //#rearb
    for (i=0; i<4; i++) {f >> cardata.rearb[i];}
    f >> textdata; //#brakea
    for (i=0; i<4; i++) {f >> cardata.brakea[i];}
    f >> textdata; //#brakeb
    for (i=0; i<4; i++) {f >> cardata.brakeb[i];}
    f >> textdata; //#rearc
    for (i=0; i<4; i++) {f >> cardata.rearc[i];}
    f >> textdata; //#reard
    for (i=0; i<4; i++) {f >> cardata.reard[i];}
    f >> textdata; //#brakec
    for (i=0; i<4; i++) {f >> cardata.brakec[i];}
    f >> textdata; //#braked
    for (i=0; i<4; i++) {f >> cardata.braked[i];}
    f >> textdata; //#brake2a
    for (i=0; i<4; i++) {f >> cardata.brake2a[i];}
    f >> textdata; //#brake2b
    for (i=0; i<4; i++) {f >> cardata.brake2b[i];}

    f >> textdata; //#head1aS
    f >> cardata.head1aS;
    f >> textdata; //#head1bS
    f >> cardata.head1bS;
    f >> textdata; //#head2aS
    f >> cardata.head2aS;
    f >> textdata; //#head2bS
    f >> cardata.head2bS;
    f >> textdata; //#rearaS
    f >> cardata.rearaS;
    f >> textdata; //#rearbS
    f >> cardata.rearbS;
    f >> textdata; //#brakeaS
    f >> cardata.brakeaS;
    f >> textdata; //#brakebS
    f >> cardata.brakebS;
    f >> textdata; //#rearcS
    f >> cardata.rearcS;
    f >> textdata; //#reardS
    f >> cardata.reardS;
    f >> textdata; //#brakeS
    f >> cardata.brakecS;
    f >> textdata; //#brakedS
    f >> cardata.brakedS;
    f >> textdata; //#brake2aS
    f >> cardata.brake2aS;
    f >> textdata; //#brake2bS
    f >> cardata.brake2bS;

    f >> textdata; //#flamesS
    f >> cardata.flameS;
    f >> textdata; //#flamePower
    f >> cardata.flamePower;
    f >> textdata; //#flame1
    for (i=0; i<3; i++) {f >> cardata.flame1[i];}
    f >> textdata; //#flame2
    for (i=0; i<3; i++) {f >> cardata.flame2[i];}

    f.close();  //cierre del fichero

    gearboxRatioSpeed ( 1 );



    cout << "File loaded:" << cardata.carname << ".txt\n" << endl;

    text1 =  "Loaded: ";
    text1 += fichero3;

    GLUI_Master.sync_live_all();
    glutPostRedisplay(); /* we call this function to display the changes
      because the  GLUI_Master.sync_live_all(); doen'nt update the
      glut window if no variable is updated*/

    }
}

