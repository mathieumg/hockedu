//////////////////////////////////////////////////////////////////////////////
/// @file HUDTexte.cpp
/// @author Gabriel Couvrette, Charles Étienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////

#include "HUDTexte.h"
#include "FacadeModele.h"
#include "Partie.h"
#include "Tournoi.h"
#include <sstream>
#include "Utilitaire.h"
#include "JoueurAbstrait.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDTexte::HUDTexte(std::string* texte, Vecteur4 couleur)
///
/// Constructeur par paramètre.
///
/// @param[in] std::string* texte : le texte de l'élément.
/// @param[in] Vecteur4 couleur : la couleur du texte.
///
/// @return 
///
////////////////////////////////////////////////////////////////////////
HUDTexte::HUDTexte(std::string texte, Vecteur4f& couleur, std::string& typeFont)
	:message_(texte), couleur_(couleur), typeTexte_(TEXTE)
{
	if(!textRenderer_.load("", ("media\\"+typeFont).c_str()))
	{
		utilitaire::afficherErreur("Erreur: incapable d'ouvrir le font " + typeFont);
		exit(-1);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDTexte::HUDTexte(TypeTexte typeJoueur, Vecteur4 couleur)
///
/// Constructeur par paramètre.
///
/// @param[in] TypeTexte typeJoueur : le type du joueur.
/// @param[in] Vecteur4 couleur : la couleur du texte.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
HUDTexte::HUDTexte(TypeTexte typeTexte, Vecteur4f& couleur, std::string& typeFont)
	:couleur_(couleur), typeTexte_(typeTexte)
{
	if(!textRenderer_.load("", ("media\\"+typeFont).c_str()))
	{
		utilitaire::afficherErreur("Erreur: incapable d'ouvrir le font game_over");
		exit(-1);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn HUDTexte::HUDTexte(unsigned int& idPartie, unsigned int& joueur, Vecteur4f& couleur, Vecteur4f& couleurSiGagnantPartie, std::string& typeFont)
///
/// Constructeur par paramètre.
///
/// @param[in] unsigned int& idPartie : l'identifiant de la partie.
/// @param[in] unsigned int& joueur : le joueur.
/// @param[in] Vecteur4f& couleur : la couleur.
/// @param[in] Vecteur4f& couleurSiGagnantPartie : la couleur du message de fin de partie.
/// @param[in] std::string& typeFont : la police de caractère.
///
/// @return
///
////////////////////////////////////////////////////////////////////////
HUDTexte::HUDTexte(unsigned int& idPartie, unsigned int& joueur, Vecteur4f& couleur, Vecteur4f& couleurSiGagnantPartie, std::string& typeFont)
	:typeTexte_(TOURNOI_JOUEUR), indexPartie_(idPartie), indexJoueur_(joueur), couleur_(couleur), couleurSiGagnantPartie_(couleurSiGagnantPartie)
{
	if(!textRenderer_.load("", ("media\\"+typeFont).c_str()))
	{
		utilitaire::afficherErreur("Erreur: incapable d'ouvrir le font game_over");
		exit(-1);
	}
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void HUDTexte::peindreElement()
///
/// Application d'une couleur.
///
/// @return : Aucune.
///
////////////////////////////////////////////////////////////////////////
void HUDTexte::peindreElement()
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(couleur_[0], couleur_[1], couleur_[2], couleur_[3]);
	
	std::stringstream mess;
	switch(typeTexte_)
	{
		case TEXTE:
			mess << message_.c_str();
			break;
		case POINTAGE_JOUEUR_GAUCHE:
			mess << FacadeModele::getInstance()->obtenirPartieCourante()->obtenirReferencePointsJoueurGauche();
			break;
		case POINTAGE_JOUEUR_DROIT:
			mess << FacadeModele::getInstance()->obtenirPartieCourante()->obtenirReferencePointsJoueurDroit();
			break;
		case NOM_JOUEUR_GAUCHE:
			mess << FacadeModele::getInstance()->obtenirPartieCourante()->obtenirNomJoueurGauche();
			break;
		case NOM_JOUEUR_DROIT:
			mess << FacadeModele::getInstance()->obtenirPartieCourante()->obtenirNomJoueurDroit();
			break;
		case NOM_VAINQUEUR:
			mess << FacadeModele::getInstance()->obtenirPartieCourante()->obtenirGagnant()->obtenirNom();
			break;
		case TOURNOI_JOUEUR:
			{
				SPJoueurAbstrait joueur;
				if(indexPartie_ > (Tournoi::nbrParties_-1))
				{
					unsigned int index = 0;
					joueur = FacadeModele::getInstance()->obtenirTournoi()->obtenirPartie(index)->obtenirGagnant();
					glColor4f(couleurSiGagnantPartie_[0], couleurSiGagnantPartie_[1], couleurSiGagnantPartie_[2], couleurSiGagnantPartie_[3]);
				}
				else if(indexJoueur_)
				{
					joueur = FacadeModele::getInstance()->obtenirTournoi()->obtenirPartie(indexPartie_)->obtenirJoueurDroit();
				}
				else
				{
					joueur = FacadeModele::getInstance()->obtenirTournoi()->obtenirPartie(indexPartie_)->obtenirJoueurGauche();
				}
				if(joueur)
				{
					Vainqueurs chemin = FacadeModele::getInstance()->obtenirTournoi()->obtenirCheminDernierVainqueur();
					if(find(chemin.begin(),chemin.end(),Vainqueur(indexPartie_,indexJoueur_ == 1 ? GAGNANT_DROITE : GAGNANT_GAUCHE)) != chemin.end())
					{
						glColor4f(couleurSiGagnantPartie_[0], couleurSiGagnantPartie_[1], couleurSiGagnantPartie_[2], couleurSiGagnantPartie_[3]);
					}
// 
// 					unsigned int indexPartiePrecedente;
// 					if(FacadeModele::getInstance()->obtenirTournoi()->estTermine())
// 						indexPartiePrecedente = 0;
// 					else 
// 						indexPartiePrecedente = FacadeModele::getInstance()->obtenirTournoi()->obtenirIndexPartieCourante() + 1;
// 					if(indexPartiePrecedente < Tournoi::nbrParties_ && joueur == FacadeModele::getInstance()->obtenirTournoi()->obtenirPartie(indexPartiePrecedente)->obtenirGagnant())
// 					{
// 						glColor4f(couleurSiGagnantPartie_[0], couleurSiGagnantPartie_[1], couleurSiGagnantPartie_[2], couleurSiGagnantPartie_[3]);
// 					}
					mess << joueur->obtenirNom() << std::endl;
				}
				break;
			}
		case TEMPS_JEU:
			float tempsJeu = FacadeModele::getInstance()->obtenirPartieCourante()->obtenirTempsJeu();
			int nbHeures = (int)tempsJeu/3600;
			tempsJeu -= nbHeures*3600;
			if(nbHeures > 0)
				mess << nbHeures << ":";
			int nbMinutes = (int)tempsJeu/60;
			if(nbMinutes > 0)
				mess << ((nbHeures>0 && nbMinutes<10)?"0":"") << nbMinutes << ":";
			tempsJeu -= nbMinutes*60;
			tempsJeu = (float)((int)(tempsJeu*100));
			tempsJeu /= 100.0;
			mess << ((tempsJeu<10 && nbMinutes > 0)?"0":"") << tempsJeu;
			break;
	}
	textRenderer_.print(obtenirX(),1-obtenirY(),true,mess.str().c_str());
	glPopAttrib();
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////


