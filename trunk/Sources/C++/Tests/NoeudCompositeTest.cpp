//////////////////////////////////////////////////////////////////////////////
/// @file NoeudCompositeTest.cpp
/// @author Vincent Lemire
/// @date 2012-02-03
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
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
	arbre = new RazerGameTree();
	enfants[0] = new NoeudComposite();
	enfants[1] = new NoeudComposite();
	enfants[2] = new NoeudComposite();
	enfants[3] = new NoeudComposite();
	enfants[4] = new NoeudComposite();

	arbre->ajouter(enfants[0]);
	arbre->ajouter(enfants[1]);
	enfants[0]->ajouter(enfants[2]);
	enfants[2]->ajouter(enfants[3]);
	enfants[2]->ajouter(enfants[4]);
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
	arbre->vider();
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
	CPPUNIT_ASSERT(arbre->obtenirNombreEnfants() == 2);
	CPPUNIT_ASSERT(enfants[0]->obtenirNombreEnfants() == 1);
	CPPUNIT_ASSERT(enfants[1]->obtenirNombreEnfants() == 0);
	CPPUNIT_ASSERT(enfants[2]->obtenirNombreEnfants() == 2);
	CPPUNIT_ASSERT(enfants[3]->obtenirNombreEnfants() == 0);
	CPPUNIT_ASSERT(enfants[4]->obtenirNombreEnfants() == 0);

	CPPUNIT_ASSERT( arbre->calculerProfondeur() == 4);
	CPPUNIT_ASSERT( enfants[0]->calculerProfondeur() == 3);
	CPPUNIT_ASSERT( enfants[2]->calculerProfondeur() == 2);
	CPPUNIT_ASSERT( enfants[3]->calculerProfondeur() == 1);

	// Test pour s'assurer qu'un noeud ne peut s'ajouter lui-même
	NoeudComposite* n = new NoeudComposite();
	n->ajouter(n);
	CPPUNIT_ASSERT( n->obtenirNombreEnfants() == 0);
	CPPUNIT_ASSERT( n->calculerProfondeur() == 1);

}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudCompositeTest::viderArbreTest()
///
/// S'assure que l'arbre est vider et que les enfants sont invalidés
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::viderArbreTest()
{
	//On vide l'arbre et s'assure qu'il n'a plus d'enfant
	arbre->vider();
	CPPUNIT_ASSERT( arbre->obtenirNombreEnfants() == 0);

	for(int i=0; i<5; i++)
	{
		try
		{
			enfants[i]->obtenirParent();
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
	arbre->vider();
	enfants[0] = new NoeudComposite();
	enfants[1] = new NoeudComposite();

	arbre->ajouter(enfants[0]);

	CPPUNIT_ASSERT( arbre->obtenirNombreEnfants() == 1);
	CPPUNIT_ASSERT( arbre->calculerProfondeur() == 2);

	enfants[1]->ajouter(enfants[0]);
	CPPUNIT_ASSERT( arbre->obtenirNombreEnfants() == 0);
	CPPUNIT_ASSERT( arbre->calculerProfondeur() == 1);
	CPPUNIT_ASSERT( enfants[1]->obtenirNombreEnfants() == 1);
	CPPUNIT_ASSERT( enfants[0]->obtenirParent() == enfants[1]);

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
	arbre->effacer(enfants[1]);
	CPPUNIT_ASSERT(arbre->obtenirNombreEnfants() == 1);
	for(int i=1; i<5; i++)
	{
		try
		{
			enfants[i]->obtenirParent();
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
	arbre->effacer(enfants[1]);
	// Rien ne devrait ce passer puisque enfant[1] n'est pas un enfant de arbre
	CPPUNIT_ASSERT(arbre->obtenirNombreEnfants() == 1);
	try
	{
		enfants[1]->obtenirParent();
		CPPUNIT_ASSERT(true); // L'enfant existe toujours
	}
	catch(...)
	{
		CPPUNIT_ASSERT(false); //L'enfant à bien été invalidé
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
	CPPUNIT_ASSERT(arbre->chercher(0) == enfants[0]);
	CPPUNIT_ASSERT(arbre->chercher(1) == enfants[1]);
	// Test pour la recherche avec un index hors de l'intervalle
	CPPUNIT_ASSERT(arbre->chercher(2) == 0);
	CPPUNIT_ASSERT(enfants[0]->chercher(0) == enfants[2]);
	CPPUNIT_ASSERT(enfants[2]->chercher(0) == enfants[3]);
	CPPUNIT_ASSERT(enfants[2]->chercher(1) == enfants[4]);

	arbre->vider();
	enfants[0] = new NoeudComposite("maillet");
	enfants[1] = new NoeudComposite("maillet");
	enfants[2] = new NoeudComposite("portail");
	arbre->ajouter(enfants[0]);
	arbre->ajouter(enfants[1]);
	enfants[0]->ajouter(enfants[2]);
	CPPUNIT_ASSERT(arbre->chercher("maillet") == enfants[0]);
	CPPUNIT_ASSERT(arbre->chercher("portail") == enfants[2]);

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
	arbre->selectionne_ = false;
	for(int i=0; i<5; i++)
	{
		enfants[i]->selectionne_ = false;
	}
	arbre->selectionnerTout();
	CPPUNIT_ASSERT(arbre->selectionne_ == false); // False car 
	for(int i=0; i<5; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->selectionne_ == true);
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
	arbre->selectionne_ = true;
	for(int i=0; i<5; i++)
	{
		enfants[i]->selectionne_ = true;
	}
	arbre->deselectionnerTout();
	CPPUNIT_ASSERT(arbre->selectionne_ == false);
	for(int i=0; i<5; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->selectionne_ == false);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn NoeudCompositeTest::positionAbsolueTest()
///
/// Test pour les position absolue des enfants
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void NoeudCompositeTest::positionAbsolueTest()
{
	arbre->vider();
	typedef std::vector<NoeudAbstrait*> Noeuds;
	Noeuds vec;

	NoeudComposite* racine = new NoeudComposite();
	arbre->ajouter(racine);
	std::string TABLEAU_NOMS[8] = 
	{
		"but",
		"muret",
		"table",
		"portail",
		"rondelle",
		"maillet",
		"accelerateur",
		"point"
	};
	// Essaie d'instancier tous les noeuds possibles
	for(unsigned int i=0; i< 8; i++)
	{
		NoeudAbstrait* n = arbre->creerNoeud(TABLEAU_NOMS[i]);
		if(n != 0)
		{
			n->assignerPositionRelative(Vecteur3(100.0,100.0));
			racine->ajouter(n);
			vec.push_back(n);
		}
	}
	racine->assignerPositionRelative(Vecteur3(0.0,0.0));

	for(unsigned int i =0; i< vec.size(); i++)
		CPPUNIT_ASSERT(vec[i]->obtenirPositionAbsolue() == Vecteur3(100.0,100.0));

	racine->assignerPositionRelative(Vecteur3(-50.0,25.0));
	for(unsigned int i =0; i< vec.size(); i++)
		CPPUNIT_ASSERT(vec[i]->obtenirPositionAbsolue() == Vecteur3(50.0,125.0));

	racine->assignerPositionRelative(Vecteur3(20.0,-25.0));
	for(unsigned int i =0; i< vec.size(); i++)
		CPPUNIT_ASSERT(vec[i]->obtenirPositionAbsolue() == Vecteur3(120.0,75.0));
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
	Terrain* terrain = new Terrain();

	NoeudAbstrait* n = arbre->creerNoeud(RazerGameUtilities::NOM_MAILLET);
    
	// Assignation d'un terrain a la racine d'un arbre
	arbre->modifierTerrain(terrain);
	CPPUNIT_ASSERT(arbre->GetTerrain() == terrain);
	for (int i = 0; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->GetTerrain() == terrain);
	}
	CPPUNIT_ASSERT(n->GetTerrain() == NULL);
	// Verificaiton du terrain apres un ajout
	arbre->ajouter(n);
	CPPUNIT_ASSERT(n->GetTerrain() == terrain);

	// Verificaiton du terrain apres un detachement d'enfant
	enfants[0]->detacherEnfant(enfants[2]);
	for (int i = 2; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->GetTerrain() == NULL);
	}
	enfants[3]->ajouter(n);

	Terrain* terrain2 = new Terrain();
	enfants[2]->modifierTerrain(terrain2);
	// S'assure que les noeuds qui ne devrait pas etre affecté pointe toujours sur le bon terrain
	for (int i = 0; i < 2 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->GetTerrain() == terrain);
	}
	CPPUNIT_ASSERT(arbre->GetTerrain() == terrain);
	
	for (int i = 2; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->GetTerrain() == terrain2);
	}
	CPPUNIT_ASSERT(n->GetTerrain() == terrain2);

	// Verificaiton du terrain apres un ajout d'un noeud composé
	enfants[0]->ajouter(enfants[2]);
	for (int i = 2; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->GetTerrain() == terrain);
	}
	CPPUNIT_ASSERT(n->GetTerrain() == terrain);

	// Mise a null du terrain de la racine
	arbre->modifierTerrain(NULL);
	CPPUNIT_ASSERT(arbre->GetTerrain() == NULL);
	for (int i = 0; i < 5 ; i++)
	{
		CPPUNIT_ASSERT(enfants[i]->GetTerrain() == NULL);
	}
	CPPUNIT_ASSERT(n->GetTerrain() == NULL);


	// On ne supprime pas n puisqu'il fait partie de l'arbre principal
	delete terrain;
	delete terrain2;

}






///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


