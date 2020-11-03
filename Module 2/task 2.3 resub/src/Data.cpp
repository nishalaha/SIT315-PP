#include "Data.h"

std::ostream& operator<<(std::ostream& stream, Data& data)
{
	stream << data.timestamp << '\t' << data.trafficSignalId << '\t' << data.nCars;
	return stream;
}

std::istream& operator>>(std::istream& stream, Data& data)
{
	stream >> data.timestamp >> data.trafficSignalId >> data.nCars;
	return stream;
}
