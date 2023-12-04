#define NOB_IMPLEMENTATION
#include "nob.h"

static inline int char_to_int(int c)
{
	return c - 0x30 > 9 || c - 0x30 < 0 ? -1 : c - 0x30;
}

static void parse_numbers(Nob_String_View line, int *first_num, int *last_num)
{
	bool found_first_num = false;
	for (size_t i = 0; i < line.count; i++) {
		if (char_to_int(line.data[i]) != -1) {
			if (!found_first_num) {
				*first_num = char_to_int(line.data[i]);
				found_first_num = true;
			}
			*last_num = char_to_int(line.data[i]);	
		}
	}
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

	int num_sum = 0;
	int line_num = 1;
	
	content = nob_sv_trim(content);
	while (content.count > 0) {
		Nob_String_View line = nob_sv_chop_by_delim(&content, '\n');
		
		int first_num = 0;
		int last_num = 0;
		parse_numbers(line, &first_num, &last_num);
		int fin_num = first_num * 10 + last_num;
		nob_log(NOB_INFO, "Line %d: found %d", line_num, fin_num);
		num_sum += fin_num;
		
		content = nob_sv_trim(content);
		line_num++;
	}
	
	nob_log(NOB_INFO, "Sum: %d", num_sum);
	
	return 0;
}
