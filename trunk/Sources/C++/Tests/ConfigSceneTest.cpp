////////////////////////////////////////////////////////////////////////////////////
/// @file ConfigSceneTest.cpp
/// @author Michael Ferris
/// @date 2012-02-02
/// @version 2.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "ConfigSceneTest.h"
#include "PlayerComputer.h"
#include "PlayerHuman.h"
#include "Utilitaire.h"
#include <algorithm>
#include "RazerGameTree.h"
#include "VisiteurEcrireXML.h"
#include "Terrain.h"
#include "NoeudMaillet.h"
#include "XMLUtils.h"

// Enregistrement de la suite de tests au sein du registre
CPPUNIT_TEST_SUITE_REGISTRATION( ConfigSceneTest );

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::setUp()
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
void ConfigSceneTest::setUp()
{
	// On initialise un arbre arbitraire pour les test
	arbre = new RazerGameTree(NULL,999,999);
	noeuds_[0] = arbre->creerNoeud(RAZER_KEY_WALL);
	noeuds_[1] = arbre->creerNoeud(RAZER_KEY_PORTAL); 
	noeuds_[2] = arbre->creerNoeud(RAZER_KEY_PUCK); 
	noeuds_[3] = arbre->creerNoeud(RAZER_KEY_MALLET);
	if(noeuds_[0] != 0)
	{
		if(noeuds_[1] != 0)
		{
			noeuds_[0]->add(noeuds_[1]);
			if(noeuds_[3] != 0)
			{
				noeuds_[1]->add(noeuds_[3]); //ne s'ajoutera pas car un portail est un noeudAbstrait
			}
		}
		if(noeuds_[2] != 0)
		{
			noeuds_[0]->add(noeuds_[2]);
		}
		arbre->add(noeuds_[0]);
	}

#ifdef WIN32
	CreateDirectoryA("tests_xml",NULL);
#else
	// creation d'un dossier avec un autre API, dunno which yet
#endif

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::tearDown()
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
void ConfigSceneTest::tearDown()
{
	arbre->empty();
	delete arbre;

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testEcritureArbreRenduXML()
///
/// Cas de test: �criture dans un fichier XML.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testEcritureArbreRenduXML() 
{
	arbre->empty();

    NoeudAbstrait* n1 = arbre->creerNoeud( RAZER_KEY_MALLET );
    NoeudAbstrait* n2 = arbre->creerNoeud( RAZER_KEY_MALLET );
    NoeudAbstrait* n3 = arbre->creerNoeud( RAZER_KEY_PORTAL );
    NoeudAbstrait* n4 = arbre->creerNoeud( RAZER_KEY_PORTAL );
    NoeudAbstrait* n5 = arbre->creerNoeud( RAZER_KEY_PUCK );
    NoeudAbstrait* n6 = arbre->creerNoeud( RAZER_KEY_PUCK );
    NoeudAbstrait* n7 = arbre->creerNoeud( RAZER_KEY_BOOST );
    NoeudComposite* nC1 = new NoeudComposite( RAZER_KEY_GROUP );
    NoeudComposite* nC2 = new NoeudComposite( RAZER_KEY_GROUP );
    NoeudComposite* nC3 = new NoeudComposite( RAZER_KEY_GROUP );
    NoeudComposite* nC4 = new NoeudComposite( RAZER_KEY_GROUP );

	arbre->add(n1);
	arbre->add(n2);
	arbre->add(nC1);
    {
        nC1->add(n3);
        nC1->add(n4);
    }
    arbre->add(nC2);
    {
        nC2->add(nC4);
        {
            nC4->add(n5);
        }
        nC2->add(n6);
        nC2->add(nC3);
        {
            nC3->add(n7);
        }
    }

	nC4->setRecordable(false);

	// �criture initiale du document
	XmlDocument document ;
    XMLUtils::CreateDocument(document);
    VisiteurEcrireXML v;
    arbre->acceptVisitor(v);

    XmlElement* elem = v.obtenirRacine();
    XMLUtils::LinkEndChild(document,elem);

    XMLUtils::SaveDocument(document,"tests_xml\\TestEnregistrement.xml" );
	
	// Lecture manuelle du document et v�rification des noeuds lus (et de la non-existence de noeuds qui ne sont pas supposer exister
    const XmlNode* elementConfiguration = XMLUtils::FirstChild( document.GetElem(), ConfigScene::ETIQUETTE_ARBRE );
    const XmlElement *child, *grantChild, *grantGrantChild;//, *grantGrantGrantChild;
	CPPUNIT_ASSERT (elementConfiguration != NULL);
    int nodeKey;
    {
        // n1
        child = elementConfiguration->FirstChildElement();
        CPPUNIT_ASSERT (child != NULL);
        XMLUtils::readAttribute( child, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
        CPPUNIT_ASSERT(nodeKey == RAZER_KEY_MALLET);

        // n2
        child = child->NextSiblingElement();
        CPPUNIT_ASSERT (child != NULL);
        XMLUtils::readAttribute( child, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
        CPPUNIT_ASSERT( nodeKey == RAZER_KEY_MALLET );

        // nC1
        child = child->NextSiblingElement( );
        CPPUNIT_ASSERT (child != NULL);
        XMLUtils::readAttribute( child, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
        CPPUNIT_ASSERT(nodeKey == RAZER_KEY_GROUP);
        {
            // n3
            grantChild = child->FirstChildElement( );
            CPPUNIT_ASSERT (grantChild != NULL);
            XMLUtils::readAttribute( grantChild, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
            CPPUNIT_ASSERT(nodeKey == RAZER_KEY_PORTAL);

            // n4
            grantChild = grantChild->NextSiblingElement();
            CPPUNIT_ASSERT (grantChild != NULL);
            XMLUtils::readAttribute( grantChild, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
            CPPUNIT_ASSERT( nodeKey == RAZER_KEY_PORTAL );

            // n'existe pas
            grantChild = grantChild->NextSiblingElement( );
            CPPUNIT_ASSERT (grantChild == NULL);
        }
        // nC2
        child = child->NextSiblingElement( );
        CPPUNIT_ASSERT (child != NULL);
        XMLUtils::readAttribute( child, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
        CPPUNIT_ASSERT( nodeKey == RAZER_KEY_GROUP );
        {
            {
                // n5, car nC4 n'est pas enregistrable
                grantChild = child->FirstChildElement( );
                CPPUNIT_ASSERT (grantChild != NULL);
                XMLUtils::readAttribute( grantChild, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
                CPPUNIT_ASSERT(nodeKey == RAZER_KEY_PUCK);
            }

            // n6
            grantChild = grantChild->NextSiblingElement( );
            CPPUNIT_ASSERT (grantChild != NULL);
            XMLUtils::readAttribute( grantChild, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
            CPPUNIT_ASSERT( nodeKey == RAZER_KEY_PUCK );

            // nC3
            grantChild = grantChild->NextSiblingElement( );
            CPPUNIT_ASSERT (grantChild != NULL);
            XMLUtils::readAttribute( grantChild, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
            CPPUNIT_ASSERT( nodeKey == RAZER_KEY_GROUP );
            {
                // n7
                grantGrantChild = grantChild->FirstChildElement( );
                CPPUNIT_ASSERT (grantGrantChild != NULL);
                XMLUtils::readAttribute( grantGrantChild, NoeudAbstrait::ETIQUETTE_KEY, nodeKey );
                CPPUNIT_ASSERT( nodeKey == RAZER_KEY_BOOST );

                // n'existe pas
                grantGrantChild = grantGrantChild->NextSiblingElement( );
                CPPUNIT_ASSERT (grantGrantChild == NULL);
            }

            // n'existe pas
            grantChild = grantChild->NextSiblingElement( );
            CPPUNIT_ASSERT (grantChild == NULL);
        }
        child = child->NextSiblingElement( );
        CPPUNIT_ASSERT (child == NULL);
    }
	// Parcours de l'arbre complet

    remove("tests_xml\\TestEnregistrement.xml");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testLectureArbreRenduXML()
///
/// Cas de test: Lecture dans un fichier XML.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testLectureArbreRenduXML() 
{
    Terrain terrainWrite(NULL),terrainRead(NULL);
    terrainWrite.createRandomField("test");
    XmlElement* racine = XMLUtils::createNode("racine");
    XmlElement* elem = terrainWrite.creerNoeudXML();
    XMLUtils::LinkEndChild(racine,elem);
    try
    {
        terrainRead.initialiserXml(racine);
    }
    catch (ExceptionJeu&)
    {
    	CPPUNIT_ASSERT(false);
    }

    CPPUNIT_ASSERT(terrainRead.getLogicTree()->equals(terrainWrite.getLogicTree()));
    auto m = terrainWrite.getLeftMallet();
    CPPUNIT_ASSERT(!!m);
    if(m)
    {
        m->getParent()->empty();
    }
    CPPUNIT_ASSERT(!terrainRead.getLogicTree()->equals(terrainWrite.getLogicTree()));


    delete racine;
}



////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testLectureArbreRenduXMLCorrompu()
///
/// Cas de test: On tente de lire un fichier XML de format invalide
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testLectureArbreRenduXMLCorrompu()
{
	// Chargement du fichier
	TiXmlDocument document;
	document.LoadFile( "tests_xml\\xmlCorrompu.xml" );
	
	// On tente de le lire
	try
	{
		ConfigScene::obtenirInstance()->lireDOM(document, arbre);
		CPPUNIT_ASSERT(false);
	}
	catch(...)
	{
		// La m�thode doit lancer une exception
		CPPUNIT_ASSERT(true);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testLectureArbreRenduXMLInvalide()
///
/// Cas de test: On tente de lire un fichier XML poss�dant un attribut non-reconnu
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testLectureArbreRenduXMLInvalide()
{
	// Chargement du document
	TiXmlDocument document;
	document.LoadFile( "tests_xml\\xmlInvalideAttributInexistant.xml" );
	
	// On tente de le lire
	try
	{
		ConfigScene::obtenirInstance()->lireDOM(document, arbre);
		CPPUNIT_ASSERT(false);
	}
	catch (ExceptionJeu&)	
	{
		// La m�thode doit lancer une exception
		CPPUNIT_ASSERT(true);
	}

	// On r�p�te avec un autre document
	document.LoadFile( "tests_xml\\xmlInvalideNoeudInexistant.xml" );
	try
	{
		ConfigScene::obtenirInstance()->lireDOM(document, arbre);
		CPPUNIT_ASSERT(false);
	}
	catch (ExceptionJeu&)	
	{
		CPPUNIT_ASSERT(true);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigSceneTest::testLectureEcritureJoueurXML()
///
/// Test de lecture d'un fichier XML
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigSceneTest::testLectureEcritureJoueurXML()
{
	ConteneurJoueur conteneurJoueur;
	SPPlayerAbstract joueurs[15];
	joueurs[0] =  SPPlayerAbstract(new PlayerHuman("Micha�l"		  ));
	joueurs[1] =  SPPlayerAbstract(new PlayerHuman("Samuel"		  ));
	joueurs[2] =  SPPlayerAbstract(new PlayerHuman("Vincent"		  ));
	joueurs[3] =  SPPlayerAbstract(new PlayerHuman("Mathieu"		  ));
	joueurs[4] =  SPPlayerAbstract(new PlayerHuman("Gabriel"		  ));
	joueurs[5] =  SPPlayerAbstract(new PlayerHuman("Charles Etienne" ));
	joueurs[6] =  SPPlayerAbstract(new PlayerComputer("bot1",10,50)	  );
	joueurs[7] =  SPPlayerAbstract(new PlayerComputer("bot2",15,25)	  );
	joueurs[8] =  SPPlayerAbstract(new PlayerComputer("bot3",25,0)	  );
	joueurs[9] =  SPPlayerAbstract(new PlayerComputer("bot4",10,-10)   );
	joueurs[10] = SPPlayerAbstract(new PlayerComputer("bot5",12,25)	  );
	joueurs[11] = SPPlayerAbstract(new PlayerComputer("bot6",48,75)	  );
	joueurs[12] = SPPlayerAbstract(new PlayerComputer("bot8",95,60)	  );
	joueurs[13] = SPPlayerAbstract(new PlayerComputer("bot9",47,55)	  );
	joueurs[14].reset();

	for (int i = 0; i < 14 ; i++)
	{
		conteneurJoueur[joueurs[i]->obtenirNom()] = joueurs[i];
	}
	conteneurJoueur["bidons"] = joueurs[14];

	// �criture initiale du document
	TiXmlDocument document;
	ConfigScene::obtenirInstance()->creerDOM(document, conteneurJoueur);
	document.SaveFile( "tests_xml\\TestEnregistrementJoueur.xml" );
	
	// Chargement du document
	ConteneurJoueur conteneurJoueur2;
	ConfigScene::obtenirInstance()->lireDOM(document, conteneurJoueur2);
	CPPUNIT_ASSERT(conteneurJoueur2.find("Micha�l") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Samuel") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Vincent") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Mathieu") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Gabriel") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("Charles Etienne") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot1") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot2") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot3") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot4") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot5") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot6") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot8") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("bot9") != conteneurJoueur2.end());
	CPPUNIT_ASSERT(conteneurJoueur2.find("random") == conteneurJoueur2.end());

    remove("tests_xml\\TestEnregistrementJoueur.xml");
	//for_each(conteneurJoueur.begin(),conteneurJoueur.end(),utilitaire::LibererMappe());
	//for_each(conteneurJoueur2.begin(),conteneurJoueur2.end(),utilitaire::LibererMappe());

}




///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////


