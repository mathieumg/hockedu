#pragma once
#include <set>


///////////////////////////////////////////////////////////////////////////
/// @class Observer
/// @brief Classe abstraite repr�sentant un observateur. Doit �tre templat� avec
///        le type concret du sujet. La m�thode updateObserver( const ) doit absolument 
///        �tre surcharg�e
///
/// @author Michael Ferris
/// @date 2013-01-25
///////////////////////////////////////////////////////////////////////////
template<class T>
class Observer{
public:       
    virtual ~Observer();                                         
    virtual void updateObserver( const  T* pSubject) = 0;
    void addSubject(T* pSubject, void* orig=NULL);                
    void removeSubject( T* pSubject, void* orig=NULL);             
    std::set<T*> mSubjects;                      
};


///////////////////////////////////////////////////////////////////////////
/// @class Subject
/// @brief Classe abstraite repr�sentant un sujet. Doit �tre templat� avec
///        le type concret du sujet. Aucune op�ration suppl�mentaire necessaire
///        mise � part la signalisation des observateur lors de l'�v�nement 
///        d�sir�.
///
/// @author Michael Ferris
/// @date 2013-01-25
///////////////////////////////////////////////////////////////////////////
template<class T>
class Subject{
public:                       
    virtual ~Subject();             
    void attach(class Observer<T>* o, void* orig=NULL);     
    void detach(class Observer<T>* o, void* orig=NULL);     
    void signalObservers()const;
    std::set<class Observer<T>*> mObservers;   
};

////////////////////////////////////////////////////////////////////////
///
/// @fn  Observer<T>::~Observer(  )
///
/// Destructeur d'un observateur, ce d�tache des sujets.
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
template<class T>
Observer<T>::~Observer()                                   
{                                         
    for ( auto
        it = mSubjects.begin();           
        it != mSubjects.end();            
    ++it )                                
    {                                     
        (*it)->detach(this, this);             
    }                                     
}                                         

////////////////////////////////////////////////////////////////////////
///
/// @fn void Observer<T>::removeSubject( T* pSubject, void* orig )
///
/// Enl�ve un sujet observ�, ne recevra plus d'�v�nement de celui-ci
///
/// @param[in] T * pSubject : le sujet � enlever
/// @param[in] void * orig : utilisation interne pour �viter une recursion de
///                          suppression, Laisser la valeur pas d�faut !
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template<class T>
void Observer<T>::removeSubject( T* pSubject, void* orig/* = NULL */ )
{                                         
    if(orig != this)
    {
        if(orig == NULL)
            orig = this;
        auto it = mSubjects.find( pSubject );
        if(it != mSubjects.end())       
            mSubjects.erase(it);        
        pSubject->detach(this, orig);
    }
}                                         
////////////////////////////////////////////////////////////////////////
///
/// @fn void Observer<T>::addSubject( T* pSubject, void* orig )
///
/// Ajout d'un sujet observ�
///
/// @param[in] T * pSubject : le sujet � enlever
/// @param[in] void * orig : utilisation interne pour �viter une recursion de
///                          d'ajout, Laisser la valeur pas d�faut !
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template<class T>
void Observer<T>::addSubject( T* pSubject, void* orig/* = NULL */ )   
{                                         
    if(orig != this)
    {
        if(orig == NULL)
            orig = this;
        mSubjects.insert(pSubject);          
        pSubject->attach(this,orig);
    }
}                                         

////////////////////////////////////////////////////////////////////////
///
/// @fn  Subject<T>::~Subject(  )
///
/// Destructeur d'un sujet, ce d�tache des observateur.
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
template<class T>
Subject<T>::~Subject()                              
{                                    
    for ( auto
        it = mObservers.begin();     
        it != mObservers.end();      
    ++it )                           
    {                                
        (*it)->removeSubject((T*)this, this); 
    }                                
}                                    

////////////////////////////////////////////////////////////////////////
///
/// @fn void Subject<T>::attach( Observer<T>* pObserver, void* orig )
///
/// Ajout d'un observateur
///
/// @param[in] Observer<T> * pObserver : observateur � ajouter
/// @param[in] void * orig : utilisation interne pour �viter une recursion de
///                          d'ajout, Laisser la valeur pas d�faut !
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template<class T>
void Subject<T>::attach( Observer<T>* pObserver, void* orig/* = NULL */ )               
{                                    
    if(orig != this)
    {
        if(orig == NULL)
            orig = this;
        mObservers.insert( pObserver );         
        pObserver->addSubject((T*)this, orig);             
    }
}                                    

////////////////////////////////////////////////////////////////////////
///
/// @fn void Subject<T>::detach( Observer<T>* pObserver, void* orig/* = NULL */ )
///
/// Enl�ve un observateur
///
/// @param[in] Observer<T> * pObserver : l'observateur � enlever
/// @param[in] void * orig : utilisation interne pour �viter une recursion de
///                          suppression, Laisser la valeur pas d�faut !
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template<class T>
void Subject<T>::detach( Observer<T>* pObserver, void* orig/* = NULL */ )               
{                                    
    if(orig != this)
    {
        if(orig == NULL)
            orig = this;
        auto it = mObservers.find( pObserver );
        if(it != mObservers.end())       
            mObservers.erase(it);
        pObserver->removeSubject((T*)this,orig);
    }        
}       

////////////////////////////////////////////////////////////////////////
///
/// @fn void Subject<T>::signalObservers(  )
///
/// Signal que l'�v�nement attendu s'est produit
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
template<class T>
void Subject<T>::signalObservers() const           
{                                    
    for ( auto
        it = mObservers.begin();     
        it != mObservers.end();      
    ++it )                           
    {                                
        (*it)->updateObserver( (const T*)this);
    }                                
}                                    