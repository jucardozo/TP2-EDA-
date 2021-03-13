#ifndef BACKEND_H
#define BACKEND_H	1

/***********DEFINES******************/
#define SUCCESS		(0)
#define FAILURE		(-1)
#define TILE_CLEAN	(1)			// This will also be returned when the whole floor is clean
#define TILE_DIRTY	(0)

/*************STRUCT AND TYPEDEF*****************/
struct Floor			// this struct will save all the information of floor
{
	int height;			
	int width;
	int* clean;					//will save  the memory segment location
	int clean_size;
	struct RobotCollection robots;		//will save all the information of the robots such as the amount ,position,etc
	double time_to_clean;		
};
 struct RobotCollection
{
	int robots_number;
	struct Robot* robots;
};
struct Robot
{
	struct Position coordinates;
	double angle;
};
struct Position
{
	double x;
	double y;
};

typedef int(*statusCallback)(struct Floor*, void*);

/**********************PROTOTYPES******************/

void initBackend(int argc, char* argv[], statusCallback publishStatus,void* frontdata);

#endif   //backend.h