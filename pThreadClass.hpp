#ifndef _PTHREADCLASS_HPP_
#define _PTHREADCLASS_HPP_
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <cassert>




unsigned int GetNumberOfProcessor(void);



class pThreadClass
{
   public:
      pThreadClass(void);
      virtual ~pThreadClass(void);
      bool create (void);
      bool destroy(void);
      void wait(void);
      virtual void* execThread(void);
      static void *runThread(void*);


      unsigned int getMyThread(void)
      { return mMyThread; };

      bool getRunning(void)
      { return mRunning; };

      void stop(void);

		pthread_mutex_t   *m_Mutex;
		pthread_cond_t    *m_Cond;
        pthread_barrier_t *m_Barrier;

   protected:
      pthread_t mThreadId;
      int       mThread;

      static unsigned int mNumberofthreads;
      unsigned int        mMyThread,
                          mMyCount;
      bool                mRunning;
};

#endif
