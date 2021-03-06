#include <string.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

// DO NOT MODIFY
#define BINS 25
#define LIBRARY_SIZE 3
#define INPUT_BUFFER_ALLOC 1024
#define NEWLINE_SIGNAL_DISPLAY 5

const double baseline[BINS] = {	
1.4827, 0.0438, 0.9608, 1.7382, 0.4302, 
1.6273, 0.1663, 0.3763, 0.2270, 1.1489, 
2.0243, 2.3595, 0.5100, 1.3216, 0.6361, 
0.3179, 0.1380, 0.7107, 0.7770, 0.6224,
0.6474, 0.4256, 1.0486, 0.6607, 2.5088
};

const double plutonium[BINS] = {	
1.4827, 0.0438, 0.9608, 1.7382, 0.4302, 
1.6273, 3.1663, 9.3763, 5.2270, 2.1489, 
2.0243, 2.3595, 0.5100, 1.3216, 0.6361, 
0.3179, 1.1380, 3.7107, 1.7770, 0.6224,
0.6474, 0.4256, 1.0486, 0.6607, 2.5088
};
							
const double uranium[BINS] = {	
1.4827, 3.0438, 7.9608, 13.738, 5.4302, 
1.6273, 0.1663, 0.3763, 0.2270, 1.1489, 
2.0243, 2.3595, 0.5100, 1.3216, 0.6361, 
0.3179, 3.1380, 5.7107, 5.7770, 2.6224,
0.6474, 0.4256, 1.0486, 0.6607, 2.5088
};
// END DO NOT MODIFY

struct reference_t
{
	char name[255];
	int bins;
	double *test;
};

struct reference_t library[LIBRARY_SIZE];

double correlation;
int match( double * test, double * reference, int bins, double threshold )
{
	// Your detector should output a 1 (“yes”) if these two arrays are sufficiently similar, and 0 (“no”) otherwise;
	// Calculate correlation, if you need this link DO NOT TOUCH THE CODE: https://en.wikipedia.org/wiki/Correlation_and_dependence
	double meanTest = 0.0;
	double meanReference = 0.0;
	for(int i=0; i<bins ; i++)
	{
		meanTest += (test[i]/(double)bins);
		meanReference += (reference[i]/(double)bins);
	}

	double cov = 0.0;
	double devTest = 0.0;
	double devReference = 0.0;
	for(int i=0; i<bins ; i++)
	{
		double errorTest = test[i]-meanTest;
		double errorReference = reference[i] - meanReference;
		cov += errorTest*errorReference;
		devTest += errorTest*errorTest;
		devReference += errorReference*errorReference;
	}
	
	double r = cov/sqrt(devTest*devReference);
	correlation = r*r;
	
	if(correlation > threshold)
	{
		return 1;
	}
	return 0;
}

void addToLibrary(int index, char name[], const double *test, int bins)
{
	strcpy(library[index].name, name);
	int size = bins*sizeof(double);
	library[index].test = (double*)malloc(size);
	memcpy(library[index].test, test, size);
	library[index].bins = bins;
}

void removeFromLibrary(int index)
{
	free(library[index].test);
}

void init()
{
	// Make working copies from the secure signal database
	addToLibrary(0, "Baseline", (double*)baseline, BINS);
	addToLibrary(1, "Plutonium", (double*)plutonium, BINS);
	addToLibrary(2, "Uranium", (double*)uranium, BINS);
	//
}

void cleanUp()
{
	for(int i=0; i<LIBRARY_SIZE; i++)
	{
		removeFromLibrary(i);
	}
}

void selfTest()
{
	for(int i=0; i<LIBRARY_SIZE; i++)
	{
		for(int j=0; j<LIBRARY_SIZE; j++)
		{
			int expectedRes = (i == j) ? 1 : 0;
			int res = match(library[i].test, library[j].test, BINS, 0.8);
			
			printf("Selftest %s %s vs %s %s with score %0.2f\r\n", 
				(expectedRes==res) ? "PASSED" : "FAILED", library[i].name, library[j].name, res ? "Match" : "No match", correlation );
		}
	}
}

double randomDouble(double min, double max)
{
	double r = (double)rand()/(double)RAND_MAX;
	return min + (max-min)*r;
}

void readInputData(double *test, int bins)
{
	int dataRead = 0;
	int currentBin = 0;
	char inputBuffer[INPUT_BUFFER_ALLOC];
	memset(inputBuffer, 0x00, INPUT_BUFFER_ALLOC);
	
	char ch;
	while(read(STDIN_FILENO, &ch, 1) > 0 && currentBin < bins)
	{
		if(ch == ',')
		{
			test[currentBin] = atof(inputBuffer);
			currentBin++;
			dataRead = 0;
			memset(inputBuffer, 0x00, INPUT_BUFFER_ALLOC);
		}
		else
		{
			inputBuffer[dataRead++] = ch;
		}
	}
	
	if(dataRead != 0)
	{
		test[currentBin] = atof(inputBuffer); // Get the last one
	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	init();
	
	time_t timer;
    char buffer[26];
    struct tm* tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
	
	// Start report
	printf("*****************************************************\r\n");
	printf("* Nuclear disarmament treaty                        *\r\n");
	printf("* the Peoples Glorious Democratic Republic of Alice *\r\n");
	printf("* the Glorious Democratic Peoples Republic of Bob   *\r\n");
	printf("*****************************************************\r\n");
	printf("* Report generated on: %s          * \r\n", buffer);
	printf("*****************************************************\r\n");
	
	selfTest();
	
	printf("*****************************************************\r\n");
	printf("* Testing current sample                            *\r\n");
	printf("*****************************************************\r\n");
	// Read data from detector from stdin
	double test[BINS];
	readInputData(test, BINS);
	
	#ifdef DEBUG
	printf("Sample raw data:\r\n");
	for(int i=0; i<BINS; i++)
	{
		printf("%0.2f ", test[i]);
		if(i%NEWLINE_SIGNAL_DISPLAY == (NEWLINE_SIGNAL_DISPLAY-1) )
		{
			printf("\r\n");
		}
	}
	printf("\r\n");
	#endif
	
	bool isFissile = false;
	
	for(int i=1; i<LIBRARY_SIZE; i++)
	{
		int res = match(test, library[i].test, BINS, 0.8);
		if(res == 1)
		{
			isFissile = true;
		}
	}

	bool isBaseline = ( match(test, (double*)baseline, BINS, 0.8) == 1);
	
	if(isFissile && !isBaseline)
	{
		printf("Nuclear material detected\r\n");
	}
	else
	{
		printf("NO Nuclear material detected\r\n");
	}
	
	cleanUp();
}
