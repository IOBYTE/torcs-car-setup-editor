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
           strcat(fichero,carname.c_str());
           strcat(fichero,"/");
           strcat(fichero,carname.c_str());
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
           strcat(fichero,carname.c_str());
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
           strcat(fichero,carname.c_str());
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
        cout << "The car " << carname << " doesn't exits." << endl;
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
f <<  endl;
f << "<!--" << endl;
f << "	    file                 : "<<carname<<".xml"<< endl;
f << "	    created              : " << asctime(localtime(&tAct)); /*we don't need the endl */
f << "	    copyright            : (C) "<< getYear() << " by " << autorName << endl;
f << "	    email                : " << autorEmail << endl;
f << "	    version              : $Id: "<<carname<<".xml $" << endl;
f << "	    created with " << text  << " version: "  << TCSE_version << " created by Vicente Marti"        << endl;
f << "	-->	 " << endl;
f << "		 " << endl;
f << "	<!--    This program is free software; you can redistribute it and/or modify  -->" << endl;
f << "	<!--    it under the terms of the GNU General Public License as published by  -->" << endl;
f << "	<!--    the Free Software Foundation; either version 2 of the License, or     -->" << endl;
f << "	<!--    (at your option) any later version.                                   -->" << endl;
f << "		 " << endl;
f << "	<!DOCTYPE params SYSTEM \"../../../../src/libs/tgf/params.dtd\">" << endl;
f << "		 " << endl;
f << "	<params name=\""<< fullCarName <<"\" type=\"template\">" << endl;
f << "	  <section name=\"Driver\">	 " << endl;
f << "	    <!-- Position of the driver -->	 " << endl;
f << "	    <attnum name=\"xpos\" val=\""<< driverPosition[0] <<"\" unit=\"m\"/>" << endl;
f << "	    <attnum name=\"ypos\" val=\""<< driverPosition[1] <<"\" unit=\"m\"/>" << endl;
f << "	    <attnum name=\"zpos\" val=\""<< driverPosition[2] <<"\" unit=\"m\"/>" << endl;
f << "	  </section>" << endl;
f << "		 " << endl;
f << "	  <section name=\"Bonnet\">	 " << endl;
f << "	    <!-- Position of the driver -->	 " << endl;
f << "	    <attnum name=\"xpos\" val=\""<< bonnetPosition[0] <<"\" unit=\"m\"/>" << endl;
f << "	    <attnum name=\"ypos\" val=\""<< bonnetPosition[1] <<"\" unit=\"m\"/>" << endl;
f << "	    <attnum name=\"zpos\" val=\""<< bonnetPosition[2] <<"\" unit=\"m\"/>" << endl;
f << "	  </section>" << endl;
f << "		 " << endl;
f << "	  <section name=\"Sound\">	 " << endl;
f << "	 <attstr name=\"engine sample\" val=\""<< enginesample <<"\"/>" << endl;
f << "	 <attnum name=\"rpm scale\" val=\""<< rmpscale <<"\"/>" << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Graphic Objects\">" << endl;
f << "	    <attstr name=\"env\" val=\""<< graphicenv <<"\"/>" << endl;
f << "	    <attstr name=\"wheel texture\" val=\""<<wheelTexture<<"\"/>	 " << endl;
f << "	    <attstr name=\"shadow texture\" val=\""<<shadowTexture<<"\"/>	 " << endl;
f << "	    <attstr name=\"tachometer texture\" val=\""<<tachometerTexture<<"\"/>	 " << endl;
f << "	    <attnum name=\"tachometer min value\" val=\""<<tachometerMinMax[0]<<"\" unit=\"rpm\"/>	 " << endl;
f << "	    <attnum name=\"tachometer max value\" val=\""<<tachometerMinMax[1]<<"\" unit=\"rpm\"/>	 " << endl;
f << "	    <attstr name=\"speedometer texture\" val=\""<<speedometerTexture<<"\"/>	 " << endl;
f << "	    <attnum name=\"speedometer min value\" val=\""<<speedometerMinMax[0]<<"\" unit=\"km/h\"/>	 " << endl;
f << "	    <attnum name=\"speedometer max value\" val=\""<<speedometerMinMax[1]<<"\" unit=\"km/h\"/>	 " << endl;
f << "		 " << endl;
f << "		 " << endl;
f << "	    <section name=\"Ranges\">	 " << endl;
f << "	   <section name=\"1\">	 " << endl;
f << "	    <attnum name=\"threshold\" val=\""<<carRange1dataA<<"\"/>	 " << endl;
f << "	    <attstr name=\"car\" val=\""<<carRange1<<"\"/>	 " << endl;
std::string yes_no;
yes_no =(carRange1dataB==1)?"yes":"no";
f << "	    <attstr name=\"wheels\" val=\""<< yes_no <<"\"/>	 " << endl;
f << "	   </section>	 " << endl;
if (carRange2dataC == 1)
{
f << "	   <section name=\"2\">	 " << endl;
f << "	    <attnum name=\"threshold\" val=\""<<carRange2dataA<<"\"/>	 " << endl;
f << "	    <attstr name=\"car\" val=\""<<carRange2<<"\"/>	 " << endl;
yes_no =(carRange2dataB==1)?"yes":"no";
f << "	    <attstr name=\"wheels\" val=\""<< yes_no <<"\"/>	 " << endl;
f << "	   </section>	 " << endl;
}
if (carRange3dataC == 1)
{
f << "	   <section name=\"3\">	 " << endl;
f << "	    <attnum name=\"threshold\" val=\""<<carRange3dataA<<"\"/>	 " << endl;
f << "	    <attstr name=\"car\" val=\""<<carRange3<<"\"/>	 " << endl;
yes_no =(carRange3dataB==1)?"yes":"no";
f << "	    <attstr name=\"wheels\" val=\""<< yes_no <<"\"/>	 " << endl;
f << "	   </section>	 " << endl;
}
if (carRange4dataC == 1)
{
f << "	   <section name=\"4\">	 " << endl;
f << "	    <attnum name=\"threshold\" val=\""<<carRange4dataA<<"\"/>	 " << endl;
f << "	    <attstr name=\"car\" val=\""<<carRange4<<"\"/>	 " << endl;
yes_no =(carRange4dataB==1)?"yes":"no";
f << "	    <attstr name=\"wheels\" val=\""<< yes_no <<"\"/>	 " << endl;
f << "	   </section>	 " << endl;
}
if (carRange5dataC == 1)
{
f << "	   <section name=\"5\">	 " << endl;
f << "	    <attnum name=\"threshold\" val=\""<<carRange5dataA<<"\"/>	 " << endl;
f << "	    <attstr name=\"car\" val=\""<<carRange5<<"\"/>	 " << endl;
yes_no =(carRange5dataB==1)?"yes":"no";
f << "	    <attstr name=\"wheels\" val=\""<< yes_no <<"\"/>	 " << endl;
f << "	   </section>	 " << endl;
}
f << "	    </section>	 " << endl;
f << "	  	 " << endl;
f << "		 " << endl;
f << "	    <section name=\"Light\">	 " << endl;
int numberLight = 1;
if (head1aS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"head1\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<head1a[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<head1a[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<head1a[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<head1a[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (head1bS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"head1\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<head1b[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<head1b[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<head1b[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<head1b[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (head2aS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"head2\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<head2a[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<head2a[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<head2a[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<head2a[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (head2bS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"head2\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<head2b[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<head2b[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<head2b[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<head2b[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (rearaS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"rear\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<reara[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<reara[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<reara[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<reara[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (rearbS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"rear\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<rearb[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<rearb[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<rearb[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<rearb[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (brakeaS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"brake\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<brakea[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<brakea[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<brakea[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<brakea[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (brakebS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"brake\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<brakeb[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<brakeb[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<brakeb[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<brakeb[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (rearcS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"rear\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<rearc[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<rearc[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<rearc[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<rearc[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (reardS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"rear\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<reard[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<reard[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<reard[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<reard[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (brakecS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"brake\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<brakec[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<brakec[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<brakec[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<brakec[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (brakedS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"brake\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<braked[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<braked[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<braked[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<braked[3]<<"\"/>	 " << endl;
f << "	      </section>      	 " << endl;
}
if (brake2aS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"brake2\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<brake2a[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<brake2a[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<brake2a[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<brake2a[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
}
if (brake2bS == 1)
{
f << "	      <section name=\""<<numberLight<<"\">	 " << endl;
numberLight++;
f << "	 <attstr name=\"type\" val=\"brake2\"/>	 " << endl;
f << "	 <attnum name=\"xpos\" val=\""<<brake2b[0]<<"\"/>	 " << endl;
f << "	 <attnum name=\"ypos\" val=\""<<brake2b[1]<<"\"/>	 " << endl;
f << "	 <attnum name=\"zpos\" val=\""<<brake2b[2]<<"\"/>	 " << endl;
f << "	 <attnum name=\"size\" val=\""<<brake2b[3]<<"\"/>	 " << endl;
f << "	      </section>      	 " << endl;
}
f << "	    </section>	 " << endl;
f << "		 " << endl;
f << "	    " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Car\">	 " << endl;
f << "	    <attstr name=\"category\" val=\""<< carCategory <<"\"/>	 " << endl;
f << "	    <attnum name=\"body length\" unit=\"m\" val=\""<< carBodyDimensions[0] <<"\"/>	 " << endl;
f << "	    <attnum name=\"body width\" unit=\"m\" val=\""<< carBodyDimensions[1] <<"\"/>	 " << endl;
f << "	    <attnum name=\"body height\" unit=\"m\" val=\""<< carBodyDimensions[2] <<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- collision bounding box -->	 " << endl;
f << "	    <attnum name=\"overall length\" unit=\"m\" val=\""<< carOverallDimensions[0] <<"\"/>	 " << endl;
f << "	    <attnum name=\"overall width\" unit=\"m\" val=\""<< carOverallDimensions[1] <<"\"/>	 " << endl;
f << "	    <attnum name=\"mass\" unit=\"kg\" val=\""<< massdata[6] <<"\"/>	 " << endl;
f << "	    <attnum name=\"GC height\" unit=\"m\" val=\""<< massdata[7] <<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- weight bias -->	 " << endl;
f << "	    <attnum name=\"front-rear weight repartition\" min=\"0.1\" max=\"0.9\" val=\""<< massdata[0] <<"\"/>	 " << endl;
f << "	    <attnum name=\"front right-left weight repartition\" min=\"0.4\" max=\"0.6\" val=\""<< massdata[1] <<"\"/>	 " << endl;
f << "	    <attnum name=\"rear right-left weight repartition\" min=\"0.4\" max=\"0.6\" val=\""<< massdata[2] <<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- used for inertia, smaller values indicate better mass centering -->	 " << endl;
f << "	    <attnum name=\"mass repartition coefficient\" max=\"1.0\" min=\"0.4\" val=\""<< massdata[3] <<"\"/>	 " << endl;
f << "	    <attnum name=\"fuel tank\" unit=\"l\" val=\""<< massdata[4] <<"\"/>	 " << endl;
f << "	    <attnum name=\"initial fuel\" unit=\"l\" min=\"1.0\" max=\"100.0\" val=\""<< massdata[5] <<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
if (flameS ==1)
{
f << "	  <section name=\"Exhaust\">	 " << endl;
f << "		 " << endl;
f << "	    <!-- for flames -->	 " << endl;
f << "	    <attnum name=\"power\" val=\""<<flamePower<<"\"/>	 " << endl;
f << "	    <section name=\"1\">	 " << endl;
f << "	      <attnum name=\"xpos\" val=\""<<flame1[0]<<"\"/>	 " << endl;
f << "	      <attnum name=\"ypos\" val=\""<<flame1[1]<<"\"/>	 " << endl;
f << "	      <attnum name=\"zpos\" val=\""<<flame1[2]<<"\"/>	 " << endl;
f << "	    </section>	 " << endl;
f << "		 " << endl;
f << "	    <section name=\"2\">	 " << endl;
f << "	      <attnum name=\"xpos\" val=\""<<flame2[0]<<"\"/>	 " << endl;
f << "	      <attnum name=\"ypos\" val=\""<<flame2[1]<<"\"/>	 " << endl;
f << "	      <attnum name=\"zpos\" val=\""<<flame2[2]<<"\"/>	 " << endl;
f << "	    </section>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
}
f << "		 " << endl;
f << "	  <section name=\"Aerodynamics\">	 " << endl;
f << "	    <attnum name=\"Cx\" val=\""<<aerodynamics[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"front area\" unit=\"m2\" val=\""<<aerodynamics[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"front Clift\" val=\""<<aerodynamics[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"rear Clift\" val=\""<<aerodynamics[3]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Wing\">	 " << endl;
f << "	    <attnum name=\"area\" unit=\"m2\" val=\""<<frontwing[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"angle\" unit=\"deg\" val=\""<<frontwing[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"xpos\" unit=\"m\" val=\""<<frontwing[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"zpos\" unit=\"m\" val=\""<<frontwing[3]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Rear Wing\">	 " << endl;
f << "	    <attnum name=\"area\" unit=\"m2\" val=\""<<rearwing[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"angle\" unit=\"deg\" val=\""<<rearwing[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"xpos\" unit=\"m\" val=\""<<rearwing[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"zpos\" unit=\"m\" val=\""<<rearwing[3]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "		 " << endl;
f << "	  <!-- Same engine for every one -->	 " << endl;
f << "	  <section name=\"Engine\">	 " << endl;
f << "	    <attnum name=\"inertia\" min=\"0.1\" max=\"0.5\" unit=\"kg.m2\" val=\""<<engineparams[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"revs maxi\" unit=\"rpm\" min=\"5000\" max=\"20000\" val=\""<<engineparams[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"revs limiter\" unit=\"rpm\" min=\"3000\" max=\"20000\" val=\""<<engineparams[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"tickover\" unit=\"rpm\" val=\""<<engineparams[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"fuel cons factor\" min=\"1.1\" max=\"1.3\" val=\""<<engineparams[4]<<"\"/>	 " << endl;

f << "	    <section name=\"data points\">	 " << endl;
if (turboS == 1)
{
f << "	      <attstr name=\"turbo\"  val=\"true\"/>	 " << endl;
f << "	      <attnum name=\"turbo rpm\" unit=\"rpm\" val=\""<<turbo[0]<<"\"/>" << endl;
f << "	      <attnum name=\"turbo factor\"  val=\""<<turbo[1]<<"\"/>" << endl; 
f << "	      <attnum name=\"turbo lag\"  val=\""<<turbo[2]<<"\"/>" << endl;     
}    
f << "	      <section name=\"1\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[0]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"2000.0\" val=\""<<tqValue[0]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"2\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[1]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1473.0\" val=\""<<tqValue[1]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"3\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[2]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1355.0\" val=\""<<tqValue[2]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"4\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[3]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1275.0\" val=\""<<tqValue[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"5\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[4]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1145.0\" val=\""<<tqValue[4]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"6\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[5]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"1000.0\" val=\""<<tqValue[5]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"7\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[6]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"884.0\" val=\""<<tqValue[6]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"8\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[7]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"799.0\" val=\""<<tqValue[7]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"9\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[8]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"735.0\" val=\""<<tqValue[8]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"10\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[9]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"686.0\" val=\""<<tqValue[9]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"11\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[10]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"647.0\" val=\""<<tqValue[10]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"12\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[11]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"971.0\" val=\""<<tqValue[11]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"13\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[12]<<"\"/>	 " << endl;
f << "	 <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"890.0\" val=\""<<tqValue[12]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"14\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[13]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"821.0\" val=\""<<tqValue[13]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"15\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[14]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"762.0\" val=\""<<tqValue[14]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"16\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[15]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"712.0\" val=\""<<tqValue[15]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"17\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[16]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"667.0\" val=\""<<tqValue[16]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"18\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[17]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"628.0\" val=\""<<tqValue[17]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"19\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[18]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"628.0\" val=\""<<tqValue[18]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"20\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[19]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"628.0\" val=\""<<tqValue[19]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
f << "	      <section name=\"21\">	 " << endl;
f << "	        <attnum name=\"rpm\" unit=\"rpm\" val=\""<<rpmValue[20]<<"\"/>	 " << endl;
f << "	        <attnum name=\"Tq\" unit=\"N.m\" min=\"0.0\" max=\"628.0\" val=\""<<tqValue[20]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "	    </section>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Clutch\">	 " << endl;
f << "		 " << endl;
f << "	    <!-- pressure plate -->	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<gearboxinertia[0]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Gearbox\">	 " << endl;
f << "	    <attnum name=\"shift time\" val=\""<<gearbox_shift_time<<"\" unit=\"s\"/>	 " << endl;
f << "	    <section name=\"gears\">	 " << endl;
f << "	      <section name=\"r\">	 " << endl;
f << "	        <attnum name=\"ratio\" min=\"-3\" max=\"0\" val=\""<<gearboxratio[0]<<"\"/>	 " << endl;
f << "	        <attnum name=\"inertia\" val=\""<<gearboxinertia[1]<<"\"/>	 " << endl;
f << "	        <attnum name=\"efficiency\" val=\""<<gearboxefficiency[0]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
if ( numberOfGears >= 1)
{
f << "	      <section name=\"1\">	 " << endl;
f << "	        <attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<gearboxratio[1]<<"\"/>	 " << endl;
f << "	        <attnum name=\"inertia\" val=\""<<gearboxinertia[2]<<"\"/>	 " << endl;
f << "	        <attnum name=\"efficiency\" val=\""<<gearboxefficiency[1]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
}
if ( numberOfGears >= 2)
{
f << "	      <section name=\"2\">	 " << endl;
f << "	        <attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<gearboxratio[2]<<"\"/>	 " << endl;
f << "	        <attnum name=\"inertia\" val=\""<<gearboxinertia[3]<<"\"/>	 " << endl;
f << "	        <attnum name=\"efficiency\" val=\""<<gearboxefficiency[2]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
}
if ( numberOfGears >= 3)
{
f << "	      <section name=\"3\">	 " << endl;
f << "	        <attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<gearboxratio[3]<<"\"/>	 " << endl;
f << "	        <attnum name=\"inertia\" val=\""<<gearboxinertia[4]<<"\"/>	 " << endl;
f << "	        <attnum name=\"efficiency\" val=\""<<gearboxefficiency[3]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
}
if ( numberOfGears >= 4)
{
f << "	      <section name=\"4\">	 " << endl;
f << "	        <attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<gearboxratio[4]<<"\"/>	 " << endl;
f << "	        <attnum name=\"inertia\" val=\""<<gearboxinertia[5]<<"\"/>	 " << endl;
f << "	        <attnum name=\"efficiency\" val=\""<<gearboxefficiency[4]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
}
if ( numberOfGears >= 5)
{
f << "	      <section name=\"5\">	 " << endl;
f << "	        <attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<gearboxratio[5]<<"\"/>	 " << endl;
f << "	        <attnum name=\"inertia\" val=\""<<gearboxinertia[6]<<"\"/>	 " << endl;
f << "	        <attnum name=\"efficiency\" val=\""<<gearboxefficiency[5]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
}
if ( numberOfGears >= 6)
{
f << "	      <section name=\"6\">	 " << endl;
f << "	        <attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<gearboxratio[6]<<"\"/>	 " << endl;
f << "	        <attnum name=\"inertia\" val=\""<<gearboxinertia[7]<<"\"/>	 " << endl;
f << "	        <attnum name=\"efficiency\" val=\""<<gearboxefficiency[6]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
}
if ( numberOfGears >= 7)
{
f << "	   <section name=\"7\">	 " << endl;
f << "	        <attnum name=\"ratio\" min=\"0\" max=\"5\" val=\""<<gearboxratio[7]<<"\"/>	 " << endl;
f << "	        <attnum name=\"inertia\" val=\""<<gearboxinertia[8]<<"\"/>	 " << endl;
f << "	        <attnum name=\"efficiency\" val=\""<<gearboxefficiency[7]<<"\"/>	 " << endl;
f << "	      </section>	 " << endl;
f << "		 " << endl;
}
f << "		 " << endl;
f << "	 </section>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Steer\">	 " << endl;
f << "	    <attnum name=\"steer lock\" unit=\"deg\" min=\"1\" max=\"45\" val=\""<<steer[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"max steer speed\" unit=\"deg/s\" min=\"1\" max=\"360\" val=\""<<steer[1]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Drivetrain\">	 " << endl;
f << "	    <attstr name=\"type\" val=\""<<drivetrain_type[curr_drivetrain_type]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<drivetrain<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Differential\">	 " << endl;
f << "	    <attstr name=\"type\" in=\"SPOOL,FREE,LIMITED SLIP\" val=\""<<differential_type[curr_frontdifferential_type]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<frontdifferential[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"ratio\" min=\"0\" max=\"10\" val=\""<<frontdifferential[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"efficiency\" val=\""<<frontdifferential[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"min torque bias\" val=\""<<frontdifferential[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"max torque bias\" val=\""<<frontdifferential[4]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "	  <section name=\"Rear Differential\">	 " << endl;
f << "	    <attstr name=\"type\" in=\"SPOOL,FREE,LIMITED SLIP\" val=\""<<differential_type[curr_reardifferential_type]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<reardifferential[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"ratio\" min=\"0\" max=\"10\" val=\""<<reardifferential[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"efficiency\" val=\""<<reardifferential[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"min torque bias\" val=\""<<reardifferential[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"max torque bias\" val=\""<<reardifferential[4]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Central Differential\">	 " << endl;
f << "	    <attstr name=\"type\" in=\"SPOOL,FREE,LIMITED SLIP\" val=\""<<differential_type[curr_centraldifferential_type]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<centraldifferential[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"ratio\" min=\"0\" max=\"10\" val=\""<<centraldifferential[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"efficiency\" val=\""<<centraldifferential[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"min torque bias\" val=\""<<centraldifferential[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"max torque bias\" val=\""<<centraldifferential[4]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "	  <section name=\"Brake System\">	 " << endl;
f << "	    <attnum name=\"front-rear brake repartition\" min=\"0.3\" max=\"0.7\" val=\""<<brakesystem[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"max pressure\" unit=\"kPa\" min=\"100\" max=\"150000\" val=\""<<brakesystem[1]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Axle\">	 " << endl;
f << "	    <attnum name=\"xpos\" unit=\"m\" val=\""<<frontaxle[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<frontaxle[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"roll center height\" unit=\"m\" val=\""<<frontaxle[2]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Rear Axle\">	 " << endl;
f << "	    <attnum name=\"xpos\" unit=\"m\" val=\""<<rearaxle[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<rearaxle[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"roll center height\" unit=\"m\" val=\""<<rearaxle[2]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Right Wheel\">	 " << endl;
f << "	    <attnum name=\"ypos\" unit=\"m\" val=\""<<wheel1[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"rim diameter\" unit=\"in\" val=\""<<wheel1[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"tire width\" unit=\"mm\" val=\""<<wheel1[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"tire height-width ratio\" val=\""<<wheel1[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<wheel1[4]<<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- initial ride height -->	 " << endl;
f << "	    <attnum name=\"ride height\" unit=\"mm\" min=\"100\" max=\"300\" val=\""<<wheel1[5]<<"\"/>	 " << endl;
f << "	    <attnum name=\"toe\" unit=\"deg\" min=\"-5\" max=\"5\" val=\""<<wheel1[6]<<"\"/>	 " << endl;
f << "	    <attnum name=\"camber\" min=\"-5\" max=\"0\" unit=\"deg\" val=\""<<wheel1[7]<<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- Adherence -->	 " << endl;
f << "	    <attnum name=\"stiffness\" min=\"20.0\" max=\"50.0\" val=\""<<wheel1[8]<<"\"/>	 " << endl;
f << "	    <attnum name=\"dynamic friction\" unit=\"%\" min=\"80\" max=\"70\" val=\""<<wheel1[9]<<"\"/>	 " << endl;
f << "	    <attnum name=\"rolling resistance\" val=\""<<wheel1[10]<<"\"/>	 " << endl;
f << "	    <attnum name=\"mu\" min=\"0.05\" max=\"1.8\" val=\""<<wheel1[11]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Left Wheel\">	 " << endl;
f << "	    <attnum name=\"ypos\" unit=\"m\" val=\""<<wheel2[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"rim diameter\" unit=\"in\" val=\""<<wheel2[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"tire width\" unit=\"mm\" val=\""<<wheel2[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"tire height-width ratio\" val=\""<<wheel2[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<wheel2[4]<<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- initial ride height -->	 " << endl;
f << "	    <attnum name=\"ride height\" unit=\"mm\" min=\"100\" max=\"300\" val=\""<<wheel2[5]<<"\"/>	 " << endl;
f << "	    <attnum name=\"toe\" unit=\"deg\" min=\"-5\" max=\"5\" val=\""<<wheel2[6]<<"\"/>	 " << endl;
f << "	    <attnum name=\"camber\" min=\"-5\" max=\"0\" unit=\"deg\" val=\""<<wheel2[7]<<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- Adherence -->	 " << endl;
f << "	    <attnum name=\"stiffness\" min=\"20.0\" max=\"50.0\" val=\""<<wheel2[8]<<"\"/>	 " << endl;
f << "	    <attnum name=\"dynamic friction\" unit=\"%\" min=\"80\" max=\"70\" val=\""<<wheel2[9]<<"\"/>	 " << endl;
f << "	    <attnum name=\"rolling resistance\" val=\""<<wheel2[10]<<"\"/>	 " << endl;
f << "	    <attnum name=\"mu\" min=\"0.05\" max=\"1.8\" val=\""<<wheel2[11]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Rear Right Wheel\">	 " << endl;
f << "	    <attnum name=\"ypos\" unit=\"m\" val=\""<<wheel3[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"rim diameter\" unit=\"in\" val=\""<<wheel3[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"tire width\" unit=\"mm\" val=\""<<wheel3[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"tire height-width ratio\" val=\""<<wheel3[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<wheel3[4]<<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- initial ride height -->	 " << endl;
f << "	    <attnum name=\"ride height\" unit=\"mm\" min=\"100\" max=\"300\" val=\""<<wheel3[5]<<"\"/>	 " << endl;
f << "	    <attnum name=\"toe\" unit=\"deg\" min=\"-5\" max=\"5\" val=\""<<wheel3[6]<<"\"/>	 " << endl;
f << "	    <attnum name=\"camber\" min=\"-5\" max=\"0\" unit=\"deg\" val=\""<<wheel3[7]<<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- Adherence -->	 " << endl;
f << "	    <attnum name=\"stiffness\" min=\"20.0\" max=\"50.0\" val=\""<<wheel3[8]<<"\"/>	 " << endl;
f << "	    <attnum name=\"dynamic friction\" unit=\"%\" min=\"80\" max=\"70\" val=\""<<wheel3[9]<<"\"/>	 " << endl;
f << "	    <attnum name=\"rolling resistance\" val=\""<<wheel3[10]<<"\"/>	 " << endl;
f << "	    <attnum name=\"mu\" min=\"0.05\" max=\"1.8\" val=\""<<wheel3[11]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Rear Left Wheel\">	 " << endl;
f << "	    <attnum name=\"ypos\" unit=\"m\" val=\""<<wheel4[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"rim diameter\" unit=\"in\" val=\""<<wheel4[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"tire width\" unit=\"mm\" val=\""<<wheel4[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"tire height-width ratio\" val=\""<<wheel4[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<wheel4[4]<<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- initial ride height -->	 " << endl;
f << "	    <attnum name=\"ride height\" unit=\"mm\" min=\"100\" max=\"300\" val=\""<<wheel4[5]<<"\"/>	 " << endl;
f << "	    <attnum name=\"toe\" unit=\"deg\" min=\"-5\" max=\"5\" val=\""<<wheel4[6]<<"\"/>	 " << endl;
f << "	    <attnum name=\"camber\" min=\"-5\" max=\"0\" unit=\"deg\" val=\""<<wheel4[7]<<"\"/>	 " << endl;
f << "		 " << endl;
f << "	    <!-- Adherence -->	 " << endl;
f << "	    <attnum name=\"stiffness\" min=\"20.0\" max=\"50.0\" val=\""<<wheel4[8]<<"\"/>	 " << endl;
f << "	    <attnum name=\"dynamic friction\" unit=\"%\" min=\"80\" max=\"70\" val=\""<<wheel4[9]<<"\"/>	 " << endl;
f << "	    <attnum name=\"rolling resistance\" val=\""<<wheel4[10]<<"\"/>	 " << endl;
f << "	    <attnum name=\"mu\" min=\"0.05\" max=\"1.8\" val=\""<<wheel4[11]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Anti-Roll Bar\">	 " << endl;
f << "	    <attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"5000\" val=\""<<antirollbar1[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<antirollbar1[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"bellcrank\" min=\"1\" max=\"5\" val=\""<<antirollbar1[2]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Rear Anti-Roll Bar\">	 " << endl;
f << "	    <attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"5000\" val=\""<<antirollbar2[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<antirollbar2[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"bellcrank\" min=\"1\" max=\"5\" val=\""<<antirollbar2[2]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Right Suspension\">	 " << endl;
f << "	    <attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"10000\" val=\""<<suspension1[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<suspension1[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"bellcrank\" min=\"0.1\" max=\"5\" val=\""<<suspension1[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"packers\" unit=\"mm\" min=\"0\" max=\"10\" val=\""<<suspension1[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"slow bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension1[4]<<"\"/>	 " << endl;
f << "	    <attnum name=\"slow rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension1[5]<<"\"/>	 " << endl;
f << "	    <attnum name=\"fast bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension1[6]<<"\"/>	 " << endl;
f << "	    <attnum name=\"fast rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension1[7]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Left Suspension\">	 " << endl;
f << "	    <attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"10000\" val=\""<<suspension2[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<suspension2[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"bellcrank\" min=\"0.1\" max=\"5\" val=\""<<suspension2[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"packers\" unit=\"mm\" min=\"0\" max=\"10\" val=\""<<suspension2[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"slow bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension2[4]<<"\"/>	 " << endl;
f << "	    <attnum name=\"slow rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension2[5]<<"\"/>	 " << endl;
f << "	    <attnum name=\"fast bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension2[6]<<"\"/>	 " << endl;
f << "	    <attnum name=\"fast rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension2[7]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Rear Right Suspension\">	 " << endl;
f << "	    <attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"10000\" val=\""<<suspension3[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<suspension3[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"bellcrank\" min=\"0.1\" max=\"5\" val=\""<<suspension3[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"packers\" unit=\"mm\" min=\"0\" max=\"10\" val=\""<<suspension3[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"slow bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension3[4]<<"\"/>	 " << endl;
f << "	    <attnum name=\"slow rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension3[5]<<"\"/>	 " << endl;
f << "	    <attnum name=\"fast bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension3[6]<<"\"/>	 " << endl;
f << "	    <attnum name=\"fast rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension3[7]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Rear Left Suspension\">	 " << endl;
f << "	    <attnum name=\"spring\" unit=\"lbs/in\" min=\"0\" max=\"10000\" val=\""<<suspension4[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"suspension course\" unit=\"m\" min=\"0\" max=\"0.2\" val=\""<<suspension4[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"bellcrank\" min=\"0.1\" max=\"5\" val=\""<<suspension4[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"packers\" unit=\"mm\" min=\"0\" max=\"10\" val=\""<<suspension4[3]<<"\"/>	 " << endl;
f << "	    <attnum name=\"slow bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension4[4]<<"\"/>	 " << endl;
f << "	    <attnum name=\"slow rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension4[5]<<"\"/>	 " << endl;
f << "	    <attnum name=\"fast bump\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension4[6]<<"\"/>	 " << endl;
f << "	    <attnum name=\"fast rebound\" unit=\"lbs/in/s\" min=\"0\" max=\"1000\" val=\""<<suspension4[7]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Right Brake\">	 " << endl;
f << "	    <attnum name=\"disk diameter\" unit=\"mm\" min=\"100\" max=\"380\" val=\""<<brake1[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"piston area\" unit=\"cm2\" val=\""<<brake1[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"mu\" val=\""<<brake1[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<brake1[3]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Front Left Brake\">	 " << endl;
f << "	    <attnum name=\"disk diameter\" unit=\"mm\" min=\"100\" max=\"380\" val=\""<<brake2[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"piston area\" unit=\"cm2\" val=\""<<brake2[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"mu\" val=\""<<brake2[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<brake2[3]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Rear Right Brake\">	 " << endl;
f << "	    <attnum name=\"disk diameter\" unit=\"mm\" min=\"100\" max=\"380\" val=\""<<brake3[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"piston area\" unit=\"cm2\" val=\""<<brake3[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"mu\" val=\""<<brake3[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<brake3[3]<<"\"/>	 " << endl;
f << "	  </section>	 " << endl;
f << "		 " << endl;
f << "	  <section name=\"Rear Left Brake\">	 " << endl;
f << "	    <attnum name=\"disk diameter\" unit=\"mm\" min=\"100\" max=\"380\" val=\""<<brake4[0]<<"\"/>	 " << endl;
f << "	    <attnum name=\"piston area\" unit=\"cm2\" val=\""<<brake4[1]<<"\"/>	 " << endl;
f << "	    <attnum name=\"mu\" val=\""<<brake4[2]<<"\"/>	 " << endl;
f << "	    <attnum name=\"inertia\" unit=\"kg.m2\" val=\""<<brake4[3]<<"\"/>	 " << endl;
f << "	  </section>" << endl;
f << "	</params>" << endl;



    f.close();  //cierre del fichero
    cout << "File expoted:" << carname << ".xml" << endl;
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
           strcat(fichero4,carname.c_str());
           strcat(fichero4,"/");
           strcat(fichero4,carname.c_str());
           strcat(fichero4,".xml");


    ifstream f;  //fichero de salid
    int i;
    f.open(fichero4);  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "Error openig the file " << fichero4 << endl;
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
    cout << "Readed " << xmlLine.size() << " lines from " << fichero4  << endl;
    f.close();
    }

/** SAVE THE BACKUP **/
           
           fichero4[0]='\0';
           strcat(fichero4,"xml-backups/");
           strcat(fichero4,carname.c_str());
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
       
           strcat(fichero4,carname.c_str());
           strcat(fichero4,"-");
           strcat(fichero4,timetorcs.c_str());
           strcat(fichero4,".xml");
                   

    ofstream f2;  //fichero de salid
    f2.open(fichero4);  //apertura del fichero o creacción si no existe
    if(!f2)
        cout << "Error openig the file " << fichero4 << endl;
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
