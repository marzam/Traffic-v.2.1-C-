#include <TModel.hpp>
#include <string>
#include <cassert>

#define PI      (float) 3.1415926535897932384626433832795
#define PI2     (float) 2.0f*PI
#define SQRT2PI (float) sqrt(PI2)
#define ERROR   (double) 1E-10

//#define getParam()->decelearation 10

//Calcular o horizonte de observação. Ou seja, quanto tempo o Follower leva para
//atingir o Leader. Carros fora do horizonte, aplicar regra padrão. Caso contrário,
//aplicar regra de desaceleração.

using namespace std;

TModel::TModel():
mRedLights(NULL),
mClusters(NULL),
mReAjusted(0),
mLastVehicles(NULL){
     srand ( time(NULL) );
}
TModel::~TModel(){}

void TModel::finalizer(void){
  if (mLastVehicles != NULL)
    free(mLastVehicles);

  getGrid()->finalize();

}

void TModel::addONEObstables(void){
    Obstacle *obstacle = new Obstacle();

    obstacle->ID = getGrid()->getID();

    obstacle->x  = mParam->cellX - 1;
    obstacle->y  = mParam->cellY - 1;
    obstacle->lg = 1;
    getGrid()->addEntity(obstacle);

}

void TModel::initialCondition(float dGlobal){
    //Define how many vehicles will be on the road
    Vehicle vehicle;
/*
 * Observation 2017, March 18th
 * For each vehicle Verifying if it is in a list of log vehicles vehicleLog
 * To do this: Verifying if there is a substring with the number.
 *             Set true vehicle parameter
 *             Put in Vehicle.cpp code to log the vehicle performance
 *
 */
    int totalVehicles = 0,
        *vehiclesTypes = new int[getGrid()->getVehiclesTypeSize()];
    mStatistic = false;

    for (int i = 0; i < getGrid()->getVehiclesTypeSize(); i++){
        tpVehiclesType *type  = getGrid()->getVehicleType(i);

        float dVehicle  = type->percent * dGlobal / static_cast<float> (type->size);

        int   nVehicles = static_cast <int> (dVehicle * static_cast <float> (mParam->cellX));

        if (nVehicles == 0) nVehicles = 1;

        totalVehicles += nVehicles;

        vehiclesTypes[i] = nVehicles;
    }
    totalVehicles *= mParam->cellY;

    getGrid()->clearMemories();

    if (mParam->logVehicles > 0)
      assert(posix_memalign((void**) &mLastVehicles, ALIGN, mParam->logVehicles *  sizeof(Vehicle*)) == 0);

    int lIndex = 0;
    for (int road = 0; road < mParam->cellY; road++){
        int cell = 0;
        for (int i = 0; i < getGrid()->getVehiclesTypeSize(); i++){
            tpVehiclesType *type  = getGrid()->getVehicleType(i);
            int length    = type->size;
            int nVehicles = vehiclesTypes[i];
            for(int iveicles = 0; iveicles < nVehicles; iveicles++){
                Vehicle *vehicle = new Vehicle();

                vehicle->ID = getGrid()->getID();

                vehicle->x = cell + length - 1;
                vehicle->vx = 0;
                vehicle->vxNew = 0;
                vehicle->y = road;
                vehicle->vy = 0;
                vehicle->yChange = false;
                vehicle->mLights = false;
                vehicle->lg = type->size;
                vehicle->myDensity = dGlobal;
                vehicle->mSensor = this->getSensor();
                vehicle->next = NULL;
                vehicle->prev = NULL;
                vehicle->type = getGrid()->getVehicleType(i);
                vehicle->defineLattice(mParam->cellX, mParam->cellY);
                vehicle->save = false;
                vehicle->mDV = 0;
                vehicle->mDIST = 0;
                vehicle->setFileSufix(mParam->modelName);
                if (mParam->logVehicles > 0){
                  if (iveicles >= (nVehicles - mParam->logVehicles)){
                      vehicle->save = true;
                      mLastVehicles[lIndex++] = vehicle;

                  }//end-if (iveicles == (nVehicles - 1 - mParam->logVehicles)){
                }//end-                if (mParam->logVehicles > 0){

                getGrid()->addEntity(vehicle);
                cell += length;

            }

        }
    }


    delete[] vehiclesTypes;
    vehiclesTypes = NULL;


    //Setting up sensor
    mSensor->setFileName(mParam->modelName);
    mSensor->setGrid(this->getGrid());
    mSensor->reset();


    mReAjusted = 0;



}

void TModel::finalCondition(void){

   string fileName = "";
   fstream log;
   unsigned int nVehicles = 0;
   Entity *e = getGrid()->getEntities();
   int iOcc = 0;
   while (e != NULL){
      if (e->whoAmI().compare("Vehicle") == 0){
         nVehicles++;
         iOcc += e->lg;
      }

      e = e->next;
   }

   double percent = static_cast<double> (mReAjusted) /  static_cast<double>(getParam()->sTime - getParam()->dTime);
   if (nVehicles > 0)
      percent = percent / static_cast<double> (nVehicles) ;

   double occupation = static_cast<double>(iOcc) / static_cast<double>(getParam()->cellX * getParam()->cellY);

   if (getParam()->logCluster){

      fileName = mPath + "cluster.adjusted." + getParam()->modelName + ".txt";
      log.open(fileName, fstream::out | fstream::app);
      assert(log.is_open());



      log << fixed << setw(10) << setprecision(5) << occupation << " " << setw(10) << (getParam()->sTime - getParam()->dTime) << " " << setw(10) << mReAjusted;
      log << fixed << setw(10) << setprecision(5) <<  percent <<  endl;

      log.close();


   }


}


void TModel::applyRules(int step){





    //Applying movement rules
    Entity *ptr = getGrid()->getEntities();
    while (ptr != NULL){
        executeRules(ptr);
        ptr = ptr->next;
    }


    do{
         tpCluster *list = NULL;
         Vehicle *vehicle = NULL;


        // sprintf(line, "\t<mRedLights> %p </mRedLights>", mRedLights);

         while (mRedLights != NULL){
            list = mRedLights;
            vehicle = list->vehicle;
            vehicle->mLights = false;



            buildCluster(vehicle);
            mRedLights = mRedLights->next;
            delete list;
            list = NULL;
         }//end-if (list != NULL){

         list = NULL;
         vehicle = NULL;


         while (mClusters != NULL){
            list = mClusters;
            vehicle = list->vehicle;
            solveCluster(vehicle);
            mClusters = mClusters->next;
            delete list;
            list = NULL;
         }


    }while(mRedLights != NULL);




   update(step,  false);



}


void TModel::update(int step,  bool changing, bool initial){

   std::fstream output;

   //Updating all vehicles
   bool  statistic = (step > getParam()->dTime) ;
   getGrid()->clearGrid();


    Entity *pEntity = getGrid()->getEntities();
    while (pEntity != NULL){


      if (pEntity->whoAmI().compare("Vehicle") == 0){
         if ((step % getParam()->stTime) == 0){//Whether save statistic time step, clear
            pEntity->clear();

         }

         Vehicle **ppVehicle = (Vehicle**) (&pEntity);
//-----------------------------------------------------------------------------
         assert(ppVehicle != NULL);
         if (!initial){
            if (changing)
            (*ppVehicle)->updateY(statistic, step);
            else{
                (*ppVehicle)->update(statistic, step);


            }
         }



         int y = pEntity->y;
         for (int k = 0 ; k <   pEntity->lg; k++){
            int x  =  pEntity->x - k;
            if (x < 0)
               x = getParam()->cellX + x;

           // assert(!changing);

            if ((getGrid()->getGrid(x, y) != EMPTY) && (!changing)){
               std::cerr << "\t [ERROR] " << __FILE__ << " at " << __LINE__ << "\t Time:" << step <<  std::endl ;

               Vehicle *v = static_cast<Vehicle*>(pEntity) ; //getGrid()->getGrid(x, y));

               // v.mBreakForeseenForeseenLight = true;
               std::cerr << "V1(ID= " << v->ID << ",X= "
               <<  v->x  << ",Y= "
               <<  v->y  << ", VX="
               <<  v->vx << ", VNEW="
               <<  v->vxNew <<  ")" << std::endl;


               v = static_cast<Vehicle*>(getGrid()->getGrid(x, y));
               std::cerr << "V2(ID= " <<  v->ID << ",X= "
               <<  v->x  << ",Y= "
               <<  v->y  << ", VX="
               <<  v->vx << ", VNEW="
               <<  v->vxNew << ")" << std::endl;
               exit(-1);

            }

            getGrid()->setGrid(x, y, pEntity);

         }//end-for (int k = 0 ; k <  static_cast <int> ( pVehicle->lg); k++){
//-----------------------------------------------------------------------------
      }else  if (pEntity->whoAmI().compare("Obstacle") == 0){//end-      if (pEntity->whoAmI().compare("Vehicle") == 0){
            int y = pEntity->y;
            for (int k = 0 ; k <   pEntity->lg; k++){
                int x  =  pEntity->x - k;
                if (x < 0)
                    x = getParam()->cellX + x;

                getGrid()->setGrid(x, y, pEntity);
            }
      }
      pEntity = pEntity->next;
    }//while (pEntity != NULL){




   if ((!changing) && (step > 0)){
      //Verifying if there is vehicle stopped on moviment detector cell.
      if (statistic){
         for (int j = 0; j < getParam()->cellY; j++){

            Entity *e = getGrid()->getGrid(getParam()->cellX-1, j);
            if (e != NULL){
               if (e->whoAmI().compare("Vehicle") == 0){
                  Vehicle *v = reinterpret_cast<Vehicle*> (e);
                  if (v->vx == 0)
                     getSensor()->setCellInStopped(j);
               }

            }



         }//end- for (int j = 0; j < mRules->getParam()->cellY; j++){

         this->getSensor()->update();
         if ((step % getParam()->stTime) == 0){

            getSensor()->saveStatistic(step);


         }


//--------------------------------------------------
//After updating all vehicles velocity, the model logs vehicles
  if (mParam->logVehicles > 0){
    for (int i = 0; i < mParam->logVehicles; i++){
      Vehicle *v =   mLastVehicles[i];
      int iAheadVel = 0,
          iDist     = 0;

      getDistanceAndVelocity(v, &iDist, &iAheadVel, NULL);

      v->mDV = v->vx - iAheadVel;
      v->mDIST = iDist;


      v->log();
    }
  }

//--------------------------------------------------
      }//end- if (statistic){
   }//end- if ((!changing) && (step > 0)){
}

void TModel::executeRules(Entity *entity, bool changing){

   if (entity->whoAmI().compare("Vehicle") == 0){
       movementRules(static_cast<Vehicle *> (entity));
   }

}

int TModel::decelerationRule(int iDist,
			                 int iAheadVel,
			                 int iVel){

      double d = 2.0f * static_cast<double> (iDist);
      double vl =  static_cast<double>(iAheadVel);
      double vf =  static_cast<double>(iVel);
      double acc = ((vf * vf) - (vl * vl)) / d;
      //AQUI

      if (acc < 0.1f)
         return 0;

      if (acc > getParam()->deceleration)
         acc = getParam()->deceleration;

      return truncCurve3(acc);

}

void TModel::movementRules(Vehicle *pVehicle, bool isCluster){
   int iVel = 0,
       iDist = 0,
       iAheadVel = 0,
       iOldV = 0;

   Vehicle **ppVehicle  = &pVehicle;
   tpVehiclesType *type = (*ppVehicle)->type;

  // double   alpha = (1.0f - betaFunction(type->param));
   double   alpha = betaFunction(type->param);



   getDistanceAndVelocity(pVehicle, &iDist, &iAheadVel, isCluster);

                                                                                 int myDistance = iDist;


   double dAheadVel = static_cast<double> (iAheadVel) * alpha;
   //iDist += iAheadVel;
   iDist += truncCurve3(dAheadVel);

   bool breaking = false;

   if (!isCluster){
      iVel = (*ppVehicle)->vx;
      iOldV = iVel;
      double acc = type->inc * alpha;
      int DV = (iVel - iAheadVel);
      double dVel = static_cast <double> (iOldV);
      double dDist = static_cast <double> (myDistance) * 1.0f; //alpha;






      if ( (dDist <= (getParam()->spacePerception * dVel)) && (DV > 0)){

            int dAcc = decelerationRule(myDistance, iAheadVel, iVel);
            assert(dAcc <= getParam()->deceleration);
            iVel = iVel - dAcc;



           breaking = true;
       }else{
        //Free flow rule
        iVel += (truncCurve3(acc) - type->desc);
     }

      iVel = max(iVel, 0);
      iVel = min (iVel,  min(mParam->vMax, (*ppVehicle)->type->vMax));
   }
   else{
       iOldV = iVel;
       iVel = (*ppVehicle)->vxNew;
       int DV = (iVel - iAheadVel);

       if ((iDist > 0) && (DV > 0)){

           int dAcc = decelerationRule(myDistance, iAheadVel, iVel);
           assert(dAcc <= getParam()->deceleration);


           iVel = (*ppVehicle)->vxNew - dAcc;

           iVel = max(iVel, 0);
           breaking = true;
       }//end-if (iDist >0){
   }

   if (iVel > iDist){
      iVel = iDist;
      breaking = true;
   }

   if ((iVel < iOldV) && (!breaking))
      breaking = true;

   (*ppVehicle)->mLights =  breaking;

   if (breaking){
      insertIntoRedLights(pVehicle);
   }

   (*ppVehicle)->vxNew = iVel;

}



void TModel::getDistanceAndVelocity(Vehicle *v, int *dist, int *vel, bool rule){
   //rule == true then solve cluster
   //otherwise movement rule
   //tpVehiclesType *type = getGrid()->getVehicles(idx).type;

   int x = v->x,
       y = v->y,
       iDist = 0;
   Entity *cell = NULL;

   do
   {
      x++;
      iDist++;
      if (x == mParam->cellX)
         x = 0;
      cell = getGrid()->getGrid(x, y);

   } while (cell == EMPTY); //((cell == EMPTY) && (iDist <= mParam->vMax));

   //if (iDist >= mParam->vMax   ){
     //    *vel = 0; //Significa que nao tem veiculo

   //}else{

         if (cell->whoAmI().compare("Vehicle") == 0){
            Vehicle *v = static_cast<Vehicle *> (cell);
            if (!rule){
                *vel = v->vx;

            }else{

               *vel = v->vxNew;


            }

         }else{
            *vel = 0;


         }

  // }


   iDist--;
   if (iDist < 0)
      iDist = 0;

   *dist = iDist;

}

Entity * TModel::getBack(int *out,
                          int myX,
                          int myY,
                          int myLg){

   int x = myX - myLg,
       y = myY,
       iDist = 0;
   Entity *entity = NULL;

   //x++;
   do{

      if (x < 0)
         x = mParam->cellX + x;


      entity = getGrid()->getGrid(x, y);
      if (entity == EMPTY)
      {
          iDist++;
          x--;
      }
   } while  (entity == EMPTY);


   *out = iDist;

   return entity;



}

/*
 * It is a method that depends on what king of information I would show.
 * It means: I can see on the screen or saved into a file.
 */
void TModel::debug(int time){
   /*
   cerr << "Time = " << time << endl;
   for (int j = 0; j < getParam()->cellY; j++){
      for (int i = 0; i < getParam()->cellX;  i++){
               int idx = getGrid()->getGrid(i, j);
         if (idx > EMPTY){
               Vehicle *pVehicle    = getGrid()->getVehiclePointer(idx);
               cerr << pVehicle->ID << "," << pVehicle->x << " , " << (int) pVehicle->y << endl;

         }
      }
   }
   cerr << "--------------------------------------------------------------------------------------------";
   cerr << endl;
   */
}


int TModel::truncCurve3(double v){
    int result = static_cast<int> (floor(v+0.5f));


    return result;

}




double  TModel::betaFunction (double gamaAlpha,
                                    double gamaBeta,
                                    double gamaAlphaBeta,
                                    double alpha,
                                    double beta,
                                    double x){


	double  xalpha = pow(x, (alpha-1.0f)),
                xbeta  = pow((1.0f - x), (beta-1.0f));

	double ret = gamaAlphaBeta / (gamaAlpha * gamaBeta);

    ret *= xalpha * xbeta;

    return ret;
}

double  TModel::betaFunction (double *vet){

	double x, y, y1;
    do{
        x  = static_cast <double> (rand() % 65535 + 1) / 65535.0f;
        y1 = static_cast <double> (rand() % 65535 + 1) / 65535.0f;

        y = betaFunction( vet[2],
                         vet[3],
                         vet[4],
                         vet[0],
                         vet[1],
                         x);


    }while (y1 > y);

    if (x < 0.0f)
        x = 0.0f;

    if (x > 1.0f)
        x = 1.0f;

    return x;

}


void TModel::buildCluster(Vehicle *vehicle){

        Vehicle *pVehicle = vehicle;
        Vehicle *bVehicle = NULL;
        Entity *backPtr = NULL;

        int iDist = 0,
            iVel  = 0;

        do{
         backPtr = getBack(&iDist, pVehicle->x, pVehicle->y, pVehicle->lg);



         if (backPtr->whoAmI().compare("Vehicle") != 0)
            return;

         bVehicle = static_cast<Vehicle *> (backPtr);

         iVel = bVehicle->vxNew;
         iDist += pVehicle->vxNew;

         if (iVel > iDist){

            insertIntoCluster(bVehicle);
         }

         pVehicle = bVehicle;
         bVehicle = NULL;

      }while (iVel > iDist);



}





void TModel::solveCluster(Vehicle *vehicle){
  movementRules(vehicle, true);

  if (mStatistic)
      mReAjusted++;
}

void TModel::insertIntoCluster(Vehicle *vehicle){
   tpCluster *cluster =  new tpCluster();

   cluster->vehicle = vehicle;
   cluster->next = NULL;

   tpCluster *ptr = mClusters;

   if (mClusters == NULL){
      mClusters = cluster;
      return;
   }

   while (ptr->next != NULL){
      ptr = ptr->next;
   }

   ptr->next = cluster;

}





void TModel::insertIntoRedLights(Vehicle *vehicle){
   tpCluster *cluster =  new tpCluster();
   cluster->vehicle = vehicle;
   cluster->next = NULL;

   tpCluster *ptr = mRedLights;

   if (mRedLights == NULL){
      mRedLights = cluster;
      return;
   }

   while (ptr->next != NULL){
      ptr = ptr->next;
   }

   ptr->next = cluster;

}


float TModel::getRandom(void)
{ return static_cast <float> (rand() % 10000 + 1) / 10000.0f;};


float TModel::alphaFunction(float a)
{ return 1.0f-a; }; ///*powf((1-a), 2);*/ };

void TModel::saveLog(string msg){
    fstream log;
    log.open("Log.txt", fstream::out | fstream::app);
    log << msg << endl;

    log.close();

}
/*
    //////////////////////////////////
    // Highway direction
    // Initial --> Final = 0 is left and 1 is right
    // 0 - left
    // 1 - right
    //////////////////////////////////
*/
TModel_ML_SYM::TModel_ML_SYM():
TModel(){
    //mIndexDensity(0){
};

TModel_ML_SYM::~TModel_ML_SYM(){

};




/**
 * Apply symmetric rules or simple ahead movement based on T_UFRRJ
 */
void TModel_ML_SYM::applyRules(int step){


      Entity *pEntity = getGrid()->getEntities();
      while (pEntity != NULL){
           if (pEntity->whoAmI().compare("Vehicle") == 0){
             changeLaneRules((Vehicle**) (&pEntity));
           }
          pEntity = pEntity->next;
      }
      update(step,  true);

      TModel::applyRules(step);


}

void TModel_ML_SYM::changeLaneRules(Vehicle **ppVehicle){

    tpVehiclesType *type = (*ppVehicle)->type;

    bool changed = false;
    bool bestFlow = rule_BestFlow(ppVehicle, RIGHT2LEFT);

    if(rule_MovingFaster(ppVehicle)  && !changed && bestFlow){
        if (rule_Safe(ppVehicle, RIGHT2LEFT)){
            if (getRandom() <= type->left_p){

                (*ppVehicle)->vy = -1;
                changed = true;
            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){


    bestFlow = rule_BestFlow(ppVehicle, LEFT2RIGHT);
    if(rule_MovingFaster(ppVehicle) && !changed && bestFlow){
        if (rule_Safe(ppVehicle, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                (*ppVehicle)->vy = +1;
                changed = true;

            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
}

/*
 * Safe rules, verify if the car can chance, it he fits on the destination lane
 */
bool TModel_ML_SYM::rule_Safe(Vehicle **ppVehicle, int iDir){
    Entity    *pEntity = NULL;
    Vehicle   *vAheadVehicle = NULL,
              *vBackVehicle = NULL;

    tpVehiclesType *type = (*ppVehicle)->type;


    int iMyY = static_cast<int> ((*ppVehicle)->y),
        iMyX = (*ppVehicle)->x,
        iBackDistance = 0,
        iAheadDistance = 0,
        iPtr = 0,
        iLen =  type->size ,
        iVel =  ((*ppVehicle)->vx);

    if ((iDir == RIGHT2LEFT) && (iMyY == 0))
        return false;

    if ((iDir == LEFT2RIGHT) && (iMyY == (mParam->cellY-1)))
        return false;

    switch (iDir){
        case RIGHT2LEFT: iMyY--; break;
        case LEFT2RIGHT: iMyY++; break;
    }




    //Immediately beside
    for (int i = 0; i < iLen; i++){
    	iMyX = (*ppVehicle)->x - i;

    	if (iMyX < 0)
    		iMyX = mParam->cellX + iMyX;

    	pEntity = getGrid()->getGrid(iMyX, iMyY);
    	if (pEntity != NULL)
    		return false;
    }



    //Find ahead  vehicle
    iMyX = (*ppVehicle)->x;
    iAheadDistance = 0;
    do{
    	iMyX++;
        iAheadDistance++;
        if (iMyX == mParam->cellX)
        	iMyX = 0;

        pEntity =  getGrid()->getGrid(iMyX, iMyY);


    }while ((pEntity == NULL) && (iAheadDistance <= mParam->cellX));



    if (iAheadDistance > mParam->cellX)
    	return true;

    if (iAheadDistance > 0)
    	iAheadDistance--;

    vAheadVehicle = (Vehicle *) pEntity;



    //Find back  vehicle
    iMyX = (*ppVehicle)->x - iLen;
    iBackDistance = 0;
    //Finding the previous vehicle
    do{
    	iMyX--;
    	iBackDistance++;
        if (iMyX < 0)
        	iMyX = mParam->cellX - 1;

        pEntity = getGrid()->getGrid(iMyX, iMyY);
        //assert(iAheadDistance < 101);
    }while ((pEntity == NULL) && (iBackDistance <= mParam->cellX));


    if (iBackDistance > mParam->cellX)
     	return true;


    vBackVehicle =  (Vehicle*) pEntity;
    if (iBackDistance > 0)
    	iBackDistance--;

    //#1
    if (vBackVehicle->whoAmI().compare("Vehicle") == 0)
    {
        type = vBackVehicle->type;
        //double alpha     = betaFunction(type->param);
        double beta      = 1.0f - betaFunction(type->param);
        double dheadway  =  type->aheadInt;

        int diffV = vBackVehicle->vx - iVel;
        if (diffV > 0){
            double dTH = static_cast<double>(iBackDistance) / static_cast<double>(diffV);

            if (dTH < (dheadway * beta))
                return false;



        }//end-if (diffV != 0){
        /*
        assert(ds >= 0.0f);

        if (iBackDistance < iVel);
            return false;
        */
        //The rule is based on temporal distance which considers as distance as velocity of both vehicles
        //Both vehicles move at the same velocity: diffV is zero and in this case it is safe to change
        //My vehicle is faster than previous, diffV is minor than zero. It is safe to change
        //It is important to not hit one to another when analyzed vehicle is slower than the previous one
    }




    //#2
    if (vAheadVehicle->whoAmI().compare("Vehicle") == 0)
    {
        type = (*ppVehicle)->type;
        //double alpha     = betaFunction(type->param);
        double beta      = 1.0f - betaFunction(type->param);
        double dheadway  =  type->aheadInt;

        int diffV = iVel - vAheadVehicle->vx;
        if (diffV != 0){
            double dTH = static_cast<double>(iAheadDistance) / static_cast<double>(diffV);
            if (dTH < (dheadway * beta))
                return false;


        }//end-if (diffV != 0){

    }else{
      cerr << "Erro in considering ahead vehicle in line 995 at TModel.cpp file" << endl;
      exit(-1);
    }


//    if (iVel > iAheadDistance)
  //  	return false;

    return true;
}

/*
 * Being motivated whenever the driver cannot move in desired fVelocity
 * In case of symmetric hule, this motivation can be applied for both sides
 * Left to Right - Right to Left
 *
 */
bool TModel_ML_SYM::rule_MovingFaster(Vehicle **ppVehicle){

   double   dVel = 0.0f,
            dTH   = 0.0f,
            deltaV    = 0.0f,
            dheadway  = 0.0f,
            dsafedist = 0.0f,
//            deltaVMax = 50.0f,
            ds = 0.0f,
            alpha = 0.0f,
            beta  = 0.0f;

   int iVel = 0,
       iDist = 0,
       iAheadVel = 0,
       iMax = 0;

   tpVehiclesType *type = (*ppVehicle)->type;

   alpha = betaFunction(type->param),
   beta  = 1.0f - betaFunction(type->param);

   deltaV    =  (*ppVehicle)->type->inc;
   dheadway  =  (*ppVehicle)->type->aheadInt;
   dsafedist =   (*ppVehicle)->type->safeDist;

   iMax = min(mParam->vMax, (*ppVehicle)->type->vMax);
   getDistanceAndVelocity(*ppVehicle  , &iDist, &iAheadVel);


   dVel = static_cast<double> ((*ppVehicle)->vx) + ( (deltaV * alpha) - 0.5f);
   //plot '2_4.txt' u (floor((4*$1)-0.5)):($2) w p
   dVel = floor(dVel);

   iVel = static_cast<int> (dVel);

   iVel = max(iVel, 0);

   iVel = min (iVel, iMax);
   dVel = static_cast<double> (iVel);
   if (fabs(dVel - static_cast<double> (iAheadVel)) < ERROR)
       return false;

   dTH = static_cast<double>(iDist) / (dVel - static_cast<double> (iAheadVel)); //+ (deltaV * alpha));

   if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
    ds = (dsafedist * beta) ;//* dTH	;
   }


   assert(ds >= 0.0f);
   iDist -= truncCurve3(ds);

   if (iDist < 0)
      iDist = 0;

   if (iVel > iDist)
       return true;

   return false;



}

/*
 * Verify if the destination lane presents best flow
 */
bool TModel_ML_SYM::rule_BestFlow(Vehicle **ppVehicle, int iDir){


    Vehicle *vAheadVehicle  = NULL,
            *vBesideVehicle = NULL;

    Entity *pEntity = NULL;
    tpVehiclesType *type = (*ppVehicle)->type;

    int iMyY = static_cast<int> ((*ppVehicle)->y),
        iMyX = (*ppVehicle)->x,
        iBesideDistance = 0,
        iLen =  type->size ,
        iVel = static_cast<int> ((*ppVehicle)->vx),
        iVelBeside = 0;

    if ((iDir == RIGHT2LEFT) && (iMyY == 0))
        return false;

    if ((iDir == LEFT2RIGHT) && (iMyY == (mParam->cellY-1)))
        return false;

    switch (iDir){
        case RIGHT2LEFT: iMyY--; break;
        case LEFT2RIGHT: iMyY++; break;
    }
    //Immediately beside
    for (int i = 0; i < iLen; i++){
        iMyX = (*ppVehicle)->x - i;

        if (iMyX < 0)
            iMyX = mParam->cellX + iMyX;

        pEntity = getGrid()->getGrid(iMyX, iMyY);
        if (pEntity != NULL)
            return false;
    }

    iMyX = (*ppVehicle)->x;
    //iAheadDistance = 0;
    do{
        iMyX++;
        iBesideDistance++;
        if (iMyX == mParam->cellX)
            iMyX = 0;

        pEntity = getGrid()->getGrid(iMyX, iMyY);


    }while ((pEntity == NULL) && (iBesideDistance <= mParam->cellX));

    if (iBesideDistance > mParam->vMax)
        return true;

    if (iBesideDistance > 0)
        iBesideDistance--;

    vBesideVehicle =  (Vehicle *) pEntity;
    iVelBeside = static_cast<int> (vBesideVehicle->vx);



    //iBesideDistance += iVelBeside;




    if (iVelBeside <= 0)
        return false;




    //iBesideDistance += iVelBeside;

    if (iVel <=  iVelBeside)
        return true;


    return false;


}

void TModel_ML_SYM::finalizer(void){

    TModel::finalizer();

}


/**
 * Apply symmetric rules or simple ahead movement based on T_UFRRJ
 */
TModel_ML_ASYM::TModel_ML_ASYM():
TModel_ML_SYM(){
    //mIndexDensity(0){
};

TModel_ML_ASYM::~TModel_ML_ASYM(){
};

void TModel_ML_ASYM::changeLaneRules(Vehicle **ppVehicle){
    tpVehiclesType *type = (*ppVehicle)->type;

    bool changed = false;
    bool bestFlow = rule_BestFlow(ppVehicle, RIGHT2LEFT);

    if(rule_MovingFaster(ppVehicle)  && !changed && bestFlow){
        if (rule_Safe(ppVehicle, RIGHT2LEFT)){
            if (getRandom() <= type->left_p){

                (*ppVehicle)->vy = -1;
                changed = true;


            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){


    //bestFlow = rule_BestFlow(idx, LEFT2RIGHT);
    if(rule_FasterBehind(ppVehicle) && !changed ){
        if (rule_Safe(ppVehicle, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                (*ppVehicle)->vy = +1;
                changed = true;


            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){


   bestFlow = rule_BestFlow(ppVehicle, LEFT2RIGHT);
    if(rule_FasterAhead(ppVehicle) && !changed && bestFlow){
        if (rule_Safe(ppVehicle, LEFT2RIGHT)){
            if (getRandom() <= type->right_p){
                (*ppVehicle)->vy = +1;
                changed = true;


            }//end-if (getRandom() < type->left_p){
        }//end-if (r05_Change(idx, RIGHT2LEFT)){
    }//end-if(r01_Motivated(idx) && !changed ){
}


/*
 * r03_Motivated rule makes drivers change to slower lane.
 * r03_Motivated rule takes into account ahead vehicle.
 * In case of Brazil, it is right lane.
 */
bool TModel_ML_ASYM::rule_FasterAhead(Vehicle **ppVehicle){

   double   dVel = 0.0f,
            dTH   = 0.0f,
            deltaV    = 0.0f,
            dheadway  = 0.0f,
            dsafedist = 0.0f,
//            deltaVMax = 50.0f,
            ds = 0.0f,
            alpha = 0.0f,
            beta  = 0.0f;

   int iVel = 0,
       iDist = 0,
       iAheadVel = 0,
       iMax = 0;


   tpVehiclesType *type = (*ppVehicle)->type;

   alpha = betaFunction(type->param),
   beta  = 1.0f - betaFunction(type->param);

   deltaV    =  (*ppVehicle)->type->inc;
   dheadway  =  (*ppVehicle)->type->aheadInt;
   dsafedist =   (*ppVehicle)->type->safeDist;

   iMax = min(mParam->vMax, (*ppVehicle)->type->vMax);
   getDistanceAndVelocity(*ppVehicle, &iDist, &iAheadVel);
   if (iDist >= iMax)
       return true;


   dVel = static_cast<double> ((*ppVehicle)->vx) + ( (deltaV * alpha) - 0.5f);
   //plot '2_4.txt' u (floor((4*$1)-0.5)):($2) w p
   dVel = floor(dVel);

   iVel = static_cast<int> (dVel);

   iVel = max(iVel, 0);

   iVel = min (iVel, iMax);

   dVel = static_cast<double> (iVel);


    if (fabs((dVel - static_cast<double>(iAheadVel))) <= ERROR)
        return false;

   dTH = static_cast<double>(iDist) / (dVel - static_cast<double> (iAheadVel)); //+ (deltaV * alpha));

   if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
    ds = (dsafedist * beta) ;//* dTH	;
   }


   assert(ds >= 0.0f);
   iDist -= truncCurve3(ds);

   if (iDist < 0)
      iDist = 0;

   if (iVel <= iDist)
       return true;

   return false;



}

/*
 * Brief: back vehicle is getting closer, the analyzed one becomes
 * motived to change the lane
 */

bool TModel_ML_ASYM::rule_FasterBehind(Vehicle **ppVehicle){
    double   dVel = 0.0f,
        dTH   = 0.0f,
        deltaV    = 0.0f,
        dheadway  = 0.0f,
        dsafedist = 0.0f,
//            deltaVMax = 50.0f,
        ds = 0.0f,
        alpha = 0.0f,
        beta  = 0.0f;

    //Veículo que vem detrás
    Vehicle *vBackVehicle;

    //Vehicle **ppVehicle  = &pVehicle;
    tpVehiclesType *type = NULL;

    int iVel       = 0,
        //iBackVel   = 0,
        iBackDist  = 0,
        iMax = 0;

    Entity *entity = getBack(&iBackDist, (*ppVehicle)->x,  (*ppVehicle)->y, (*ppVehicle)->lg);

    if (entity->whoAmI().compare("Vehicle") == 0){
      vBackVehicle = (Vehicle*) entity;

      type = vBackVehicle->type;

       iMax = min(mParam->vMax, type->vMax);

      alpha = betaFunction(type->param),
      beta  = 1.0f - betaFunction(type->param);

      deltaV    =  type->inc;
      dheadway  =  type->aheadInt;
      dsafedist =   type->safeDist;

      dVel = static_cast<double> (vBackVehicle->vx) + ( (deltaV * alpha) -   0.5f);

      dVel = floor(dVel);

      iVel = static_cast<int> (dVel);

      iVel = max(iVel, 0);

      iVel = min (iVel, iMax);

      dVel = static_cast<double> (iVel);
      int auxv = (*ppVehicle)->vx;

      if (fabs((dVel - static_cast<double>(auxv))) <= ERROR)
          return false;

      dTH = static_cast<double>(iBackDist) / (dVel - static_cast<double>(auxv)); //+ (deltaV * alpha));

      if (( dTH >  0.0f) && (dTH <= (dheadway * beta))){
       ds = (dsafedist * beta) ;//* dTH	;
      }


     assert(ds >= 0.0f);
     iBackDist -= truncCurve3(ds);

     if (iBackDist < 0)
        iBackDist = 0;

     if (iVel > iBackDist)
         return true;

     return false;
    }
    return false;


}


