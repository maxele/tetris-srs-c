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

#define CMD_COUNT 10
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

/*
 * Allocate and initialize the GameData struct
 */
struct GameData *game_data_new(char w, char h);
/*
 * Show the field using stdout
 */
void show_stdout(struct GameData *d);
/*
 * Check if the current piece collides with another piece
 */
char collides(struct GameData *d, char /*rotation*/ r, struct Point /*offset*/ o);
/*
 * Place down the current piece
 * Clear filled lines
 * Advance curent piece
 */
void place(struct GameData *d);
/*
 * Rotate the current piece
 * If it's not possible try the SRS table
 */
void rotate(struct GameData *d, char /*roatation*/ r);
/*
 * Generate 7 pieces and shuffle them
 */
void generate_bag(char *b);
/*
 * Take piece from bag
 * If necessary regenerate bag
 */
void next_piece(struct GameData *d);
/*
 * Handle input
 */
void do_input(struct GameData *d, char /*input*/ i[CMD_COUNT]);

static struct Point LOGIC_PIECE_DATA[8][4][4] = {/*{{{*/
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
		[0] = { [0] = {0, 1}, [1] = {1, 0}, [2] = {1, 1}, [3] = {2, 0} },
		[1] = { [0] = {1, 0}, [1] = {2, 2}, [2] = {1, 1}, [3] = {2, 1} },
		[2] = { [0] = {2, 1}, [1] = {1, 1}, [2] = {0, 2}, [3] = {1, 2} },
		[3] = { [0] = {0, 0}, [1] = {1, 2}, [2] = {0, 1}, [3] = {1, 1} },
	},
	[ZPiece] = {
		[0] = { [0] = {0, 0}, [1] = {1, 0}, [2] = {1, 1}, [3] = {2, 1} },
		[1] = { [0] = {2, 0}, [1] = {1, 2}, [2] = {1, 1}, [3] = {2, 1} },
		[2] = { [0] = {0, 1}, [1] = {1, 1}, [2] = {1, 2}, [3] = {2, 2} },
		[3] = { [0] = {1, 0}, [1] = {0, 2}, [2] = {0, 1}, [3] = {1, 1} },
	},
};/*}}}*/

static struct Point SRS_NORMAL_TABLE[4][4][4] = {/*{{{*/
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
};/*}}}*/

static struct Point SRS_IPIECE_TABLE[4][4][4] = {/*{{{*/
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
};/*}}}*/

#endif // LOGIC_H
