////////////////////////////////////////////////////////////////////////////////////
/// @file ConfigSceneTest.cpp
/// @author Michael Ferris
/// @date 2012-04-06
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
////////////////////////////////////////////////////////////////////////////////////
#include "TerrainTest.h"
#include "Terrain.h"
#include "ArbreNoeudLibre.h"
#include "NoeudGroupe.h"
#include "NoeudTable.h"
#include "ArbreRenduINF2990.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( TerrainTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void TerrainTest::setUp()
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
void TerrainTest::setUp()
{
	terrain_ = new Terrain();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void TerrainTest::tearDown()
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
	CPPUNIT_ASSERT(terrain_->zoneEdition_.obtenirLimiteExtLargeur() == ZoneEdition::DEFAUT_LIMITE_EXT_LARGEUR);
	CPPUNIT_ASSERT(terrain_->zoneEdition_.obtenirLimiteExtLongueur() == ZoneEdition::DEFAUT_LIMITE_EXT_LONGUEUR);
	CPPUNIT_ASSERT(terrain_->zoneEdition_.obtenirLimiteIntLargeur() == ZoneEdition::DEFAUT_LIMITE_INT_LARGEUR);
	CPPUNIT_ASSERT(terrain_->zoneEdition_.obtenirLimiteIntLongueur() == ZoneEdition::DEFAUT_LIMITE_INT_LONGUEUR);

	CPPUNIT_ASSERT(terrain_->arbreAjoutNoeud_);
	CPPUNIT_ASSERT(terrain_->arbreAjoutNoeud_->obtenirNombreEnfants() == 0);
	CPPUNIT_ASSERT(terrain_->arbreRendu_);
	CPPUNIT_ASSERT(terrain_->table_);
	CPPUNIT_ASSERT(terrain_->table_->obtenirNombreEnfants() == 13);
	NoeudGroupe* gAccel = terrain_->table_->obtenirGroupe(ArbreRenduINF2990::NOM_ACCELERATEUR);
	CPPUNIT_ASSERT(gAccel);
	CPPUNIT_ASSERT(gAccel->obtenirNombreEnfants() == 0);
	NoeudGroupe* gRondelle = terrain_->table_->obtenirGroupe(ArbreRenduINF2990::NOM_RONDELLE);
	CPPUNIT_ASSERT(gRondelle);
	CPPUNIT_ASSERT(gRondelle->obtenirNombreEnfants() == 0);
	NoeudGroupe* gPortail = terrain_->table_->obtenirGroupe(ArbreRenduINF2990::NOM_PORTAIL);
	CPPUNIT_ASSERT(gPortail);
	CPPUNIT_ASSERT(gPortail->obtenirNombreEnfants() == 0);
	NoeudGroupe* gMaillet = terrain_->table_->obtenirGroupe(ArbreRenduINF2990::NOM_MAILLET);
	CPPUNIT_ASSERT(gMaillet);
	CPPUNIT_ASSERT(gMaillet->obtenirNombreEnfants() == 0);
	NoeudGroupe* gMuret = terrain_->table_->obtenirGroupe(ArbreRenduINF2990::NOM_MURET);
	CPPUNIT_ASSERT(gMuret);
	CPPUNIT_ASSERT(gMuret->obtenirNombreEnfants() == 8);
	CPPUNIT_ASSERT(terrain_->nom_ == "terrainTest");

}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


