/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <CellularAutomata.hpp>
//CellularAutomata mCellularAutomata;
int main(int argc, char *argv[])
{
  tpParam mParam;
  mParam.modelName = "TModel_ML_SYM";
  mParam.logVehicles = 0;
  mParam.deceleration = 3;
  mParam.spacePerception = 12;
  mParam.vMax = 25;
  mParam.cellX = 10000;
  mParam.cellY = 2;
  mParam.deltaH = 1.5f;
  mParam.defaultSize = 7.5;
  mParam.sTime = 3600 * 5;
  mParam.dTime = 3600;
  mParam.stTime = 120;
  mParam.iDensity = 0.01f;
  mParam.dDensity = 0.01f;
  mParam.fDensity = 0.95f;
  mParam.photo = true;
  mParam.fixed = true;
  mParam.logCluster = true;
  mParam.nProfiles = 1;


  MasterCellularAutomata mCAParallel = MasterCellularAutomata(mParam, 2);


/*
   mCellularAutomata.initDefault() ;

   while (mCellularAutomata.isRunning()){
      mCellularAutomata.update();
   }

   mCellularAutomata.finalize();
  */
   return EXIT_SUCCESS;
}
