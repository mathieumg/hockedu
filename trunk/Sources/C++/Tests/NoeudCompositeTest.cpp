//////////////////////////////////////////////////////////////////////////////
/// @file NoeudCompositeTest.cpp
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "NoeudCompositeTest.h"
#include "NoeudMaillet.h"
#include "NoeudPortail.h"
#include "Terrain.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( NoeudCompositeTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCompositeTest::setUp()
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
void NoeudCompositeTest::setUp()
{
	arbre = new RazerGameTree(NULL);
	enfants[0] = new NoeudComposite();
	enfants[1] = new NoeudComposite();
	enfants[2] = new NoeudComposite();
	enfants[3] = new NoeudComposite();
	enfants[4] = new NoeudComposite();

	arbre->add(enfants[0]);
	arbre->add(enfants[1]);
	enfants[0]->add(enfants[2]);
	enfants[2]->add(enfants[3]);
	enfants[2]->add(enfants[4]);
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCompositeTest::tearDown()
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
void NoeudCompositeTest::tearDown()
{
	arbre->empty();
	delete arbre;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudCompositeTest::ajoutEnfantTest()
///
/// test d'un noeud composite pour l'ajout d'enfant
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::ajoutEnfantTest()
{
	CPPUNIT_ASSERT(arbre->childCount() == 2);
	CPPUNIT_ASSERT(enfants[0]->childCount() == 1);
	CPPUNIT_ASSERT(enfants[1]->childCount() == 0);
	CPPUNIT_ASSERT(enfants[2]->childCount() == 2);
	CPPUNIT_ASSERT(enfants[3]->childCount() == 0);
	CPPUNIT_ASSERT(enfants[4]->childCount() == 0);

	CPPUNIT_ASSERT( arbre->treeDepth() == 4);
	CPPUNIT_ASSERT( enfants[0]->treeDepth() == 3);
	CPPUNIT_ASSERT( enfants[2]->treeDepth() == 2);
	CPPUNIT_ASSERT( enfants[3]->treeDepth() == 1);

	// Test pour s'assurer qu'un noeud ne peut s'add lui-même
	NoeudComposite* n = new NoeudComposite();
	n->add(n);
	CPPUNIT_ASSERT( n->childCount() == 0);
	CPPUNIT_ASSERT( n->treeDepth() == 1);

}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudCompositeTest::viderArbreTest()
///
/// S'assure que l'arbre est empty et que les enfants sont invalidés
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::viderArbreTest()
{
	//On vide l'arbre et s'assure qu'il n'a plus d'enfant
	arbre->empty();
	CPPUNIT_ASSERT( arbre->childCount() == 0);

	for(int i=0; i<5; i++)
	{
		try
		{
			enfants[i]->getParent();
			CPPUNIT_ASSERT(false); // L'enfant existe toujours
		}
		catch(...)
		{
			CPPUNIT_ASSERT(true); //L'enfant à bien été invalidé
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudCompositeTest::ajoutRecursifTest()
///
/// S'assure que si on ajoute un enfant ayant déjà un parent, le parent perd son pointeur sur cet enfant
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::ajoutRecursifTest()
{
	arbre->empty();
	enfants[0] = new NoeudComposite();
	enfants[1] = new NoeudComposite();

	arbre->add(enfants[0]);

	CPPUNIT_ASSERT( arbre->childCount() == 1);
	CPPUNIT_ASSERT( arbre->treeDepth() == 2);

	enfants[1]->add(enfants[0]);
	CPPUNIT_ASSERT( arbre->childCount() == 0);
	CPPUNIT_ASSERT( arbre->treeDepth() == 1);
	CPPUNIT_ASSERT( enfants[1]->childCount() == 1);
	CPPUNIT_ASSERT( enfants[0]->getParent() == enfants[1]);

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCompositeTest::effacerEnfantsTest()
///
/// Test pour la suprresion d'enfant de l'arbe de rendu
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::effacerEnfantsTest()
{
    if(enfants[1])enfants[1]->deleteThis();
	CPPUNIT_ASSERT(arbre->childCount() == 1);
	for(int i=1; i<5; i++)
	{
		try
		{
			enfants[i]->getParent();
			CPPUNIT_ASSERT(false); // L'enfant existe toujours
		}
		catch(...)
		{
			CPPUNIT_ASSERT(true); //L'enfant à bien été invalidé
		}
	}
	for(int i=1; i<5; i++)
	{
		enfants[i] = new NoeudComposite();
	}
	// Rien ne devrait ce passer puisque enfant[1] n'est pas un enfant de arbre
	CPPUNIT_ASSERT(arbre->childCount() == 1);
	try
	{
		CPPUNIT_ASSERT(enfants[1]->getParent() == NULL); // L'enfant n'a pas de parent
	}
	catch(...)
	{
		CPPUNIT_ASSERT(false); //L'enfant n'existe pas
	}

}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudCompositeTest::chercherEnfantTest()
///
/// Test pour la recherche d'enfant dans l'arbre de rendu
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::chercherEnfantTest()
{
	CPPUNIT_ASSERT(arbre->find(0) == enfants[0]);
	CPPUNIT_ASSERT(arbre->find(1) == enfants[1]);
	// Test pour la recherche avec un index hors de l'intervalle
	CPPUNIT_ASSERT(arbre->find(2) == 0);
	CPPUNIT_ASSERT(enfants[0]->find(0) == enfants[2]);
	CPPUNIT_ASSERT(enfants[2]->find(0) == enfants[3]);
	CPPUNIT_ASSERT(enfants[2]->find(1) == enfants[4]);

	arbre->empty();
	enfants[0] = new NoeudComposite("maillet");
	enfants[1] = new NoeudComposite("maillet");
	enfants[2] = new NoeudComposite("portail");
	arbre->add(enfants[0]);
	arbre->add(enfants[1]);
	enfants[0]->add(enfants[2]);
	CPPUNIT_ASSERT(arbre->find("maillet") == enfants[0]);
	CPPUNIT_ASSERT(arbre->find("portail") == enfants[2]);

}


////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCompositeTest::selectionTousTest()
///
/// Test pour la selection d'un arbre de rendu
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::selectionTousTest()
{
	arbre->setSelection(false);
	for(int i=0; i<5; i++)
	{
		enfants[i]->setSelection(false);
	}
	arbre->selectAll();
	CPPUNIT_ASSERT(!arbre->IsSelected()); // False car 
	for(int i=0; i<5; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->IsSelected());
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCompositeTest::deselectionTousTest()
///
/// Test pour la déselection d'un arbre de rendu
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::deselectionTousTest()
{
    arbre->setSelection(true);
    for(int i=0; i<5; i++)
    {
        enfants[i]->setSelection(true);
    }
    arbre->deselectAll();
    CPPUNIT_ASSERT(!arbre->IsSelected()); // False car 
    for(int i=0; i<5; i++)
    {
        CPPUNIT_ASSERT(!enfants[i]->IsSelected());
    }
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void NoeudCompositeTest::modificationTerrain()
///
/// /*Description*/
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::modificationTerrain()
{
	Terrain* terrain = new Terrain(NULL);

	NoeudAbstrait* n = arbre->creerNoeud(RazerGameUtilities::NOM_MAILLET);
    
	// Assignation d'un terrain a la racine d'un arbre
	arbre->setField(terrain);
	CPPUNIT_ASSERT(arbre->getField() == terrain);
	for (int i = 0; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->getField() == terrain);
	}
	CPPUNIT_ASSERT(n->getField() == NULL);
	// Verificaiton du terrain apres un ajout
	arbre->add(n);
	CPPUNIT_ASSERT(n->getField() == terrain);

	// Verificaiton du terrain apres un detachement d'enfant
	enfants[0]->unlinkChild(enfants[2]);
	for (int i = 2; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->getField() == NULL);
	}
	enfants[3]->add(n);

	Terrain* terrain2 = new Terrain(NULL);
	enfants[2]->setField(terrain2);
	// S'assure que les noeuds qui ne devrait pas etre affecté pointe toujours sur le bon terrain
	for (int i = 0; i < 2 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->getField() == terrain);
	}
	CPPUNIT_ASSERT(arbre->getField() == terrain);
	
	for (int i = 2; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->getField() == terrain2);
	}
	CPPUNIT_ASSERT(n->getField() == terrain2);

	// Verificaiton du terrain apres un ajout d'un noeud composé
	enfants[0]->add(enfants[2]);
	for (int i = 2; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->getField() == terrain);
	}
	CPPUNIT_ASSERT(n->getField() == terrain);

	// Mise a null du terrain de la racine
	arbre->setField(NULL);
	CPPUNIT_ASSERT(arbre->getField() == NULL);
	for (int i = 0; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->getField() == NULL);
	}
	CPPUNIT_ASSERT(n->getField() == NULL);


	// On ne supprime pas n puisqu'il fait partie de l'arbre principal
	delete terrain;
	delete terrain2;

}






///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


