#ifndef __SINGLETON_DICTION__
#define __SINGLETON_DICTION__

#include<string>

#ifdef UTFLAG
#include<set>
#else
#include<unordered_set>
#endif

class SingletonDiction
{
#ifdef UTFLAG
  public:
    std::set<std::string> uset_dictionary_;
#else
  private:
    std::unordered_set<std::string> uset_dictionary_;
#endif

    SingletonDiction() = default;
    SingletonDiction(const SingletonDiction&) = delete;
    SingletonDiction& operator=(const SingletonDiction&) = delete;

  public:
    ~SingletonDiction() = default;

    static SingletonDiction& GetInstance();

    void Init();
    bool LookupDict(std::string str_letters);
};
#endif
