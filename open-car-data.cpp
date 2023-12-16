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
           strcat(fichero3,carname.c_str());
           strcat(fichero3,"/");
           strcat(fichero3,carname.c_str());
           strcat(fichero3,".txt");
           break;
       case 2:
           fichero3[0]='\0';
           strcat(fichero3,"car-setups/");
           strcat(fichero3,carname.c_str());
           strcat(fichero3,"/");
           strcat(fichero3,carname.c_str());
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
    //getVal("#engineparams",engineparams,5);
    //f >> textdata; //1st line
    getline(f, bufer);
    f >> textdata; //#TcseVersion
    f >> tcseFileVersion; //#TcseVersion
    if (tcseFileVersion != TCSE_version)
    {
        warningMsg(3);
    }
    f >> textdata; //#carname
    f >> carname;
    f >> textdata; //#engineparams
    for (i=0; i<5; i++) {f >> engineparams[i];}
    f >> textdata; //#rpmValue or #engineCapacity
    if (textdata == "engineCapacity")
    {
	f >> engineCapacity;
	f >> textdata; // #engineCapacityUnits
	string engineCapacityUnits;
	f >> engineCapacityUnits;
	for (int i = 0; i < 3; i++)
	{
            if (engineCapacityUnits == engine_capacity_units[i])
	    {
		curr_engine_capacity_units = i;
		list_engine_capacity_units->set_int_val(curr_engine_capacity_units);
		break;
	    }
	}
	f >> textdata; // #engineCylinders
	f >> engineCylinders;
	f >> textdata; // #engineShape
	string engineShape;
	f >> engineShape;
	for (int i = 0; i < 4; i++)
	{
            if (engineShape == engine_shape[i])
	    {
		curr_engine_shape = i;
		list_engine_shape->set_int_val(curr_engine_shape);
		break;
	    }
	}
	f >> textdata; // #enginePosition
	string enginePosition;
	f >> enginePosition;
	for (int i = 0; i < 5; i++)
	{
            if (enginePosition == engine_position[i])
	    {
		curr_engine_position = i;
		list_engine_position->set_int_val(curr_engine_position);
		break;
	    }
	}
    }
    for (i=0; i<21; i++) {f >> rpmValue[i];}
    f >> textdata; //#tqValue
    for (i=0; i<21; i++) {f >> tqValue[i];}
    CalcCV(); // after reading the tq we calculate the cv to update the values of cv
    f >> textdata; //#turboS
    f >> turboS;
    f >> textdata; //#turbo
    for (i=0; i<3; i++) {f >> turbo[i];}
    f >> textdata; //#numberOfGears
    f >> numberOfGears;
    numberOfGearsDisable( );
    f >> textdata; //#gearbox_shift_time
    f >> gearbox_shift_time;
    f >> textdata; //#gearboxratio
    for (i=0; i<8; i++) {f >> gearboxratio[i];}
    f >> textdata; //#gearboxinertia
    for (i=0; i<9; i++) {f >> gearboxinertia[i];}
    f >> textdata; //#gearboxefficiency
    for (i=0; i<8; i++) {f >> gearboxefficiency[i];}
    f >> textdata; //#antirollbar1
    for (i=0; i<3; i++) {f >> antirollbar1[i];}
    f >> textdata; //#antirollbar2
    for (i=0; i<3; i++) {f >> antirollbar2[i];}
    f >> textdata; //#suspension1
    for (i=0; i<8; i++) {f >> suspension1[i];}
    f >> textdata; //#suspension2
    for (i=0; i<8; i++) {f >> suspension2[i];}
    f >> textdata; //#suspension3
    for (i=0; i<8; i++) {f >> suspension3[i];}
    f >> textdata; //#suspension4
    for (i=0; i<8; i++) {f >> suspension4[i];}
    f >> textdata; //#aerodynamics
    for (i=0; i<4; i++) {f >> aerodynamics[i];}        
    f >> textdata; //#frontwing
    for (i=0; i<4; i++) {f >> frontwing[i];} 
    f >> textdata; //#rearwing
    for (i=0; i<4; i++) {f >> rearwing[i];} 
    f >> textdata; //#massdata
    for (i=0; i<8; i++) {f >> massdata[i];}
    f >> textdata; //#brakesystem
    for (i=0; i<2; i++) {f >> brakesystem[i];}  
    f >> textdata; //#brake1
    for (i=0; i<4; i++) {f >> brake1[i];} 
    f >> textdata; //#brake2
    for (i=0; i<4; i++) {f >> brake2[i];}
    f >> textdata; //#brake3
    for (i=0; i<4; i++) {f >> brake3[i];}
    f >> textdata; //#brake4
    for (i=0; i<4; i++) {f >> brake4[i];}
    f >> textdata; //#wheel1
    for (i=0; i<12; i++) {f >> wheel1[i];} 
    f >> textdata; //#wheel2
    for (i=0; i<12; i++) {f >> wheel2[i];} 
    f >> textdata; //#wheel3
    for (i=0; i<12; i++) {f >> wheel3[i];} 
    f >> textdata; //#wheel4
    for (i=0; i<12; i++) {f >> wheel4[i];} 
    f >> textdata; //#steer
    for (i=0; i<2; i++) {f >> steer[i];}
    f >> textdata; //#curr_drivetrain_type
    f >> curr_drivetrain_type;
    f >> textdata; //#drivetrain
    f >> drivetrain;
    f >> textdata; //#curr_frontdifferential_type
    f >> curr_frontdifferential_type;
    f >> textdata; //#frontdifferential
    for (i=0; i<5; i++) {f >> frontdifferential[i];}
    f >> textdata; //#curr_frontdifferential_type
    f >> curr_frontdifferential_type;
    f >> textdata; //#reardifferential
    for (i=0; i<5; i++) {f >> reardifferential[i];}
    f >> textdata; //#curr_centraldifferential_type
    f >> curr_centraldifferential_type;
    f >> textdata; //#centraldifferential
    for (i=0; i<5; i++) {f >> centraldifferential[i];}
    f >> textdata; //#frontaxle
    for (i=0; i<3; i++) {f >> frontaxle[i];}
    f >> textdata; //#rearaxle
    for (i=0; i<3; i++) {f >> rearaxle[i];}
    f >> textdata; //#enginesample
    f >> enginesample;
    f >> textdata; //#rmpscale
    f >> rmpscale;
    f >> textdata; //#wheelTexture
    f >> wheelTexture;
    f >> textdata; //#shadowTexture
    f >> shadowTexture;    
    f >> textdata; //#tachometerTexture
    f >> tachometerTexture;
    f >> textdata; //#speedometerTexture
    f >> speedometerTexture;
    f >> textdata; //#graphicenv
    f >> graphicenv;    
    f >> textdata; //#tachometerMinMax
    for (i=0; i<2; i++) {f >> tachometerMinMax[i];}
    f >> textdata; //#speedometerMinMax
    for (i=0; i<2; i++) {f >> speedometerMinMax[i];}
    f >> textdata; //#carRange1
    f >> carRange1;
    f >> textdata; //#carRange2
    f >> carRange2;
    f >> textdata; //#carRange3
    f >> carRange3; 
    f >> textdata; //#carRange4
    f >> carRange4;    
    f >> textdata; //#carRange5
    f >> carRange5;   
    f >> textdata; //#carRange1data
    f >> carRange1dataA;
    f >> textdata; //#carRange1data
    f >> carRange1dataB;
    f >> textdata; //#carRange1data
    f >> carRange1dataC;
    f >> textdata; //#carRange2data
    f >> carRange2dataA;
    f >> textdata; //#carRange2data
    f >> carRange2dataB;
    f >> textdata; //#carRange2data
    f >> carRange2dataC;
    f >> textdata; //#carRange3data
    f >> carRange3dataA;
    f >> textdata; //#carRange3data
    f >> carRange3dataB;
    f >> textdata; //#carRange3data
    f >> carRange3dataC;
    f >> textdata; //#carRange4data
    f >> carRange4dataA;
    f >> textdata; //#carRange4data
    f >> carRange4dataB;
    f >> textdata; //#carRange4data
    f >> carRange4dataC;
    f >> textdata; //#carRange5data
    f >> carRange5dataA;
    f >> textdata; //#carRange5data
    f >> carRange5dataB;
    f >> textdata; //#carRange5data
    f >> carRange5dataC;
    
    activateDeativateGraphicRanges ( 0 );
    
    f >> textdata; //#bonnetPosition
    for (i=0; i<3; i++) {f >> bonnetPosition[i];}
    f >> textdata; //#driverPosition
    for (i=0; i<3; i++) {f >> driverPosition[i];}
    f >> textdata; //#carCategory
    f >> carCategory;
    f >> textdata; //#carBodyDimensions
    for (i=0; i<3; i++) {f >> carBodyDimensions[i];}
    f >> textdata; //#carOverallDimensions
    for (i=0; i<2; i++) {f >> carOverallDimensions[i];}
    
    f >> textdata; //#head1a
    for (i=0; i<4; i++) {f >> head1a[i];}    
    f >> textdata; //#head1b
    for (i=0; i<4; i++) {f >> head1b[i];}
    f >> textdata; //#head2a
    for (i=0; i<4; i++) {f >> head2a[i];}    
    f >> textdata; //#head2b
    for (i=0; i<4; i++) {f >> head2b[i];}
    f >> textdata; //#reara
    for (i=0; i<4; i++) {f >> reara[i];}    
    f >> textdata; //#rearb
    for (i=0; i<4; i++) {f >> rearb[i];}
    f >> textdata; //#brakea
    for (i=0; i<4; i++) {f >> brakea[i];}    
    f >> textdata; //#brakeb
    for (i=0; i<4; i++) {f >> brakeb[i];}
    f >> textdata; //#rearc
    for (i=0; i<4; i++) {f >> rearc[i];}    
    f >> textdata; //#reard
    for (i=0; i<4; i++) {f >> reard[i];}
    f >> textdata; //#brakec
    for (i=0; i<4; i++) {f >> brakec[i];}    
    f >> textdata; //#braked
    for (i=0; i<4; i++) {f >> braked[i];}
    f >> textdata; //#brake2a
    for (i=0; i<4; i++) {f >> brake2a[i];}    
    f >> textdata; //#brake2b
    for (i=0; i<4; i++) {f >> brake2b[i];}

    f >> textdata; //#head1aS
    f >> head1aS;    
    f >> textdata; //#head1bS
    f >> head1bS;
    f >> textdata; //#head2aS
    f >> head2aS;    
    f >> textdata; //#head2bS
    f >> head2bS;
    f >> textdata; //#rearaS
    f >> rearaS;    
    f >> textdata; //#rearbS
    f >> rearbS;
    f >> textdata; //#brakeaS
    f >> brakeaS;    
    f >> textdata; //#brakebS
    f >> brakebS;
    f >> textdata; //#rearcS
    f >> rearcS;    
    f >> textdata; //#reardS
    f >> reardS;
    f >> textdata; //#brakeS
    f >> brakecS;    
    f >> textdata; //#brakedS
    f >> brakedS;   
    f >> textdata; //#brake2aS
    f >> brake2aS; 
    f >> textdata; //#brake2bS
    f >> brake2bS; 

    f >> textdata; //#flamesS
    f >> flameS;
    f >> textdata; //#flamePower
    f >> flamePower;     
    f >> textdata; //#flame1
    for (i=0; i<3; i++) {f >> flame1[i];}
    f >> textdata; //#flame2
    for (i=0; i<3; i++) {f >> flame2[i];}

    f.close();  //cierre del fichero
    
    gearboxRatioSpeed ( 1 );
    
    
   
    cout << "File loaded:" << carname << ".txt\n" << endl;
    
    text1 =  "Loaded: ";
    text1 += fichero3;
    
    GLUI_Master.sync_live_all();
    glutPostRedisplay(); /* we call this function to display the changes
      because the  GLUI_Master.sync_live_all(); doen'nt update the
      glut window if no variable is updated*/
    
    }     
}
   
