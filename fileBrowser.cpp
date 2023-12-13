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
#include "portability.h"

using namespace std;

#include "cardata.h"

extern GLUI *glui15;
void hideglui (int gluiname);
void savexml( int i );
void importxml (int k);

GLUI_FileBrowser *FileBrowser;
std::string file_Name = "";
char dir[500] = "";

void getFilenameFormFileBrowser ( int k )
{      
    switch ( k )
    {
    case 0:
            file_Name = FileBrowser->get_file();
            cout << "file_name " << file_Name << endl;
    break;
    case 1:
            file_Name = FileBrowser->get_file();
            cout << "file_name " << file_Name << endl;
            
            //carname.assign(file_Name,0,file_Name.size()-4);
            importxml(1);
            chdir (dir);
            glui15->hide();
    break;
    }    
    
}    

void hideFileBrowserWindow ( void )
{ 
    chdir (dir);
    glui15->hide();
}    

/* Window for Warning Message: call warningMsg(int warningn) to set the message*/
void fileBrowserWindow(int file)
{
  glui15 = GLUI_Master.create_glui("File Browser:", 0, 100, 150);/* name, flags,
								 x, and y */
  //glui15->hide();  we don't hide the window because we show it when we call warningMsg(int warningn)

  new GLUI_StaticText( glui15, "Select the file" );
  
  FileBrowser =  new GLUI_FileBrowser(glui15, "File", true, 0, (GLUI_Update_CB)getFilenameFormFileBrowser);
  FileBrowser->set_w(450);
  FileBrowser->set_w(300);
  
  getcwd(dir,500);
  cout << "TCSE dir: " << dir << endl; 
  
  new GLUI_Button( glui15, "Import XML",  1, (GLUI_Update_CB)getFilenameFormFileBrowser);
              
  new GLUI_Button( glui15, "Close Window", 15, (GLUI_Update_CB)hideFileBrowserWindow );
}
