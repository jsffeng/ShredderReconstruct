#ifndef __SINGLETON__
#define __SINGLETON__

#include<memory>

class Singleton_rand
{
#ifdef UTFLAG 
  public:
    static int instance_num;
#else 
  private:
#endif
    Singleton_rand() = default;
    Singleton_rand(const Singleton_rand&) = delete;
    Singleton_rand& operator=(const Singleton_rand&) = delete;

    // Use unique pointerto ensure to release all resources  
    static std::unique_ptr<Singleton_rand> instance;

  public:
    ~Singleton_rand() = default;

    static unsigned int max_val;

    static Singleton_rand& getInstance();
    unsigned int rand_generator();
};

#endif
