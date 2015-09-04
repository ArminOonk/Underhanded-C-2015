#include <string.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

// DO NOT MODIFY
#define BINS 25
#define LIBRARY_SIZE 3
double baseline[BINS] = {	
1.4827, 0.0438, 0.9608, 1.7382, 0.4302, 
1.6273, 0.1663, 0.3763, 0.2270, 1.1489, 
2.0243, 2.3595, 0.5100, 1.3216, 0.6361, 
0.3179, 0.1380, 0.7107, 0.7770, 0.6224,
0.6474, 0.4256, 1.0486, 0.6607, 2.5088
};

double plutonium[BINS] = {	
1.4827, 0.0438, 0.9608, 1.7382, 0.4302, 
1.6273, 3.1663, 9.3763, 5.2270, 2.1489, 
2.0243, 2.3595, 0.5100, 1.3216, 0.6361, 
0.3179, 1.1380, 3.7107, 1.7770, 0.6224,
0.6474, 0.4256, 1.0486, 0.6607, 2.5088
};
							
double uranium[BINS] = {	
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

struct reference_t library[LIBRARY_SIZE] = 
{
	{"Baseline", BINS, baseline},
	{"Plutonium", BINS, plutonium},
	{"Uranium", BINS, uranium},
};

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
	
	//printf("Correlation: %0.2f\r\n", correlation);
	
	if(correlation > threshold)
	{
		return 1;
	}
	return 0;
}

void selfTest()
{
	for(int i=0; i<LIBRARY_SIZE; i++)
	{
		for(int j=0; j<LIBRARY_SIZE; j++)
		{
			int res = match(library[i].test, library[j].test, BINS, 0.8);
			printf("Selftest %s vs %s %s with score %0.2f\r\n", library[i].name, library[j].name, res ? "Match" : "No match", correlation );
		}
	}
	// Cross reference samples
	/*printf("Testing: baseline vs baseline %d\r\n", match(baseline, baseline, BINS, 0.8));
	printf("Testing: plutonium  vs baseline %d\r\n", match(plutonium, baseline, BINS, 0.8));
	printf("Testing: uranium  vs baseline: %d\r\n", match(uranium, baseline, BINS, 0.8));
	
	printf("Testing: baseline vs plutonium %d\r\n", match(baseline, plutonium, BINS, 0.8));
	printf("Testing: plutonium  vs plutonium %d\r\n", match(plutonium, plutonium, BINS, 0.8));
	printf("Testing: uranium  vs plutonium: %d\r\n", match(uranium, plutonium, BINS, 0.8));
	
	printf("Testing: baseline vs uranium %d\r\n", match(baseline, uranium, BINS, 0.8));
	printf("Testing: plutonium  vs uranium %d\r\n", match(plutonium, uranium, BINS, 0.8));
	printf("Testing: uranium  vs uranium: %d\r\n", match(uranium, uranium, BINS, 0.8));*/
}
int main(int argc, char **argv)
{
	selfTest();
	
	double TwoTimesPu[BINS];
	for(int i=0; i<BINS; i++)
	{
		TwoTimesPu[i] = 2.0*plutonium[i];
	}
	
	printf("Testing: plutonium  vs TwoTimesPu %d\r\n", match(plutonium, TwoTimesPu, BINS, 0.8));
}
