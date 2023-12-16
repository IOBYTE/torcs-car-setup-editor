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
           fichero4 += carname;
           fichero4 += "/";
           fichero4 += carname;
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

void getXmlValf (
    float &valueRead,
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
    while ( line < xmlLine.size() )
    {
    idx = xmlLine[line].find(val);
    //cout << "idx: " << idx << endl;
    //cout << "Line " << line << ": "<< xmlLine[line] << endl;
    if (idx > 0) break;
    line++;
    }
    if ( idx < 0 )
    {
        cout << "Value " << val << " not found" << endl;
    }
    else
    { // else 3
    idx1 = idx + val.length();
    idx2 = xmlLine[line].find("\"",idx1);
    valueString.assign(xmlLine[line],idx1,idx2-idx1);
    //cout << "Value string: " << valueString << endl;
    if (xmlLine[line].find("<attnum") != std::string::npos)
    {
        valueRead = atof( valueString.c_str() ); /* atoi --> int */
        cout << "Value " << section << ":" << name << " read: " << valueRead << endl;
    }
    }//else3
}//else2
}//else1


    GLUI_Master.sync_live_all();
    }
///////////////////////////////////////////////////////////////////

void getXmlVali (
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
    while ( line < xmlLine.size() )
    {
    idx = xmlLine[line].find(val);
    //cout << "idx: " << idx << endl;
    //cout << "Line " << line << ": "<< xmlLine[line] << endl;
    if (idx > 0) break;
    line++;
    }
    if ( idx < 0 )
    {
        cout << "Value " << val << " not found" << endl;
    }
    else
    { // else 3
    idx1 = idx + val.length();
    idx2 = xmlLine[line].find("\"",idx1);
    valueString.assign(xmlLine[line],idx1,idx2-idx1);
    //cout << "Value string: " << valueString << endl;
    if (xmlLine[line].find("<attnum") != std::string::npos)
    {
        valueRead = atoi( valueString.c_str() ); /* atoi --> int */
        cout << "Value " << section << ":" << name << " read: " << valueRead << endl;
    }
    }//else3
}//else2
}//else1


    GLUI_Master.sync_live_all();
    }

////////////////////////////////////////////////////////////////////

void getXmlVal (
    std::string &valueRead,
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
    int endOfSubSection2 = -1;
    int endOfSubSection1 = -1;
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
    while ( line < xmlLine.size())
    {
    idxSubSection2 = xmlLine[line].find("name=\"" + subSection2 +  "\"");
    //cout << "idx: " << idxSection << endl;
    //cout << "Line " << line << ": "<< xmlLine[line] << endl;

/*
    int startOfSubSection2 = -1;
    int startOfSubSection2T = -1;
    int endOfSubSection2T = -1;
    startOfSubSection2 = xmlLine.at(line).find("<section");
    endOfSubSection2 = xmlLine.at(line).find("</section>");
    if (startOfSubSection2 >= 0 )
    {
        startOfSubSection2T = 1;
    }
    if (endOfSubSection2 >= 0 && startOfSubSection2T == 1)
    {
        startOfSubSection2T = -1;
        endOfSubSection2T = -1;
    }
    if (endOfSubSection2 >= 0 && startOfSubSection2T == -1)
    {
        startOfSubSection2T = -1;
        endOfSubSection2T = 1;
    }
    if (endOfSubSection2T == 1)
    {
     cout << "Value " << val << " not found" << endl;   break;
    }
 */
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
    while ( line < xmlLine.size() )
    {
    idx = xmlLine[line].find(val);
    //cout << "idx: " << idx << endl;
    //cout << "Line " << line << ": "<< xmlLine[line] << endl;
    if (idx > 0) break;
    line++;

    }
    if ( idx < 0 )
    {
        cout << "Value " << val << " not found" << endl;
    }
    else
    { // else 3
    idx1 = idx + val.length();
    idx2 = xmlLine[line].find("\"",idx1);
    valueString.assign(xmlLine[line],idx1,idx2-idx1);
    valueRead = valueString;
    cout << "Value " << section << ":" << name << " read: " << valueRead << endl;
   }//else3
}//else2
}//else1


    GLUI_Master.sync_live_all();
    }

////////////////////////////////////////////////////////////////////

void getXmlUnits (
    std::string &valueRead,
    const std::string &name,
    const std::string &section,
    const std::string &subSection1 = "",
    const std::string &subSection2 = "" )
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
    int endOfSubSection2 = -1;
    int endOfSubSection1 = -1;
    string units = "unit=\"";
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
    if (!subSection1.empty())
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
    if (!subSection2.empty())
    {
    while ( line < xmlLine.size())
    {
    idxSubSection2 = xmlLine[line].find("name=\"" + subSection2 +  "\"");
    //cout << "idx: " << idxSection << endl;
    //cout << "Line " << line << ": "<< xmlLine[line] << endl;

/*
    int startOfSubSection2 = -1;
    int startOfSubSection2T = -1;
    int endOfSubSection2T = -1;
    startOfSubSection2 = xmlLine.at(line).find("<section");
    endOfSubSection2 = xmlLine.at(line).find("</section>");
    if (startOfSubSection2 >= 0 )
    {
        startOfSubSection2T = 1;
    }
    if (endOfSubSection2 >= 0 && startOfSubSection2T == 1)
    {
        startOfSubSection2T = -1;
        endOfSubSection2T = -1;
    }
    if (endOfSubSection2 >= 0 && startOfSubSection2T == -1)
    {
        startOfSubSection2T = -1;
        endOfSubSection2T = 1;
    }
    if (endOfSubSection2T == 1)
    {
     cout << "Value " << val << " not found" << endl;   break;
    }
 */
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
    while ( line < xmlLine.size() )
    {
        idx = xmlLine[line].find(units);
        cout << "idx: " << idx << endl;
        cout << "Line " << line << ": "<< xmlLine[line] << endl;
        if (idx > 0)
            break;
        line++;
    }
    if ( idx < 0 )
    {
        cout << "Units " << units << " not found" << endl;
    }
    else
    { // else 3
    idx1 = idx + units.length();
    idx2 = xmlLine[line].find("\"",idx1);
    valueString.assign(xmlLine[line],idx1,idx2-idx1);
    valueRead = valueString;
    cout << "Units " << section << ":" << name << " read: " << valueRead << endl;
   }//else3
}//else2
}//else1


    GLUI_Master.sync_live_all();
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
    getXmlParamName(fullCarName);

    getXmlValf (engineparams[0],"inertia","Engine");
    getXmlValf (engineparams[1],"revs maxi","Engine");
    getXmlValf (engineparams[2],"revs limiter","Engine");
    getXmlValf (engineparams[3],"tickover","Engine");
    getXmlValf (engineparams[4],"fuel cons factor","Engine");

    getXmlValf (engineCapacity, "capacity","Engine");
    string engineCapacityUnits;
    getXmlUnits (engineCapacityUnits, "capacity","Engine");
    for (int i = 0; i < 3; i++)
    {
        if (engineCapacityUnits == engine_capacity_units[i])
        {
            curr_engine_capacity_units = i;
            list_engine_capacity_units->set_int_val(curr_engine_capacity_units);
	    break;
        }
    }

    getXmlVali (engineCylinders, "cylinders","Engine");
    string engineShape;
    getXmlVal (engineShape, "shape","Engine");
    for (int i = 0; i < 4; i++)
    {
        if (engineShape == engine_shape[i])
        {
            curr_engine_shape = i;
            list_engine_shape->set_int_val(curr_engine_shape);
	    break;
        }
    }

    string enginePosition;
    getXmlVal (enginePosition, "position","Engine");
    for (int i = 0; i < 5; i++)
    {
        if (enginePosition == engine_position[i])
        {
            curr_engine_position = i;
            list_engine_position->set_int_val(curr_engine_position);
	    break;
        }
    }

    getXmlValf (rpmValue[0],"rpm","Engine","data points","1");
    getXmlValf (rpmValue[1],"rpm","Engine","data points","2");
    getXmlValf (rpmValue[2],"rpm","Engine","data points","3");
    getXmlValf (rpmValue[3],"rpm","Engine","data points","4");
    getXmlValf (rpmValue[4],"rpm","Engine","data points","5");
    getXmlValf (rpmValue[5],"rpm","Engine","data points","6");
    getXmlValf (rpmValue[6],"rpm","Engine","data points","7");
    getXmlValf (rpmValue[7],"rpm","Engine","data points","8");
    getXmlValf (rpmValue[8],"rpm","Engine","data points","9");
    getXmlValf (rpmValue[9],"rpm","Engine","data points","10");
    getXmlValf (rpmValue[10],"rpm","Engine","data points","11");
    getXmlValf (rpmValue[11],"rpm","Engine","data points","12");
    getXmlValf (rpmValue[12],"rpm","Engine","data points","13");
    getXmlValf (rpmValue[13],"rpm","Engine","data points","14");
    getXmlValf (rpmValue[14],"rpm","Engine","data points","15");
    getXmlValf (rpmValue[15],"rpm","Engine","data points","16");
    getXmlValf (rpmValue[16],"rpm","Engine","data points","17");
    getXmlValf (rpmValue[17],"rpm","Engine","data points","18");
    getXmlValf (rpmValue[18],"rpm","Engine","data points","19");
    getXmlValf (rpmValue[19],"rpm","Engine","data points","20");
    getXmlValf (rpmValue[20],"rpm","Engine","data points","21");

    getXmlValf (tqValue[0],"Tq","Engine","data points","1");
    getXmlValf (tqValue[1],"Tq","Engine","data points","2");
    getXmlValf (tqValue[2],"Tq","Engine","data points","3");
    getXmlValf (tqValue[3],"Tq","Engine","data points","4");
    getXmlValf (tqValue[4],"Tq","Engine","data points","5");
    getXmlValf (tqValue[5],"Tq","Engine","data points","6");
    getXmlValf (tqValue[6],"Tq","Engine","data points","7");
    getXmlValf (tqValue[7],"Tq","Engine","data points","8");
    getXmlValf (tqValue[8],"Tq","Engine","data points","9");
    getXmlValf (tqValue[9],"Tq","Engine","data points","10");
    getXmlValf (tqValue[10],"Tq","Engine","data points","11");
    getXmlValf (tqValue[11],"Tq","Engine","data points","12");
    getXmlValf (tqValue[12],"Tq","Engine","data points","13");
    getXmlValf (tqValue[13],"Tq","Engine","data points","14");
    getXmlValf (tqValue[14],"Tq","Engine","data points","15");
    getXmlValf (tqValue[15],"Tq","Engine","data points","16");
    getXmlValf (tqValue[16],"Tq","Engine","data points","17");
    getXmlValf (tqValue[17],"Tq","Engine","data points","18");
    getXmlValf (tqValue[18],"Tq","Engine","data points","19");
    getXmlValf (tqValue[19],"Tq","Engine","data points","20");
    getXmlValf (tqValue[20],"Tq","Engine","data points","21");
    CalcCV(); // after reading the tq we calculate the cv to update the values of cv

    std::string bufferValStr = "";
    getXmlVal (bufferValStr,"turbo","Engine","data points");
    if (bufferValStr == "true")
    {
        turboS = 1;
        GLUI_Master.sync_live_all();
    }
    else
    {
        turboS = 0;
        GLUI_Master.sync_live_all();
    }
    getXmlValf (turbo[0],"turbo rpm","Engine","data points");
    getXmlValf (turbo[1],"turbo factor","Engine","data points");
    getXmlValf (turbo[2],"turbo lag","Engine","data points");

    getXmlValf (gearbox_shift_time,"shift time","Gearbox");
    getXmlValf (gearboxratio[0],"ratio","Gearbox","gears","r");
    getXmlValf (gearboxratio[1],"ratio","Gearbox","gears","1");
    getXmlValf (gearboxratio[2],"ratio","Gearbox","gears","2");
    getXmlValf (gearboxratio[3],"ratio","Gearbox","gears","3");
    getXmlValf (gearboxratio[4],"ratio","Gearbox","gears","4");
    getXmlValf (gearboxratio[5],"ratio","Gearbox","gears","5");
    getXmlValf (gearboxratio[6],"ratio","Gearbox","gears","6");
    getXmlValf (gearboxratio[7],"ratio","Gearbox","gears","7");

    getXmlNumberOfGears (numberOfGears,"ratio","Gearbox","gears","1");
    getXmlNumberOfGears (numberOfGears,"ratio","Gearbox","gears","2");
    getXmlNumberOfGears (numberOfGears,"ratio","Gearbox","gears","3");
    getXmlNumberOfGears (numberOfGears,"ratio","Gearbox","gears","4");
    getXmlNumberOfGears (numberOfGears,"ratio","Gearbox","gears","5");
    getXmlNumberOfGears (numberOfGears,"ratio","Gearbox","gears","6");
    getXmlNumberOfGears (numberOfGears,"ratio","Gearbox","gears","7");
    numberOfGearsDisable( );



    getXmlValf (gearboxinertia[0],"inertia","Clutch");
    getXmlValf (gearboxinertia[1],"inertia","Gearbox","gears","r");
    getXmlValf (gearboxinertia[2],"inertia","Gearbox","gears","1");
    getXmlValf (gearboxinertia[3],"inertia","Gearbox","gears","2");
    getXmlValf (gearboxinertia[4],"inertia","Gearbox","gears","3");
    getXmlValf (gearboxinertia[5],"inertia","Gearbox","gears","4");
    getXmlValf (gearboxinertia[6],"inertia","Gearbox","gears","5");
    getXmlValf (gearboxinertia[7],"inertia","Gearbox","gears","6");
    getXmlValf (gearboxinertia[8],"inertia","Gearbox","gears","7");

    getXmlValf (gearboxefficiency[0],"efficiency","Gearbox","gears","r");
    getXmlValf (gearboxefficiency[1],"efficiency","Gearbox","gears","1");
    getXmlValf (gearboxefficiency[2],"efficiency","Gearbox","gears","2");
    getXmlValf (gearboxefficiency[3],"efficiency","Gearbox","gears","3");
    getXmlValf (gearboxefficiency[4],"efficiency","Gearbox","gears","4");
    getXmlValf (gearboxefficiency[5],"efficiency","Gearbox","gears","5");
    getXmlValf (gearboxefficiency[6],"efficiency","Gearbox","gears","6");
    getXmlValf (gearboxefficiency[7],"efficiency","Gearbox","gears","7");

    getXmlValf (antirollbar1[0],"spring","Front Anti-Roll Bar");
    getXmlValf (antirollbar1[1],"suspension course","Front Anti-Roll Bar");
    getXmlValf (antirollbar1[2],"bellcrank","Front Anti-Roll Bar");

    getXmlValf (antirollbar2[0],"spring","Rear Anti-Roll Bar");
    getXmlValf (antirollbar2[1],"suspension course","Rear Anti-Roll Bar");
    getXmlValf (antirollbar2[2],"bellcrank","Rear Anti-Roll Bar");

    getXmlValf (suspension1[0],"spring","Front Right Suspension");
    getXmlValf (suspension1[1],"suspension course","Front Right Suspension");
    getXmlValf (suspension1[2],"bellcrank","Front Right Suspension");
    getXmlValf (suspension1[3],"packers","Front Right Suspension");
    getXmlValf (suspension1[4],"slow bump","Front Right Suspension");
    getXmlValf (suspension1[5],"slow rebound","Front Right Suspension");
    getXmlValf (suspension1[6],"fast bump","Front Right Suspension");
    getXmlValf (suspension1[7],"fast rebound","Front Right Suspension");

    getXmlValf (suspension2[0],"spring","Front Left Suspension");
    getXmlValf (suspension2[1],"suspension course","Front Left Suspension");
    getXmlValf (suspension2[2],"bellcrank","Front Left Suspension");
    getXmlValf (suspension2[3],"packers","Front Left Suspension");
    getXmlValf (suspension2[4],"slow bump","Front Left Suspension");
    getXmlValf (suspension2[5],"slow rebound","Front Left Suspension");
    getXmlValf (suspension2[6],"fast bump","Front Left Suspension");
    getXmlValf (suspension2[7],"fast rebound","Front Left Suspension");

    getXmlValf (suspension3[0],"spring","Rear Right Suspension");
    getXmlValf (suspension3[1],"suspension course","Rear Right Suspension");
    getXmlValf (suspension3[2],"bellcrank","Rear Right Suspension");
    getXmlValf (suspension3[3],"packers","Rear Right Suspension");
    getXmlValf (suspension3[4],"slow bump","Rear Right Suspension");
    getXmlValf (suspension3[5],"slow rebound","Rear Right Suspension");
    getXmlValf (suspension3[6],"fast bump","Rear Right Suspension");
    getXmlValf (suspension3[7],"fast rebound","Rear Right Suspension");

    getXmlValf (suspension4[0],"spring","Rear Left Suspension");
    getXmlValf (suspension4[1],"suspension course","Rear Left Suspension");
    getXmlValf (suspension4[2],"bellcrank","Rear Left Suspension");
    getXmlValf (suspension4[3],"packers","Rear Left Suspension");
    getXmlValf (suspension4[4],"slow bump","Rear Left Suspension");
    getXmlValf (suspension4[5],"slow rebound","Rear Left Suspension");
    getXmlValf (suspension4[6],"fast bump","Rear Left Suspension");
    getXmlValf (suspension4[7],"fast rebound","Rear Left Suspension");

    getXmlValf (aerodynamics[0],"Cx","Aerodynamics");
    getXmlValf (aerodynamics[1],"front area","Aerodynamics");
    getXmlValf (aerodynamics[2],"front Clift","Aerodynamics");
    getXmlValf (aerodynamics[3],"rear Clift","Aerodynamics");

    getXmlValf (frontwing[0],"area","Front Wing");
    getXmlValf (frontwing[1],"angle","Front Wing");
    getXmlValf (frontwing[2],"xpos","Front Wing");
    getXmlValf (frontwing[3],"zpos","Front Wing");

    getXmlValf (rearwing[0],"area","Rear Wing");
    getXmlValf (rearwing[1],"angle","Rear Wing");
    getXmlValf (rearwing[2],"xpos","Rear Wing");
    getXmlValf (rearwing[3],"zpos","Rear Wing");

    getXmlValf (massdata[0],"front-rear weight repartition","Car");
    getXmlValf (massdata[1],"front right-left weight repartition","Car");
    getXmlValf (massdata[2],"rear right-left weight repartition","Car");
    getXmlValf (massdata[3],"mass repartition coefficient","Car");
    getXmlValf (massdata[4],"fuel tank","Car");
    getXmlValf (massdata[5],"initial fuel","Car");
    getXmlValf (massdata[6],"mass","Car");
    getXmlValf (massdata[7],"GC height","Car");

    getXmlValf (brakesystem[0],"front-rear brake repartition","Brake System");
    getXmlValf (brakesystem[1],"max pressure","Brake System");

    getXmlValf (brake1[0],"disk diameter","Front Right Brake");
    getXmlValf (brake1[1],"piston area","Front Right Brake");
    getXmlValf (brake1[2],"mu","Front Right Brake");
    getXmlValf (brake1[3],"inertia","Front Right Brake");

    getXmlValf (brake2[0],"disk diameter","Front Left Brake");
    getXmlValf (brake2[1],"piston area","Front Left Brake");
    getXmlValf (brake2[2],"mu","Front Left Brake");
    getXmlValf (brake2[3],"inertia","Front Left Brake");

    getXmlValf (brake3[0],"disk diameter","Rear Right Brake");
    getXmlValf (brake3[1],"piston area","Rear Right Brake");
    getXmlValf (brake3[2],"mu","Rear Right Brake");
    getXmlValf (brake3[3],"inertia","Rear Right Brake");

    getXmlValf (brake4[0],"disk diameter","Rear Left Brake");
    getXmlValf (brake4[1],"piston area","Rear Left Brake");
    getXmlValf (brake4[2],"mu","Rear Left Brake");
    getXmlValf (brake4[3],"inertia","Rear Left Brake");

    getXmlValf (wheel1[0],"ypos","Front Right Wheel");
    getXmlValf (wheel1[1],"rim diameter","Front Right Wheel");
    getXmlValf (wheel1[2],"tire width","Front Right Wheel");
    getXmlValf (wheel1[3],"tire height-width ratio","Front Right Wheel");
    getXmlValf (wheel1[4],"inertia","Front Right Wheel");
    getXmlValf (wheel1[5],"ride height","Front Right Wheel");
    getXmlValf (wheel1[6],"toe","Front Right Wheel");
    getXmlValf (wheel1[7],"camber","Front Right Wheel");
    getXmlValf (wheel1[8],"stiffness","Front Right Wheel");
    getXmlValf (wheel1[9],"dynamic friction","Front Right Wheel");
    getXmlValf (wheel1[10],"rolling resistance","Front Right Wheel");
    getXmlValf (wheel1[11],"mu","Front Right Wheel");

    getXmlValf (wheel2[0],"ypos","Front Left Wheel");
    getXmlValf (wheel2[1],"rim diameter","Front Left Wheel");
    getXmlValf (wheel2[2],"tire width","Front Left Wheel");
    getXmlValf (wheel2[3],"tire height-width ratio","Front Left Wheel");
    getXmlValf (wheel2[4],"inertia","Front Left Wheel");
    getXmlValf (wheel2[5],"ride height","Front Left Wheel");
    getXmlValf (wheel2[6],"toe","Front Left Wheel");
    getXmlValf (wheel2[7],"camber","Front Left Wheel");
    getXmlValf (wheel2[8],"stiffness","Front Left Wheel");
    getXmlValf (wheel2[9],"dynamic friction","Front Left Wheel");
    getXmlValf (wheel2[10],"rolling resistance","Front Left Wheel");
    getXmlValf (wheel2[11],"mu","Front Left Wheel");

    getXmlValf (wheel3[0],"ypos","Rear Right Wheel");
    getXmlValf (wheel3[1],"rim diameter","Rear Right Wheel");
    getXmlValf (wheel3[2],"tire width","Rear Right Wheel");
    getXmlValf (wheel3[3],"tire height-width ratio","Rear Right Wheel");
    getXmlValf (wheel3[4],"inertia","Rear Right Wheel");
    getXmlValf (wheel3[5],"ride height","Rear Right Wheel");
    getXmlValf (wheel3[6],"toe","Rear Right Wheel");
    getXmlValf (wheel3[7],"camber","Rear Right Wheel");
    getXmlValf (wheel3[8],"stiffness","Rear Right Wheel");
    getXmlValf (wheel3[9],"dynamic friction","Rear Right Wheel");
    getXmlValf (wheel3[10],"rolling resistance","Rear Right Wheel");
    getXmlValf (wheel3[11],"mu","Rear Right Wheel");

    getXmlValf (wheel4[0],"ypos","Rear Left Wheel");
    getXmlValf (wheel4[1],"rim diameter","Rear Left Wheel");
    getXmlValf (wheel4[2],"tire width","Rear Left Wheel");
    getXmlValf (wheel4[3],"tire height-width ratio","Rear Left Wheel");
    getXmlValf (wheel4[4],"inertia","Rear Left Wheel");
    getXmlValf (wheel4[5],"ride height","Rear Left Wheel");
    getXmlValf (wheel4[6],"toe","Rear Left Wheel");
    getXmlValf (wheel4[7],"camber","Rear Left Wheel");
    getXmlValf (wheel4[8],"stiffness","Rear Left Wheel");
    getXmlValf (wheel4[9],"dynamic friction","Rear Left Wheel");
    getXmlValf (wheel4[10],"rolling resistance","Rear Left Wheel");
    getXmlValf (wheel4[11],"mu","Rear Left Wheel");

    getXmlValf (steer[0],"steer lock","Steer");
    getXmlValf (steer[1],"max steer speed","Steer");

    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Drivetrain");
    int i;
    for (i = 0; i < 3; i++)
    {
        if (bufferValStr == drivetrain_type[i])
        {
            curr_drivetrain_type = i;
            list_drivetrain_type->set_int_val(curr_drivetrain_type);
        }
    }
    getXmlValf (drivetrain,"inertia","Drivetrain");

    //curr_frontdifferential_type
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Front Differential");
    for (i = 0; i < 5; i++)
    {
        if (bufferValStr == differential_type[i])
        {
            curr_frontdifferential_type = i;
            list_frontdifferential_type->set_int_val(curr_frontdifferential_type);
        }
    }
    getXmlValf (frontdifferential[0],"inertia","Front Differential");
    getXmlValf (frontdifferential[1],"ratio","Front Differential");
    getXmlValf (frontdifferential[2],"efficiency","Front Differential");
    getXmlValf (frontdifferential[3],"min torque bias","Front Differential");
    getXmlValf (frontdifferential[4],"max torque bias","Front Differential");

    //curr_reardifferential_type
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Rear Differential");
    for (i = 0; i < 5; i++)
    {
        if (bufferValStr == differential_type[i])
        {
            curr_reardifferential_type = i;
            list_reardifferential_type->set_int_val(curr_reardifferential_type);
        }
    }
    getXmlValf (reardifferential[0],"inertia","Rear Differential");
    getXmlValf (reardifferential[1],"ratio","Rear Differential");
    getXmlValf (reardifferential[2],"efficiency","Rear Differential");
    getXmlValf (reardifferential[3],"min torque bias","Rear Differential");
    getXmlValf (reardifferential[4],"max torque bias","Rear Differential");

    //curr_centraldifferential_type
    bufferValStr = "";
    getXmlVal (bufferValStr,"type","Central Differential");
    for (i = 0; i < 5; i++)
    {
        if (bufferValStr == differential_type[i])
        {
            curr_centraldifferential_type = i;
            list_centraldifferential_type->set_int_val(curr_centraldifferential_type);
        }
    }
    getXmlValf (centraldifferential[0],"inertia","Central Differential");
    getXmlValf (centraldifferential[1],"ratio","Central Differential");
    getXmlValf (centraldifferential[2],"efficiency","Central Differential");
    getXmlValf (centraldifferential[3],"min torque bias","Central Differential");
    getXmlValf (centraldifferential[4],"max torque bias","Central Differential");

    getXmlValf (frontaxle[0],"xpos","Front Axle");
    getXmlValf (frontaxle[1],"inertia","Front Axle");
    getXmlValf (frontaxle[2],"roll center height","Front Axle");

    getXmlValf (rearaxle[0],"xpos","Rear Axle");
    getXmlValf (rearaxle[1],"inertia","Rear Axle");
    getXmlValf (rearaxle[2],"roll center height","Rear Axle");

    getXmlVal (enginesample,"engine sample","Sound");
    getXmlValf (rmpscale,"rpm scale","Sound");

    getXmlVal (graphicenv,"env","Graphic Objects");
    getXmlVal (wheelTexture,"wheel texture","Graphic Objects");
    getXmlVal (shadowTexture,"shadow texture","Graphic Objects");
    getXmlVal (tachometerTexture,"tachometer texture","Graphic Objects");
    getXmlVal (speedometerTexture,"speedometer texture","Graphic Objects");

    getXmlVali (tachometerMinMax[0],"tachometer min value","Graphic Objects");
    getXmlVali (tachometerMinMax[1],"tachometer max value","Graphic Objects");

    getXmlVali (speedometerMinMax[0],"speedometer min value","Graphic Objects");
    getXmlVali (speedometerMinMax[1],"speedometer max value","Graphic Objects");

    getXmlVal (carRange1,"car","Ranges", "1");
    getXmlVal (carRange2,"car","Ranges", "2");
    getXmlVal (carRange3,"car","Ranges", "3");
    getXmlVal (carRange4,"car","Ranges", "4");
    getXmlVal (carRange5,"car","Ranges", "5");

    getXmlVali (carRange1dataA,"threshold","Ranges", "1");
    getXmlVali (carRange2dataA,"threshold","Ranges", "2");
    getXmlVali (carRange3dataA,"threshold","Ranges", "3");
    getXmlVali (carRange4dataA,"threshold","Ranges", "4");
    getXmlVali (carRange5dataA,"threshold","Ranges", "5");

    bufferValStr = "";
    std::string yesStr = "yes";
    getXmlVal (bufferValStr,"wheels","Ranges","1");
    carRange1dataB = 0;
    if (bufferValStr == yesStr) carRange1dataB = 1;

    bufferValStr = "";
    carRange2dataB = 0;
    getXmlVal (bufferValStr,"wheels","Ranges","2");
    if (bufferValStr == yesStr) carRange2dataB = 1;

    bufferValStr = "";
    carRange3dataB = 0;
    getXmlVal (bufferValStr,"wheels","Ranges","3");
    if (bufferValStr == yesStr) carRange3dataB = 1;

    bufferValStr = "";
    carRange4dataB = 0;
    getXmlVal (bufferValStr,"wheels","Ranges","4");
    if (bufferValStr == yesStr) carRange4dataB = 1;

    bufferValStr = "";
    carRange5dataB = 0;
    getXmlVal (bufferValStr,"wheels","Ranges","5");
    if (bufferValStr == yesStr) carRange5dataB = 1;

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
    carRange5dataC = 1;
    carRange4dataC = 1;
    carRange3dataC = 1;
    carRange2dataC = 1;
    carRange1dataC = 1;
    }
    if (numberOfRanges == 4) {
    carRange5dataC = 0;
    carRange4dataC = 1;
    carRange3dataC = 1;
    carRange2dataC = 1;
    carRange1dataC = 1;
    }
    if (numberOfRanges == 3) {
    carRange5dataC = 0;
    carRange4dataC = 0;
    carRange3dataC = 1;
    carRange2dataC = 1;
    carRange1dataC = 1;
    }
    if (numberOfRanges == 2) {
    carRange5dataC = 0;
    carRange4dataC = 0;
    carRange3dataC = 0;
    carRange2dataC = 1;
    carRange1dataC = 1;
    }
    if (numberOfRanges == 1) {
    carRange5dataC = 0;
    carRange4dataC = 0;
    carRange3dataC = 0;
    carRange2dataC = 0;
    carRange1dataC = 1;
    }
    activateDeativateGraphicRanges ( 0 );

    getXmlValf (bonnetPosition[0],"xpos","Bonnet");
    getXmlValf (bonnetPosition[1],"ypos","Bonnet");
    getXmlValf (bonnetPosition[2],"zpos","Bonnet");

    getXmlValf (driverPosition[0],"xpos","Driver");
    getXmlValf (driverPosition[1],"ypos","Driver");
    getXmlValf (driverPosition[2],"zpos","Driver");

    getXmlVal (carCategory,"category","Car");
    getXmlValf (carBodyDimensions[0],"body length","Car");
    getXmlValf (carBodyDimensions[1],"body width","Car");
    getXmlValf (carBodyDimensions[2],"body height","Car");
    getXmlValf (carOverallDimensions[0],"overall length","Car");
    getXmlValf (carOverallDimensions[1],"overall width","Car");

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
        head1aS = 1;
        getXmlValf (head1a[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head1a[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head1a[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head1a[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "head1")
        {
        head1bS = 1;
        getXmlValf (head1b[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head1b[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head1b[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head1b[3],"size","Graphic Objects","Light",lightNumbers.at(k));
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
        head2aS = 1;
        getXmlValf (head2a[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head2a[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head2a[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head2a[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "head2")
        {
        head2bS = 1;
        getXmlValf (head2b[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head2b[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head2b[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (head2b[3],"size","Graphic Objects","Light",lightNumbers.at(k));
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
        rearaS = 1;
        getXmlValf (reara[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (reara[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (reara[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (reara[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "rear")
        {
        rearbS = 1;
        getXmlValf (rearb[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (rearb[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (rearb[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (rearb[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "rear")
        {
        rearcS = 1;
        getXmlValf (rearc[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (rearc[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (rearc[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (rearc[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "rear")
        {
        reardS = 1;
        getXmlValf (reard[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (reard[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (reard[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (reard[3],"size","Graphic Objects","Light",lightNumbers.at(k));
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
        brakeaS = 1;
        getXmlValf (brakea[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brakea[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brakea[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brakea[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        cout << "read brake 1" << endl;
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "brake")
        {
        brakebS = 1;
        getXmlValf (brakeb[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brakeb[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brakeb[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brakeb[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        cout << "read brake 2" << endl;
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "brake")
        {
        brakecS = 3;
        getXmlValf (brakec[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brakec[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brakec[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brakec[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        cout << "read brake 3" << endl;
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "brake")
        {
        brakedS = 1;
        getXmlValf (braked[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (braked[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (braked[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (braked[3],"size","Graphic Objects","Light",lightNumbers.at(k));
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
        brake2aS = 1;
        getXmlValf (brake2a[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brake2a[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brake2a[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brake2a[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        k++;
        getXmlVal (bufferValStr,"type","Graphic Objects","Light",lightNumbers.at(k));
        }
    if (bufferValStr == "brake2")
        {
        brake2bS = 1;
        getXmlValf (brake2b[0],"xpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brake2b[1],"ypos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brake2b[2],"zpos","Graphic Objects","Light",lightNumbers.at(k));
        getXmlValf (brake2b[3],"size","Graphic Objects","Light",lightNumbers.at(k));
        break;
        }
    }
     /* FLAMES */
     getXmlValf (flamePower,"power","Exhaust");
     if (flamePower > 0.0)
     {
         flameS = 1;
     }
     else  flameS = 0;
     cout << "flameS= " << flameS << endl;

     getXmlValf (flame1[0],"xpos","Exhaust","1");
     getXmlValf (flame1[1],"ypos","Exhaust","1");
     getXmlValf (flame1[2],"zpos","Exhaust","1");
     getXmlValf (flame2[0],"xpos","Exhaust","2");
     getXmlValf (flame2[1],"ypos","Exhaust","2");
     getXmlValf (flame2[2],"zpos","Exhaust","2");

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

