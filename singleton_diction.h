#ifndef __SINGLETON_DICTION__
#define __SINGLETON_DICTION__

#include <string>
#include <set>

class SingletonDiction
{
#ifdef UTFLAG
  public:
#else
  private:
#endif
    std::set<std::string> uset_dictionary_;

    SingletonDiction() = default;
    SingletonDiction(const SingletonDiction&) = delete;
    SingletonDiction& operator=(const SingletonDiction&) = delete;

  public:
    ~SingletonDiction() = default;

    static SingletonDiction& GetInstance();

    void Init();
    void BuildWordPiece();
    bool LookupDict(std::string str_letters);
};
#endif
