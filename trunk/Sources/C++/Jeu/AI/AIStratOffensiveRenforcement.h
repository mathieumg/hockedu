//////////////////////////////////////////////////////////////////////////////
/// @file AIStratOffensiveRenforcement.h
/// @author Samuel Ledoux
/// @date 2012-04-02
/// @version 1.0
///
/// @addtogroup razergame RazerGame
/// @{
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "aistrat.h"

///////////////////////////////////////////////////////////////////////////
/// @class AIStratOffensiveRenforcement
/// @brief Représente la stratégie d'un maillet par renforcement.
///
/// @author Samuel Ledoux
/// @date 2012-04-02
///////////////////////////////////////////////////////////////////////////
class AIStratOffensiveRenforcement : public AIStrat
{
public:
	/// Constructeur
	AIStratOffensiveRenforcement(const AIMaillet&);
	/// Destructeur
	virtual ~AIStratOffensiveRenforcement();
	/// Retourne la direction à prendre pour le maillet selon une stratégie offensive
	virtual Vecteur2 appliquerStrategie( NoeudMaillet* maillet );

	/// Attributs
protected:
	bool tirReussi_;
	Vecteur2 malletTargetPos;
	Vecteur2 mPointVise;
	Vecteur2 mPointImpact;
	int mTimeBeforeImpact;

public:
	virtual typeStrat obtenirType() { return OFFENSIVE; }
	/// Accesseur de malletTargetPos
	Vecteur2 getMalletTargetPos() const { return malletTargetPos; }
	void setMalletTargetPos(Vecteur2 val) { malletTargetPos = val; }
	/// Accesseur de mTimeBeforeImpact
	int getTimeBeforeImpact() const { return mTimeBeforeImpact; }
	void setTimeBeforeImpact(int val) { mTimeBeforeImpact = val; }
	/// Accesseur de mPointImpact
	Vecteur2 getPointImpact() const { return mPointImpact; }
	void setPointImpact(Vecteur2 val) { mPointImpact = val; }
	/// Accesseur de mPointVise
	Vecteur2 getPointVise() const { return mPointVise; }
	void setPointVise(Vecteur2 val) { mPointVise = val; }
};

///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////