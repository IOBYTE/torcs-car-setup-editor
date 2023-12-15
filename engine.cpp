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

#include "cardata.h"

#define TAM 100

float CVW = 735.49875;
/* cv and hp to w
#define CVW 735.49875
#define HPW 745.69872
*/

extern float tqmax;
extern float cvmax;
extern float cvmaxY;
extern float rpmattqmax;
extern float rpmatcvmax;
extern float rpmatcvmaxY;
extern float revsmax;
extern float cvmaxNew;
extern float tqmaxNew;
extern float tqctNew;
extern float cvctNew;


void gearboxRatioSpeed ( int k );

/* calculation of the number of points before the revs limiter */
int calcEnginePointsLesRL (void)
{
    int i=0;
    int points = 21;
    
    while (i < 22)
    {
        if ( rpmValue[i] >= engineparams[2] )
            {
              points = i;
              return points;
              break;
            }
        i++;
    }
    return points;    
}    

/* calculation of the number of points before the revs maxi */
int calcEnginePointsLesRM (void)
{
    int i=0;
    int points = 21;
    
    while (i < 22)
    {
        if ( rpmValue[i] >= engineparams[1] )
            {
              points = i;
              return points;
              break;
            }
        i++;
    }
    return points;    
} 

/* interpolating tq and hp data*/
float interpolationTQCV (float vectx[50], float vecty[50], float vectx0)
{
    float vecty0;
    int i = 0;
    while (i < 20)
    {
        if (vectx0 >= vectx[i] && vectx0 <= vectx[i+1]) 
        {
            vecty0 = vecty[i] + ( (vecty[i+1]-vecty[i]) / (vectx[i+1]-vectx[i]+0.000000001) ) * (vectx0-vectx[i]);
            break;
        }
        if (vectx0 == vectx[i]) 
        {
            vecty0 = vecty[i];
            break;
        }        
        i++;
    }
    
    return vecty0;
} 

float interpolationTQCV (float vectx[50], float vecty[50], float vectx0, int imax)
{
    float vecty0;
    int i = 0 ;
    while (i < imax)
    {
        if (vectx0 >= vectx[i] && vectx0 <= vectx[i+1]) 
        {
            vecty0 = vecty[i] + ( (vecty[i+1]-vecty[i]) / (vectx[i+1]-vectx[i]+0.000000001) ) * (vectx0-vectx[i]);
            break;
        }
        if (vectx0 == vectx[i]) 
        {
            vecty0 = vecty[i];
            break;
        }        
        i++;
    }
    
    return vecty0;
}

/*---------------------------Max Engine--------------------------------*/

float CalcMaxEngine(float vector[TAM], float vector2[TAM],int init, int tamv, float &revs, int real)
{
     float min;
     float max;
     float suma;
     int imax;
     int i;
     max=vector[init];
     min=max;
     suma=max;

     for (i = init; i < (tamv+1); i++)
     {
         suma=suma + vector[i];
        if(vector[i]>max)
         {
          max = vector[i];
          revs = vector2[i];
          imax = i;
         }
         else if (vector[i]<min)
         {
          min = vector[i];
         }
     }
     
     if (real==1)
     {
         int points = calcEnginePointsLesRL ();
         
         if (  revs > engineparams[2] )
         {   
             revs = engineparams[2];
                     
                int i = 0 ;
                while (i < 20)
                {
                    if (engineparams[2] >= vector2[i] && engineparams[2] <= vector2[i+1]) 
                    {
                        max=vector[i]+((vector[i+1]-vector[i])/(vector2[i+1]-vector2[i]))*(engineparams[2]-vector2[i]);
                        break;
                    }
                           
                    i++;
                }
        
         }    
     }
    
             
     return max;
}
/*---------------------------Max Engine---------------------------------*/

/*---------------------------Max---------------------------------*/

float CalcMax(float vector[TAM], int tamv)
{
     float min;
     float max;
     float suma;
     int i;
     max=vector[0];
     min=max;
     suma=max;

     for (i=0; i < tamv; i++)
     {
         suma=suma + vector[i];
        if(vector[i]>max)
         {
          max = vector[i];
         }
         else if (vector[i]<min)
         {
          min=vector[i];
         }
     }
     return max;
}
/*---------------------------Max---------------------------------*/
 void gearMaxMin( int i )
 {
     switch ( i )
     {
         case 0:  break;
         case 1:  if (gearboxratio[i] < gearboxratio[i+1]) 
                  gearboxratio[i] = gearboxratio[i+1]+0.001;
                  break;
         case 2:  if (gearboxratio[i] < gearboxratio[i+1]) 
                  gearboxratio[i] = gearboxratio[i+1]+0.001;
                  if (gearboxratio[i] > gearboxratio[i-1]) 
                  gearboxratio[i] = gearboxratio[i-1]-0.001;
                  break;
         case 3:  if (gearboxratio[i] < gearboxratio[i+1]) 
                  gearboxratio[i] = gearboxratio[i+1]+0.001;
                  if (gearboxratio[i] > gearboxratio[i-1]) 
                  gearboxratio[i] = gearboxratio[i-1]-0.001;
                  break;
         case 4:  if (gearboxratio[i] < gearboxratio[i+1]) 
                  gearboxratio[i] = gearboxratio[i+1]+0.001;
                  if (gearboxratio[i] > gearboxratio[i-1]) 
                  gearboxratio[i] = gearboxratio[i-1]-0.001;
                  break;
         case 5:  if (gearboxratio[i] < gearboxratio[i+1]) 
                  gearboxratio[i] = gearboxratio[i+1]+0.001;
                  if (gearboxratio[i] > gearboxratio[i-1]) 
                  gearboxratio[i] = gearboxratio[i-1]-0.001;
                  break;
         case 6:  if (gearboxratio[i] < gearboxratio[i+1]) 
                  gearboxratio[i] = gearboxratio[i+1]+0.001;
                  if (gearboxratio[i] > gearboxratio[i-1]) 
                  gearboxratio[i] = gearboxratio[i-1]-0.001;
                  break;
         case 7:  if (gearboxratio[i] > gearboxratio[i-1]) 
                  gearboxratio[i] = gearboxratio[i-1]-0.001;
                  break;
     }
     
     gearboxRatioSpeed ( 1 );
     GLUI_Master.sync_live_all();    
 } 

 /* rpm  */

 void CalcRPM( int i )
 {
  if (i == 0)
    if (rpmValue[i] > rpmValue[i+1]) rpmValue[i] = rpmValue[i+1]-1;
  if (i == 20)
    if (rpmValue[i] < rpmValue[i-1]) rpmValue[i] = rpmValue[i-1]+1;
  if ( i > 0 && i < 20 )
    {
      if (rpmValue[i] > rpmValue[i+1]) rpmValue[i] = rpmValue[i+1]-1;
      if (rpmValue[i] < rpmValue[i-1]) rpmValue[i] = rpmValue[i-1]+1;
    }       
    GLUI_Master.sync_live_all();    
 }    





 void CalcCV(void)
 {
        int i;
        cvValue[0] = 0.0;   
                
        for (i=0; i < 20; i++)
        {
            if (rpmValue[i] > rpmValue[i+1])
            { 
              rpmValue[i] = rpmValue[i+1]-1;
            }
        }
        for (i=1; i < 21; i++)
        {
            if (rpmValue[i] < rpmValue[i-1])
            { 
              rpmValue[i] = rpmValue[i-1]+1;
            }
        }

        for (i=1; i < 21; i++)
        {
            cvValue[i] = tqValue[i]*rpmValue[i]*2*3.141592654/(60*CVW);
        }
        
        /* calc the max values of cv and tq under the revs lim */
        int points = calcEnginePointsLesRL ();
        
        cvmaxY = CalcMaxEngine(cvValue,rpmValue,1,21,rpmatcvmaxY,0);
        revsmax = CalcMax(rpmValue,21);
        cvmax = CalcMaxEngine(cvValue,rpmValue,1,points,rpmatcvmax,1);
        tqmax = CalcMaxEngine(tqValue,rpmValue,1,points,rpmattqmax,1);
        
        GLUI_Master.sync_live_all();
 }

void CalcCVTQmax ( void )
{

        /* calc the max values of cv and tq under the revs lim */
        int points = calcEnginePointsLesRL ();
        
        cvmaxY = CalcMaxEngine(cvValue,rpmValue,1,21,rpmatcvmaxY,0);
        revsmax = CalcMax(rpmValue,21);
        cvmax = CalcMaxEngine(cvValue,rpmValue,1,points,rpmatcvmax,1);
        tqmax = CalcMaxEngine(tqValue,rpmValue,1,points,rpmattqmax,1);
        
        GLUI_Master.sync_live_all();
    
}    

 void CalcTQ(void)
 {
        tqValue[0]=0.0;
        int i;
        for (i=1; i < 21; i++)
        {
            tqValue[i] = cvValue[i] / (rpmValue[i]*2*3.141592654/(60*CVW));
        }
        /* calc the max values of cv and tq under the revs lim */
        int points = calcEnginePointsLesRL ();
        
        cvmaxY = CalcMaxEngine(cvValue,rpmValue,1,21,rpmatcvmaxY,0);
        revsmax = CalcMax(rpmValue,21);
        cvmax = CalcMaxEngine(cvValue,rpmValue,1,points,rpmatcvmax,1);
        tqmax = CalcMaxEngine(tqValue,rpmValue,1,points,rpmattqmax,1);
        
        GLUI_Master.sync_live_all();
 }

void undoEngine ( void )
{
    GLUI_Master.sync_live_all();
}

void scaleAllEnginePoints ( int k )
{
  int i;
  float cvmax;
  float rpmatcvmax;
  float tqmax;
  float rpmattqmax;
  
  int points = calcEnginePointsLesRM ();
  
switch (k)
{
case 1: /* cv */
  /* calc the max of cv in all the range*/
  cvmax = CalcMaxEngine(cvValue,rpmValue,1,points,rpmatcvmax,0);
  //cout << "cvmax: " << cvmax << "at rpm:"<< rpmatcvmax <<endl;
  /* update all engine points */
  for (i=0;i<21;i++)
  {
     cvValue[i]=cvValue[i]*cvmaxNew/cvmax;
  }
  CalcTQ();    
    
break;

case 2: /* tq */
  /* calc the max of tq in all the range*/
  tqmax = CalcMaxEngine(tqValue,rpmValue,1,points,rpmattqmax,0);
  /* update all engine points */
  for (i=0;i<21;i++)
  {
     tqValue[i]=tqValue[i]*tqmaxNew/tqmax;
  }
  CalcCV();
  
break;

case 3: /* tq */

  /* update all engine points */
  for (i=0;i<21;i++)
  {
     tqValue[i]=tqctNew;
  }
  CalcCV();
  
break;

case 4: /* cv */

  /* update all engine points */
  for (i=0;i<21;i++)
  {
     cvValue[i]=cvctNew;
  }
  CalcTQ();
  
break;

}    

GLUI_Master.sync_live_all();

}    


/*  modifyShapeOfEnginePoints */
void CalcMaxEngine2(float vector[TAM], float vector2[TAM], int &imax)
{
     float min;
     float max;
     float suma;
     float revs;
     int i;
     min=max;
     suma=max;

     for (i = 0; i < 21; i++)
     {
        suma=suma + vector[i];
        if(vector[i]>max)
         {
          max = vector[i];
          revs = vector2[i];
          imax = i;
         }
         else if (vector[i]<min)
         {
          min = vector[i];
         }
     }
}

float cvEngineShape[4]={10.0,15.1289,6.4029,-0.2431};

void modifyShapeOfEnginePoints ( int k )
{
int i;
int cvimax;
int modifyWidth=2;
float factor = 0.0;
int factorCount = 0;
float newCVvalue;

switch ( k )
{
case 1:
    /* calc the cv imax */
    CalcMaxEngine2(cvValue,rpmValue,cvimax);
    if (cvimax>21) cvimax = 21;
    if (cvimax<0) cvimax = 0;
    
    cvValue[0]=0.0;
    
    factorCount = 0;
    for (i=cvimax+1;i<21;i++)
    {        
       cvValue[i]=cvValue[i]+factor;
    }    
    
    factorCount = 0;
    for (i=cvimax;i>0;i--)
    {
       if (factorCount < modifyWidth ){
          factor = -5.0*i;
       }
       else{
          factor = -1.0*i;  
       }
            
       newCVvalue=cvValue[i]+factor;
       
       cvValue[i] = newCVvalue;
       
       ++factorCount;
    }
    
    CalcTQ();
break;

case 2:
    for (i=0;i<21;i++)
    {        
       //cvValue[i]=10.0+15.1289*i+6.4029*pow(i,2)-0.2431*pow(i,3);
       cvValue[i]=cvEngineShape[0]+cvEngineShape[1]*i+cvEngineShape[2]*pow((float)i,2)+cvEngineShape[3]*pow((float)i,3);
       if (cvValue[i]<0.0) cvValue[i]=0.0;
       rpmValue[i]=1000.0*i;
    }
    
    scaleAllEnginePoints ( 1 );
        
    CalcTQ();

break;

case 3: /* change the rpmengine points */
    for (i=0;i<21;i++)
    {        
       rpmValue[i]=engineparams[1]/20.0*i;
    }
break;

}
   
GLUI_Master.sync_live_all();
    
}    

extern GLUI_Spinner *revsLimiterSpinner;
void setRevsLimiterLimits ( void )
{
    revsLimiterSpinner -> set_float_limits( engineparams[3], engineparams[1] );
}    
