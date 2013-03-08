#pragma once
#include <set>


///////////////////////////////////////////////////////////////////////////
/// @class Observer
/// @brief Classe abstraite représentant un observateur. Doit être templaté avec
///        le type concret du sujet. La méthode updateObserver( const ) doit absolument 
///        être surchargée
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
/// @brief Classe abstraite représentant un sujet. Doit être templaté avec
///        le type concret du sujet. Aucune opération supplémentaire necessaire
///        mise à part la signalisation des observateur lors de l'événement 
///        désiré.
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
/// Destructeur d'un observateur, ce détache des sujets.
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
/// Enlève un sujet observé, ne recevra plus d'événement de celui-ci
///
/// @param[in] T * pSubject : le sujet à enlever
/// @param[in] void * orig : utilisation interne pour éviter une recursion de
///                          suppression, Laisser la valeur pas défaut !
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
/// Ajout d'un sujet observé
///
/// @param[in] T * pSubject : le sujet à enlever
/// @param[in] void * orig : utilisation interne pour éviter une recursion de
///                          d'ajout, Laisser la valeur pas défaut !
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
/// Destructeur d'un sujet, ce détache des observateur.
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
/// @param[in] Observer<T> * pObserver : observateur à ajouter
/// @param[in] void * orig : utilisation interne pour éviter une recursion de
///                          d'ajout, Laisser la valeur pas défaut !
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
/// Enlève un observateur
///
/// @param[in] Observer<T> * pObserver : l'observateur à enlever
/// @param[in] void * orig : utilisation interne pour éviter une recursion de
///                          suppression, Laisser la valeur pas défaut !
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
/// Signal que l'événement attendu s'est produit
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