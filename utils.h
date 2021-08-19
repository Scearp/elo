#ifndef UTILS_H
#define UTILS_H

#include "defines.h"

int is_int_in_array(int n, int array[]);
double expected(double a, double b);
int rating_change(double a, double b, int k);
int *read_csv(char *filename);
int *get_player_ids(int *raw_data);
match *get_matches(int *raw_data);

#endif