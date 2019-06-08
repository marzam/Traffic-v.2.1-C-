/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <CellularAutomata.hpp>
CellularAutomata mCellularAutomata;
int main(int argc, char *argv[])
{
   
   

   mCellularAutomata.initDefault() ;
        
   while (mCellularAutomata.isRunning()){
      mCellularAutomata.update();    
   }
   
   mCellularAutomata.finalize();
   
   return EXIT_SUCCESS;
}

