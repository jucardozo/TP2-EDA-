#ifndef BACKEND_H
#define BACKEND_H	1

/******************** DEFINITIONS ********************/
#define SUCCESS				(0)
#define FAILURE				(-1)

#define TILE_CLEAN			(1)			// This will also be returned when the whole floor is clean
#define TILE_DIRTY			(0)

#define FLOOR_WIDTH			(100)
#define FLOOR_HEIGHT		(70)

// Minimum decimal precision taken into account for floats and double
#define MINIMUM_PRECISION	(3)

/******************** MACROS ********************/

/******************** STRUCTURES AND TYPEDEF ********************/
struct Position
{
	double x;
	double y;
};
struct Robot
{
	struct Position coordinates;
	double angle;
};
struct RobotCollection
{
	int robots_number;
	struct Robot* robots;
};

struct Floor			// this struct will save all the information of floor
{
	int height;			
	int width;
	int* clean;					//will save  the memory segment location
	int clean_size;
	struct RobotCollection robots;		//will save all the information of the robots such as the amount ,position,etc
	double time_to_clean;		
};

typedef int(*statusCallback)(struct Floor*, void*);

/******************** ********************/
int initBackend(int argc, char* argv[], statusCallback publishStatus,void* frontdata);

#endif   //backend.h