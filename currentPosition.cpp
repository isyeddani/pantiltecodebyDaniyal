#include "currentPosition.hpp"
#include <cmath>
#include <iostream>
#include <string.h>

using namespace std;

void pulseCount::ReadDisplay(std::string receivedPacket, std::string comment)
{
	std::cout << comment + ": ";
	if (receivedPacket.length() == 6)
	{
		std::cout << receivedPacket << std::endl;
	}
}
bool pulseCount::ReceivedDataProcessing(std::string receivedPacket)
{
	if (receivedPacket.length() == 6) // If only acknowledge received with checksum, Not for received data from servo
	{
		if (receivedPacket[2] != 0x41) // 0x41 is the normal operation mode, hence nothing will be done if received 41 at index 3
		{
			std::cout << errorCode[receivedPacket[2] - 0x41]; // Accessing the index of errorCode Array.
			return false;
		}
		if (receivedPacket[4] != 0x037 && receivedPacket[5] != 0x034) // If Normal operation received but checksum is not correct.
		{
			std::cout << "Correct Check Sum Not Received" << std::endl;
			return false;
		}
		return true;
	}
	else if (receivedPacket.length() == 18)
	{
		// Converting Hex String Value into Decimal Value
		std::stringstream newReceivedPacket;
		for (int i = receivedPacket.length() - 12; i < receivedPacket.length(); i++) // If the message length is 12, loop will start from 12 till end.
		{
			newReceivedPacket << receivedPacket[i]; // Packing words in stringstream, from 12 - 0
		}
		// std::cout<<" : "<<receivedPacket;
		int decVal;							// Thsi will store the Decimal value
		newReceivedPacket >> hex >> decVal; // String words will be converted in to Decimal and stored in decVal
		// std::cout <<" : ";
		std::cout << decVal << std::endl;
		return true;
	}
	else
	{
		std::cout << "Received Bad Ack" <<std::endl<<"Sending Again ..."<< std::endl;
		return false;
	}
}
int pulseCount::getAzimuthPulses()
{
	return 0;
}
void pulseCount::setAzimuthPulses(int azimuthPul)
{
}

int pulseCount::getAzimuthRevCount()
{
	return 0;
}
void pulseCount::setAzimuthRevCount(int azimuthRev)
{
}

int pulseCount::getElevationPulses()
{
	return 0;
}
void pulseCount::setElevationPulses(int elevationPul)
{
}

int pulseCount::getElevationRevCount()
{
	return 0;
}
void pulseCount::setElevationRevCount(int elevationRev)
{
}
