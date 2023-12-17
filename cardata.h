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

#include <list>
#include <string>

struct CarData
{
    /* parameters */
    /* AUTOR INFO */
    std::string autorName = "Vicente Marti";
    std::string autorEmail = "vtemarticent@gmail.com";

    /* CAR NAME */
    std::string carname = "formula-torcs";
    std::string fullCarName = "Formula TORCS";

    /* ENGINE */
    struct Engine
    {
        float inertia = 0.02;
        float revsMaxi = 20000;
        float revsLimiter = 19000;
        float tickover = 5000;
        float fuelConsFactor = 1.2;
        float tqValue[21] = {0,150,200,220,250,300,320,340,370,360,400,410,405,402,403,404,390,380,370,360,320};
        float rpmValue[21] = {0,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000};
        float cvValue[21] = { 0 };
        int turboS = 0;
        float turbo[3] = {3000.0, 1.0, 0.75};
        float capacity = 0;
        const char *capacity_units[3] = { "l", "cc", "ci" };
        int curr_capacity_units = 0;
        int cylinders = 0;
        const char *shape[4] = { "v", "l", "h", "w" };
        int curr_shape = 0;
        const char *position[5] = { "front", "front-mid", "mid", "rear-mid", "rear" };
        int curr_position = 0;
        float brakeLinearCoefficient = 0;
        float brakeCoefficient = 0;
    } engine;

    /* GEARBOX */
    int numberOfGears = 7;
    float gearbox_shift_time = 0.05;
    float gearboxratio[8]={-2.00,4.04,3.37,2.89,2.53,2.20,1.98,1.81};
    float gearboxinertia[9]={0.1150,0.0037,0.003,0.0037,0.0048,0.0064,0.0107,0.0150,0.0150};
    float gearboxefficiency[8]={0.957,0.955,0.957,0.950,0.983,0.948,0.940,0.940};

    /* SUSPENSION  */
    float  antirollbar1[3] = {1000,0.1,2.5};
    float  antirollbar2[3] = {300,0.1,2.5};
    float  suspension1[8] = {3500,0.08,1.5,0,80,80,50,50};
    float  suspension2[8] = {3500,0.08,1.5,0,80,80,50,50};
    float  suspension3[8] = {5500,0.14,1.5,0,80,80,50,50};
    float  suspension4[8] = {5500,0.14,1.5,0,80,80,50,50};

    /* AERODINAMICS */
    float aerodynamics[4] = {0.35,2.2,0.7,0.8};
    float frontwing[4] = {0.45,10.0,2.0,0.1};
    float rearwing[4] = {0.9,8.0,-2.3,0.6};

    /* MASS DATA */
    float massdata[8] = {0.515,0.5,0.5,0.8,100,100,615,0.31};

    /* BRAKE SYSTEM & BRAKES*/
    float brakesystem[2] = {0.53,20000};
    float brake1[4] = {278,50,0.3,0.1241};
    float brake2[4] = {278,50,0.3,0.1241};
    float brake3[4] = {278,25,0.3,0.0714};
    float brake4[4] = {278,25,0.3,0.0714};

    /* WHEELS AND TIRES */
    float wheel1[12] = {-0.77,13,355,0.464788732,1.2200,50,0,-2,21,80,0.02,1.8};
    float wheel2[12] = {0.77,13,355,0.464788732,1.2200,50,0,-2,21,80,0.02,1.8};
    float wheel3[12] = {-0.80,13,380,0.434210526,1.2200,80,0,-1.5,21,80,0.02,1.8};
    float wheel4[12] = {0.80,13,380,0.434210526,1.2200,80,0,-1.5,21,80,0.02,1.8};
    float wheelRadius[4] = {0.2,0.2,0.2,0.2}; // calculate don't export-imprort

    /* STEER */
    float steer[2] = {45,120};

    /* DRIVETRAIN */
    const char *drivetrain_type[3] = { "RWD", "FWD", "4WD" };
    int curr_drivetrain_type = 0; // list box drivetrain
    float drivetrain = 0.0091;

    /* DIFFERENTIAL */
    const char *differential_type[5] = { "FREE", "SPOOL", "LIMITED SLIP", "VISCOUS COUPLER", "NONE" };
    int curr_frontdifferential_type = 4; // list box frong differential
    int curr_reardifferential_type = 0; // list box rear differential
    int curr_centraldifferential_type = 4; // list box central differential
    float frontdifferential[5] = {0.0488,4.5,0.9625,50.0,60.0};
    float reardifferential[5] = {0.0488,4.5,0.9625,50.0,60.0};
    float centraldifferential[5] = {0.0488,4.5,0.9625,50.0,60.0};

    /* FRONT AND REAR AXLES */
    float frontaxle[3] = {1.16,0.0056,0.012};
    float rearaxle[3] = {-1.83,0.0080,0.040};

    /* SOUND */
    std::string enginesample = "renault-v10.wav";
    float rmpscale = 0.30;

    /* GRAPHIC */
    std::string graphicenv = "formula-torcs.ac";
    std::string wheelTexture = "tex-wheel.rgb";
    std::string shadowTexture = "shadow.rgb";
    std::string tachometerTexture = "rpm20000.rgb";
    std::string speedometerTexture = "speed360.rgb";
    int tachometerMinMax[2] = {0,20000};
    int speedometerMinMax[2] = {0,360};

    std::string carRange1 = "formula-torcs.ac";
    std::string carRange2 = "formula-torcs.ac";
    std::string carRange3 = "formula-torcs.ac";
    std::string carRange4 = "formula-torcs.ac";
    std::string carRange5 = "formula-torcs.ac";

    int carRange1dataA = 20;
    int carRange2dataA = 15;
    int carRange3dataA = 10;
    int carRange4dataA = 5;
    int carRange5dataA = 0;
    int carRange1dataB = 1;
    int carRange2dataB = 1;
    int carRange3dataB = 1;
    int carRange4dataB = 1;
    int carRange5dataB = 0;

    int carRange1dataC = 1;
    int carRange2dataC = 1;
    int carRange3dataC = 1;
    int carRange4dataC = 1;
    int carRange5dataC = 1;

    /* BONNET POSITION */
    float bonnetPosition[3] = {-0.50,0.0,1.10};

    /* DRIVER POSITION */
    float driverPosition[3] = {-0.33,0.0,0.75};

    /* CAR DIMENSIONS */
    std::string carCategory = "F1";
    float carBodyDimensions[3] = {4.683,1.65,1.17};
    float carOverallDimensions[2] = {4.7,1.7};

    /* CAR LIGHTS */
    float head1a[4]={2.11,0.65,0.44,0.2};
    float head1b[4]={2.11,-0.65,0.44,0.2};
    float head2a[4]={2.18,0.47,0.42,0.2};
    float head2b[4]={2.18,-0.47,0.42,0.2};
    float reara[4]={1.17,0.75,0.31,0.15};
    float rearb[4]={1.17,-0.75,0.31,0.15};
    float brakea[4]={1.17,0.75,0.31,0.15};
    float brakeb[4]={1.17,-0.75,0.31,0.15};
    float rearc[4]={-1.77,0.75,0.31,0.15};
    float reard[4]={-1.77,-0.75,0.31,0.15};
    float brakec[4]={-1.77,0.75,0.31,0.15};
    float braked[4]={-1.77,-0.75,0.31,0.15};
    float brake2a[4]={-2.30,0.0,0.42,0.10};
    float brake2b[4]={-2.30,0.0,0.44,0.10};

    int head1aS = 0;
    int head1bS = 0;
    int head2aS = 0;
    int head2bS = 0;
    int rearaS = 0;
    int rearbS = 0;
    int brakeaS = 0;
    int brakebS = 0;
    int rearcS = 0;
    int reardS = 0;
    int brakecS = 0;
    int brakedS = 0;
    int brake2aS = 0;
    int brake2bS = 0;

    /* FLAMES */
    int flameS = 1;
    float flamePower = 0.3;
    float flame1[3] = {-1.15,-0.31,0.57};
    float flame2[3] = {-1.15,0.31,0.57};

    // STEER WHEEL
    struct SteeringWheel
    {
        std::string model;
        std::string hiResModel;
        float xpos = 0;
        float ypos = 0;
        float zpos = 0;
        float angle = 0;
        float movt = 0;
    } steeringWheel;

    // DRIVER
    struct Driver
    {
        float steer = 0;
        std::string driver;
        float xpos = 0;
        float ypos = 0;
        float zpos = 0;
    };
    std::list<Driver> drivers;
};
