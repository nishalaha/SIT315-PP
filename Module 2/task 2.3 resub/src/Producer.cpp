#include "Producer.h"

std::atomic_int Producer::nProduced;
std::fstream Producer::fileStream;
pthread_mutex_t Producer::fileAccessMutex;

Producer::Producer(BoundedBuffer<Data>* _buffer, Args* _args): buffer(_buffer), args(_args)
{
}

void Producer::start()
{
	pthread_create(&thread, nullptr, Producer::doWork, this);
}

void Producer::init(Args* args)
{
	Producer::nProduced = { 0 };
	pthread_mutex_init(&Producer::fileAccessMutex, nullptr);

	if (args->inputFile != nullptr)
	{
		Producer::fileStream.open(args->inputFile, std::ios::in);

		Producer::fileStream >> args->bufferSize;
		Producer::fileStream >> args->nTrafficSignals;
		Producer::fileStream >> args->nProducers;
		Producer::fileStream >> args->nConsumers;
		Producer::fileStream >> args->nHours;
	}
	else if (args->outputFile != nullptr)
	{
		Producer::fileStream.open(args->outputFile, std::ios::out);

		// Write args.
		Producer::fileStream << args->bufferSize << '\n';
		Producer::fileStream << args->nTrafficSignals << '\n';
		Producer::fileStream << args->nProducers << '\n';
		Producer::fileStream << args->nConsumers << '\n';
		Producer::fileStream << args->nHours << '\n';
	}
}

void* Producer::doWork(void* arg)
{
	Producer* work = (Producer*)arg;
	time_t now = time(nullptr);

	if (work->args->inputFile != nullptr)
	{
		while (Producer::fileStream.is_open())
		{
			int n = work->args->nProducers * work->args->nHours * 12;

			pthread_mutex_lock(&Producer::fileAccessMutex);

			if (n > Producer::nProduced)
			{
				Data d;
				Producer::fileStream >> d;
				work->buffer->produce(d);

				Producer::nProduced++;
			}

			pthread_mutex_unlock(&Producer::fileAccessMutex);
		}
	}
	else
	{
		int n = work->args->nHours * 12;
		for (int i = 0; i < n; i++)
		{
			TrafficData d = { now + (60 * 5 * i), (rand() % work->args->nTrafficSignals) + 1, (rand() % 10) + 1 };

			if (work->args->outputFile != nullptr)
			{
				pthread_mutex_lock(&Producer::fileAccessMutex);

				if (Producer::fileStream.is_open())
					Producer::fileStream << d << '\n';

				pthread_mutex_unlock(&Producer::fileAccessMutex);
			}

			work->buffer->produce(d);
			Producer::nProduced++;
		}
	}

	return nullptr;
}
