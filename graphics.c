#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "logic.h"
#include "graphics.h"

SDL_Renderer *renderer = NULL;
SDL_Window *window;

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
	window = SDL_CreateWindow("Tertis",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, w, h, windowFlags);
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

void show_piece(int p, int r, int mw, int x, int y, char is_ghost)
{/*{{{*/
	SDL_SetRenderDrawColor(renderer,
			GRAPHICS_PIECE_COLORS[p].r*(is_ghost ? 0.7 : 1),
			GRAPHICS_PIECE_COLORS[p].g*(is_ghost ? 0.7 : 1),
			GRAPHICS_PIECE_COLORS[p].b*(is_ghost ? 0.7 : 1),
			255);
	for (int i = 0; i < 4; i++) {
		SDL_Rect rect = {
			x + LOGIC_PIECE_DATA[p][r][i].x * mw,
			y + LOGIC_PIECE_DATA[p][r][i].y * mw,
			mw, mw
		};
		SDL_RenderFillRect(renderer, &rect);
	}
}/*}}}*/

void show(struct GameData *d)
{/*{{{*/
	if (renderer == NULL) graphics_init(800, 800);
	SDL_SetRenderDrawColor(renderer,
			GRAPHICS_BACKGROUND_COLOR.r,
			GRAPHICS_BACKGROUND_COLOR.g,
			GRAPHICS_BACKGROUND_COLOR.b, 255);
	SDL_RenderClear(renderer);

	// Layout:
	// display held piece
	SDL_Rect hold = {gs.mw, gs.mw, gs.mw*5, gs.mw*5};
	SDL_SetRenderDrawColor(renderer,
			GRAPHICS_FOREGROUND_COLOR.r,
			GRAPHICS_FOREGROUND_COLOR.g,
			GRAPHICS_FOREGROUND_COLOR.b, 255);
	SDL_RenderFillRect(renderer, &hold);
	if (d->hp)
		show_piece(d->hp, 0, gs.mw, hold.x+gs.mw/2, hold.y+gs.mw/2, 0);
	SDL_Rect field = {hold.x + hold.w + gs.mw, hold.y, d->w*gs.mw, d->h*gs.mw};
	SDL_SetRenderDrawColor(renderer,
			GRAPHICS_FOREGROUND_COLOR.r,
			GRAPHICS_FOREGROUND_COLOR.g,
			GRAPHICS_FOREGROUND_COLOR.b, 255);
	SDL_RenderFillRect(renderer, &field);
	// display the field
	for (int i = 0; i < d->w * d->h; i++) {
		if (d->field[i] == NoPiece) continue;
		SDL_SetRenderDrawColor(renderer,
				GRAPHICS_PIECE_COLORS[d->field[i]].r,
				GRAPHICS_PIECE_COLORS[d->field[i]].g,
				GRAPHICS_PIECE_COLORS[d->field[i]].b, 255);
		SDL_Rect r = {
			field.x + i % d->w * gs.mw,
			field.y + i / d->w * gs.mw,
			gs.mw,
			gs.mw };
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
	SDL_Rect preview = {field.x + field.w + gs.mw, field.y, gs.mw*4, gs.mw*4 * 5};
	SDL_SetRenderDrawColor(renderer,
			GRAPHICS_FOREGROUND_COLOR.r,
			GRAPHICS_FOREGROUND_COLOR.g,
			GRAPHICS_FOREGROUND_COLOR.b, 255);
	SDL_RenderFillRect(renderer, &preview);
	for (int i = 0; i < 5; i++) {
		show_piece(d->bag[(d->pc + i) % 14], 0, gs.mw*3/4,
				preview.x+gs.mw*3/8,
				preview.y+gs.mw*3/8 + gs.mw*15/4*i,
				0);
	}
	SDL_RenderPresent(renderer);
}/*}}}*/

char input_buffer[CMD_COUNT];
void get_input(struct GameData *d, char input[CMD_COUNT])
{/*{{{*/
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
	// Activate on first frame and after 5 frames
	input_buffer[CMD_Left] = keystates[KEY_CMD_Left] && input_buffer[CMD_Left] <= 10 ? input_buffer[CMD_Left]+1 : 0;
	input[CMD_Left] = input_buffer[CMD_Left] == 1 || input_buffer[CMD_Left] > 5;
	input_buffer[CMD_Right] = keystates[KEY_CMD_Right] && input_buffer[CMD_Right] <= 10 ? input_buffer[CMD_Right]+1 : 0;
	input[CMD_Right] = input_buffer[CMD_Right] == 1 || input_buffer[CMD_Right] > 5;
	input_buffer[CMD_Up] = keystates[KEY_CMD_Up] && input_buffer[CMD_Up] <= 10 ? input_buffer[CMD_Up]+1 : 0;
	input[CMD_Up] = input_buffer[CMD_Up] == 1 || input_buffer[CMD_Up] > 5;
	input_buffer[CMD_SoftDrop] = keystates[KEY_CMD_SoftDrop] && input_buffer[CMD_SoftDrop] <= 10 ? input_buffer[CMD_SoftDrop]+1 : 0;
	input[CMD_SoftDrop] = input_buffer[CMD_SoftDrop] == 1 || input_buffer[CMD_SoftDrop] > 5;
	// Activte only first time
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
}/*}}}*/
