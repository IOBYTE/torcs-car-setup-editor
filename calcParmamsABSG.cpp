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
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


#include "cardata.h"

extern CarData cardata;

extern CarData cardata;
extern float tqmax;
extern float cvmax;
extern float rpmattqmax;
extern float rpmatcvmax;
extern float revsmax;

float calcAcceleration ( void )
{
    float param = 0;
    param = (cvmax*500)/(1200*cardata.massdata[6])*100;
    param += (10/(cardata.frontwing[0]*cardata.frontwing[1]+2))*100;
    param += (10/(cardata.rearwing[0]*cardata.rearwing[1]+2))*100;
    param = param/10;
    if (param >100) param = 100;
    return param;
}
 
float calcSpeed ( void )
{   
    
  float wRadius = 0.0;
  float wRadius3 = (cardata.wheel3[1]*0.0254/2.0)+(cardata.wheel3[2]*cardata.wheel3[3]/1000.0);
  float wRadius4 = (cardata.wheel4[1]*0.0254/2.0)+(cardata.wheel4[2]*cardata.wheel4[3]/1000.0);
  ( wRadius3 > wRadius4 ) ? (wRadius = wRadius3) : (wRadius = wRadius4);
  if ( wRadius <= 0.0 ) wRadius = 1.0;
  float finalRatio = 1.0;
  
   if (cardata.curr_drivetrain_type == 0){
   finalRatio = cardata.reardifferential[1]*cardata.gearboxratio[cardata.numberOfGears];
   }
   if (cardata.curr_drivetrain_type == 1){
   finalRatio = cardata.frontdifferential[1]*cardata.gearboxratio[cardata.numberOfGears];
   }
   if (cardata.curr_drivetrain_type == 2){
   finalRatio = cardata.centraldifferential[1]*cardata.gearboxratio[cardata.numberOfGears];
   }
   if ( finalRatio <= 0.0 ) finalRatio = 1.0;
   
   float maxSpeed = cardata.engine.params[2]*60*wRadius*2.0*3.1416*0.001/(finalRatio);
    
    float param = 0;
    param = maxSpeed/4; 
    if (param >100) param = 100;
    return param;
}

float calcGrip ( void )
{
    float param = 0;
    // wheel1[11]--> mu, wheel1[9] --> dinamic friction
    float mu = (cardata.wheel1[11]+cardata.wheel2[11]+cardata.wheel3[11]+cardata.wheel4[11])/4;
    float dinamicFriction = (cardata.wheel1[9]+cardata.wheel2[9]+cardata.wheel3[9]+cardata.wheel4[9])/4;
    param = mu*dinamicFriction/1.62;
    param += (cardata.frontwing[0]*cardata.frontwing[1]/50)*100;
    param += (cardata.rearwing[0]*cardata.rearwing[1]/50)*100;
    param = param/2;
    if (param >100) param = 100;
    return param; 
}

float calcBraking ( void )
{
    float param = 0;
    param = (cardata.brakesystem[0]/0.7)*100;
    param += (cardata.brakesystem[1]/150000.0)*100;
    param = param/2;
    if (param >100) param = 100;
    return param;
}   
