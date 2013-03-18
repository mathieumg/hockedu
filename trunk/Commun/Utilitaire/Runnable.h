#ifndef RUNNABLE_H
#define RUNNABLE_H

#include <functional>
#include "RunnableBreaker.h"

///////////////////////////////////////////////////////////////////////////
/// @class Runnable
/// @brief Objet conservant du code a executer plus tard ou sur un autre thread
///         d'execution. S'assurer que les variables dans le code a executer
///         ait un scope de vie plus grand que le runnable. Sinon, cet objet
///         doit heriter de RunnableBreaker, attacher le Runnable et signaler les
///         Runnable dans son destructeur.
///
/// @author Michael Ferris
/// @date 2013-02-15
///////////////////////////////////////////////////////////////////////////
class Runnable : public Observer<RunnableBreaker>
{
public:
    Runnable(std::function<void (Runnable*)> function, bool keepAlive = false):
        mFunction(function), mbKeepAlive(keepAlive), mbIsValid(true)
    {

    }
    inline void Run()
    {
        if(mbIsValid)
        {
            mFunction(this);
        }
        else
        {
            // make sure we destroy the runnable
            setKeepAlive(false);
        }
    }

    /// Accessors of mbKeepAlive
    inline bool KeepAlive() const { return mbKeepAlive; }
    inline void setKeepAlive(bool val) { mbKeepAlive = val; }

    /// Callback when an object breaks the runnable
    virtual void updateObserver( const RunnableBreaker* pSubject ) 
    {
        mbIsValid = false;
    }

private:
    std::function<void (Runnable*)> mFunction;
    bool mbKeepAlive;
    bool mbIsValid;
};

#endif