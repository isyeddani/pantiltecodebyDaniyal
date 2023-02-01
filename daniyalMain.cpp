#include "interface.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cmath>
#include <fstream>

int deg = 0;
int azzdeg = 0;
int i = 0;

int main()
{
<<<<<<< HEAD
    Packet elevationObject("/dev/ttyUSB1");
    elevationObject.print = false;
    Packet azimuthObject("/dev/ttyUSB2");
    azimuthObject.print = false;
    int speed = 1000; // 200 for azi
    int acceleration = 1000;
    int usleep_time = 80000;
    int azzspeed = 200; // 200 for azi
    int azzacceleration = 500;
    int azzusleep_time = 80000;
    while (i < 100)
    {
        
        if (i % 2 == 0)
        {
            deg = -40;
            azzdeg = -100;
        }
        else
        {
            deg = 40;
            azzdeg = 100;
        }
        std::cout<<"Degree Elevation: "<<deg<<std::endl;
        elevationObject.serialPort.Open();
        elevationObject.StartupSequenceJ2S(usleep_time);
        elevationObject.SpeedAccelSetupJ2S(usleep_time, speed, acceleration);
        elevationObject.DegreeRotationJ2S(deg, usleep_time, speed, 1310720);
        usleep(usleep_time);
        elevationObject.StopMotionJ2S();
        elevationObject.serialPort.Close();
        std::cout<<"Degree Azimuth: "<<azzdeg<<std::endl;
        azimuthObject.serialPort.Open();
        azimuthObject.StartupSequenceJ2S(azzusleep_time);
        azimuthObject.SpeedAccelSetupJ2S(azzusleep_time, azzspeed, azzacceleration);
        azimuthObject.DegreeRotationJ2S(azzdeg, azzusleep_time, azzspeed, 131072);
        usleep(usleep_time);
        azimuthObject.StopMotionJ2S();
        azimuthObject.serialPort.Close();
        i = i + 1;
    }
=======
    Packet elevationObject("/dev/ttyUSB2");
    elevationObject.serialPort.Open();
    elevationObject.print=false;
    int speed =1000;
    int acceleration = 1000;
    int usleep_time = 80000;
    elevationObject.StartupSequenceJ2S(usleep_time);
    elevationObject.SpeedAccelSetupJ2S(usleep_time,speed,acceleration);
    elevationObject.DegreeRotationJ2S(-20,usleep_time,speed,1310720);
    usleep(usleep_time);
    elevationObject.StopMotionJ2S();
    elevationObject.serialPort.Close();
>>>>>>> parent of d1d4bf2 (Acknowledge Reveice and Process, Pulse Read Commands)
    return 0;
}
