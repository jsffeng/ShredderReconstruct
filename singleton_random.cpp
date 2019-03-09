#ifdef MULTI_THREAD_FLAG
#include<mutex>
#endif

#include <random>
#include "singleton_random.h"

using namespace std;

#ifdef UTFLAG
int SingletonRandom::s_instance_numbers_ = 0;
#endif
unsigned int SingletonRandom::s_max_val_ = 0;
std::unique_ptr<SingletonRandom> SingletonRandom::p_instance_ = nullptr;

SingletonRandom& SingletonRandom::GetInstance()
{
  // Following will be used if multi-threads
#ifdef MULTI_THREAD_FLAG
   static once_flag oc_flag;
#endif
  
  if (s_max_val_ == 0)
  {
    throw runtime_error("Need to set s_max_val_ value before using SingletonRandom!");	
  }

  // Following will be used if multi-threads
#ifdef MULTI_THREAD_FLAG
   call_once(oc_flag, [&](){ p_instance_.reset(new SingletonRandom);});
#else
  if (p_instance_ == nullptr)
  {
     p_instance_.reset(new SingletonRandom);
  #ifdef UTFLAG
     s_instance_numbers_++; 
  #endif 
  }
#endif
  return *p_instance_;

}

#if (defined UTFLAG) && (defined MULTI_THREAD_FLAG) 
// Test function for multi-threads for SingletonRandom
mutex TESTSingleton::instance_mutex;
bool TESTSingleton::b_instance_equal = false;
int TESTSingleton::b_insance_update = 0;

void TESTSingleton::TESTGetInstance()
{
  static SingletonRandom *single_random_ptr = nullptr;
  
  SingletonRandom::s_max_val_ = 100;
  SingletonRandom& random_instance = SingletonRandom::GetInstance();
  if (single_random_ptr == nullptr)
  {
    single_random_ptr = &random_instance;

    lock_guard<mutex> lockGuard(instance_mutex);
    b_instance_equal = true;
    ++b_insance_update;
  }
  else
  {
    if ( single_random_ptr != &random_instance)
    {  
      lock_guard<mutex> lockGuard(instance_mutex);
      b_instance_equal = false;
      ++b_insance_update;
    }
    else
    {
      ++b_insance_update;
    }
  }
}

#endif
// Generate random numbers
unsigned int SingletonRandom::GenerateRandom()
{
  static default_random_engine s_engine(time(0));
  static uniform_int_distribution<unsigned> s_uniform(0, s_max_val_);

  return s_uniform(s_engine);
}
