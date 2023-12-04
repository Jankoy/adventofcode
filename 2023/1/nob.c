#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char* argv[])
{
	NOB_GO_REBUILD_URSELF(argc, argv);

	Nob_Cmd cmd = {0};
	nob_cmd_append(&cmd, "cc");
	nob_cmd_append(&cmd, "-Wall", "-Wextra", "-pedantic");
	nob_cmd_append(&cmd, "-o");
	nob_cmd_append(&cmd, "partone");
	nob_cmd_append(&cmd, "partone.c");
	if (!nob_cmd_run_sync(cmd)) return 1;
	cmd.count -= 2;
	nob_cmd_append(&cmd, "parttwo");
	nob_cmd_append(&cmd, "parttwo.c");
	if (!nob_cmd_run_sync(cmd)) return 1;
	
	return 0;
}
