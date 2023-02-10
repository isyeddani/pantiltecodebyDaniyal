#include "interface.hpp"
// #include "currentPosition.hpp"
#include <cmath>

Packet::Packet(std::string port, std::string objectName)
{
	serialPort = SerialPort(port, BaudRate::B_9600, NumDataBits::EIGHT, Parity::EVEN, NumStopBits::ONE);
	serialPort.SetTimeout(1000);
	serialPort.Open();
	this->SOH = 0x01;
	this->STX = 0x02;
	this->ETX = 0x03;
	this->EOT = 0x04;
	this->Axis = 0x30;
	this->ValidSentRec = false;
	this->TestMode = "0000";
	this->JogMode = "0001";
	this->PositionMode = "0002";
	this->MotorLessMode = "0003";
	this->ForcedMode = "0004";
	this->Speed = "0AFF";
	this->Acc = "7FFFFFFF";
	this->Forward = "00000807";
	this->Backward = "00001007";
	this->Stop = "00000007";
	this->print = false;
	this->ReceivedPacket = "";
	this->Tag = objectName;
}

// For J2S Motor
////////////////// NEW StartupSequenceJ2S Daniyal 20-Jan//////////////////////
void Packet::StartupSequenceJ2S(int usleep_time)
{
	std::cout << "----------" + Tag + "-----------" << std::endl;
	serialPort.Read(ReceivedPacket);
	FlushSerialBuffer();

	while (ValidSentRec == false)
	{
		SendGeneralPacket2("90", "00", "1EA5", Axis); // DisableExInputsPacket
		serialPort.Read(ReceivedPacket);
		usleep(usleep_time);
		pulseCountObject.ReadDisplay(ReceivedPacket, "DisableExInputsPacket");
		ValidSentRec = pulseCountObject.ReceivedDataProcessing(ReceivedPacket);
	}
	
	ValidSentRec = false;
	while (ValidSentRec == false)
	{
		SendGeneralPacket2("8B", "00", "0002", Axis); // Position Operation Enable
		usleep(usleep_time);
		serialPort.Read(ReceivedPacket);
		pulseCountObject.ReadDisplay(ReceivedPacket, "Position Operation Enable");
		ValidSentRec = pulseCountObject.ReceivedDataProcessing(ReceivedPacket);
	}

	ValidSentRec = false;
	while (ValidSentRec == false)
	{
		SendGeneralPacket2("01", "80", Axis); // Cummulative Feedback Pulses
		usleep(usleep_time);
		serialPort.Read(ReceivedPacket);
		pulseCountObject.ReadDisplay(ReceivedPacket, "Cummulative Feedback Pulses");
		ValidSentRec = pulseCountObject.ReceivedDataProcessing(ReceivedPacket);
	}

	ValidSentRec = false;
	while (ValidSentRec == false)
	{
		SendGeneralPacket2("01", "82", Axis); // Droop Pulses
		usleep(usleep_time);
		serialPort.Read(ReceivedPacket);
		pulseCountObject.ReadDisplay(ReceivedPacket, "Droop Pulses");
		ValidSentRec = pulseCountObject.ReceivedDataProcessing(ReceivedPacket);
	}
	// SendGeneralPacket2("01", "83", Axis); // Droop Pulses
	// usleep(usleep_time);
	// serialPort.Read(ReceivedPacket);
	// pulseCountObject.ReadDisplay(ReceivedPacket, "Droop Pulses");
	// pulseCountObject.ReceivedDataProcessing(ReceivedPacket);

	// SendGeneralPacket2("01", "8B", Axis); // Within one-revolution position
	// usleep(usleep_time);
	// serialPort.Read(ReceivedPacket);
	// pulseCountObject.ReadDisplay(ReceivedPacket, "Within one-revolution position");
	// pulseCountObject.ReceivedDataProcessing(ReceivedPacket);

	ValidSentRec = false;
	while (ValidSentRec == false)
	{
		SendGeneralPacket2("01", "8C", Axis); // ABS Counter
		usleep(usleep_time);
		serialPort.Read(ReceivedPacket);
		pulseCountObject.ReadDisplay(ReceivedPacket, "ABS Counter");
		ValidSentRec = pulseCountObject.ReceivedDataProcessing(ReceivedPacket);
	}
}

void Packet::SpeedAccelSetupJ2S(int usleep_time, int speed, int acceleration)
{
	SendGeneralPacket2("A0", "10", Convert(speed, 4), Axis); // Set Speed in RPM
	usleep(usleep_time);
	// std::cout<<Tag+" : ";
	// serialPort.Read(ReceivedPacket);
	// pulseCountObject.ReadDisplay(ReceivedPacket, "General3");
	SendGeneralPacket2("A0", "11", Convert(acceleration, 8), Axis); // acceleration time constant ms
	usleep(usleep_time);
	SendGeneralPacket2("92", "00", "00000007", Axis); // servo ON
	usleep(usleep_time);
}

void Packet::SendGeneralPacket2(std::string com, std::string dataNo, char stNo)
{
	ClearPacketStream();
	PacketStream << SOH << stNo << com << STX << dataNo << ETX;
	std::string checksum = CalCheckSum();
	PacketStream << checksum[1] << checksum[0];
	PacketString = PacketStream.str();
	// std::cout << PacketString << std::endl;
	serialPort.Write(PacketString);
	// serialPort.Read(ReceivedPacket);
	if (print)
	{
		std::cout << "This is the packet string: " << PacketString << std::endl;
	}
}

void Packet::SendGeneralPacket2(std::string com, std::string dataNo, std::string data, char stNo)
{
	ClearPacketStream();
	PacketStream << SOH << stNo << com << STX << dataNo << data << ETX;
	// std::cout << "Data Without CheckSum:";
	// std::cout << std::hex << PacketStream.str() << std::endl;
	std::string checksum = CalCheckSum();
	PacketStream << checksum[1] << checksum[0];
	PacketString = PacketStream.str();
	// std::cout << PacketString << std::endl;
	serialPort.Write(PacketString);
	// serialPort.Read(ReceivedPacket);
	if (print)
	{
		std::cout << "This is the packet string: " << PacketString << std::endl;
	} // std::cout << ReceivedPacket << std::endl;
}

void Packet::SendAllStatusReadPacket(void)
{
	std::string str = "80";
	for (int i = 0; i <= 12; i++)
	{
		SendGeneralPacket2("01", str, Axis);
		// std::cout << str << std::endl;
		if (str[1] == '9')
		{
			str[1] += 8;
		}
		else
		{
			str[1] += 1;
		}

		// usleep(0.08 * microsecond);
	}
}

void Packet::ClearPacketStream(void)
{
	PacketStream.str(std::string());
}

std::string Packet::CalCheckSum(void)
{
	int sum = 0;
	std::string str = PacketStream.str();
	// std::cout << str << std::endl;

	for (int i = 1; i < str.length(); i++)
	{
		sum = sum + str[i];
	}
	// std::cout << sum << std::endl;

	int last2cs = ((unsigned char *)&sum)[0];
	int cs1 = ((last2cs / 16) % 16);
	int cs2 = (last2cs % 16);
	// std::cout << cs1 << " " << cs2 << std::endl;

	std::string checkSumStr = "00";

	if (0 <= cs1 && cs1 <= 9)
	{
		checkSumStr[1] = cs1 + '0';
	}
	else
	{
		checkSumStr[1] = cs1 + '7';
	}
	if (0 <= cs2 && cs2 <= 9)
	{
		checkSumStr[0] = cs2 + '0';
	}
	else
	{
		checkSumStr[0] = cs2 + '7';
	}
	return checkSumStr;
}

std::string Packet::Stringpad(std::string stringer, int length, char character_pad)
{
	// std::cout<<"stringer.length"<<stringer.length()<<"\n"<<"length:"<<length<<"\n length - stringer.length(): "<<length - stringer.length();
	// Sheheryar edit
	if (character_pad == 'F')
	{
		return stringer.erase(0, length);
	}
	else
	{
		length = length - stringer.length();
		for (int i = 0; i < length; i++)
		{
			stringer = character_pad + stringer;
		}
		return stringer;
	}
	// Sheheryar edit

	// std::string string_to_pad;
	// string_to_pad=stringer;
	// if (character_pad =='F')
	// {
	//   string_to_pad.erase(0,length);
	//
	//   string_to_pad = string_to_pad;
	// }
	// else
	// {
	//   for (int i =0; i <length - stringer.length(); i++)
	//   {
	//     string_to_pad = character_pad + string_to_pad;
	//   }
	//
	// }// std::cout<<stringer.length()<<std::endl;
	// return string_to_pad;
}

std::string Packet::Convert(long int num, int bit_length)
{
	std::stringstream stream;
	stream << std::hex << std::uppercase << num;
	std::string result(stream.str());
	return result = (num < 0) ? Stringpad(result, bit_length, 'F') : result = Stringpad(result, bit_length, '0');
	// arg=result;
	// std::cout<<result;
	//    return result;
	// sheheryar edit
	// if (num<0)
	// {
	//   stream << std::hex <<std::uppercase<< num;
	//   // std::cout<<"wew\n";
	//   std::cout<<stream.str()<<std::endl;
	//   std::string result( stream.str() );
	//   // std::cout<<result;
	//   result = Stringpad(result, bit_length, 'F');
	//   // std::cout<<"pad\n";
	//   arg =result;
	//   std::cout<<arg<<std::endl;
	//
	//
	//
	// }
	// else
	// {
	//   stream << std::hex <<std::uppercase<< num;
	//   std::string result( stream.str() );
	//   result = Stringpad(result, bit_length, '0');
	//   arg=result;
	//
	// }
	// return arg;
}

void Packet::StopMotionJ2S(void)
{
	SendGeneralPacket2("90", "00", "1EA5", 0x30);
}

void Packet::FlushSerialBuffer()
{
	while (ReceivedPacket.length() > 0)
	{
		serialPort.Read(ReceivedPacket);
	}
}

void Packet::HeartbeatJ2S(int usleep_time, int loop_time)
{
	for (int i = 0; i < loop_time; i++)
	{
		SendGeneralPacket2("00", "12", 0x30);
		usleep(usleep_time);
	}
	SendGeneralPacket2("90", "00", "1EA5", 0x30);
}

void Packet::DegreeRotationJ2S(double degree, int usleep_time, int speed, double total_pulses)
{
	double second_data;
	double second_data2;
	double second_data_time;
	int loop_count2;
	int loop_count; // check value on different PCs
	double degree2;
	// double seconds =60 / ((double)speed/60);
	double degree_time;

	// calc = degree2/degree_per_minute * 60

	// double sleep_time = (seconds *degree2)/360;
	// degree = (degree*1317920)/6;
	if (total_pulses == 1310720)
	{
		if ((degree < -45) && (degree >= -90))
		{

			second_data = degree + 45;
			second_data2 = sqrt(pow(second_data, 2));
			second_data_time = (second_data2 / ((double)speed / 1.66667)) * 45;
			second_data = (second_data * total_pulses) / 6;

			loop_count2 = second_data_time * 13; // check value on different PCs

			degree2 = sqrt(pow(degree, 2));
			degree_time = (degree2 / ((double)speed / 1.66667)) * 45;
			loop_count = degree_time * 13;

			degree = (-45 * total_pulses) / 6;

			SendGeneralPacket2("A0", "13", Convert(degree, 8), Axis);
			HeartbeatJ2S(usleep_time, loop_count);

			SendGeneralPacket2("A0", "13", Convert(second_data, 8), Axis);
			HeartbeatJ2S(usleep_time, loop_count2);
		}

		else if ((degree > 45) && (degree <= 90))
		{

			second_data = degree - 45;
			second_data2 = sqrt(pow(second_data, 2));
			second_data_time = (second_data2 / ((double)speed / 1.66667)) * 45;
			second_data = (second_data * total_pulses) / 6;

			loop_count2 = second_data_time * 13; // check value on different PCs

			degree2 = sqrt(pow(degree, 2));
			degree_time = (degree2 / ((double)speed / 1.66667)) * 45;
			loop_count = degree_time * 20;

			degree = (45 * total_pulses) / 6;

			SendGeneralPacket2("A0", "13", Convert(degree, 8), Axis);
			HeartbeatJ2S(usleep_time, loop_count);

			SendGeneralPacket2("A0", "13", Convert(second_data, 8), Axis);
			HeartbeatJ2S(usleep_time, loop_count2);
		}

		else if ((degree <= 45) && (degree > 0)) // less than 45
		{
			degree2 = sqrt(pow(degree, 2));
			degree_time = (degree2 / ((double)speed / 1.66667)) * 45;
			loop_count = degree_time * 20;

			degree = (degree * total_pulses) / 6;
			// std::cout<<"Current degree pulse: "<<degree<<std::endl;
			SendGeneralPacket2("A0", "13", Convert(degree, 8), Axis); // Forward Moving Distance

			HeartbeatJ2S(usleep_time, loop_count);
		}
		else if ((degree >= -45) && (degree < 0))
		{

			degree2 = sqrt(pow(degree, 2));
			degree_time = (degree2 / ((double)speed / 1.66667)) * 45;
			loop_count = degree_time * 13;

			degree = (degree * total_pulses) / 6;
			// std::cout<<"Current degree pulse: "<<degree<<std::endl;
			SendGeneralPacket2("A0", "13", Convert(degree, 8), Axis); // Forward Moving Distance

			HeartbeatJ2S(usleep_time, loop_count);
		}

		else
		{
			std::cout << "Incorrect Degree Value Sent : " << std::endl;
		}
	}

	else if (total_pulses == 131072)
	{
		degree2 = sqrt(pow(degree, 2));
		degree_time = (degree2 / ((double)speed / 1.66667)) * 20;
		loop_count = degree_time * 4;

		degree = (degree * total_pulses) / 6;
		// std::cout<<"Current degree pulse: "<<degree<<std::endl;
		SendGeneralPacket2("A0", "13", Convert(degree, 8), Axis); // Forward Moving Distance

		HeartbeatJ2S(usleep_time, loop_count);

		// std::cout<<Convert(degree,8)<<std::endl;
	}

	else if (total_pulses == 8196)
	{
		degree2 = sqrt(pow(degree, 2));
		degree_time = (degree2 / ((double)speed / 1.66667)) * 30;
		loop_count = degree_time * 4;

		degree = (degree * total_pulses) / 6;
		// std::cout<<"Current degree pulse: "<<degree<<std::endl;
		SendGeneralPacket2("A0", "13", Convert(degree, 8), Axis); // Forward Moving Distance

		HeartbeatJ2S(usleep_time, loop_count);
		// SendAllStatusReadPacket();

		// std::cout<<Convert(degree,8)<<std::endl;
	}

	else
	{
		std::cout << "Incorrect Number of Pulses" << std::to_string(total_pulses) << std::endl;
	}

	// degree = (degree * total_pulses)/6;
	// std::cout<<"Current degree pulse: "<<degree<<std::endl;
	// SendGeneralPacket2("A0", "13", Convert(degree,8));	//Forward Moving Distance
	//
	// HeartbeatElevation(usleep_time, loop_count);

	// usleep(usleep_time);

	// std::cout<<Convert(degree,8)<<std::endl;
	// serialPort.Read(ReceivedPacket);
	// // serialPort.Read(ReceivedPacket);
	// // serialPort.Read(ReceivedPacket);
	// // serialPort.Read(ReceivedPacket);
	// // serialPort.Read(ReceivedPacket);
	// // serialPort.Read(ReceivedPacket);
	// SendGeneralPacket2("01", "83", Axis);
	// usleep(80000);
	// serialPort.Read(ReceivedPacket);
	// ReadDisplay(ReceivedPacket, "Cummulative Feedback Pulses after Motion: ");

	// // SendGeneralPacket2("81", "00", "1EA5", Axis);
	// // usleep(80000);
	// // serialPort.Read(ReceivedPacket);
	// // ReadDisplay(ReceivedPacket, "Setting Display Clear:");
	// // usleep(80000);

	// SendGeneralPacket2("01", "83", Axis);
	// usleep(80000);
	// serialPort.Read(ReceivedPacket);
	// ReadDisplay(ReceivedPacket, "Cummulative Feedback Pulses after Set Clear: ");
	// usleep(80000);
	usleep(usleep_time);
	std::cout << "_________" << std::endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////DegreeRotationJ2S/////////////////////////////////////
//////////////////////////////////Without electronic gear ratio///////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

// void Packet::HeartbeatAzimuth(int usleep_time, int loop_time)
// {
//
// 	for (int i =0; i<loop_time; i++)
// 	{
// 		SendGeneralPacket("00", "12");
// 		usleep(usleep_time);
// 	}
// 	SendGeneralPacket("90", "00", "1EA5");
// }

// void Packet::HeartbeatElevation(int usleep_time, int loop_time)
// {
// 	auto start = high_resolution_clock::now();
// 	for (int i =0; i<loop_time; i++)
// 	{
// 		SendGeneralPacket2("00", "12");
// 		usleep(usleep_time);
// 	}
// 	auto stop = high_resolution_clock::now();
// 	auto duration = duration_cast<microseconds>(stop - start);
// 	std::cout <<"duration: " <<duration.count() <<"\tloop time: " <<loop_time<<std::endl;
// 	SendGeneralPacket2("90", "00", "1EA5");
// }

// void Packet::SpeedAccelSetupAzimuth(int usleep_time,int  speed,int acceleration )
// {
// 	SendGeneralPacket("A0", "10",Convert(speed,4));
// 	std::cout<<"Speed: "<<Convert(speed,4)<<std::endl;	//speed r/min
// 	usleep(usleep_time);
// 	SendGeneralPacket("A0", "11", Convert(acceleration,8));
// 	std::cout<<"Acceleration: "<<Convert(acceleration,8)<<std::endl;	//acceleration time constant ms
// 	usleep(usleep_time);
// 	SendGeneralPacket("92", "00", "00000007");	//servo ON
// 	usleep(usleep_time);
// 	SendGeneralPacket("12", "80");
// 	usleep(usleep_time);
// 	SendGeneralPacket("12", "80");
// 	usleep(usleep_time);
// 	std::cout<<"test\n";
// }
//
// void Packet::SpeedAccelSetupElevation(int usleep_time,int speed,int acceleration )
// {
// 	SendGeneralPacket2("A0", "10",Convert(speed,4) );	//speed r/min
// 	usleep(usleep_time);
// 	SendGeneralPacket2("A0", "11", Convert(acceleration,8));	//acceleration time constant ms
// 	usleep(usleep_time);
// 	SendGeneralPacket2("92", "00", "00000007");	//servo ON
// 	usleep(usleep_time);
// 	SendGeneralPacket2("12", "80");
// 	usleep(usleep_time);
// 	SendGeneralPacket2("12", "80");
// 	usleep(usleep_time);
// }
// void Packet::DegreeRotationAzimuth( double degree, int usleep_time,int speed)
// {
// 	// std::cout<<degree<<std::endl;
// 	// std::cout<<speed<<std::endl;
// 	double degree2 = sqrt(pow(degree,2));
// 	std::cout<<"test 5"<<degree2<<speed<<std::endl;
//
// 	double seconds =60 / ((double)speed/60);
// 	std::cout<<"test 6"<<seconds<<std::endl;
// 	// std::cout<<degree 2<<seconds<<std::endl;
//
// 	double sleep_time = (seconds *degree2)/360;
// 	// std::cout<<"test3 \n";
//
// 	int loop_count =(sleep_time *1000)/(usleep_time/1000) ;
//
//
//
//
// 	degree = (degree*8192)/6;
// 	// std::cout<<"test 4"<<degree <<std::endl;
// 	SendGeneralPacket("A0", "13", Convert(degree,8));	//Forward Moving Distance
// 	// usleep(usleep_time);
// 	HeartbeatAzimuth(usleep_time,loop_count +1);
// 	// std::cout<<degree<<std:;endl;
// 	// std::cout<<speed<<std::endl;
//
// 	// std::cout<<Convert(degree,8)<<std::endl;
// }

// void Packet::DegreeRotationElevation(double degree, int usleep_time, int speed, double total_pulses)
// {
// 	double second_data;
// 	double second_data2;
// 	double second_data_time;
// 	int loop_count2;
//
// 	double degree2;
//
//
// 	// double seconds =60 / ((double)speed/60);
// 	double degree_time;
//
// 	// calc = degree2/degree_per_minute * 60
//
//
// 	// double sleep_time = (seconds *degree2)/360;
//
// 	int loop_count; // check value on different PCs
//
//
//
// 	// degree = (degree*1317920)/6;
//
// 	if ((degree <-45 ) && (degree >=-90))
// 	{
//
// 		second_data = degree +45;
// 		second_data2 = sqrt(pow(second_data,2));
// 		second_data_time = (second_data2/((double) speed/1.66667))*60;
// 		second_data = (second_data  *total_pulses)/6;
//
// 		loop_count2 = second_data_time * 13; // check value on different PCs
//
// 		degree2 = sqrt(pow(degree,2));
// 		degree_time = (degree2/((double) speed/1.66667))*60;
// 		loop_count = degree_time * 13;
//
// 		degree = (-45 * total_pulses)/6;
//
//
// 		SendGeneralPacket2("A0", "13", Convert(degree,8));
// 		HeartbeatElevation(usleep_time, loop_count);
//
// 		SendGeneralPacket2("A0", "13", Convert(second_data,8));
// 		HeartbeatElevation(usleep_time, loop_count2);
// 	}
//
// 	else if ((degree>45) && (degree<=90))
// 	{
//
// 				second_data = degree  -45;
// 				second_data2 = sqrt(pow(second_data,2));
// 				second_data_time = (second_data2/((double) speed/1.66667))*60;
// 				second_data = (second_data  *total_pulses)/6;
//
// 				loop_count2 = second_data_time * 13; // check value on different PCs
//
// 				degree2 = sqrt(pow(degree,2));
// 				degree_time = (degree2/((double) speed/1.66667))*60;
// 				loop_count = degree_time * 13;
//
//
//
// 				degree = (45 * total_pulses)/6;
//
//
// 				SendGeneralPacket2("A0", "13", Convert(degree,8));
// 				HeartbeatElevation(usleep_time, loop_count);
//
//
// 				SendGeneralPacket2("A0", "13", Convert(second_data,8));
// 				HeartbeatElevation(usleep_time, loop_count2);
//
// 	}
//
// 	else if ((degree<=45)  && (degree >0  )) //less than 45
// 	{
// 		degree2 = sqrt(pow(degree,2));
// 		degree_time = (degree2/((double) speed/1.66667))*60;
// 		loop_count = degree_time * 13;
//
//
//
// 		degree = (degree * total_pulses)/6;
// 		std::cout<<"Current degree pulse: "<<degree<<std::endl;
// 		SendGeneralPacket2("A0", "13", Convert(degree,8));	//Forward Moving Distance
//
// 		HeartbeatElevation(usleep_time, loop_count);
//
//
// 	}
// 	else if ((degree>=-45) && (degree <0))
// 	{
//
// 		degree2 = sqrt(pow(degree,2));
// 		degree_time = (degree2/((double) speed/1.66667))*60;
// 		loop_count = degree_time * 13;
//
// 		degree = (degree * total_pulses)/6;
// 		std::cout<<"Current degree pulse: "<<degree<<std::endl;
// 		SendGeneralPacket2("A0", "13", Convert(degree,8));	//Forward Moving Distance
//
// 		HeartbeatElevation(usleep_time, loop_count);
//
// 	}
//
// 	else
// 	{
// 		std::cout<<"Incorrect Degree Value Sent : "<<std::endl;
// 	}
//
//
//
//
// 	// degree = (degree * total_pulses)/6;
// 	// std::cout<<"Current degree pulse: "<<degree<<std::endl;
// 	// SendGeneralPacket2("A0", "13", Convert(degree,8));	//Forward Moving Distance
// 	//
// 	// HeartbeatElevation(usleep_time, loop_count);
//
//
// 	// usleep(usleep_time);
//
// 	// std::cout<<Convert(degree,8)<<std::endl;
// }

// void Packet::StopMotionAzimuth(void)
// {
// 	SendGeneralPacket("90", "00", "1EA5");
// }
//
// void Packet::StopMotionElevation(void)
// {
// 	SendGeneralPacket2("90", "00", "1EA5");
// }

// ////////////////////// OLD StartupSequenceJ2S //////////////////////
// void Packet::StartupSequenceJ2S (int usleep_time)
// {
// 	SendGeneralPacket2("90", "00", "1EA5"); //SendDisableExInputsPacket
// 	usleep(usleep_time);
// 	SendGeneralPacket2("8B", "00", "0002"); //Position Operation Enable
// 	usleep(usleep_time);
// 	 SendGeneralPacket2("92", "00", "00000000"); //Forward/Backward Direction // But working without it
// 	 usleep(usleep_time);
// 	 SendGeneralPacket2("12", "20");  //Start // But working without it
// 	 usleep(usleep_time);
// 	 SendGeneralPacket2("00", "20"); //SendDisableExInputsPacket // But working without it
// 	 usleep(usleep_time);
// 	 SendGeneralPacket2("00", "21"); //SendDisableExInputsPacket // But working without it
// 	 usleep(usleep_time);
// }

////////////////////////SendDisableExInputsPacket///////////////////
//////// It was placed above for testing but don't need anymore////////
// void Packet::SendDisableExInputsPacket(void)
// {
// 	// ClearPacketStream();
// 	SendGeneralPacket("90", "00", "1EA5");
// 	// this->PacketStream << this->SOH  << '9' << '0' << this->STX << '0' << '0' << '1' << 'E' << 'A' << '5' << this->ETX;
// 	// std::string checksum = CalCheckSum();
// 	// this->PacketStream << checksum[1] << checksum[0];
// 	// PacketString = PacketStream.str();
// }

////////////////////////SendSpeedPacket///////////////////
//////// It was placed above for testing but don't need anymore////////
// void Packet::SendSpeedPacket(std::string speed)
// {
// 	// ClearPacketStream();
// 	SendGeneralPacket("A0", "10", speed);
// 	// this->PacketStream << this->SOH << 'A' << '0' << this->STX << '1' << '0' << speed << this->ETX;
// 	// std::string checksum = CalCheckSum();
// 	// this->PacketStream << checksum[1] << checksum[0];
// 	// PacketString = PacketStream.str();
// }

////////////////////////SendModeSelect///////////////////
//////// It was placed above for testing but don't need anymore////////
// void Packet::SendModeSelect(std::string mode)
// {
// 	// ClearPacketStream();
// 	SendGeneralPacket("8B", "00", mode);
// 	// this->PacketStream << this->SOH << '8' << 'B' << this->STX << '0' << '0' << mode << this->ETX;
// 	// std::string checksum = CalCheckSum();
// 	// this->PacketStream << checksum[1] << checksum[0];
// 	// PacketString = PacketStream.str();
// }

////////////////////////SendServoOnPacket///////////////////
//////// It was placed above for testing but don't need anymore////////
// void Packet::SendServoOnPacket(void)
// {
// 	// ClearPacketStream();
// 	SendGeneralPacket("92", "00", "00000001");
// 	// this->PacketStream << this->SOH << '9' << '2' << this->STX << '0' << '0' << '0' << '0' << '0' << '0' << '0' << '0' << '0' << '1' << this->ETX;
// 	// std::string checksum = CalCheckSum();
// 	// this->PacketStream << checksum[1] << checksum[0];
// 	// PacketString = PacketStream.str();
// }

////////////////////////SendStrokeOnPacket///////////////////
//////// It was placed above for testing but don't need anymore////////
// void Packet::SendStrokeOnPacket(void){
// 	SendGeneralPacket("92", "00", "00000007");
// }

////////////////////////SendAccelerationPacket///////////////////
//////// It was placed above for testing but don't need anymore////////
// void Packet::SendAccelerationPacket(std::string acc){
// 	SendGeneralPacket("A0", "11", acc);
// }

////////////////////////SendDirectionPacket///////////////////
//////// It was placed above for testing but don't need anymore////////
// void Packet::SendDirectionPacket(std::string dir){
// 	SendGeneralPacket("92", "00", dir);
// }

////////////////////////SendMoveDistancePacket///////////////////
//////// It was placed above for testing but don't need anymore////////
// void Packet::SendMoveDistancePacket(std::string pulses)
// {
// 	SendGeneralPacket("A0", "13", pulses);
// }

////////////////////////SendParameterPacket///////////////////
//////// It was placed above for testing but don't need anymore////////
// void Packet::SendParameterPacket(std::string para)
// {
// 	SendGeneralPacket("05", para);
// }

////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
////////////////////////EVERYTHING BELOW IS FOR J2/////////////////

////////////////////////GeneralPacketForJ2Motor///////////////////
// void Packet::SendGeneralPacket(std::string com, std::string dataNo)
// {
// 	ClearPacketStream();
// 	PacketStream << SOH <<com << STX << dataNo << ETX;
// 	std::string checksum = CalCheckSum();
// 	PacketStream << checksum[1] << checksum[0];
// 	PacketString = PacketStream.str();
// 	serialPort.Write(PacketString);
// 	if (print)
// 	{
// 		std::cout<<"This is the packet string: "<< PacketString<<std::endl;
// 	}
// }

////////////////////////GeneralPacketWithDataForJ2Motor///////////////////
// void Packet::SendGeneralPacket(std::string com, std::string dataNo, std::string data)
// {
// 	ClearPacketStream();
// 	PacketStream << SOH <<com << STX << dataNo << data << ETX;
// 	std::string checksum = CalCheckSum();
// 	PacketStream << checksum[1] << checksum[0];
// 	PacketString = PacketStream.str();
// 	serialPort.Write(PacketString);
// 	// serialPort.Read(ReceivedPacket);
// 	if (print)
// 	{
// 		std::cout<<"This is the packet string: "<<PacketString << std::endl;
// 	}
// }

// void Packet::StartupSequenceJ2 (int usleep_time)
// {
// 	SendGeneralPacket("12", "20");
// 	usleep(usleep_time);
// 	SendGeneralPacket("92", "00", "00000000");
// 	usleep(usleep_time);
// 	SendGeneralPacket("90", "00", "1EA5");
// 	usleep(usleep_time);
// 	SendGeneralPacket("00", "20");
// 	usleep(usleep_time);
// 	SendGeneralPacket("00", "21");
// 	usleep(usleep_time);
// 	SendGeneralPacket("8B", "00", "0002");
// 	usleep(usleep_time);
// }

// void Packet::StopMotionJ2(void)
// {
// 	SendGeneralPacket("90", "00", "1EA5");
// }

// void Packet::HeartbeatJ2(int usleep_time, int loop_time)
// {
// 	for (int i =0; i<loop_time; i++)
// 	{
// 		SendGeneralPacket("00", "12");
// 		usleep(usleep_time);
// 	}
// 	SendGeneralPacket("90", "00", "1EA5");
// }

// void Packet::SpeedAccelSetupJ2(int usleep_time,int  speed,int acceleration )
// {
// 	SendGeneralPacket("A0", "10",Convert(speed,4));
// 	// std::cout<<"Speed: "<<Convert(speed,4)<<std::endl;	//speed r/min
// 	usleep(usleep_time);
// 	SendGeneralPacket("A0", "11", Convert(acceleration,8));
// 	// std::cout<<"Acceleration: "<<Convert(acceleration,8)<<std::endl;	//acceleration time constant ms
// 	usleep(usleep_time);
// 	SendGeneralPacket("92", "00", "00000007");	//servo ON
// 	usleep(usleep_time);
// 	SendGeneralPacket("12", "80");
// 	usleep(usleep_time);
// 	SendGeneralPacket("12", "80");
// 	usleep(usleep_time);
// 	// std::cout<<"test\n";
// }

// void Packet::DegreeRotationJ2( double degree, int usleep_time,int speed)
// {
// 	// std::cout<<degree<<std::endl;
// 	// std::cout<<speed<<std::endl;
// 	double degree2 = sqrt(pow(degree,2));
// 	// std::cout<<"test 5"<<degree2<<speed<<std::endl;

// 	double seconds =60 / ((double)speed/60);
// 	// std::cout<<"test 6"<<seconds<<std::endl;
// 	// std::cout<<degree 2<<seconds<<std::endl;
// 	double sleep_time = (seconds *degree2)/360;
// 	// std::cout<<"test3 \n";
// 	int loop_count =(sleep_time *1000)/(usleep_time/1000) ;

// 	degree = (degree*8192)/6;
// 	// std::cout<<"test 4"<<degree <<std::endl;
// 	SendGeneralPacket("A0", "13", Convert(degree,8));	//Forward Moving Distance
// 	// usleep(usleep_time);
// 	HeartbeatJ2(usleep_time,loop_count +1);
// 	// std::cout<<degree<<std:;endl;
// 	// std::cout<<speed<<std::endl;
// 	// std::cout<<Convert(degree,8)<<std::endl;
// }
