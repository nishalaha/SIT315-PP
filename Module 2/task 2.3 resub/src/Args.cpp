#include "Args.h"

#include <getopt.h>
#include <stdlib.h>

Args* Args::parse(int argc, char** argv)
{
	Args* a = new Args;

	char c;
	while ((c = getopt(argc, argv, "b:t:p:c:H:o:i")) != -1)
	{
		a->isParsed = true;

		switch (c)
		{
			case 'b': { a->bufferSize = atoi(optarg); break; }
			case 'c': { a->nConsumers = atoi(optarg); break; }
			case 't': { a->nTrafficSignals = atoi(optarg); break; }
			case 'p': { a->nProducers = atoi(optarg); break; }
			case 'H': { a->nHours = atoi(optarg); break; }
			case 'i': { a->inputFile = optarg; break; }
			case 'o': { a->outputFile = optarg; break; }
		}
	}

	return a;
}
