#ifndef FILE_IO_OPERATION_H
#define FILE_IO_OPERATION_H

#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <chrono>
#include <bitset>
#include <bits/stdc++.h>
#include "interface.hpp"
class FileOperations
{
    private:
        bool fileCreated;
        bool proofDataSet;
        int usleep_time;
    public:
        FileOperations();
        void Park(Packet &elevationObject, Packet &azimuthObject, int elevationSpeed, int azimuthSpeed);
        bool CreateFile();
        void SetData();
        bool ReadData();
        void StoreDate(double elevationAngle, double azimuthAngle);
        bool FileInitialize();
};  

#endif //FILE_IO_OPERATION_H