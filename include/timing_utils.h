/*
 ============================================================================
 Name        : timing_utils.h
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Header file for Snake-by-InfinitePain
=============================================================================
*/

#ifndef TIMING_UTILS_H_
#define TIMING_UTILS_H_

double get_current_time_in_seconds();
double calculate_elapsed_time(double start_time);
void reset_timer(double* start_time, double* pause_total_time);

#endif /*TIMING_UTILS_H_*/