#ifndef BACKEND_H
#define BACKEND_H	1

#include "Robots.h" /* RobotCollection ..too late to set as a * in Floor ): */

/******************** DEFINITIONS ********************/
#define SUCCESS			    	(0)
#define FAILURE				    (-1)

#define TILE_CLEAN			    (1)			// This will also be returned when the whole floor is clean
#define TILE_DIRTY			    (0)

#define FLOOR_WIDTH			    (100)
#define FLOOR_HEIGHT		    (70)

// Number of iterations for each simulation in mode 2
#define SIMULATION_ITERATIONS   (1000)

// Time difference between two simulations in mode 2
#define SIMULATIONS_DELTA       (0.1)

enum game_mode {
    MODE_UNSET = 0,
    MODE1,
    MODE2,
};

/******************** MACROS ********************/

/******************** STRUCTURES AND TYPEDEF ********************/

struct Floor {			// this struct will save all the information of floor
    int game_mode;      // Uses game_mode enum.
    int height;
    int width;
    int* clean;			// Array with tiles status
    int clean_size;     // Size of *clean array
    struct RobotCollection robots; //will save all the information of the robots such as the amount ,position,etc
    double time_to_clean; // Time used to fully clean the floor
};

// Should return 0 on success. Any other value means error.
typedef int(*statusCallback)(struct Floor*, void*);

/******************** PROTOTYPES ********************/
int initBackend(int argc, char* argv[], statusCallback publishStatus, void* front_data);

#endif   //backend.h