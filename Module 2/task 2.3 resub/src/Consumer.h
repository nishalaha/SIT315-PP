#ifndef CONSUMER_H
#define CONSUMER_H

#include <atomic>
#include <vector>
#include <pthread.h>

#include "Args.h"
#include "Data.h"
#include "Signal.h"
#include "buf/BoundedBuffer.h"

class Consumer
{
private:
	BoundedBuffer<Data>* buffer;
	std::vector<Signal*>* trafficSignals;
	Args* args;
	pthread_t thread;
	static void* doWork(void* arg);

public:

	Consumer(BoundedBuffer<Data>* _buffer, std::vector<Signal*>* _trafficSignals, Args* _args);
	void start();
	static std::atomic_int nConsumed;
	static void init(Args* args);
};

#endif
