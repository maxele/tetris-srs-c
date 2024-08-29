#ifndef LOGIC_H
#define LOGIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Point {
	int x, y;
};

struct GameData {
	char *field;
	char w, h, cp, cr, hp;
	char game_over;
	struct Point cpos;
	char bag[14];
	int pc; // pieces count
};

enum {
	NoPiece,
	IPiece,
	TPiece,
	OPiece,
	JPiece,
	LPiece,
	SPiece,
	ZPiece,
	GPiece
};

enum {
	CMD_Left,
	CMD_Right,
	CMD_Clockwise,
	CMD_CounterClockwise,
	CMD_OneEighty,
	CMD_SoftDrop,
	CMD_HardDrop,
	CMD_Hold,
	CMD_Reset,
	CMD_Up
};

struct GameData *game_data_new(char /*width*/ w, char /*height*/ h);
void show_stdout(struct GameData *d);
char collides(struct GameData *d, char /*rotation*/ r, struct Point /*offset*/ o);
void /*drop the current piece*/ drop(struct GameData *d);
void /*place the current piece*/ place(struct GameData *d);
void /*execute the given input*/ do_input(struct GameData *d, char /*input*/ i[10]);
void next_piece(struct GameData *d);
void generate_bag(char *b);

static struct Point pieces[8][4][4] = {/*{{{*/
	[IPiece] = {
		[0] = { [0] = {0, 1}, [1] = {1, 1}, [2] = {2, 1}, [3] = {3, 1} },
		[1] = { [0] = {2, 0}, [1] = {2, 1}, [2] = {2, 2}, [3] = {2, 3} },
		[2] = { [0] = {0, 2}, [1] = {1, 2}, [2] = {2, 2}, [3] = {3, 2} },
		[3] = { [0] = {1, 0}, [1] = {1, 1}, [2] = {1, 2}, [3] = {1, 3} }
	},
	[TPiece] = {
		[0] = { [0] = {0, 1}, [1] = {1, 1}, [2] = {2, 1}, [3] = {1, 0} },
		[1] = { [0] = {1, 2}, [1] = {1, 1}, [2] = {2, 1}, [3] = {1, 0} },
		[2] = { [0] = {1, 2}, [1] = {1, 1}, [2] = {2, 1}, [3] = {0, 1} },
		[3] = { [0] = {1, 2}, [1] = {1, 1}, [2] = {1, 0}, [3] = {0, 1} }
	},
	[OPiece] = {
		[0] = { [0] = {1, 1}, [1] = {2, 1}, [2] = {1, 0}, [3] = {2, 0} },
		[1] = { [0] = {1, 1}, [1] = {2, 1}, [2] = {1, 0}, [3] = {2, 0} },
		[2] = { [0] = {1, 1}, [1] = {2, 1}, [2] = {1, 0}, [3] = {2, 0} },
		[3] = { [0] = {1, 1}, [1] = {2, 1}, [2] = {1, 0}, [3] = {2, 0} }
	},
	[JPiece] = {
		[0] = { [0] = {0, 1}, [1] = {1, 1}, [2] = {2, 1}, [3] = {0, 0} },
		[1] = { [0] = {1, 0}, [1] = {1, 1}, [2] = {2, 0}, [3] = {1, 2} },
		[2] = { [0] = {0, 1}, [1] = {1, 1}, [2] = {2, 1}, [3] = {2, 2} },
		[3] = { [0] = {1, 0}, [1] = {1, 1}, [2] = {0, 2}, [3] = {1, 2} },
	},
	[LPiece] = {
		[0] = { [0] = {0, 1}, [1] = {1, 1}, [2] = {2, 1}, [3] = {2, 0} },
		[1] = { [0] = {1, 0}, [1] = {1, 1}, [2] = {2, 2}, [3] = {1, 2} },
		[2] = { [0] = {0, 1}, [1] = {1, 1}, [2] = {2, 1}, [3] = {0, 2} },
		[3] = { [0] = {1, 0}, [1] = {1, 1}, [2] = {0, 0}, [3] = {1, 2} },
	},
	[SPiece] = {
		[0] = { [0] = {1, 0}, [1] = {2, 0}, [2] = {0, 1}, [3] = {1, 1} },
		[1] = { [0] = {1, 0}, [1] = {2, 1}, [2] = {2, 2}, [3] = {1, 1} },
		[2] = { [0] = {1, 1}, [1] = {2, 1}, [2] = {0, 2}, [3] = {1, 2} },
		[3] = { [0] = {1, 0}, [1] = {2, 1}, [2] = {2, 2}, [3] = {1, 1} },
	},
	[ZPiece] = {
		[0] = { [0] = {0, 0}, [1] = {1, 0}, [2] = {1, 1}, [3] = {2, 1} },
		[1] = { [0] = {2, 0}, [1] = {1, 2}, [2] = {1, 1}, [3] = {2, 1} },
		[2] = { [0] = {0, 1}, [1] = {1, 1}, [2] = {1, 2}, [3] = {2, 2} },
		[3] = { [0] = {1, 0}, [1] = {0, 2}, [2] = {0, 1}, [3] = {1, 1} },
	},
};/*}}}*/

/* Rotation tables {{{*/
static struct Point jltszrotations[4][4][4] = {
	[0] = {
		[1] = {{-1, 0}, {-1,-1}, { 0, 2}, {-1, 2}},
		[3] = {{ 1, 0}, { 1,-1}, { 0, 2}, { 1, 2}}
	},
	[1] = {
		[0] = {{ 1, 0}, { 1, 1}, { 0,-2}, { 1,-2}},
		[2] = {{ 1, 0}, { 1, 1}, { 0,-2}, { 1,-2}}
	},
	[2] = {
		[1] = {{-1, 0}, {-1,-1}, { 0, 2}, {-1, 2}},
		[3] = {{ 1, 0}, { 1,-1}, { 0, 2}, { 1, 2}}
	},
	[3] = {
		[2] = {{-1, 0}, {-1, 1}, { 0,-2}, {-1,-2}},
		[0] = {{-1, 0}, {-1, 1}, { 0,-2}, {-1,-2}}
	}
};

static struct Point irotations[4][4][4] = {
	[0] = {
		[1] = {{-2, 0}, { 1, 0}, {-2, 1}, { 1,-2}},
		[3] = {{-1, 0}, { 2, 0}, {-1,-2}, { 2, 1}}
	},
	[1] = {
		[0] = {{ 2, 0}, {-1, 0}, { 2,-1}, {-1, 2}},
		[2] = {{-1, 0}, { 2, 0}, {-1,-2}, { 2, 1}}
	},
	[2] = {
		[1] = {{ 1, 0}, {-2, 0}, { 1, 2}, {-2,-1}},
		[3] = {{ 2, 0}, {-1, 0}, { 2,-1}, {-1, 2}}
	},
	[3] = {
		[2] = {{-2, 0}, { 1, 0}, {-2, 1}, { 1,-2}},
		[0] = {{ 1, 0}, {-2, 0}, { 1, 2}, {-2,-1}}
	}
};
/*}}}*/

#endif // LOGIC_H
