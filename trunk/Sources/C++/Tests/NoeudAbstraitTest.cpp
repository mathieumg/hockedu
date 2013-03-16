////////////////////////////////////////////////////////////////////////////////////
/// @file NoeudAbstraitTest.cpp
/// @author Julien Gascon-Samson
/// @date 2011-07-16
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "NoeudAbstraitTest.h"
#include "Utilitaire.h"
#include "NoeudTable.h"
#include "VisiteurFunction.h"
#include "Terrain.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( NoeudAbstraitTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::setUp()
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
void NoeudAbstraitTest::setUp()
{
	noeud = new NoeudAbstrait("TestingNode");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::tearDown()
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
void NoeudAbstraitTest::tearDown()
{
	delete noeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testPositionRelative()
///
/// Cas de test: écriture/lecture de la position relative
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testPositionRelative()
{
	noeud->setPosition(Vecteur3(2.2f, 3.3f, 4.4f));

	Vecteur3 vecteur = noeud->getPosition();

	CPPUNIT_ASSERT( utilitaire::EGAL_ZERO(vecteur[0] - 2.2f) );
	CPPUNIT_ASSERT( utilitaire::EGAL_ZERO(vecteur[1] - 3.3f) );
	CPPUNIT_ASSERT( utilitaire::EGAL_ZERO(vecteur[2] - 4.4f) );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testType()
///
/// Cas de test: type de noeud
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testType()
{
	CPPUNIT_ASSERT( noeud->getType() == "TestingNode");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testEtat()
///
/// Cas de test: définition/obtention de l'état (booléens) du noeud
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testEtat()
{
	// Régler tous les états (booléens) à faux
	noeud->setVisible(false);
	noeud->setRecordable(false);

	// Vérifier que la valeur attribuée est fausse
	CPPUNIT_ASSERT( noeud->isVisible() == false );
	CPPUNIT_ASSERT( noeud->isRecordable() == false );

	// Régler tous les états (booléens) à true
	noeud->setVisible(true);
	noeud->setRecordable(true);

	// Vérifier que la valeur attribuée est fausse
	CPPUNIT_ASSERT( noeud->isVisible() == true );
	CPPUNIT_ASSERT( noeud->isRecordable() == true );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testSelection()
///
/// Cas de test: définition/obtention des états de sélection du noeud
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testSelection()
{
	// Cas 1 : non sélectionnable, non sélectionné
	noeud->setCanBeSelected(false);
	noeud->setSelection(false);
	CPPUNIT_ASSERT( noeud->canBeSelected() == false );
	CPPUNIT_ASSERT( noeud->IsSelected() == false );

	// Cas 2 : non sélectionnable, sélectionné
	noeud->setCanBeSelected(false);
	noeud->setSelection(true);
	CPPUNIT_ASSERT( noeud->canBeSelected() == false );
	CPPUNIT_ASSERT( noeud->IsSelected() == false );

	// Cas 3 : sélectionnable, non sélectionné
	noeud->setCanBeSelected(true);
	noeud->setSelection(false);
	CPPUNIT_ASSERT( noeud->canBeSelected() == true );
	CPPUNIT_ASSERT( noeud->IsSelected() == false );

	// Cas 4 : non sélectionnable, sélectionné
	noeud->setCanBeSelected(true);
	noeud->setSelection(true);
	CPPUNIT_ASSERT( noeud->canBeSelected() == true );
	CPPUNIT_ASSERT( noeud->IsSelected() == true );
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testEnfants()
///
/// Cas de test: s'assurer que le noeud abstrait n'a pas d'enfant
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testEnfants()
{
	CPPUNIT_ASSERT( noeud->childCount() == 0);
	CPPUNIT_ASSERT( noeud->treeDepth() == 1);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testAjout()
///
/// Cas de test: s'assurer qu'il est impossible d'add un enfant
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testAjout()
{
// 	// Essaie d'add un noeud
// 	NoeudAbstrait* nouveauNoeud = new NoeudConeCube(RazerGameUtilities::NOM_CUBECONE);
// 	bool ajout = noeud->add(nouveauNoeud);
// 
// 	// L'ajout devrait avoir échoué puisqu'il s'agit d'un noeud abstrait...
// 	CPPUNIT_ASSERT( ajout==false );
// 
// 	// Assurons-nous que le noeud ne possède pas d'enfant...
// 	CPPUNIT_ASSERT( noeud->childCount() == 0 );
// 
// 	// Nettoyage
// 	delete nouveauNoeud;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testGetTreeRoot()
///
/// Cas de test: s'assurer de bien retrouver la racine du noeud si elle existe
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudAbstraitTest::testGetTreeRoot()
{
    RazerGameTree tree(NULL);

    NoeudComposite* c1 = new NoeudComposite(""),
        * c2 = new NoeudComposite(""),
        * c3 = new NoeudComposite(""),
        * c4 = new NoeudComposite(""),
        * c5 = new NoeudComposite("");

    c1->add(c2);
    c2->add(c3);
    c3->add(c4);
    c4->add(c5);
    c5->add(noeud);

    const ArbreRendu* root = noeud->GetTreeRoot();

    // arbre pas encore add
    CPPUNIT_ASSERT( root == NULL );

    tree.add(c1);
    root = noeud->GetTreeRoot();
    CPPUNIT_ASSERT( root == &tree );


    // pour ne pas liberer 2 fois la memoire
    c5->unlinkChild(noeud);

    
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudAbstraitTest::testInsideTable()
///
/// Cas de test: s'assurer de bien detecter si un noeud est a lexterieur de la table
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
VISITEUR_FUNC_FUNC_DECLARATION(rebuild)
{
    pNoeud->forceFullUpdate();
}
void NoeudAbstraitTest::testInsideTable()
{
    Terrain terrain(NULL);
    terrain.creerTerrainParDefaut("testTable");
    // on assume ici que la table est rectangulaire pour les tests
    // les tests pour une table de forme différente seront fait
    // avec l'interface

    auto table = terrain.getTable();
    CPPUNIT_ASSERT(table);
    if(table)
    {
        float dim[2];
        table->calculerHautLongMax(dim);
        
        for(float i=-0.5f; i<=0.5f; i+=0.5f)
        {
            for(float j=-0.5f; j<=0.5f; j+=0.5f)
            {
                Vecteur2 pos(dim[1]*i,dim[0]*j);
                CPPUNIT_ASSERT(table->estSurTable(pos));
            }
        }

        for(int i=-2; i<=2; i+=2)
        {
            for(int j=-2; j<=2; j+=2)
            {
                if(i!=0 && j!=0)
                {
                    Vecteur2 pos(dim[1]*i,dim[0]*j);
                    CPPUNIT_ASSERT(!table->estSurTable(pos));
                }
            }
        }
    }

}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
