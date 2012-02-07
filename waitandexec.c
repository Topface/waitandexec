/*
 * waitandexec.c
 *
 * ask @bobrov
 */


#include <stdlib.h>
#include <errno.h>

#define MAX_PARAM_COUNT 0x10


void close_fds() {
	int fd;
	for (fd = getdtablesize(); fd >= 0; --fd) {
		close(fd);
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		return 0;
	}
	
	if (fork() != 0) {
		exit(0);
	}

	close_fds();

	char *args[MAX_PARAM_COUNT];
	short i, kill_result, callee_found;
	int pid = atoi(argv[1]);

	while (1) {
		callee_found = 0;		
		kill_result = kill(pid, 0);

		if (kill_result == 0) {
			callee_found = 1;
		}

		if (kill_result == -1) {
			if (errno == EPERM) {
				callee_found = 1;
			}
		}

		if (!callee_found) {
			break;
		}

		sleep(1);
	}

	for (i = 0; i < MAX_PARAM_COUNT; i++) {
		args[i] = NULL;
	}

	for (i = 2; i < argc; i++) {
		args[i-2] = argv[i];
	}

	execv(args[0], args);
}