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
/// Effectue l'initialisation pr�alable � l'ex�cution de l'ensemble des
/// cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets doivent �tre construits, il est conseill� de le
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
/// Effectue les op�rations de finalisation n�cessaires suite � l'ex�cution
/// de l'ensemble des cas de tests de cette suite de tests (si n�cessaire).
/// 
/// Si certains objets ont �t� allou�s � l'initialisation, ils doivent �tre
/// d�sallou�s, et il est conseill� de le faire ici.
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

	// On tente d'obtenir un mod�le inexistant
	modeleTest = GestionnaireModeles::obtenirInstance()->obtenirModele("VincentLemire.sr");
	CPPUNIT_ASSERT(!modeleTest);
	// On tente d'obtenir mod�le existant
	modeleTest = GestionnaireModeles::obtenirInstance()->obtenirModele(RazerGameUtilities::NOM_MAILLET);
	//CPPUNIT_ASSERT(!!modeleTest);
}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


