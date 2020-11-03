#ifndef PRODUCER_H
#define PRODUCER_H

#include <atomic>
#include <fstream>
#include <pthread.h>

#include "Args.h"
#include "Data.h"
#include "buf/BoundedBuffer.h"

class Producer
{
private:

	BoundedBuffer<Data>* buffer;
	Args* args;
	static std::fstream fileStream;
	static pthread_mutex_t fileAccessMutex;
	pthread_t thread;
	static void* doWork(void* arg);

public:
	Producer(BoundedBuffer<Data>* _buffer, Args* _args);
	void start();
	static std::atomic_int nProduced;
	static void init(Args* args);
};

#endif
