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
#include <string>
#include <iostream>
#include <GL/glui.h>

#include "../portability.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef WIN32
#include <Windows.h>
#endif

#ifndef WIN32
#include <sys/time.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


#include "ac3d.h"

#include "../cardata.h"

using namespace std;

GLUI    *gluiLoadAC3D;
GLUI    *gluiLoadAC3D2;
int   main_window_AC3D;
float obj_pos[] = { 0.0, 0.0, 0.0 };
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
int showWheels = 1;
int showPhysics = 1;
int showCar = 1;
int showLight = 0;
int showFlames = 0;
int ShowGluiLoadAC3D2 = 0;
int solidBox = 0;

int ChdirCar = 1;

extern int simetricaledityn2;

//prototypes
void SimetricalEditL (int val);
void SimetricalEditR (int val);

#define WINDOW_WIDTH    900
#define WINDOW_HEIGHT   680

int window_width = WINDOW_WIDTH;
int window_height = WINDOW_HEIGHT;

int display_list;

void sincAll ( void )
{
        GLUI_Master.sync_live_all();   
}    

void myGlutIdle2( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window_AC3D ) 
    glutSetWindow(main_window_AC3D);  

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */

  glutPostRedisplay();
}

/***************************************** myGlutMotion() **********/

void myGlutMotion_AC3D(int x, int y )
{
  glutPostRedisplay(); 
}

void set_projection(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w/(float)h, 0.1, 10000.0);
}

void init_gfx(void)
{
    ac_prepare_render();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    set_projection(WINDOW_WIDTH,WINDOW_HEIGHT);
}

/*
#ifdef WIN32
#include <time.h>
#endif

struct timeval tv;
int ts, tu;
int ets, etu;

#ifdef WIN32
void gettimeofday( struct timeval *tv, void *dummy )
{
	LARGE_INTEGER perfCount;
	LARGE_INTEGER perfFreq;

	double freq, count, time;
	tv->tv_sec = 0;
	tv->tv_usec= 0;

	// Get the frequency
	if( !QueryPerformanceFrequency( &perfFreq ) )
		return;

	// Get the current count.
	if( !QueryPerformanceCounter( &perfCount ) )
		return;

	freq  = (double) perfFreq.LowPart;
	count = (double) perfCount.LowPart;
	freq += (double) perfFreq.HighPart  *  4294967296.0;
	count+= (double) perfCount.HighPart *  4294967296.0;

	time = count / freq;

	tv->tv_sec = (int) time;
	tv->tv_usec= (int) ((time - (double)tv->tv_sec) * 1000000.0);

}
#endif // WIN32



void timer_start()
{
    gettimeofday(&tv, NULL);
    ts = tv.tv_sec;
    tu = tv.tv_usec;
}

float timer_stop()
{
    int s, u;
    
    gettimeofday(&tv, NULL);
    ets = tv.tv_sec;
    etu = tv.tv_usec;
    
    s = ets-ts;
    u = etu-tu;
    return(s+u/1.0E+6);
}
*/




void drawWheel ( float wheel1[12] , int FrontRear , int LeftRight )
{   //Front 1 //// rear 0
    // Left -1  /// right 1
    glDisable(GL_LIGHTING);
    glPushMatrix();
    float wRadius = (wheel1[1]*0.0254/2.0)+(wheel1[2]*wheel1[3]/1000.0);
    float rRadius = wheel1[1]*0.0254/2.0;
    float wWidth = wheel1[2]/1000.0;
    float wYpos = wheel1[0];
    float wToe = wheel1[7]*LeftRight;
    float axlePos = 0.0;
    if ( FrontRear == 1) axlePos = frontaxle[0];
    else axlePos = rearaxle[0];
    GLUquadricObj *quadratic;
    quadratic=gluNewQuadric();
    glDisable(GL_CULL_FACE);
    glTranslatef(axlePos,0,0);
    glTranslatef(0,wRadius,0);
    glTranslatef(0,0,wYpos);
    glTranslatef(0,0,-0.5*wWidth);
    glRotatef(wToe, 1.0f, 0.0f, 0.0f);
    glColor3f (0.05f, 0.05f,0.05f);
    gluCylinder(quadratic,wRadius,wRadius,wWidth,32,32);
    gluDisk(quadratic,rRadius,wRadius,32,32);
    glTranslatef(0,0,wWidth);
    gluDisk(quadratic,rRadius,wRadius,32,32);
    glTranslatef(0,0,-1*wWidth);
    glColor3f (0.5f, 0.5f,0.5f);
    glTranslatef(0,0,0.05);
    gluDisk(quadratic,0.0f,rRadius,32,32);
    glTranslatef(0,0,wWidth);
    glTranslatef(0,0,-0.05);
    gluDisk(quadratic,0.0f,rRadius,32,32);
    glEnable(GL_CULL_FACE);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}    

void drawLight ( float light[4] , float red, float green, float blue )
{
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(red,green,blue);
    glTranslatef(light[0],0,0); //x
    glTranslatef(0,0,light[1]);//y
    glTranslatef(0,light[2],0); //z
    glutSolidSphere( light[3],10, 10 );
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawFlame ( float light[4] , float red, float green, float blue )
{
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(red,green,blue);
    glTranslatef(light[0],0,0); //x
    glTranslatef(0,0,light[1]);//y
    glTranslatef(0,light[2],0); //z
    glutSolidSphere( 0.10,10, 10 );
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawscene(void)
{
    static float rot = 0.0;
    float frametime;
    static float tottime = 0.0;
    static int framec = 0;


    GLfloat light_position[] = { -1000.0, 1000.0, 1000.0, 0.0 };
    GLfloat light_position2[] = { 0.0, 1000.0, 1000.0, 0.0 };

    //timer_start();


    set_projection(window_width, window_height);

    gluLookAt(0, 1.8, 4, 0, 0, 0, 0, 1, 0);

    glClearColor(0,0,0,0);
    glClearColor(0.5,0.5,0.5,0); //background color

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position2);
    
    
    //rotation and translation from the glui
   // glutWireTorus( 0.5,0.8,35,30 );
    //glRotatef(rot, 0, 1, 0);
    glTranslatef( obj_pos[0], obj_pos[1], obj_pos[2] );
    glMultMatrixf( view_rotate );

if (showLight == 1)
{
if (head1aS == 1) drawLight ( head1a, 1.0, 1.0, 0.8 );
if (head1bS == 1) drawLight ( head1b, 1.0, 1.0, 0.8 );
if (head2aS == 1) drawLight ( head2a, 1.0, 1.0, 0.6 );
if (head2bS == 1) drawLight ( head2b, 1.0, 1.0, 0.6 );          
if (rearaS == 1) drawLight ( reara, 0.8, 0.0, 0.0 );
if (rearbS == 1) drawLight ( rearb, 0.8, 0.0, 0.0 );
if (rearcS == 1) drawLight ( rearc, 0.8, 0.0, 0.0 );
if (reardS == 1) drawLight ( reard, 0.8, 0.0, 0.0 );  
if (brakeaS == 1) drawLight ( brakea, 1.0, 0.2, 0.0 );
if (brakebS == 1) drawLight ( brakeb, 1.0, 0.2, 0.0 );
if (brakecS == 1) drawLight ( brakec, 1.0, 0.2, 0.0 );
if (brakedS == 1) drawLight ( braked, 1.0, 0.2, 0.0 );  
if (brake2aS == 1) drawLight ( brake2a, 1.0, 0.0, 0.0 );
if (brake2bS == 1) drawLight ( brake2b, 1.0, 0.0, 0.0 );
}

if (showFlames == 1)
{
    if (flameS == 1)
    {
    drawFlame ( flame1, 1.0, 0.2, 0.2 );
    drawFlame ( flame2, 1.0, 0.2, 0.2 );
    }    
}
 
if (showPhysics == 1)
{        
 glDisable(GL_LIGHTING);
 
 ///////////////////
 // Driver position 
 ///////////////////
 
    glPushMatrix();
    glColor3f(1.,0.,0.);
    glTranslatef(driverPosition[0],0,0); //x
    glTranslatef(0,0,driverPosition[1]);//y
    glTranslatef(0,driverPosition[2],0); //z
    glutSolidCube( 0.07 );
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.,0.,1.);
    glTranslatef(bonnetPosition[0],0,0); //x
    glTranslatef(0,0,bonnetPosition[1]);//y
    glTranslatef(0,bonnetPosition[2],0); //z
    glutSolidCube( 0.07 );
    glPopMatrix();             
     //////////////////////////////
    //  Colision bounding box  //
    /////////////////////////////  
    // float carBodyDimensions[3] = {4.683,1.65,1.17};
    glPushMatrix();
    if (solidBox == 0)
    {
    glColor4f (0.1f, 0.8f,0.9f,0.02f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_DEPTH_TEST);
    }
    else
    {
        glColor3f (0.1f, 0.8f,0.9f);
    }        
    
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE); //for double sided
    glEnable(GL_NORMALIZE);
    
    glBegin(GL_QUAD_STRIP);
        //bottom
        glVertex3f(-1*carBodyDimensions[0]/2,0,1*carBodyDimensions[1]/2);
        glVertex3f(1*carBodyDimensions[0]/2,0,1*carBodyDimensions[1]/2);  
        glVertex3f(-1*carBodyDimensions[0]/2,0,-1*carBodyDimensions[1]/2);
        glVertex3f(1*carBodyDimensions[0]/2,0,-1*carBodyDimensions[1]/2);
        //lateral   
        glVertex3f(-1*carBodyDimensions[0]/2,carBodyDimensions[2],-1*carBodyDimensions[1]/2);
        glVertex3f(1*carBodyDimensions[0]/2,carBodyDimensions[2],-1*carBodyDimensions[1]/2);
        //top
        glVertex3f(-1*carBodyDimensions[0]/2,carBodyDimensions[2],1*carBodyDimensions[1]/2);
        glVertex3f(1*carBodyDimensions[0]/2,carBodyDimensions[2],1*carBodyDimensions[1]/2);
        //lateral
        glVertex3f(-1*carBodyDimensions[0]/2,0,1*carBodyDimensions[1]/2);
        glVertex3f(1*carBodyDimensions[0]/2,0,1*carBodyDimensions[1]/2);
    glEnd();
    glBegin(GL_QUADS);
        //front
        glVertex3f(1*carBodyDimensions[0]/2,0,1*carBodyDimensions[1]/2);
        glVertex3f(1*carBodyDimensions[0]/2,carBodyDimensions[2],1*carBodyDimensions[1]/2); 
        glVertex3f(1*carBodyDimensions[0]/2,carBodyDimensions[2],-1*carBodyDimensions[1]/2);
        glVertex3f(1*carBodyDimensions[0]/2,0,-1*carBodyDimensions[1]/2); 
    glEnd();
    glBegin(GL_QUADS);
        //rear
        glVertex3f(-1*carBodyDimensions[0]/2,0,1*carBodyDimensions[1]/2);
        glVertex3f(-1*carBodyDimensions[0]/2,carBodyDimensions[2],1*carBodyDimensions[1]/2); 
        glVertex3f(-1*carBodyDimensions[0]/2,carBodyDimensions[2],-1*carBodyDimensions[1]/2);
        glVertex3f(-1*carBodyDimensions[0]/2,0,-1*carBodyDimensions[1]/2); 
    glEnd();
    
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();

    //////////////
    //  wings  //
    //////////////
    //    float frontwing[4] = {0.45,10.0,2.0,0.1};
    //    float rearwing[4] = {0.9,8.0,-2.3,0.6};
    

    glPushMatrix();
    glColor4f (0.1f, 0.8f,0.2f,0.1f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   // glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE); //for double sided
    glEnable(GL_NORMALIZE);
    
    //front
    float fWingL = 1.3;
    float fWingW = 0.5;
    fWingW = frontwing[0]/fWingL;
    float fWingA = frontwing[1]*3.141592654/180.0;
    glBegin(GL_QUADS);
        glVertex3f(frontwing[2],frontwing[3],-1*fWingL/2);
        glVertex3f(frontwing[2],frontwing[3],1*fWingL/2);
        glVertex3f(frontwing[2]-fWingW,frontwing[3]+fWingW*sin(fWingA),1*fWingL/2);
        glVertex3f(frontwing[2]-fWingW,frontwing[3]+fWingW*sin(fWingA),-1*fWingL/2);
    glEnd();
    
    //rear
    float rWingL = 1.3;
    float rWingW = 0.5;
    rWingW = rearwing[0]/rWingL;
    float rWingA = rearwing[1]*3.141592654/180.0;
    glBegin(GL_QUADS);
        glVertex3f(rearwing[2],rearwing[3]+rWingW*sin(rWingA),-1*rWingL/2);
        glVertex3f(rearwing[2],rearwing[3]+rWingW*sin(rWingA),1*rWingL/2);
        glVertex3f(rearwing[2]+rWingW,rearwing[3],1*rWingL/2);
        glVertex3f(rearwing[2]+rWingW,rearwing[3],-1*rWingL/2);
    glEnd();
    
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    
    
    //////////////
    //  wheels  //
    //////////////
    //float wheel1[12] = {-0.77,16,355,0.44,1.2200,50,0,-2,21,80,0.02,1.8};
    //wheel->radius = rimdiam / 2.0f + tirewidth * tireratio;
    //float	wheelRadius = car->_rimRadius(wheel_index) + car->_tireHeight(wheel_index);
        

    glPushMatrix();
    float wRadius = (wheel1[1]*0.0254/2.0)+(wheel1[2]*wheel1[3]/1000.0);
    glTranslatef(frontaxle[0],0,0); //x
    glTranslatef(0,0,wheel1[0]);//y
    glTranslatef(0,wRadius,0); //z
    glColor3f (0.0f, 0.0f,0.9f);
    glutSolidCube( 0.03 );
    glPopMatrix();
        
    glPushMatrix();
    wRadius = (wheel3[1]*0.0254/2.0)+(wheel3[2]*wheel3[3]/1000.0);
    glTranslatef(rearaxle[0],0,0); //x
    glTranslatef(0,0,wheel3[0]);//y
    glTranslatef(0,wRadius,0); //z
    glColor3f (0.0f, 0.0f,0.9f);
    glutSolidCube( 0.03 );
    glPopMatrix();
   
    glPushMatrix();
    wRadius = (wheel2[1]*0.0254/2.0)+(wheel2[2]*wheel2[3]/1000.0);
    glTranslatef(frontaxle[0],0,0); //x
    glTranslatef(0,0,wheel2[0]);//y
    glTranslatef(0,wRadius,0); //z
    glColor3f (0.0f, 0.0f,0.9f);
    glutSolidCube( 0.03 );
    glPopMatrix();
        
    glPushMatrix();
    wRadius = (wheel4[1]*0.0254/2.0)+(wheel4[2]*wheel4[3]/1000.0);
    glTranslatef(rearaxle[0],0,0); //x
    glTranslatef(0,0,wheel4[0]);//y
    glTranslatef(0,wRadius,0); //z
    glColor3f (0.0f, 0.0f,0.9f);
    glutSolidCube( 0.03 );
    glPopMatrix();
    

     //////////////
    //  AXLE  //
    //////////////   

//front
    glPushMatrix();
    glColor3f(1.,0.,0.);
    glTranslatef(frontaxle[0],0,0); //x
    glTranslatef(0,0,0);//y
    glTranslatef(0,frontaxle[2],0); //z
    glScalef(1,1,110);
    glutSolidCube( 0.02 );
    glPopMatrix();

//rear
    glPushMatrix();
    glColor3f(1.,0.,0.);
    glTranslatef(rearaxle[0],0,0); //x
    glTranslatef(0,0,0);//y
    glTranslatef(0,rearaxle[2],0); //z
    glScalef(1,1,110);
    glutSolidCube( 0.02 );
    glPopMatrix();

 
    glEnable(GL_LIGHTING);   
    /*int i;
    for (i=0;i<9;i++)
    cout << view_rotate[i] << " ";
    cout <<"***********" << endl ;*/
}

    if (showWheels ==1)
    {
    drawWheel ( wheel1,1,-1 );
    drawWheel ( wheel2,1,1 );
    drawWheel ( wheel3,0,-1 );
    drawWheel ( wheel4,0,1 );
    }      

    if (showCar == 1)
    { 
        glCallList(display_list);
    }    

    /* draw on screen car model info */
    std::string acFileName ="";
    
    if ( ChdirCar == 1)
    {         
        acFileName ="Loaded model ../cars/";
        acFileName += carname;
        acFileName += "/";
        acFileName += carRange1;
    }
    else
    {
        acFileName ="Can't load model ../cars/";
        acFileName += carname;
        acFileName += "/";
        acFileName += carRange1;
        acFileName += " - Loaded ac3dLoader/test.ac";
    }    
        
        glDisable( GL_LIGHTING );  /* Disable lighting while we render text */
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D( 0.0, 100.0, 0.0, 100.0  );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glColor3f (0.5, 0.0, 0.);
        glRasterPos2f ( 1.0, 98.0  );
        for (unsigned int i=0; i<acFileName.length(); ++i)
              glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, acFileName[i] );
        glEnable(GL_LIGHTING);




    glShadeModel(GL_SMOOTH);
    glFlush();
    glFinish();
    glutSwapBuffers();
    

    /*frametime = timer_stop();
    tottime += frametime;
    framec++;
    if (tottime > 5.0)
    {
	printf("approx frames per sec: %f\n", 1.0/(tottime/framec));
	tottime = 0;
	framec = 0;
    }

    rot = rot + 2;*/     
    
}

void reshape_gfx(int w, int h)
{
    window_width = w; window_height = h;
    glViewport(0, 0, w, h);
    set_projection(w,h);

}

void hideMainAC3Dloader ( void )
{   

    //gluiLoadAC3D -> close();
    //gluiLoadAC3D2-> close();
    //gluiLoadAC3D->hide();
    //glutHideWindow();
    //cout << "main_window_AC3D: " << main_window_AC3D <<endl;  
    //glutDestroyWindow( main_window_AC3D );
    glutSetWindow( main_window_AC3D );
    glutHideWindow();
    //glutIconifyWindow();

}

void hideShowGluiLoadAC3D2 ( void )
{  
    if ( ShowGluiLoadAC3D2 == 0)
    {
    ShowGluiLoadAC3D2 = 1;
    gluiLoadAC3D2->show();
    }    
    else 
    {
    ShowGluiLoadAC3D2 = 0;
    gluiLoadAC3D2->hide();
    }    
}    

int mainAC3Dloader( int ac3d )
{

    
    ACObject *ob;
    std::string acFilePath ="";
    std::string acFileName ="";
    acFileName ="../cars/";
    acFileName += carname;
    acFilePath = acFileName;
    acFileName += "/";
    acFileName += carRange1;
    
    cout << "chdir " << acFilePath << endl;
    cout << "Loading " <<  acFileName << endl;
    
    
    /* check if we can chdir*/
    FILE *fTest = fopen(acFileName.c_str(), "r");

    if (fTest == NULL)
    {
        fTest = NULL;
        ChdirCar = 0;
    }
    if (ChdirCar == 1)
        fclose(fTest);
    
    /* */
    
    if (ChdirCar == 1)
    chdir(acFilePath.c_str()); //the ac files and rgb are in this dir
    
    if (ChdirCar == 0)
    chdir("ac3dLoader"); //the ac files and rgb are in this dir

    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    main_window_AC3D = glutCreateWindow("AC3D loader");
    //cout << "main_window_AC3D: " << main_window_AC3D <<endl;
    glutReshapeFunc(reshape_gfx);
    glutDisplayFunc(drawscene);
    glutMotionFunc( myGlutMotion_AC3D );
    //GLUI_Master.set_glutIdleFunc( myGlutIdle2 );
    //glutIdleFunc(myGlutIdle2);

    init_gfx();
    
    //if ( argc > 1 ) acFileName = argv[argc];

    ob = ac_load_ac3d(carRange1);

    display_list = ac_display_list_render_object(ob);
    
    if (ChdirCar == 1)
    chdir("../../torcs-car-setup-editor"); // return to tcse dir
    
    if (ChdirCar == 0)
    chdir("../"); // return to tcse dir



  printf( "GLUI version: %3.2f\n", GLUI_Master.get_version() );


    
      /*** Create the right subwindow ***/
  gluiLoadAC3D2 = GLUI_Master.create_glui_subwindow( main_window_AC3D, 
					    GLUI_SUBWINDOW_RIGHT );
 gluiLoadAC3D2->set_main_gfx_window( main_window_AC3D );
 gluiLoadAC3D2->hide();
  
  GLUI_Panel *wheelsPanel = new GLUI_Rollout(gluiLoadAC3D2, "Setup Wheels",false);
  //GLUI_Panel *wheel2_panel = new GLUI_Panel(gluiLoadAC3D2, "Front Left Wheel");
  GLUI_Panel *wheel2_panel = new GLUI_Rollout(wheelsPanel, "Front Left Wheel",false);
   (new GLUI_Spinner( wheel2_panel, "Ypos(m)", &wheel2[0], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel2_panel, "Rim diameter(in)", &wheel2[1], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 30 );
   (new GLUI_Spinner( wheel2_panel, "Tire Width(mm)", &wheel2[2], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( wheel2_panel, "Tire height-width ratio", &wheel2[3], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel2_panel, "Inertia(kg.m2)", &wheel2[4], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel2_panel, "Initial Ride Height(mm)", &wheel2[5], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 300 );
   (new GLUI_Spinner( wheel2_panel, "Toe(deg)", &wheel2[6], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel2_panel, "Camber(deg)", &wheel2[7], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( -5, 0 );
   (new GLUI_Spinner( wheel2_panel, "Stiffness", &wheel2[8], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 20, 50 );
   (new GLUI_Spinner( wheel2_panel, "Dynamic Fricction(%)", &wheel2[9], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 70, 90 );
   (new GLUI_Spinner( wheel2_panel, "Rolling Resistance", &wheel2[10], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( wheel2_panel, "Mu", &wheel2[11], 2, (GLUI_Update_CB)SimetricalEditL  ))
    ->set_float_limits( 0.05, 1.8 ); 
         


  GLUI_Panel *wheel4_panel = new GLUI_Rollout(wheelsPanel, "Rear Left Wheel",false);
   (new GLUI_Spinner( wheel4_panel, "Ypos(m)", &wheel4[0], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel4_panel, "Rim diameter(in)", &wheel4[1], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 30 );
   (new GLUI_Spinner( wheel4_panel, "Tire Width(mm)", &wheel4[2], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( wheel4_panel, "Tire height-width ratio", &wheel4[3], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel4_panel, "Inertia(kg.m2)", &wheel4[4], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel4_panel, "Initial Ride Height(mm)", &wheel4[5], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 300 );
   (new GLUI_Spinner( wheel4_panel, "Toe(deg)", &wheel4[6], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel4_panel, "Camber(deg)", &wheel4[7], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( -5, 0 );
   (new GLUI_Spinner( wheel4_panel, "Stiffness", &wheel4[8], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 20, 50 );
   (new GLUI_Spinner( wheel4_panel, "Dynamic Fricction(%)", &wheel4[9], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 70, 90 );
   (new GLUI_Spinner( wheel4_panel, "Rolling Resistance", &wheel4[10], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( wheel4_panel, "Mu", &wheel4[11], 2, (GLUI_Update_CB)SimetricalEditL   ))
    ->set_float_limits( 0.05, 1.8 );

  //new GLUI_Column( gluiLoadAC3D2, false );  

  GLUI_Panel *wheel1_panel = new GLUI_Rollout(wheelsPanel, "Front Rigth Wheel",false);
   (new GLUI_Spinner( wheel1_panel, "Ypos(m)", &wheel1[0], 2, (GLUI_Update_CB)SimetricalEditR    ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel1_panel, "Rim diameter(in)", &wheel1[1], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 30 );
   (new GLUI_Spinner( wheel1_panel, "Tire Width(mm)", &wheel1[2], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( wheel1_panel, "Tire height-width ratio", &wheel1[3], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel1_panel, "Inertia(kg.m2)", &wheel1[4], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel1_panel, "Initial Ride Height(mm)", &wheel1[5], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 300 );
   (new GLUI_Spinner( wheel1_panel, "Toe(deg)", &wheel1[6], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel1_panel, "Camber(deg)", &wheel1[7], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 0 );
   (new GLUI_Spinner( wheel1_panel, "Stiffness", &wheel1[8], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 20, 50 );
   (new GLUI_Spinner( wheel1_panel, "Dynamic Fricction(%)", &wheel1[9], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 70, 90 );
   (new GLUI_Spinner( wheel1_panel, "Rolling Resistance", &wheel1[10], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( wheel1_panel, "Mu", &wheel1[11], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0.05, 1.8 ); 

  GLUI_Panel *wheel3_panel = new GLUI_Rollout(wheelsPanel, "Rear Right Wheel",false);
   (new GLUI_Spinner( wheel3_panel, "Ypos(m)", &wheel3[0], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel3_panel, "Rim diameter(in)", &wheel3[1], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 30 );
   (new GLUI_Spinner( wheel3_panel, "Tire Width(mm)", &wheel3[2], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1000 );
   (new GLUI_Spinner( wheel3_panel, "Tire height-width ratio", &wheel3[3], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel3_panel, "Inertia(kg.m2)", &wheel3[4], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 5 );
   (new GLUI_Spinner( wheel3_panel, "Initial Ride Height(mm)", &wheel3[5], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 300 );
   (new GLUI_Spinner( wheel3_panel, "Toe(deg)", &wheel3[6], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 5 );
   (new GLUI_Spinner( wheel3_panel, "Camber(deg)", &wheel3[7], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( -5, 0 );
   (new GLUI_Spinner( wheel3_panel, "Stiffness", &wheel3[8], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 20, 50 );
   (new GLUI_Spinner( wheel3_panel, "Dynamic Fricction(%)", &wheel3[9], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 70, 90 );
   (new GLUI_Spinner( wheel3_panel, "Rolling Resistance", &wheel3[10], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0, 1 );
   (new GLUI_Spinner( wheel3_panel, "Mu", &wheel3[11], 2, (GLUI_Update_CB)SimetricalEditR ))
    ->set_float_limits( 0.05, 1.8 );
    
   new GLUI_Checkbox(wheelsPanel, "Simetrical edit", &simetricaledityn2, 2, (GLUI_Update_CB)SimetricalEditL );

    //new GLUI_StaticText( gluiLoadAC3D2, "Aerodynamics & wings" );      
   GLUI_Panel *aerodynamics_wings_panel = new GLUI_Rollout(gluiLoadAC3D2, "Aerodynamics & wings",false);     
  GLUI_Panel *aerodynamics_panel = new GLUI_Rollout( aerodynamics_wings_panel, "Aerodynamics", false );
   (new GLUI_Spinner( aerodynamics_panel, "Cx:", &aerodynamics[0] ))
    ->set_float_limits( 0, 1 );
    (new GLUI_Spinner( aerodynamics_panel, "Front Area(m2):", &aerodynamics[1] ))
    ->set_float_limits( 0, 20 );
    (new GLUI_Spinner( aerodynamics_panel, "Front Clift:", &aerodynamics[2] ))
    ->set_float_limits( 0, 5 );
    (new GLUI_Spinner( aerodynamics_panel, "Rear Clift:", &aerodynamics[3] ))
    ->set_float_limits( 0, 5 );
    
  GLUI_Panel *frontwing_panel = new GLUI_Rollout( aerodynamics_wings_panel, "Front Wing", false );
   (new GLUI_Spinner( frontwing_panel, "Area(m2):", &frontwing[0] ))
    ->set_float_limits( 0, 10 );
    (new GLUI_Spinner( frontwing_panel, "angle(deg):", &frontwing[1] ))
    ->set_float_limits( 0, 180 );
    (new GLUI_Spinner( frontwing_panel, "xpos(m):", &frontwing[2] ))
    ->set_float_limits( -5, 5 );
    (new GLUI_Spinner( frontwing_panel, "zpos(m):", &frontwing[3] ))
    ->set_float_limits( 0, 5 );
    
  GLUI_Panel *rearwing_panel = new GLUI_Rollout( aerodynamics_wings_panel, "Rear Wing", false );
   (new GLUI_Spinner( rearwing_panel, "Area(m2):", &rearwing[0] ))
    ->set_float_limits( 0, 10 );
    (new GLUI_Spinner( rearwing_panel, "angle(deg):", &rearwing[1] ))
    ->set_float_limits( 0, 180 );
    (new GLUI_Spinner( rearwing_panel, "xpos(m):", &rearwing[2] ))
    ->set_float_limits( -5, 5 );
    (new GLUI_Spinner( rearwing_panel, "zpos(m):", &rearwing[3] ))
    ->set_float_limits( 0, 5 ); 



  GLUI_Panel *axle_panel = new GLUI_Rollout( gluiLoadAC3D2, "Axle & Steer", false );
  GLUI_Panel *steer_panel = new GLUI_Rollout(axle_panel, "Steer", false);
   (new GLUI_Spinner( steer_panel, "Steer Lock (deg)", &steer[0] ))
    ->set_float_limits( 1, 45 );
   (new GLUI_Spinner( steer_panel, "Max Steer Speed (deg/s)", &steer[1] ))
    ->set_float_limits( 1, 360 );

  GLUI_Panel *frontaxle_panel = new GLUI_Rollout(axle_panel, "Front Axle",false);
  frontaxle_panel->set_alignment( GLUI_ALIGN_RIGHT );
   (new GLUI_Spinner( frontaxle_panel, "xpos (m)", &frontaxle[0] ))
    ->set_float_limits( -3, 3 );
   (new GLUI_Spinner( frontaxle_panel, "Inertia (kg.m2)", &frontaxle[1] ))
    ->set_float_limits( -3, 3 );
   (new GLUI_Spinner( frontaxle_panel, "Roll Center Height (m)", &frontaxle[2] ))
    ->set_float_limits( 0, 1 );

  GLUI_Panel *rearaxle_panel = new GLUI_Rollout(axle_panel, "Rear Axle",false);
  rearaxle_panel->set_alignment( GLUI_ALIGN_RIGHT );  
   (new GLUI_Spinner( rearaxle_panel, "xpos (m)", &rearaxle[0] ))
    ->set_float_limits( -3, 3 );
   (new GLUI_Spinner( rearaxle_panel, "Inertia (kg.m2)", &rearaxle[1] ))
    ->set_float_limits( -3, 3 );
   (new GLUI_Spinner( rearaxle_panel, "Roll Center Height (m)", &rearaxle[2] ))
    ->set_float_limits( 0, 1 );

  GLUI_Panel *driverPosition_panel2 = new GLUI_Rollout( gluiLoadAC3D2, "Driver&Bonnet Pos", false );
  GLUI_Panel *driver_position_panel = new GLUI_Rollout( driverPosition_panel2, "Driver Position", false );
   (new GLUI_Spinner( driver_position_panel, "x pos (m)", &driverPosition[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( driver_position_panel, "y pos (m)", &driverPosition[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( driver_position_panel, "z pos (m)", &driverPosition[2]))
    ->set_float_limits( -10, 10 );
    
  GLUI_Panel *bonnet_position_panel = new GLUI_Panel( driverPosition_panel2, "Bonnet Position" );
   (new GLUI_Spinner( bonnet_position_panel, "x pos (m)", &bonnetPosition[0]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( bonnet_position_panel, "y pos (m)", &bonnetPosition[1]))
    ->set_float_limits( -10, 10 );
   (new GLUI_Spinner( bonnet_position_panel, "z pos (m)", &bonnetPosition[2]))
    ->set_float_limits( -10, 10 );
  
  GLUI_Panel *carDimensions = new GLUI_Rollout( gluiLoadAC3D2, "Car Body Dimensions", false );
  GLUI_Panel *carBodyDimensions_panel = new GLUI_Rollout( carDimensions, "Car Body Dimensions", false );
   (new GLUI_Spinner( carBodyDimensions_panel, "Body length (m)", &carBodyDimensions[0]))
    ->set_float_limits( 0, 20 );
   (new GLUI_Spinner( carBodyDimensions_panel, "Body width (m)", &carBodyDimensions[1]))
    ->set_float_limits( 0, 20 );
   (new GLUI_Spinner( carBodyDimensions_panel, "Body height (m)", &carBodyDimensions[2]))
    ->set_float_limits( 0, 20 );
    
  GLUI_Panel *carOverallDimensions_panel = new GLUI_Rollout( carDimensions, "Car Body Dimensions", false );
   (new GLUI_Spinner( carOverallDimensions_panel, "Body length (m)", &carOverallDimensions[0]))
    ->set_float_limits( 0, 20 );
   (new GLUI_Spinner( carOverallDimensions_panel, "Body width (m)", &carOverallDimensions[1]))
    ->set_float_limits( 0, 20 );
    
    new GLUI_Checkbox(carDimensions, "Solid Box", &solidBox, 0, (GLUI_Update_CB)sincAll );

    //new GLUI_Column( gluiLoadAC3D2, false );
  /*** Create the right subwindow end***/



  /*** Create the bottom subwindow ***/
  gluiLoadAC3D = GLUI_Master.create_glui_subwindow( main_window_AC3D, 
					    GLUI_SUBWINDOW_BOTTOM );
 gluiLoadAC3D->set_main_gfx_window( main_window_AC3D );
 
  GLUI_Rotation *view_rot = new GLUI_Rotation(gluiLoadAC3D, "Rotate", view_rotate);
  view_rot->set_spin( 1.0 );
  new GLUI_Column( gluiLoadAC3D, false );
  GLUI_Translation *trans_xy = 
    new GLUI_Translation(gluiLoadAC3D, "Objects XY", GLUI_TRANSLATION_XY, obj_pos );
  trans_xy->set_speed( .005 );
  new GLUI_Column( gluiLoadAC3D, false );
  GLUI_Translation *trans_x = 
    new GLUI_Translation(gluiLoadAC3D, "Objects X", GLUI_TRANSLATION_X, obj_pos );
  trans_x->set_speed( .005 );
  new GLUI_Column( gluiLoadAC3D, false );
  GLUI_Translation *trans_y = 
    new GLUI_Translation( gluiLoadAC3D, "Objects Y", GLUI_TRANSLATION_Y, &obj_pos[1] );
  trans_y->set_speed( .005 );
  new GLUI_Column( gluiLoadAC3D, false );
  GLUI_Translation *trans_z = 
    new GLUI_Translation( gluiLoadAC3D, "Objects Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
  trans_z->set_speed( .005 );
  new GLUI_Column( gluiLoadAC3D, false );
  new GLUI_Button( gluiLoadAC3D , "Close", 0,(GLUI_Update_CB)hideMainAC3Dloader );
  new GLUI_Checkbox(gluiLoadAC3D, "Show Wheels", &showWheels, 1, (GLUI_Update_CB)sincAll );
  new GLUI_Checkbox(gluiLoadAC3D, "Show Phisics", &showPhysics, 1, (GLUI_Update_CB)sincAll );
  new GLUI_Checkbox(gluiLoadAC3D, "Show Car", &showCar, 1, (GLUI_Update_CB)sincAll );
  new GLUI_Column( gluiLoadAC3D, false );
  new GLUI_Button( gluiLoadAC3D , "SideWindow", 0,(GLUI_Update_CB)hideShowGluiLoadAC3D2 );
  new GLUI_Checkbox(gluiLoadAC3D, "Show Light", &showLight, 1, (GLUI_Update_CB)sincAll );
  new GLUI_Checkbox(gluiLoadAC3D, "Show Flames", &showFlames, 1, (GLUI_Update_CB)sincAll );
    /*** Create the bottom subwindow end ***/





    glutMainLoop();
    
    return 0;
}
