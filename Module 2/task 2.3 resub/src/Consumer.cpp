#include "Consumer.h"

std::atomic_int Consumer::nConsumed;

Consumer::Consumer(BoundedBuffer<Data>* _buffer, std::vector<Signal*>* _trafficSignals, Args* _args):
	buffer(_buffer), trafficSignals(_trafficSignals), args(_args)
{
}

void Consumer::start()
{
	pthread_create(&thread, nullptr, Consumer::doWork, this);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Consumer::init(Args* args)
{
}
#pragma GCC diagnostic pop

void* Consumer::doWork(void* arg)
{
	Consumer* work = (Consumer*)arg;
	int n = work->args->nProducers * work->args->nHours * 12;

	while (Consumer::nConsumed < n)
	{
		Data data = work->buffer->consume();
		(*(work->trafficSignals))[data.trafficSignalId - 1]->nCars += data.nCars;
		Consumer::nConsumed++;
	}

	return nullptr;
}
