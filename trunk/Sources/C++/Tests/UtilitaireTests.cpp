////////////////////////////////////////////////////////////////////////////////////
/// @file UtilitaireTests.cpp
/// @author Michael Ferris
/// @date 2013-03-05
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "UtilitaireTests.h"
#include "Utilitaire.h"
#include "Runnable.h"
#include "RazerGameUtilities.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( UtilitaireTests );

////////////////////////////////////////////////////////////////////////
///
/// @fn void UtilitaireTests::setUp()
///
/// Effectue l'initialisation pr�alable � l'ex�cution de l'ensemble des
/// cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets doivent �tre construits, il est conseill� de le
/// faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void UtilitaireTests::setUp()
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void UtilitaireTests::tearDown()
///
/// Effectue les op�rations de finalisation n�cessaires suite � l'ex�cution
/// de l'ensemble des cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets ont �t� allou�s � l'initialisation, ils doivent �tre
/// d�sallou�s, et il est conseill� de le faire ici.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void UtilitaireTests::tearDown()
{
}

class BreakerBidon : public RunnableBreaker
{
public: 
    ~BreakerBidon(){RunnableBreaker::signalObservers();}
    virtual void bidon() 
    {
        CPPUNIT_ASSERT(false && "Tentative d'execution du runnable, Breaker non fonctionnel");
    }
};


////////////////////////////////////////////////////////////////////////
///
/// @fn void UtilitaireTests::testRunnableBreaker()
///
/// Cas de test: breaking Runnables
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void UtilitaireTests::testRunnableBreaker()
{
    BreakerBidon* b = new BreakerBidon();
    Runnable* runnable = new Runnable([=](Runnable*)->void{b->bidon();});
    b->attach(runnable);
    RazerGameUtilities::RunOnUpdateThread(runnable,true);
    delete b;
    try
    {
        RazerGameUtilities::ExecuteUpdateRunnables();
    }
    catch (...)
    {
        CPPUNIT_ASSERT(false && "Tentative d'execution du runnable, Breaker non fonctionnel");
    }
}
