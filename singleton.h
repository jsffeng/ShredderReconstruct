#ifndef __SINGLETON__
#define __SINGLETON__

#include<memory>

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

    // Use unique pointer to ensure resources release being taken care of
    static std::unique_ptr<SingletonRandom> p_instance_;

  public:
    ~SingletonRandom() = default;

    static unsigned int s_max_val_;

    static SingletonRandom& GetInstance();
    unsigned int GenerateRandom();
};

#endif
