#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include "logic.h"

// controls
static int KEY_CMD_Left = SDL_SCANCODE_K;
static int KEY_CMD_Right = SDL_SCANCODE_SEMICOLON;
static int KEY_CMD_Clockwise = SDL_SCANCODE_D;
static int KEY_CMD_CounterClockwise = SDL_SCANCODE_A;
static int KEY_CMD_OneEighty = SDL_SCANCODE_S;
static int KEY_CMD_SoftDrop = SDL_SCANCODE_L;
static int KEY_CMD_HardDrop = SDL_SCANCODE_SPACE;
static int KEY_CMD_Hold = SDL_SCANCODE_LSHIFT;
static int KEY_CMD_Reset = SDL_SCANCODE_R;
static int KEY_CMD_Up = SDL_SCANCODE_O;

static SDL_Color GRAPHICS_BACKGROUND_COLOR = { 48, 62, 69 };
static SDL_Color GRAPHICS_FOREGROUND_COLOR = { 64, 83, 92 };

static SDL_Color GRAPHICS_PIECE_COLORS[] = {
	[NoPiece] = (SDL_Color){},
	[IPiece] = (SDL_Color){17, 193, 190},
	[TPiece] = (SDL_Color){193, 7, 181},
	[OPiece] = (SDL_Color){193, 168, 7},
	[JPiece] = (SDL_Color){7, 17, 193},
	[LPiece] = (SDL_Color){193, 103, 7},
	[SPiece] = (SDL_Color){7, 193, 38},
	[ZPiece] = (SDL_Color){193, 57, 17},
	[GPiece] = (SDL_Color){30, 38, 42}
};

static struct GraphicsSettings {
	int /*width*/ w, /*height*/ h;
	int /*mino width*/ mw;
} gs;

/*
 * Initialize GraphicsSettings and SDL
 */
void graphics_init(int w, int h);

/*
 * Draw a specific piece at (x, y)
 */
void show_piece(int p, int r, int mw, /*mino width*/
		int x, int y, char is_ghost);

/*
 * Draw the whole window using GameData
 */
void show(struct GameData *d);

/*
 * Get an array of commands
 * Timings are hardcoded and based on fps
 */
void get_input(struct GameData *d, char input[CMD_COUNT]);

#endif // GRAPHICS_H
