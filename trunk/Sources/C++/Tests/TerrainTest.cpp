////////////////////////////////////////////////////////////////////////////////////
/// @file ConfigSceneTest.cpp
/// @author Michael Ferris
/// @date 2012-04-06
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////
#include "TerrainTest.h"
#include "Terrain.h"
#include "ArbreNoeudLibre.h"
#include "NoeudGroupe.h"
#include "NoeudTable.h"
#include "RazerGameTree.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( TerrainTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void TerrainTest::setUp()
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
void TerrainTest::setUp()
{
	terrain_ = new Terrain(false);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void TerrainTest::tearDown()
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
void TerrainTest::tearDown()
{
	delete terrain_;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void TerrainTest::testVerifierValiditer()
///
/// Test de la methode qui verifie la validite d'un terrain
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void TerrainTest::testVerifierValiditer()
{
	terrain_->initialiser("terrainTest");
	CPPUNIT_ASSERT(!terrain_->verifierValidite(false));
	terrain_->creerTerrainParDefaut("terrainTest");
	CPPUNIT_ASSERT(terrain_->verifierValidite(false));
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void TerrainTest::testInitialiser()
///
/// Test l'initialisation d'un terrain
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void TerrainTest::testInitialiser()
{
	terrain_->initialiser("terrainTest");
    CPPUNIT_ASSERT(terrain_->mEditionZone->obtenirLimiteExtLargeur() == ZoneEdition::DEFAUT_LIMITE_EXT_LARGEUR);
    CPPUNIT_ASSERT(terrain_->mEditionZone->obtenirLimiteExtLongueur() == ZoneEdition::DEFAUT_LIMITE_EXT_LONGUEUR);
    CPPUNIT_ASSERT(terrain_->mEditionZone->obtenirLimiteIntLargeur() == ZoneEdition::DEFAUT_LIMITE_INT_LARGEUR);
    CPPUNIT_ASSERT(terrain_->mEditionZone->obtenirLimiteIntLongueur() == ZoneEdition::DEFAUT_LIMITE_INT_LONGUEUR);

	CPPUNIT_ASSERT(terrain_->mNewNodeTree);
	CPPUNIT_ASSERT(terrain_->mNewNodeTree->obtenirNombreEnfants() == 0);
	CPPUNIT_ASSERT(terrain_->mLogicTree);
	CPPUNIT_ASSERT(terrain_->mTable);
	CPPUNIT_ASSERT(terrain_->mTable->obtenirNombreEnfants() == 21);
	NoeudGroupe* gAccel = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_ACCELERATEUR);
	CPPUNIT_ASSERT(gAccel);
	CPPUNIT_ASSERT(gAccel->obtenirNombreEnfants() == 0);
	NoeudGroupe* gRondelle = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_RONDELLE);
	CPPUNIT_ASSERT(gRondelle);
	CPPUNIT_ASSERT(gRondelle->obtenirNombreEnfants() == 0);
	NoeudGroupe* gPortail = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_PORTAIL);
	CPPUNIT_ASSERT(gPortail);
	CPPUNIT_ASSERT(gPortail->obtenirNombreEnfants() == 0);
	NoeudGroupe* gMaillet = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
	CPPUNIT_ASSERT(gMaillet);
	CPPUNIT_ASSERT(gMaillet->obtenirNombreEnfants() == 0);
	NoeudGroupe* gMuret = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_MURET);
	CPPUNIT_ASSERT(gMuret);
	CPPUNIT_ASSERT(gMuret->obtenirNombreEnfants() == 0);
	CPPUNIT_ASSERT(terrain_->mFieldName == "terrainTest");

}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


