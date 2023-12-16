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

extern GLUI_List       *helpList;
extern GLUI_TextBox    *helpTextBox;

std::string helpText = "";

void openHelpFile(std::string &helpString, const std::string &file)
{
    helpString.clear();
    std::string filePath = "data/help_list/";
    filePath += file;
    ifstream f;  //fichero de salid
    f.open(filePath.c_str());  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "Error opening the " << filePath << " file." << endl;
    else
    {
        while (!f.eof()!=0)
        {
	    std::string buffer;
            getline(f, buffer);
            helpString += buffer;
            helpString += "\n";
        }
    } 
    f.close(); 
}    


void helpSelectList(int control)
{   
    int item;
    item = helpList->get_current_item();
    switch (item)
    {
       case 0: openHelpFile(helpText,"readme.txt"); break;
       case 1: openHelpFile(helpText,"frontWing.txt"); break;
       case 2: openHelpFile(helpText,"rearWing.txt"); break;
       case 3: openHelpFile(helpText,"engine.txt"); break;
       case 4: openHelpFile(helpText,"gears.txt"); break;
       case 5: openHelpFile(helpText,"brakes.txt"); break;
       case 6: openHelpFile(helpText,"wheelsTires.txt"); break;
       case 7: openHelpFile(helpText,"steer.txt");break;
       case 8: openHelpFile(helpText,"suspension.txt"); break;
   }
   helpTextBox->set_text(helpText.c_str());    
}   
