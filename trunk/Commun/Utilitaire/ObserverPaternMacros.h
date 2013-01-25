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

template<class T>
class Observer{
public:       
    virtual ~Observer();                                         
    virtual void updateObserver(T* pSubject) = 0;
    void addSubject(T* pSubject, void* orig=NULL);                
    void removeSubject( T* pSubject, void* orig=NULL);             
    std::set<T*> mSubjects;                      
};

template<class T>
class Subject{
public:                       
    virtual ~Subject();             
    void attach(class Observer<T>* o, void* orig=NULL);     
    void detach(class Observer<T>* o, void* orig=NULL);     
    void signalObservers();
    std::set<class Observer<T>*> mObservers;   
};

template<class T>
Observer<T>::~Observer()                                   
{                                         
    for ( std::set<T*>::iterator          
        it = mSubjects.begin();           
        it != mSubjects.end();            
    ++it )                                
    {                                     
        (*it)->detach(this, this);             
    }                                     
}                                         
template<class T>
void Observer<T>::removeSubject( T* pSubject, void* orig )
{                                         
    if(orig != this)
    {
        if(orig == NULL)
            orig = this;
        std::set<T*>::iterator it = mSubjects.find( pSubject );
        if(it != mSubjects.end())       
            mSubjects.erase(it);        
        pSubject->detach(this, orig);
    }
}                                         
template<class T>
void Observer<T>::addSubject( T* pSubject, void* orig )   
{                                         
    if(orig != this)
    {
        if(orig == NULL)
            orig = this;
        mSubjects.insert(pSubject);          
        pSubject->attach(this,orig);
    }
}                                         

template<class T>
Subject<T>::~Subject()                              
{                                    
    for ( std::set<Observer<T>*>::iterator     
        it = mObservers.begin();     
        it != mObservers.end();      
    ++it )                           
    {                                
        (*it)->removeSubject((T*)this, this); 
    }                                
}                                    

template<class T>
void Subject<T>::attach( Observer<T>* o, void* orig )               
{                                    
    if(orig != this)
    {
        if(orig == NULL)
            orig = this;
        mObservers.insert( o );         
        o->addSubject((T*)this, orig);             
    }
}                                    

template<class T>
void Subject<T>::detach( Observer<T>* o, void* orig )               
{                                    
    if(orig != this)
    {
        if(orig == NULL)
            orig = this;
        std::set<class Observer<T>*>::iterator it = mObservers.find( o );
        if(it != mObservers.end())       
            mObservers.erase(it);
        o->removeSubject((T*)this,orig);
    }        
}       

template<class T>
void Subject<T>::signalObservers()            
{                                    
    for ( std::set<class Observer<T>*>::iterator     
        it = mObservers.begin();     
        it != mObservers.end();      
    ++it )                           
    {                                
        (*it)->updateObserver((T*)this);
    }                                
}                                    