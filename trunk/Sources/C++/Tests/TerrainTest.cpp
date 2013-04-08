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
#include "Utilitaire.h"

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
	terrain_ = new Terrain(NULL);
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
    CPPUNIT_ASSERT(terrain_->mEditionZone->obtenirLimiteExtY() == ZoneEdition::DEFAUT_LIMITE_EXT_Y);
    CPPUNIT_ASSERT(terrain_->mEditionZone->obtenirLimiteExtX() == ZoneEdition::DEFAUT_LIMITE_EXT_X);
    CPPUNIT_ASSERT(terrain_->mEditionZone->obtenirLimiteIntY() == ZoneEdition::DEFAUT_LIMITE_INT_Y);
    CPPUNIT_ASSERT(terrain_->mEditionZone->obtenirLimiteIntX() == ZoneEdition::DEFAUT_LIMITE_INT_X);

	CPPUNIT_ASSERT(terrain_->mNewNodeTree);
	CPPUNIT_ASSERT(terrain_->mNewNodeTree->childCount() == 0);
	CPPUNIT_ASSERT(terrain_->mLogicTree);
	CPPUNIT_ASSERT(terrain_->mTable);
	CPPUNIT_ASSERT(terrain_->mTable->childCount() == NoeudTable::expectedChildCount());
	NoeudGroupe* gAccel = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_ACCELERATEUR);
	CPPUNIT_ASSERT(gAccel);
	CPPUNIT_ASSERT(gAccel->childCount() == 0);
	NoeudGroupe* gRondelle = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_RONDELLE);
	CPPUNIT_ASSERT(gRondelle);
	CPPUNIT_ASSERT(gRondelle->childCount() == 0);
	NoeudGroupe* gPortail = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_PORTAIL);
	CPPUNIT_ASSERT(gPortail);
	CPPUNIT_ASSERT(gPortail->childCount() == 0);
	NoeudGroupe* gMaillet = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_MAILLET);
	CPPUNIT_ASSERT(gMaillet);
	CPPUNIT_ASSERT(gMaillet->childCount() == 0);
	NoeudGroupe* gMuret = terrain_->mTable->obtenirGroupe(RazerGameUtilities::NOM_MURET);
	CPPUNIT_ASSERT(gMuret);
	CPPUNIT_ASSERT(gMuret->childCount() == 0);
	CPPUNIT_ASSERT(terrain_->mFieldName == "terrainTest");

}


void TerrainTest::testInsideTable()
{
    terrain_->creerTerrainParDefaut("testTable");
    // on assume ici que la table est rectangulaire pour les tests
    // les tests pour une table de forme différente seront fait
    // avec l'interface

    auto table = terrain_->getTable();
    CPPUNIT_ASSERT(table);
    if(table)
    {
        float dim[2];
        table->calculerHautLongMax(dim);

        for(float i=-2; i<=2; i+=0.5f)
        {
            for(float j=-2; j<=2; j+=0.5f)
            {
                Vecteur2 pos(dim[1]*i,dim[0]*j);
                bool res = table->estSurTable(pos);
                bool expectedRes = (utilitaire::ABSOLUTETABARNAK(i)<1 && utilitaire::ABSOLUTETABARNAK(j)<1);
                checkf(res == expectedRes);
                CPPUNIT_ASSERT(res == expectedRes);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void TerrainTest::testUndoRedo()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void TerrainTest::testUndoRedo()
{
    terrain_->createRandomField("testName");

    auto xml = terrain_->creerNoeudXML();
    Terrain terrainRead(NULL);
    terrainRead.initialiserXml(xml,false);

    CPPUNIT_ASSERT(terrainRead.getLogicTree());
    CPPUNIT_ASSERT(terrainRead.getLogicTree()->equals(terrain_->getLogicTree()));
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


