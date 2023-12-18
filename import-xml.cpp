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
#include <cstdio>
#include <time.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cctype>

#include "cardata.h"


using namespace std;

extern std::string text1;
extern int   main_window;
extern CarData cardata;

extern GLUI_Listbox *list_engine_capacity_units;
extern GLUI_Listbox *list_engine_shape;
extern GLUI_Listbox *list_engine_position;
extern GLUI_Listbox *list_drivetrain_type;
extern GLUI_Listbox *list_frontdifferential_type;
extern GLUI_Listbox *list_reardifferential_type;
extern GLUI_Listbox *list_centraldifferential_type;


//prototypes
void warningMsg(int warningn);
void gearboxRatioSpeed ( int k );
void numberOfGearsDisable   ( void );
void CalcCV(void);
void activateDeativateGraphicRanges ( int k );

vector<string> xmlLine;
std::string fichero4;

extern std::string file_Name;

bool fileRead = false;

void readXmlFile( int k )
{

xmlLine.reserve(200);
string bufer;

    switch (k)
    {
        case 1:
            fichero4 = file_Name;
        break;
        default:
           fichero4="";
           fichero4 += "../cars/";
           fichero4 += cardata.carname;
           fichero4 += "/";
           fichero4 += cardata.carname;
           fichero4 += ".xml";
    }

    ifstream f;  //fichero de salid
    int i;
    f.open(fichero4.c_str());  //apertura del fichero o creacción si no existe
    if(!f)
        {
        cout << "Error opening the file " << fichero4 << endl;
        fileRead = false;
        }
    else
    {
    /* read the data of the xml file */
    int lineComentBegin = -1;
    int lineComentEnd = -1;
    int lineComentLines = -1;

    while (!f.eof())
        {

            getline(f, bufer);

            lineComentBegin = bufer.find("<!--");
            lineComentEnd = bufer.rfind("-->");


            if (lineComentBegin >=0)
            {
                lineComentLines = 1;
            }

            if (lineComentBegin >= 0 && lineComentEnd >= 0)
            {
                lineComentLines = 2;
                //cout << "Coment Line <!--*-->: " << bufer << endl;
            }

            if (lineComentLines == 1)
            {
              while (!f.eof())
              {
                lineComentEnd = bufer.rfind("-->");
                //cout << "Coment Line: " << bufer << endl;
                bufer = "";
                if (lineComentEnd >= 0) break;
                getline(f, bufer);
              }
              lineComentLines = -2;
            }

            int k;
            int isBlank = 0;
            int nSpace = 0;
            for ( k=0; k < bufer.size(); k++)
            {
                if (bufer.at(k) == ' ')
                {
                    nSpace++;
                }
            }
            if ( nSpace == bufer.size()-1)
            {
                isBlank = 1;
            }

            if (lineComentLines == -1 && isBlank == 0)
            {
                xmlLine.push_back(bufer);
            }

            //cout << bufer << endl;
            lineComentLines = -1;
            isBlank = 0;
            nSpace = 0;
            bufer = "";
        }

    /*
    int k;
    for ( k=0;k<xmlLine.size();k++)
    cout << "Line " << k << " :" << xmlLine.at(k) << "---" << endl;
    cout << "------------------" << endl;
    */
    cout << "Read " << xmlLine.size() << " lines from " << fichero4  << endl;
    fileRead = true;
    f.close();
    }
}

////////////////////////////////////////////////////////////////////

bool findSection(
    int &line,
    int &depth,
    const std::string &section,
    const std::string &subSection1 = "",
    const std::string &subSection2 = "" )
{

    /* SEARCH THE SECTION */
    while (line < xmlLine.size())
    {
        if (xmlLine[line].find("<section") != std::string::npos)
        {
            depth++;
            if (depth == 1 && xmlLine[line].find("name=\"" + section + "\"") != std::string::npos)
                break;
        }
        else if (xmlLine[line].find("</section>") != std::string::npos)
        {
            depth--;
        }
        line++;
    }

    if (depth != 1 || line + 1 >= xmlLine.size())
    {
        return false;
    }

    /* SEARCH THE SUBSECTION1 IF DEFINED*/
    if (!subSection1.empty())
    {
        line++;

        while (line < xmlLine.size())
        {
            if (xmlLine[line].find("<section") != std::string::npos)
            {
                depth++;
                if (depth == 2 && xmlLine[line].find("name=\"" + subSection1 + "\"") != std::string::npos)
                    break;
            }
            else if (xmlLine[line].find("</section>") != std::string::npos)
            {
                depth--;
                if (depth < 1)
                    return false;
            }
            line++;
        }

        if (depth != 2 || line + 1 >= xmlLine.size())
        {
            return false;
        }
    }

    /* SEARCH THE SUBSECTION2 IF DEFINED*/
    if (!subSection2.empty())
    {
        line++;

        while (line < xmlLine.size())
        {
            if (xmlLine[line].find("<section") != std::string::npos)
            {
                depth++;
                if (depth == 3 && xmlLine[line].find("name=\"" + subSection2 + "\"") != std::string::npos)
                    break;
            }
            else if (xmlLine[line].find("</section>") != std::string::npos)
            {
                depth--;
                if (depth < 2)
                    return false;
            }
            line++;
        }

        if (depth != 3 || line + 1 >= xmlLine.size())
        {
            return false;
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////

bool hasXmlSection (
    const std::string &section,
    const std::string &subSection1 = "",
    const std::string &subSection2 = "" )
{
    int line = 0;
    int depth = 0;

    return findSection(line, depth, section, subSection1, subSection2);
}

////////////////////////////////////////////////////////////////////

bool getXmlValf (
    float &valueRead,
    const std::string &name,
    const std::string &section,
    const std::string &subSection1 = "",
    const std::string &subSection2 = "" )
{
    int line = 0;
    int depth = 0;

    if (!findSection(line, depth, section, subSection1, subSection2))
	 return false;

    line++;

    while ( line < xmlLine.size() )
    {
        if (xmlLine[line].find("<attnum") != std::string::npos &&
            xmlLine[line].find("name=\"" + name +  "\"" ) != std::string::npos)
        {
            int idx = 0;
            const std::string val = "val=\"";
            if ((idx = xmlLine[line].find(val)) != std::string::npos)
            {
                int idx1 = idx + val.length();
                int idx2 = xmlLine[line].find("\"",idx1);
                std::string valueString;
                valueString.assign(xmlLine[line],idx1,idx2-idx1);
                valueRead = atof( valueString.c_str() );
                cout << "Value " << section << ":"
                     << (subSection1.empty() ? "" : (subSection1 + ':'))
                     << (subSection2.empty() ? "" : (subSection2 + ':'))
		     << name << " read: " << valueRead << endl;

                GLUI_Master.sync_live_all();

                return true;
            }
            return false;
        }
        else if (xmlLine[line].find("</section>") != std::string::npos)
            return false;
        line++;
    }

    return false;
}

///////////////////////////////////////////////////////////////////

bool getXmlVali (
    int &valueRead,
    const std::string &name,
    const std::string &section,
    const std::string &subSection1 = "",
    const std::string &subSection2 = "" )
{
    int line = 0;
    int depth = 0;

    if (!findSection(line, depth, section, subSection1, subSection2))
	 return false;

    line++;

    while ( line < xmlLine.size() )
    {
        if (xmlLine[line].find("<attnum") != std::string::npos &&
            xmlLine[line].find("name=\"" + name +  "\"" ) != std::string::npos)
        {
            int idx = 0;
            const std::string val = "val=\"";
            if ((idx = xmlLine[line].find(val)) != std::string::npos)
            {
                int idx1 = idx + val.length();
                int idx2 = xmlLine[line].find("\"",idx1);
                std::string valueString;
                valueString.assign(xmlLine[line],idx1,idx2-idx1);
                valueRead = atoi( valueString.c_str() );
                cout << "Value " << section << ":"
                     << (subSection1.empty() ? "" : (subSection1 + ':'))
                     << (subSection2.empty() ? "" : (subSection2 + ':'))
		     << name << " read: " << valueRead << endl;

                GLUI_Master.sync_live_all();

                return true;
            }
            return false;
        }
        else if (xmlLine[line].find("</section>") != std::string::npos)
            return false;
        line++;
    }

    return false;
}

////////////////////////////////////////////////////////////////////

bool getXmlVal (
    std::string &valueRead,
    const std::string &name,
    const std::string &section,
    const std::string &subSection1 = "",
    const std::string &subSection2 = "" )
{
    int line = 0;
    int depth = 0;

    if (!findSection(line, depth, section, subSection1, subSection2))
	 return false;

    line++;

    while ( line < xmlLine.size() )
    {
        if (xmlLine[line].find("<attstr") != std::string::npos &&
            xmlLine[line].find("name=\"" + name +  "\"" ) != std::string::npos)
        {
            int idx = 0;
            const std::string val = "val=\"";
            if ((idx = xmlLine[line].find(val)) != std::string::npos)
            {
                int idx1 = idx + val.length();
                int idx2 = xmlLine[line].find("\"",idx1);
                std::string valueString;
                valueString.assign(xmlLine[line],idx1,idx2-idx1);
                valueRead = valueString;
                cout << "Value " << section << ":"
                     << (subSection1.empty() ? "" : (subSection1 + ':'))
                     << (subSection2.empty() ? "" : (subSection2 + ':'))
		     << name << " read: " << valueRead << endl;

                GLUI_Master.sync_live_all();

                return true;
            }
            return false;
        }
        else if (xmlLine[line].find("</section>") != std::string::npos)
            return false;
        line++;
    }

    return false;
}

////////////////////////////////////////////////////////////////////

bool getXmlUnits (
    std::string &valueRead,
    const std::string &name,
    const std::string &section,
    const std::string &subSection1 = "",
    const std::string &subSection2 = "" )
{
    int line = 0;
    int depth = 0;

    if (!findSection(line, depth, section, subSection1, subSection2))
	 return false;

    line++;

    while ( line < xmlLine.size() )
    {
        if (xmlLine[line].find("<attnum") != std::string::npos &&
            xmlLine[line].find("name=\"" + name +  "\"" ) != std::string::npos)
        {
            int idx = 0;
            const std::string val = "unit=\"";
            if ((idx = xmlLine[line].find(val)) != std::string::npos)
            {
                int idx1 = idx + val.length();
                int idx2 = xmlLine[line].find("\"",idx1);
                std::string valueString;
                valueString.assign(xmlLine[line],idx1,idx2-idx1);
                valueRead = valueString;
                cout << "Value " << section << ":"
                     << (subSection1.empty() ? "" : (subSection1 + ':'))
                     << (subSection2.empty() ? "" : (subSection2 + ':'))
		     << name << " read: " << valueRead << endl;

                GLUI_Master.sync_live_all();

                return true;
            }
            return false;
        }
        else if (xmlLine[line].find("</section>") != std::string::npos)
            return false;
        line++;
    }

    return false;
}

///////////////////////////////////////////////////////////////////

void getXmlNumberOfGears (
    int &valueRead,
    string name = "revs limiter",
    string section = "Engine",
    string subSection1 = "subSection1",
    string subSection2 = "subSection2" )
{

   /* find */
    //cout << xmlLine[8] << endl;
    int idxSection = 0;
    int idxSubSection1 = 0;
    int idxSubSection2 = 0;
    int idxAtt = 0;
    int idx = 0;
    int idx1 = 0;
    int idx2 = 0;
    int line = 0;
    string val = "val=\"";
    string valueString;

    /* SEARCH THE SECTION */
    while ( line < xmlLine.size() )
    {
    idxSection = xmlLine[line].find("name=\"" + section +  "\"" );
    //cout << "idx: " << idxSection << endl;
    //cout << "Line " << line << ": "<< xmlLine[line] << endl;
    if (xmlLine[line].find("<section") == std::string::npos)
    {
        idxSection = -1;
    }
    if (idxSection > 0) break;
    line++;
    }

    if ( line + 1 >= xmlLine.size() )
        {
        cout << "Section " << "name=\"" + section +  "\""  << " not found." << endl;
        }
    else
{ //   else 1

    line++;
    /* SEARCH THE SUBSECTION1 IF DEFINED*/
    if (subSection1 != "subSection1")
    {
    while ( line < xmlLine.size() )
    {
    idxSubSection1 = xmlLine[line].find("name=\"" + subSection1 +  "\"" );
    //cout << "idx: " << idxSection << endl;
    //cout << "Line " << line << ": "<< xmlLine[line] << endl;
    if (idxSubSection1 > 0) break;
    line++;
    }

    if ( line + 1 >= xmlLine.size() )
        {
        cout << "Section " << subSection1 << " not found." << endl;
        }
    }
    //line++;

    /* SEARCH THE SUBSECTION2 IF DEFINED*/
    if (subSection2 != "subSection2")
    {
    while ( line < xmlLine.size() )
    {
    idxSubSection2 = xmlLine[line].find("name=\"" + subSection2 +  "\"");
    //cout << "idx: " << idxSection << endl;
    //cout << "Line " << line << ": "<< xmlLine[line] << endl;
    if (idxSubSection2 > 0) break;
    line++;
    }

    if ( line + 1 >= xmlLine.size() )
        {
        cout << "Section " << subSection2 << " not found." << endl;
        }
    }
    //line++;

    while ( line < xmlLine.size() )
    {
    idxAtt = xmlLine[line].find("name=\"" + name +  "\"" );
    //cout << "idx: " << idxAtt << endl;
    //cout << "Line " << line << ": "<< xmlLine[line] << endl;
    if (idxAtt > 0) break;
    line++;
    }

    if ( line + 1 >= xmlLine.size() )
        {
        cout << "Name name=\"" << name << "\" not found." << endl;
        }
    else
{ // else 2

        valueRead = atoi( subSection2.c_str() ); /* atoi --> int */
        cout << "Value " << name << " read: " << valueRead << endl;

}//else2
}//else1


    GLUI_Master.sync_live_all();
    }

////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////

void getXmlParamName ( std::string &valueRead )
{
    int idxParams = 0;
    int idxName = 0;
    std::string paramsString = "<params";
    std::string nameString = "name=\"";

    int idxAtt = 0;
    int idx = 0;
    int idx1 = 0;
    int idx2 = 0;
    int line = 0;
    string valueString;

    /* SEARCH THE PARAMS NAME */
    while ( line < xmlLine.size() )
    {
      idxParams = xmlLine.at(line).find( paramsString );

      if (idxParams >= 0) break;
      line++;
    }

    if ( line + 1 >= xmlLine.size() )
    {
        cout << paramsString << " not found." << endl;
    }

    else
{ //   else 1

    idxName = xmlLine.at(line).find(nameString);

    if (idxName < 0)
        {
        cout << "<params name not found." << endl;
        }
    else
{ // else 2
    idx1 = idxName + nameString.length();
    idx2 = xmlLine.at(line).find("\"",idx1);
    valueRead.assign(xmlLine.at(line),idx1,idx2-idx1);
    cout << "Value <params name read: " << valueRead << endl;
}//else2
}//else1


    GLUI_Master.sync_live_all();
    }

/* ------------------------------------------------*/
void importxml( int param )
{



    switch (param)
    {
    case 1:
    readXmlFile( 1 );
    break;
    default:
    readXmlFile( 0 );
    }
    if (fileRead == false )
    {
        cout << "File: " << fichero4 << " not imported." << endl;
        warningMsg(2);
    }

    if (fileRead == true )
    {
    getXmlParamName(cardata.fullCarName);

    getXmlValf (cardata.engine.inertia,"inertia","Engine");
    getXmlValf (cardata.engine.revsMaxi,"revs maxi","Engine");
    getXmlValf (cardata.engine.revsLimiter,"revs limiter","Engine");
    getXmlValf (cardata.engine.tickover,"tickover","Engine");
    getXmlValf (cardata.engine.fuelConsFactor,"fuel cons factor","Engine");

    getXmlValf (cardata.engine.capacity, "capacity","Engine");
    string engineCapacityUnits;
    getXmlUnits (engineCapacityUnits, "capacity","Engine");
    for (int i = 0; i < 3; i++)
    {
        if (engineCapacityUnits == cardata.engine.capacity_units[i])
        {
            cardata.engine.curr_capacity_units = i;
            list_engine_capacity_units->set_int_val(cardata.engine.curr_capacity_units);
	    break;
        }
    }

    getXmlVali (cardata.engine.cylinders, "cylinders","Engine");
    string engineShape;
    getXmlVal (engineShape, "shape","Engine");
    for (int i = 0; i < 4; i++)
    {
        if (engineShape == cardata.engine.shape[i])
        {
            cardata.engine.curr_shape = i;
            list_engine_shape->set_int_val(cardata.engine.curr_shape);
	    break;
        }
    }

    string enginePosition;
    getXmlVal (enginePosition, "position","Engine");
    for (int i = 0; i < 5; i++)
    {
        if (enginePosition == cardata.engine.position[i])
        {
            cardata.engine.curr_position = i;
            list_engine_position->set_int_val(cardata.engine.curr_position);
	    break;
        }
    }

    getXmlValf (cardata.engine.brakeLinearCoefficient,"brake linear coefficient","Engine");
    getXmlValf (cardata.engine.brakeCoefficient,"brake coefficient","Engine");

    getXmlValf (cardata.engine.rpmValue[0],"rpm","Engine","data points","1");
    getXmlValf (cardata.engine.rpmValue[1],"rpm","Engine","data points","2");
    getXmlValf (cardata.engine.rpmValue[2],"rpm","Engine","data points","3");
    getXmlValf (cardata.engine.rpmValue[3],"rpm","Engine","data points","4");
    getXmlValf (cardata.engine.rpmValue[4],"rpm","Engine","data points","5");
    getXmlValf (cardata.engine.rpmValue[5],"rpm","Engine","data points","6");
    getXmlValf (cardata.engine.rpmValue[6],"rpm","Engine","data points","7");
    getXmlValf (cardata.engine.rpmValue[7],"rpm","Engine","data points","8");
    getXmlValf (cardata.engine.rpmValue[8],"rpm","Engine","data points","9");
    getXmlValf (cardata.engine.rpmValue[9],"rpm","Engine","data points","10");
    getXmlValf (cardata.engine.rpmValue[10],"rpm","Engine","data points","11");
    getXmlValf (cardata.engine.rpmValue[11],"rpm","Engine","data points","12");
    getXmlValf (cardata.engine.rpmValue[12],"rpm","Engine","data points","13");
    getXmlValf (cardata.engine.rpmValue[13],"rpm","Engine","data points","14");
    getXmlValf (cardata.engine.rpmValue[14],"rpm","Engine","data points","15");
    getXmlValf (cardata.engine.rpmValue[15],"rpm","Engine","data points","16");
    getXmlValf (cardata.engine.rpmValue[16],"rpm","Engine","data points","17");
    getXmlValf (cardata.engine.rpmValue[17],"rpm","Engine","data points","18");
    getXmlValf (cardata.engine.rpmValue[18],"rpm","Engine","data points","19");
    getXmlValf (cardata.engine.rpmValue[19],"rpm","Engine","data points","20");
    getXmlValf (cardata.engine.rpmValue[20],"rpm","Engine","data points","21");

    getXmlValf (cardata.engine.tqValue[0],"Tq","Engine","data points","1");
    getXmlValf (cardata.engine.tqValue[1],"Tq","Engine","data points","2");
    getXmlValf (cardata.engine.tqValue[2],"Tq","Engine","data points","3");
    getXmlValf (cardata.engine.tqValue[3],"Tq","Engine","data points","4");
    getXmlValf (cardata.engine.tqValue[4],"Tq","Engine","data points","5");
    getXmlValf (cardata.engine.tqValue[5],"Tq","Engine","data points","6");
    getXmlValf (cardata.engine.tqValue[6],"Tq","Engine","data points","7");
    getXmlValf (cardata.engine.tqValue[7],"Tq","Engine","data points","8");
    getXmlValf (cardata.engine.tqValue[8],"Tq","Engine","data points","9");
    getXmlValf (cardata.engine.tqValue[9],"Tq","Engine","data points","10");
    getXmlValf (cardata.engine.tqValue[10],"Tq","Engine","data points","11");
    getXmlValf (cardata.engine.tqValue[11],"Tq","Engine","data points","12");
    getXmlValf (cardata.engine.tqValue[12],"Tq","Engine","data points","13");
    getXmlValf (cardata.engine.tqValue[13],"Tq","Engine","data points","14");
    getXmlValf (cardata.engine.tqValue[14],"Tq","Engine","data points","15");
    getXmlValf (cardata.engine.tqValue[15],"Tq","Engine","data points","16");
    getXmlValf (cardata.engine.tqValue[16],"Tq","Engine","data points","17");
    getXmlValf (cardata.engine.tqValue[17],"Tq","Engine","data points","18");
    getXmlValf (cardata.engine.tqValue[18],"Tq","Engine","data points","19");
    getXmlValf (cardata.engine.tqValue[19],"Tq","Engine","data points","20");
    getXmlValf (cardata.engine.tqValue[20],"Tq","Engine","data points","21");
    CalcCV(); // after reading the tq we calculate the cv to update the values of cv

    std::string bufferValStr = "";
    getXmlVal (bufferValStr,"turbo","Engine","data points");
    if (bufferValStr == "true")
    {
        cardata.engine.turboS = 1;
        GLUI_Master.sync_live_all();
    }
    else
    {
        cardata.engine.turboS = 0;
        GLUI_Master.sync_live_all();
    }
    getXmlValf (cardata.engine.turbo[0],"turbo rpm","Engine","data points");
    getXmlValf (cardata.engine.turbo[1],"turbo factor","Engine","data points");
    getXmlValf (cardata.engine.turbo[2],"turbo lag","Engine","data points");

    getXmlValf (cardata.gearbox_shift_time,"shift time","Gearbox");
    getXmlValf (cardata.gearboxratio[0],"ratio","Gearbox","gears","r");
    getXmlValf (cardata.gearboxratio[1],"ratio","Gearbox","gears","1");
    getXmlValf (cardata.gearboxratio[2],"ratio","Gearbox","gears","2");
    getXmlValf (cardata.gearboxratio[3],"ratio","Gearbox","gears","3");
    getXmlValf (cardata.gearboxratio[4],"ratio","Gearbox","gears","4");
    getXmlValf (cardata.gearboxratio[5],"ratio","Gearbox","gears","5");
    getXmlValf (cardata.gearboxratio[6],"ratio","Gearbox","gears","6");
    getXmlValf (cardata.gearboxratio[7],"ratio","Gearbox","gears","7");

    getXmlNumberOfGears (cardata.numberOfGears,"ratio","Gearbox","gears","1");
    getXmlNumberOfGears (cardata.numberOfGears,"ratio","Gearbox","gears","2");
    getXmlNumberOfGears (cardata.numberOfGears,"ratio","Gearbox","gears","3");
    getXmlNumberOfGears (cardata.numberOfGears,"ratio","Gearbox","gears","4");
    getXmlNumberOfGears (cardata.numberOfGears,"ratio","Gearbox","gears","5");
    getXmlNumberOfGears (cardata.numberOfGears,"ratio","Gearbox","gears","6");
    getXmlNumberOfGears (cardata.numberOfGears,"ratio","Gearbox","gears","7");
    numberOfGearsDisable( );



    getXmlValf (cardata.gearboxinertia[0],"inertia","Clutch");
    getXmlValf (cardata.gearboxinertia[1],"inertia","Gearbox","gears","r");
    getXmlValf (cardata.gearboxinertia[2],"inertia","Gearbox","gears","1");
    getXmlValf (cardata.gearboxinertia[3],"inertia","Gearbox","gears","2");
    getXmlValf (cardata.gearboxinertia[4],"inertia","Gearbox","gears","3");
    getXmlValf (cardata.gearboxinertia[5],"inertia","Gearbox","gears","4");
    getXmlValf (cardata.gearboxinertia[6],"inertia","Gearbox","gears","5");
    getXmlValf (cardata.gearboxinertia[7],"inertia","Gearbox","gears","6");
    getXmlValf (cardata.gearboxinertia[8],"inertia","Gearbox","gears","7");

    getXmlValf (cardata.gearboxefficiency[0],"efficiency","Gearbox","gears","r");
    getXmlValf (cardata.gearboxefficiency[1],"efficiency","Gearbox","gears","1");
    getXmlValf (cardata.gearboxefficiency[2],"efficiency","Gearbox","gears","2");
    getXmlValf (cardata.gearboxefficiency[3],"efficiency","Gearbox","gears","3");
    getXmlValf (cardata.gearboxefficiency[4],"efficiency","Gearbox","gears","4");
    getXmlValf (cardata.gearboxefficiency[5],"efficiency","Gearbox","gears","5");
    getXmlValf (cardata.gearboxefficiency[6],"efficiency","Gearbox","gears","6");
    getXmlValf (cardata.gearboxefficiency[7],"efficiency","Gearbox","gears","7");

    getXmlValf (cardata.antirollbar1[0],"spring","Front Anti-Roll Bar");
    getXmlValf (cardata.antirollbar1[1],"suspension course","Front Anti-Roll Bar");
    getXmlValf (cardata.antirollbar1[2],"bellcrank","Front Anti-Roll Bar");

    getXmlValf (cardata.antirollbar2[0],"spring","Rear Anti-Roll Bar");
    getXmlValf (cardata.antirollbar2[1],"suspension course","Rear Anti-Roll Bar");
    getXmlValf (cardata.antirollbar2[2],"bellcrank","Rear Anti-Roll Bar");

    getXmlValf (cardata.suspension1[0],"spring","Front Right Suspension");
    getXmlValf (cardata.suspension1[1],"suspension course","Front Right Suspension");
    getXmlValf (cardata.suspension1[2],"bellcrank","Front Right Suspension");
    getXmlValf (cardata.suspension1[3],"packers","Front Right Suspension");
    getXmlValf (cardata.suspension1[4],"slow bump","Front Right Suspension");
    getXmlValf (cardata.suspension1[5],"slow rebound","Front Right Suspension");
    getXmlValf (cardata.suspension1[6],"fast bump","Front Right Suspension");
    getXmlValf (cardata.suspension1[7],"fast rebound","Front Right Suspension");

    getXmlValf (cardata.suspension2[0],"spring","Front Left Suspension");
    getXmlValf (cardata.suspension2[1],"suspension course","Front Left Suspension");
    getXmlValf (cardata.suspension2[2],"bellcrank","Front Left Suspension");
    getXmlValf (cardata.suspension2[3],"packers","Front Left Suspension");
    getXmlValf (cardata.suspension2[4],"slow bump","Front Left Suspension");
    getXmlValf (cardata.suspension2[5],"slow rebound","Front Left Suspension");
    getXmlValf (cardata.suspension2[6],"fast bump","Front Left Suspension");
    getXmlValf (cardata.suspension2[7],"fast rebound","Front Left Suspension");

    getXmlValf (cardata.suspension3[0],"spring","Rear Right Suspension");
    getXmlValf (cardata.suspension3[1],"suspension course","Rear Right Suspension");
    getXmlValf (cardata.suspension3[2],"bellcrank","Rear Right Suspension");
    getXmlValf (cardata.suspension3[3],"packers","Rear Right Suspension");
    getXmlValf (cardata.suspension3[4],"slow bump","Rear Right Suspension");
    getXmlValf (cardata.suspension3[5],"slow rebound","Rear Right Suspension");
    getXmlValf (cardata.suspension3[6],"fast bump","Rear Right Suspension");
    getXmlValf (cardata.suspension3[7],"fast rebound","Rear Right Suspension");

    getXmlValf (cardata.suspension4[0],"spring","Rear Left Suspension");
    getXmlValf (cardata.suspension4[1],"suspension course","Rear Left Suspension");
    getXmlValf (cardata.suspension4[2],"bellcrank","Rear Left Suspension");
    getXmlValf (cardata.suspension4[3],"packers","Rear Left Suspension");
    getXmlValf (cardata.suspension4[4],"slow bump","Rear Left Suspension");
    getXmlValf (cardata.suspension4[5],"slow rebound","Rear Left Suspension");
    getXmlValf (cardata.suspension4[6],"fast bump","Rear Left Suspension");
    getXmlValf (cardata.suspension4[7],"fast rebound","Rear Left Suspension");

    getXmlValf (cardata.aerodynamics[0],"Cx","Aerodynamics");
    getXmlValf (cardata.aerodynamics[1],"front area","Aerodynamics");
    getXmlValf (cardata.aerodynamics[2],"front Clift","Aerodynamics");
    getXmlValf (cardata.aerodynamics[3],"rear Clift","Aerodynamics");

    getXmlValf (cardata.frontwing[0],"area","Front Wing");
    getXmlValf (cardata.frontwing[1],"angle","Front Wing");
    getXmlValf (cardata.frontwing[2],"xpos","Front Wing");
    getXmlValf (cardata.frontwing[3],"zpos","Front Wing");

    getXmlValf (cardata.rearwing[0],"area","Rear Wing");
    getXmlValf (cardata.rearwing[1],"angle","Rear Wing");
    getXmlValf (cardata.rearwing[2],"xpos","Rear Wing");
    getXmlValf (cardata.rearwing[3],"zpos","Rear Wing");

    getXmlValf (cardata.massdata[0],"front-rear weight repartition","Car");
    getXmlValf (cardata.massdata[1],"front right-left weight repartition","Car");
    getXmlValf (cardata.massdata[2],"rear right-left weight repartition","Car");
    getXmlValf (cardata.massdata[3],"mass repartition coefficient","Car");
    getXmlValf (cardata.massdata[4],"fuel tank","Car");
    getXmlValf (cardata.massdata[5],"initial fuel","Car");
    getXmlValf (cardata.massdata[6],"mass","Car");
    getXmlValf (cardata.massdata[7],"GC height","Car");

    getXmlValf (cardata.brakesystem[0],"front-rear brake repartition","Brake System");
    getXmlValf (cardata.brakesystem[1],"max pressure","Brake System");

    getXmlValf (cardata.brake1[0],"disk diameter","Front Right Brake");
    getXmlValf (cardata.brake1[1],"piston area","Front Right Brake");
    getXmlValf (cardata.brake1[2],"mu","Front Right Brake");
    getXmlValf (cardata.brake1[3],"inertia","Front Right Brake");

    getXmlValf (cardata.brake2[0],"disk diameter","Front Left Brake");
    getXmlValf (cardata.brake2[1],"piston area","Front Left Brake");
    getXmlValf (cardata.brake2[2],"mu","Front Left Brake");
    getXmlValf (cardata.brake2[3],"inertia","Front Left Brake");

    getXmlValf (cardata.brake3[0],"disk diameter","Rear Right Brake");
    getXmlValf (cardata.brake3[1],"piston area","Rear Right Brake");
    getXmlValf (cardata.brake3[2],"mu","Rear Right Brake");
    getXmlValf (cardata.brake3[3],"inertia","Rear Right Brake");

    getXmlValf (cardata.brake4[0],"disk diameter","Rear Left Brake");
    getXmlValf (cardata.brake4[1],"piston area","Rear Left Brake");
    getXmlValf (cardata.brake4[2],"mu","Rear Left Brake");
    getXmlValf (cardata.brake4[3],"inertia","Rear Left Brake");

    getXmlValf (cardata.wheel1[0],"ypos","Front Right Wheel");
    getXmlValf (cardata.wheel1[1],"rim diameter","Front Right Wheel");
    getXmlValf (cardata.wheel1[2],"tire width","Front Right Wheel");
    getXmlValf (cardata.wheel1[3],"tire height-width ratio","Front Right Wheel");
    getXmlValf (cardata.wheel1[4],"inertia","Front Right Wheel");
    getXmlValf (cardata.wheel1[5],"ride height","Front Right Wheel");
    getXmlValf (cardata.wheel1[6],"toe","Front Right Wheel");
    getXmlValf (cardata.wheel1[7],"camber","Front Right Wheel");
    getXmlValf (cardata.wheel1[8],"stiffness","Front Right Wheel");
    getXmlValf (cardata.wheel1[9],"dynamic friction","Front Right Wheel");
    getXmlValf (cardata.wheel1[10],"rolling resistance","Front Right Wheel");
    getXmlValf (cardata.wheel1[11],"mu","Front Right Wheel");

    getXmlValf (cardata.wheel2[0],"ypos","Front Left Wheel");
    getXmlValf (cardata.wheel2[1],"rim diameter","Front Left Wheel");
    getXmlValf (cardata.wheel2[2],"tire width","Front Left Wheel");
    getXmlValf (cardata.wheel2[3],"tire height-width ratio","Front Left Wheel");
    getXmlValf (cardata.wheel2[4],"inertia","Front Left Wheel");
    getXmlValf (cardata.wheel2[5],"ride height","Front Left Wheel");
    getXmlValf (cardata.wheel2[6],"toe","Front Left Wheel");
    getXmlValf (cardata.wheel2[7],"camber","Front Left Wheel");
    getXmlValf (cardata.wheel2[8],"stiffness","Front Left Wheel");
    getXmlValf (cardata.wheel2[9],"dynamic friction","Front Left Wheel");
    getXmlValf (cardata.wheel2[10],"rolling resistance","Front Left Wheel");
    getXmlValf (cardata.wheel2[11],"mu","Front Left Wheel");

    getXmlValf (cardata.wheel3[0],"ypos","Rear Right Wheel");
    getXmlValf (cardata.wheel3[1],"rim diameter","Rear Right Wheel");
    getXmlValf (cardata.wheel3[2],"tire width","Rear Right Wheel");
    getXmlValf (cardata.wheel3[3],"tire height-width ratio","Rear Right Wheel");
    getXmlValf (cardata.wheel3[4],"inertia","Rear Right Wheel");
    getXmlValf (cardata.wheel3[5],"ride height","Rear Right Wheel");
    getXmlValf (cardata.wheel3[6],"toe","Rear Right Wheel");
    getXmlValf (cardata.wheel3[7],"camber","Rear Right Wheel");
    getXmlValf (cardata.wheel3[8],"stiffness","Rear Right Wheel");
    getXmlValf (cardata.wheel3[9],"dynamic friction","Rear Right Wheel");
    getXmlValf (cardata.wheel3[10],"rolling resistance","Rear Right Wheel");
    getXmlValf (cardata.wheel3[11],"mu","Rear Right Wheel");

    getXmlValf (cardata.wheel4[0],"ypos","Rear Left Wheel");
    getXmlValf (cardata.wheel4[1],"rim diameter","Rear Left Wheel");
    getXmlValf (cardata.wheel4[2],"tire width","Rear Left Wheel");
    getXmlValf (cardata.wheel4[3],"tire height-width ratio","Rear Left Wheel");
    getXmlValf (cardata.wheel4[4],"inertia","Rear Left Wheel");
    getXmlValf (cardata.wheel4[5],"ride height","Rear Left Wheel");
    getXmlValf (cardata.wheel4[6],"toe","Rear Left Wheel");
    getXmlValf (cardata.wheel4[7],"camber","Rear Left Wheel");
    getXmlValf (cardata.wheel4[8],"stiffness","Rear Left Wheel");
    getXmlValf (cardata.wheel4[9],"dynamic friction","Rear Left Wheel");
    getXmlValf (cardata.wheel4[10],"rolling resistance","Rear Left Wheel");
    getXmlValf (cardata.wheel4[11],"mu","Rear Left Wheel");

    getXmlValf (cardata.steer[0],"steer lock","Steer");
    getXmlValf (cardata.steer[1],"max steer speed","Steer");

    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Drivetrain");
    int i;
    for (i = 0; i < 3; i++)
    {
        if (bufferValStr == cardata.drivetrain_type[i])
        {
            cardata.curr_drivetrain_type = i;
            list_drivetrain_type->set_int_val(cardata.curr_drivetrain_type);
        }
    }
    getXmlValf (cardata.drivetrain,"inertia","Drivetrain");

    //curr_frontdifferential_type
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Front Differential");
    for (i = 0; i < 5; i++)
    {
        if (bufferValStr == cardata.differential_type[i])
        {
            cardata.curr_frontdifferential_type = i;
            list_frontdifferential_type->set_int_val(cardata.curr_frontdifferential_type);
        }
    }
    getXmlValf (cardata.frontdifferential[0],"inertia","Front Differential");
    getXmlValf (cardata.frontdifferential[1],"ratio","Front Differential");
    getXmlValf (cardata.frontdifferential[2],"efficiency","Front Differential");
    getXmlValf (cardata.frontdifferential[3],"min torque bias","Front Differential");
    getXmlValf (cardata.frontdifferential[4],"max torque bias","Front Differential");

    //curr_reardifferential_type
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Rear Differential");
    for (i = 0; i < 5; i++)
    {
        if (bufferValStr == cardata.differential_type[i])
        {
            cardata.curr_reardifferential_type = i;
            list_reardifferential_type->set_int_val(cardata.curr_reardifferential_type);
        }
    }
    getXmlValf (cardata.reardifferential[0],"inertia","Rear Differential");
    getXmlValf (cardata.reardifferential[1],"ratio","Rear Differential");
    getXmlValf (cardata.reardifferential[2],"efficiency","Rear Differential");
    getXmlValf (cardata.reardifferential[3],"min torque bias","Rear Differential");
    getXmlValf (cardata.reardifferential[4],"max torque bias","Rear Differential");

    //curr_centraldifferential_type
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Central Differential");
    for (i = 0; i < 5; i++)
    {
        if (bufferValStr == cardata.differential_type[i])
        {
            cardata.curr_centraldifferential_type = i;
            list_centraldifferential_type->set_int_val(cardata.curr_centraldifferential_type);
        }
    }
    getXmlValf (cardata.centraldifferential[0],"inertia","Central Differential");
    getXmlValf (cardata.centraldifferential[1],"ratio","Central Differential");
    getXmlValf (cardata.centraldifferential[2],"efficiency","Central Differential");
    getXmlValf (cardata.centraldifferential[3],"min torque bias","Central Differential");
    getXmlValf (cardata.centraldifferential[4],"max torque bias","Central Differential");

    getXmlValf (cardata.frontaxle[0],"xpos","Front Axle");
    getXmlValf (cardata.frontaxle[1],"inertia","Front Axle");
    getXmlValf (cardata.frontaxle[2],"roll center height","Front Axle");

    getXmlValf (cardata.rearaxle[0],"xpos","Rear Axle");
    getXmlValf (cardata.rearaxle[1],"inertia","Rear Axle");
    getXmlValf (cardata.rearaxle[2],"roll center height","Rear Axle");

    getXmlVal (cardata.enginesample,"engine sample","Sound");
    getXmlValf (cardata.rmpscale,"rpm scale","Sound");

    getXmlVal (cardata.graphicenv,"env","Graphic Objects");
    getXmlVal (cardata.wheelTexture,"wheel texture","Graphic Objects");
    getXmlVal (cardata.shadowTexture,"shadow texture","Graphic Objects");
    getXmlVal (cardata.tachometerTexture,"tachometer texture","Graphic Objects");
    getXmlVal (cardata.speedometerTexture,"speedometer texture","Graphic Objects");

    getXmlVali (cardata.tachometerMinMax[0],"tachometer min value","Graphic Objects");
    getXmlVali (cardata.tachometerMinMax[1],"tachometer max value","Graphic Objects");

    getXmlVali (cardata.speedometerMinMax[0],"speedometer min value","Graphic Objects");
    getXmlVali (cardata.speedometerMinMax[1],"speedometer max value","Graphic Objects");

    int index = 1;
    std::string indexStr = "1";
    cardata.drivers.clear();
    while (hasXmlSection("Graphic Objects", "Driver", indexStr))
    {
        CarData::Driver driver;

        getXmlValf (driver.steer, "steer", "Graphic Objects", "Driver", indexStr);
        getXmlVal (driver.driver, "driver", "Graphic Objects", "Driver", indexStr);
        getXmlValf (driver.xpos, "xpos", "Graphic Objects", "Driver", indexStr);
        getXmlValf (driver.ypos, "ypos", "Graphic Objects", "Driver", indexStr);
        getXmlValf (driver.zpos, "zpos", "Graphic Objects", "Driver", indexStr);

        cardata.drivers.push_back(driver);
        indexStr = std::to_string(++index);
    }

    getXmlVal (cardata.steeringWheel.model, "model", "Graphic Objects", "Steer Wheel");
    getXmlVal (cardata.steeringWheel.hiResModel, "hi res model", "Graphic Objects", "Steer Wheel");
    getXmlValf (cardata.steeringWheel.xpos, "xpos", "Graphic Objects", "Steer Wheel");
    getXmlValf (cardata.steeringWheel.ypos, "ypos", "Graphic Objects", "Steer Wheel");
    getXmlValf (cardata.steeringWheel.zpos, "zpos", "Graphic Objects", "Steer Wheel");
    getXmlValf (cardata.steeringWheel.angle, "angle", "Graphic Objects", "Steer Wheel");
    getXmlValf (cardata.steeringWheel.movt, "movt", "Graphic Objects", "Steer Wheel");

    getXmlVal (cardata.carRange1,"car","Ranges", "1");
    getXmlVal (cardata.carRange2,"car","Ranges", "2");
    getXmlVal (cardata.carRange3,"car","Ranges", "3");
    getXmlVal (cardata.carRange4,"car","Ranges", "4");
    getXmlVal (cardata.carRange5,"car","Ranges", "5");

    getXmlVali (cardata.carRange1dataA,"threshold","Ranges", "1");
    getXmlVali (cardata.carRange2dataA,"threshold","Ranges", "2");
    getXmlVali (cardata.carRange3dataA,"threshold","Ranges", "3");
    getXmlVali (cardata.carRange4dataA,"threshold","Ranges", "4");
    getXmlVali (cardata.carRange5dataA,"threshold","Ranges", "5");

    bufferValStr = "";
    std::string yesStr = "yes";
    getXmlVal (bufferValStr,"wheels","Ranges","1");
    cardata.carRange1dataB = 0;
    if (bufferValStr == yesStr) cardata.carRange1dataB = 1;

    bufferValStr = "";
    cardata.carRange2dataB = 0;
    getXmlVal (bufferValStr,"wheels","Ranges","2");
    if (bufferValStr == yesStr) cardata.carRange2dataB = 1;

    bufferValStr = "";
    cardata.carRange3dataB = 0;
    getXmlVal (bufferValStr,"wheels","Ranges","3");
    if (bufferValStr == yesStr) cardata.carRange3dataB = 1;

    bufferValStr = "";
    cardata.carRange4dataB = 0;
    getXmlVal (bufferValStr,"wheels","Ranges","4");
    if (bufferValStr == yesStr) cardata.carRange4dataB = 1;

    bufferValStr = "";
    cardata.carRange5dataB = 0;
    getXmlVal (bufferValStr,"wheels","Ranges","5");
    if (bufferValStr == yesStr) cardata.carRange5dataB = 1;

    GLUI_Master.sync_live_all();

    int numberOfRanges = 0;
    getXmlNumberOfGears (numberOfRanges,"threshold","Graphic Objects","Ranges","1");
    cout << numberOfRanges <<  endl;
    getXmlNumberOfGears (numberOfRanges,"threshold","Graphic Objects","Ranges","2");
    cout << numberOfRanges <<  endl;
    getXmlNumberOfGears (numberOfRanges,"threshold","Graphic Objects","Ranges","3");
    cout << numberOfRanges <<  endl;
    getXmlNumberOfGears (numberOfRanges,"threshold","Graphic Objects","Ranges","4");
    cout << numberOfRanges <<  endl;
    getXmlNumberOfGears (numberOfRanges,"threshold","Graphic Objects","Ranges","5");
    cout << numberOfRanges <<  endl;

    if (numberOfRanges == 5) {
    cardata.carRange5dataC = 1;
    cardata.carRange4dataC = 1;
    cardata.carRange3dataC = 1;
    cardata.carRange2dataC = 1;
    cardata.carRange1dataC = 1;
    }
    if (numberOfRanges == 4) {
    cardata.carRange5dataC = 0;
    cardata.carRange4dataC = 1;
    cardata.carRange3dataC = 1;
    cardata.carRange2dataC = 1;
    cardata.carRange1dataC = 1;
    }
    if (numberOfRanges == 3) {
    cardata.carRange5dataC = 0;
    cardata.carRange4dataC = 0;
    cardata.carRange3dataC = 1;
    cardata.carRange2dataC = 1;
    cardata.carRange1dataC = 1;
    }
    if (numberOfRanges == 2) {
    cardata.carRange5dataC = 0;
    cardata.carRange4dataC = 0;
    cardata.carRange3dataC = 0;
    cardata.carRange2dataC = 1;
    cardata.carRange1dataC = 1;
    }
    if (numberOfRanges == 1) {
    cardata.carRange5dataC = 0;
    cardata.carRange4dataC = 0;
    cardata.carRange3dataC = 0;
    cardata.carRange2dataC = 0;
    cardata.carRange1dataC = 1;
    }
    activateDeativateGraphicRanges ( 0 );

    getXmlValf (cardata.bonnetPosition[0],"xpos","Bonnet");
    getXmlValf (cardata.bonnetPosition[1],"ypos","Bonnet");
    getXmlValf (cardata.bonnetPosition[2],"zpos","Bonnet");

    getXmlValf (cardata.driverPosition[0],"xpos","Driver");
    getXmlValf (cardata.driverPosition[1],"ypos","Driver");
    getXmlValf (cardata.driverPosition[2],"zpos","Driver");

    getXmlVal (cardata.carCategory,"category","Car");
    getXmlValf (cardata.carBodyDimensions[0],"body length","Car");
    getXmlValf (cardata.carBodyDimensions[1],"body width","Car");
    getXmlValf (cardata.carBodyDimensions[2],"body height","Car");
    getXmlValf (cardata.carOverallDimensions[0],"overall length","Car");
    getXmlValf (cardata.carOverallDimensions[1],"overall width","Car");

    vector<string> lightNumbers;
    lightNumbers.push_back("0");
    lightNumbers.push_back("1");
    lightNumbers.push_back("2");
    lightNumbers.push_back("3");
    lightNumbers.push_back("4");
    lightNumbers.push_back("5");
    lightNumbers.push_back("6");
    lightNumbers.push_back("7");
    lightNumbers.push_back("8");
    lightNumbers.push_back("9");
    lightNumbers.push_back("10");
    lightNumbers.push_back("11");
    lightNumbers.push_back("12");
    lightNumbers.push_back("13");
    lightNumbers.push_back("14");


    int k;
    //head1----------------------------------------------
    for (k=1;k<lightNumbers.size();k++)
    {
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
    if (bufferValStr == "head1")
        {
        cardata.head1aS = 1;
        getXmlValf (cardata.head1a[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head1a[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head1a[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head1a[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "head1")
        {
        cardata.head1bS = 1;
        getXmlValf (cardata.head1b[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head1b[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head1b[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head1b[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        break;
        }
    }

    //head2----------------------------------------------
    for (k=1;k<lightNumbers.size();k++)
    {
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
    if (bufferValStr == "head2")
        {
        cardata.head2aS = 1;
        getXmlValf (cardata.head2a[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head2a[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head2a[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head2a[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "head2")
        {
        cardata.head2bS = 1;
        getXmlValf (cardata.head2b[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head2b[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head2b[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.head2b[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        break;
        }
    }


    //rear----------------------------------------------
    for (k=1;k<lightNumbers.size();k++)
    {
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
    if (bufferValStr == "rear")
        {
        cardata.rearaS = 1;
        getXmlValf (cardata.reara[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.reara[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.reara[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.reara[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "rear")
        {
        cardata.rearbS = 1;
        getXmlValf (cardata.rearb[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.rearb[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.rearb[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.rearb[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "rear")
        {
        cardata.rearcS = 1;
        getXmlValf (cardata.rearc[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.rearc[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.rearc[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.rearc[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "rear")
        {
        cardata.reardS = 1;
        getXmlValf (cardata.reard[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.reard[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.reard[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.reard[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        break;
        }
    }

    //brake----------------------------------------------
    for (k=1;k<lightNumbers.size();k++)
    {
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
    if (bufferValStr == "brake")
        {
        cardata.brakeaS = 1;
        getXmlValf (cardata.brakea[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brakea[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brakea[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brakea[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        cout << "read brake 1" << endl;
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "brake")
        {
        cardata.brakebS = 1;
        getXmlValf (cardata.brakeb[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brakeb[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brakeb[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brakeb[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        cout << "read brake 2" << endl;
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "brake")
        {
        cardata.brakecS = 3;
        getXmlValf (cardata.brakec[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brakec[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brakec[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brakec[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        cout << "read brake 3" << endl;
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "brake")
        {
        cardata.brakedS = 1;
        getXmlValf (cardata.braked[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.braked[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.braked[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.braked[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        cout << "read brake 3" << endl;
        //k = lightNumbers.size(); //exit for the for
        break;
        }
    }


    //brake2----------------------------------------------
    for (k=1;k<lightNumbers.size();k++)
    {
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
    if (bufferValStr == "brake2")
        {
        cardata.brake2aS = 1;
        getXmlValf (cardata.brake2a[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brake2a[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brake2a[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brake2a[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "brake2")
        {
        cardata.brake2bS = 1;
        getXmlValf (cardata.brake2b[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brake2b[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brake2b[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (cardata.brake2b[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        break;
        }
    }
     /* FLAMES */
     getXmlValf (cardata.flamePower,"power","Exhaust");
     if (cardata.flamePower > 0.0)
     {
         cardata.flameS = 1;
     }
     else  cardata.flameS = 0;
     cout << "flameS= " << cardata.flameS << endl;

     getXmlValf (cardata.flame1[0],"xpos","Exhaust","1");
     getXmlValf (cardata.flame1[1],"ypos","Exhaust","1");
     getXmlValf (cardata.flame1[2],"zpos","Exhaust","1");
     getXmlValf (cardata.flame2[0],"xpos","Exhaust","2");
     getXmlValf (cardata.flame2[1],"ypos","Exhaust","2");
     getXmlValf (cardata.flame2[2],"zpos","Exhaust","2");

     gearboxRatioSpeed ( 1 ); /* calc the speed of each gear */

    cout << "File: " << fichero4 << " imported." << endl;

    text1 = "File: ";
    text1 += fichero4;
    text1 += " imported.";

    /*for ( i=0; i<xmlLine.size();i++)
    {
        xmlLine.at(i)="";
    }  */
    xmlLine.clear();
    GLUI_Master.sync_live_all();
}
}

