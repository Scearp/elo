#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "utils.h"

#define K 12

int main() {
    clock_t start;

    start = clock();

    int *raw_data = read_csv("matches.csv");
    int *players = get_players(raw_data);
    match *matches = get_matches(raw_data);
    free(raw_data);

    int i;
    int *ratings = malloc(sizeof(int) * (players[0] + 1));
    int *old_ratings = malloc(sizeof(int) * (players[0] + 1));

    for (i=0; i<=players[0]; i++) {
        ratings[i] = 1200;
    }

    int a, b, j, k;
    int wins[5] = {0};
    int tots[5] = {0};
    int last_date = 0;
    int weekly_players[10000];

    FILE *out = fopen("ratings.csv", "w");

    for (i=1; i<=matches[0].winner; i++) {
        if (matches[i].date != last_date) {
            for (j=0; j<=players[0]; j++) {
                old_ratings[j] = ratings[j];
            }
            if (last_date > 19950000) {
                for (j=0; j<=weekly_players[0]; j++) {
                    for (k=0; k<=players[0]; k++) {
                        if (weekly_players[j] == players[k]) {
                            fprintf(out, "%i,%i,%i\n", last_date, players[k], ratings[k]);
                        }
                    }
                }
            }
            for (j=0; j<10000; j++) {
                    weekly_players[j] = 0;
                }
        }

        for (j=0; j<=players[0]; j++) {
            if (players[j] == matches[i].winner) {
                a = j;
            }
        }
        for (k=0; k<=players[0]; k++) {
            if (players[k] == matches[i].loser) {
                b = k;
            }
        }
        if (matches[i].date > 19950000) {
            for (k=0; k<=4; k++) {
                if (old_ratings[a] - old_ratings[b] > 100 * k - 25 && old_ratings[a] - old_ratings[b] < 100 * k + 25) {
                    wins[k]++;
                    tots[k]++;
                }
                if (old_ratings[b] - old_ratings[a] > 100 * k - 25 && old_ratings[b] - old_ratings[a] < 100 * k + 25) {
                    tots[k]++;
                }
            }
        }

        int delta = rating_change((double) old_ratings[a],
                                  (double) old_ratings[b],
                                  K);

        ratings[a] = ratings[a] + delta;
        ratings[b] = ratings[b] - delta;

        if (!is_int_in_array(players[a], weekly_players)) {
            weekly_players[0]++;
            weekly_players[weekly_players[0]] = players[a];
        }
        if (!is_int_in_array(players[b], weekly_players)) {
            weekly_players[0]++;
            weekly_players[weekly_players[0]] = players[b];
        }

        last_date = matches[i].date;
    }

    for (j=0; j<=weekly_players[0]; j++) {
        for (k=0; k<=players[0]; k++) {
            if (weekly_players[j] == players[k]) {
                fprintf(out, "%i,%i,%i\n", last_date, players[k], ratings[k]);
            }
        }
    }

    free(players);
    free(matches);
    free(ratings);
    free(old_ratings);
    fclose(out);

    printf("%f\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);
    
    return 0;

}