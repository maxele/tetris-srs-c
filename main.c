#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "logic.h"
#include "graphics.h"

int main(void) {
	srand(time(NULL));
	struct GameData *d = game_data_new(10, 20); // logic
	char /*input*/ i[10];

	while (!d->game_over) {
		get_input(d, i); // graphics
		do_input(d, i);  // logic
		show(d);         // graphics
		usleep(1000* 17);
	}
	show(d);
	return 0;
}
