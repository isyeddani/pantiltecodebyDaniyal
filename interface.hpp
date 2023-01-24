#include <CppLinuxSerial/SerialPort.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <chrono>



using namespace mn::CppLinuxSerial;
using namespace std::chrono;

class Packet{
	public:
		char SOH;
		char STX;
		char ETX;
		char EOT;
		char Axis;
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
		// std::string CheckSum;
		std::stringstream PacketStream;
		std::string PacketString;
		std::string ReceivedPacket;
		bool print;
		SerialPort serialPort;
		Packet(std::string port);
		void SendDisableExInputsPacket(void);
		void SendSpeedPacket(std::string speed);
		void SendModeSelect(std::string mode);
		void SendServoOnPacket(void);
		void SendStrokeOnPacket(void);
		void SendAccelerationPacket(std::string acc);
		void SendDirectionPacket(std::string dir);
		void SendMoveDistancePacket(std::string pulses);
		void SendParameterPacket(std::string para);
		void SendGeneralPacket(std::string Com, std::string dataNo);
		void SendGeneralPacket(std::string Com, std::string dataNo, std::string data);
		void SendGeneralPacket2(std::string Com, std::string dataNo);
		void SendGeneralPacket2(std::string Com, std::string dataNo, std::string data);
		void SendAllStatusReadPacket(void);
		void ClearPacketStream(void);
		void StartupSequenceJ2 (int usleep_time);
		void StartupSequenceJ2S (int usleep_time);
		void HeartbeatJ2(int usleep_time, int loop_time);
		void HeartbeatJ2S(int usleep_time, int loop_time);
		void SpeedAccelSetupJ2(int usleep_time,int speed,int acceleration);
		void SpeedAccelSetupJ2S(int usleep_time,int speed,int acceleration);
		void DegreeRotationJ2(double degree, int usleep_time,int sleep);
		void DegreeRotationJ2S(double degree, int usleep_time, int sleep, double total_pulses);
		void StopMotionJ2(void);
		void StopMotionJ2S(void);
		// void StartupSequenceAzimuth (int usleep_time);
		// void StartupSequenceElevation (int usleep_time);
		// void HeartbeatAzimuth(int usleep_time, int loop_time);
		// void HeartbeatElevation(int usleep_time, int loop_time);
		// void SpeedAccelSetupAzimuth(int usleep_time,int speed,int acceleration);
		// void SpeedAccelSetupElevation(int usleep_time,int speed,int acceleration);
		//
		// void DegreeRotationAzimuth(double degree, int usleep_time,int sleep);
		// void DegreeRotationElevation(double degree, int usleep_time, int sleep, double total_pulses);
		//
		// void StopMotionAzimuth(void);
		// void StopMotionElevation(void);
		std::string CalCheckSum(void);
		std::string Stringpad(std::string string_to_pad, int length, char character_pad);
		std::string Convert(long int num, int bit_length);

};
