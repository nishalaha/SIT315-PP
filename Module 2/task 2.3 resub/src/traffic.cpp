#include <iostream>
#include <algorithm>
#include <string.h>

#include "Args.h"
#include "Data.h"
#include "Signal.h"
#include "Producer.h"
#include "Consumer.h"
#include "buf/BoundedBuffer.h"

int main(int argc, char** argv)
{
	Args args = *Args::parse(argc, argv);

	char* progName = argv[0];
	if (progName[0] == '\0')
		progName = strdup("traffic");

	if (args.showHelp)
	{
		std::cout << "Usage:\n";
		std::cout << "  " << progName << " [-b BUFFER_SIZE] [-t TRAFFIC_SIGNALS] [-p PRODUCERS] [-c CONSUMERS]\n";
		std::cout << "    [-H HOURS] [-o OUTPUT]\n";
		std::cout << "  " << progName << " -i INPUT\n";
		std::cout << "  " << progName << " -h\n";

		Args def;

		std::cout << "\nArguments:\n";
		std::cout << "  -b BUFFER_SIZE     : " << def.bufferSize << ".\n";
		std::cout << "  -t TRAFFIC_SIGNALS : " << def.nTrafficSignals << ".\n";
		std::cout << "  -p PRODUCERS       : " << def.nProducers << ".\n";
		std::cout << "  -c CONSUMERS       : " << def.nConsumers << ".\n";
		std::cout << "  -H HOURS           : " << def.nHours << ".\n";
		std::cout << std::endl;

		return -1;
	}

	if (args.hasErrors)
	{
		std::cerr << "Run " << progName << " -h to view usage information." << std::endl;
		return -2;
	}

	if (args.bufferSize < 0 || args.nTrafficSignals < 0 || args.nProducers < 0 || args.nConsumers < 0 || args.nHours < 0)
	{
		std::cerr << "BUFFER_SIZE, TRAFFIC_SIGNALS, PRODUCERS, CONSUMERS, and HOURS cannot be negative.\n";
		std::cerr << "Please run " << progName << " -h to view usage information." << std::endl;
		return -3;
	}

	if (args.inputFile != nullptr && args.outputFile != nullptr)
	{
		std::cerr << "Both INPUT and OUTPUT cannot be specified simultaneously.\n";
		return -4;
	}

	srand(time(nullptr));

	Producer::init(&args);
	Consumer::init(&args);

	int n = args.nProducers * args.nHours * 12;

	std::cout << "Arguments:\n";
	std::cout << "  Buffer size: " << args.bufferSize << "\n";
	std::cout << "  Traffic signals: " << args.nTrafficSignals << "\n";
	std::cout << "  Producers: " << args.nProducers << "\n";
	std::cout << "  Consumers: " << args.nConsumers << "\n";
	std::cout << "  Hours: " << args.nHours << "\n";
	std::cout << "  Total traffic data generated: " << n << std::endl;

	BoundedBuffer<Data> buf(args.bufferSize);

	std::vector<Signal*> trafficSignals;
	for (int i = 0; i < args.nTrafficSignals; i++)
		trafficSignals.push_back(new TrafficSignal(i + 1));

	Producer* producers = (Producer*)malloc(sizeof(Producer) * args.nProducers);
	for (int i = 0; i < args.nProducers; i++)
	{
		producers[i] = Producer(&buf, &args);
		producers[i].start();
	}

	Consumer* consumers = (Consumer*)malloc(sizeof(Consumer) * args.nConsumers);
	for (int i = 0; i < args.nConsumers; i++)
	{
		consumers[i] = Consumer(&buf, &trafficSignals, &args);
		consumers[i].start();
	}

	while((n * 2) > (Producer::nProduced + Consumer::nConsumed));

	std::sort(trafficSignals.begin(), trafficSignals.end(), [] (Signal* l, Signal* r) {
		return l->nCars > r->nCars;
	});

	std::cout << "\nTraffic signals by descending number of cars passed by:\n";
	for (int i = 0; i < args.nTrafficSignals; i++)
		std::cout << "  TrafficSignal(id=" << trafficSignals[i]->id << ", nCars=" << trafficSignals[i]->nCars << ")" << std::endl;

	// Free malloc-ed memory.
	free(consumers);
	free(producers);
}

