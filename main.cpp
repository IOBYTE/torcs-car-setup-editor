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

#include "cardata.h"

#define TAM 100

using namespace std;

CarData cardata;

float TCSE_version = 0.11;

float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;
int   main_window;


void savexml( int i ); // prototipo de la funcion
void importxml( int k);
void opencardata( int i );
void savecardata( int i );
void helpSelectList(int control);
void loadTrackList(void);
void updateTrackName (int i);
void loadDriversList(void);
void updateDriverName ( void );
void loadCarsList(void);
void updateCarName ( void );
void CalcCV (void);
void CalcTQ (void);
void CalcCVTQmax ( void );
void gearMaxMin( int i );
void CalcRPM ( int i );
float interpolationTQCV (float vectx[50], float vecty[50], float vectx0);
void helpSelectList(int control);
void loadEngine(int k);
void loadEngineList ( void );
void warningMsg(int warningn);
void fileBrowserWindow(int file);
int mainAC3Dloader( int ac3d ); //AC3Dloader window
void gearboxRatioSpeed ( int k );
void gearboxRatioCalcAll ( int k );
void numberOfGearsDisable   ( void );
void scaleAllEnginePoints ( int k );
void modifyShapeOfEnginePoints ( int k );
float CalcMaxEngine(float vector[TAM], float vector2[TAM],int init, int tamv, float &revs, int real);
int calcEnginePointsLesRL (void);
void setRevsLimiterLimits ( void );
void create3dFilesNames ( int k );
void activateDeativateGraphicRanges ( int k );
void gearsAutoinertiaEff ( int k );
void cmd_line_cb (int k);
float calcWheelRadius ( int k );
void optimalShiftPoint (int k);

float calcAcceleration ( void );
float calcSpeed ( void );
float calcGrip ( void );
float calcBraking ( void );

float tqmax;
float cvmax;
float cvmaxY;
float rpmattqmax;
float rpmatcvmax;
float rpmatcvmaxY;
float revsmax;

/* gear spinners*/
GLUI_Spinner *r_gear[8];
GLUI_Spinner *i_gear[8];
GLUI_Spinner *e_gear[8];
GLUI_Spinner *s_gear[8];

GLUI_Spinner *revsLimiterSpinner;

extern float gearbox_speed[8];
extern float gearRed100;
extern float bestOptShiftPoint[6];
extern float rpmAfterBestOptShiftPoint[6];

/** These are the live variables passed into GLUI ***/
int   wireframe = 0;
int   obj = 0;
int   segments = 8;
float tq12 = 100.0;

int fullRpmScale = 1;

int simetricaledityn1 = 0;
int simetricaledityn2 = 0;
int simetricaledityn3 = 0;

int currentEngine = 0;

int ac_acc_autogenerate = 0;

float gearsAutoInertia = 0.003;
float gearsAutoEff = 0.95;

float cvmaxNew = 800.0;
float tqmaxNew = 400.0;
float tqctNew = 400.0;
float cvctNew = 300.0;
extern float cvEngineShape[4];

const char *tracks[100] = {"t0","t1","t2","t3","t4","t5"};
int trackVal;
std::string trackname = "e-track-1";
std::string track1;
char track2[100];
char track3[100];
vector<string> tracksRoad;
vector<string> tracksOval;
vector<string> tracksDirt;
vector<string> tracksGprix;
vector<string> driversList;
int tracksRoadVal;
int tracksOvalVal;
int tracksDirtVal;
int tracksGprixVal;
int driverVal;
std::string driverName = "human" ;
std::string driverNameNumber = "1" ;
vector<string> carsList;
int carsListVal;

// Using a std::string as a live variable is safe.
//std::string  carname = "formula-torcs";

std::string file_name;
std::string text = "TORCS Car Setup Editor";
std::string text1 = "Program Started";
std::string text2=""; // the max value of the tq
std::string text3=""; // the max value of the cv
std::string text4=""; // used in gear ratio diff graph
std::string text5 = "Warning"; // used in glui9 for warnings

// Using a char buffer as a live var is also possible, but it is dangerous
// because GLUI doesn't know how big your buffer is.
// But still, it works as long as text doesn't happen to overflow.
//char  text[200] = {"Hello World!"};

GLUI_Checkbox   *checkbox;
GLUI_Spinner    *spinner;
GLUI_RadioGroup *radio;
GLUI_EditText   *edittext;
GLUI_List       *helpList;
GLUI_TextBox    *helpTextBox;
GLUI_TextBox    *warningTextBox;
GLUI_FileBrowser *fb;
GLUI_FileBrowser *fb2;
GLUI_Listbox *list_engine_capacity_units;
GLUI_Listbox *list_engine_shape;
GLUI_Listbox *list_engine_position;
GLUI_Listbox *list_drivetrain_type; // we need it extern to update when reading the value
GLUI_Listbox *list_frontdifferential_type;
GLUI_Listbox *list_reardifferential_type;
GLUI_Listbox *list_centraldifferential_type;
GLUI_CommandLine *cmd_line; /* command line */

GLUI_Listbox *loadEngine_List;

GLUI_EditText *carRangeEditText[5];
GLUI_Spinner *carRangeSpinner[5];
GLUI_Checkbox *carRangeCheckbox[5];

GLUI            *glui;
GLUI            *glui2;
GLUI            *glui3;
GLUI            *glui4; /* wheels and tires */
GLUI            *glui5; /* Diferential, Axles, Steer */
GLUI            *glui6; /* Gear Ratio Params */
GLUI            *glui7; /* Suspension */
GLUI            *glui8; /* Graphic and Sound */
GLUI            *glui9; /* Warning Message */
GLUI            *glui10; /* Save Car Setup Track */
GLUI            *glui11; /* Engine */
GLUI            *glui12; /* Car dimensions */
GLUI            *glui13; /* Car lights */
GLUI            *glui14; /* Car name, full name */
GLUI            *glui15; /* File Browser window */

/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb( int control )
{
  /********************************************************************
    Here we'll print the user id of the control that generated the
    callback, and we'll also explicitly get the values of each control.
    Note that we really didn't have to explicitly get the values, since
    they are already all contained within the live variables:
    'wireframe',  'segments',  'obj',  and 'text'
    ********************************************************************/

 // printf( "callback: %d\n", control );
 // printf( "             checkbox: %d\n", checkbox->get_int_val() );
  //printf( "              spinner: %d\n", spinner->get_int_val() );
  //printf( "          radio group: %d\n", radio->get_int_val() );
  //printf( "                 text: %s\n", edittext->get_text() );
   cout << "The new CarName is: " << cardata.carname << endl;
}

void saveAutorInfo(void)
{
    ofstream f;  //fichero de salid
    f.open("data/autorInfo.txt");  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "The file autorInfo.txt is not correct" << endl;
    else
    {
    f <<  cardata.autorName << endl;
    f <<  cardata.autorEmail << endl;
    }
    f.close();
}
void loadAutorInfo(void)
{
    ifstream f;  //fichero de salid
    f.open("data/autorInfo.txt");  //apertura del fichero o creacción si no existe
    if(!f)
        cout << "The file autorInfo.txt is not correct" << endl;
    else
    {
    getline(f, cardata.autorName);
    getline(f, cardata.autorEmail);
    }
    f.close();
}

void printFileName( void )
{
   file_name = fb->get_file();
   cout << "file_name"  << endl;
   cout << file_name  << endl;
   cout << file_name.c_str()  << endl;
   cout << fb->current_dir << endl;
}

void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  //if ( glutGetWindow() != main_window )
  //  glutSetWindow(main_window);


  //glutPostRedisplay();

  /****************************************************************/
  /*            This demonstrates GLUI::sync_live()               */
  /*   We change the value of a variable that is 'live' to some   */
  /*   control.  We then call sync_live, and the control          */
  /*   associated with that variable is automatically updated     */
  /*   with the new value.  This frees the programmer from having */
  /*   to always remember which variables are used by controls -  */
  /*   simply change whatever variables are necessary, then sync  */
  /*   the live ones all at once with a single call to sync_live  */
  /****************************************************************/

  GLUI_Master.sync_live_all();

}

/******************my functions*********************************/



/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
   /* // A few keys here to test the sync_live capability.
  case 'o':
    // Cycle through object types
    ++obj %= 3;
    GLUI_Master.sync_live_all();
    break;
  case 'w':
    // Toggle wireframe mode
    wireframe = !wireframe;
    GLUI_Master.sync_live_all();
    break;*/
  case 27:
  case 'q':
    exit(0);
    break;
  };
  glutPostRedisplay();
}



/***************************************** myGlutMenu() ***********/

void myGlutMenu( int value )
{
  myGlutKeyboard( value, 0, 0 );
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y )
{
  if ( button == GLUT_LEFT_BUTTON && button_state == GLUT_DOWN ) {
    last_x = x;
    last_y = y;
  }
}


/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y )
{
  rotationX += (float) (y - last_y);
  rotationY += (float) (x - last_x);

  last_x = x;
  last_y = y;

  glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
  xy_aspect = (float)x / (float)y;
  glViewport( 0, 0, x, y );

  glutPostRedisplay();
}

/*****************************Line Graph********************/






/**********************SimetricalEdit********************/
void SimetricalEditL (int val)
{
        int i;
        switch (val)
        {
        case 1:        /* Suspension */
            if (simetricaledityn1)
            {
            for (i=0; i < 8; i++)
            {
            cardata.suspension2[i] = cardata.suspension1[i];
            cardata.suspension4[i] = cardata.suspension3[i];
            }
            }
        break;

        case 2:        /* Wheels */
            if (simetricaledityn2)
            {
            cardata.wheel1[0] = -1 * cardata.wheel2[0];
            cardata.wheel3[0] = -1 * cardata.wheel4[0];
            for (i=1; i < 12; i++)
            {
            cardata.wheel1[i] = cardata.wheel2[i];
            cardata.wheel3[i] = cardata.wheel4[i];
            }
            cardata.wheel1[6] = -1 * cardata.wheel2[6];
            cardata.wheel3[6] = -1 * cardata.wheel4[6];
            }
        break;

        case 3:        /* Brakes */
            if (simetricaledityn3)
            {
            for (i=0; i < 4; i++)
            {
            cardata.brake1[i] = cardata.brake2[i];
            cardata.brake3[i] = cardata.brake4[i];
            }
            }
        break;
        }
        GLUI_Master.sync_live_all();
}

void SimetricalEditR (int val)
{
        int i;
        switch (val)
        {
        case 1:        /* Suspension */
            if (simetricaledityn1)
            {
            for (i=0; i < 8; i++)
            {
            cardata.suspension1[i] = cardata.suspension2[i];
            cardata.suspension3[i] = cardata.suspension4[i];
            }
            }
        break;

        case 2:        /* Wheels */
        if (simetricaledityn2)
            {
            cardata.wheel2[0] = -1 * cardata.wheel1[0];
            cardata.wheel4[0] = -1 * cardata.wheel3[0];
            for (i=1; i < 12; i++)
            {
            cardata.wheel2[i] = cardata.wheel1[i];
            cardata.wheel4[i] = cardata.wheel3[i];
            }
            cardata.wheel2[6] = -1 * cardata.wheel1[6];
            cardata.wheel4[6] = -1 * cardata.wheel3[6];
            }
        break;

        case 3:        /* Brakes */
            if (simetricaledityn3)
            {
            for (i=0; i < 4; i++)
            {
            cardata.brake2[i] = cardata.brake1[i];
            cardata.brake4[i] = cardata.brake3[i];
            }
            }
        break;
    }
    GLUI_Master.sync_live_all();
}


/**********************SimetricalEdit********************/
void syncLiveAll (void)
{
    GLUI_Master.sync_live_all();
}


/* draw text*/

void drawText ( std::string text, int k, float r, float g, float b, float x, float y )
{

    glDisable( GL_LIGHTING );  /* Disable lighting while we render text */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 100.0, 0.0, 100.0  );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glColor3f (r, g, b);
    glRasterPos2f( x, y );

    /*  printf( "text: %s\n", text );              */

    /*** Render the live character array 'text' ***/
switch (k)
{
case 12:
  for (unsigned int i=0; i<text.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, text[i] );
break;

case 10:
  for (unsigned int i=0; i<text.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, text[i] );
break;
}

  glEnable( GL_LIGHTING );
}

/* Engine Graph: torque and power vs rpm*/

void lineGraph (void)
{
  glDisable( GL_LIGHTING );  /* Disable lighting while we render text */
  //glEnable( GL_LIGHTING );  /* Enable lighting after we render text */

    int rpmscale;
    int k;
    //position of the graph (under-right vertex) 0-100
    int x0 = 6;
    int y0 = 72;
    std::string xaxis="rpm";
    std::string yaxis="Tq";
    std::string yraxis="CV";

    /* draw the graph*/

    float tqMaxGraphic = 100.0;

    if (tqmax < cardata.engine.tqValue[0]){
        tqMaxGraphic = 2*cardata.engine.tqValue[0];
    }
    else{
        tqMaxGraphic = 2*tqmax;
    }

    int kMax = 21;
    float cvMaxGraph = 200.0;
    if (fullRpmScale){
        kMax = 21;
        cvMaxGraph = cvmaxY;
    }
    else{
       float rpmatcvmax;
       int points = calcEnginePointsLesRL ();
       cvMaxGraph = CalcMaxEngine(cardata.engine.cvValue,cardata.engine.rpmValue,1,points,rpmatcvmax,1);
       int i;
       for (i=0;i<21;i++)
       {
           if (cardata.engine.rpmValue[i]>cardata.engine.params[1]){
               kMax = i;
               break;
           }
       }
    }

    /* SMOOTH THE LINE */
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

/* graph */
    glEnable (GL_LINE_STIPPLE);
    glLineStipple (3, 0xDDDD);

    glBegin (GL_LINE_STRIP); // tq
    glColor3f (0.0, 0.0, 1.0);
        for (k = 0; k < kMax; k++)
            //glVertex2f(x0 + k*4.5, y0 + cardata.engine.tqValue [k]*(99-y0)/(1.5*cvmax));
            glVertex2f(x0 + cardata.engine.rpmValue[k]/cardata.engine.rpmValue[kMax-1]*(100-2*x0), y0 + cardata.engine.tqValue [k]*(97-y0)/tqMaxGraphic);
    glEnd();
    glDisable (GL_LINE_STIPPLE);

    glBegin (GL_LINE_STRIP); // hp
    glColor3f (1.0, 0.0, 0.0);
        for (k = 0; k < kMax; k++)
            //glVertex2f(x0 + k*4.5, y0 + cardata.engine.cvValue[k]*(99-y0)/cvmax);
            glVertex2f(x0 + cardata.engine.rpmValue[k]/cardata.engine.rpmValue[kMax-1]*(100-2*x0), y0 + cardata.engine.cvValue[k]*(97-y0)/cvMaxGraph);
    glEnd();

    /* revs limiter */
    glEnable (GL_LINE_STIPPLE);
    glLineStipple (3, 0xAAAA);
    glBegin (GL_LINES);
            glColor3f (0.2, 0.4, 0.2);
            glVertex2f(x0 + cardata.engine.params[2]/cardata.engine.rpmValue[kMax-1]*(100-2*x0),y0);
            glVertex2f(x0 + cardata.engine.params[2]/cardata.engine.rpmValue[kMax-1]*(100-2*x0),(100-3));
    glEnd();
    glDisable (GL_LINE_STIPPLE);





    /*----------x axis ------------*/
    /* rpm*/
    drawText ( xaxis, 10, 0.0, 0.4, 0.0, 49, y0-2);

    /*----------y axis ------------*/
    /* tq */
    drawText ( yaxis, 10, 0.0, 0.0, 1.0, 0.1, 87);

    /* hp */
    drawText ( yraxis, 10, 1.0, 0.0, 0.0, 94.5, 87);

 /* write the cv max value on the screen*/
char cadena2[150];
char cadena3[150];
sprintf(cadena2, "%6.1f", cvmax);
sprintf(cadena3, "%6.1f", rpmatcvmax);
strcat(cadena2, "CV at ");
strcat(cadena2, cadena3);
strcat(cadena2, " rpm");
text3 = cadena2;

      drawText ( text3, 10, 1.0, 0.0, 0.0, 8, 95);

 /* write the tq max value on the screen*/
sprintf(cadena2, "%6.1f", tqmax);
sprintf(cadena3, "%6.1f", rpmattqmax);
strcat(cadena2, "N.m at ");
strcat(cadena2, cadena3);
strcat(cadena2, " rpm");
text2 = cadena2;

    drawText ( text2, 10, 0.0, 0.0, 1.0, 8, 92);

/* axes of the graph */
    glBegin (GL_LINE_STRIP);
            glColor3f (0.0, 0.0, 0.0);
            glVertex2f((100-x0),y0);
            glVertex2f(x0,y0);
            glVertex2f(x0,(100-3));
            glVertex2f((100-x0),(100-3));
            glVertex2f((100-x0),y0);
    glEnd();


/* rpm min (x axle) */
  std::string minRPM = "0";
  char bufferSTR[20];
  sprintf(bufferSTR, "%6.0f", cardata.engine.rpmValue[0]);
  minRPM = bufferSTR;

 drawText ( minRPM, 10, 0.0, 0.4, 0.0, 3, y0-2);

/* rpm max (x axle) */
  std::string maxRPM = "20000";
  bufferSTR[0] = '\0';
  if (fullRpmScale){
    sprintf(bufferSTR, "%6.0f", cardata.engine.rpmValue[20]);
  }
  else {
    sprintf(bufferSTR, "%6.0f", cardata.engine.params[1]);
  }

  maxRPM = bufferSTR;
drawText ( maxRPM, 10, 0.0, 0.4, 0.0, 93-x0, y0-2);

/* cv max y axle */
  std::string maxCV = "0";
  bufferSTR[0] = '\0';
  sprintf(bufferSTR, "%6.0f", cvMaxGraph);
  maxCV = bufferSTR;
 drawText ( maxCV, 10, 1.0, 0.0, 0.0, 90-x0, 98);

/* tq max y axle */
  std::string maxTQ = "0";
  bufferSTR[0] = '\0';
  sprintf(bufferSTR, "%6.0f", tqMaxGraphic);
  maxTQ = bufferSTR;
 drawText ( maxTQ, 10, 0.0, 0.0, 1.0, 0, 98);

    /* X graphic division */
glDisable( GL_LIGHTING );
    int i;
    for (i=1;i<(kMax-1);i++)
    {
    glBegin (GL_LINES);
            glColor3f (0.8, 0.8, 0.7);
            glVertex2f(x0 + i*1000/cardata.engine.rpmValue[kMax-1]*(100-2*x0),y0);
            glVertex2f(x0 + i*1000/cardata.engine.rpmValue[kMax-1]*(100-2*x0),(100-3));
    glEnd();
    }
glEnable( GL_LIGHTING );

    /* Y graphic division */
glDisable( GL_LIGHTING );
    for (i=1;i<11;i++)
    {
    glBegin (GL_LINES);
            glColor3f (0.8, 0.8, 0.7);
            glVertex2f(x0,y0+i*(97.0-y0)/10.0);
            glVertex2f(100.0-x0,y0+i*(97.0-y0)/10.0);
    glEnd();
    }
glEnable( GL_LIGHTING );


/* background of the graph */
 /*   glBegin (GL_QUADS);
            glColor3f (0.72, 0.75, 0.82);
            glVertex2i((100-x0),y0);
            glVertex2i(x0,y0);
            glVertex2i(x0,(100-1));
            glVertex2i((100-x0),(100-1));
    glEnd();*/



//}




/************************** accel-grip-speed-braking***********************/
  std::string agsb ="acceleration-grip-speed-braking";
  drawText ( agsb, 12, 1.0, 0.1, 0.1, 20.0, 67.5);

/* ...................Acceleration........................... */
float x0a = 15;
float y0a = 65.5;
float hagsb = 1.2;
float acceleration; /* min 0, max 100*/

/* calculation of the acceleration of the car from the car parameters */
acceleration = calcAcceleration();

/*  graph */
    glDisable( GL_LIGHTING );
    glBegin (GL_QUADS);
            glColor3f (0.2, 0.2, 0.90);
            glVertex2f(x0a+8.1+acceleration/100*(100-2*x0a-9.1),y0a+0.4);
            glVertex2f(x0a+8.1,y0a+0.35);
            glVertex2f(x0a+8.1,y0a + hagsb-0.35);
            glVertex2f(x0a+8.1+acceleration/100*(100-2*x0a-9.1),y0a + hagsb-0.4);
    glEnd();

  std::string atext ="Acceleration";
  drawText ( "Acceleration", 10, 0.2, 0.2, 0.9, 2, y0a+0.3);

glDisable( GL_LIGHTING );
/* background of the line graph */
    glBegin (GL_QUADS);
            glColor3f (0.72, 0.75, 0.90);
            glVertex2f((100-x0a),y0a);
            glVertex2f(x0a+7.1,y0a);
            glVertex2f(x0a+7.1,y0a + hagsb);
            glVertex2f((100-x0a),y0a + hagsb);
    glEnd();


/* ...................Speed........................... */
x0a = x0a;
y0a = y0a-1.6;
hagsb = 1.2;
float speed; /* min 0, max 100*/

/* calculation of the acceleration of the car from the car parameters */
speed = calcSpeed();

/*  graph */
    glBegin (GL_QUADS);
            glColor3f (0.2, 0.2, 0.90);
            glVertex2f(x0a+8.1+speed/100*(100-2*x0a-9.1),y0a+0.4);
            glVertex2f(x0a+8.1,y0a+0.35);
            glVertex2f(x0a+8.1,y0a + hagsb-0.35);
            glVertex2f(x0a+8.1+speed/100*(100-2*x0a-9.1),y0a + hagsb-0.4);
    glEnd();

  drawText ( "Speed", 10, 0.2, 0.2, 0.9, 2, y0a+0.3);
glDisable( GL_LIGHTING );
/* background of the line graph */
    glBegin (GL_QUADS);
            glColor3f (0.72, 0.75, 0.90);
            glVertex2f((100-x0a),y0a);
            glVertex2f(x0a+7.1,y0a);
            glVertex2f(x0a+7.1,y0a + hagsb);
            glVertex2f((100-x0a),y0a + hagsb);
    glEnd();

/* ...................grip........................... */
x0a = x0a;
y0a = y0a-1.6;
hagsb = 1.2;
float grip; /* min 0, max 100*/

/* calculation of the acceleration of the car from the car parameters */
grip = calcGrip();

/*  graph */
    glBegin (GL_QUADS);
            glColor3f (0.2, 0.2, 0.90);
            glVertex2f(x0a+8.1+grip/100*(100-2*x0a-9.1),y0a+0.4);
            glVertex2f(x0a+8.1,y0a+0.35);
            glVertex2f(x0a+8.1,y0a + hagsb-0.35);
            glVertex2f(x0a+8.1+grip/100*(100-2*x0a-9.1),y0a + hagsb-0.4);
    glEnd();

  drawText ( "Grip", 10, 0.2, 0.2, 0.9, 2, y0a+0.3);
glDisable( GL_LIGHTING );
/* background of the line graph */
    glBegin (GL_QUADS);
            glColor3f (0.72, 0.75, 0.90);
            glVertex2f((100-x0a),y0a);
            glVertex2f(x0a+7.1,y0a);
            glVertex2f(x0a+7.1,y0a + hagsb);
            glVertex2f((100-x0a),y0a + hagsb);
    glEnd();

/* ...................braking........................... */
x0a = x0a;
y0a = y0a-1.6;
hagsb = 1.2;
float braking; /* min 0, max 100*/

/* calculation of the acceleration of the car from the car parameters */
braking = calcBraking();

/*  graph */
    glBegin (GL_QUADS);
            glColor3f (0.2, 0.2, 0.90);
            glVertex2f(x0a+8.1+braking/100*(100-2*x0a-9.1),y0a+0.4);
            glVertex2f(x0a+8.1,y0a+0.35);
            glVertex2f(x0a+8.1,y0a + hagsb-0.35);
            glVertex2f(x0a+8.1+braking/100*(100-2*x0a-9.1),y0a + hagsb-0.4);
    glEnd();

  drawText ( "Braking", 10, 0.2, 0.2, 0.9, 2, y0a+0.3);
glDisable( GL_LIGHTING );

/* background of the line graph */
    glBegin (GL_QUADS);
            glColor3f (0.72, 0.75, 0.90);
            glVertex2f((100-x0a),y0a);
            glVertex2f(x0a+7.1,y0a);
            glVertex2f(x0a+7.1,y0a + hagsb);
            glVertex2f((100-x0a),y0a + hagsb);
    glEnd();


/* GEAR RATIO GRAPH*/

    x0 = 5;
    y0 = 49;
    glColor3f (0.75, 0.0, 0.0);
    atext ="0123456789";

        for (k = 1; k < (cardata.numberOfGears+1); k++)
        {
            glRectf(x0 + (k-1)*4.5, y0,1.5+ x0 + (k-1)*4.5, y0 + cardata.gearboxratio[k]*2);
            glRasterPos2f (x0 + (k-1)*4.5, y0-1.3);
            glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, atext[k] );
        }

  atext ="Gear Ratio";
  glRasterPos2f (x0+5, y0+9);
  for (unsigned int i=0; i<atext.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, atext[i] );

  atext ="Gear Ratio increment";
  glRasterPos2f (x0+50, y0+9);
  for (unsigned int i=0; i<atext.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, atext[i] );

char cadena4[150];
char cadena5[150];

        for (k = 2; k < (cardata.numberOfGears+1) ; k++)
        {
           sprintf(cadena5, "%2.3f", cardata.gearboxratio[k]-cardata.gearboxratio[k-1]);
           sprintf(cadena4, "%i-%i", k-1, k);
           strcat(cadena4, ": ");
           strcat(cadena4, cadena5);
           text4 = cadena4;

            glRasterPos2f (x0 + 50, y0 + 10.5-1.6*k);
            for (unsigned int i=0; i<text4.length(); ++i)
                  glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, text4[i] );
        }

/* background of the gear ratio */
    glBegin (GL_LINE_STRIP);
            glColor3f (0.0, 0.0, 0.2);
            glVertex2f((100-x0+2),y0-15.8);
            glVertex2f(x0-2,y0-15.8);
            glVertex2f(x0-2,y0+11.2);
            glVertex2f((100-x0+2),y0+11.2);
            glVertex2f((100-x0+2),y0-15.8);
    glEnd();
   /* glBegin (GL_QUADS);
            glColor3f (0.65, 0.70, 0.89);
            glVertex2f((100-x0+2),y0-1.5);
            glVertex2f(x0-2,y0-1.5);
            glVertex2f(x0-2,y0+11.2);
            glVertex2f((100-x0+2),y0+11.2);
    glEnd();*/

/* gear ratio calculations */
    glBegin (GL_QUADS);
            glColor3f (0.05, 0.00, 0.89);
    glEnd();
  atext ="Gear";
  glRasterPos2f (x0 + 4.5, y0 -3.7);
  for (unsigned int i=0; i<atext.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, atext[i] );
  atext ="Rel";
  glRasterPos2f (x0 + 4.5 + 8 , y0 -3.7);
  for (unsigned int i=0; i<atext.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, atext[i] );
    atext ="Vmax";
  glRasterPos2f (x0 + 4.5 + 20, y0 -3.7);
  for (unsigned int i=0; i<atext.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, atext[i] );
      atext ="revs";
  glRasterPos2f (x0 + 4.5 + 32.5, y0 -3.7);
  for (unsigned int i=0; i<atext.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, atext[i] );
      atext ="tq";
  glRasterPos2f (x0 + 4.5 + 49, y0 -3.7);
  for (unsigned int i=0; i<atext.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, atext[i] );
      atext ="cv";
  glRasterPos2f (x0 + 4.5 +62, y0 -3.7);
  for (unsigned int i=0; i<atext.length(); ++i)
    glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, atext[i] );
    glBegin (GL_QUADS);
            glColor3f (0.05, 0.10, 0.60);
    glEnd();

  for (k = 1; k < (cardata.numberOfGears+1); k++)
        {
           strcat(cadena4, "Gear");
           sprintf(cadena4, "%i", k);
           text4 = cadena4;
            glRasterPos2f (x0 + 5, y0 -4 -1.6*k);
            for (unsigned int i=0; i<text4.length(); ++i)
                  glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, text4[i] );
        }

  for (k = 1; k < (cardata.numberOfGears+1); k++)
        {
           sprintf(cadena4, "%2.3f", cardata.gearboxratio[k]);
           text4 = cadena4;
            glRasterPos2f (x0 + 5 + 7, y0 -4 -1.6*k);
            for (unsigned int i=0; i<text4.length(); ++i)
                  glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, text4[i] );
        }
  float wRadius =  calcWheelRadius ( 1 );

  float finalRatio = 1.0;

  for (k = 1; k < (cardata.numberOfGears+1); k++)
        {
           if (cardata.curr_drivetrain_type == 0)
           {
           finalRatio = cardata.reardifferential[1]*cardata.gearboxratio[k];
           }
           if (cardata.curr_drivetrain_type == 1)
           {
           finalRatio = cardata.frontdifferential[1]*cardata.gearboxratio[k];
           }
           if (cardata.curr_drivetrain_type == 2)
           {
           finalRatio = cardata.centraldifferential[1]*cardata.gearboxratio[k];
           }
           if ( finalRatio <= 0.0 ) finalRatio = 1.0;
           sprintf(cadena4, "%2.1f", cardata.engine.params[2]*60*wRadius*2.0*3.1416*0.001/(finalRatio));
           text4 = cadena4;
            glRasterPos2f (x0 + 5 + 20, y0 -4 -1.6*k);
            for (unsigned int i=0; i<text4.length(); ++i)
                  glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, text4[i] );
        }
  for (k = 2; k < (cardata.numberOfGears+1); k++)
        {
           sprintf(cadena4, "%2.1f", cardata.engine.params[2]*cardata.gearboxratio[k]/cardata.gearboxratio[k-1]);
           text4 = cadena4;
            glRasterPos2f (x0 + 5 + 32.5, y0 -4 -1.6*k);
            for (unsigned int i=0; i<text4.length(); ++i)
                  glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, text4[i] );
        }
  for (k = 2; k < (cardata.numberOfGears+1); k++)
        {
           sprintf(cadena4, "%2.1f", interpolationTQCV(cardata.engine.rpmValue, cardata.engine.tqValue, cardata.engine.params[2]*cardata.gearboxratio[k]/cardata.gearboxratio[k-1]));
           text4 = cadena4;
            glRasterPos2f (x0 + 5 + 49, y0 -4 -1.6*k);
            for (unsigned int i=0; i<text4.length(); ++i)
                  glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, text4[i] );
        }
  for (k = 2; k < (cardata.numberOfGears+1); k++)
        {
           sprintf(cadena4, "%2.1f", interpolationTQCV(cardata.engine.rpmValue, cardata.engine.cvValue, cardata.engine.params[2]*cardata.gearboxratio[k]/cardata.gearboxratio[k-1]));
           text4 = cadena4;
            glRasterPos2f (x0 + 5 + 62, y0 -4 -1.6*k);
            for (unsigned int i=0; i<text4.length(); ++i)
                  glutBitmapCharacter( GLUT_BITMAP_HELVETICA_10, text4[i] );
        }

}// end of the graphic function

/***************************************** show-hide-glui-window() *****************/
 void showglui (int gluiname)
 {
  switch (gluiname)
  {
   case 2:
      glui2->show();break;
   case 3:
      glui3->show();break;
   case 4:
      glui4->show();break;
   case 5:
      glui5->show();break;
   case 6:
      glui6->show();break;
   case 7:
      glui7->show();break;
   case 8:
      glui8->show();break;
   case 9:
      glui9->show();break;
   case 10:
      glui10->show();break;
   case 11:
      glui11->show();break;
   case 12:
      glui12->show();break;
   case 13:
      glui13->show();break;
   case 14:
      glui14->show();break;
   case 15:
      glui15->show();break;
   }
 }

 void hideglui (int gluiname)
 {
  switch (gluiname)
  {
   case 2:
      glui2->hide();break;
   case 3:
      glui3->hide();break;
   case 4:
      glui4->hide();break;
   case 5:
      glui5->hide();break;
   case 6:
      glui6->hide();break;
   case 7:
      glui7->hide();break;
   case 8:
      glui8->hide();break;
   case 9:
      glui9->hide();break;
   case 10:
      glui10->hide();break;
   case 11:
      glui11->hide();break;
   case 12:
      glui12->hide();break;
   case 13:
      glui13->hide();break;
   case 14:
      glui14->hide();break;
   case 15:
      glui15->hide();break;
    }
 }

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void )
{

  glClearColor( .9f, .9f, .9f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

/*
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  glFrustum( -xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0 );

  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  glTranslatef( 0.0f, 0.0f, -1.6f );
  glRotatef( rotationY, 0.0, 1.0, 0.0 );
  glRotatef( rotationX, 1.0, 0.0, 0.0 );
  */




  /*** Now we render object, using the variables 'obj', 'segments', and
    'wireframe'.  These are _live_ variables, which are transparently
    updated by GLUI ***/
  /*
  if ( obj == 0 ) {
    if ( wireframe )
      glutWireSphere( .6, segments, segments );
    else
      glutSolidSphere( .6, segments, segments );
  }
  else if ( obj == 1 ) {
    if ( wireframe )
      glutWireTorus( .2,.5,16,segments );
    else
      glutSolidTorus( .2,.5,16,segments );
  }
  else if ( obj == 2 ) {
    if ( wireframe )
      glutWireTeapot( .5 );
    else
      glutSolidTeapot( .5 );
  }
*/



  /* TEXT INFO ON GLUT SCREEN */
  /*draw the tcse version*/
  char bufferStr[30];
  sprintf(bufferStr, "%3.2f", TCSE_version);
  text = "Torcs Car Setup Editor";
  text += " v";
  text += bufferStr;
  drawText ( text, 12, 0.8, 0.1, 0.1, 1.0, 31.0);

  /* draw the info*/
  drawText ( text1, 10, 0.0, 0.0, 0.4, 1.0, 29.0);

   /* Draw all the glut window */
  lineGraph ();

  glutSwapBuffers();
}


/********************************************************************/


/**************************************** main() ********************/

int main(int argc, char* argv[])
{
int iargc = 1;

while (iargc < argc)
{

	if (strncmp(argv[iargc], "-c", 2) == 0)
    {
       cardata.carname="";
       cardata.carname= argv[iargc+1];
       GLUI_Master.sync_live_all();
   }
iargc++;
}

/* Time */
time_t tAct = time(NULL);
cout << "Program Started at: " << asctime(localtime(&tAct));

/* calculate the cv for the graph */
CalcCV();

/* calculate the wheel radius for each gear */
float wRadius =  calcWheelRadius ( 1 );

/* calculate the max speed for each gear */
gearboxRatioSpeed(1);

/* asign osp at cvmax */
int i;
for(i=0;i<6;i++){
    bestOptShiftPoint[i]= rpmatcvmax;
    rpmAfterBestOptShiftPoint[i]=bestOptShiftPoint[i]*cardata.gearboxratio[i+2]/cardata.gearboxratio[i+1];
}
/* load autor info */
loadAutorInfo();

  /****************************************/
  /*   Initialize GLUT and create window  */
  /****************************************/
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowPosition( 724, 0 );
  glutInitWindowSize( 300, 700 );

  main_window = glutCreateWindow( "Torcs Car Setup Editor" );
  glutDisplayFunc( myGlutDisplay );
  glutReshapeFunc( myGlutReshape );
  glutKeyboardFunc( myGlutKeyboard );
  glutMotionFunc( myGlutMotion );
  glutMouseFunc( myGlutMouse );

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/

  GLfloat light0_ambient[] =  {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat light0_diffuse[] =  {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat light0_position[] = {1.0f, 1.0f, 1.0f, 0.0f};

  glEnable(GL_LIGHTING);
  //glEnable(GL_LIGHT0);
  //glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  //glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  //glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  /****************************************/
  /*          Enable z-buferring          */
  /****************************************/

  glEnable(GL_DEPTH_TEST);


  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/

  printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );
  printf( "Torcs Car Setup Editor version %3.2f\n", TCSE_version);
  printf( "created by Vicente Marti\n");
  printf( "You can at start the progarm -c carname to set the carname.\n");



  GLUI *glui = GLUI_Master.create_glui( "Torcs Car Setup Editor", 0, 0, 0 ); /* name, flags,
								 x, and y */

    (new GLUI_EditText( glui, "CarName", cardata.carname, 0, (GLUI_Update_CB)syncLiveAll ))->w=170;
    (new GLUI_EditText( glui, "Category", cardata.carCategory, 0, (GLUI_Update_CB)syncLiveAll ))->w=170;
    new GLUI_Button( glui, "Help", 2, (GLUI_Update_CB)showglui );
    cmd_line=new GLUI_CommandLine(glui,"C.Line:",NULL,0,(GLUI_Update_CB)cmd_line_cb);
    cmd_line->set_w( 170 );  /** Widen 'command line' control **/


  GLUI_Panel *aerodynamics_panel = new GLUI_Panel( glui, "Aerodynamics" );
   (new GLUI_Spinner( aerodynamics_panel, "Cx:", &cardata.aerodynamics[0] ))
    ->set_float_limits( 0, 1 );
    (new GLUI_Spinner( aerodynamics_panel, "Front Area(m2):", &cardata.aerodynamics[1] ))
    ->set_float_limits( 0, 20 );
    (new GLUI_Spinner( aerodynamics_panel, "Front Clift:", &cardata.aerodynamics[2] ))
    ->set_float_limits( 0, 5 );
    (new GLUI_Spinner( aerodynamics_panel, "Rear Clift:", &cardata.aerodynamics[3] ))
    ->set_float_limits( 0, 5 );

  GLUI_Panel *frontwing_panel = new GLUI_Panel( glui, "Front Wing" );
   (new GLUI_Spinner( frontwing_panel, "Area(m2):", &cardata.frontwing[0] ))
    ->set_float_limits( 0, 10 );
    (new GLUI_Spinner( frontwing_panel, "angle(deg):", &cardata.frontwing[1] ))
    ->set_float_limits( 0, 180 );
    (new GLUI_Spinner( frontwing_panel, "xpos(m):", &cardata.frontwing[2] ))
    ->set_float_limits( -5, 5 );
    (new GLUI_Spinner( frontwing_panel, "zpos(m):", &cardata.frontwing[3] ))
    ->set_float_limits( 0, 5 );

  GLUI_Panel *rearwing_panel = new GLUI_Panel( glui, "Rear Wing" );
   (new GLUI_Spinner( rearwing_panel, "Area(m2):", &cardata.rearwing[0] ))
    ->set_float_limits( 0, 10 );
    (new GLUI_Spinner( rearwing_panel, "angle(deg):", &cardata.rearwing[1] ))
    ->set_float_limits( 0, 180 );
    (new GLUI_Spinner( rearwing_panel, "xpos(m):", &cardata.rearwing[2] ))
    ->set_float_limits( -5, 5 );
    (new GLUI_Spinner( rearwing_panel, "zpos(m):", &cardata.rearwing[3] ))
    ->set_float_limits( 0, 5 );

  GLUI_Panel *massdata_panel = new GLUI_Panel( glui, "Weight Repartition & Fuel" );
   (new GLUI_Spinner( massdata_panel, "front-rear:", &cardata.massdata[0] ))
    ->set_float_limits( 0.1, 0.9 );
   (new GLUI_Spinner( massdata_panel, "front right-left:", &cardata.massdata[1] ))
    ->set_float_limits( 0.4, 0.6 );
   (new GLUI_Spinner( massdata_panel, "rear right-left:", &cardata.massdata[2] ))
    ->set_float_limits( 0.4, 0.6 );
   (new GLUI_Spinner( massdata_panel, "mass.rep.coef:", &cardata.massdata[3] ))
    ->set_float_limits( 0.4, 1.0 );
   (new GLUI_Spinner( massdata_panel, "fuel tank(l):", &cardata.massdata[4] ))
    ->set_float_limits( 0.0, 100.0 );
   (new GLUI_Spinner( massdata_panel, "initial fuel(l):", &cardata.massdata[5] ))
    ->set_float_limits( 1.0, 100.0 );
   (new GLUI_Spinner( massdata_panel, "Mass (kg):", &cardata.massdata[6] ))
    ->set_float_limits( 1.0, 5000.0 );
   (new GLUI_Spinner( massdata_panel, "GC height(m):", &cardata.massdata[7] ))
    ->set_float_limits( 0.0, 100.0 );




  new GLUI_Column( glui , false);
   /* Engine parameters */
   GLUI_Panel *engineparams_panel = new GLUI_Panel( glui, "Engine configuartion" );
   (new GLUI_Spinner( engineparams_panel, "inertia(kg.m2):", &cardata.engine.params[0] ))
    ->set_float_limits( 0.0, 0.5 );
   (new GLUI_Spinner( engineparams_panel, "revs maxi:", &cardata.engine.params[1],0,(GLUI_Update_CB)setRevsLimiterLimits ))
    ->set_float_limits( 5000, 20000 );

   revsLimiterSpinner = new GLUI_Spinner( engineparams_panel, "revs limiter:", &cardata.engine.params[2], 0, (GLUI_Update_CB)CalcCVTQmax );
    revsLimiterSpinner -> set_float_limits( cardata.engine.params[3], cardata.engine.params[1] );

   (new GLUI_Spinner( engineparams_panel, "tickover:", &cardata.engine.params[3],0,(GLUI_Update_CB)setRevsLimiterLimits ))
    ->set_float_limits( 500, 20000 );
   (new GLUI_Spinner( engineparams_panel, "fuel cons factor:", &cardata.engine.params[4] ))
    ->set_float_limits( 1.1, 1.3 );

   new GLUI_Button( engineparams_panel, "Engine Advanced", 11, (GLUI_Update_CB)showglui );


  //new GLUI_Column( glui );

   /* Gearbox */

    GLUI_Panel *gearboxratio_panel = new GLUI_Panel( glui, "Gearbox Ratio" );
   r_gear[0] = new GLUI_Spinner( gearboxratio_panel, "rear", &cardata.gearboxratio[0], 0, (GLUI_Update_CB)gearMaxMin );
   r_gear[0]->set_float_limits( -5.0, -0.001 );
   r_gear[1] = new GLUI_Spinner( gearboxratio_panel, "1st", &cardata.gearboxratio[1], 1, (GLUI_Update_CB)gearMaxMin  );
   r_gear[1]->set_float_limits( 0.001, 5.0 );
   r_gear[2] = new GLUI_Spinner( gearboxratio_panel, "2nd", &cardata.gearboxratio[2], 2, (GLUI_Update_CB)gearMaxMin  );
   r_gear[2]->set_float_limits( 0.001, 5.0 );
   r_gear[3] = new GLUI_Spinner( gearboxratio_panel, "3rd", &cardata.gearboxratio[3], 3, (GLUI_Update_CB)gearMaxMin  );
   r_gear[3]->set_float_limits( 0.001, 5.0 );
   r_gear[4] = new GLUI_Spinner( gearboxratio_panel, "4th", &cardata.gearboxratio[4], 4, (GLUI_Update_CB)gearMaxMin  );
   r_gear[4]->set_float_limits( 0.001, 5.0 );
   r_gear[5] = new GLUI_Spinner( gearboxratio_panel, "5th", &cardata.gearboxratio[5], 5, (GLUI_Update_CB)gearMaxMin  );
   r_gear[5]->set_float_limits( 0.001, 5.0 );
   r_gear[6] = new GLUI_Spinner( gearboxratio_panel, "6th", &cardata.gearboxratio[6], 6, (GLUI_Update_CB)gearMaxMin  );
   r_gear[6]->set_float_limits( 0.001, 5.0 );
   r_gear[7] = new GLUI_Spinner( gearboxratio_panel, "7th", &cardata.gearboxratio[7], 7, (GLUI_Update_CB)gearMaxMin  );
   r_gear[7]->set_float_limits( 0.001, 5.0 );

    new GLUI_Separator( gearboxratio_panel );

   (new GLUI_Spinner( gearboxratio_panel, "Shift Time(s)", &cardata.gearbox_shift_time))
    ->set_float_limits( 0.0, 5.0 );

  new GLUI_Button( gearboxratio_panel, "Gear Advanced", 6, (GLUI_Update_CB)showglui );



    GLUI_Panel *brakesystem_panel = new GLUI_Panel( glui, "Brake System" );
   (new GLUI_Spinner( brakesystem_panel, "front-rear", &cardata.brakesystem[0], 0, (GLUI_Update_CB)syncLiveAll ))
    ->set_float_limits( 0.3, 0.7 );

    (new GLUI_Scrollbar( brakesystem_panel, "Red",
    GLUI_SCROLL_HORIZONTAL,&cardata.brakesystem[0], 0, (GLUI_Update_CB)syncLiveAll))->set_float_limits(0.3,0.7);

     new GLUI_Separator( brakesystem_panel );

   GLUI_Spinner *MaxPress=new GLUI_Spinner( brakesystem_panel, "MaxP(kPa)", &cardata.brakesystem[1] );
   MaxPress -> set_float_limits( 100, 150000 );
   MaxPress -> w=350;

  new GLUI_Button( glui, "Disk Brakes", 3, (GLUI_Update_CB)showglui );

  new GLUI_Button( glui, "Wheels & Tires", 4, (GLUI_Update_CB)showglui );

  new GLUI_Button( glui, "Steer, Diferential...", 5, (GLUI_Update_CB)showglui );

  new GLUI_Button( glui, "Car dimensions&Driver pos", 12, (GLUI_Update_CB)showglui );

  new GLUI_Button( glui, "CarName&AutorInfo", 14, (GLUI_Update_CB)showglui );

  new GLUI_Button( glui, "Sound & Graphic", 8, (GLUI_Update_CB)showglui );

  new GLUI_Column( glui , false);



  new GLUI_StaticText( glui, "Suspension" );

 /* Suspension parameters */

    GLUI_Panel *antirollbar = new GLUI_Panel( glui, "Anti-Roll Bar" );
    GLUI_Spinner *fantirollbar = new GLUI_Spinner( antirollbar, "Front(lbs/in)", &cardata.antirollbar1[0] );
    fantirollbar -> set_int_limits( 0, 10000 );
    fantirollbar -> user_speed = 0.05;
    fantirollbar -> increase_growth();
    GLUI_Spinner *rantirollbar = new GLUI_Spinner( antirollbar, "Rear(lbs/in)", &cardata.antirollbar2[0] );
    rantirollbar -> set_int_limits( 0, 10000 );
    rantirollbar -> set_speed( 0.05 );
    rantirollbar -> reset_growth();

  new GLUI_Button( glui, "Advanced Suspension", 7, (GLUI_Update_CB)showglui );

 checkbox =
    new GLUI_Checkbox(glui, "Simetrical edit", &simetricaledityn1, 1, (GLUI_Update_CB)SimetricalEditL );


  GLUI_Panel *suspensionlf = new GLUI_Panel(glui, "Left Front Wheel");
   (new GLUI_Spinner( suspensionlf, "Spring(lbs/in)", &cardata.suspension1[0], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 10000 );
   (new GLUI_Spinner( suspensionlf, "Susp.Course(m)", &cardata.suspension1[1], 1, (GLUI_Update_CB)SimetricalEditL ))
   ->set_float_limits( 0, 0.2 );

  GLUI_Panel *suspensionrf = new GLUI_Panel(glui, "Right Front Wheel");
   (new GLUI_Spinner( suspensionrf, "Spring(lbs/in)", &cardata.suspension2[0], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 10000 );
   (new GLUI_Spinner( suspensionrf, "Susp.Course(m)", &cardata.suspension2[1], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 0.2 );

  GLUI_Panel *suspensionlr = new GLUI_Panel(glui, "Left Rear Wheel");
   (new GLUI_Spinner( suspensionlr, "Spring(lbs/in)", &cardata.suspension3[0], 1, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 10000 );
   (new GLUI_Spinner( suspensionlr, "Susp.Course(m)", &cardata.suspension3[1], 1, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 0.2 );

  GLUI_Panel *suspensionrr = new GLUI_Panel(glui, "Right Rear Wheel");
   (new GLUI_Spinner( suspensionrr, "Spring(lbs/in)", &cardata.suspension4[0], 1, (GLUI_Update_CB)SimetricalEditR  ))
    ->set_float_limits( 0, 10000 );
   (new GLUI_Spinner( suspensionrr, "Susp.Course(m)", &cardata.suspension4[1], 1, (GLUI_Update_CB)SimetricalEditR  ))
    ->set_float_limits( 0, 0.2 );



  new GLUI_Button( glui, "Open CarData", 1, (GLUI_Update_CB)opencardata ); /* button to
                              save the xml file calling the function open car data */
  new GLUI_Button( glui, "Save CarData", 1, (GLUI_Update_CB)savecardata ); /* button to
                              save the xml file calling the function save car data */
  new GLUI_Button( glui, "Import XML", 0, (GLUI_Update_CB)importxml ); /* button to
                              import the xml file calling the function import xml */
  new GLUI_Button( glui, "Save XML", 1, (GLUI_Update_CB)savexml ); /* button to
                              save the xml file calling the function savexml */
  new GLUI_StaticText( glui, "" );

  new GLUI_Button( glui, "Advanced Save-Open", 10, (GLUI_Update_CB)showglui ); /* button to
                              save the xml file calling the function save car data */
  new GLUI_StaticText( glui, "" );
  new GLUI_Button( glui, "View AC3D file", 0,(GLUI_Update_CB)mainAC3Dloader );
  new GLUI_StaticText( glui, "" );
  new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );/* EXIT */

 /*examples */
  //checkbox = new GLUI_Checkbox( glui, "Wireframe", &wireframe, 1, control_cb );
 /* spinner  = new GLUI_Spinner( glui, "Segments:", &segments, 2, control_cb );
  spinner->set_int_limits( 3, 60 );
  edittext = new GLUI_EditText( glui, "Text:", text, 3, control_cb );



  GLUI_Panel *obj_panel = new GLUI_Panel( glui, "Object Type" );
  radio = new GLUI_RadioGroup( obj_panel,&obj,4,control_cb );
  new GLUI_RadioButton( radio, "Sphere" );
  new GLUI_RadioButton( radio, "Torus" );
  new GLUI_RadioButton( radio, "Teapot" );
  */
  /* Roll out */
  /*GLUI_Panel *obj_panel2 = new GLUI_Rollout(glui, "Properties", false );

   (new GLUI_Spinner( obj_panel2, "Tq 0.00rpm:", &cardata.engine.tqValue[0] ))
    ->set_int_limits( 0, 1000 );
    (new GLUI_Spinner( obj_panel2, "Tq 0.00rpm:", &cardata.engine.tqValue[0] ))
    ->set_int_limits( 0, 1000 );
*/
  /* buttons */
 /* GLUI_Panel *obj_panel3 = new GLUI_Panel( glui, "Object Type" );
  radio = new GLUI_RadioGroup( obj_panel3,&obj,4,control_cb );
  new GLUI_RadioButton( radio, "Sphere" );
  new GLUI_RadioButton( radio, "Torus" );
  new GLUI_RadioButton( radio, "Teapot" );
  */
  /* chec box */
/*  GLUI_Panel *obj_panel4 = new GLUI_Panel( glui, "Object Type" );
  radio = new GLUI_RadioGroup( obj_panel3,&obj,4,control_cb );
  checkbox =
    new GLUI_Checkbox(obj_panel4, "Wireframe", &wireframe, 1, control_cb );
    */

    /* scroll bar */
    /*
      GLUI_Panel *obj_panel5 = new GLUI_Panel( glui, "Object Type" );
    GLUI_Scrollbar *sb;
  sb = new GLUI_Scrollbar( obj_panel5, "Red",GLUI_SCROLL_HORIZONTAL,
                           &wireframe,1,control_cb);

  sb->set_float_limits(0,1);

*/

     /* Window for the Brakes */

  glui3 = GLUI_Master.create_glui("Disk Brakes Setup", 0, 50, 0);/* name, flags,
								 x, and y */
  glui3->hide();


    /* Front Left Brake */
    GLUI_Panel *brake2_panel = new GLUI_Panel( glui3, "Front Left Brake" );
   (new GLUI_Spinner( brake2_panel, "Disk Diameter(mm)", &cardata.brake2[0], 3, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 100, 380 );
   (new GLUI_Spinner( brake2_panel, "Piston Area(cm2)", &cardata.brake2[1], 3, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 10, 100 );
   (new GLUI_Spinner( brake2_panel, "mu", &cardata.brake2[2], 3, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( brake2_panel, "Inertia(Kg.m2)", &cardata.brake2[3], 3, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 5 );

    /* Rear Left Brake */
    GLUI_Panel *brake4_panel = new GLUI_Panel( glui3, "Rear Left Brake" );
   (new GLUI_Spinner( brake4_panel, "Disk Diameter(mm)", &cardata.brake4[0], 3, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 100, 380 );
   (new GLUI_Spinner( brake4_panel, "Piston Area(cm2)", &cardata.brake4[1], 3, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 10, 100 );
   (new GLUI_Spinner( brake4_panel, "mu", &cardata.brake4[2], 3, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( brake4_panel, "Inertia(Kg.m2)", &cardata.brake4[3], 3, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 5 );

  new GLUI_Checkbox(glui3, "Simetrical edit", &simetricaledityn3, 3, (GLUI_Update_CB)SimetricalEditL );
  new GLUI_Column( glui3, false );


   /* Front Right Brake */
   GLUI_Panel *brake1_panel = new GLUI_Panel( glui3, "Front Right Brake" );
   (new GLUI_Spinner( brake1_panel, "Disk Diameter(mm)", &cardata.brake1[0], 3, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 100, 380 );
   (new GLUI_Spinner( brake1_panel, "Piston Area(cm2)", &cardata.brake1[1], 3, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 10, 100 );
   (new GLUI_Spinner( brake1_panel, "mu", &cardata.brake1[2], 3, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( brake1_panel, "Inertia(Kg.m2)", &cardata.brake1[3], 3, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );

   /* Rear Right Brake */
   GLUI_Panel *brake3_panel = new GLUI_Panel( glui3, "Rear Right Brake" );
   (new GLUI_Spinner( brake3_panel, "Disk Diameter(mm)", &cardata.brake3[0], 3, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 100, 380 );
   (new GLUI_Spinner( brake3_panel, "Piston Area(cm2)", &cardata.brake3[1], 3, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 10, 100 );
   (new GLUI_Spinner( brake3_panel, "mu", &cardata.brake3[2], 3, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( brake3_panel, "Inertia(Kg.m2)", &cardata.brake3[3], 3, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );


    new GLUI_Button( glui3, "Close Window", 3, (GLUI_Update_CB)hideglui );


/* Window for Wheels and Tires*/
  glui4 = GLUI_Master.create_glui("Wheels & Tires", 0, 0, 0);/* name, flags,
								 x, and y */
  glui4->hide();

  GLUI_Panel *wheel2_panel = new GLUI_Panel(glui4, "Front Left Wheel");
   (new GLUI_Spinner( wheel2_panel, "Ypos(m)", &cardata.wheel2[0], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel2_panel, "Rim diameter(in)", &cardata.wheel2[1], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 30 );
   (new GLUI_Spinner( wheel2_panel, "Tire Width(mm)", &cardata.wheel2[2], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( wheel2_panel, "Tire height-width ratio", &cardata.wheel2[3], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 5 );
    (new GLUI_Spinner( wheel2_panel, "Wheel radius(m)", &cardata.wheelRadius[1] ))
    ->disable( );
   (new GLUI_Spinner( wheel2_panel, "Inertia(kg.m2)", &cardata.wheel2[4], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel2_panel, "Initial Ride Height(mm)", &cardata.wheel2[5], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 300 );
   (new GLUI_Spinner( wheel2_panel, "Toe(deg)", &cardata.wheel2[6], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel2_panel, "Camber(deg)", &cardata.wheel2[7], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( -5, 0 );
   (new GLUI_Spinner( wheel2_panel, "Stiffness", &cardata.wheel2[8], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 20, 50 );
   (new GLUI_Spinner( wheel2_panel, "Dynamic Fricction(%)", &cardata.wheel2[9], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 70, 90 );
   (new GLUI_Spinner( wheel2_panel, "Rolling Resistance", &cardata.wheel2[10], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( wheel2_panel, "Mu", &cardata.wheel2[11], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0.05, 1.8 );



  GLUI_Panel *wheel4_panel = new GLUI_Panel(glui4, "Rear Left Wheel");
   (new GLUI_Spinner( wheel4_panel, "Ypos(m)", &cardata.wheel4[0], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel4_panel, "Rim diameter(in)", &cardata.wheel4[1], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 30 );
   (new GLUI_Spinner( wheel4_panel, "Tire Width(mm)", &cardata.wheel4[2], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( wheel4_panel, "Tire height-width ratio", &cardata.wheel4[3], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 5 );
    (new GLUI_Spinner( wheel4_panel, "Wheel radius(m)", &cardata.wheelRadius[3] ))
    ->disable( );
   (new GLUI_Spinner( wheel4_panel, "Inertia(kg.m2)", &cardata.wheel4[4], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel4_panel, "Initial Ride Height(mm)", &cardata.wheel4[5], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 300 );
   (new GLUI_Spinner( wheel4_panel, "Toe(deg)", &cardata.wheel4[6], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel4_panel, "Camber(deg)", &cardata.wheel4[7], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( -5, 0 );
   (new GLUI_Spinner( wheel4_panel, "Stiffness", &cardata.wheel4[8], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 20, 50 );
   (new GLUI_Spinner( wheel4_panel, "Dynamic Fricction(%)", &cardata.wheel4[9], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 70, 90 );
   (new GLUI_Spinner( wheel4_panel, "Rolling Resistance", &cardata.wheel4[10], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( wheel4_panel, "Mu", &cardata.wheel4[11], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0.05, 1.8 );

  new GLUI_Checkbox(glui4, "Simetrical edit", &simetricaledityn2, 2, (GLUI_Update_CB)SimetricalEditL );

  new GLUI_Column( glui4, false );


  GLUI_Panel *wheel1_panel = new GLUI_Panel(glui4, "Front Rigth Wheel");
   (new GLUI_Spinner( wheel1_panel, "Ypos(m)", &cardata.wheel1[0], 2, (GLUI_Update_CB)SimetricalEditR    ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel1_panel, "Rim diameter(in)", &cardata.wheel1[1], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 30 );
   (new GLUI_Spinner( wheel1_panel, "Tire Width(mm)", &cardata.wheel1[2], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( wheel1_panel, "Tire height-width ratio", &cardata.wheel1[3], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );
    (new GLUI_Spinner( wheel1_panel, "Wheel radius(m)", &cardata.wheelRadius[0] ))
    ->disable( );
   (new GLUI_Spinner( wheel1_panel, "Inertia(kg.m2)", &cardata.wheel1[4], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel1_panel, "Initial Ride Height(mm)", &cardata.wheel1[5], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 300 );
   (new GLUI_Spinner( wheel1_panel, "Toe(deg)", &cardata.wheel1[6], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel1_panel, "Camber(deg)", &cardata.wheel1[7], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 0 );
   (new GLUI_Spinner( wheel1_panel, "Stiffness", &cardata.wheel1[8], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 20, 50 );
   (new GLUI_Spinner( wheel1_panel, "Dynamic Fricction(%)", &cardata.wheel1[9], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 70, 90 );
   (new GLUI_Spinner( wheel1_panel, "Rolling Resistance", &cardata.wheel1[10], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( wheel1_panel, "Mu", &cardata.wheel1[11], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0.05, 1.8 );

  GLUI_Panel *wheel3_panel = new GLUI_Panel(glui4, "Rear Right Wheel");
   (new GLUI_Spinner( wheel3_panel, "Ypos(m)", &cardata.wheel3[0], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel3_panel, "Rim diameter(in)", &cardata.wheel3[1], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 30 );
   (new GLUI_Spinner( wheel3_panel, "Tire Width(mm)", &cardata.wheel3[2], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( wheel3_panel, "Tire height-width ratio", &cardata.wheel3[3], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );
    (new GLUI_Spinner( wheel3_panel, "Wheel radius(m)", &cardata.wheelRadius[2] ))
    ->disable( );
   (new GLUI_Spinner( wheel3_panel, "Inertia(kg.m2)", &cardata.wheel3[4], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel3_panel, "Initial Ride Height(mm)", &cardata.wheel3[5], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 300 );
   (new GLUI_Spinner( wheel3_panel, "Toe(deg)", &cardata.wheel3[6], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel3_panel, "Camber(deg)", &cardata.wheel3[7], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 0 );
   (new GLUI_Spinner( wheel3_panel, "Stiffness", &cardata.wheel3[8], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 20, 50 );
   (new GLUI_Spinner( wheel3_panel, "Dynamic Fricction(%)", &cardata.wheel3[9], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 70, 90 );
   (new GLUI_Spinner( wheel3_panel, "Rolling Resistance", &cardata.wheel3[10], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( wheel3_panel, "Mu", &cardata.wheel3[11], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0.05, 1.8 );

  new GLUI_Button( glui4, "Close Window", 4, (GLUI_Update_CB)hideglui );


/* Window for Wheels and Tires*/
  glui5 = GLUI_Master.create_glui("Steer Drivetrain Differential and Axles", 0, 0, 0);/* name, flags,
								 x, and y */
  glui5->hide();

  GLUI_Panel *steer_panel = new GLUI_Panel(glui5, "Steer");
   (new GLUI_Spinner( steer_panel, "Steer Lock (deg)", &cardata.steer[0] ))
    ->set_float_limits( 1, 45 );
   (new GLUI_Spinner( steer_panel, "Max Steer Speed (deg/s)", &cardata.steer[1] ))
    ->set_float_limits( 1, 360 );

  GLUI_Panel *frontaxle_panel = new GLUI_Panel(glui5, "Front Axle");
  frontaxle_panel->set_alignment( GLUI_ALIGN_RIGHT );
   (new GLUI_Spinner( frontaxle_panel, "xpos (m)", &cardata.frontaxle[0] ))
    ->set_float_limits( -3, 3 );
   (new GLUI_Spinner( frontaxle_panel, "Inertia (kg.m2)", &cardata.frontaxle[1] ))
    ->set_float_limits( -3, 3 );
   (new GLUI_Spinner( frontaxle_panel, "Roll Center Height (m)", &cardata.frontaxle[2] ))
    ->set_float_limits( 0, 1 );

  GLUI_Panel *rearaxle_panel = new GLUI_Panel(glui5, "Rear Axle");
  rearaxle_panel->set_alignment( GLUI_ALIGN_RIGHT );
   (new GLUI_Spinner( rearaxle_panel, "xpos (m)", &cardata.rearaxle[0] ))
    ->set_float_limits( -3, 3 );
   (new GLUI_Spinner( rearaxle_panel, "Inertia (kg.m2)", &cardata.rearaxle[1] ))
    ->set_float_limits( -3, 3 );
   (new GLUI_Spinner( rearaxle_panel, "Roll Center Height (m)", &cardata.rearaxle[2] ))
    ->set_float_limits( 0, 1 );

  new GLUI_Column( glui5, false );

//front differential
  GLUI_Panel *frontdifferential_panel = new GLUI_Panel(glui5, "Front Differential");
  frontdifferential_panel->set_alignment( GLUI_ALIGN_RIGHT );
  list_frontdifferential_type = new GLUI_Listbox( frontdifferential_panel, "Type:", &cardata.curr_frontdifferential_type );
  for( i=0; i<5; i++ )
    list_frontdifferential_type->add_item( i, cardata.differential_type[i] );
    list_frontdifferential_type->set_int_val(cardata.curr_frontdifferential_type);
   (new GLUI_Spinner( frontdifferential_panel, "Inertia (kg.m2)", &cardata.frontdifferential[0] ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( frontdifferential_panel, "Ratio", &cardata.frontdifferential[1] ))
    ->set_float_limits( 0, 10 );
   (new GLUI_Spinner( frontdifferential_panel, "Efficiency", &cardata.frontdifferential[2] ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( frontdifferential_panel, "min torque bias", &cardata.frontdifferential[3] ))
    ->set_float_limits( 0, 100 );
   (new GLUI_Spinner( frontdifferential_panel, "max torque bias", &cardata.frontdifferential[4] ))
    ->set_float_limits( 0, 100 );

//rear differential
  GLUI_Panel *reardifferential_panel = new GLUI_Panel(glui5, "Rear Differential");
  reardifferential_panel->set_alignment( GLUI_ALIGN_RIGHT );
  list_reardifferential_type = new GLUI_Listbox( reardifferential_panel, "Type:", &cardata.curr_reardifferential_type );
  for( i=0; i<5; i++ )
    list_reardifferential_type->add_item( i, cardata.differential_type[i] );
    list_reardifferential_type->set_int_val(cardata.curr_reardifferential_type);
   (new GLUI_Spinner( reardifferential_panel, "Inertia (kg.m2)", &cardata.reardifferential[0] ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( reardifferential_panel, "Ratio", &cardata.reardifferential[1] ))
    ->set_float_limits( 0, 10 );
   (new GLUI_Spinner( reardifferential_panel, "Efficiency", &cardata.reardifferential[2] ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( reardifferential_panel, "min torque bias", &cardata.reardifferential[3] ))
    ->set_float_limits( 0, 100 );
   (new GLUI_Spinner( reardifferential_panel, "max torque bias", &cardata.reardifferential[4] ))
    ->set_float_limits( 0, 100 );

//central differential
  GLUI_Panel *centraldifferential_panel = new GLUI_Panel(glui5, "Central Differential");
  centraldifferential_panel->set_alignment( GLUI_ALIGN_RIGHT );
  list_centraldifferential_type = new GLUI_Listbox( centraldifferential_panel, "Type:", &cardata.curr_centraldifferential_type );
  for( i=0; i<5; i++ )
    list_centraldifferential_type->add_item( i, cardata.differential_type[i] );
    list_centraldifferential_type->set_int_val(cardata.curr_centraldifferential_type);

   (new GLUI_Spinner( centraldifferential_panel, "Inertia (kg.m2)", &cardata.centraldifferential[0] ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( centraldifferential_panel, "Ratio", &cardata.centraldifferential[1] ))
    ->set_float_limits( 0, 10 );
   (new GLUI_Spinner( centraldifferential_panel, "Efficiency", &cardata.centraldifferential[2] ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( centraldifferential_panel, "min torque bias", &cardata.centraldifferential[3] ))
    ->set_float_limits( 0, 100 );
   (new GLUI_Spinner( centraldifferential_panel, "max torque bias", &cardata.centraldifferential[4] ))
    ->set_float_limits( 0, 100 );

//Drevetrain
  GLUI_Panel *drivetrain_panel = new GLUI_Panel(glui5, "Drivetrain");
  drivetrain_panel->set_alignment( GLUI_ALIGN_RIGHT );
  list_drivetrain_type = new GLUI_Listbox( drivetrain_panel, "Type:", &cardata.curr_drivetrain_type );
  for( i=0; i<3; i++ )
    list_drivetrain_type->add_item( i, cardata.drivetrain_type[i] );

   (new GLUI_Spinner( drivetrain_panel, "Inertia (kg.m2)", &cardata.drivetrain ))
    ->set_float_limits( 0, 1 );


  new GLUI_Button( glui5, "Close Window", 5, (GLUI_Update_CB)hideglui );


/* Window for Wheels and Tires*/
  glui6 = GLUI_Master.create_glui("Gear Box Inertia and Efficiency", 0, 0, 0);/* name, flags,
								 x, and y */
  glui6->hide();

   GLUI_Panel *glui6a = new GLUI_Panel( glui6, "", GLUI_PANEL_NONE );
   (new GLUI_Spinner( glui6a, "Number Of Gears", &cardata.numberOfGears,  0, (GLUI_Update_CB)numberOfGearsDisable ))
    ->set_int_limits( 1, 7 );

    new GLUI_Column( glui6a, false );

    new GLUI_StaticText( glui6a, "Clutch Inertia (kg.m2)" );
   (new GLUI_Spinner( glui6a, "PresPlate", &cardata.gearboxinertia[0] ))
    ->set_float_limits( 0.001, 0.500 );

    GLUI_Panel *glui6b = new GLUI_Panel( glui6, "" ,GLUI_PANEL_NONE );
    GLUI_Panel *gearboxinertia_panel = new GLUI_Panel( glui6b, "Inertia (kg.m2)" );
   i_gear[0]=new GLUI_Spinner( gearboxinertia_panel, "rear:", &cardata.gearboxinertia[1] );
   i_gear[0]->set_float_limits( 0.001, 0.050 );
   i_gear[1]=new GLUI_Spinner( gearboxinertia_panel, "1st", &cardata.gearboxinertia[2] );
   i_gear[1]->set_float_limits( 0.001, 0.050 );
   i_gear[2]=new GLUI_Spinner( gearboxinertia_panel, "2nd", &cardata.gearboxinertia[3] );
   i_gear[2]->set_float_limits( 0.001, 0.050 );
   i_gear[3]=new GLUI_Spinner( gearboxinertia_panel, "3rd", &cardata.gearboxinertia[4] );
   i_gear[3]->set_float_limits( 0.001, 0.050 );
   i_gear[4]=new GLUI_Spinner( gearboxinertia_panel, "4th", &cardata.gearboxinertia[5] );
   i_gear[4]->set_float_limits( 0.001, 0.050 );
   i_gear[5]=new GLUI_Spinner( gearboxinertia_panel, "5th", &cardata.gearboxinertia[6] );
   i_gear[5]->set_float_limits( 0.001, 0.050 );
   i_gear[6]=new GLUI_Spinner( gearboxinertia_panel, "6th", &cardata.gearboxinertia[7] );
   i_gear[6]->set_float_limits( 0.001, 0.050 );
   i_gear[7]=new GLUI_Spinner( gearboxinertia_panel, "7th", &cardata.gearboxinertia[8] );
   i_gear[7]->set_float_limits( 0.001, 0.050 );

  new GLUI_StaticText( glui6b, "" );

  (new GLUI_Spinner( glui6b, "Inertia", &gearsAutoInertia))
    ->set_float_limits( 0.001, 0.050 );

  new GLUI_Button( glui6b, "Update Inertia", 2, (GLUI_Update_CB)gearsAutoinertiaEff );

  new GLUI_Column( glui6b, false );

    GLUI_Panel *gearboxefficiency_panel = new GLUI_Panel( glui6b, "Efficiency" );
   e_gear[0]=new GLUI_Spinner( gearboxefficiency_panel, "rear:", &cardata.gearboxefficiency[0] );
   e_gear[0]->set_float_limits( 0.00, 1 );
   e_gear[1]=new GLUI_Spinner( gearboxefficiency_panel, "1st", &cardata.gearboxefficiency[1] );
   e_gear[1]->set_float_limits( 0.00, 1 );
   e_gear[2]=new GLUI_Spinner( gearboxefficiency_panel, "2nd", &cardata.gearboxefficiency[2] );
   e_gear[2]->set_float_limits( 0.00, 1 );
   e_gear[3]=new GLUI_Spinner( gearboxefficiency_panel, "3rd", &cardata.gearboxefficiency[3] );
   e_gear[3]->set_float_limits( 0.00, 1 );
   e_gear[4]=new GLUI_Spinner( gearboxefficiency_panel, "4th", &cardata.gearboxefficiency[4] );
   e_gear[4]->set_float_limits( 0.00, 1 );
   e_gear[5]=new GLUI_Spinner( gearboxefficiency_panel, "5th", &cardata.gearboxefficiency[5] );
   e_gear[5]->set_float_limits( 0.00, 1 );
   e_gear[6]=new GLUI_Spinner( gearboxefficiency_panel, "6th", &cardata.gearboxefficiency[6] );
   e_gear[6]->set_float_limits( 0.00, 1 );
   e_gear[7]=new GLUI_Spinner( gearboxefficiency_panel, "7th", &cardata.gearboxefficiency[7] );
   e_gear[7]->set_float_limits( 0.00, 1 );

  new GLUI_StaticText( glui6b, "" );

  (new GLUI_Spinner( glui6b, "Efficiency", &gearsAutoEff))
    ->set_float_limits( 0.0, 1.0 );

  new GLUI_Button( glui6b, "Update Eff", 1, (GLUI_Update_CB)gearsAutoinertiaEff );

  new GLUI_Column( glui6b, false );

    GLUI_Panel *gearbox_vel_panel = new GLUI_Panel( glui6b, "Speed (km/h)" );
    s_gear[0]=new GLUI_Spinner( gearbox_vel_panel, "rear:", &gearbox_speed[0], 0, (GLUI_Update_CB)gearboxRatioSpeed );
    s_gear[0]->set_float_limits( -3000, 0.0 );
    s_gear[1]=new GLUI_Spinner( gearbox_vel_panel, "1st", &gearbox_speed[1], 0, (GLUI_Update_CB)gearboxRatioSpeed );
    s_gear[1]->set_float_limits( 0.00, 3000 );
    s_gear[2]=new GLUI_Spinner( gearbox_vel_panel, "2nd", &gearbox_speed[2], 0, (GLUI_Update_CB)gearboxRatioSpeed );
    s_gear[2]->set_float_limits( 0.00, 3000 );
    s_gear[3]=new GLUI_Spinner( gearbox_vel_panel, "3rd", &gearbox_speed[3], 0, (GLUI_Update_CB)gearboxRatioSpeed );
    s_gear[3]->set_float_limits( 0.00, 3000 );
    s_gear[4]=new GLUI_Spinner( gearbox_vel_panel, "4th", &gearbox_speed[4], 0, (GLUI_Update_CB)gearboxRatioSpeed );
    s_gear[4]->set_float_limits( 0.00, 3000 );
    s_gear[5]=new GLUI_Spinner( gearbox_vel_panel, "5th", &gearbox_speed[5], 0, (GLUI_Update_CB)gearboxRatioSpeed );
    s_gear[5]->set_float_limits( 0.00, 3000 );
    s_gear[6]=new GLUI_Spinner( gearbox_vel_panel, "6th", &gearbox_speed[6], 0, (GLUI_Update_CB)gearboxRatioSpeed );
    s_gear[6]->set_float_limits( 0.00, 3000 );
    s_gear[7]=new GLUI_Spinner( gearbox_vel_panel, "7th", &gearbox_speed[7], 0, (GLUI_Update_CB)gearboxRatioSpeed );
    s_gear[7]->set_float_limits( 0.00, 3000 );


  new GLUI_StaticText( glui6b, "" );

  (new GLUI_Spinner( glui6b, "% reduction gear", &gearRed100))
    ->set_float_limits( 0, 99 );

  new GLUI_Button( glui6b, "Auto Calculate Gears", 0, (GLUI_Update_CB)gearboxRatioCalcAll );

  new GLUI_Column( glui6b, false );

    GLUI_Panel *gearbox_osp_panel = new GLUI_Panel( glui6b, "Optimal Shift Points (rpm)" );
    GLUI_Spinner *osp_gear[6];
    osp_gear[0]=new GLUI_Spinner( gearbox_osp_panel, "gear 1-2:", &bestOptShiftPoint[0] );
    osp_gear[0]->disable();
    osp_gear[1]=new GLUI_Spinner( gearbox_osp_panel, "gear 2-3:", &bestOptShiftPoint[1] );
    osp_gear[1]->disable();
    osp_gear[2]=new GLUI_Spinner( gearbox_osp_panel, "gear 3-4:", &bestOptShiftPoint[2] );
    osp_gear[2]->disable();
    osp_gear[3]=new GLUI_Spinner( gearbox_osp_panel, "gear 4-5:", &bestOptShiftPoint[3] );
    osp_gear[3]->disable();
    osp_gear[4]=new GLUI_Spinner( gearbox_osp_panel, "gear 5-6:", &bestOptShiftPoint[4] );
    osp_gear[4]->disable();
    osp_gear[5]=new GLUI_Spinner( gearbox_osp_panel, "gear 6-7:", &bestOptShiftPoint[5] );
    osp_gear[5]->disable();

    GLUI_Panel *gearbox_rpmaosp_panel = new GLUI_Panel( glui6b, "rpm after Optimal Shift Points" );
    GLUI_Spinner *rpmaosp_gear[6];
    rpmaosp_gear[0]=new GLUI_Spinner( gearbox_rpmaosp_panel, "gear 1-2:", &rpmAfterBestOptShiftPoint[0] );
    rpmaosp_gear[0]->disable();
    rpmaosp_gear[1]=new GLUI_Spinner( gearbox_rpmaosp_panel, "gear 2-3:", &rpmAfterBestOptShiftPoint[1] );
    rpmaosp_gear[1]->disable();
    rpmaosp_gear[2]=new GLUI_Spinner( gearbox_rpmaosp_panel, "gear 3-4:", &rpmAfterBestOptShiftPoint[2] );
    rpmaosp_gear[2]->disable();
    rpmaosp_gear[3]=new GLUI_Spinner( gearbox_rpmaosp_panel, "gear 4-5:", &rpmAfterBestOptShiftPoint[3] );
    rpmaosp_gear[3]->disable();
    rpmaosp_gear[4]=new GLUI_Spinner( gearbox_rpmaosp_panel, "gear 5-6:", &rpmAfterBestOptShiftPoint[4] );
    rpmaosp_gear[4]->disable();
    rpmaosp_gear[5]=new GLUI_Spinner( gearbox_rpmaosp_panel, "gear 6-7:", &rpmAfterBestOptShiftPoint[5] );
    rpmaosp_gear[5]->disable();

  new GLUI_Button( glui6b, "Calc Opt Shift Points", 0, (GLUI_Update_CB)optimalShiftPoint );

  new GLUI_Button( glui6, "Close Window", 6, (GLUI_Update_CB)hideglui );

/* Window for Suspension*/
  glui7 = GLUI_Master.create_glui("Suspension", 0, 0, 0);/* name, flags,
								 x, and y */
  glui7->hide();


  GLUI_Panel *suspension1_panel = new GLUI_Panel(glui7, "Left Front Wheel");
   (new GLUI_Spinner( suspension1_panel, "Spring(lbs/in)", &cardata.suspension1[0], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 10000 );
   (new GLUI_Spinner( suspension1_panel, "Suspension Course(m)", &cardata.suspension1[1], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1.0 );
   (new GLUI_Spinner( suspension1_panel, "Bellcrank", &cardata.suspension1[2], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( suspension1_panel, "Packers(mm)", &cardata.suspension1[3], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 10 );
   (new GLUI_Spinner( suspension1_panel, "Slow Bump(lbs/in/s)", &cardata.suspension1[4], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension1_panel, "Slow Rebound(lbs/in/s)", &cardata.suspension1[5], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension1_panel, "Fast Bump(lbs/in/s)", &cardata.suspension1[6], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension1_panel, "Fast Rebound(lbs/in/s)", &cardata.suspension1[7], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1000 );



  GLUI_Panel *suspension3_panel = new GLUI_Panel(glui7, "Left Rear Wheel");
   (new GLUI_Spinner( suspension3_panel, "Spring(lbs/in)", &cardata.suspension3[0], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 10000 );
   (new GLUI_Spinner( suspension3_panel, "Suspension Course(m)", &cardata.suspension3[1], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1.0 );
   (new GLUI_Spinner( suspension3_panel, "Bellcrank", &cardata.suspension3[2], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( suspension3_panel, "Packers(mm)", &cardata.suspension3[3], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0., 10 );
   (new GLUI_Spinner( suspension3_panel, "Slow Bump(lbs/in/s)", &cardata.suspension3[4], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension3_panel, "Slow Rebound(lbs/in/s)", &cardata.suspension3[5], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension3_panel, "Fast Bump(lbs/in/s)", &cardata.suspension3[6], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension3_panel, "Fast Rebound(lbs/in/s)", &cardata.suspension3[7], 1, (GLUI_Update_CB)SimetricalEditL ))
    ->set_float_limits( 0, 1000 );


    GLUI_Panel *antirollbar_1 = new GLUI_Panel( glui7, "Front Anti-Roll Bar" );
   (new GLUI_Spinner( antirollbar_1, "Front(lbs/in)", &cardata.antirollbar1[0]))
    -> set_int_limits( 0, 10000 );
   (new GLUI_Spinner( antirollbar_1, "Suspension Course(m)", &cardata.antirollbar1[1]))
    ->set_float_limits( 0, 1.0 );
   (new GLUI_Spinner( antirollbar_1, "Bellcrank", &cardata.antirollbar1[2] ))
    ->set_float_limits( 0, 5 );

new GLUI_Checkbox(glui7, "Simetrical edit", &simetricaledityn1, 1, (GLUI_Update_CB)SimetricalEditL );

  new GLUI_Column( glui7, false );

  GLUI_Panel *suspension2_panel = new GLUI_Panel(glui7, "Right Front Wheel");
   (new GLUI_Spinner( suspension2_panel, "Spring(lbs/in)", &cardata.suspension2[0], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 10000 );
   (new GLUI_Spinner( suspension2_panel, "Suspension Course(m)", &cardata.suspension2[1], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1.0 );
   (new GLUI_Spinner( suspension2_panel, "Bellcrank", &cardata.suspension2[2], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0.1, 5 );
   (new GLUI_Spinner( suspension2_panel, "Packers(mm)", &cardata.suspension2[3], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 10 );
   (new GLUI_Spinner( suspension2_panel, "Slow Bump(lbs/in/s)", &cardata.suspension2[4], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension2_panel, "Slow Rebound(lbs/in/s)", &cardata.suspension2[5], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension2_panel, "Fast Bump(lbs/in/s)", &cardata.suspension2[6], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension2_panel, "Fast Rebound(lbs/in/s)", &cardata.suspension2[7], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );


  GLUI_Panel *suspension4_panel = new GLUI_Panel(glui7, "Right Rear Wheel");
   (new GLUI_Spinner( suspension4_panel, "Spring(lbs/in)", &cardata.suspension4[0], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 10000 );
   (new GLUI_Spinner( suspension4_panel, "Suspension Course(m)", &cardata.suspension4[1], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1.0 );
   (new GLUI_Spinner( suspension4_panel, "Bellcrank", &cardata.suspension4[2], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0.1, 5 );
   (new GLUI_Spinner( suspension4_panel, "Packers(mm)", &cardata.suspension4[3], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 10 );
   (new GLUI_Spinner( suspension4_panel, "Slow Bump(lbs/in/s)", &cardata.suspension4[4], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension4_panel, "Slow Rebound(lbs/in/s)", &cardata.suspension4[5], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension4_panel, "Fast Bump(lbs/in/s)", &cardata.suspension4[6], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( suspension4_panel, "Fast Rebound(lbs/in/s)", &cardata.suspension4[7], 1, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );

    GLUI_Panel *antirollbar_2 = new GLUI_Panel( glui7, "Rear Anti-Roll Bar" );
   (new GLUI_Spinner( antirollbar_2, "Front(lbs/in)", &cardata.antirollbar2[0]))
    -> set_int_limits( 0, 10000 );
   (new GLUI_Spinner( antirollbar_2, "Suspension Course(m)", &cardata.antirollbar2[1]))
    ->set_float_limits( 0, 1.0 );
   (new GLUI_Spinner( antirollbar_2, "Bellcrank", &cardata.antirollbar2[2] ))
    ->set_float_limits( 0, 5 );


  new GLUI_Button( glui7, "Close Window", 7, (GLUI_Update_CB)hideglui );

/* Window for Sound and Graphic*/
  glui8 = GLUI_Master.create_glui("Sound and Graphics", 0, 0, 0);/* name, flags,
								 x, and y */
  glui8->hide();


  GLUI_Panel *sound_panel = new GLUI_Panel(glui8, "Sound");
   (new GLUI_EditText( sound_panel, "Engine Sample(.wav)", cardata.enginesample))->w=220;
   (new GLUI_Spinner( sound_panel, "RPM Scale", &cardata.rmpscale ))
    ->set_float_limits( 0, 1 );

  GLUI_Panel *graphic_panel = new GLUI_Panel(glui8, "Graphic Objects");
   (new GLUI_EditText( graphic_panel, "ENV", cardata.graphicenv))->w=220;
   (new GLUI_EditText( graphic_panel, "Wheel texture", cardata.wheelTexture))->w=220;
   (new GLUI_EditText( graphic_panel, "Shadow texture", cardata.shadowTexture))->w=220;
   (new GLUI_EditText( graphic_panel, "Tachometer texture", cardata.tachometerTexture))->w=220;
   (new GLUI_Spinner( graphic_panel, "Tachometer min value (rpm)", &cardata.tachometerMinMax[0]))
   ->set_int_limits( 0, 20000 );
   (new GLUI_Spinner( graphic_panel, "Tachometer max value (rpm)", &cardata.tachometerMinMax[1]))
   ->set_int_limits( 0, 20000 );
   (new GLUI_EditText( graphic_panel, "Speedometer texture", cardata.speedometerTexture))->w=220;
   (new GLUI_Spinner( graphic_panel, "Speedometer min value (km/h)", &cardata.speedometerMinMax[0]))
   ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( graphic_panel, "Speedometer max value (km/h)", &cardata.speedometerMinMax[1]))
   ->set_int_limits( 0, 1000 );

  new GLUI_StaticText( graphic_panel, "" );

  new GLUI_Button( graphic_panel, "Car Lights & Flames", 13, (GLUI_Update_CB)showglui );


  GLUI_Panel *ac_acc_type_panel = new GLUI_Panel( glui8, ".ac or .acc" );
  GLUI_RadioGroup *radio = new GLUI_RadioGroup(ac_acc_type_panel,&ac_acc_autogenerate);
  new GLUI_RadioButton( radio, ".ac" );
  new GLUI_RadioButton( radio, ".acc" );
  new GLUI_Button( glui8, "Autogenerate 3D Names", 0, (GLUI_Update_CB)create3dFilesNames );

  new GLUI_Column( glui8, false );

  GLUI_Panel *ranges_panel = new GLUI_Panel(glui8, "Ranges");
   new GLUI_StaticText( ranges_panel, "Car Range 1" );
   //new GLUI_Checkbox(ranges_panel, "Car Range 1", &carRange1dataC, 1, (GLUI_Update_CB)activateDeativateGraphicRanges);
   carRangeEditText[0]=new GLUI_EditText( ranges_panel, "File name", cardata.carRange1);
   carRangeEditText[0]->w=250;
   carRangeSpinner[0]=new GLUI_Spinner( ranges_panel, "Threshold", &cardata.carRange1dataA);
   carRangeSpinner[0]->set_int_limits( 0, 40 );
   carRangeCheckbox[0]=new GLUI_Checkbox(ranges_panel, "Wheels", &cardata.carRange1dataB);

   new GLUI_Separator( ranges_panel );

   new GLUI_Checkbox(ranges_panel, "Car Range 2", &cardata.carRange2dataC, 2, (GLUI_Update_CB)activateDeativateGraphicRanges);
   carRangeEditText[1]=new GLUI_EditText( ranges_panel, "File name", cardata.carRange2);
   carRangeEditText[1]->w=250;
   carRangeSpinner[1]=new GLUI_Spinner( ranges_panel, "Threshold", &cardata.carRange2dataA);
   carRangeSpinner[1]->set_int_limits( 0, 40 );
   carRangeCheckbox[1]=new GLUI_Checkbox(ranges_panel, "Wheels", &cardata.carRange2dataB);

   new GLUI_Separator( ranges_panel );

   new GLUI_Checkbox(ranges_panel, "Car Range 3", &cardata.carRange3dataC, 3, (GLUI_Update_CB)activateDeativateGraphicRanges);
   carRangeEditText[2]=new GLUI_EditText( ranges_panel, "File name", cardata.carRange3);
   carRangeEditText[2]->w=250;
   carRangeSpinner[2]=new GLUI_Spinner( ranges_panel, "Threshold", &cardata.carRange3dataA);
   carRangeSpinner[2]->set_int_limits( 0, 40 );
   carRangeCheckbox[2]=new GLUI_Checkbox(ranges_panel, "Wheels", &cardata.carRange3dataB);

   new GLUI_Separator( ranges_panel );

   new GLUI_Checkbox(ranges_panel, "Car Range 4", &cardata.carRange4dataC, 4, (GLUI_Update_CB)activateDeativateGraphicRanges);
   carRangeEditText[3]=new GLUI_EditText( ranges_panel, "File name", cardata.carRange4);
   carRangeEditText[3]->w=250;
   carRangeSpinner[3]=new GLUI_Spinner( ranges_panel, "Threshold", &cardata.carRange4dataA);
   carRangeSpinner[3]->set_int_limits( 0, 40 );
   carRangeCheckbox[3]=new GLUI_Checkbox(ranges_panel, "Wheels", &cardata.carRange4dataB);

   new GLUI_Separator( ranges_panel );

   new GLUI_Checkbox(ranges_panel, "Car Range 5", &cardata.carRange5dataC, 5, (GLUI_Update_CB)activateDeativateGraphicRanges);
   carRangeEditText[4]=new GLUI_EditText( ranges_panel, "File name", cardata.carRange5);
   carRangeEditText[4]->w=250;
   carRangeSpinner[4]=new GLUI_Spinner( ranges_panel, "Threshold", &cardata.carRange5dataA);
   carRangeSpinner[4]->set_int_limits( 0, 40 );
   carRangeCheckbox[4]=new GLUI_Checkbox(ranges_panel, "Wheels", &cardata.carRange5dataB);


   new GLUI_Button( glui8, "Close Window", 8, (GLUI_Update_CB)hideglui );


/* Window for Save Car Setup For Track*/
  glui10 = GLUI_Master.create_glui("Save Car Setup For Track", 0, 0, 0);/* name, flags,
								 x, and y */
  glui10->hide();

/* *********** using files ----> not used yet
#include <sys/types.h>
#ifdef __GNUC__
#include <dirent.h>
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif
#include <sys/stat.h>



chdir("../cars/");
 fb = new GLUI_FileBrowser(glui10, "Car", true, 1, (GLUI_Update_CB)printFileName);
//fb->set_allow_change_dir(0);
  new GLUI_Column( glui10, false );

chdir("../tracks/");
 fb2 = new GLUI_FileBrowser(glui10, "Track", true, 1, (GLUI_Update_CB)printFileName);


chdir("../torcs-car-setup-editor/");
new GLUI_Column( glui10, false );
 new GLUI_FileBrowser(glui10, "torcs-car-setup-editor", true, 1, (GLUI_Update_CB)printFileName);
************************/
  new GLUI_StaticText( glui10, "Save Car Setup for Player (Human) and Robots" );

  GLUI_Panel *driversPanel = new GLUI_Panel(glui10, "Car setup for driver:");
  (new GLUI_EditText( driversPanel, "Driver:", driverName ))->w=170;
  GLUI_Listbox *driverList = new GLUI_Listbox( driversPanel, "Names:", &driverVal, 0, (GLUI_Update_CB)updateDriverName );
  loadDriversList();
  for( i=0; i<driversList.size(); i++ )
    driverList->add_item( i, driversList[i].c_str() );

  new GLUI_StaticText( driversPanel, "If you select a robot select" );
  new GLUI_StaticText( driversPanel, "the number of the robot" );
  new GLUI_EditText( driversPanel, "Number:", driverNameNumber );

  new GLUI_Column( glui10, false );

  new GLUI_StaticText( glui10, "" );
  GLUI_Panel *carPanel = new GLUI_Panel(glui10, "Car setup for car");
  (new GLUI_EditText( carPanel, "CarName", cardata.carname ))->w=170;

  new GLUI_StaticText( carPanel, "" );
  loadCarsList();
  GLUI_Listbox *carsListPanel = new GLUI_Listbox( carPanel, "Cars:", &carsListVal, 0, (GLUI_Update_CB)updateCarName );
  for( i=0; i<carsList.size(); i++ )
    carsListPanel->add_item( i, carsList[i].c_str() );

  new GLUI_Column( glui10, false );

  new GLUI_StaticText( glui10, "" );

  GLUI_Panel *trackPanel = new GLUI_Panel(glui10, "Select the track");
  (new GLUI_EditText( trackPanel, "Track:", trackname ))->w=170;
  new GLUI_StaticText( glui10, "" );

  GLUI_Listbox *tracksRoadList = new GLUI_Listbox( trackPanel, "Road:", &tracksRoadVal, 1, (GLUI_Update_CB)updateTrackName );
  loadTrackList();
  for( i=0; i<tracksRoad.size(); i++ )
    tracksRoadList->add_item( i, tracksRoad[i].c_str() );

  GLUI_Listbox *trackOvalList = new GLUI_Listbox( trackPanel, "Oval:", &tracksOvalVal, 2, (GLUI_Update_CB)updateTrackName );
  for( i=0; i<tracksOval.size(); i++ )
    trackOvalList->add_item( i, tracksOval[i].c_str() );

  GLUI_Listbox *trackDirtList = new GLUI_Listbox( trackPanel, "Dirt:", &tracksDirtVal, 3, (GLUI_Update_CB)updateTrackName );
  for( i=0; i<tracksDirt.size(); i++ )
    trackDirtList->add_item( i, tracksDirt[i].c_str() );

  GLUI_Listbox *trackGprixList = new GLUI_Listbox( trackPanel, "Gprix:", &tracksGprixVal, 4, (GLUI_Update_CB)updateTrackName );
  for( i=0; i<tracksGprix.size(); i++ )
    trackGprixList->add_item( i, tracksGprix[i].c_str() );

 new GLUI_Column( glui10, false );
 new GLUI_StaticText( glui10, "" );

  GLUI_Panel *savexmlPanel = new GLUI_Panel(glui10, "Save the car setup data");
  new GLUI_Button( savexmlPanel, "Save Car Data for All Tracks", 1, (GLUI_Update_CB)savecardata );
  new GLUI_Button( savexmlPanel, "Save Car Data for Selected Track", 2, (GLUI_Update_CB)savecardata );
  new GLUI_StaticText( savexmlPanel, "" );
  new GLUI_Button( savexmlPanel, "Save XML for the Car", 1, (GLUI_Update_CB)savexml );
  new GLUI_StaticText( savexmlPanel, "" );
  new GLUI_StaticText( savexmlPanel, "Save XML for the selected driver" );
  new GLUI_Button( savexmlPanel, "Save XML for All Tracks", 2, (GLUI_Update_CB)savexml );
  new GLUI_Button( savexmlPanel, "Save XML for Selected Track", 3, (GLUI_Update_CB)savexml );

  new GLUI_StaticText( savexmlPanel, "" );

  new GLUI_Button( savexmlPanel, "Open CarData", 1, (GLUI_Update_CB)opencardata );
  new GLUI_Button( savexmlPanel, "Open CarData for Selected Track", 2, (GLUI_Update_CB)opencardata );

  new GLUI_StaticText( savexmlPanel, "" );

  new GLUI_Button( savexmlPanel, "Import XML", 0, (GLUI_Update_CB)importxml );

  new GLUI_StaticText( savexmlPanel, "" );

  new GLUI_Button( savexmlPanel, "Browser Window", 0, (GLUI_Update_CB)fileBrowserWindow );

  new GLUI_StaticText( savexmlPanel, "" );

   new GLUI_Button( glui10, "Close Window", 10, (GLUI_Update_CB)hideglui );

/* Window for Sound and Graphic*/
  glui11 = GLUI_Master.create_glui("Engine", 0, 0, 0);/* name, flags,
								 x, and y */
  glui11->hide();

  GLUI_Panel *engine_panel1 = new GLUI_Panel( glui11, "RPM" );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[0], 0,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[1], 1,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[2], 2,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[3], 3,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[4], 4,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[5], 5,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[6], 6,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[7], 7,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[8], 8,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[9], 9,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[10], 10,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[11], 11,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[12], 12,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[13], 13,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[14], 14,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[15], 15,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[16], 16,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[17], 17,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[18], 18,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[19], 19,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );
   (new GLUI_Spinner( engine_panel1, "", &cardata.engine.rpmValue[20], 20,(GLUI_Update_CB)CalcRPM  ))
    ->set_int_limits( 0, 30000 );

     new GLUI_Column( glui11, false );

  GLUI_Panel *engine_panel = new GLUI_Panel( glui11, "Torque(N.m)" );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[0], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[1], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[2], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[3], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[4], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[5], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[6], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[7], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[8], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[9], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[10], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[11], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[12], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[13], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[14], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[15], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[16], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[17], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[18], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[19], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );
   (new GLUI_Spinner( engine_panel, "", &cardata.engine.tqValue[20], 1,(GLUI_Update_CB)CalcCV  ))
    ->set_int_limits( 0, 1000 );


   new GLUI_Column( glui11, false );

  GLUI_Panel *engine_panel3 = new GLUI_Panel( glui11, "Power (CV)" );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[0], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[1], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[2], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[3], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[4], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[5], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[6], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[7], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[8], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[9], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[10], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[11], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[12], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[13], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[14], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[15], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[16], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[17], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[18], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[19], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );
   (new GLUI_Spinner( engine_panel3, "", &cardata.engine.cvValue[20], 1,(GLUI_Update_CB)CalcTQ ))
    ->set_int_limits( 0, 3000 );

   new GLUI_Column( glui11, false );


  GLUI_Panel *engine_type_panel = new GLUI_Panel( glui11, "Type" );
   (new GLUI_Spinner( engine_type_panel, "Capacity", &cardata.engine.capacity))
    ->set_float_limits( 0, 20000 );
   list_engine_capacity_units = new GLUI_Listbox( engine_type_panel, "Units:", &cardata.engine.curr_capacity_units );
   for( i=0; i<3; i++ )
    list_engine_capacity_units->add_item( i, cardata.engine.capacity_units[i] );
   (new GLUI_Spinner( engine_type_panel, "Cylinders", &cardata.engine.cylinders))
    ->set_int_limits( 0, 16 );
   list_engine_shape = new GLUI_Listbox( engine_type_panel, "Shape:", &cardata.engine.curr_shape );
   for( i=0; i<4; i++ )
    list_engine_shape->add_item( i, cardata.engine.shape[i] );
   list_engine_position = new GLUI_Listbox( engine_type_panel, "Position:", &cardata.engine.curr_position );
   for( i=0; i<5; i++ )
    list_engine_position->add_item( i, cardata.engine.position[i] );

  GLUI_Panel *engine_brake_panel = new GLUI_Panel( glui11, "Brake" );
   (new GLUI_Spinner( engine_brake_panel, "Linear Coefficient", &cardata.engine.brakeLinearCoefficient))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( engine_brake_panel, "Coefficient", &cardata.engine.brakeCoefficient))
    ->set_float_limits( 0, 1 );

  GLUI_Panel *engine_turbo_panel = new GLUI_Panel( glui11, "Turbo" );
    new GLUI_Checkbox(engine_turbo_panel, "Activate", &cardata.engine.turboS);
   (new GLUI_Spinner( engine_turbo_panel, "rpm", &cardata.engine.turbo[0]))
    ->set_float_limits( 0, 20000 );
   (new GLUI_Spinner( engine_turbo_panel, "factor", &cardata.engine.turbo[1]))
    ->set_float_limits( 0, 10 );
   (new GLUI_Spinner( engine_turbo_panel, "lag", &cardata.engine.turbo[2]))
    ->set_float_limits( 0, 10 );

  new GLUI_Checkbox(glui11, "Full RPM Scale", &fullRpmScale);

  GLUI_Panel *loadEngine_panel = new GLUI_Panel( glui11, "Load Engine" );
  loadEngine_List = new GLUI_Listbox( loadEngine_panel, "Engine:", &currentEngine);
  loadEngineList ();
  new GLUI_Button( loadEngine_panel, "loadEngine", 0, (GLUI_Update_CB)loadEngine );

  GLUI_Panel *glui11a = new GLUI_Panel( glui11, "", GLUI_PANEL_NONE );
  (new GLUI_Spinner( glui11a, "", &cvmaxNew))
     ->set_float_limits( 0, 9000 );
   new GLUI_Column( glui11a, false );
  new GLUI_Button( glui11a, "Scale All CV", 1, (GLUI_Update_CB)scaleAllEnginePoints );

  GLUI_Panel *glui11b = new GLUI_Panel( glui11, "", GLUI_PANEL_NONE );
  (new GLUI_Spinner( glui11b, "", &cvctNew))
    ->set_float_limits( 0, 9000 );
  new GLUI_Column( glui11b, false );
  new GLUI_Button( glui11b, "constant CV", 4, (GLUI_Update_CB)scaleAllEnginePoints );

  GLUI_Panel *glui11c = new GLUI_Panel( glui11, "", GLUI_PANEL_NONE );
  (new GLUI_Spinner( glui11c, "", &tqmaxNew))
    ->set_float_limits( 0, 9000 );
  new GLUI_Column( glui11c, false );
  new GLUI_Button( glui11c, "Scale All TQ", 2, (GLUI_Update_CB)scaleAllEnginePoints );

  GLUI_Panel *glui11d = new GLUI_Panel( glui11, "", GLUI_PANEL_NONE );
  (new GLUI_Spinner( glui11d, "", &tqctNew))
    ->set_float_limits( 0, 9000 );
  new GLUI_Column( glui11d, false );
  new GLUI_Button( glui11d, "constant TQ", 3, (GLUI_Update_CB)scaleAllEnginePoints );

  GLUI_Panel *glui11e = new GLUI_Panel( glui11, "", GLUI_PANEL_NONE );
  new GLUI_Button( glui11e, "Change Shape", 2, (GLUI_Update_CB)modifyShapeOfEnginePoints );
  (new GLUI_Spinner( glui11e, "", &cvEngineShape[0]))->set_float_limits( 0, 300 );
  (new GLUI_Spinner( glui11e, "", &cvEngineShape[1]))->set_float_limits( 0, 50 );
  (new GLUI_Spinner( glui11e, "", &cvEngineShape[2]))->set_float_limits( -10, 10 );
  (new GLUI_Spinner( glui11e, "", &cvEngineShape[3]))->set_float_limits( -5, 5 );
  new GLUI_Column( glui11e, false );
  new GLUI_Button( glui11e, "Change RPM Points", 3, (GLUI_Update_CB)modifyShapeOfEnginePoints );

  new GLUI_StaticText( glui11, "");
  new GLUI_Button( glui11, "Close Window", 11, (GLUI_Update_CB)hideglui );


/* Window for Car dimensions and driver position*/
  glui12 = GLUI_Master.create_glui("Car dimensions and driver position", 0, 0, 0);/* name, flags,
								 x, and y */
  glui12->hide();

  GLUI_Panel *driver_position_panel = new GLUI_Panel( glui12, "Driver Position" );
   (new GLUI_Spinner( driver_position_panel, "x pos (m)", &cardata.driverPosition[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( driver_position_panel, "y pos (m)", &cardata.driverPosition[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( driver_position_panel, "z pos (m)", &cardata.driverPosition[2]))
    ->set_float_limits( -10, 10 );

  GLUI_Panel *bonnet_position_panel = new GLUI_Panel( glui12, "Bonnet Position" );
   (new GLUI_Spinner( bonnet_position_panel, "x pos (m)", &cardata.bonnetPosition[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( bonnet_position_panel, "y pos (m)", &cardata.bonnetPosition[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( bonnet_position_panel, "z pos (m)", &cardata.bonnetPosition[2]))
    ->set_float_limits( -10, 10 );

  GLUI_Panel *carBodyDimensions_panel = new GLUI_Panel( glui12, "Car Body Dimensions" );
   (new GLUI_Spinner( carBodyDimensions_panel, "Body length (m)", &cardata.carBodyDimensions[0]))
    ->set_float_limits( 0, 20 );
   (new GLUI_Spinner( carBodyDimensions_panel, "Body width (m)", &cardata.carBodyDimensions[1]))
    ->set_float_limits( 0, 20 );
   (new GLUI_Spinner( carBodyDimensions_panel, "Body height (m)", &cardata.carBodyDimensions[2]))
    ->set_float_limits( 0, 20 );

  GLUI_Panel *carOverallDimensions_panel = new GLUI_Panel( glui12, "Car Body Dimensions" );
   (new GLUI_Spinner( carOverallDimensions_panel, "Body length (m)", &cardata.carOverallDimensions[0]))
    ->set_float_limits( 0, 20 );
   (new GLUI_Spinner( carOverallDimensions_panel, "Body width (m)", &cardata.carOverallDimensions[1]))
    ->set_float_limits( 0, 20 );

   new GLUI_Button( glui12, "Close Window", 12, (GLUI_Update_CB)hideglui );

/* Window for Car Lights*/
  glui13 = GLUI_Master.create_glui("Car Lights & Flames", 0, 0, 0);/* name, flags,
								 x, and y */
  glui13->hide();

  GLUI_Panel *car_lights1 = new GLUI_Panel( glui13, "Head1" );
    new GLUI_Checkbox(car_lights1, "Activate", &cardata.head1aS);
   (new GLUI_Spinner( car_lights1, "x pos (m)", &cardata.head1a[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights1, "y pos (m)", &cardata.head1a[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights1, "z pos (m)", &cardata.head1a[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights1, "size (m)", &cardata.head1a[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights2 = new GLUI_Panel( glui13, "Head1" );
    new GLUI_Checkbox(car_lights2, "Activate", &cardata.head1bS);
   (new GLUI_Spinner( car_lights2, "x pos (m)", &cardata.head1b[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights2, "y pos (m)", &cardata.head1b[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights2, "z pos (m)", &cardata.head1b[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights2, "size (m)", &cardata.head1b[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights3 = new GLUI_Panel( glui13, "Head2" );
    new GLUI_Checkbox(car_lights3, "Activate", &cardata.head2aS);
   (new GLUI_Spinner( car_lights3, "x pos (m)", &cardata.head2a[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights3, "y pos (m)", &cardata.head2a[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights3, "z pos (m)", &cardata.head2a[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights3, "size (m)", &cardata.head2a[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights4 = new GLUI_Panel( glui13, "Head2" );
    new GLUI_Checkbox(car_lights4, "Activate", &cardata.head2bS);
   (new GLUI_Spinner( car_lights4, "x pos (m)", &cardata.head2b[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights4, "y pos (m)", &cardata.head2b[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights4, "z pos (m)", &cardata.head2b[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights4, "size (m)", &cardata.head2b[3]))
    ->set_float_limits( 0, 0.8 );

   new GLUI_Column( glui13, false );

  GLUI_Panel *car_lights5 = new GLUI_Panel( glui13, "Rear" );
    new GLUI_Checkbox(car_lights5, "Activate", &cardata.rearaS);
   (new GLUI_Spinner( car_lights5, "x pos (m)", &cardata.reara[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights5, "y pos (m)", &cardata.reara[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights5, "z pos (m)", &cardata.reara[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights5, "size (m)", &cardata.reara[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights6 = new GLUI_Panel( glui13, "Rear" );
    new GLUI_Checkbox(car_lights6, "Activate", &cardata.rearbS);
   (new GLUI_Spinner( car_lights6, "x pos (m)", &cardata.rearb[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights6, "y pos (m)", &cardata.rearb[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights6, "z pos (m)", &cardata.rearb[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights6, "size (m)", &cardata.rearb[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights7 = new GLUI_Panel( glui13, "Brake" );
    new GLUI_Checkbox(car_lights7, "Activate", &cardata.brakeaS);
   (new GLUI_Spinner( car_lights7, "x pos (m)", &cardata.brakea[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights7, "y pos (m)", &cardata.brakea[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights7, "z pos (m)", &cardata.brakea[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights7, "size (m)", &cardata.brakea[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights8 = new GLUI_Panel( glui13, "Brake" );
    new GLUI_Checkbox(car_lights8, "Activate", &cardata.brakebS);
   (new GLUI_Spinner( car_lights8, "x pos (m)", &cardata.brakeb[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights8, "y pos (m)", &cardata.brakeb[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights8, "z pos (m)", &cardata.brakeb[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights8, "size (m)", &cardata.brakeb[3]))
    ->set_float_limits( 0, 0.8 );

   new GLUI_Column( glui13, false );

  GLUI_Panel *car_lights9 = new GLUI_Panel( glui13, "Rear" );
    new GLUI_Checkbox(car_lights9, "Activate", &cardata.rearcS);
   (new GLUI_Spinner( car_lights9, "x pos (m)", &cardata.rearc[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights9, "y pos (m)", &cardata.rearc[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights9, "z pos (m)", &cardata.rearc[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights9, "size (m)", &cardata.rearc[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights10 = new GLUI_Panel( glui13, "Rear" );
    new GLUI_Checkbox(car_lights10, "Activate", &cardata.reardS);
   (new GLUI_Spinner( car_lights10, "x pos (m)", &cardata.reard[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights10, "y pos (m)", &cardata.reard[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights10, "z pos (m)", &cardata.reard[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights10, "size (m)", &cardata.reard[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights11 = new GLUI_Panel( glui13, "Brake" );
    new GLUI_Checkbox(car_lights11, "Activate", &cardata.brakecS);
   (new GLUI_Spinner( car_lights11, "x pos (m)", &cardata.brakec[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights11, "y pos (m)", &cardata.brakec[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights11, "z pos (m)", &cardata.brakec[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights11, "size (m)", &cardata.brakec[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights12 = new GLUI_Panel( glui13, "Brake" );
    new GLUI_Checkbox(car_lights12, "Activate", &cardata.brakedS);
   (new GLUI_Spinner( car_lights12, "x pos (m)", &cardata.braked[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights12, "y pos (m)", &cardata.braked[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights12, "z pos (m)", &cardata.braked[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights12, "size (m)", &cardata.braked[3]))
    ->set_float_limits( 0, 0.8 );

   new GLUI_Column( glui13, false );

  GLUI_Panel *car_lights13 = new GLUI_Panel( glui13, "Brake2" );
    new GLUI_Checkbox(car_lights13, "Activate", &cardata.brake2aS);
   (new GLUI_Spinner( car_lights13, "x pos (m)", &cardata.brake2a[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights13, "y pos (m)", &cardata.brake2a[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights13, "z pos (m)", &cardata.brake2a[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights13, "size (m)", &cardata.brake2a[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_lights14 = new GLUI_Panel( glui13, "Brake2" );
    new GLUI_Checkbox(car_lights14, "Activate", &cardata.brake2bS);
   (new GLUI_Spinner( car_lights14, "x pos (m)", &cardata.brake2b[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights14, "y pos (m)", &cardata.brake2b[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights14, "z pos (m)", &cardata.brake2b[2]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_lights14, "size (m)", &cardata.brake2b[3]))
    ->set_float_limits( 0, 0.8 );

  GLUI_Panel *car_flames = new GLUI_Panel( glui13, "Flames" );

    new GLUI_Checkbox(car_flames, "Activate flames", &cardata.flameS);
   (new GLUI_Spinner( car_flames, "Power", &cardata.flamePower))
    ->set_float_limits( 0, 10 );

    new GLUI_Separator( car_flames );

   (new GLUI_Spinner( car_flames, "x pos (m)", &cardata.flame1[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_flames, "y pos (m)", &cardata.flame1[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_flames, "z pos (m)", &cardata.flame1[2]))
    ->set_float_limits( -10, 10 );

    new GLUI_Separator( car_flames );

   (new GLUI_Spinner( car_flames, "x pos (m)", &cardata.flame2[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_flames, "y pos (m)", &cardata.flame2[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( car_flames, "z pos (m)", &cardata.flame2[2]))
    ->set_float_limits( -10, 10 );

   new GLUI_Button( glui13, "Close Window", 13, (GLUI_Update_CB)hideglui );

/* Window for Car Lights*/
  glui14 = GLUI_Master.create_glui("Car Name & Category -- Autor info", 0, 0, 0);/* name, flags,
								 x, and y */
  glui14->hide();

  GLUI_Panel *carCategory_panel = new GLUI_Panel( glui14, "Car Category" );
  (new GLUI_EditText( carCategory_panel, "Category:", cardata.carCategory ))->w=200;

  GLUI_Panel *carName_panel = new GLUI_Panel( glui14, "Car Name" );
  (new GLUI_EditText( carName_panel, "Car name:", cardata.carname, 0,(GLUI_Update_CB)syncLiveAll ))->w=200;
  (new GLUI_EditText( carName_panel, "Full Car name:", cardata.fullCarName, 0,(GLUI_Update_CB)syncLiveAll ))->w=300;

  GLUI_Panel *autorInfo_panel = new GLUI_Panel( glui14, "Autor Info" );
  (new GLUI_EditText( autorInfo_panel, "name:", cardata.autorName, 0, (GLUI_Update_CB)saveAutorInfo ))->w=250;
  (new GLUI_EditText( autorInfo_panel, "e-mail:", cardata.autorEmail, 0, (GLUI_Update_CB)saveAutorInfo ))->w=250;


   new GLUI_Button( glui14, "Close Window", 14, (GLUI_Update_CB)hideglui );

/* Window for the Help */
  glui2 = GLUI_Master.create_glui("Torcs Car Setup Editor Help", 0, 0, 0);/* name, flags,
								 x, and y */
  glui2->hide();
     /* color of the glui2 window */
    //glui2->bkgd_color.r = 0.5;
    //glui2->bkgd_color.g = 0.5;
    //glui2->bkgd_color.b = 0.5;


  GLUI_Panel *help1_panel =new GLUI_Panel(glui2, "How to use the program");
    new GLUI_StaticText( help1_panel, "You can edit all the parameters by "
    "using the text boxes and spiners of the program." );
    new GLUI_StaticText( help1_panel,
    "At the right you can see the engine graph and how afect your changes "
    "in acceleration, grip, speed and braking at your car." );
    new GLUI_StaticText( help1_panel,
    "You can select in this window at the box of the left an item and "
    "it will be displayed at the right." );

  GLUI_Panel *help2_panel =new GLUI_Panel(glui2, "Description of the parameters");

    new GLUI_StaticText(help2_panel,"Items");
    helpList = new GLUI_List(help2_panel,true,0,helpSelectList);
      helpList->set_h(400);
      helpList->set_w(150);
      helpList->add_item(0,"Readme");
      helpList->add_item(1,"Front wing");
      helpList->add_item(2,"Rear wing");
      helpList->add_item(3,"Gear Ratio");
      helpList->add_item(4,"Engine");
      helpList->add_item(5,"Brakes");
      helpList->add_item(6,"Wheels & Tires");
      helpList->add_item(7,"Steer, Differential...");
      helpList->add_item(8,"Suspension");

   new GLUI_Column( help2_panel, false);

    helpTextBox = new GLUI_TextBox(help2_panel,true);
    //helpTextBox->set_text(help_general.c_str());
    helpSelectList(0);
    helpTextBox->set_h(400);
    helpTextBox->set_w(650);
    helpTextBox->disable();
    helpTextBox->scrollbar->enable();

  new GLUI_Button( glui2, "Close Window", 2, (GLUI_Update_CB)hideglui );

  /* end of Window for help */




  glui->set_main_gfx_window( main_window );
  glui2->set_main_gfx_window( main_window );
  glui3->set_main_gfx_window( main_window );
  glui4->set_main_gfx_window( main_window );
  glui5->set_main_gfx_window( main_window );
  glui6->set_main_gfx_window( main_window );
  glui7->set_main_gfx_window( main_window );
  glui8->set_main_gfx_window( main_window );
  //glui9->set_main_gfx_window( main_window ); is the warning window
  glui10->set_main_gfx_window( main_window );
  glui11->set_main_gfx_window( main_window );
  glui12->set_main_gfx_window( main_window );
  glui13->set_main_gfx_window( main_window );
  glui14->set_main_gfx_window( main_window );

//hide all the windows
glui2->hide();
glui3->hide();
glui4->hide();
glui5->hide();
glui6->hide();
glui7->hide();
glui8->hide();
glui10->hide();
glui11->hide();
glui12->hide();
glui13->hide();
glui14->hide();

  /* We register the idle callback with GLUI, *not* with GLUT */
  GLUI_Master.set_glutIdleFunc( NULL );
  //GLUI_Master.set_glutIdleFunc( myGlutIdle );


  glutMainLoop();

  return EXIT_SUCCESS;

}




