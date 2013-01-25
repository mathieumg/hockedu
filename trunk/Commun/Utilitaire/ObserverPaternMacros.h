#pragma once
#include <set>

#define OBSERVER_DECLARATION(O,S)                         \
public:                                                   \
    virtual ~O();                                         \
    virtual void updateObserver(class S& pSubject) = 0;\
    void addSubject(class S& pSubject);                \
    void removeSubject(class S& pSubject);             \
private:                                                  \
    std::set<class S*> mSubjects;                      \

#define OBSERVER_IMPLEMENTATION(O,S)\
O::~O()                                   \
{                                         \
    for ( std::set<S*>::iterator          \
        it = mSubjects.begin();           \
        it != mSubjects.end();            \
    ++it )                                \
    {                                     \
        (*it)->detach(*this);             \
    }                                     \
}                                         \
void O::removeSubject( class S& pSubject )\
{                                         \
    mSubjects.erase(                      \
        mSubjects.find( &pSubject ) );    \
}                                         \
void O::addSubject( class S& pSubject )   \
{                                         \
    mSubjects.insert(&pSubject);          \
}                                         \

#define SUBJECT_DECLARATION(S,O)  \
    public:                       \
        virtual ~S();             \
        void attach(class O& o);     \
        void detach(class O& o);     \
        void signalObservers();\
    private:                      \
    std::set<class O*> mObservers;   \

#define SUBJECT_IMPLEMENTATION(S,O)  \
S::~S()                              \
{                                    \
    for ( std::set<O*>::iterator     \
        it = mObservers.begin();     \
        it != mObservers.end();      \
    ++it )                           \
    {                                \
        (*it)->removeSubject(*this); \
    }                                \
}                                    \
void S::attach( O& o )               \
{                                    \
    mObservers.insert( &o );         \
    o.addSubject(*this);             \
}                                    \
void S::detach( O& o )               \
{                                    \
    std::set<class O*>::iterator it = mObservers.find( &o );\
    if(it != mObservers.end())       \
        mObservers.erase(it);        \
}                                    \
void S::signalObservers()            \
{                                    \
    for ( std::set<O*>::iterator     \
        it = mObservers.begin();     \
        it != mObservers.end();      \
    ++it )                           \
    {                                \
        (*it)->updateObserver(*this);\
    }                                \
}                                    \



template <class T>
class Observer
{
public:
    Observer() {}
    virtual ~Observer() {}
    virtual void updateObserver(T *subject)= 0;
};


template <class T>
class Subject
{
public:
    typedef std::set<Observer<T> *> Observers;
    Subject() {}
    virtual ~Subject() {}
    void attach (Observer<T> &observer);
    void detach(Observer<T>& observer)
    {
        Observers::iterator it = mObservers.find( &observer );
        if(it != mObservers.end())
        {
            mObservers.erase(it);
        }
    }
    void signalObservers()
    {
        Observers::iterator it;
        for ( it=mObservers.begin(); it!=mObservers.end(); it++ )
        {
            (*it)->updateObserver(static_cast<T *>(this));
        }
    }
private:
    Observers mObservers;
};



////////////////////////////////////////////////////////////////////////
///
/// @fn template <class T> void Subject<T>::attach( Observer<T> &observer )
///
/// /*Description*/
///
/// @param[in] Observer<T> & observer
///
/// @return template <class T>
///
////////////////////////////////////////////////////////////////////////
template <class T>
void Subject<T>::attach( Observer<T> &observer )
{
    mObservers.insert( &observer );
}
