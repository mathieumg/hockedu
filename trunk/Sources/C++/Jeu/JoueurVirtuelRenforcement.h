//////////////////////////////////////////////////////////////////////////////
/// @file JoueurVirtuelRenforcement.h
/// @author Mathieu Parent
/// @date 2013-04-1
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "JoueurVirtuel.h"
#include <map>
#include "Renforcement\AILearner.h"


///////////////////////////////////////////////////////////////////////////
/// @class JoueurVirtuelRenforcement
/// @brief Classe qui représente un joueur contrôlé par l'ordinateur selon l'apprentissage par renforcement.
///
/// @author Mathieu Parent
/// @date 2013-04-1
///////////////////////////////////////////////////////////////////////////
class JoueurVirtuelRenforcement : public JoueurVirtuel
{
public:
    friend class AiLearnerTests;
	/// Constructeur par paramètres
	JoueurVirtuelRenforcement(const std::string& pAiLogicFilepath, const std::string& nom = "", unsigned int vitesse = 0, unsigned int probabiliteEchec = 0);


	/// Destructeur virtuel
	virtual ~JoueurVirtuelRenforcement(void);

	/// Creation du noeud XML du joueur
	virtual XmlElement* creerNoeudXML() const;


	/// Permet de generer des informations aleatoire pour un joueur,
	/// utile si on a besoin d'un joueur, mais probleme rencontrer a son chargement
	virtual void genererAleatoirement();

    /// Permet d'obtenir l'action a utiliser pour une certaine situation donnee
    AiRuntimeInfosOutput getActionFor(const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse) const;


	/// Questionne le AIMaillet sur la direction à prendre du maillet
	virtual Vecteur2 obtenirDirectionAI(NoeudMaillet* maillet);

    inline std::string GetAiLogicFilepath() const {return mAiLogicFilepath;}

private:
    bool hasMapEntryFor(const Vecteur3& pPositionAi, const Vecteur3& pVelociteAi, const Vecteur3& pPositionRondelle, const Vecteur3& pVelociteRondelle, const Vecteur3& pPositionJoueurAdverse) const;

    bool chargerAiLogic(const std::string& pAiLogicFilepath);

    std::map<AiRuntimeInfosInput, AiRuntimeInfosOutput> mActionMap;

    std::string mAiLogicFilepath;

protected:
	/// Initialisaiton du joueur à partir d'un element XML
	virtual bool initialiser(const XmlElement* element);
};


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
