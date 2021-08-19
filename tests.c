#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

int main() {

    //expected()
    assert(expected(1200, 1200) == 0.5);
    assert(round(expected(1300, 1200) * 100) == 64);
    assert(round(expected(1200, 1300) * 100) == 36);

    //rating_change()
    assert(rating_change(1200, 1200, 24) == 12);
    assert(rating_change(1300, 1200, 24) != 12);
    assert(rating_change(1200, 1300, 24) == 15);

    //read_csv()
    int *ids = read_csv("test.csv");
    assert(ids[0] == 10 * 3);
    assert(ids[1] == 200569);
    assert(ids[ids[0] - 1] == 200628);
    

    //get_player_ids()
    int *pids = get_player_ids(ids);
    free(ids);

    //is_int_in_array()
    int array[4] = {2, 1, 3};
    assert(is_int_in_array(3, array));
    assert(!is_int_in_array(2, array));
    assert(!is_int_in_array(4, array));

    return 0;
    
}