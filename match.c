#include <string.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

// DO NOT MODIFY
#define BINS 25
#define LIBRARY_SIZE 3
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

struct reference_t library[LIBRARY_SIZE];/* = 
{
	{"Baseline", BINS, (double*)baseline},
	{"Plutonium", BINS, (double*)plutonium},
	{"Uranium", BINS, (double*)uranium},
};*/

double correlation;
int match( double * test, double * reference, int bins, double threshold )
{
	//Your detector should output a 1 (“yes”) if these two arrays are sufficiently similar, and 0 (“no”) otherwise;
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

void init()
{
	addToLibrary(0, "Baseline", (double*)baseline, BINS);
	addToLibrary(1, "Plutonium", (double*)plutonium, BINS);
	addToLibrary(2, "Uranium", (double*)uranium, BINS);
}

void cleanUp()
{
	for(int i=0; i<LIBRARY_SIZE; i++)
	{
		free(library[i].test);
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

int main(int argc, char **argv)
{
	srand(time(NULL));
	
	init();
	//selfTest();
	
	double ranTest[BINS];
	double amplitude = 2.0;
	
	for(int x=0; x<10; x++)
	{
		for(int i=0; i<BINS; i++)
		{
			ranTest[i] = plutonium[i] + randomDouble(0.0, amplitude);
		}
		
		int expectedRes = 1;
		int res = match(ranTest, (double*)plutonium, BINS, 0.8);
				
		printf("Selftest %s %s vs %s %s with score %0.2f\r\n", 
				(expectedRes==res) ? "PASSED" : "FAILED", "ranTest", "plutonium", res ? "Match" : "No match", correlation );
	}
	
	/*
	double TwoTimesPu[BINS];
	for(int i=0; i<BINS; i++)
	{
		TwoTimesPu[i] = 2.0*plutonium[i];
	}
	
	printf("Testing: plutonium  vs TwoTimesPu %d\r\n", match((double*)plutonium, TwoTimesPu, BINS, 0.8));
	*/
	cleanUp();
}
