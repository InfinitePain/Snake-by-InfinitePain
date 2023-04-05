/*
 ============================================================================
 Name        : timing_utils.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include "timing_utils.h"
#if _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <stdlib.h>
#endif

double get_current_time_in_seconds() {
#ifdef _WIN32
	LARGE_INTEGER freq, counter;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&counter);
	return (double)counter.QuadPart / (double)freq.QuadPart;
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
#endif
}

double calculate_elapsed_time(double start_time) {
	double end_time = get_current_time_in_seconds();
	return end_time - start_time;
}
void reset_timer(double* start_time, double* pause_total_time) {
	*start_time = get_current_time_in_seconds();
	*pause_total_time = 0;
}