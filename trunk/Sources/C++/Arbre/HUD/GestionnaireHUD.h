//////////////////////////////////////////////////////////////////////////////
/// @file GestionnaireHUD.h
/// @author Gabriel Couvrette, Charles �tienne Lalonde
/// @date 2012-03-26
/// @version 1.0 
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Singleton.h"
#include "glew.h"

enum RacineHUD{RACINE_JEU,RACINE_EDITION,RACINE_TOURNOI};

class HUDElement;

///////////////////////////////////////////////////////////////////////////
/// @class GestionnaireHUD
/// @brief Classe singleton qui repr�sente un HUD avec tous ses �l�ments.
///
/// @author Gabriel Couvrette, Charles �tienne Lalonde
/// @date 2012-03-09
///////////////////////////////////////////////////////////////////////////
class GestionnaireHUD : public Singleton<GestionnaireHUD>
{
public:
	/// Affichage du HUD selon le mode
	void dessinerHUDJeu();
	void dessinerHUDEdition();
	void dessinerHUDTournoi();

	/// Visibilit�
	void visible(bool& estVisible);


private:
	/// Contructeur
	GestionnaireHUD();

	/// Creation du HUD
	void creerHUDJeu();
	void creerHUDEdition();
	void creerHUDTournoi();
	void effectuerDecompte();
	/// Destructeur
	~GestionnaireHUD(void);

	/// D�clarations pour le singleton
	SINGLETON_DECLARATION_CLASSE_SANS_CONSTRUCTEUR(GestionnaireHUD);
	


	/// La racine de l'arbre des �l�ment du HUD
	HUDElement* racineJeu_,
			  *	racineEdition_,
			  *	racineTournoi_;
	HUDElement* decompte_;
	GLuint listePause_;

    class HUDBonus* mLeftPlayerBonuses;

    float mDeltaXOrtho;




/// Accesseurs
public:
	HUDElement* obtenirRacine(RacineHUD racine);
    /// Accessors of mLeftPlayerBonuses
    inline class HUDBonus* getLeftPlayerBonuses() const { return mLeftPlayerBonuses; }
    /// Sets the ratio  X/Y of the window to keep hud at correct size
    inline void setRatio(float windowRatio)
    {
        mDeltaXOrtho = 0;//(windowRatio-1.f)/2.f;
    }
    /// Accessors of mDeltaXOrtho
    inline float getDeltaXOrtho() const { return mDeltaXOrtho; }
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





