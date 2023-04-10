#ifndef INTERFACE_H
#define INTERFACE_H

#include <CppLinuxSerial/SerialPort.hpp>
#include "currentPosition.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <chrono>
using namespace mn::CppLinuxSerial;
using namespace std::chrono;

class Packet
{
public:
	// Instance Variable.
	char SOH;
	char STX;
	char ETX;
	char EOT;
	char Axis;
	bool print;
	bool ValidSentRec;
	bool Direction; 
	bool SerialBufferFlag;
	bool motionFlag;
	std::string Speed;
	std::string Acc;
	std::string Forward;
	std::string Backward;
	std::string Stop;
	std::string TestMode;
	std::string JogMode;
	std::string PositionMode;
	std::string MotorLessMode;
	std::string ForcedMode;
	std::string Tag;
	SerialPort serialPort;
	/// @brief 
	PulseCount pulseCountObject;
	std::stringstream PacketStream;
	std::string PacketString;
	std::string ReceivedPacket;


	// Methods 
	Packet(std::string port, std::string Cap);
	void SendGeneralPacket2(std::string Com, std::string dataNo, char stNo);
	void SendGeneralPacket2(std::string Com, std::string dataNo, std::string data, char stNo);
	void SendAllStatusReadPacket(void);
	void ClearPacketStream(void);
	void StartupSequenceJ2S(int usleep_time);
	void HeartbeatJ2S(int usleep_time, int loop_time);
	// void HeartbeatJ2S(int ReqVal,int usleep_time, int loop_time);
	void SpeedAccelSetupJ2S(int usleep_time, int speed, int acceleration);
	void DegreeRotationJ2S(double degree, int usleep_time, int sleep, double total_pulses);
	void StopMotionJ2S(void);
	void FlushSerialBuffer();
	void DisplayData(int usleep_time,bool doCalculation);
	int RandomFunction();
	int InputDegree();
	std::string CalCheckSum(void);
	std::string Stringpad(std::string string_to_pad, int length, char character_pad);
	std::string Convert(long int num, int bit_length);
};
#endif // INTERFACE_H