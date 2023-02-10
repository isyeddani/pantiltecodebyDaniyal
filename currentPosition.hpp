#include <CppLinuxSerial/SerialPort.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <chrono>

using namespace mn::CppLinuxSerial;
using namespace std::chrono;

class pulseCount
{
private:
    /// @brief
    int TotalAzimuthPulses;
    /// @brief
    int TotalElevationPulses;
    /// @brief
    int RevCountsAzimuth;
    /// @brief
    int RevCountsElevation;
    /// @brief 
    std::string errorCode[6] = {"Normal Operation", "Parity Error", "CheckSum Error", "Character Error", "Command Error", "DataNo. Error"};

public:
    /// @brief
    /// @return
    int getAzimuthPulses();

    /// @brief
    /// @param azimuthPul
    void setAzimuthPulses(int azimuthPul);

    /// @brief
    /// @return
    int getAzimuthRevCount();

    /// @brief
    /// @param azimuthRev
    void setAzimuthRevCount(int azimuthRev);

    /// @brief
    /// @return
    int getElevationPulses();

    /// @brief
    /// @param elevationPul
    void setElevationPulses(int elevationPul);

    /// @brief
    /// @return
    int getElevationRevCount();

    /// @brief
    /// @param elevationRev
    void setElevationRevCount(int elevationRev);

    /// @brief
    /// @param receivedPacket
    /// @param comment
    void ReadDisplay(std::string receivedPacket, std::string comment);

    /// @brief 
    /// @param receivedPacket 
    /// @return True if authentic Acknowledge, False is Bad Acknowledge  
    bool ReceivedDataProcessing(std::string receivedPacket);
};