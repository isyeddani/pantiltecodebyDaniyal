#ifndef CURRENTPOSITION_H // PHYSICS_H
#define CURRENTPOSITION_H

// #include <CppLinuxSerial/SerialPort.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <chrono>
#include <bitset>
#include <bits/stdc++.h>

// using namespace mn::CppLinuxSerial;
// using namespace std::chrono;

/// @brief
class PulseCount
{
private:
    /// @brief
    double CurrentAngle = 0.0;
    double PreviousAnlge = 0.0;
    double NewAngle = 0.0;
    double liveAngle = 0.0;
    /// @brief
    int RevCount;
    /// @brief
    std::string errorCode[6] = {"Normal Operation", "Parity Error", "CheckSum Error", "Character Error", "Command Error", "DataNo. Error"};  

public:
    /// @brief
    std::string Tag;
    bool MotionStart;
    int OverLoadFlag;
    bool overLoadOccuranceFlag;

    PulseCount();
    /// @brief
    /// @param Name
    /// @param parsedirection
    // PulseCount(std::string Name,bool parsedirection);
    PulseCount(std::string ObjectTag);

    /// @brief
    /// @return
    double getCurrentAngle();

    /// @brief
    /// @param Pul
    void setCurrentAngle(double Angle, bool directionFlag);

    /// @brief
    /// @return
    double getRevCount();

    /// @brief
    /// @param Rev
    void setRevCount(double Revolution);

    /// @brief
    /// @param receivedPacket
    /// @param comment
    void ReadDisplay(std::string receivedPacket, std::string comment);

    /// @brief
    /// @param receivedPacket
    /// @return True if authentic Acknowledge, False is Bad Acknowledge
    bool ReceivedDataProcessing(std::string receivedPacket, bool Process);

    /// @brief
    /// @param n
    /// @return Ones Complement of Integer.
    unsigned int onesComplement(unsigned int n);

    double PulsestoDegree(unsigned int Pulses);

    void LiveAngleCalculation(double NewAngle, bool directionFlag);  
};

#endif //CURRENTPOSITION_H