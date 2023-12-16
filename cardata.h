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
    /* AUTOR INFO */
extern std::string autorName;
extern std::string autorEmail;

/* CAR NAME */
extern std::string carname;
extern std::string fullCarName;

/* ENGINE */
extern float engineparams[5];
extern float tqValue[21];
extern float  rpmValue[21];
extern float  cvValue[21]; /* don' import */
extern int turboS;
extern float turbo[3];
extern float engineCapacity;
extern int curr_engine_capacity_units;
extern const char *engine_capacity_units[];
extern int engineCylinders;
extern int curr_engine_shape;
extern const char *engine_shape[];
extern int curr_engine_position;
extern const char *engine_position[];
extern float brakeLinearCoefficient;
extern float brakeCoefficient;

/* GEARBOX */
extern int numberOfGears;
extern float gearbox_shift_time;
extern float gearboxratio[8];
extern float gearboxinertia[9];
extern float gearboxefficiency[8];

/* SUSPENSION  */
extern float  antirollbar1[3];
extern float  antirollbar2[3];
extern float suspension1[8];
extern float suspension2[8];
extern float suspension3[8];
extern float suspension4[8];

/* AERODINAMICS */
extern float aerodynamics[4];
extern float frontwing[4];
extern float rearwing[4];

/* MASS DATA */
extern float massdata[8];

/* BRAKE SYSTEM & BRAKES*/
extern float brakesystem[2];
extern float brake1[4];
extern float brake2[4];
extern float brake3[4];
extern float brake4[4];

/* WHEELS AND TIRES */
extern float wheel1[12];
extern float wheel2[12];
extern float wheel3[12];
extern float wheel4[12];
extern float wheelRadius[4];
    
/* STEER */
extern float steer[2];
    
/* DRIVETRAIN */
extern char *drivetrain_type[];
extern int curr_drivetrain_type;
extern float drivetrain;
    
/* DIFFERENTIAL */
extern char *differential_type[];
extern int curr_frontdifferential_type;
extern float frontdifferential[5];
extern int curr_reardifferential_type;
extern float reardifferential[5];
extern int curr_centraldifferential_type;
extern float centraldifferential[5];
    
/* FRONT AND REAR AXLES */
extern float frontaxle[3];
extern float rearaxle[3];

/* SOUND */
extern std::string enginesample;
extern float rmpscale ;

/* GRAPHIC */
extern std::string graphicenv;
extern std::string wheelTexture;
extern std::string shadowTexture;
extern std::string tachometerTexture;
extern std::string speedometerTexture;
extern int tachometerMinMax[2];
extern int speedometerMinMax[2];
    
extern std::string carRange1;
extern std::string carRange2;
extern std::string carRange3;
extern std::string carRange4;
extern std::string carRange5;

extern int carRange1dataA;
extern int carRange2dataA;
extern int carRange3dataA;
extern int carRange4dataA;
extern int carRange5dataA;
extern int carRange1dataB;
extern int carRange2dataB;
extern int carRange3dataB;
extern int carRange4dataB;
extern int carRange5dataB;

extern int carRange1dataC;
extern int carRange2dataC;
extern int carRange3dataC;
extern int carRange4dataC;
extern int carRange5dataC;

/* BONNET POSITION */
extern float bonnetPosition[3];

/* DRIVER POSITION */
extern float driverPosition[3];
    
/* CAR DIMENSIONS */
extern std::string carCategory;
extern float carBodyDimensions[3];
extern float carOverallDimensions[2];

/* CAR LIGHTS */
extern float head1a[4];
extern float head1b[4];
extern float head2a[4];
extern float head2b[4];
extern float reara[4];
extern float rearb[4];
extern float brakea[4];
extern float brakeb[4];
extern float rearc[4];
extern float reard[4];
extern float brakec[4];
extern float braked[4];
extern float brake2a[4];
extern float brake2b[4];
    
extern int head1aS;
extern int head1bS;
extern int head2aS;
extern int head2bS;
extern int rearaS;
extern int rearbS;
extern int brakeaS;
extern int brakebS;
extern int rearcS;
extern int reardS;
extern int brakecS;
extern int brakedS;
extern int brake2aS;
extern int brake2bS;
    
/* FLAMES */
extern int flameS;
extern float flamePower;
extern float flame1[3];
extern float flame2[3];

/* text message of the glut window */
extern std::string text1;
extern int   main_window;
