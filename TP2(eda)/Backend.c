#include <stdio.h>				/* printf */
#include <stdlib.h>				/* atoi */
#include <string.h>				/* strcmp */

#include "lib/libparse/libparse.h"
#include "Backend.h"
#include "Mode.h"
#include "Robots.h"

struct cmdData {
	int width;
	int height;
	int robots_number;
	int mode;
};


/**********************PROTOTYPES******************/
static int parseCallback(char* key, char* value, void* userData);
static void printHelp(void);

int
initBackend(int argc, char* argv[], 
	statusCallback publishStatus, void* front_data) {

	// Verify valid input
	if (argc < 1) {
		return FAILURE;
	}
	else if (argv == NULL) {
		return FAILURE;
	}
	else if (publishStatus == NULL) {
		return FAILURE;
	}
	else if (front_data == NULL) {
		return FAILURE;
	}

	struct cmdData command_line_input = { 0 };
	if (parseCmdLine(argc, argv, parseCallback, &command_line_input) == PARSECMD_FAIL) {
		printHelp();
		return FAILURE;
	}
	else if(command_line_input.mode = 1) {
		runModeOne(command_line_input.robots_number,
			command_line_input.width, command_line_input.height, 
			publishStatus, front_data);
	}
	else if (command_line_input.mode = 2) {
		runModeTwo(command_line_input.width, command_line_input.height,
			publishStatus, front_data);
	}

	return SUCCESS;
}

static int
parseCallback(char* key, char* value, void* userData) {
	if (userData == NULL) return PARAMS_INVALID;
	
	struct cmdData* args_storage = (struct cmdData*) userData;

	// Parameter
	if (key == NULL) {
		return PARAMS_INVALID;
	}
	// Key-value pair
	if (!strcmp(key, "Mode")) {
		switch (atoi(value)) {
			case 1:
				args_storage->mode = 1;
				break;
			case 2:
				args_storage->mode = 2;
				break;
			default:
				printf("Invalid mode.\n");
				return PARAMS_INVALID;
				break;
			}
	}
	else if (!strcmp(key, "Robots")) {
		if (is_data_numeric(value) != NULL) {
			args_storage->robots_number = atoi(value);
		}
		else {
			printf("Invalid number of robots.\n");
			return PARAMS_INVALID;
		}
	}
	else if (!strcmp(key, "Height")) {
		if (is_data_numeric(value) != NULL 
			&& atoi(value) <= FLOOR_HEIGHT) {
			args_storage->height = atoi(value);
		}
		else {
			printf("Invalid board height.\n");
			return PARAMS_INVALID;
		}
	}
	else if (!strcmp(key, "Width")) {
		if (is_data_numeric(value) != NULL 
			&& atoi(value) <= FLOOR_WIDTH) {
				args_storage->width = atoi(value);
		}
		else {
			printf("Invalid board width.\n");
			return PARAMS_INVALID;
		}
	}
	else {
		return PARAMS_INVALID;
	}
	return PARAMS_VALID;
}


static void printHelp(void) {
	putchar('\n');
	printf("==== Usage ====\n");
	printf("Key\tValue\n");

	printf("-Mode\t1 or 2.\n");
	printf("\t1: Given a number of robots, perform a real-time cleaning of the floor.\n");
	printf("\t2: Simulate 1000 times the ammount of time needed to clean the floor using N robots.\n");
	printf("\t   Stops when the time elapsed between N and N-1 is less than 0.1 seconds.\n");

	printf("-Robots\tNumber of robots to use at the same time.\n");
	printf("\tOnly used in mode 1; ignored in mode 2.\n");

	printf("-Width\tFloor witdh.\n");
	printf("\tInteger greater or equal than zero.\n");
	printf("-Height\tFloor height\n");
	printf("\tInteger greater or equal than zero.\n");
}
