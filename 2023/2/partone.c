#define NOB_IMPLEMENTATION
#include "nob.h"

typedef enum {
	RED,
	GREEN,
	BLUE,
	INVALID
} color_t;

static color_t get_color(const char *str) {
	if (!strncmp(str, "red", 3)) {
		return RED;
	} else if (!strncmp(str, "green", 5)) {
		return GREEN;
	} else if (!strncmp(str, "blue", 4)) {
		return BLUE;
	} else {
		return INVALID;
	}
}

static inline void shift_sv_by(Nob_String_View *sv, int offset)
{
	sv->data += offset;
	sv->count -= offset;
}

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

typedef struct {
	bool possible;
	int id;
	int red;
	int blue;
	int green;
} game_t;

#define BUF_CAP 64

static game_t parse_game(Nob_String_View line)
{
	static char num_buf[BUF_CAP + 1] = {0};
	
	size_t i = 5;
	for (; i < line.count && isdigit(line.data[i]) && i - 5 < BUF_CAP; i++) {
		num_buf[i - 5] = line.data[i];
	}
	
	game_t game = {0};

	game.id = atoi(num_buf);
	shift_sv_by(&line, i + 2);
	
	bool first_showing = true;
	while (line.count > 0) {
		Nob_String_View shown = nob_sv_chop_by_delim(&line, ';');
		if (!first_showing)
			shift_sv_by(&shown, 1);
		else
			first_showing = false;
		
		bool first_info = true;
		while (shown.count > 0) {
			Nob_String_View info = nob_sv_chop_by_delim(&shown, ',');
			if (!first_info)
				shift_sv_by(&info, 1);
			else
				first_info = false;
			
			int num = atoi(nob_sv_chop_by_delim(&info, ' ').data);
			
			switch (get_color(info.data)) {
			case RED:
				if (num > game.red)
					game.red = num;
				break;
			case GREEN:
				if (num > game.green)
					game.green = num;
				break;
			case BLUE:
				if (num > game.blue)
					game.blue = num;
				break;
			case INVALID:
				nob_log(NOB_ERROR, "Invalid color "SV_Fmt, SV_Arg(info));
				exit(1);
				break;
			}
		}
	}
	
	if (game.red <= MAX_RED && game.green <= MAX_GREEN && game.blue <= MAX_BLUE)
		game.possible = true;
	
	return game;
}

int main(int argc, char* argv[])
{
	assert(*argv);
	nob_shift_args(&argc, &argv);
	
	if (!*argv) {
		nob_log(NOB_ERROR, "No file was provided for reading");
		return 1;
	}

	const char *file_path = nob_shift_args(&argc, &argv);
	Nob_String_Builder file_data = {0};
	
	if (!nob_read_entire_file(file_path, &file_data)) return 1;

	Nob_String_View content = {
		.data = file_data.items,
		.count = file_data.count
	};

	int possible_sum = 0;
	
	content = nob_sv_trim(content);
	while (content.count > 0) {
		Nob_String_View line = nob_sv_chop_by_delim(&content, '\n');

		game_t game = parse_game(line);

		if (game.possible) {
			nob_log(NOB_INFO, "Found possible game with id %d that contains atleast %d red, %d green, and %d blue", game.id, game.red, game.green, game.blue);
			possible_sum += game.id;
		}
		
		content = nob_sv_trim(content);
	}

	nob_log(NOB_INFO, "Sum of all possible ids: %d", possible_sum);
	
	return 0;
}
