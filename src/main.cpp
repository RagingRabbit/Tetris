#include "console.h"

#include <vector>

#define INVISIBLE_HEIGHT 2
#define BOARD_WIDTH 10
#define BOARD_HEIGHT (18 + INVISIBLE_HEIGHT)

#define GAME_OVER_TIMER 1.0f

#define LEGACY_COLORS false

enum PieceType
{
	PIECE_I = 1,
	PIECE_J = 2,
	PIECE_L = 3,
	PIECE_O = 4,
	PIECE_S = 5,
	PIECE_T = 6,
	PIECE_Z = 7
};

struct PieceData
{
	int size;
	std::vector<int> grid;
};

struct Piece
{
	int x, y;
	PieceType type;
	PieceData data;
};

std::vector<int> board;

float timer;
bool changed;

bool down;
float dtimer;
float gameovertimer;
float globaltimer;

int lasttype, nexttype;
Piece *current_piece;

int score;
int numlines;

Piece *create_piece(int x, int y, PieceType type)
{
	switch (type)
	{
	case PIECE_I:
		return new Piece{ x, y, type, 4, {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0} };
	case PIECE_J:
		return new Piece{ x, y, type, 3, {1, 0, 0, 1, 1, 1, 0, 0, 0} };
	case PIECE_L:
		return new Piece{ x, y, type, 3, {0, 0, 1, 1, 1, 1, 0, 0, 0} };
	case PIECE_O:
		return new Piece{ x, y, type, 4, {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0} };
	case PIECE_S:
		return new Piece{ x, y, type, 3, {0, 1, 1, 1, 1, 0, 0, 0, 0} };
	case PIECE_T:
		return new Piece{ x, y, type, 3, {0, 1, 0, 1, 1, 1, 0, 0, 0} };
	case PIECE_Z:
		return new Piece{ x, y, type, 3, {1, 1, 0, 0, 1, 1, 0, 0, 0} };
	default:
		return NULL;
	}
}

int get_color(PieceType type)
{
	if (LEGACY_COLORS)
	{
		switch (type)
		{
		case 0:
			return 0x000000;
		case PIECE_I:
			return 0x00FFFF;
		case PIECE_J:
			return 0x0000FF;
		case PIECE_L:
			return 0xFF7700;
		case PIECE_O:
			return 0xFFFF00;
		case PIECE_S:
			return 0x00FF00;
		case PIECE_T:
			return 0x770077;
		case PIECE_Z:
			return 0xFF0000;
		default:
			return 0;
		}
	}
	else
	{
		switch (type)
		{
		case 0:
			return 0x000000;
		case PIECE_I:
			return 0x22C9E1;
		case PIECE_J:
			return 0x223AE1;
		case PIECE_L:
			return 0xE18122;
		case PIECE_O:
			return 0xE1D122;
		case PIECE_S:
			return 0x189D18;
		case PIECE_T:
			return 0xA122E1;
		case PIECE_Z:
			return 0xE1223A;
		default:
			return 0;
		}
	}
}

void init_game()
{
	board = std::vector<int>(BOARD_WIDTH * BOARD_HEIGHT);
	changed = true;
	current_piece = NULL;
	down = false;
	dtimer = 0.0f;
	gameovertimer = -1.0f;
	globaltimer = 0.0f;

	score = 0;
	numlines = 0;
}

void init()
{
	set_repeat_delay(0.2f);
	set_repeat_rate(0.02f);

	srand((int)time(0));

	init_game();
}

void game_over()
{
	print_s(11, 32, "Game Over", 0x770000, 0);
	gameovertimer = 0.0f;
}

int calculate_score(int lines)
{
	switch (lines)
	{
	case 1:
		return 40;
	case 2:
		return 100;
	case 3:
		return 300;
	case 4:
		return 1200;
	}
}

void check_lines()
{
	std::vector<int> fulllines;
	for (int y = BOARD_HEIGHT - 1; y >= 0; y--)
	{
		bool linefull = true;
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			if (!board[x + y * BOARD_WIDTH])
			{
				linefull = false;
				break;
			}
		}
		if (linefull)
		{
			fulllines.push_back(y);
		}
	}
	for (int i = 0; i < (int)fulllines.size(); i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			put_char(10 + j, 10 - INVISIBLE_HEIGHT + fulllines[i], (char)219, 0xFFFFFF, 0);
		}
	}
	Sleep(50);
	for (int i = fulllines.size() - 1; i >= 0; i--)
	{
		board.erase(board.begin() + fulllines[i] * BOARD_WIDTH, board.begin() + fulllines[i] * BOARD_WIDTH + BOARD_WIDTH);
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			board.insert(board.begin(), 0);
		}
		numlines++;
	}
	score += calculate_score(fulllines.size());
	numlines += fulllines.size();
}

void place_piece(int xpos, int ypos, const PieceData* data, PieceType type)
{
	for (int y = 0; y < data->size; y++)
	{
		if (ypos + y >= BOARD_HEIGHT)
		{
			break;
		}
		for (int x = 0; x < data->size; x++)
		{
			if (data->grid[x + y * data->size])
			{
				board[(xpos + x) + (ypos + y) * BOARD_WIDTH] = get_color(type);
			}
		}
	}

	check_lines();
}

bool piece_collides(int xpos, int ypos, const PieceData* data)
{
	for (int y = 0; y < data->size; y++)
	{
		for (int x = 0; x < data->size; x++)
		{
			if (data->grid[x + y * data->size])
			{
				if ((xpos + x) < 0 || (xpos + x) >= BOARD_WIDTH || (ypos + y) >= BOARD_HEIGHT || board[(xpos + x) + (ypos + y) * BOARD_WIDTH])
				{
					return true;
				}
			}
		}
	}
	return false;
}

int raycast()
{
	for (int i = 0; i < BOARD_HEIGHT * 2; i++)
	{
		if (piece_collides(current_piece->x, current_piece->y + i, &current_piece->data))
		{
			return i - 1;
		}
	}
	return 0;
}

int get_random_type()
{
	int type = -1;
	while ((type = rand() % 7 + 1) == lasttype)
	{
	}
	return type;
}

void spawn_piece()
{
	if (nexttype == 0)
	{
		nexttype = get_random_type();
	}
	lasttype = nexttype;
	nexttype = get_random_type();
	current_piece = create_piece(3, 0, (PieceType)lasttype);
	if (piece_collides(current_piece->x, current_piece->y, &current_piece->data))
	{
		game_over();
	}
}

bool move_piece(int dx, int dy)
{
	if (current_piece != NULL)
	{
		bool collides = false;
		int newx = current_piece->x + dx;
		int newy = current_piece->y + dy;

		if (piece_collides(newx, newy, &current_piece->data))
		{
			if (dx == 0 && !piece_collides(newx, newy - 1, &current_piece->data))
			{
				place_piece(newx, newy - 1, &current_piece->data, current_piece->type);
				delete current_piece;

				spawn_piece();
			}
		}
		else
		{
			current_piece->x += dx;
			current_piece->y += dy;

			return true;
		}
	}

	return false;
}

PieceData rotate_piece(const PieceData* data)
{
	PieceData datacpy = *data;
	for (int y = 0; y < data->size; y++)
	{
		for (int x = 0; x < data->size; x++)
		{
			int rotx = data->size - 1 - y;
			int roty = x;
			datacpy.grid[rotx + roty * data->size] = data->grid[x + y * data->size];
		}
	}
	return datacpy;
}

void process_key(int key, bool kdown)
{
	if (gameovertimer != -1.0f)
	{
		return;
	}
	switch (key)
	{
	case VK_ESCAPE:
		stop_loop();
		break;
	case VK_LEFT:
		if (kdown)
		{
			move_piece(-1, 0);
			changed = true;
		}
		break;
	case VK_RIGHT:
		if (kdown)
		{
			move_piece(1, 0);
			changed = true;
		}
		break;
	case VK_UP:
		if (kdown)
		{
			PieceData rotateddata = rotate_piece(&current_piece->data);
			if (!piece_collides(current_piece->x, current_piece->y, &rotateddata))
			{
				current_piece->data = rotateddata;
			}
			else
			{
				if (!piece_collides(current_piece->x + 1, current_piece->y, &rotateddata))
				{
					current_piece->data = rotateddata;
					current_piece->x++;
				}
				else if (!piece_collides(current_piece->x - 1, current_piece->y, &rotateddata))
				{
					current_piece->data = rotateddata;
					current_piece->x--;
				}
			}
			changed = true;
		}
		break;
	case VK_DOWN:
		if (!down && kdown)
		{
			dtimer = 1.0f;
		}
		down = kdown;
		break;
	case VK_SPACE:
		if (kdown)
		{
			int ydelta = raycast() + 1;
			move_piece(0, ydelta);
			score += ydelta;
			changed = true;
		}
		break;
	default:
		break;
	}
}

int brightness(int col, float brightness)
{
	int r = (int)(((col & 0xFF0000) >> 16) * brightness);
	int g = (int)(((col & 0x00FF00) >> 8) * brightness);
	int b = (int)(((col & 0x0000FF) >> 0) * brightness);
	return (r << 16) | (g << 8) | (b << 0);
}

float roundf(float var)
{
	float value = (int)(var * 100 + .5);
	return (float)value / 100;
}

void draw()
{
	// Draw next piece
	print_s(22, 9, "NEXT", 0x999999, 0);
	Piece* nextpiece = create_piece(0, 0, (PieceType)nexttype);
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			int color = (x < nextpiece->data.size && y < nextpiece->data.size && nextpiece->data.grid[x + y * nextpiece->data.size]) ? get_color(nextpiece->type) : 0;
			put_char(22 + x, 10 + y, (char)219, color, 0);
		}
	}
	delete nextpiece;

	// Draw score
	print_s(22, 15, "SCORE", 0x999999, 0);
	print_s(22, 16, std::to_string(score).c_str(), 0xFFFFFF, 0);

	// Draw number of lines
	print_s(22, 18, "LINES", 0x999999, 0);
	print_s(22, 19, std::to_string(numlines).c_str(), 0xFFFFFF, 0);

	// Draw borders
	print_s(9, 9, std::string(BOARD_WIDTH + 2, (char)177).c_str(), 0x777777, 0);
	print_s(9, 28, std::string(BOARD_WIDTH + 2, (char)177).c_str(), 0x777777, 0);
	for (int i = 0; i < BOARD_HEIGHT - INVISIBLE_HEIGHT; i++)
	{
		put_char(9, 10 + i, (char)177, 0x777777, 0);
		put_char(20, 10 + i, (char)177, 0x777777, 0);
	}

	// Draw board
	for (int y = INVISIBLE_HEIGHT; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			put_char(10 + x, 10 + y - INVISIBLE_HEIGHT, (char)219, board[x + y * BOARD_WIDTH], 0);
		}
	}

	if (current_piece != NULL)
	{
		// Draw preview
		int previewy = raycast() + current_piece->y;
		for (int y = 0; y < current_piece->data.size; y++)
		{
			for (int x = 0; x < current_piece->data.size; x++)
			{
				if (current_piece->data.grid[x + y * current_piece->data.size])
				{
					put_char(10 + current_piece->x + x, 10 + previewy + y - INVISIBLE_HEIGHT, (char)219, brightness(get_color(current_piece->type), 0.4f), 0);
				}
			}
		}

		// Draw piece
		for (int y = 0; y < current_piece->data.size; y++)
		{
			if (current_piece->y + y < INVISIBLE_HEIGHT)
			{
				continue;
			}
			for (int x = 0; x < current_piece->data.size; x++)
			{
				if (current_piece->data.grid[x + y * current_piece->data.size])
				{
					put_char(10 + current_piece->x + x, 10 - INVISIBLE_HEIGHT + current_piece->y + y, (char)219, get_color(current_piece->type), 0);
				}
			}
		}
	}
}

void tick()
{
	if (current_piece != NULL)
	{
		move_piece(0, 1);
	}
}

void update(float delta)
{
	if (gameovertimer != -1.0f)
	{
		gameovertimer += delta;
		if (gameovertimer >= GAME_OVER_TIMER)
		{
			gameovertimer = -1.0f;
			init_game();
			print_s(4, 33, "Press any key to restart", 0x770000, 0);
			_getch();
			printf(CLEAR_SCREEN);
		}
		else
		{
			return;
		}
	}

	globaltimer += delta;

	if (current_piece == NULL)
	{
		spawn_piece();
	}

	if (down)
	{
		if (dtimer >= 0.05f)
		{
			dtimer = 0.0f;

			if (move_piece(0, 1))
			{
				score++;
			}
			timer = 0.0f;
			changed = true;
		}
		else
		{
			dtimer += delta;
		}
	}

	timer += delta;
	if (timer >= 1.0f)
	{
		timer = 0.0f;
		tick();
		changed = true;
	}

	if (changed)
	{
		draw();
		changed = false;
	}

	// Draw score per minute
	print_s(22, 21, "SPM", 0x999999, 0);
	char* spmstr = new char[10];
	sprintf(spmstr, "%.2f", score / globaltimer * 60.0f);
	print_s(22, 22, spmstr, 0xFFFFFF, 0);
	delete spmstr;

	// Draw lines per minute
	print_s(22, 24, "LPM", 0x999999, 0);
	char* lpmstr = new char[10];
	sprintf(lpmstr, "%.2f", numlines / globaltimer * 60.0f);
	print_s(22, 25, lpmstr, 0xFFFFFF, 0);
	delete lpmstr;

	// Draw timer
	print_s(22, 27, "TIME", 0x999999, 0);
	char* timstr = new char[10];
	sprintf(timstr, "%.2f", globaltimer);
	print_s(22, 28, timstr, 0xFFFFFF, 0);
	delete timstr;

}

void end()
{
	delete current_piece;
}

int main()
{
	init_scr();
	printf(USE_ALTERNATE_BUFFER CLEAR_SCREEN HIDE_CURSOR);
	set_font(16, 16, L"Terminal");
	set_size(30, 40);

	run_loop("Tetris", init, update, end, process_key);
}