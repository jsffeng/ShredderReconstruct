#ifndef __SINGLETON_RANDOM__
#define __SINGLETON_RANDOM__

#include <memory>

#if (defined UTFLAG) && (defined MULTI_THREAD_FLAG)
#include <mutex>
#endif

// A Singleton class to generate random number. Here is the reason for only 1 instance allowed for 
// this class:
// A static variable has been used to store the random number (if not use static variable, the same 
// series of random number will be generated when being called each time). So it doesn't make sense 
// to allow more than 1 instance.
// Furthermore, this random number will be used to select a strip randomly from the shredded text 
// strips. Once the number of shredded width gets defined, the number of shredded text strips cannot
// be changed, so the range of random number need to keep the same in an execution of each instance/process
// of this programme.
class SingletonRandom
{
#ifdef UTFLAG 
  public:
    static int s_instance_numbers_;
#else 
  private:
#endif
    SingletonRandom() = default;
    SingletonRandom(const SingletonRandom&) = delete;
    SingletonRandom& operator=(const SingletonRandom&) = delete;

    // Use unique pointer to ensure resources released by programme
    static std::unique_ptr<SingletonRandom> p_instance_;

  public:
    ~SingletonRandom() = default;

    static unsigned int s_max_val_;

    static SingletonRandom& GetInstance();
    unsigned int GenerateRandom();
};

#if (defined UTFLAG) && (defined MULTI_THREAD_FLAG)
// Test class for multi-threads for SingletonRandom
class TESTSingleton
{
  public:
    static std::mutex instance_mutex;
    static bool b_instance_equal;
    static int b_insance_update;

    static void TESTGetInstance();
};
#endif

#endif
