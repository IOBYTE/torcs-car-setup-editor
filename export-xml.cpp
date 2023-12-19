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
 /* numvar --->  "<<tqValue[5]<<"*/
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <time.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <vector>

#include "cardata.h"
#include "portability.h"

using namespace std;

extern std::string text1;
extern int   main_window;

extern CarData cardata;
extern GLUI *glui9; /* window for warnings */
extern int warningn;
extern std::string trackname;
extern std::string driverName;
extern std::string driverNameNumber;

extern std::string text; //torcs car setup editor
extern float TCSE_version;

char fichero[100];
time_t tAct = time(NULL); /* the time */

void warningMsg(int warningn);

void backupXmlFile( void );
int getYear (void)
{
            std::string year = "";
            int yearInt = 0;
            tAct = time(NULL);
            year = asctime(localtime(&tAct));
            year = year.substr(year.size()-5,year.size()-1);
            yearInt = atoi (year.c_str());
            //cout << "year "<< yearInt << endl;
            return yearInt;
}


void savexml( int i )
{

    backupXmlFile();

    switch ( i )
    {
       case 1: /* save xml for the car*/
           /* the exported xml file es located in the directory torcs/cars */
           fichero[0]='\0';
           strcat(fichero,"../cars/");
           strcat(fichero,cardata.carname.c_str());
           strcat(fichero,"/");
           strcat(fichero,cardata.carname.c_str());
           strcat(fichero,".xml");
           break;
       case 2: /* save xml for all tracks for the selected driver*/
           if ( driverName == "human" )
           {
           /* the exported xml file es located in the directory torcs/drivers/human */
           fichero[0]='\0';
           strcat(fichero,"../drivers/");
           strcat(fichero,driverName.c_str());
           strcat(fichero,"/");
           strcat(fichero,cardata.carname.c_str());
           strcat(fichero,".xml");
           break;
           }
           if ( driverName != "human" )
           {
           /* the exported xml file es located in the directory torcs/drivers/driverName/number */
           fichero[0]='\0';
           strcat(fichero,"../drivers/");
           strcat(fichero,driverName.c_str());
           strcat(fichero,"/");
           strcat(fichero,driverNameNumber.c_str());
           strcat(fichero,"/");
           strcat(fichero,"default");
           strcat(fichero,".xml");
           break;
           }

       case 3: /* save xml for the selected track for the selected driver*/
           if ( driverName == "human" )
           {
           /* the exported xml file es located in the directory torcs/drivers/human */
           fichero[0]='\0';
           strcat(fichero,"../drivers/");
           strcat(fichero,driverName.c_str());
           strcat(fichero,"/");
           strcat(fichero,trackname.c_str());
           strcat(fichero,"/");
           strcat(fichero,cardata.carname.c_str());
           strcat(fichero,".xml");
           break;
           }
           if ( driverName != "human" )
           {
           /* the exported xml file es located in the directory torcs/drivers/driverName */
           fichero[0]='\0';
           strcat(fichero,"../drivers/");
           strcat(fichero,driverName.c_str());
           strcat(fichero,"/");
           strcat(fichero,driverNameNumber.c_str());
           strcat(fichero,"/");
           strcat(fichero,trackname.c_str());
           strcat(fichero,".xml");
           break;
           }
    }


    ofstream f;  //fichero de salida

    f.open(fichero);  //apertura del fichero o creacción si no existe
    if(!f)
      {
        cout << "The car " << cardata.carname << " doesn't exits." << endl;
        cout << "Please check the carname or create the folder " << fichero << endl;

        switch ( i )
        {
        case 1:
                warningMsg(0);/* xml for the car */
                break;
        case 2:
                warningMsg(4);/* xml for the driver all tracks */
                break;
        case 3:
                warningMsg(5);/* xml for the driver selected track */
                break;
        }
      }
    else
    { //operacions in the file

f << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;

f << "<!--" << endl;
f << "    file                 : " << cardata.carname<<".xml" << endl;
f << "    created              : " << asctime(localtime(&tAct)); /*we don't need the endl */
f << "    copyright            : (C) " << getYear() << " by " << cardata.autorName << endl;
f << "    email                : " << cardata.autorEmail << endl;
f << "    version              : $Id: " << cardata.carname << ".xml $" << endl;
f << "    created with " << text  << " version: " << TCSE_version << " created by Vicente Marti" << endl;

f << "<!--    This program is free software; you can redistribute it and/or modify  -->" << endl;
f << "<!--    it under the terms of the GNU General Public License as published by  -->" << endl;
f << "<!--    the Free Software Foundation; either version 2 of the License, or     -->" << endl;
f << "<!--    (at your option) any later version.                                   -->" << endl;
f << endl;

f << "<!DOCTYPE params SYSTEM \"../../../../src/libs/tgf/params.dtd\">" << endl;
f << "<params name=\"" << cardata.fullCarName << "\" type=\"template\">" << endl;
f << "\t<section name=\"Driver\">" << endl;
f << "\t\t<!-- Position of the driver -->" << endl;
f << "\t\t<attnum name=\"xpos\" unit=\"m\" val=\"" << cardata.driverPosition[0] << "\"/>" << endl;
f << "\t\t<attnum name=\"ypos\" unit=\"m\" val=\"" << cardata.driverPosition[1] << "\"/>" << endl;
f << "\t\t<attnum name=\"zpos\" unit=\"m\" val=\"" << cardata.driverPosition[2] << "\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Bonnet\">" << endl;
f << "\t\t<!-- Position of the driver -->" << endl;
f << "\t\t<attnum name=\"xpos\" unit=\"m\" val=\"" << cardata.bonnetPosition[0] << "\"/>" << endl;
f << "\t\t<attnum name=\"ypos\" unit=\"m\" val=\"" << cardata.bonnetPosition[1] << "\"/>" << endl;
f << "\t\t<attnum name=\"zpos\" unit=\"m\" val=\"" << cardata.bonnetPosition[2] << "\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Sound\">" << endl;
f << "\t\t<attstr name=\"engine sample\" val=\"" << cardata.enginesample <<"\"/>" << endl;
f << "\t\t<attnum name=\"rpm scale\" val=\"" << cardata.rmpscale <<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Graphic Objects\">" << endl;
f << "\t\t<attstr name=\"env\" val=\"" << cardata.graphicenv << "\"/>" << endl;
f << "\t\t<attstr name=\"wheel texture\" val=\"" << cardata.wheelTexture << "\"/>" << endl;
f << "\t\t<attstr name=\"shadow texture\" val=\"" << cardata.shadowTexture << "\"/>" << endl;
f << "\t\t<attstr name=\"tachometer texture\" val=\"" << cardata.tachometerTexture << "\"/>" << endl;
f << "\t\t<attnum name=\"tachometer min value\" unit=\"rpm\" val=\"" << cardata.tachometerMinMax[0] << "\"/>" << endl;
f << "\t\t<attnum name=\"tachometer max value\" unit=\"rpm\" val=\"" << cardata.tachometerMinMax[1] << "\"/>" << endl;
f << "\t\t<attstr name=\"speedometer texture\" val=\"" << cardata.speedometerTexture << "\"/>" << endl;
f << "\t\t<attnum name=\"speedometer min value\" unit=\"km/h\" val=\"" << cardata.speedometerMinMax[0] << "\"/>" << endl;
f << "\t\t<attnum name=\"speedometer max value\" unit=\"km/h\" val=\"" << cardata.speedometerMinMax[1] << "\"/>" << endl;

f << "\t\t<section name=\"Ranges\">" << endl;
f << "\t\t\t<section name=\"1\">" << endl;
f << "\t\t\t\t<attnum name=\"threshold\" val=\"" << cardata.carRange1dataA << "\"/>" << endl;
f << "\t\t\t\t<attstr name=\"car\" val=\"" << cardata.carRange1 << "\"/>" << endl;
std::string yes_no =(cardata.carRange1dataB==1)?"yes":"no";
f << "\t\t\t\t<attstr name=\"wheels\" val=\"" << yes_no << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
if (cardata.carRange2dataC == 1)
{
f << "\t\t\t<section name=\"2\">" << endl;
f << "\t\t\t\t<attnum name=\"threshold\" val=\"" << cardata.carRange2dataA << "\"/>" << endl;
f << "\t\t\t\t<attstr name=\"car\" val=\"" << cardata.carRange2 << "\"/>" << endl;
yes_no =(cardata.carRange2dataB==1)?"yes":"no";
f << "\t\t\t\t<attstr name=\"wheels\" val=\"" << yes_no << "\"/>" << endl;
f << "\t\t/t</section>" << endl;
}
if (cardata.carRange3dataC == 1)
{
f << "\t\t\t<section name=\"3\">" << endl;
f << "\t\t\t\t<attnum name=\"threshold\" val=\"" << cardata.carRange3dataA << "\"/>" << endl;
f << "\t\t\t\t<attstr name=\"car\" val=\"" << cardata.carRange3 << "\"/>" << endl;
yes_no =(cardata.carRange3dataB==1)?"yes":"no";
f << "\t\t\t\t<attstr name=\"wheels\" val=\"" << yes_no << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.carRange4dataC == 1)
{
f << "\t\t\t<section name=\"4\">" << endl;
f << "\t\t\t\t<attnum name=\"threshold\" val=\"" << cardata.carRange4dataA << "\"/>" << endl;
f << "\t\t\t\t<attstr name=\"car\" val=\"" << cardata.carRange4 << "\"/>" << endl;
yes_no =(cardata.carRange4dataB==1)?"yes":"no";
f << "\t\t\t\t<attstr name=\"wheels\" val=\"" << yes_no << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.carRange5dataC == 1)
{
f << "\t\t\t<section name=\"5\">" << endl;
f << "\t\t\t\t<attnum name=\"threshold\" val=\"" << cardata.carRange5dataA << "\"/>" << endl;
f << "\t\t\t\t<attstr name=\"car\" val=\"" << cardata.carRange5 << "\"/>" << endl;
yes_no =(cardata.carRange5dataB==1)?"yes":"no";
f << "\t\t\t\t<attstr name=\"wheels\" val=\"" << yes_no << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
f << "\t\t</section>" << endl;

f << "\t\t<section name=\"Steer Wheel\">" << endl;
f << "\t\t\t<attstr name=\"model\" val=\"" << cardata.steeringWheel.model << "\"/>" << endl;
f << "\t\t\t<attstr name=\"hi res model\" val=\"" << cardata.steeringWheel.hiResModel << "\"/>" << endl;
f << "\t\t\t<attnum name=\"xpos\" val=\"" << cardata.steeringWheel.xpos << "\"/>" << endl;
f << "\t\t\t<attnum name=\"ypos\" val=\"" << cardata.steeringWheel.ypos << "\"/>" << endl;
f << "\t\t\t<attnum name=\"zpos\" val=\"" << cardata.steeringWheel.zpos << "\"/>" << endl;
f << "\t\t\t<attnum name=\"angle\" val=\"" << cardata.steeringWheel.angle << "\"/>" << endl;
f << "\t\t\t<attnum name=\"movt\" val=\"" << cardata.steeringWheel.movt << "\"/>" << endl;
f << "\t\t</section>" << endl;

if (!cardata.drivers.empty())
{
f << "\t\t<section name=\"Driver\">" << std::endl;
int index = 1;
for (const auto & driver : cardata.drivers)
{
f << "\t\t\t<section name=\"" << index++ << "\">" << endl;
f << "\t\t\t\t<attnum name=\"steer\" val=\"" << driver.steer << "\"/>" << endl;
f << "\t\t\t\t<attstr name=\"driver\" val=\"" << driver.driver << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << driver.xpos << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << driver.ypos << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << driver.zpos << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
f << "\t\t</section>" << endl;
}
f << "\t\t<section name=\"Light\">" << endl;
int numberLight = 1;
if (cardata.head1aS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"head1\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.head1a[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.head1a[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.head1a[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.head1a[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.head1bS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"head1\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.head1b[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.head1b[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.head1b[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.head1b[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.head2aS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"head2\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.head2a[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.head2a[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.head2a[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.head2a[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.head2bS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"head2\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.head2b[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.head2b[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.head2b[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.head2b[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.rearaS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"rear\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.reara[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.reara[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.reara[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.reara[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.rearbS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"rear\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.rearb[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.rearb[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.rearb[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.rearb[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.brakeaS == 1)
{
f << "/t/t/t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "/t/t/t/t<attstr name=\"type\" val=\"brake\"/>" << endl;
f << "/t/t/t/t<attnum name=\"xpos\" val=\"" << cardata.brakea[0] << "\"/>" << endl;
f << "/t/t/t/t<attnum name=\"ypos\" val=\"" << cardata.brakea[1] << "\"/>" << endl;
f << "/t/t/t/t<attnum name=\"zpos\" val=\"" << cardata.brakea[2] << "\"/>" << endl;
f << "/t/t/t/t<attnum name=\"size\" val=\"" << cardata.brakea[3] << "\"/>" << endl;
f << "/t/t/t</section>" << endl;
}
if (cardata.brakebS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"brake\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.brakeb[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.brakeb[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.brakeb[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.brakeb[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.rearcS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"rear\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.rearc[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.rearc[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.rearc[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.rearc[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.reardS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"rear\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.reard[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.reard[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.reard[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.reard[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.brakecS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"brake\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.brakec[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.brakec[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.brakec[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.brakec[3] << "\"/>" << endl;
f << "\t</section>" << endl;
}
if (cardata.brakedS == 1)
{
f << "\t\t\t<section name=\""<<numberLight<<"\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"brake\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.braked[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.braked[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.braked[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.braked[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.brake2aS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"brake2\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.brake2a[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.brake2a[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.brake2a[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.brake2a[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if (cardata.brake2bS == 1)
{
f << "\t\t\t<section name=\"" << numberLight << "\">" << endl;
numberLight++;
f << "\t\t\t\t<attstr name=\"type\" val=\"brake2\"/>" << endl;
f << "\t\t\t\t<attnum name=\"xpos\" val=\"" << cardata.brake2b[0] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"ypos\" val=\"" << cardata.brake2b[1] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"zpos\" val=\"" << cardata.brake2b[2] << "\"/>" << endl;
f << "\t\t\t\t<attnum name=\"size\" val=\"" << cardata.brake2b[3] << "\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
f << "\t\t</section>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Car\">" << endl;
f << "\t\t<attstr name=\"category\" val=\"" << cardata.carCategory << "\"/>" << endl;
f << "\t\t<attnum name=\"body length\" unit=\"m\" val=\"" << cardata.carBodyDimensions[0] << "\"/>" << endl;
f << "\t\t<attnum name=\"body width\" unit=\"m\" val=\"" << cardata.carBodyDimensions[1] << "\"/>" << endl;
f << "\t\t<attnum name=\"body height\" unit=\"m\" val=\"" << cardata.carBodyDimensions[2] << "\"/>" << endl;

f << "\t\t<!-- collision bounding box -->" << endl;
f << "\t\t<attnum name=\"overall length\" unit=\"m\" val=\"" << cardata.carOverallDimensions[0] << "\"/>" << endl;
f << "\t\t<attnum name=\"overall width\" unit=\"m\" val=\"" << cardata.carOverallDimensions[1] << "\"/>" << endl;
f << "\t\t<attnum name=\"mass\" unit=\"kg\" val=\"" << cardata.massdata[6] << "\"/>" << endl;
f << "\t\t<attnum name=\"GC height\" unit=\"m\" val=\"" << cardata.massdata[7] << "\"/>" << endl;

f << "\t\t<!-- weight bias -->" << endl;
f << "\t\t<attnum name=\"front-rear weight repartition\" min=\"0.1\" max=\"0.9\" val=\"" << cardata.massdata[0] << "\"/>" << endl;
f << "\t\t<attnum name=\"front right-left weight repartition\" min=\"0.4\" max=\"0.6\" val=\"" << cardata.massdata[1] << "\"/>" << endl;
f << "\t\t<attnum name=\"rear right-left weight repartition\" min=\"0.4\" max=\"0.6\" val=\"" << cardata.massdata[2] << "\"/>" << endl;

f << "\t\t<!-- used for inertia, smaller values indicate better mass centering -->" << endl;
f << "\t\t<attnum name=\"mass repartition coefficient\" max=\"1.0\" min=\"0.4\" val=\"" << cardata.massdata[3] << "\"/>" << endl;
f << "\t\t<attnum name=\"fuel tank\" unit=\"l\" val=\""<< cardata.massdata[4] << "\"/>" << endl;
f << "\t\t<attnum name=\"initial fuel\" unit=\"l\" min=\"1.0\" max=\"100.0\" val=\"" << cardata.massdata[5] << "\"/>" << endl;
f << "\t</section>" << endl;

if (cardata.flameS ==1)
{
f << "\t<section name=\"Exhaust\">" << endl;
f << "\t\t<!-- for flames -->" << endl;
f << "\t\t<attnum name=\"power\" val=\""<<cardata.flamePower<<"\"/>" << endl;
f << "\t\t<section name=\"1\">" << endl;
f << "\t\t\t<attnum name=\"xpos\" val=\""<<cardata.flame1[0]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"ypos\" val=\""<<cardata.flame1[1]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"zpos\" val=\""<<cardata.flame1[2]<<"\"/>" << endl;
f << "\t\t</section>" << endl;

f << "\t\t<section name=\"2\">" << endl;
f << "\t\t\t<attnum name=\"xpos\" val=\""<<cardata.flame2[0]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"ypos\" val=\""<<cardata.flame2[1]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"zpos\" val=\""<<cardata.flame2[2]<<"\"/>" << endl;
f << "\t\t</section>" << endl;
f << "\t</section>" << endl;
}

f << "\t<section name=\"Aerodynamics\">" << endl;
f << "\t\t<attnum name=\"Cx\" val=\""<<cardata.aerodynamics[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"front area\" unit=\"m2\" val=\""<<cardata.aerodynamics[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"front Clift\" val=\""<<cardata.aerodynamics[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"rear Clift\" val=\""<<cardata.aerodynamics[3]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Front Wing\">" << endl;
f << "\t\t<attnum name=\"area\" unit=\"m2\" val=\""<<cardata.frontwing[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"angle\" unit=\"deg\" val=\""<<cardata.frontwing[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"xpos\" unit=\"m\" val=\""<<cardata.frontwing[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"zpos\" unit=\"m\" val=\""<<cardata.frontwing[3]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Rear Wing\">" << endl;
f << "\t\t<attnum name=\"area\" unit=\"m2\" val=\""<<cardata.rearwing[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"angle\" unit=\"deg\" val=\""<<cardata.rearwing[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"xpos\" unit=\"m\" val=\""<<cardata.rearwing[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"zpos\" unit=\"m\" val=\""<<cardata.rearwing[3]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<!-- Same engine for every one -->" << endl;
f << "\t<section name=\"Engine\">" << endl;
f << "\t\t<attnum name=\"capacity\" unit=\"l\" val=\"" << cardata.engine.capacity << "\"/>" << endl;
f << "\t\t<attnum name=\"cylinders\" val=\"" << cardata.engine.cylinders << "\"/>" << endl;
f << "\t\t<attstr name=\"shape\" val=\"" << cardata.engine.shape[cardata.engine.curr_shape] << "\"/>" << endl;
f << "\t\t<attstr name=\"position\" val=\"" << cardata.engine.position[cardata.engine.curr_position] << "\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" min=\"0.1\" max=\"0.5\" unit=\"kg.m2\" val=\""<<cardata.engine.inertia<<"\"/>" << endl;
f << "\t\t<attnum name=\"revs maxi\" unit=\"rpm\" min=\"5000\" max=\"20000\" val=\""<<cardata.engine.revsMaxi<<"\"/>" << endl;
f << "\t\t<attnum name=\"revs limiter\" unit=\"rpm\" min=\"3000\" max=\"20000\" val=\""<<cardata.engine.revsLimiter<<"\"/>" << endl;
f << "\t\t<attnum name=\"tickover\" unit=\"rpm\" val=\""<<cardata.engine.tickover<<"\"/>" << endl;
f << "\t\t<attnum name=\"fuel cons factor\" min=\"1.1\" max=\"1.3\" val=\""<<cardata.engine.fuelConsFactor<<"\"/>" << endl;
f << "\t\t<attnum name=\"brake linear coefficient\" val=\"" << cardata.engine.brakeLinearCoefficient << "\"/>" << endl;
f << "\t\t<attnum name=\"brake coefficient\" val=\"" << cardata.engine.brakeCoefficient << "\"/>" << endl;

f << "\t\t<section name=\"data points\">" << endl;
if (cardata.engine.turboS == 1)
{
f << "\t\t\t<attstr name=\"turbo\"  val=\"true\"/>" << endl;
f << "\t\t\t<attnum name=\"turbo rpm\" unit=\"rpm\" val=\""<<cardata.engine.turbo[0]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"turbo factor\"  val=\""<<cardata.engine.turbo[1]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"turbo lag\"  val=\""<<cardata.engine.turbo[2]<<"\"/>" << endl;
}
f << "\t\t\t<section name=\"1\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[0]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"2000.0\" val=\""<<cardata.engine.tqValue[0]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"2\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[1]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1473.0\" val=\""<<cardata.engine.tqValue[1]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"3\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[2]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1355.0\" val=\""<<cardata.engine.tqValue[2]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"4\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[3]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1275.0\" val=\""<<cardata.engine.tqValue[3]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"5\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[4]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1145.0\" val=\""<<cardata.engine.tqValue[4]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"6\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[5]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1000.0\" val=\""<<cardata.engine.tqValue[5]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"7\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[6]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"884.0\" val=\""<<cardata.engine.tqValue[6]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"8\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[7]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"799.0\" val=\""<<cardata.engine.tqValue[7]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"9\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[8]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"735.0\" val=\""<<cardata.engine.tqValue[8]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"10\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[9]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"686.0\" val=\""<<cardata.engine.tqValue[9]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"11\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[10]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"647.0\" val=\""<<cardata.engine.tqValue[10]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"12\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[11]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"971.0\" val=\""<<cardata.engine.tqValue[11]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"13\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[12]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"890.0\" val=\""<<cardata.engine.tqValue[12]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"14\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[13]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"821.0\" val=\""<<cardata.engine.tqValue[13]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"15\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[14]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"762.0\" val=\""<<cardata.engine.tqValue[14]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"16\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[15]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"712.0\" val=\""<<cardata.engine.tqValue[15]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"17\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[16]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"667.0\" val=\""<<cardata.engine.tqValue[16]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"18\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[17]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"628.0\" val=\""<<cardata.engine.tqValue[17]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"19\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[18]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"628.0\" val=\""<<cardata.engine.tqValue[18]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"20\">" << endl;
f << "\t\t\t  <attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[19]<<"\"/>" << endl;
f << "\t\t\t  <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"628.0\" val=\""<<cardata.engine.tqValue[19]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;

f << "\t\t\t<section name=\"21\">" << endl;
f << "\t\t\t\t<attnum name=\"rpm\" unit=\"rpm\" val=\""<<cardata.engine.rpmValue[20]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"628.0\" val=\""<<cardata.engine.tqValue[20]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;
f << "\t\t</section>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Clutch\">" << endl;
f << "\t\t<!-- pressure plate -->" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.gearboxinertia[0]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Gearbox\">" << endl;
f << "\t\t<attnum name=\"shift time\" val=\""<<cardata.gearbox_shift_time<<"\" unit=\"s\"/>" << endl;
f << "\t\t<section name=\"gears\">" << endl;
f << "\t\t\t<section name=\"r\">" << endl;
f << "\t\t\t\t<attnum name=\"ratio\" min=\"-3\" max=\"0\" val=\""<<cardata.gearboxratio[0]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"inertia\" val=\""<<cardata.gearboxinertia[1]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"efficiency\" val=\""<<cardata.gearboxefficiency[0]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;
if ( cardata.numberOfGears >= 1)
{
f << "\t\t\t<section name=\"1\">" << endl;
f << "\t\t\t\t<attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<cardata.gearboxratio[1]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"inertia\" val=\""<<cardata.gearboxinertia[2]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"efficiency\" val=\""<<cardata.gearboxefficiency[1]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if ( cardata.numberOfGears >= 2)
{
f << "\t\t\t<section name=\"2\">" << endl;
f << "\t\t\t\t<attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<cardata.gearboxratio[2]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"inertia\" val=\""<<cardata.gearboxinertia[3]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"efficiency\" val=\""<<cardata.gearboxefficiency[2]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if ( cardata.numberOfGears >= 3)
{
f << "\t\t\t<section name=\"3\">" << endl;
f << "\t\t\t\t<attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<cardata.gearboxratio[3]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"inertia\" val=\""<<cardata.gearboxinertia[4]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"efficiency\" val=\""<<cardata.gearboxefficiency[3]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if ( cardata.numberOfGears >= 4)
{
f << "\t\t\t<section name=\"4\">" << endl;
f << "\t\t\t\t<attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<cardata.gearboxratio[4]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"inertia\" val=\""<<cardata.gearboxinertia[5]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"efficiency\" val=\""<<cardata.gearboxefficiency[4]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if ( cardata.numberOfGears >= 5)
{
f << "\t\t\t<section name=\"5\">" << endl;
f << "\t\t\t\t<attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<cardata.gearboxratio[5]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"inertia\" val=\""<<cardata.gearboxinertia[6]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"efficiency\" val=\""<<cardata.gearboxefficiency[5]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if ( cardata.numberOfGears >= 6)
{
f << "\t\t\t<section name=\"6\">" << endl;
f << "\t\t\t\t<attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<cardata.gearboxratio[6]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"inertia\" val=\""<<cardata.gearboxinertia[7]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"efficiency\" val=\""<<cardata.gearboxefficiency[6]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
if ( cardata.numberOfGears >= 7)
{
f << "\t\t\t<section name=\"7\">" << endl;
f << "\t\t\t\t<attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<cardata.gearboxratio[7]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"inertia\" val=\""<<cardata.gearboxinertia[8]<<"\"/>" << endl;
f << "\t\t\t\t<attnum name=\"efficiency\" val=\""<<cardata.gearboxefficiency[7]<<"\"/>" << endl;
f << "\t\t\t</section>" << endl;
}
f << "\t\t</section>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Steer\">" << endl;
f << "\t\t<attnum name=\"steer lock\" unit=\"deg\" min=\"1\" max=\"45\" val=\""<<cardata.steer[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"max steer speed\" unit=\"deg/s\" min=\"1\" max=\"360\" val=\""<<cardata.steer[1]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Drivetrain\">" << endl;
f << "\t\t<attstr name=\"type\" val=\""<<cardata.drivetrain_type[cardata.curr_drivetrain_type]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.drivetrain<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Front Differential\">" << endl;
f << "\t\t<attstr name=\"type\" in=\"SPOOL,FREE,LIMITED SLIP\" val=\""<<cardata.differential_type[cardata.curr_frontdifferential_type]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.frontdifferential[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"ratio\" min=\"0\" max=\"10\" val=\""<<cardata.frontdifferential[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"efficiency\" val=\""<<cardata.frontdifferential[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"min torque bias\" val=\""<<cardata.frontdifferential[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"max torque bias\" val=\""<<cardata.frontdifferential[4]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Rear Differential\">" << endl;
f << "\t\t<attstr name=\"type\" in=\"SPOOL,FREE,LIMITED SLIP\" val=\""<<cardata.differential_type[cardata.curr_reardifferential_type]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.reardifferential[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"ratio\" min=\"0\" max=\"10\" val=\""<<cardata.reardifferential[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"efficiency\" val=\""<<cardata.reardifferential[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"min torque bias\" val=\""<<cardata.reardifferential[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"max torque bias\" val=\""<<cardata.reardifferential[4]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Central Differential\">" << endl;
f << "\t\t<attstr name=\"type\" in=\"SPOOL,FREE,LIMITED SLIP\" val=\""<<cardata.differential_type[cardata.curr_centraldifferential_type]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.centraldifferential[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"ratio\" min=\"0\" max=\"10\" val=\""<<cardata.centraldifferential[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"efficiency\" val=\""<<cardata.centraldifferential[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"min torque bias\" val=\""<<cardata.centraldifferential[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"max torque bias\" val=\""<<cardata.centraldifferential[4]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Brake System\">" << endl;
f << "\t\t<attnum name=\"front-rear brake repartition\" min=\"0.3\" max=\"0.7\" val=\""<<cardata.brakesystem[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"max pressure\" unit=\"kPa\" min=\"100\" max=\"150000\" val=\""<<cardata.brakesystem[1]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Front Axle\">" << endl;
f << "\t\t<attnum name=\"xpos\" unit=\"m\" val=\""<<cardata.frontaxle[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.frontaxle[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"roll center height\" unit=\"m\" val=\""<<cardata.frontaxle[2]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Rear Axle\">" << endl;
f << "\t\t<attnum name=\"xpos\" unit=\"m\" val=\""<<cardata.rearaxle[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.rearaxle[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"roll center height\" unit=\"m\" val=\""<<cardata.rearaxle[2]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Front Right Wheel\">" << endl;
f << "\t\t<attnum name=\"ypos\" unit=\"m\" val=\""<<cardata.wheel1[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"rim diameter\" unit=\"in\" val=\""<<cardata.wheel1[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"tire width\" unit=\"mm\" val=\""<<cardata.wheel1[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"tire height-width ratio\" val=\""<<cardata.wheel1[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.wheel1[4]<<"\"/>" << endl;
f << "\t\t<!-- initial ride height -->" << endl;
f << "\t\t<attnum name=\"ride height\" unit=\"mm\" min=\"100\" max=\"300\" val=\""<<cardata.wheel1[5]<<"\"/>" << endl;
f << "\t\t<attnum name=\"toe\" unit=\"deg\" min=\"-5\" max=\"5\" val=\""<<cardata.wheel1[6]<<"\"/>" << endl;
f << "\t\t<attnum name=\"camber\" min=\"-5\" max=\"0\" unit=\"deg\" val=\""<<cardata.wheel1[7]<<"\"/>" << endl;
f << "\t\t<!-- Adherence -->" << endl;
f << "\t\t<attnum name=\"stiffness\" min=\"20.0\" max=\"50.0\" val=\""<<cardata.wheel1[8]<<"\"/>" << endl;
f << "\t\t<attnum name=\"dynamic friction\" unit=\"%\" min=\"80\" max=\"70\" val=\""<<cardata.wheel1[9]<<"\"/>" << endl;
f << "\t\t<attnum name=\"rolling resistance\" val=\""<<cardata.wheel1[10]<<"\"/>" << endl;
f << "\t\t<attnum name=\"mu\" min=\"0.05\" max=\"1.8\" val=\""<<cardata.wheel1[11]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t\t<section name=\"Front Left Wheel\">" << endl;
f << "\t\t\t<attnum name=\"ypos\" unit=\"m\" val=\""<<cardata.wheel2[0]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"rim diameter\" unit=\"in\" val=\""<<cardata.wheel2[1]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"tire width\" unit=\"mm\" val=\""<<cardata.wheel2[2]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"tire height-width ratio\" val=\""<<cardata.wheel2[3]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.wheel2[4]<<"\"/>" << endl;
f << "\t\t\t<!-- initial ride height -->" << endl;
f << "\t\t\t<attnum name=\"ride height\" unit=\"mm\" min=\"100\" max=\"300\" val=\""<<cardata.wheel2[5]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"toe\" unit=\"deg\" min=\"-5\" max=\"5\" val=\""<<cardata.wheel2[6]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"camber\" min=\"-5\" max=\"0\" unit=\"deg\" val=\""<<cardata.wheel2[7]<<"\"/>" << endl;
f << "\t\t\t<!-- Adherence -->" << endl;
f << "\t\t\t<attnum name=\"stiffness\" min=\"20.0\" max=\"50.0\" val=\""<<cardata.wheel2[8]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"dynamic friction\" unit=\"%\" min=\"80\" max=\"70\" val=\""<<cardata.wheel2[9]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"rolling resistance\" val=\""<<cardata.wheel2[10]<<"\"/>" << endl;
f << "\t\t\t<attnum name=\"mu\" min=\"0.05\" max=\"1.8\" val=\""<<cardata.wheel2[11]<<"\"/>" << endl;
f << "\t\t</section>" << endl;

f << "\t<section name=\"Rear Right Wheel\">" << endl;
f << "\t\t<attnum name=\"ypos\" unit=\"m\" val=\""<<cardata.wheel3[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"rim diameter\" unit=\"in\" val=\""<<cardata.wheel3[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"tire width\" unit=\"mm\" val=\""<<cardata.wheel3[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"tire height-width ratio\" val=\""<<cardata.wheel3[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.wheel3[4]<<"\"/>" << endl;
f << "\t\t<!-- initial ride height -->" << endl;
f << "\t\t<attnum name=\"ride height\" unit=\"mm\" min=\"100\" max=\"300\" val=\""<<cardata.wheel3[5]<<"\"/>" << endl;
f << "\t\t<attnum name=\"toe\" unit=\"deg\" min=\"-5\" max=\"5\" val=\""<<cardata.wheel3[6]<<"\"/>" << endl;
f << "\t\t<attnum name=\"camber\" min=\"-5\" max=\"0\" unit=\"deg\" val=\""<<cardata.wheel3[7]<<"\"/>" << endl;
f << "\t\t<!-- Adherence -->" << endl;
f << "\t\t<attnum name=\"stiffness\" min=\"20.0\" max=\"50.0\" val=\""<<cardata.wheel3[8]<<"\"/>" << endl;
f << "\t\t<attnum name=\"dynamic friction\" unit=\"%\" min=\"80\" max=\"70\" val=\""<<cardata.wheel3[9]<<"\"/>" << endl;
f << "\t\t<attnum name=\"rolling resistance\" val=\""<<cardata.wheel3[10]<<"\"/>" << endl;
f << "\t\t<attnum name=\"mu\" min=\"0.05\" max=\"1.8\" val=\""<<cardata.wheel3[11]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Rear Left Wheel\">" << endl;
f << "\t\t<attnum name=\"ypos\" unit=\"m\" val=\""<<cardata.wheel4[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"rim diameter\" unit=\"in\" val=\""<<cardata.wheel4[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"tire width\" unit=\"mm\" val=\""<<cardata.wheel4[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"tire height-width ratio\" val=\""<<cardata.wheel4[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.wheel4[4]<<"\"/>" << endl;
f << "\t\t<!-- initial ride height -->" << endl;
f << "\t\t<attnum name=\"ride height\" unit=\"mm\" min=\"100\" max=\"300\" val=\""<<cardata.wheel4[5]<<"\"/>" << endl;
f << "\t\t<attnum name=\"toe\" unit=\"deg\" min=\"-5\" max=\"5\" val=\""<<cardata.wheel4[6]<<"\"/>" << endl;
f << "\t\t<attnum name=\"camber\" min=\"-5\" max=\"0\" unit=\"deg\" val=\""<<cardata.wheel4[7]<<"\"/>" << endl;
f << "\t\t<!-- Adherence -->" << endl;
f << "\t\t<attnum name=\"stiffness\" min=\"20.0\" max=\"50.0\" val=\""<<cardata.wheel4[8]<<"\"/>" << endl;
f << "\t\t<attnum name=\"dynamic friction\" unit=\"%\" min=\"80\" max=\"70\" val=\""<<cardata.wheel4[9]<<"\"/>" << endl;
f << "\t\t<attnum name=\"rolling resistance\" val=\""<<cardata.wheel4[10]<<"\"/>" << endl;
f << "\t\t<attnum name=\"mu\" min=\"0.05\" max=\"1.8\" val=\""<<cardata.wheel4[11]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Front Anti-Roll Bar\">" << endl;
f << "\t\t<attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"5000\" val=\""<<cardata.antirollbar1[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<cardata.antirollbar1[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"bellcrank\" min=\"1\" max=\"5\" val=\""<<cardata.antirollbar1[2]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Rear Anti-Roll Bar\">" << endl;
f << "\t\t<attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"5000\" val=\""<<cardata.antirollbar2[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<cardata.antirollbar2[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"bellcrank\" min=\"1\" max=\"5\" val=\""<<cardata.antirollbar2[2]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Front Right Suspension\">" << endl;
f << "\t\t<attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"10000\" val=\""<<cardata.suspension1[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<cardata.suspension1[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"bellcrank\" min=\"0.1\" max=\"5\" val=\""<<cardata.suspension1[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"packers\" unit=\"mm\" min=\"0\" max=\"10\" val=\""<<cardata.suspension1[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"slow bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension1[4]<<"\"/>" << endl;
f << "\t\t<attnum name=\"slow rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension1[5]<<"\"/>" << endl;
f << "\t\t<attnum name=\"fast bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension1[6]<<"\"/>" << endl;
f << "\t\t<attnum name=\"fast rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension1[7]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Front Left Suspension\">" << endl;
f << "\t\t<attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"10000\" val=\""<<cardata.suspension2[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<cardata.suspension2[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"bellcrank\" min=\"0.1\" max=\"5\" val=\""<<cardata.suspension2[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"packers\" unit=\"mm\" min=\"0\" max=\"10\" val=\""<<cardata.suspension2[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"slow bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension2[4]<<"\"/>" << endl;
f << "\t\t<attnum name=\"slow rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension2[5]<<"\"/>" << endl;
f << "\t\t<attnum name=\"fast bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension2[6]<<"\"/>" << endl;
f << "\t\t<attnum name=\"fast rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension2[7]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Rear Right Suspension\">" << endl;
f << "\t\t<attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"10000\" val=\""<<cardata.suspension3[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<cardata.suspension3[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"bellcrank\" min=\"0.1\" max=\"5\" val=\""<<cardata.suspension3[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"packers\" unit=\"mm\" min=\"0\" max=\"10\" val=\""<<cardata.suspension3[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"slow bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension3[4]<<"\"/>" << endl;
f << "\t\t<attnum name=\"slow rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension3[5]<<"\"/>" << endl;
f << "\t\t<attnum name=\"fast bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension3[6]<<"\"/>" << endl;
f << "\t\t<attnum name=\"fast rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension3[7]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Rear Left Suspension\">" << endl;
f << "\t\t<attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"10000\" val=\""<<cardata.suspension4[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<cardata.suspension4[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"bellcrank\" min=\"0.1\" max=\"5\" val=\""<<cardata.suspension4[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"packers\" unit=\"mm\" min=\"0\" max=\"10\" val=\""<<cardata.suspension4[3]<<"\"/>" << endl;
f << "\t\t<attnum name=\"slow bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension4[4]<<"\"/>" << endl;
f << "\t\t<attnum name=\"slow rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension4[5]<<"\"/>" << endl;
f << "\t\t<attnum name=\"fast bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension4[6]<<"\"/>" << endl;
f << "\t\t<attnum name=\"fast rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<cardata.suspension4[7]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Front Right Brake\">" << endl;
f << "\t\t<attnum name=\"disk diameter\" unit=\"mm\" min=\"100\" max=\"380\" val=\""<<cardata.brake1[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"piston area\" unit=\"cm2\" val=\""<<cardata.brake1[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"mu\" val=\""<<cardata.brake1[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.brake1[3]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Front Left Brake\">" << endl;
f << "\t\t<attnum name=\"disk diameter\" unit=\"mm\" min=\"100\" max=\"380\" val=\""<<cardata.brake2[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"piston area\" unit=\"cm2\" val=\""<<cardata.brake2[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"mu\" val=\""<<cardata.brake2[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.brake2[3]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Rear Right Brake\">" << endl;
f << "\t\t<attnum name=\"disk diameter\" unit=\"mm\" min=\"100\" max=\"380\" val=\""<<cardata.brake3[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"piston area\" unit=\"cm2\" val=\""<<cardata.brake3[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"mu\" val=\""<<cardata.brake3[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.brake3[3]<<"\"/>" << endl;
f << "\t</section>" << endl;

f << "\t<section name=\"Rear Left Brake\">" << endl;
f << "\t\t<attnum name=\"disk diameter\" unit=\"mm\" min=\"100\" max=\"380\" val=\""<<cardata.brake4[0]<<"\"/>" << endl;
f << "\t\t<attnum name=\"piston area\" unit=\"cm2\" val=\""<<cardata.brake4[1]<<"\"/>" << endl;
f << "\t\t<attnum name=\"mu\" val=\""<<cardata.brake4[2]<<"\"/>" << endl;
f << "\t\t<attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<cardata.brake4[3]<<"\"/>" << endl;
f << "\t</section>" << endl;
f << "</params>" << endl;



    f.close();  //cierre del fichero
    cout << "File expoted:" << cardata.carname << ".xml" << endl;
    cout << "on: " << fichero << endl;

    text1 = "Exported:";
    text1 += fichero;

    glutSetWindow(main_window);
    glutPostRedisplay();
    }
  //Impressió de resultats en el fitxer d'eixida.
}



void backupXmlFile( void )
{

vector<string> xmlLine;
char fichero4[100];
xmlLine.reserve(100);
string bufer;
char textdata2[150];

           fichero4[0]='\0';
           strcat(fichero4,"../cars/");
           strcat(fichero4,cardata.carname.c_str());
           strcat(fichero4,"/");
           strcat(fichero4,cardata.carname.c_str());
           strcat(fichero4,".xml");


    ifstream f;  //fichero de salid
    int i;
    f.open(fichero4);  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "Error opening the file " << fichero4 << endl;
    else
    {
    /* read the data of the xml file */
    while (!f.eof()!=0)
        {
            getline(f, bufer);
            f.ignore(); // igonere the '\n'
            xmlLine.push_back(bufer);
            //cout << bufer << endl;
        }
    cout << "Read " << xmlLine.size() << " lines from " << fichero4  << endl;
    f.close();
    }

/** SAVE THE BACKUP **/

           fichero4[0]='\0';
           strcat(fichero4,"xml-backups/");
           strcat(fichero4,cardata.carname.c_str());
           makedir(fichero4);
           strcat(fichero4,"/");

           std::string timetorcs = "";
           tAct = time(NULL);
           timetorcs = asctime(localtime(&tAct));
           timetorcs = timetorcs.substr(0,timetorcs.size()-1);

           int k;
           for (k=0; k <timetorcs.size(); k++)
           {
               if (timetorcs.at(k)==' ' || timetorcs.at(k)==':')
               timetorcs.at(k)='-';
           }

           strcat(fichero4,cardata.carname.c_str());
           strcat(fichero4,"-");
           strcat(fichero4,timetorcs.c_str());
           strcat(fichero4,".xml");


    ofstream f2;  //fichero de salid
    f2.open(fichero4);  //apertura del fichero o creacción si no existe
    if(!f2)
        cout << "Error opening the file " << fichero4 << endl;
    else
    {
    /* WRITHE the data in the xml file */
    int k = 0;
    while ( k < xmlLine.size() )
        {
            f2 <<  xmlLine[k] << endl;
            k++;
        }


    cout << "Created backup: " << fichero4  << endl;


    f2.close();
    }


}
