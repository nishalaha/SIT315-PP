#ifndef SIGNAL_H
#define SIGNAL_H

#include <atomic>

class Signal
{
public:
	const int id;
	std::atomic_int nCars = { 0 };
	Signal(int _id);
};

#endif
