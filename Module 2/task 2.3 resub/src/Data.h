#ifndef DATA_H
#define DATA_H

#include <ctime>
#include <istream>
#include <ostream>


struct TrafficData
{
	time_t timestamp;
	int trafficSignalId;
	int nCars;
	friend std::ostream& operator<<(std::ostream& stream, Data& data);
	friend std::istream& operator>>(std::istream& stream, Data& data);
};

#endif
