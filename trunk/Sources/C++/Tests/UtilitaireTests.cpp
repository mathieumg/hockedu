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
/// Effectue l'initialisation préalable à l'exécution de l'ensemble des
/// cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets doivent être construits, il est conseillé de le
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
/// Effectue les opérations de finalisation nécessaires suite à l'exécution
/// de l'ensemble des cas de tests de cette suite de tests (si nécessaire).
/// 
/// Si certains objets ont été alloués à l'initialisation, ils doivent être
/// désalloués, et il est conseillé de le faire ici.
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
