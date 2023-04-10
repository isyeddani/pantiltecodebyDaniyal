#include "currentPosition.hpp"
// #include <cmath>
// #include <iostream>
// #include <string.h>
// #include <bitset>
// #include <bits/stdc++.h>

using namespace std;

PulseCount::PulseCount() {}

/// @brief
/// @param objectName
/// @param parsedirection
PulseCount::PulseCount(std::string ObjectTag)
{
	this->Tag = ObjectTag;
	this->OverLoadFlag = 0;
	this->RevCount = 0;
}

void PulseCount::ReadDisplay(std::string receivedPacket, std::string comment)
{
	// std::cout << comment + ":";
	if (receivedPacket.length() == 6)
	{
		std::cout << comment + ":";
		std::cout << receivedPacket << std::endl;
		return;
	}
	else if (receivedPacket.length() == 18)
	{
		std::cout << comment + ":";
		std::cout << receivedPacket << std::endl;
		return;
	}
	else
	{
		std::cout << comment + ":";
		std::cout << receivedPacket << std::endl;
	}
}

bool PulseCount::ReceivedDataProcessing(std::string receivedPacket, bool Process)
{
	if (receivedPacket.length() == 18)
	{
		if (Process)
		{
			std::stringstream newReceivedPacket;
			if (receivedPacket[7] != 'F')
			{
				// i = 18 - 12 = 4 This means ignoring first 4 characters.
				for (int i = receivedPacket.length() - 11; i < receivedPacket.length(); i++) // If the message length is 18, loop will start from 12 till end.
				{
					newReceivedPacket << receivedPacket[i]; // Packing words in stringstream, from 12 - 0
				}
				int decVal = 0;
				newReceivedPacket >> hex >> decVal;
				double degree = PulsestoDegree(decVal);
				setCurrentAngle(degree, true);
				std::cout << decVal << " Angle: " << getCurrentAngle() << std::endl;
				setRevCount(getCurrentAngle());
				std::cout << "Revolution:" << getRevCount() << std::endl;
				return true;
			}
			else
			{
				std::stringstream newReceivedPacket;
				unsigned int decVal = 0;
				for (int i = receivedPacket.length() - 11; i < receivedPacket.length(); i++) // If the message length is 12, loop will start from 12 till end.
				{
					newReceivedPacket << receivedPacket[i];
				}
				newReceivedPacket >> hex >> decVal;
				double degree = PulsestoDegree(onesComplement(decVal));
				// std::cout << ":" << onesComplement(decVal) << std::endl;
				setCurrentAngle(degree, false);
				std::cout << decVal << " Angle: " << getCurrentAngle() << std::endl;
				setRevCount(getCurrentAngle());
				std::cout << "Revolution:" << getRevCount() << std::endl;
				return true;
			}
		}
		else
		{
			std::stringstream newReceivedPacket;
			if (receivedPacket[7] != 'F')
			{
				for (int i = receivedPacket.length() - 12; i < receivedPacket.length(); i++) // If the message length is 12, loop will start from 12 till end.
				{
					newReceivedPacket << receivedPacket[i]; // Packing words in stringstream, from 12 - 0
				}
				int decVal = 0;
				newReceivedPacket >> hex >> decVal;
				std::cout << decVal << std::endl;
				return true;
			}
			else
			{
				std::stringstream newReceivedPacket;
				unsigned int decVal = 0;
				for (int i = receivedPacket.length() - 12; i < receivedPacket.length(); i++) // If the message length is 12, loop will start from 12 till end.
				{
					newReceivedPacket << receivedPacket[i];
				}
				newReceivedPacket >> hex >> decVal;
				std::cout << decVal << std::endl;
				return true;
			}
		}
	}
	else if (receivedPacket.length() == 6 && !Process) // If only acknowledge received with checksum, Not for received data from servo
	{
		bool check = true;
		if (receivedPacket[2] != 0x41) // 0x41 is the normal operation mode, hence nothing will be done if received 41 at index 3
		{
			std::cout << errorCode[receivedPacket[2] - 0x41]; // Accessing the index of errorCode Array.
			check = false;
		}
		else if (receivedPacket[4] != 0x037 && receivedPacket[5] != 0x034) // If Normal operation received but checksum is not correct.
		{
			std::cout << "Correct Check Sum Not Received" << std::endl;
			check = false;
		}
		return check;
	}
	else
	{
		std::cout << Tag << " Received Bad Ack "
				  << "Sending Again -----" << std::endl;
		return false;
	}
}

unsigned int PulseCount::onesComplement(unsigned int n)
{
	vector<int> v;
	// convert to binary representation
	while (n != 0)
	{
		v.push_back(n % 2);
		n = n / 2;
	}
	reverse(v.begin(), v.end());
	// change 1's to 0 and 0's to 1
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] == 0)
			v[i] = 1;
		else
			v[i] = 0;
	}
	// convert back to number representation
	int two = 1;
	for (int i = v.size() - 1; i >= 0; i--)
	{
		n = n + v[i] * two;
		two = two * 2;
	}
	return n;
}

double PulseCount::PulsestoDegree(unsigned int Pulses)
{
	if (Tag == "Azimuth") // Calculation for Azimuth
	{
		double pulPerRev = 131072;
		double Val = (Pulses / pulPerRev) * 6; // One revolution gives 6 degrees
		// Return Degrees
		return Val;
	}
	else if (Tag == "Elevation") // Calculation for Elevation
	{
		double pulPerRev = 1310720;
		double Val = (Pulses / pulPerRev) * 6; // 10 Revolution gives 6 degrees
		// Return Degrees
		return Val;
	}
	else
	{
		std::cout << "ERROR: Invalid Tag In Pulse to Degree" << std::endl;
		return 0; // Return Nothing
	}
}

///////////////////////////////////////////////////////
//////////////////Get Current Angle////////////////////
///////////////////////////////////////////////////////
double PulseCount::getCurrentAngle()
{
	return CurrentAngle;
}

///////////////////////////////////////////////////////
//////////////////Setting Current Angle////////////////
///////////////////////////////////////////////////////
void PulseCount::setCurrentAngle(double NewAngle, bool directionFlag)
{
	// std::cout << "Tag:" << Tag << " NewAngle:" << NewAngle << std::endl;
	if (Tag == "Elevation")
	{
		// Temp Gives the Current Revolution wrt Drive.
		int Temp = round(fabs(CurrentAngle)) / 45.7763;
		if (directionFlag)
		{
			CurrentAngle = (((OverLoadFlag)*45.7763) + NewAngle);
			return;
		}
		else
		{
			CurrentAngle = (((OverLoadFlag)*45.7763) + NewAngle);
			CurrentAngle = (-1) * CurrentAngle;
			return;
		}
	}
	else if (Tag == "Azimuth")
	{
		// Temp Gives the Current Revolution wrt Drive.
		int Temp = round(fabs(CurrentAngle)) / 457.763;
		if (directionFlag)
		{
			CurrentAngle = (((OverLoadFlag)*457.763) + NewAngle);
			return;
		}
		else
		{
			CurrentAngle = (((OverLoadFlag)*457.763) + NewAngle);
			CurrentAngle = (-1) * CurrentAngle;
			return;
		}
	}
	else
	{
		std::cout << "Error: Invalid Tag During Setting Current Angle" << std::endl;
		return;
	}
}

void PulseCount::LiveAngleCalculation(double NewAngle, bool directionFlag)
{
	// std::cout << "Tag:" << Tag << " NewAngle:" << NewAngle << std::endl;
	if (Tag == "Elevation")
	{
		// Temp Gives the Current Revolution wrt Drive.
		int Temp = round(fabs(CurrentAngle)) / 45.7763;
		if (directionFlag)
		{
			CurrentAngle = (((OverLoadFlag)*45.7763) + NewAngle);
			return;
		}
		else
		{
			CurrentAngle = (((OverLoadFlag)*45.7763) + NewAngle);
			CurrentAngle = (-1) * CurrentAngle;
			return;
		}
	}
	else if (Tag == "Azimuth")
	{
		// Temp Gives the Current Revolution wrt Drive.
		int Temp = round(fabs(CurrentAngle)) / 457.763;
		if (directionFlag)
		{
			CurrentAngle = (((OverLoadFlag)*457.763) + NewAngle);
			return;
		}
		else
		{
			CurrentAngle = (((OverLoadFlag)*457.763) + NewAngle);
			CurrentAngle = (-1) * CurrentAngle;
			return;
		}
	}
	else
	{
		std::cout << "Error: Invalid Tag During Setting Current Angle" << std::endl;
		return;
	}
}

///////////////////////////////////////////////////////
//////////////////Setting Revolution Count/////////////
///////////////////////////////////////////////////////
void PulseCount::setRevCount(double Angle)
{
	if (Tag == "Elevation")
	{
		Angle = fabs(Angle);
		int check = round(Angle) / 45.7763;
		switch (check)
		{
		case 0:
			RevCount = 0;
			break;
		case 1:
			RevCount = 1;
			break;
		case 2:
			RevCount = 2;
			break;
		default:
			std::cout << "ERROR: Setting Revolution Counter" << std::endl;
		}
	}
	else if (Tag == "Azimuth")
	{
		Angle = fabs(Angle);
		int check = round(Angle) / 360;
		switch (check)
		{
		case 0:
			RevCount = 0;
			break;
		case 1:
			RevCount = 1;
			break;
		case 2:
			RevCount = 2;
			break;
		case 3:
			RevCount = 3;
			break;
		default:
			std::cout << "ERROR: Setting Revolution Counter" << std::endl;
		}
	}
	else
	{
		std::cout << "ERROR: Invalid Tag During Setting Revolution Counter" << std::endl;
		return;
	}
}

///////////////////////////////////////////////////////
//////////////////Gives Revolution Count/////////////
///////////////////////////////////////////////////////
double PulseCount::getRevCount()
{
	return RevCount;
}