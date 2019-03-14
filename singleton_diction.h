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
    std::set<std::string> set_dictionary_;

    SingletonDiction() = default;
    SingletonDiction(const SingletonDiction&) = delete;
    SingletonDiction& operator=(const SingletonDiction&) = delete;

  public:
    ~SingletonDiction() = default;

    // Hungry Singleton
    static SingletonDiction& GetInstance();
    
    // Read dictionary from disk to memory
    void Init();

    // Build word piece with minimal charactor size 2, this will be used as the base for lookup.
    void BuildWordPiece();
   
    // Lookup the str_letters to see if exists in dictionary 
    bool LookupDict(std::string str_letters);
};
#endif
