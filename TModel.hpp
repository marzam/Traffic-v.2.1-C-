#ifndef _TMODEL_HPP_
#define _TMODEL_HPP_
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <Grid.hpp>
#include <MovementSensor.hpp>
using namespace std;
double gamaFunction (double n);


class TModel
{
public:
    TModel();

    ~TModel();

    void finalizer (void);

    void addONEObstables(void);

    void initialCondition(float);

    void finalCondition(void);

    void applyRules(int, bool = false);

    void update(int, bool=false, bool=false);

    void executeRules(Entity *, bool = false);

    void movementRules(Vehicle*, bool = false);

    void  getDistanceAndVelocity(Vehicle *, int *, int *, bool = false);

    Entity * getBack(int *,  int, int,  int);

    void debug(int);

    void setParam(tpParam *p)                       {mParam = p; }

    void setSensor(MovementSensor *m)               {mSensor = m; }

    tpParam * getParam(void)                        {return mParam; };

    MovementSensor *getSensor(void)                 {return mSensor; };

    void setStatistic(bool t)                       { mStatistic = t; };

    Grid * getGrid(void)                            {return mGrid; };

    void setGrid(Grid *g)                           {mGrid = g; }
protected:
    
   int decelerationRule(int, int, int);

   int truncCurve3(double);

   double betaFunction (double, double, double, double, double, double );

   double betaFunction (double *);

   void saveLog(string);

   void buildCluster(Vehicle *vehicle);

   void solveCluster(Vehicle *vehicle);

   void insertIntoCluster( Vehicle *vehicle );

   void insertIntoRedLights(Vehicle *vehicle);

   float getRandom(void);

   float alphaFunction(float);

   unsigned int mReAjusted;

   tpCluster                        *mRedLights,
                                    *mClusters;

   string                            mPath;

   Vehicle                          **mLastVehicles;
   tpParam                          *mParam;
   MovementSensor                   *mSensor;
   Grid                             *mGrid;
   bool mStatistic;


\

};

#endif
