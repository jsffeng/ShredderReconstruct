#include<random>
#include<memory>
#include "singleton.h"

using namespace std;

#ifdef UTFLAG
int Singleton_rand::instance_num = 0;
#endif
unsigned int Singleton_rand::max_val = 0;
std::unique_ptr<Singleton_rand> Singleton_rand::instance = nullptr;

Singleton_rand& Singleton_rand::getInstance()
{
  // for multi-threads
  // static once_flag oc_flag;
  
  if (max_val == 0)
  {
    throw runtime_error("Need to set max_val value before using Singleton_rand!");	
  }

  // for multi-threads
  // call_once(oc_flag, [&](){ instance.reset(new Singleton_rand);});

  if (instance == nullptr)
  {
     instance.reset(new Singleton_rand);
#ifdef UTFLAG
     instance_num++;  
#endif 
  }

  return *instance;

}

unsigned int Singleton_rand::rand_generator()
{
  static default_random_engine e(time(0));
  static uniform_int_distribution<unsigned> u(0,max_val);

  return u(e);
}
