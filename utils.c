#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

int is_int_in_array(int n, int array[]) {
    int i;

    if (array[0] == 0) {
        return 0;
    } else {
        for (i=array[0]; i>0; i--) {
            if (array[i] == n) {
                return 1;
            }
        }
    }
    return 0;
}

double expected(double a, double b) {
    double Qa = pow(10, a / 400);
    double Qb = pow(10, b / 400);
    return Qa / (Qa + Qb);
}

int rating_change(double a, double b, int k) {
    return (int) round(k * (1 - expected(a, b)));
}

int *read_csv(const char *filename) {
    int array_size = sizeof(int) * 3 * 100000;
    int *ints = malloc(array_size);
    char line[32];

    FILE *csv_file = fopen(filename, "r");

    int i = 0;

    while (!feof(csv_file)) {
        fgets(line, 32, csv_file);
        char *token = strtok(line, ",");

        while (token != NULL) {
            i++;
            if (i * sizeof(int) >= array_size - sizeof(int)) {
                array_size = array_size * 2;
                ints = realloc(ints, array_size);
            }
            ints[i] = atoi(token);
            token = strtok(NULL, ",");
        }
    }
    ints[0] = i;
    fclose(csv_file);

    return ints;
}

int *get_player_ids(int *raw_data) {
    int array_size = sizeof(int) * ((int) (raw_data[0] / 50));
    int *player_ids = malloc(array_size);

    player_ids[0] = 0;

    int i, j = 0;

    for (i=0; i<=raw_data[0]; i++) {
        if (j * sizeof(int) >= array_size - sizeof(int)) {
            array_size = array_size * 2;
            player_ids = realloc(player_ids, array_size);
        }
        if (i % 3 != 0) {
            if (!is_int_in_array(raw_data[i], player_ids)) {
                j++;
                player_ids[j] = raw_data[i];
                player_ids[0] = j;
            }
        }
    }

    return player_ids;
}

match *get_matches(int *raw_data) {
    int array_size = sizeof(match) * (raw_data[0] + 4) / 3;
    match *matches = malloc(array_size);

    int i, j = 0;

    for (i=1; i<raw_data[0]; i=i+3) {
        j++;
        if (j * sizeof(match) >= array_size - sizeof(match)) {
            array_size = array_size * 2;
            matches = realloc(matches, array_size);
        }
        matches[j].winner = raw_data[i];
        matches[j].loser = raw_data[i + 1];
        matches[j].date = raw_data[i + 2];
    }
    matches[0].winner = j;

    return matches;
}
