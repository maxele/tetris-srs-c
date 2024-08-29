#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "logic.h"
#include "graphics.h"

SDL_Color piece_colors[] = {
	[NoPiece] = (SDL_Color){109, 132, 120},
	[IPiece] = (SDL_Color){17, 193, 190},
	[TPiece] = (SDL_Color){193, 7, 181},
	[OPiece] = (SDL_Color){193, 168, 7},
	[JPiece] = (SDL_Color){7, 17, 193},
	[LPiece] = (SDL_Color){193, 103, 7},
	[SPiece] = (SDL_Color){7, 193, 38},
	[ZPiece] = (SDL_Color){193, 57, 17},
	[GPiece] = (SDL_Color){30, 38, 42}
};

SDL_Renderer *renderer = NULL;
SDL_Window *window;

struct GraphicsSettings {
	int /*width*/ w, /*height*/ h;
	int /*mino width*/ mw;
} gs;

int KEY_CMD_Left = SDL_SCANCODE_K;
int KEY_CMD_Right = SDL_SCANCODE_SEMICOLON;
int KEY_CMD_Clockwise = SDL_SCANCODE_D;
int KEY_CMD_CounterClockwise = SDL_SCANCODE_A;
int KEY_CMD_OneEighty = SDL_SCANCODE_S;
int KEY_CMD_SoftDrop = SDL_SCANCODE_L;
int KEY_CMD_HardDrop = SDL_SCANCODE_SPACE;
int KEY_CMD_Hold = SDL_SCANCODE_LSHIFT;
int KEY_CMD_Reset = SDL_SCANCODE_R;
int KEY_CMD_Up = SDL_SCANCODE_O;

void graphics_init(int w, int h)
{/*{{{*/
	gs.w = w;
	gs.h = h;
	gs.mw = 30;
	int rendererFlags = SDL_RENDERER_ACCELERATED,
		windowFlags = 0;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, windowFlags);
	if (!window)
	{
		printf("Failed to open %d x %d window: %s\n", w, h, SDL_GetError());
		exit(1);
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	renderer = SDL_CreateRenderer(window, -1, rendererFlags);
	if (!renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}
}/*}}}*/

void show_piece(int p, int r, int w, int x, int y, char is_ghost) {
	if (is_ghost)
		SDL_SetRenderDrawColor(renderer, piece_colors[p].r*0.7, piece_colors[p].g*0.7, piece_colors[p].b*0.7, 255);
	else
		SDL_SetRenderDrawColor(renderer, piece_colors[p].r, piece_colors[p].g, piece_colors[p].b, 255);
	for (int i = 0; i < 4; i++) {
		SDL_Rect rect = {
			x + pieces[p][r][i].x * w,
			y + pieces[p][r][i].y * w,
			w, w
		};
		SDL_RenderFillRect(renderer, &rect);
	}
}

void show(struct GameData *d)
{/*{{{*/
	if (renderer == NULL) graphics_init(800, 800);
	SDL_SetRenderDrawColor(renderer, 96, 128, 255, 255);
	SDL_RenderClear(renderer);

	// Layout:
	// display held piece
	SDL_Rect hold = {gs.mw, gs.mw, gs.mw*5, gs.mw*5};
	SDL_SetRenderDrawColor(renderer, piece_colors[NoPiece].r, piece_colors[NoPiece].g, piece_colors[NoPiece].b, 255);
	SDL_RenderFillRect(renderer, &hold);
	if (d->hp)
		show_piece(d->hp, 0, gs.mw, hold.x+gs.mw/2, hold.y+gs.mw/2, 0);
	SDL_Rect field = {hold.x + hold.w + gs.mw, hold.y, d->w*gs.mw, d->h*gs.mw};
	SDL_SetRenderDrawColor(renderer, piece_colors[NoPiece].r, piece_colors[NoPiece].g, piece_colors[NoPiece].b, 255);
	SDL_RenderFillRect(renderer, &field);
	// display the field
	for (int i = 0; i < d->w * d->h; i++) {
		SDL_SetRenderDrawColor(renderer, piece_colors[d->field[i]].r, piece_colors[d->field[i]].g, piece_colors[d->field[i]].b, 255);
		SDL_Rect r = { field.x + i % d->w * gs.mw, field.y + i / d->w * gs.mw, gs.mw, gs.mw };
		SDL_RenderFillRect(renderer, &r);
	}
	// display current piece and ghost piece
	int dy = 1;
	while (!collides(d, d->cr, (struct Point){0, dy})) {
		dy++;
	}
	dy--;
	show_piece(d->cp, d->cr, gs.mw, field.x + d->cpos.x*gs.mw, field.y + (d->cpos.y+dy)*gs.mw, 1);
	show_piece(d->cp, d->cr, gs.mw, field.x + d->cpos.x*gs.mw, field.y + d->cpos.y*gs.mw, 0);
	// display preview
	SDL_Rect preview = {field.x + field.w + gs.mw, field.y, gs.mw*5, gs.mw*5};
	for (int i = 0; i < 5; i++) {
		show_piece(d->bag[(d->pc + i) % 14], 0, gs.mw*3/4, preview.x+gs.mw*3/8, preview.y+gs.mw*3/8 + gs.mw*15/4*i, 0);
	}
	SDL_RenderPresent(renderer);
}/*}}}*/

char input_buffer[10];
void get_input(struct GameData *d, char input[10])
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
				break;
			default:
				break;
		}
	}
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	input_buffer[CMD_Left] = keystates[KEY_CMD_Left] ? input_buffer[CMD_Left]+1 : 0;
	input[CMD_Left] = input_buffer[CMD_Left] == 1 || input_buffer[CMD_Left] > 5;
	input_buffer[CMD_Right] = keystates[KEY_CMD_Right] ? input_buffer[CMD_Right]+1 : 0;
	input[CMD_Right] = input_buffer[CMD_Right] == 1 || input_buffer[CMD_Right] > 5;
	input_buffer[CMD_Up] = keystates[KEY_CMD_Up] ? input_buffer[CMD_Up]+1 : 0;
	input[CMD_Up] = input_buffer[CMD_Up] == 1 || input_buffer[CMD_Up] > 5;
	input_buffer[CMD_SoftDrop] = keystates[KEY_CMD_SoftDrop] ? input_buffer[CMD_SoftDrop]+1 : 0;
	input[CMD_SoftDrop] = input_buffer[CMD_SoftDrop] == 1 || input_buffer[CMD_SoftDrop] > 5;

	input_buffer[CMD_Clockwise] = keystates[KEY_CMD_Clockwise] ? input_buffer[CMD_Clockwise]+1 : 0;
	input[CMD_Clockwise] = input_buffer[CMD_Clockwise] == 1;
	input_buffer[CMD_CounterClockwise] = keystates[KEY_CMD_CounterClockwise] ? input_buffer[CMD_CounterClockwise]+1 : 0;
	input[CMD_CounterClockwise] = input_buffer[CMD_CounterClockwise] == 1;
	input_buffer[CMD_OneEighty] = keystates[KEY_CMD_OneEighty] ? input_buffer[CMD_OneEighty]+1 : 0;
	input[CMD_OneEighty] = input_buffer[CMD_OneEighty] == 1;
	input_buffer[CMD_HardDrop] = keystates[KEY_CMD_HardDrop] ? input_buffer[CMD_HardDrop]+1 : 0;
	input[CMD_HardDrop] = input_buffer[CMD_HardDrop] == 1;
	input_buffer[CMD_Hold] = keystates[KEY_CMD_Hold] ? input_buffer[CMD_Hold]+1 : 0;
	input[CMD_Hold] = input_buffer[CMD_Hold] == 1;
	input_buffer[CMD_Reset] = keystates[KEY_CMD_Reset] ? input_buffer[CMD_Reset]+1 : 0;
	input[CMD_Reset] = input_buffer[CMD_Reset] == 1;
}
