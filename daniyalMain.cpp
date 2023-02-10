#include "interface.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cmath>
#include <fstream>
#include <thread>

int elvdeg = 0;
int azzdeg = 0;
int i = 0;

int main()
{
    Packet elevationObject("/dev/ttyUSB1", "Elevation");
    elevationObject.print = false;
    Packet azimuthObject("/dev/ttyUSB2", "Azimuth");
    // azimuthObject.print = false;
    int speed = 200; // 200 for azi
    int acceleration = 100;
    int usleep_time = 80000;
    int azzspeed = 100; // 200 for azi
    int azzacceleration = 100;
    int azzusleep_time = 80000;
    while (i < 4)
    {
        elevationObject.serialPort.Open();
        azimuthObject.serialPort.Open();
        elevationObject.StartupSequenceJ2S(usleep_time);
        elevationObject.SpeedAccelSetupJ2S(usleep_time, speed, acceleration);
        azimuthObject.StartupSequenceJ2S(azzusleep_time);
        azimuthObject.SpeedAccelSetupJ2S(azzusleep_time, azzspeed, azzacceleration);
        // if (threadCompleteFlag == true)
        // {
        if (i % 2 == 0)
        {
            elvdeg = 45;
            azzdeg = 6;
        }
        else
        {
            elvdeg = 45;
            azzdeg = 6;
        }
        std::cout<<std::endl;
        std::cout << "Degree Azimuth: " << azzdeg << std::endl;
        std::thread azimuthThread(&Packet::DegreeRotationJ2S, &azimuthObject, azzdeg, azzusleep_time, azzspeed, 131072);
        std::cout << "Degree Elevation: " << elvdeg << std::endl;
        std::thread elevationThread(&Packet::DegreeRotationJ2S, &elevationObject, elvdeg, usleep_time, speed, 1310720);
        // elevationObject.DegreeRotationJ2S(elvdeg, usleep_time, speed, 1310720);
        // usleep(usleep_time);
        // azimuthObject.DegreeRotationJ2S(azzdeg, azzusleep_time, azzspeed, 131072);
        // usleep(usleep_time);
        azimuthThread.join();
        elevationThread.join();
        std::cout << "----------------" << std::endl;
        // threadCompleteFlag = true;
        elevationObject.StopMotionJ2S();
        azimuthObject.StopMotionJ2S();
        elevationObject.serialPort.Close();
        azimuthObject.serialPort.Close();
        i = i + 1;
        usleep(usleep_time * 10);
    }
    return 0;
}
