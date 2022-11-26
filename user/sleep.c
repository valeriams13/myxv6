#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>

int main(int argc, char **argv){
	int i;
	// printf("%d", argc);

	if (argc < 2){
		fprintf(1,"Value not given");
		exit(1);
	}
	for (i = 1; i<argc; i++){
		sleep(atoi(argv[i]) * 10);
	}

	exit(0);	
}