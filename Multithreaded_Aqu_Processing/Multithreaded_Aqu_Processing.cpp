// Multithreaded_Aqu_Processing.cpp : main project file.
/*
HW 5
CSE 5343_4342
Due March 23rd, 2012
Sean Pierce

*/

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>  
#define LINES_IN_FILTER 101
#define LINES_IN_SIGNAL 6001

float filter_value[LINES_IN_FILTER];
float signal_value[LINES_IN_SIGNAL];
float output_value[LINES_IN_SIGNAL];


int main()
{
	char x;

    printf("Opening Files...\n");
	FILE  *Filter_90Hz, *Signal_100Hz;
	Filter_90Hz = fopen("FIR.txt", "r");
	Signal_100Hz = fopen("100Hz.txt", "r");

	if(Filter_90Hz == NULL || Signal_100Hz == NULL){
		printf("There was a problem reading one of the files.\nExiting\n");
		scanf("%c", &x);
		return 0;
	}

	printf("Reading Files...\n");
	char line[128];
	
	
	//while(fgets(line,sizeof line, Filter_90Hz) != NULL){
	//doesn't work becuase it hangs on the last line
	int i = 0;
	int j = 0;
	float temp_value;
	for(i = 0; i < LINES_IN_FILTER; i++){
		fscanf(Filter_90Hz,"%f\n",&temp_value);// != NULL);
		filter_value[i] = temp_value;
		//printf("%.22f on line %d\n", filter_value[i], i);
	}

	//pulling info from signal file
	for(i = 0; i < LINES_IN_SIGNAL; i++){
		fscanf(Signal_100Hz,"%f\n",&temp_value);// != NULL);
		signal_value[i] = temp_value;
		//printf("%.22f on line %d\n", signal_value[i], i);
	}

	//initiallizing output_value array
	for(i = 0; i < LINES_IN_SIGNAL; i++)
		output_value[i] = 0;

	//the filter formula: sum(filter() * signal )
	float sum = 0.0;
#pragma omp parallel for private(j)
	for(i = 100; i < LINES_IN_SIGNAL; i++){
		for(j = 0; j < LINES_IN_FILTER; j++){
			output_value[i] += signal_value[i - j] * filter_value[j];
		}
	}

	float max = 0.0;
	float min = 0.0;
	float average = 0.0;
	for(i = 100; i < LINES_IN_SIGNAL; i++){
		if(output_value[i] < min)
			min = output_value[i];
		if(output_value[i] > max)
			max = output_value[i];
		
	}
	printf("%.14f and %.14f\n", min, max);


	for(i = 100; i < LINES_IN_SIGNAL; i++){
		if((output_value[i] >= output_value[i -1]) && (output_value[i] >= output_value[i+1]))
			average += output_value[i];
			
	}
	
	printf("Average: %f\n", average);

	//closing files
	fclose(Filter_90Hz);
	fclose(Signal_100Hz);

	//pause before ending
	scanf("%c", &x);

    return 0;
}
