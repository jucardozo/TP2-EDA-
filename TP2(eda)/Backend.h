#ifndef BACKEND_H
#define BACKEND_H

/***********DEFINES******************/
#define SUCCESS
#define FAILURE

/*************STRUCT AND TYPEDEF*****************/
struct Floor			// this struct will save all the information of floor
{
	int height;			
	int width;
	int* clean;					//will save  the memory segment location
	RobotCollection robots;		//will save all the information of the robots such as the amount ,position,etc
	double time_to_clean;		
};
struct RobotCollection
{
	int robots_number;
	Robot* robots;
};
struct Robot
{
	Position coordinates;
};
struct Position
{
	double x;
	double y;
};

typedef int(*statusCallback)(Floor*, void*);

/**********************PROTOTYPES******************/

void initBackend(int argc, char* argv[], statusCallback publishStatus,void* frontdata);

void* createFloor(Floor*, int height, int widht, int robots);			// this funtion returns NULL in case it fails to allocate the memory segment

void destroyFloor(Floor*);

void generateRobots(RobotCollection* robots, int ammount);


#endif   //backend.h