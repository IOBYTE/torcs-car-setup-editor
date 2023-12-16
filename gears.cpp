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

float gearbox_speed[8] = {-1,1,2,3,4,5,6,7};
float gearRed100 = 5;

float bestOptShiftPoint[6]={0.0,0.0,0.0,0.0,0.0,0.0};
float rpmAfterBestOptShiftPoint[6]={0.0,0.0,0.0,0.0,0.0,0.0};
float optShiftPoint[6]={0.0,0.0,0.0,0.0,0.0,0.0};

/* gear spinners*/
extern GLUI_Spinner *r_gear[8];
extern GLUI_Spinner *i_gear[8];
extern GLUI_Spinner *e_gear[8];
extern GLUI_Spinner *s_gear[8];

extern CarData cardata;
extern float gearsAutoInertia;
extern float gearsAutoEff;
extern float rpmatcvmax;

float calcWheelRadius ( int k )
{
  float wRadius = 0.0;

      float wRadius1 = (cardata.wheel1[1]*0.0254/2.0)+(cardata.wheel1[2]*cardata.wheel1[3]/1000.0);
      float wRadius2 = (cardata.wheel2[1]*0.0254/2.0)+(cardata.wheel2[2]*cardata.wheel2[3]/1000.0);
      float wRadius3 = (cardata.wheel3[1]*0.0254/2.0)+(cardata.wheel3[2]*cardata.wheel3[3]/1000.0);
      float wRadius4 = (cardata.wheel4[1]*0.0254/2.0)+(cardata.wheel4[2]*cardata.wheel4[3]/1000.0);

      cardata.wheelRadius[0]=wRadius1;
      cardata.wheelRadius[1]=wRadius2;
      cardata.wheelRadius[2]=wRadius3;
      cardata.wheelRadius[3]=wRadius4;

  /*  char *drivetrain_type[] = { "RWD", "FWD", "4WD" }; */
  if (cardata.curr_drivetrain_type == 0) /* RWD */
  {
      ( wRadius3 > wRadius4 ) ? (wRadius = wRadius3) : (wRadius = wRadius4);
      if ( wRadius <= 0.0 ) wRadius = 1.0;
  }
  else if (cardata.curr_drivetrain_type == 1) /* FWD */
  {
      ( wRadius1 > wRadius2 ) ? (wRadius = wRadius1) : (wRadius = wRadius2);
      if ( wRadius <= 0.0 ) wRadius = 1.0;
  }
  else if (cardata.curr_drivetrain_type == 2) /* 4WD */
  {
      ( wRadius1 > wRadius2 ) ? (wRadius = wRadius1) : (wRadius = wRadius2);
      if (wRadius3 > wRadius) wRadius = wRadius3;
      if (wRadius4 > wRadius) wRadius = wRadius4;
      if ( wRadius <= 0.0 ) wRadius = 1.0;
  }

  return  wRadius;
}



void gearboxRatioSpeed ( int k )
{

  float wRadius = calcWheelRadius ( 1 );
  float finalRatio = 1.0;



int i;

    switch (k)
    {
  case 0:

  for (i = 0; i < 8; i++)
        {
           finalRatio = cardata.engine.params[2]*60*wRadius*2.0*3.1416*0.001 / (gearbox_speed[i]);

           if ( finalRatio == 0.0 ) finalRatio = 1.0;

           if (cardata.curr_drivetrain_type == 0)
           {
           cardata.gearboxratio[i] = finalRatio / cardata.reardifferential[1];
           }
           if (cardata.curr_drivetrain_type == 1)
           {
           cardata.gearboxratio[i] = finalRatio / cardata.frontdifferential[1];
           }
           if (cardata.curr_drivetrain_type == 2)
           {
            cardata.gearboxratio[i] = finalRatio / cardata.centraldifferential[1];
           }
        }

  break;

  case 1:
  for (i = 0; i < 8; i++)
        {
           if (cardata.curr_drivetrain_type == 0)
           {
           finalRatio = cardata.reardifferential[1]*cardata.gearboxratio[i];
           }
           if (cardata.curr_drivetrain_type == 1)
           {
           finalRatio = cardata.frontdifferential[1]*cardata.gearboxratio[i];
           }
           if (cardata.curr_drivetrain_type == 2)
           {
           finalRatio = cardata.centraldifferential[1]*cardata.gearboxratio[i];
           }
           if ( finalRatio == 0.0 ) finalRatio = 1.0;


           gearbox_speed[i] = cardata.engine.params[2]*60*wRadius*2.0*3.1416*0.001/(finalRatio);

        }
        break;
    }

    GLUI_Master.sync_live_all();
}


void gearboxRatioCalcAll ( int k )
{
    gearboxRatioSpeed ( 1 );

    float velRange = 0.0;

    velRange = gearbox_speed[cardata.numberOfGears] - gearbox_speed[1];

    int i;

    float gearRange = 0;

    for (i=0; i<(cardata.numberOfGears-1); i++)
    {
    gearRange += pow((100-gearRed100)/100,i);
    }

    for (i=1; i<cardata.numberOfGears; i++)
    {
    //gearbox_speed[i+1] = gearbox_speed[i] + velRange / (numberOfGears - 1);
    gearbox_speed[i+1]=gearbox_speed[i]+pow((100-gearRed100)/100,(i-1))*velRange /gearRange;
    }

    gearboxRatioSpeed ( 0 );

    GLUI_Master.sync_live_all();
}

void numberOfGearsDisable   ( void )
{
    int k;
    for (k=cardata.numberOfGears+1;k<8;k++)
    {
        r_gear[k]->disable();
        i_gear[k]->disable();
        e_gear[k]->disable();
        s_gear[k]->disable();
    }

    for (k=1;k<cardata.numberOfGears+1;k++)
    {
        r_gear[k]->enable();
        i_gear[k]->enable();
        e_gear[k]->enable();
        s_gear[k]->enable();
    }

    GLUI_Master.sync_live_all();
}

/*********************************************/
/* Optimal shift point calculation           */
/*********************************************/

float interpolationTQCV (float vectx[50], float vecty[50], float vectx0);

float integral (float vectx[50], float vecty[50], float x1, float x2, int n)
{
    // segements
    float dx=0.0;
    dx=(x2-x1)/n;

    //integrate
    float integral=0.0;
    float yinterpolated=0.0;
    float xint = 0.0;

    xint = x1+dx/2;

    int i = 0;
    for (i=0;i<n;i++){
        yinterpolated=interpolationTQCV(vectx, vecty, xint);
        integral+=yinterpolated;
        xint+=dx;
    }

    integral = integral*dx;
    return integral;
}


float derivative (float vectx[50], float vecty[50], float x, float h = 0.05)
{
    float derivative = 0.0;
    derivative = 1.0/h*(interpolationTQCV(vectx, vecty, x+h)-interpolationTQCV(vectx, vecty, x));
    return derivative;
}


float calcAreaCVRPM( int n  )
{
    float areaCVRPM=0.0;
    float areaShiftPoint[7]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    areaShiftPoint[0]=integral(cardata.engine.rpmValue, cardata.engine.tqValue, cardata.engine.rpmValue[0],optShiftPoint[0],n);
    int i=0;
    for (i=0;i<(cardata.numberOfGears-1);i++){
    areaShiftPoint[i+1]=integral(cardata.engine.rpmValue, cardata.engine.tqValue,optShiftPoint[i]*cardata.gearboxratio[i+2]/cardata.gearboxratio[i+1],optShiftPoint[i+1],n);
    }
    areaShiftPoint[cardata.numberOfGears-1]=integral(cardata.engine.rpmValue, cardata.engine.tqValue,optShiftPoint[cardata.numberOfGears-2]*cardata.gearboxratio[cardata.numberOfGears]/cardata.gearboxratio[cardata.numberOfGears-1],cardata.engine.params[2],n);

    /*
    areaShiftPoint[0]=integral(rpmValue, tqValue, rpmValue[0],optShiftPoint[0],n);
    areaShiftPoint[1]=integral(rpmValue, tqValue,optShiftPoint[0]*gearboxratio[2]/gearboxratio[1],optShiftPoint[1],n);
    areaShiftPoint[2]=integral(rpmValue, tqValue,optShiftPoint[1]*gearboxratio[3]/gearboxratio[2],optShiftPoint[2],n);
    areaShiftPoint[3]=integral(rpmValue, tqValue,optShiftPoint[2]*gearboxratio[4]/gearboxratio[3],optShiftPoint[3],n);
    areaShiftPoint[4]=integral(rpmValue, tqValue,optShiftPoint[3]*gearboxratio[5]/gearboxratio[4],optShiftPoint[4],n);
    areaShiftPoint[5]=integral(rpmValue, tqValue,optShiftPoint[4]*gearboxratio[6]/gearboxratio[5],optShiftPoint[5],n);
    areaShiftPoint[6]=integral(rpmValue, tqValue,optShiftPoint[5]*gearboxratio[7]/gearboxratio[6],params[2],n);
    */




    for(i=0;i<cardata.numberOfGears;i++){
        areaCVRPM+=areaShiftPoint[i];
    }

    return areaCVRPM;
}

float derivativeOSP (int s, float h = 5.0, int n=100)
{
    float derivative = 0.0;
    float areaCVRPM1=0.0;
    float areaCVRPM2=0.0;

    areaCVRPM1 = calcAreaCVRPM(n);
    optShiftPoint[s]+=h;
    areaCVRPM2 = calcAreaCVRPM(n);
    optShiftPoint[s]-=h;
    derivative = 1.0/h*(areaCVRPM2-areaCVRPM1);

    return derivative;
}

void optimalShiftPoint (int k)
{


float bestAreaCVRPM=0.0;

float areaCVRPM=0.0;
float areaCVRPM0=0.0;
int n=100;
float dRPM = 0.5;
int i=0;
int counter = 0;
int bestIteration = 0;
float derivativeGear=0.0;
float dGear[6]={0.0,0.0,0.0,0.0,0.0,0.0};
float dGearRMS=1000.0;

    /* initial point for iterations */
    for(i=0;i<6;i++){
        optShiftPoint[i]= rpmatcvmax+100.0;
        //cout <<"rpmatcvmax " << rpmatcvmax << endl;
    }

    while (dGearRMS >= 10.0 && counter < 3000){

        dGearRMS=0.0;

        //calculate the area for all the gears
        areaCVRPM0 = areaCVRPM;
        areaCVRPM = calcAreaCVRPM(n);
        //cout << areaCVRPM << " optShiftPoint 6-7 " << optShiftPoint[5]<< endl;

        //if the area is larger, asign the shift points
        if (areaCVRPM>=bestAreaCVRPM){
            bestAreaCVRPM=areaCVRPM;
            for(i=0;i<6;i++){
                bestOptShiftPoint[i]= optShiftPoint[i];
                bestIteration = counter;
            }
        }

        //go to the next maximun area following the derivative
        /*optShiftPoint[0]+=dRPM;
        optShiftPoint[1]+=dRPM;
        optShiftPoint[2]+=dRPM;
        optShiftPoint[3]+=dRPM;
        optShiftPoint[4]+=dRPM;
        optShiftPoint[5]+=dRPM;*/



        float upperLimit = 0.01;
        float lowerLimit = -0.01;

        cout << "ITERATION " << counter << endl;
        cout << "-------------------------------" <<endl;

        if (cardata.numberOfGears>=2){
        derivativeGear=derivativeOSP (0,1.0,100);
        dGear[0]=derivativeGear;
        if (derivativeGear>upperLimit) optShiftPoint[0]+=dRPM*derivativeGear;
        else if (derivativeGear<lowerLimit) optShiftPoint[0]+=dRPM*derivativeGear;
        if (optShiftPoint[0]>cardata.engine.params[1]){
            optShiftPoint[0]=cardata.engine.params[1];
            dGear[0]=0.0;
        }
        cout << counter << " derivative " << derivativeGear<< " OSP1-2: " <<optShiftPoint[0] << endl;
        }

        if (cardata.numberOfGears>=3){
        derivativeGear=derivativeOSP (1,1.0,100);
        dGear[1]=derivativeGear;
        if (derivativeGear>upperLimit) optShiftPoint[1]+=dRPM*derivativeGear;
        else if (derivativeGear<lowerLimit) optShiftPoint[1]+=dRPM*derivativeGear;
        if (optShiftPoint[1]>cardata.engine.params[1]){
            optShiftPoint[1]=cardata.engine.params[1];
            dGear[1]=0.0;
        }
        cout << counter << " derivative " << derivativeGear<< " OSP2-3: " <<optShiftPoint[1] << endl;
        }

        if (cardata.numberOfGears>=4){
        derivativeGear=derivativeOSP (2,1.0,100);
        dGear[2]=derivativeGear;
        if (derivativeGear>upperLimit) optShiftPoint[2]+=dRPM*derivativeGear;
        else if (derivativeGear<lowerLimit) optShiftPoint[2]+=dRPM*derivativeGear;
        if (optShiftPoint[2]>cardata.engine.params[1]){
            optShiftPoint[2]=cardata.engine.params[1];
            dGear[2]=0.0;
        }
        cout << counter << " derivative " << derivativeGear<< " OSP3-4: " <<optShiftPoint[2] << endl;
        }

        if (cardata.numberOfGears>=5){
        derivativeGear=derivativeOSP (3,1.0,100);
        dGear[3]=derivativeGear;
        if (derivativeGear>upperLimit) optShiftPoint[3]+=dRPM*derivativeGear;
        else if (derivativeGear<lowerLimit) optShiftPoint[3]+=dRPM*derivativeGear;
        if (optShiftPoint[3]>cardata.engine.params[1]){
            optShiftPoint[3]=cardata.engine.params[1];
            dGear[3]=0.0;
        }
        cout << counter << " derivative " << derivativeGear<< " OSP4-5: " <<optShiftPoint[3] << endl;
        }

        if (cardata.numberOfGears>=6){
        derivativeGear=derivativeOSP (4,1.0,100);
        dGear[4]=derivativeGear;
        if (derivativeGear>upperLimit) optShiftPoint[4]+=dRPM*derivativeGear;
        else if (derivativeGear<lowerLimit) optShiftPoint[4]+=dRPM*derivativeGear;
        if (optShiftPoint[4]>cardata.engine.params[1]){
            optShiftPoint[4]=cardata.engine.params[1];
            dGear[4]=0.0;
        }
        cout << counter << " derivative " << derivativeGear<< " OSP5-6: " <<optShiftPoint[3] << endl;
        }

        if (cardata.numberOfGears==7){
        derivativeGear=derivativeOSP (5,1.0,100);
        dGear[5]=derivativeGear;
        if (derivativeGear>upperLimit) optShiftPoint[5]+=dRPM*derivativeGear;
        else if (derivativeGear<lowerLimit) optShiftPoint[5]+=dRPM*derivativeGear;
        if (optShiftPoint[5]>cardata.engine.params[1]){
            optShiftPoint[5]=cardata.engine.params[1];
            dGear[5]=0.0;
        }
        cout << counter << " derivative " << derivativeGear<< " OSP6-7: " <<optShiftPoint[5] << endl;
        }

        for(i=0;i<6;i++){
                dGearRMS+=dGear[i]*dGear[i];
        }
        cout << "dGearRMS: " << dGearRMS << " Total area: " << areaCVRPM << " best: " << bestAreaCVRPM <<" ("<< bestIteration <<")"<<endl;

        counter++; //to avoid a infinite while
    }

   for(i=0;i<6;i++){
         rpmAfterBestOptShiftPoint[i]=bestOptShiftPoint[i]*cardata.gearboxratio[i+2]/cardata.gearboxratio[i+1];
   }

    cout << "Optimal Shit Points (max area under CV-rpm graph)" << endl;
    for(i=0;i<(cardata.numberOfGears-1);i++){
        cout << "Gear Shift " << i+1<<"-"<< i+2 << ": " <<bestOptShiftPoint[i] << " rpm After shift: "<< rpmAfterBestOptShiftPoint[i] <<endl;
    }

    GLUI_Master.sync_live_all();

/* calculation using the transmision torque concept

    float gearTT[7][21];
    int i;
    int j;
    for (i=0;i<7;i++){
        for (j=0;j<21;j++){
              gearTT[i][j]=0.0;
        }
    }

    float gearTTmax[7]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    float gearTTmin[7]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
    int gearTTmaxRPM[7]={0,0,0,0,0,0,0};
    int gearTTminRPM[7]={0,0,0,0,0,0,0};
    float optimalShiftPoint[6]={0.0,0.0,0.0,0.0,0.0,0.0};

    // calculate the transmision torque

    for (i=0;i<7;i++){
        gearTTmax[i] = gearboxratio[i+1]*tqValue[0];
        gearTTmin[i] = gearTTmax[i];
        for (j=0;j<21;j++){
            gearTT[i][j]=gearboxratio[i+1]*tqValue[j];
            cout << gearTT[i][j] << endl;
            if(gearTT[i][j]>gearTTmax[i]) {
                gearTTmax[i] = gearTT[i][j];
                gearTTmaxRPM[i]=j;
            }
            else if (gearTT[i][j]<gearTTmin[i]) {
                gearTTmin[i] = gearTT[i][j];
                gearTTminRPM[i]=j;
            }
        }
    }
    for (i=0;i<7;i++){
        if (gearTTmaxRPM[i] < 0 || gearTTmaxRPM[i] > 20) gearTTmaxRPM[i] = 20;
        if (gearTTminRPM[i] < 0 || gearTTminRPM[i] > 20) gearTTminRPM[i] = 20;
        cout << "Transmision torque gear" << i+1 <<endl;
        cout << gearboxratio[i+1]*interpolationTQCV(rpmValue, tqValue, params[2]) << " at " << params[2]<< " - " << gearTTmax[i] << " at " <<  rpmValue[gearTTmaxRPM[i]];
        cout << " - "<< gearTT[i][19] << " at " << rpmValue[19] << " rpm" <<endl;
    }

    // optimalshift points
    float rpmNextGear = 10.0;
    float tqNextGear = 10.0;
    float ttNextGear = 10.0;
    for (i=1;i<7;i++){
        for (j=20;j>1;j--){
            rpmNextGear = rpmValue[j]*gearboxratio[i+1]/gearboxratio[i];
            if (rpmNextGear <= 0.0) rpmNextGear = 1000.0;
            if (rpmNextGear >rpmValue[20])  rpmNextGear = rpmValue[20];
            tqNextGear = interpolationTQCV(rpmValue, tqValue, rpmNextGear);
            ttNextGear = gearboxratio[i]*tqNextGear;
            if (gearTT[i][j]>=0.75*ttNextGear && gearTT[i][j]<=1.25*ttNextGear){
                optimalShiftPoint[i]=rpmValue[j];
            }
        }
        cout << "GEAR " << i << endl;
        cout << "tt at revs lim= "<< gearboxratio[i]*interpolationTQCV(rpmValue, tqValue, params[2])  <<" tt next gear "<< ttNextGear << " at " << rpmNextGear <<" rpm"<<endl;
        cout << "optimalShiftPoint Gear"<<i<<" = "<< optimalShiftPoint[i] <<" rpm"<<endl;
    }
*/




}



void gearsAutoinertiaEff ( int k )
{
    int i;
    switch (k)
    {
        case 1:
        /* Efficiency*/
        for (i=0;i<8;i++)
        {
            cardata.gearboxefficiency[i]= gearsAutoEff;
        }
        break;

        case 2:
        /* Inertia*/
        for (i=1;i<9;i++)
        {
            cardata.gearboxinertia[i]= gearsAutoInertia;
        }
        break;
    }

    GLUI_Master.sync_live_all();

}

