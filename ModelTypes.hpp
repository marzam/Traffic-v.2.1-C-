#ifndef _MODELTYPES_HPP_
#define _MODELTYPES_HPP_
#include <cstring>
#include <thread>
#include <string>

using namespace std;

#define EMPTY    NULL

#define  STRINGSIZE        256
#define VEHICLEPERKM      133
#define PERCENT2VEHICLES  (1000.0f / 7.5f)

#define NOTCHANGE    0
#define RIGHT2LEFT  -1
#define LEFT2RIGHT   1

class Entity;
class Vehicle;


/*
 * Vehicles parameters
 */

struct tpVehiclesType
{
    char       description[STRINGSIZE]; //       description[STRINGSIZE];
    float      percent;
    int        size;
    int        vMax;
    double     param[5];
    double     aheadInt;  //Vehicle aheadway used on multilane model
    double     safeDist;  //Safe
    double     inc;
    int        desc;
    float      left_p;
    float     right_p;

};


struct tpCluster{
  //int ID;
  Vehicle *vehicle;
  tpCluster *next;
};

/*
 * Config parameters
 */
struct tpParam
{
    string modelName;
    //char modelName[STRINGSIZE];
    char profilesOcc[STRINGSIZE];
    int nProfiles;



    int cellX;   //Space in X
    int cellY;   //Space in Y --> number of the roads
    float deltaH;
    float defaultSize; //default size of one vehicle - it means 7.5mts

    float spacePerception;
    int deceleration; //Number of cells used to descelerate vehicles. The maximum value. After that, it may be a collision

    int sTime;   //Simulation time
    int dTime;   //Discarde time
    int stTime;   //Statistic time

    int vMax;      //Max velocity
    float iDensity; //Initial density
    float fDensity; //Final density
    float dDensity; //Delta density


    bool fixed;
    bool photo;
    bool logCluster;

    int  logVehicles;

};




struct tpTableDensityMem
{
   int   vehicles;
   float density;
   unsigned long memory;
   unsigned int time;
};


/*
 *
 */
struct stDiskMemory
{char byte_a, byte_b, byte_c, byte_d;};

/*
 *
 */
union uDiskMemory
{
   int iDisk;
   unsigned int uDisk;
   float fDisk;
   stDiskMemory bytes;
};

/*
 *
 */
typedef uDiskMemory tpDiskMemory;

#endif
