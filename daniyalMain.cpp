#include "interface.hpp"
#include "File_io_Operation.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cmath>
#include <fstream>
#include <thread>
#include <cstdlib>

// for <streamsize>
#include <ios>

// for numeric_limits
#include <limits>

using namespace std;

int main()
{
    srand(time(0));
    Packet elevationObject("/dev/ttyUSB1", "Elevation");
    Packet azimuthObject("/dev/ttyUSB2", "Azimuth");
    FileOperations fileOpsObject;
    fileOpsObject.FileInitialize();
    // azimuthObject.print = false;
    int speed = 800; // 200 for azi
    int acceleration = 100;
    int usleep_time = 80000;
    int azzspeed = 200; // 200 for azi
    int azzacceleration = 100;
    int azzusleep_time = 80000;

    elevationObject.serialPort.Open();
    azimuthObject.serialPort.Open();

    std::thread A1(&Packet::FlushSerialBuffer, &azimuthObject);
    std::thread E1(&Packet::FlushSerialBuffer, &elevationObject);
    A1.join();
    E1.join();

    std::thread A2(&Packet::StartupSequenceJ2S, &elevationObject, usleep_time);
    std::thread E2(&Packet::StartupSequenceJ2S, &azimuthObject, usleep_time);
    A2.join();
    E2.join();

    std::thread A3(&Packet::SpeedAccelSetupJ2S, &azimuthObject, azzusleep_time, azzspeed, azzacceleration);
    std::thread E3(&Packet::SpeedAccelSetupJ2S, &elevationObject, usleep_time, speed, acceleration);
    A3.join();
    E3.join();

    int elvdeg = 0;
    int azzdeg = 0;

    // int A;
    // // std::cout << "Input Degrees for Elevation: ";
    // A = elevationObject.InputDegree();
    // std::cout << A;

    for (int i = 0; i < 2; i++)
    {
        // std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // std::cout << "Input Degrees for Elevation: ";
        // std::cin >> elvdeg;
        // std::cout << "Input Degrees for Azimuth: ";
        // std::cin >> azzdeg;
        // std::cout << "ERROR: Taking Degrees" << std::endl;

        // elvdeg = elevationObject.InputDegree();

        if (i % 2 == 0)
        {
            elvdeg = 10;
            azzdeg = -100;
        }
        else
        {
            elvdeg = -10;
            azzdeg = -100;
        }

        // elvdeg = elevationObject.RandomFunction();
        // azzdeg = elevationObject.RandomFunction();

        std::cout << std::endl;
        std::cout << "Degree Elevation: " << elvdeg << std::endl;
        std::cout << "Degree Azimuth: " << azzdeg << std::endl;
        std::thread azimuthThread(&Packet::DegreeRotationJ2S, &azimuthObject, azzdeg, azzusleep_time, azzspeed, 131072);
        std::thread elevationThread(&Packet::DegreeRotationJ2S, &elevationObject, elvdeg, usleep_time, speed, 1310720);
        azimuthThread.join();
        elevationThread.join();

        elevationObject.DisplayData(usleep_time,true);
        azimuthObject.DisplayData(azzusleep_time,true);

        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl
                  << std::endl;
        // threadCompleteFlag = true;
        elevationObject.StopMotionJ2S();
        azimuthObject.StopMotionJ2S();
        // usleep(usleep_time * 50);
        fileOpsObject.StoreDate(elevationObject.pulseCountObject.getCurrentAngle(), azimuthObject.pulseCountObject.getCurrentAngle());
    }
    fileOpsObject.Park(elevationObject,azimuthObject,speed,azzspeed);
    elevationObject.serialPort.Close();
    azimuthObject.serialPort.Close();
    return 0;
}
