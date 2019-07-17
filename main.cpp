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
  mParam.density = 0.10f;
//   mParam.iDensity = 0.01f;
//   mParam.dDensity = 0.01f;
// 1  mParam.fDensity = 0.95f;
  mParam.photo = true;
  mParam.fixed = true;
  mParam.logCluster = true;
  mParam.nProfiles = 1;

  tpVehiclesType types[1];
  types[0].percent = 1.0f;
  memset(types[0].param, 0x00, sizeof(double) * 5 ); //It indicates the number of parameters of beta function. Considering two policy (acceleration and safe distance)/
    
  strcpy(types[0].description, "Default");
  types[0].param[0] = 8;
  types[0].param[1] = 7;
  types[0].inc = 4;
  types[0].desc = 1;
  types[0].size = 5;
  types[0].vMax = 25;
  types[0].aheadInt = 6.0;
  types[0].safeDist = 2.0f;
  types[0].left_p = 1.0f;
  types[0].right_p = 1.0f;
  types[0].param[2] = gamaFunction(types[0].param[0]);
  types[0].param[3] = gamaFunction(types[0].param[1]);
  types[0].param[4] = gamaFunction(types[0].param[0]+types[0].param[1]);

  
  MasterCellularAutomata mCAParallel = MasterCellularAutomata(mParam, types, 1, 2);


/*
   mCellularAutomata.initDefault() ;

   while (mCellularAutomata.isRunning()){
      mCellularAutomata.update();
   }

   mCellularAutomata.finalize();
  */
   return EXIT_SUCCESS;
}
