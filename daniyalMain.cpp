#include "interface.hpp"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cmath>
#include <fstream>

int main()
{
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
    return 0;
}


