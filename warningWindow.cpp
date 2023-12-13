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

using namespace std;

#include "cardata.h"
#include "portability.h"

extern int warningn;
extern std::string trackname;
extern std::string driverName;
extern std::string driverNameNumber;

extern GLUI *glui9;
extern GLUI_TextBox *warningTextBox;
void hideglui (int gluiname);
void savexml( int i );
void fileBrowserWindow(int file);

//prototypes
void syncLiveAll (void);

void createCarnameDir ( int k )
{
    std::string carnameDir;
switch (k)
{
    
case 1:
    carnameDir = "../cars/";
    carnameDir += carname;
    makedir (carnameDir.c_str());
    cout << "created the " << carnameDir << endl;
    savexml(1);
    glui9->hide();
    break;
case 2:
    carnameDir = "../drivers/";
    carnameDir += driverName;
    makedir (carnameDir.c_str());
    carnameDir += "/";
    carnameDir += driverNameNumber;
    makedir (carnameDir.c_str());
    cout << "created the " << carnameDir << endl;
    savexml(2);
    glui9->hide();
    break;
case 3:
    carnameDir = "../drivers/";
    carnameDir += driverName;
    makedir (carnameDir.c_str());
    carnameDir += "/";
    carnameDir += driverNameNumber;
    makedir (carnameDir.c_str());
    cout << "created the " << carnameDir << endl;
    savexml(3);
    glui9->hide();
    break;

}
    
}

/* Window for Warning Message: call warningMsg(int warningn) to set the message*/
void warningMsg(int warningn)
{
  glui9 = GLUI_Master.create_glui("Warning:", 0, 100, 150);/* name, flags,
								 x, and y */
  //glui9->hide();  we don't hide the window because we show it when we call warningMsg(int warningn)

  new GLUI_StaticText( glui9, "Warning message" );
  
    warningTextBox = new GLUI_TextBox(glui9,true);
    std::string warningMessage1 = "";
    switch (warningn)
    {
        case 0: 
                warningMessage1 += "Please check the ";
                //warningMessage1 += fullCarName;
                warningMessage1 += carname;
                warningMessage1 += " or create it on the folder cars.\n";
                warningMessage1 += "The folder torcs/cars/";
                warningMessage1 += carname;
                warningMessage1 += " doesn't exists.";
                warningTextBox->set_text(warningMessage1.c_str());
                
                new GLUI_Button( glui9, "Create the carname dir and Save the XML", 1, (GLUI_Update_CB)createCarnameDir );
                new GLUI_Button( glui9, "Browse the file", 0, (GLUI_Update_CB)fileBrowserWindow );
               break;
        case 1:  
                warningMessage1 += "The cardata file hasn't been imported.\n";
                warningMessage1 += "Please check if the carname is not correct.\n\n";
                warningMessage1 += "Change the carname:\n";
                warningTextBox->set_text(warningMessage1.c_str());
                (new GLUI_EditText( glui9, "CarName", carname, 0, (GLUI_Update_CB)syncLiveAll ))->w=170;
                break;
        case 2: 
                warningMessage1 += "The XML file hasn't been imported.\n";
                warningMessage1 += "Please check if the carname is not correct.\n\n";
                warningMessage1 += "Change the carname:\n";
                warningTextBox->set_text(warningMessage1.c_str());
                (new GLUI_EditText( glui9, "CarName", carname, 0, (GLUI_Update_CB)syncLiveAll ))->w=170;
                break;
        case 3:  
                warningMessage1 += "The cardata file hasn't been imported.\n";
                warningMessage1 += "You are using a different version of TCSE.\n\n";
                warningMessage1 += "Try to import the XML file wich does'n depend on the TCSE version.\n";
                warningTextBox->set_text(warningMessage1.c_str());
                break;
        case 4: 
                warningMessage1 += "Please check the ";
                //warningMessage1 += fullCarName;
                warningMessage1 += carname;
                warningMessage1 += " or create it on the folder cars.\n";
                warningMessage1 += "The folder torcs/cars/";
                warningMessage1 += carname;
                warningMessage1 += " doesn't exists.";
                warningTextBox->set_text(warningMessage1.c_str());
                new GLUI_Button( glui9, "Create the robot dir and Save the XML for all tracks", 2, (GLUI_Update_CB)createCarnameDir );
                new GLUI_Button( glui9, "Browse the file", 0, (GLUI_Update_CB)fileBrowserWindow );
               break;
        case 5: 
                warningMessage1 += "Please check the ";
                //warningMessage1 += fullCarName;
                warningMessage1 += carname;
                warningMessage1 += " or create it on the folder cars.\n";
                warningMessage1 += "The folder torcs/cars/";
                warningMessage1 += carname;
                warningMessage1 += " doesn't exists.";
                warningTextBox->set_text(warningMessage1.c_str());
                new GLUI_Button( glui9, "Create the robot dir and Save the XML for selected track", 3, (GLUI_Update_CB)createCarnameDir );
                new GLUI_Button( glui9, "Browse the file", 0, (GLUI_Update_CB)fileBrowserWindow );
               break;
        
    }    
    
    warningTextBox->set_h(120);
    warningTextBox->set_w(750);
    warningTextBox->disable();
    warningTextBox->scrollbar->enable();
  	    
  new GLUI_Button( glui9, "Accept", 9, (GLUI_Update_CB)hideglui );
}
