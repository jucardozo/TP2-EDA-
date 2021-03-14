#ifndef BACKEND_H
#define BACKEND_H	1

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

/******************** MACROS ********************/

/******************** STRUCTURES AND TYPEDEF ********************/
struct RobotCollection; // Forward declaration. See "Robots.h"

struct Floor {			// this struct will save all the information of floor
    int height;
    int width;
    int* clean;					//will save  the memory segment location
    int clean_size;
    struct RobotCollection robots;		//will save all the information of the robots such as the amount ,position,etc
    double time_to_clean;		
};

typedef int(*statusCallback)(struct Floor*, void*);

/******************** ********************/
int initBackend(int argc, char* argv[], statusCallback publishStatus, void* front_data);

#endif   //backend.h