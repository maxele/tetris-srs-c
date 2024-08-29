#include "logic.h"

struct GameData *game_data_new(char /*width*/ w, char /*height*/ h)
{
	struct GameData *d = malloc(sizeof(struct GameData));
	d->field = malloc(w * h);
	memset(d->field, 0, w*h);
	d->w = w;
	d->h = h;
	d->cp = NoPiece;
	d->hp = NoPiece;
	generate_bag(d->bag);
	d->pc = 0;
	place(d);
	return d;
}

void show_stdout(struct GameData *d)
{
	char pieces_chars[9] = {' ', 'I', 'T', 'O', 'J', 'L', 'S', 'Z', '#'};
	char current_piece_intersection = 0;
	printf("| ");
	for (int i = 0; i < d->w * d->h; i++) {
		if (i % d->w == 0 && i != 0) printf("|\n| ");
		current_piece_intersection = 0;
		for (int j = 0; j < 4; j++) {
			int ji = (pieces[d->cp][d->cr][j].x + d->cpos.x)
				+ (pieces[d->cp][d->cr][j].y + d->cpos.y) * d->w;
			if (i == ji) {
				printf("%c%c ", pieces_chars[d->cp], pieces_chars[d->cp]);
				current_piece_intersection = 1;
			}
		}
		if (!current_piece_intersection) {
			printf("%c%c ", pieces_chars[d->field[i]], pieces_chars[d->field[i]]);
		} else if (d->field[i] && current_piece_intersection){
			printf("Something's wrong...\n");
		}
	}
	printf("|\n");
}

char collides(struct GameData *d, char /*rotation*/ r, struct Point /*offset*/ o)
{
	for (int i = 0; i < 4; i++) {
		struct Point /*position*/ p = {
			pieces[d->cp][r][i].x + o.x + d->cpos.x,
			pieces[d->cp][r][i].y + o.y + d->cpos.y
		};
		if (p.x < 0 || p.x >= d->w || p.y < 0 || p.y >= d->h)
			return 1;
		else if (d->field[p.y*d->w + p.x])
			return 1;
	}
}

void /*drop the current piece*/ drop(struct GameData *d)
{
	struct Point /*offset*/ o = {0, 1};
	while (!collides(d, d->cr, o)) {
		o.y++;
	}
	o.y--;
	d->cpos.y += o.y;
}

void /*place the current piece*/ place(struct GameData *d)
{
	if (d->cp != NoPiece) {
		for (int i = 0; i < 4; i++) {
			struct Point /*position*/ p = {
				pieces[d->cp][d->cr][i].x + d->cpos.x,
				pieces[d->cp][d->cr][i].y + d->cpos.y
			};
			d->field[p.y * d->w + p.x] = d->cp;
		}
		// TODO: doesn't work :(
		// clear full lines
		for (int h = d->h-1; h > 0; h--) {
			char f = 1;
			for (int x = 0; f && x < d->w; x++)
				f &= d->field[h*d->w + x] != NoPiece;
			// while line (h) is filled
			while (f) {
				for (int y = h; (f) && y > 0; y--) {
					for (int x = 0; x < d->w; x++) {
						d->field[(y)*d->w + x] = d->field[(y-1)*d->w + x];
					}
				}
				for (int x = 0; f && x < d->w; x++)
					f &= d->field[h*d->w + x] != NoPiece;
			}
		}
	}
	next_piece(d);
}

void rotate(struct GameData *d, char /*roatation*/ r) {
	if (!collides(d, r, (struct Point){0, 0})) {
		d->cr = r;
		return;
	}
	if (abs(d->cr - r) == 2) {
		printf("Blocked 180\n");
		return;
	}
	if (d->cp == OPiece) return;
	if (d->cp == IPiece) {
		for (int i = 0; i < 4; i++) {
			if (!collides(d, r, irotations[d->cr][r][i])) {
				d->cpos.x = d->cpos.x + irotations[d->cr][r][i].x;
				d->cpos.y = d->cpos.y + irotations[d->cr][r][i].y;
				d->cr = r;
				return;
			}
		}
		return;
	}
	for (int i = 0; i < 4; i++) {
		if (!collides(d, r, jltszrotations[d->cr][r][i])) {
			d->cpos.x = d->cpos.x + jltszrotations[d->cr][r][i].x;
			d->cpos.y = d->cpos.y + jltszrotations[d->cr][r][i].y;
			d->cr = r;
			return;
		}
	}
	return;
}

void generate_bag(char *b) {
	//printf("logic_bag_generate %d\n", bag-logic_bag);
	for (int i = 0; i < 7; i++) {
		b[i] = i+1;
	}
	for (int i = 0; i < 7; i++) {
		int rnd = random() % 7;
		int tmp = b[i];
		b[i] = b[rnd];
		b[rnd] = tmp;
	}
}

void next_piece(struct GameData *d) {
	if (d->pc % 7 == 0) {
		generate_bag(d->bag + (d->pc + 7) % 14);
	}
	d->cp = d->bag[d->pc % 14];
	d->pc++;
	d->cpos.x = d->w / 2 - 2;
	d->cpos.y = 0;
	d->cr = 0;
}

void /*execute the given input*/ do_input(struct GameData *d, char /*input*/ i[10]) {
	//char *s[] = {"CMD_None", "CMD_Left", "CMD_Right", "CMD_Clockwise", "CMD_CounterClockwise", "CMD_OneEighty", "CMD_SoftDrop", "CMD_HardDrop", "CMD_Hold", "CMD_Reset", "CMD_Up"};
	if (i[CMD_Left]) {
		if (!collides(d, d->cr, (struct Point){-1, 0})) d->cpos.x -= 1;
	}
	if (i[CMD_Right]) {
		if (!collides(d, d->cr, (struct Point){1, 0})) d->cpos.x += 1;
	}
	if (i[CMD_HardDrop]) {
		int dy = 0;
		while (!collides(d, d->cr, (struct Point){0, dy})) dy++;
		d->cpos.y += dy-1;
		place(d);
	}
	if (i[CMD_SoftDrop]) {
		if (!collides(d, d->cr, (struct Point){0, 1})) d->cpos.y += 1;
	}
	if (i[CMD_Clockwise]) {
		rotate(d, (d->cr + 1)%4);
	}
	if (i[CMD_CounterClockwise]) {
		rotate(d, (d->cr + 3)%4);
	}
	if (i[CMD_OneEighty]) {
		rotate(d, (d->cr + 2)%4);
	}
	if (i[CMD_Hold]) {
		if (d->hp == NoPiece) {
			d->hp = d->cp;
			next_piece(d);
		} else {
			char tmp = d->hp;
			d->hp = d->cp;
			d->cp = tmp;
			d->cpos.x = d->w / 2 - 2;
			d->cpos.y = 0;
			d->cr = 0;
		}
	}
	if (i[CMD_Up]) {
		if (!collides(d, d->cr, (struct Point){0, -1})) d->cpos.y -= 1;
	}
	if (i[CMD_Reset]) {
		memset(d->field, 0, d->w*d->h);
		d->cp = NoPiece;
		d->hp = NoPiece;
		generate_bag(d->bag);
		d->pc = 0;
		place(d);
	}
	return;
}
