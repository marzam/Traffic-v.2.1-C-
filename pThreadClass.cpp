#include <cstdio>
#include <pthread.h>
#include <pThreadClass.hpp>
#include <sched.h>


unsigned int pThreadClass::mNumberofthreads = 0;
pThreadClass::pThreadClass():
mThread(-1),
//mPtrBarrier(NULL),
mMyCount(0),
mRunning(false),
m_Mutex(NULL),
m_Cond(NULL)
{
   mMyThread = mNumberofthreads++;

};

pThreadClass::~pThreadClass()
{};

void pThreadClass::wait(void)
{
   if (mThread == 0)
   {
      pthread_join( mThreadId, NULL);
      mThread = -1;
      mRunning = false;
   }
};

bool pThreadClass::create (void)
{
   int p = sched_get_priority_min( SCHED_FIFO );
   pthread_attr_t tattr;
   sched_param param;

   pthread_attr_init (&tattr);
   pthread_attr_getschedparam (&tattr, &param);
   param.sched_priority = p;
   pthread_attr_setschedparam (&tattr, &param);




   if (mThread != 0)
      mThread = pthread_create( &mThreadId, &tattr, pThreadClass::runThread, reinterpret_cast <void*> (this));
   else
      return false;

   mRunning = true;
   return mRunning;
};

void  pThreadClass::stop(void){
    pthread_mutex_lock(m_Mutex);
    mRunning = false;
    pthread_mutex_unlock(m_Mutex);
};

bool pThreadClass::destroy(void)
{

   if (mThread == 0)
   {
      pthread_detach(mThreadId);
      mThread = -1;
      return true;
   }

   return false;
};

void *pThreadClass::execThread(void)
{
   mMyCount = 0;
   while (mMyCount < 10)
   {
       /*
      pthread_mutex_lock(m_Mutex);
      mMyCount++;
      fprintf(stdout, "\n(Before)[%d][%d]", mMyThread, mMyCount);
      fflush(stdout);
		assert(pthread_cond_wait(m_Cond, m_Mutex)==0);
      fprintf(stdout, "\n(Afer)[%d][%d]", mMyThread, mMyCount);
      fflush(stdout);
      pthread_mutex_unlock(m_Mutex);
      */


      fprintf(stdout, "> [%d][%d]\n", mMyThread, mMyCount);
      fflush(stdout);
      pthread_barrier_wait(m_Barrier);
      mMyCount++;
   }
   fprintf(stdout, "\nFinishing %d", mMyThread);
   fflush(stdout);
   return NULL;
};


void *pThreadClass::runThread(void* param)
{ return reinterpret_cast <pThreadClass *> (param)-> execThread(); };


unsigned int GetNumberOfProcessor(void)
{
   return static_cast <unsigned int> (sysconf(_SC_NPROCESSORS_ONLN));
};
