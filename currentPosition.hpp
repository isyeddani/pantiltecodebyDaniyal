#include <CppLinuxSerial/SerialPort.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cmath>
#include <chrono>

using namespace mn::CppLinuxSerial;
using namespace std::chrono;

class pulseCount{
    private:
        int azimuthPulses;
        int azimauthRevCounts;
        int elevationRevCounts;
        int elevationPulses;
    public:
        void set_azimuthPulses(int set_azimuthPulses);
        void set_elevationPulses(int set_elevationPulses);
        void set_elevationRevCounts(int set_elevationRevCounts);
        void set_RevCounts(int set_RevCounts);
        void get_azimuthPulses(int set_azimuthPulses);
        void get_elevationPulses(int set_elevationPulses);
        void get_elevationRevCounts(int set_elevationRevCounts);
        void get_RevCounts(int set_RevCounts);
}
