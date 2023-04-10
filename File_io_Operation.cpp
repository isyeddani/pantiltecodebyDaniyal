#include "File_io_Operation.hpp"
// #include "interface.hpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

FileOperations::FileOperations()
{

    this->fileCreated = false;
    this->proofDataSet = false;
    this->usleep_time = 80000;
}

bool FileOperations::FileInitialize()
{
    fstream DataFile;
    DataFile.open("Data.txt", ios::out);
    if (DataFile.is_open())
    {
        fileCreated = true;
        return true;
    }
    else
    {
        fileCreated = false;
        std::cout << "ERROR: File Opening" << std::endl;
        std::cout << "Do You Want To Create a File (Y/N)";
        std::string answer;
        answer = "y";
        // std::cin >> answer;
        std::cout << answer << std::endl;
        // answer = tolower(answer);
        if (answer == "y")
        {
            std::cout << "Creating New File\n";
            return CreateFile();
        }
        else if (answer == "n")
        {
            std::cout << "answer is no.\n";
            return false;
        }
        else
        {
            std::cout << "answer is ???\n;";
            return false;
        }
    }
}

void FileOperations::Park(Packet &elevationObject, Packet &azimuthObject, int elevationSpeed, int azimuthSpeed)
{
    if (fileCreated)
    {
        std::cout << "Parking...." << std::endl;
        double elvAngleToHome = (-1) * (elevationObject.pulseCountObject.getCurrentAngle());
        double azmAngleToHome = (-1) * (azimuthObject.pulseCountObject.getCurrentAngle());
        std::thread azimuthThread(&Packet::DegreeRotationJ2S, &azimuthObject, azmAngleToHome, usleep_time, azimuthSpeed, 131072);
        std::thread elevationThread(&Packet::DegreeRotationJ2S, &elevationObject, elvAngleToHome, usleep_time, elevationSpeed, 1310720);
        azimuthThread.join();
        elevationThread.join();
        elevationObject.DisplayData(usleep_time,true);
        azimuthObject.DisplayData(usleep_time,true);
    }
}

// This Will Create File if File Not Found.
bool FileOperations::CreateFile()
{
    fstream DataFile;
    DataFile.open("Data.txt", ios::out); // Write
    if (DataFile.is_open())
    {
        std::cout << "Succesfully Created a File Name Data.txt\n";
        fileCreated = true;
        DataFile.close();
        return true;
    }
    else
    {
        std::cout << "ERROR: Creating File" << std::endl;
        fileCreated = false;
        return false;
    }
}

// Use to Set Angles in Files
void FileOperations::SetData()
{
}

bool FileOperations::ReadData()
{
    return false;
}

void FileOperations::StoreDate(double elevationAngle, double azimuthAngle)
{
    fstream DataFile;
    DataFile.open("Data.txt", ios::out); // Write
    if (DataFile.is_open())
    {
        fileCreated = true;
        DataFile << "Elevation:\n";
        DataFile << elevationAngle;
        DataFile << "\n";
        DataFile << "Azimuth:\n";
        DataFile << azimuthAngle;
    }
    DataFile.close();
    DataFile.open("Data.txt", ios::in);
    if (DataFile.is_open())
    {
        fileCreated = true;
        double elevationDataCheck;
        double azimuthDataCheck;
        int dustbin;
        string Aux;
        DataFile >> Aux;
        std::cout << Aux << std::endl;
        if (Aux == "Elevation:")
        {
            proofDataSet = true;
            DataFile >> elevationDataCheck;
            (elevationDataCheck == (elevationAngle)) ? proofDataSet = true : proofDataSet = false;
            // std::cout << elvdeg << std::endl;
        }
        else
        {
            proofDataSet = false;
            std::cout << "Invalid1" << std::endl;
            DataFile >> dustbin;
        }
        DataFile >> Aux;
        std::cout << Aux << std::endl;
        if (Aux == "Azimuth:")
        {
            DataFile >> elevationDataCheck;
            (azimuthDataCheck == (azimuthAngle)) ? proofDataSet = true : proofDataSet = false;
            // std::cout << azzdeg << std::endl;
        }
        else
        {
            proofDataSet = false;
            std::cout << "Invalid2" << std::endl;
            DataFile >> dustbin;
        }
    }
    else
    {
        fileCreated = false;
        std::cout << "ERROR: File Opening (2)" << std::endl;
    }
}
