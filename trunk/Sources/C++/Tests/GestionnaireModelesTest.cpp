//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaieModeleTest.h
/// @author Vincent Lemire
/// @date 2012-02-10
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "GestionnaireModelesTest.h"
#include "GestionnaireModeles.h"
#include "Modele3D.h"
#include <stdexcept>
#include "RazerGameTree.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( GestionnaireModelesTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModelesTest::setUp()
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
void GestionnaireModelesTest::setUp()
{
	
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModelesTest::tearDown()
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
void GestionnaireModelesTest::tearDown()
{
	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void GestionnaireModelesTest::testObtenirModele()
///
/// Cas de test: Obtention d'un modele inexistant puis d'un qui existe.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void GestionnaireModelesTest::testObtenirModele()
{
	Modele3D* modeleTest;

	// On tente d'obtenir un modèle inexistant
	modeleTest = GestionnaireModeles::obtenirInstance()->obtenirModele("VincentLemire.sr");
	CPPUNIT_ASSERT(!modeleTest);
	// On tente d'obtenir modèle existant
	modeleTest = GestionnaireModeles::obtenirInstance()->obtenirModele(RazerGameUtilities::NOM_MAILLET);
	//CPPUNIT_ASSERT(!!modeleTest);
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


