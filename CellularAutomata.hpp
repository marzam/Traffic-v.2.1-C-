#ifndef _CELLULARAUTOMATA_H_
#define _CELLULARAUTOMATA_H_
#include <App.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <Entity.hpp>
// #include <TModel.hpp>
// #include <MovementSensor.hpp>
#include <ModelTypes.hpp>
#include <pThreadClass.hpp>
#include <thread>

/*
 *  CellularAutomata.h
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *  This class has the basic structure as well as transition rules
 *
 */

using namespace std;
double gamaFunction (double n);

class CellularAutomata: public pThreadClass
{

public:
    CellularAutomata();
    virtual ~CellularAutomata();
    void initDefault(void);
    void update(void);

    void finalize(void);
    void debugAllVehicles(void);
    void debug(int);
    //Save log methods

    bool  isRunning (void) {return mRunning; };

    virtual void* execThread(void);



protected:

//    int                              mState;
//    int                              mpStep;
//    int                              mStep;
//    float                            mdGlobal;

   Stopwatch                        mStopwatch;
//    TModel                           *mRules;

//Parallel approach
   tpParam                          *mParam;
//    MovementSensor                   *mSensor;

};

class MasterCellularAutomata
{
public:
  MasterCellularAutomata(tpParam, tpVehiclesType *, int, int = 0);
  ~MasterCellularAutomata(void);
  void parallelUpdate(void);
private:
  void initialCondition(float);

protected:
  CellularAutomata                  *mPCA; /* <! parallel array of cellular automata > */
  pthread_barrier_t                 m_Barrier;
  unsigned int                      mNumberOfThreads;
  unsigned int                      mNumTypes;
  tpParam                           mParam;
//   MovementSensor                    mSensor;
  uintptr_t                         *mGrid;
  Entity                            *mEntities;
  tpVehiclesType                    *mVehiclesType;  /* <! Different kind of vehicles - data strucutre > */

};
#endif
