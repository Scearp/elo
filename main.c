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
    int *player_ids = get_player_ids(raw_data);
    match *matches = get_matches(raw_data);
    free(raw_data);

    int i;
    player *players = malloc(sizeof(player) * (player_ids[0] + 1));
    player *old_players = malloc(sizeof(player) * (player_ids[0] + 1));

    for (i=0; i<=player_ids[0]; i++) {
        players[i].player = player_ids[i];
        players[i].rating = 1200;
    }

    int a, b, j, k;
    int wins[5] = {0};
    int tots[5] = {0};
    int last_date = 0;
    int weekly_players[10000];

    FILE *out = fopen("ratings.csv", "w");

    for (i=1; i<=matches[0].winner; i++) {
        if (matches[i].date != last_date) {
            for (j=0; j<=player_ids[0]; j++) {
                old_players[j].player = players[j].player;
                old_players[j].rating = players[j].rating;
            }
            if (last_date > 19950000) {
                for (j=0; j<=weekly_players[0]; j++) {
                    for (k=0; k<=player_ids[0]; k++) {
                        if (weekly_players[j] == player_ids[k]) {
                            fprintf(out, "%i,%i,%i\n", last_date, players[k].player, players[k].rating);
                        }
                    }
                }
            }
            for (j=0; j<10000; j++) {
                    weekly_players[j] = 0;
                }
        }

        for (j=0; j<=player_ids[0]; j++) {
            if (player_ids[j] == matches[i].winner) {
                a = j;
            }
        }
        for (k=0; k<=player_ids[0]; k++) {
            if (player_ids[k] == matches[i].loser) {
                b = k;
            }
        }
        if (matches[i].date > 19950000) {
            for (k=0; k<=4; k++) {
                if (old_players[a].rating - old_players[b].rating > 100 * k - 25 && old_players[a].rating - old_players[b].rating < 100 * k + 25) {
                    wins[k]++;
                    tots[k]++;
                }
                if (old_players[b].rating - old_players[a].rating > 100 * k - 25 && old_players[b].rating - old_players[a].rating < 100 * k + 25) {
                    tots[k]++;
                }
            }
        }

        int delta = rating_change((double) old_players[a].rating,
                                  (double) old_players[b].rating,
                                  K);

        players[a].rating = players[a].rating + delta;
        players[b].rating = players[b].rating - delta;

        if (!is_int_in_array(players[a].player, weekly_players)) {
            weekly_players[0]++;
            weekly_players[weekly_players[0]] = players[a].player;
        }
        if (!is_int_in_array(players[b].player, weekly_players)) {
            weekly_players[0]++;
            weekly_players[weekly_players[0]] = players[b].player;
        }

        last_date = matches[i].date;
    }

    for (j=0; j<=weekly_players[0]; j++) {
        for (k=0; k<=player_ids[0]; k++) {
            if (weekly_players[j] == player_ids[k]) {
                fprintf(out, "%i,%i,%i\n", last_date, players[k].player, players[k].rating);
            }
        }
    }

    free(player_ids);
    free(matches);
    free(players);
    free(old_players);
    fclose(out);

    printf("%f\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);
    
    return 0;

}