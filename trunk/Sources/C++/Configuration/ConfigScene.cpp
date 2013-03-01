////////////////////////////////////////////////////////////////////////////////////
/// @file ConfigScene.cpp
/// @author Jean-François Pérusse
/// @date 2007-01-10
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
////////////////////////////////////////////////////////////////////////////////////

#include "ConfigScene.h"
#include "VisiteurEcrireXML.h"
#include "NoeudTable.h"
#include "NoeudBut.h"
#include "Utilitaire.h"
#include "JoueurHumain.h"
#include "JoueurVirtuel.h"
#include "NoeudPoint.h"
#include "CodesEvenements.h"
#include <algorithm>
#include "SoundFMOD.h"
#include "Tournoi.h"
#include "Terrain.h"
#include "ZoneEdition.h"
#include "GestionnaireAnimations.h"
#include "XMLUtils.h"
#include "RazerGameTree.h"
#include "LumiereAmbiante.h"
#include "LumiereDirectionnelle.h"
#include "LumiereSpot.h"

SINGLETON_DECLARATION_CPP(ConfigScene);

const std::string ConfigScene::FICHIER_CONFIGURATION = "Configuration.xml";
const std::string ConfigScene::FICHIER_MUSIQUE = "listeCanaux.xml";
const char ConfigScene::ETIQUETTE_ARBRE[] = {"Arbre"};


////////////////////////////////////////////////////////////////////////
///
/// @fn  ConfigScene::ConfigScene()
///
/// constructeur de la configuration de la scène
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ConfigScene::ConfigScene(): toucheHaut_(VJAK_W), toucheBas_(VJAK_S), toucheGauche_(VJAK_A), toucheDroite_(VJAK_D)
{

	// Lumiere 0 qui est la lumiere ambiante
	GLfloat lumA_ambiante[4]  = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat lumA_diffuse[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat lumA_speculaire[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat lumA_position[4] = { 0.0f, 0.0f, 300.0f, 1.0f };
	GLfloat lumA_attenuation[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Presque pas d<attenuation pour voir dans toute la piece
	LumiereAbstraite* lumiereAmbiant = new LumiereAmbiante(lumA_position,lumA_ambiante,lumA_diffuse,lumA_speculaire,GL_LIGHT0, lumA_attenuation);
	conteneurLumiere_.push_back(lumiereAmbiant);
	lumiereAmbiant->initLumiere();
	lumiereAmbiant->allumerLumiere();

	// Spot sur le ScoreBoard, light1
	Vecteur3f pos(-946.6f,-11.57f,805.46f), visee(-1192.41f,-11.57f,604.98f);
	Vecteur3f direction(visee-pos);
	direction.normaliser();
	GLfloat lumB_ambiante[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lumB_diffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lumB_speculaire[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lumB_position[4] = {pos[0], pos[1], pos[2], 1.0f};
	GLfloat lumB_attenuation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat spotDirection2_[3]= { direction[0],direction[1], direction[2]};
	GLfloat spotExponent2_= {1.0f};
	GLfloat spotCutoff2_= {35.0f};
	LumiereAbstraite* lumiereSpot = new LumiereSpot(lumB_position,lumB_ambiante,lumB_diffuse,lumB_speculaire,GL_LIGHT1,spotDirection2_,spotExponent2_,spotCutoff2_, lumB_attenuation);
	conteneurLumiere_.push_back(lumiereSpot);
	lumiereSpot->initLumiere();
	lumiereSpot->allumerLumiere();
	
	// Lumiere 1 qui est la lumiere directionnelle
	GLfloat lumD_ambiante[4]  = { 0.75f, 0.59f, 0.19f, 1.0f };
	GLfloat lumD_diffuse[4] = {0.75f, 0.59f, 0.19f, 1.0f};
	GLfloat lumD_speculaire[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat lumD_direction[4] = {1.0f, -1.0f, 0.5f, 0.0f};
	GLfloat lumD_attenuation[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	LumiereAbstraite* lumiereDirectionnelle = new LumiereDirectionnelle(lumD_direction,lumD_ambiante,lumD_diffuse,lumD_speculaire,GL_LIGHT2, lumD_attenuation);
	conteneurLumiere_.push_back(lumiereDirectionnelle);
	lumiereDirectionnelle->initLumiere();
	lumiereDirectionnelle->allumerLumiere();
	

	// Lumiere 2 qui est le spot sur le centre de la table
	GLfloat lumP_ambiante[4]  = { 1.0f, 1.0, 1.0, 1.0f };
	GLfloat lumP_diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat lumP_speculaire[4] = { 1.0f, 0.0, 0.0, 1.0f };
	GLfloat lumP_position[4] = {-11.83f, -43.93f, 797.43f, 1.0f};
	GLfloat lumP_attenuation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat spotDirection_[3]= { 0.0f, 0.0f, -1.0f};
	GLfloat spotExponent_= {1.0f};
	GLfloat spotCutoff_= {30.0f};
	lumiereSpot = new LumiereSpot(lumP_position,lumP_ambiante,lumP_diffuse,lumP_speculaire,GL_LIGHT3,spotDirection_,spotExponent_,spotCutoff_, lumP_attenuation);
	conteneurLumiere_.push_back(lumiereSpot);
	lumiereSpot->initLumiere(); // Rechanger apres
	lumiereSpot->allumerLumiere();

	// Lumiere 3 qui est le spot sur la photo de nous
	Vecteur3f pos2(150.61f,-667.17f,957.83f), visee2(150.61f,-929.83f,526.79f);
	Vecteur3f direction2(visee2-pos2);
	direction2.normaliser();
	GLfloat lumPhoto_ambiante[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lumPhoto_diffuse[4] =  {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lumPhoto_speculaire[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lumPhoto_position[4] = {pos2[0], pos2[1], pos2[2], 1.0f};
	GLfloat lumPhoto_attenuation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat spotDirectionPhoto_[3]= { direction2[0],direction2[1], direction2[2]};
	GLfloat spotExponentPhoto_= {1.0f};
	GLfloat spotCutoffPhoto_= {35.0f};
	lumiereSpot = new LumiereSpot(lumPhoto_position,lumPhoto_ambiante,lumPhoto_diffuse,lumPhoto_speculaire,GL_LIGHT4,spotDirectionPhoto_,spotExponentPhoto_,spotCutoffPhoto_, lumPhoto_attenuation);
	conteneurLumiere_.push_back(lumiereSpot);
	lumiereSpot->initLumiere(); // Rechanger apres
	lumiereSpot->allumerLumiere();

	// Lumiere 4 qui est le spot sur la photo de la chix
	Vecteur3f pos3(-800.61f,-667.17f,957.83f), visee3(-800.61f,-929.83f,526.79f);
	Vecteur3f direction3(visee3-pos3);
	direction3.normaliser();
	GLfloat lumChix_ambiante[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lumChix_diffuse[4] =  {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lumChix_speculaire[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lumChix_position[4] = {pos3[0], pos3[1], pos3[2], 1.0f};
	GLfloat lumChix_attenuation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat spotDirectionChix_[3]= { direction3[0],direction3[1], direction3[2]};
	GLfloat spotExponentChix_= {0.0f};
	GLfloat spotCutoffChix_= {35.0f};
	lumiereSpot = new LumiereSpot(lumChix_position,lumChix_ambiante,lumChix_diffuse,lumChix_speculaire,GL_LIGHT5,spotDirectionChix_,spotExponentChix_,spotCutoffChix_, lumChix_attenuation);
	conteneurLumiere_.push_back(lumiereSpot);
	lumiereSpot->initLumiere(); // Rechanger apres
	lumiereSpot->allumerLumiere();

	// Lumiere 5 lumiere du coneCube
	Vecteur3f pos4(-1054.77f,740.32f,0), visee4(-1054.77f,740.32f,50);
	Vecteur3f direction4(visee4-pos4);
	direction4.normaliser();
	GLfloat lumCone_ambiante[4]  = { 0.0f, 0.0f, 228.0f/255.0f, 1.0f };
	GLfloat lumCone_diffuse[4] =  {0.0f, 1.0f, 228.0f/255.0f, 1.0f};
	GLfloat lumCone_speculaire[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lumCone_position[4] = {pos4[0], pos4[1], pos4[2], 1.0f};
	GLfloat lumCone_attenuation[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat spotDirectionCone_[3]= { direction4[0],direction4[1], direction4[2]};
	GLfloat spotExponentCone_= {1.0f};
	GLfloat spotCutoffCone_= {45.0f};
	lumiereSpot = new LumiereSpot(lumCone_position,lumCone_ambiante,lumCone_diffuse,lumCone_speculaire,GL_LIGHT6,spotDirectionCone_,spotExponentCone_,spotCutoffCone_, lumCone_attenuation);
	conteneurLumiere_.push_back(lumiereSpot);
	lumiereSpot->initLumiere(); // Rechanger apres
	lumiereSpot->allumerLumiere();


	// Lumiere 6 lumiere dans un coin forever alone
	Vecteur3f pos5(-1164.01f,-38.96f,-47.46f), visee5(1508.46f,-43.96f,68.54f);
	Vecteur3f direction5(visee5-pos5);
	direction5.normaliser();
	GLfloat lumAlone_ambiante[4]  = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat lumAlone_diffuse[4] =  { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat lumAlone_speculaire[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lumAlone_position[4] = {pos5[0], pos5[1], pos5[2], 1.0f};
	GLfloat lumAlone_attenuation[4] = {1.0f, 0.2f, 0.0f, 1.0f};
	GLfloat spotDirectionAlone_[3]= { direction5[0],direction5[1], direction5[2]};
	GLfloat spotExponentAlone_= {1.0f};
	GLfloat spotCutoffAlone_= {30.0f};
	lumiereSpot = new LumiereAmbiante(
		lumAlone_position,
		lumAlone_ambiante,
		lumAlone_diffuse,
		lumAlone_speculaire,
		GL_LIGHT7,
// 		spotDirectionAlone_,
// 		spotExponentAlone_,
// 		spotCutoffAlone_, 
		lumAlone_attenuation);
	conteneurLumiere_.push_back(lumiereSpot);
	//lumiereSpot->initLumiere(); // Rechanger apres
	//lumiereSpot->allumerLumiere();
	lumiereSpot->eteindreLumiere();

	chargerConfiguration();
	// Chargement des musiques et des canaux dans FMOD
	chargerCanaux();
}

////////////////////////////////////////////////////////////////////////
///
/// @fn  ConfigScene::~ConfigScene()
///
/// destructeur de la scène
///
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
ConfigScene::~ConfigScene()
{
	for(int i=0;i<conteneurLumiere_.size();++i)
	{
		delete conteneurLumiere_[i];
	}
}	

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::creerDOM ( XmlNode& node ) const
///
/// Cette fonction écrit les valeurs de la configuration dans un élément XML.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::enregistrerConfiguration () const
{
	XmlDocument* document = XMLUtils::CreateDocument("1.0", "", "");

	// On enregistre les différentes configurations.
	// Créer le noeud 'configuration'
	XmlElement* elementConfiguration = XMLUtils::createNode("configuration");

	// Créer le noeud scene et définir ses attributs
	XmlElement* elementScene = XMLUtils::createNode("CScene");
    XMLUtils::writeAttribute(elementScene,"toucheHaut", toucheHaut_);
    XMLUtils::writeAttribute(elementScene,"toucheBas",	   toucheBas_);
    XMLUtils::writeAttribute(elementScene,"toucheGauche", toucheGauche_);
    XMLUtils::writeAttribute(elementScene,"toucheDroite", toucheDroite_);
	
	// Enregistrement du volumes de la musique et des effets
    XMLUtils::writeAttribute(elementScene,"volSong", SoundFMOD::obtenirInstance()->getAppSongVolume());
    XMLUtils::writeAttribute(elementScene,"volEffect", SoundFMOD::obtenirInstance()->getEffectVolume());

	// Adjoindre le noeud 'elementScene'
    XMLUtils::LinkEndChild(elementConfiguration,elementScene);

	// Adjoindre le noeud 'configuration' au noeud principal
	// (Rappel : pas besoin de libérer la mémoire de elementConfiguration
	// puisque toutes les fonctions Link... le font pour nous)
    XMLUtils::LinkEndChild((XmlElement*)document,elementConfiguration);

	// Écrire dans le fichier
    XMLUtils::SaveDocument(document,FICHIER_CONFIGURATION.c_str());
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::lireDOM( const XmlNode& node )
///
/// Cette fonction lit les valeurs de la configuration à partir d'un élément
/// XML.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::chargerConfiguration( )
{
	// Vérification de l'existance du ficher

	// Si le fichier n'existe pas, on le crée.
	if ( !utilitaire::fichierExiste(FICHIER_CONFIGURATION) ) {
		enregistrerConfiguration();
	}
	// si le fichier existe on le lit
	else {
		XmlDocument* document = XMLUtils::LoadDocument(FICHIER_CONFIGURATION.c_str());

        if(document)
        {
            // Tenter d'obtenir le noeud 'Configuration'
            const XmlElement* elementConfiguration = XMLUtils::FirstChildElement((XmlElement*)document,"configuration");
            if (elementConfiguration) {
                // Tenter d'obtenir l'élément CScene, puis l'attribut CALCULS_PAR_IMAGE
                const XmlElement* elementScene =  XMLUtils::FirstChildElement(elementConfiguration,"CScene");
                if (elementScene) 
                {
                    if(!XMLUtils::readAttribute(elementScene,"toucheHaut",   toucheHaut_  ))toucheHaut_   = VJAK_W;
                    if(!XMLUtils::readAttribute(elementScene,"toucheBas",	toucheBas_    ))toucheBas_    = VJAK_S;
                    if(!XMLUtils::readAttribute(elementScene,"toucheGauche", toucheGauche_))toucheGauche_ = VJAK_A;
                    if(!XMLUtils::readAttribute(elementScene,"toucheDroite", toucheDroite_))toucheDroite_ = VJAK_D;
                    float vol;                             
                    if(XMLUtils::readAttribute(elementScene,"volSong", vol)  )SoundFMOD::obtenirInstance()->setAppVolume(vol);
                    if(XMLUtils::readAttribute(elementScene,"volEffect", vol))SoundFMOD::obtenirInstance()->setEffectVolume(vol);
                }
            }
            XMLUtils::FreeDocument(document);
        }
	}

	
}


////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::creerDOM( XmlNode& node, ConteneurJoueur* joueurs ) const
///
/// Cette fonction écrit les valeurs de la configuration dans un élément XML.
///
/// @param[in] XmlNode & node : racine pour la création, généralement le document lui-meme
/// @param[in] ConteneurJoueur * joueurs : conteneur de joueur à enregistrer
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::creerDOM( XmlNode& node, const ConteneurJoueur& joueurs ) const
{
	// Créer le noeud 
	XmlElement* racine = XMLUtils::createNode("ProfileJoueurs");
	node.LinkEndChild(racine);

	// Parcour du conteneur et création des noeud XML de chacun des joueurs
	ConteneurJoueur::const_iterator i = joueurs.begin();
	for (; i != joueurs.end() ; i++)
	{
		if(i->second != 0)
			racine->LinkEndChild(i->second->creerNoeudXML());
	}

}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::creerDOM( XmlNode& node, const Tournoi& tournoi ) const
///
/// Creation du DOM pour un tournoi pour l'enregistrement XML
///
/// @param[in] XmlNode & node
/// @param[in] const Tournoi & tournoi
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::creerDOM( XmlNode& node, const Tournoi& tournoi ) const
{
	// Créer le noeud 
	XmlElement* racine = XMLUtils::createNode("Tournoi");
	node.LinkEndChild(racine);

	// Parcour du conteneur et création des noeud XML de chacun des joueurs
	racine->LinkEndChild(tournoi.creerTournoiXML());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::lireDOM( const XmlNode& node, RazerGameTree* arbre )
///
/// Cette fonction lit les valeurs de la configuration à partir d'un élément
/// XML.
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::lireDOM( const XmlNode& node, RazerGameTree* arbre )
{
	arbre->vider();
	arbre_ = arbre;
	// Tenter d'obtenir le noeud 'Arbre'
	const XmlNode* elementConfiguration = node.FirstChild("Arbre"), *child;
	if (elementConfiguration != NULL)
	{
		for( child = elementConfiguration->FirstChild(); child/*Vérifie si child est non-null*/; child = child->NextSibling() )
		{
			ecrireArbre(arbre_,child);
		}
	}
	else
		throw std::runtime_error("Etiquette de l'arbre manquant");
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::lireDOM( const XmlNode& node, ConteneurJoueur& Joueurs )
///
/// /*Description*/
///
/// @param[in] const XmlNode & node
/// @param[in] ConteneurJoueur & Joueurs
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::lireDOM( const XmlNode& node, ConteneurJoueur& Joueurs )
{
	const XmlNode* elementConfiguration = node.FirstChild("ProfileJoueurs");
	const XmlElement* child;
	if (elementConfiguration != NULL)
	{
		for( child = elementConfiguration->FirstChildElement(); child/*Vérifie si child est non-null*/; child = child->NextSiblingElement() )
		{
			SPJoueurAbstrait joueur = JoueurAbstrait::usineJoueurXML(child);
			// Si 2 joueurs ont le meme nom, le dernier seulement sera conserve
			if(joueur)
			{
				Joueurs[joueur->obtenirNom()] = joueur;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn ConfigScene::ecrireArbre()
///
/// Méthode pour faire l'arbre de rendu à partir d'un noeud XML
///
/// @param[in] NoeudAbstrait * parentNoeud
/// @param[in] XmlNode * node
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::ecrireArbre(NoeudAbstrait* parentNoeud, const XmlNode* node)
{
	XmlElement* elem = (XmlElement*)node;
	TiXmlString nom = elem->ValueTStr();
	NoeudAbstrait* noeudCourant;

	// Si le noeud est un point, on doit retrouver ce noeud à partir de la table et non en instancier un nouveau
	if(nom.c_str() == RazerGameUtilities::NAME_TABLE_CONTROL_POINT)
	{
		int typeNoeud;
		if( unsigned short result = elem->QueryIntAttribute("typePosNoeud", &typeNoeud) != TIXML_SUCCESS )
		{
			throw std::runtime_error("Erreur de lecture d'attribut");
		}
		NoeudTable* table = dynamic_cast<NoeudTable*>(parentNoeud);
		if(table == 0)
		{
			throw std::runtime_error("Parent du point n'est pas une table");
		}
		noeudCourant = table->obtenirPoint(typeNoeud);
	}
	// Si le noeud est un but, on doit retrouver ce noeud à partir de la table et non en instancier un nouveau
	else if(nom.c_str() == RazerGameUtilities::NOM_BUT)
	{
		NoeudPoint* pointMilieu = dynamic_cast<NoeudPoint*>(parentNoeud);
		if(pointMilieu == 0)
		{
			throw std::runtime_error("Parent du but n'est pas un point");
		}
		noeudCourant = pointMilieu->chercher(0);
	}
	// Sinon on instancie une nouvelle instance du noeud
	else
	{
		noeudCourant = arbre_->creerNoeud(nom.c_str());
		if(noeudCourant == 0)
		{
			throw std::runtime_error("Type de noeud inexistant");
		}
		if(!parentNoeud->ajouter(noeudCourant))
        {
            throw std::runtime_error("Incapable d'ajouter le noeud au parent");
        }
	}
	if(!noeudCourant->initialiser(elem))
	{
		throw std::runtime_error("Erreur de lecture d'attribut");
	}
	if(!elem->NoChildren())
	{
		const XmlNode *child;
		for( child = node->FirstChild(); child; child = child->NextSibling() )
		{
			ecrireArbre(noeudCourant,child);
		}
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::creerFichierMusique()
///
/// Créer la base du fichier XML pour la sauvegarde des canaux de musique
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::creerFichierMusique()
{
	TiXmlDocument document;
	XmlElement* racine = XMLUtils::createNode("listeCanaux");

	// Écrire la déclaration XML standard...
	TiXmlDeclaration* declaration = new TiXmlDeclaration( "1.0", "", "" );
	document.LinkEndChild(declaration);

	document.LinkEndChild(racine);
	document.SaveFile(FICHIER_MUSIQUE.c_str());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::ajouterCanal(std::string nomCanal)
///
/// Ajoute un canal au fichier XML des musiques
///
/// @param[in] std::string nomCanal : le nom du canal à ajouter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::ajouterCanal(std::string nomCanal)
{
	// Vérification de l'existence du fichier
	if (!utilitaire::fichierExiste(FICHIER_MUSIQUE)) 
	{
		creerFichierMusique();
	}

	// Chargement du fichier
	TiXmlDocument document;
	document.LoadFile(FICHIER_MUSIQUE.c_str());

	// Obtention de la racine
	XmlElement* racine = document.FirstChildElement("listeCanaux");

	// Si elle existe
	if(racine != NULL)
	{
		// Ajout du canal
		XmlElement* nouveauCanal = XMLUtils::createNode("canal");
		nouveauCanal->SetAttribute("nom", nomCanal.c_str());
		racine->LinkEndChild(nouveauCanal);
		document.SaveFile(FICHIER_MUSIQUE.c_str());

		SoundFMOD::obtenirInstance()->creerPlaylist(nomCanal);

	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::supprimerCanal(std::string nomCanal)
///
/// Supprime un canal du fichier XML des musiques
///
/// @param[in] std::string nomCanal : Le nom du canal à supprimer.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::supprimerCanal(std::string nomCanal)
{
	// Vérification de l'existence du fichier
	if ( !utilitaire::fichierExiste(FICHIER_MUSIQUE) ) 
	{
		creerFichierMusique();
	}
	else
	{
		// Chargement du fichier
		TiXmlDocument document;
		document.LoadFile(FICHIER_MUSIQUE.c_str());

		// Obtention de la racine
		XmlElement* racine = document.FirstChildElement("listeCanaux");

		// Si la racine existe
		if(racine != NULL)
		{
			// Premier canal
			XmlElement* canal = racine->FirstChildElement("canal");

			// Tant qu'on n'a pas trouver le canal (ou qu'on ait atteint la fin)
			while(canal != NULL && canal->FirstAttribute()->Value() != nomCanal)
			{
				canal = canal->NextSiblingElement("canal");
			}

			// Si on l'a trouvé, on l'efface
			if(canal != NULL)
			{
				racine->RemoveChild(canal);
				SoundFMOD::obtenirInstance()->supprimerPlaylist(nomCanal);
			}
		}

		// Sauvegarde du fichier
		document.SaveFile(FICHIER_MUSIQUE.c_str());
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::ajouterChanson(std::string nomCanal, std::string nomChanson)
///
/// Ajoute une chanson au fichier XML des musiques
///
/// @param[in] std::string nomCanal : Le nom du canal où doit aller la chanson.
/// @param[in] std::string nomChanson : Le nom de la chanson à ajouter.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::ajouterChanson(std::string nomCanal, std::string nomChanson)
{
	// Vérification de l'existence du fichier
	if ( !utilitaire::fichierExiste(FICHIER_MUSIQUE) ) 
	{
		creerFichierMusique();
	}

	// Chargement du fichier
	TiXmlDocument document;
	document.LoadFile(FICHIER_MUSIQUE.c_str());

	// Racine du document
	XmlElement* racine = document.FirstChildElement("listeCanaux");

	// Si la racine existe
	if(racine != NULL)
	{
		// Premier canal 
		XmlElement* canal = racine->FirstChildElement("canal");

		// Tant qu'on a pas trouvé le bon canal ou qu'on est à la in du document
		while(canal != NULL && canal->FirstAttribute()->Value() != nomCanal)
		{
			canal = canal->NextSiblingElement("canal");
		}

		if(canal != NULL)
		{
			// Ajout de la chanson
			XmlElement* nouvelleChanson = XMLUtils::createNode("chanson");
			nouvelleChanson->SetAttribute("Nom", nomChanson.c_str());
			canal->LinkEndChild(nouvelleChanson);
			SoundFMOD::obtenirInstance()->create_sound(nomChanson, FMOD_HARDWARE, 0, nomCanal);
		}
	}

	// Sauvegarde du document
	document.SaveFile(FICHIER_MUSIQUE.c_str());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::supprimerChanson(std::string nomCanal, std::string nomChanson)
///
/// Supprime une chanson du fichier XML des musiques
///
/// @param[in] std::string nomCanal : Le nom du canal auquel appartient la chanson.
/// @param[in] std::string nomChanson : Le nom de la chanson à supprimer.
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::supprimerChanson(std::string nomCanal, std::string nomChanson)
{
	// Vérification de l'existence du fichier
	if ( !utilitaire::fichierExiste(FICHIER_MUSIQUE) ) 
	{
		creerFichierMusique();
	}

	// Chargement du fichier
	TiXmlDocument document;
	document.LoadFile(FICHIER_MUSIQUE.c_str());

	// Racine du document
	XmlElement* racine = document.FirstChildElement("listeCanaux");

	// Si la racine existe
	if(racine != NULL)
	{
		// Premier canal
		XmlElement* canal = racine->FirstChildElement("canal");

		// Tant qu'on ne trouve pas le canal ou qu'on n'a ps atteint la fin du fichier
		while(canal != NULL && canal->FirstAttribute()->Value() != nomCanal)
		{
			canal = canal->NextSiblingElement("canal");
		}

		// Si on a trouvé le canal
		if(canal != NULL)
		{
			// Premier chanson du canal
			XmlElement* chanson = canal->FirstChildElement("chanson");

			// Tant qu'on ne trouve pas la chanson ou qu'on n'a ps atteint la fin du fichier
			while(chanson != NULL && chanson->FirstAttribute()->Value() != nomChanson)
			{
				chanson = chanson->NextSiblingElement("chanson");
			}

			// Si on a trouvé la chanson
			if(chanson != NULL)
			{
				canal->RemoveChild(chanson);
				SoundFMOD::obtenirInstance()->supprimerChanson(nomCanal,nomChanson);
			}
		}
	}

	// Sauvegarde du fichier
	document.SaveFile(FICHIER_MUSIQUE.c_str());
}

////////////////////////////////////////////////////////////////////////
///
/// @fn std::vector<std::string> ConfigScene::obtenirListeCanaux()
///
/// Lit la liste des canaux depuis le XML.
///
/// @return Le vecteur des noms des canaux.
///
////////////////////////////////////////////////////////////////////////
std::vector<std::string> ConfigScene::obtenirListeCanaux()
{
	std::vector<std::string> valeurDeRetour;

	// Vérification de l'existence du fichier
	if ( !utilitaire::fichierExiste(FICHIER_MUSIQUE) ) 
	{
		creerFichierMusique();
	}
	else
	{
		// Chargement du fichier
		TiXmlDocument document;
		document.LoadFile(FICHIER_MUSIQUE.c_str());

		// Racine du document
		XmlElement* racine = document.FirstChildElement("listeCanaux");

		// Si la racine existe
		if(racine != NULL)
		{
			// Premier canal
			XmlElement* canal = racine->FirstChildElement("canal");

			// Parcours des canaux
			while(canal != NULL)
			{
				valeurDeRetour.push_back(canal->FirstAttribute()->Value());

				// Prochain canal
				canal = canal->NextSiblingElement("canal");
			}
		}
	}

	return valeurDeRetour;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn Playlist ConfigScene::obtenirCanal(std::string nom)
///
/// Lit la liste des chansons d'un canal depuis le fichier XML.
///
/// @param[in] std::string nom : Le nom du canal dont on veut les chansons.
///
/// @return La liste des chansons.
///
////////////////////////////////////////////////////////////////////////
NomsPlaylists ConfigScene::obtenirCanal(std::string nom)
{
	NomsPlaylists valeurDeRetour;

	// Vérification de l'existence du fichier
	if ( !utilitaire::fichierExiste(FICHIER_MUSIQUE) ) 
	{
		creerFichierMusique();
	}
	else
	{
		// Chargement du fichier
		TiXmlDocument document;
		document.LoadFile(FICHIER_MUSIQUE.c_str());

		// Racine du document
		XmlElement* racine = document.FirstChildElement("listeCanaux");

		// Si la racine existe
		if(racine != NULL)
		{
			// Premier canal
			XmlElement* canal = racine->FirstChildElement("canal");

			// Tant qu'on ne trouve pas le canal ou qu'on n'a ps atteint la fin du fichier
			while(canal != NULL && canal->FirstAttribute()->Value() != nom)
			{
				canal = canal->NextSiblingElement("canal");
			}

			// Si on a trouvé le canal
			if(canal != NULL)
			{
				// Premier chanson du canal
				XmlElement* chanson = canal->FirstChildElement("chanson");

				// Tant qu'on ne trouve pas la chanson ou qu'on n'a ps atteint la fin du fichier
				while(chanson != NULL)
				{
					valeurDeRetour.push_back(chanson->FirstAttribute()->Value());

					// Prochain canal
					chanson = chanson->NextSiblingElement("chanson");
				}
			}
		}
	}

	return valeurDeRetour;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::chargerCanaux()
///
/// Lit le XML des chansons et initialise les playlists de SoundFMOD.
///
///
/// @return Aucune.
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::chargerCanaux()
{
	// Vérification de l'existence du fichier
	if ( !utilitaire::fichierExiste(FICHIER_MUSIQUE) ) 
	{
		creerFichierMusique();
	}
	else
	{
		// Chargement du fichier
		TiXmlDocument document;
		document.LoadFile(FICHIER_MUSIQUE.c_str());

		// Racine du document
		XmlElement* racine = document.FirstChildElement("listeCanaux");

		// Si la racine existe
		if(racine != NULL)
		{
			// Premier canal
			XmlElement* canal = racine->FirstChildElement("canal");

			// Tant qu'on n'a ps atteint la fin de la liste des canaux
			while(canal != NULL)
			{
				// Première chanson du canal
				XmlElement* chanson = canal->FirstChildElement("chanson");

				// Tant qu'on n'a ps atteint la fin de la liste des chansons
				while(chanson != NULL)
				{
					// Enregistrement de la chanson dans SoundFMOD
					SoundFMOD::obtenirInstance()->create_sound(chanson->FirstAttribute()->Value(), FMOD_HARDWARE, 0, canal->FirstAttribute()->Value());

					// Prochain canal
					chanson = chanson->NextSiblingElement("chanson");
				}
				canal = canal->NextSiblingElement("canal");
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::obtenirLumiere( int indice )
///
/// /*Description*/
///
/// @param[in] int indice
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
LumiereAbstraite* ConfigScene::obtenirLumiere( int indice ) const
{
	return conteneurLumiere_[indice];
}
////////////////////////////////////////////////////////////////////////
///
/// @fn void ConfigScene::rafraichirLumiere()
///
/// Rafraichi la lumière
///
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void ConfigScene::rafraichirLumiere()
{
	for (int i=0 ; i<conteneurLumiere_.size() ; ++i)
	{
		conteneurLumiere_[i]->initLumiere();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////