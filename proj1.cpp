/*
	Written by Adam Sunderman for OSU CS475 Spring 2018
	Project 1 - Bézier Surfaces Area

	*Requirements
		1. Using OpenMP, compute the total volume betwen the two surfaces. 
		
		2. Use a variety of number of subdivisions (NUMNODES). Pick at least 8 different ones.
		
		3. Use a variety of number of threads (NUMT). You must use at least 1, 2, and 4. 
		
		4. Record the data in units of something that gets larger as speed increases. Joe Parallel 
	   	   used "MegaHeights Computed Per Second", but you can use anything that makes sense. 
		
		5. From the speed-up that you are seeing, use the "Inverse Amdahl's Law" to determine the 
	   	   Parallel Fraction for this application. 
    	
    	6. From the Parallel Fraction, determine what maximum speed-up you could ever get, even 
       	   with a million cores. 
 
    **Your commentary write-up (turned in as a PDF file) should include: 
		1. Tell what machine you ran this on.
		
		2. What do you think the actual volume is?
		
		3. Show the performances you achieved in tables and graphs as a function of NUMNODES 
		   and NUMT.
		
		4. What patterns are you seeing in the speeds?
		
		5. Why do you think it is behaving this way?
		
		6. What is the Parallel Fraction for this application, using the Inverse Amdahl 
		   equation?
		
		7. Given that Parallel Fraction, what is the maximum speed-up you could ever get?
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include "height.h"
#include "lib_cs475.h"


int main(int argc, char const *argv[]){

	// Check for OpenMP support. Set number of threads to NUMT
	// if the system has the cores.
	int sysMaxThreads = 0; 
	if(openmp_supported()){
		if(omp_get_num_procs() >= NUMT){
			omp_set_num_threads(NUMT);
		}
	}
	else{
		fprintf(stderr, "%s\n", "Sorry OpenMP is not supported here.");
		return 1;
	}
	
	// Sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:
	float ftArea = FullTileArea();
	float volume = 0;
	long int i = 0;
	long int iu = 0; 
	long int iv = 0;
	float currHeight = 0; 
	float currVolume = 0;

	// Start time.
	double timeIn = omp_get_wtime();

	#pragma omp parallel for reduction(+:volume), default(none), schedule(dynamic), private(iu, iv, currHeight, currVolume), shared(ftArea, i)
	for(i = 0; i < NUMNODES*NUMNODES; i++){
		iu = i % NUMNODES;
		iv = i / NUMNODES;
		currHeight = Height(iu,iv);

		if(iu == 0 || iu == NUMNODES-1){
			if(iv == 0 || iv == NUMNODES-1){
				// Corner Tile
				currVolume = currHeight * (ftArea * .25);
			}
			else{
				// Edge Tile
				currVolume = currHeight * (ftArea * .5);
			}
		}
		else if(iv == 0 || iv == NUMNODES-1){
			if(iu == 0 || iu == NUMNODES-1){
				// Corner Tile
				currVolume = currHeight * (ftArea * .25);
			}
			else{
				// Edge Tile
				currVolume = currHeight * (ftArea * .5);
			}
		}
		else{
			// Full Tile
			currVolume = currHeight * ftArea;
		}
		volume += currVolume;
	}

	// End time.
	double timeOut = omp_get_wtime();
	double diff = (timeOut-timeIn);

	FILE * outfile = fopen("results.txt","a");
	if(outfile != NULL){
		fprintf(outfile, "The volume is %.8f, using a %d node subdivision and %d threads.\nThe calculation took %10.8lf microseconds\n\n", volume,NUMNODES,NUMT,diff);
		fclose(outfile);
	}
	
	return 0;
}