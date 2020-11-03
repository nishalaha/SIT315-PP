#ifndef ARGS_H
#define ARGS_H

struct Args
{
	bool isParsed = false;

	bool hasErrors = false;

	int bufferSize = 5;
	int nTrafficSignals = 4;
	int nProducers = 4;
	int nConsumers = 2;
	int nHours = 1;
	char* outputFile = nullptr;
	char* inputFile = nullptr;
	bool showHelp = false;

	static Args* parse(int argc, char** argv);
};

#endif
