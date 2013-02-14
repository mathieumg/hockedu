///////////////////////////////////////////////////////////////////////////////
/// @file ObjetAnimable.h
/// @author Mathieu Parent
/// @date 2012-03-16
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Vecteur.h"


///////////////////////////////////////////////////////////////////////////
/// @class ObjectAnimationParameters
/// @brief Structure contenant les parametres d'animation des objet animables.
///
/// @author Michael Ferris
/// @date 2013-02-14
///////////////////////////////////////////////////////////////////////////
class ObjectAnimationParameters
{
public:
    Vecteur3 mPosition;
    Vecteur3 mAngle;
    Vecteur3 mScale;

    inline void setCanUpdatedPosition(bool can)
    {
        setFlag(can,ObjectAnimationParameters::POSITION);
    }
    inline void setCanUpdatedAngle(bool can)
    {
        setFlag(can,ObjectAnimationParameters::ANGLE);
    }
    inline void setCanUpdatedScale(bool can)
    {
        setFlag(can,ObjectAnimationParameters::SCALE);
    }
    inline bool CanUpdatedPosition() const
    {
        return isFlagSet(ObjectAnimationParameters::POSITION);
    }
    inline bool CanUpdatedAngle() const
    {
        return isFlagSet(ObjectAnimationParameters::ANGLE);
    }
    inline bool CanUpdatedScale() const
    {
        return isFlagSet(ObjectAnimationParameters::SCALE);
    }

private:
    enum Flag{
        POSITION    = 1<<0,
        ANGLE       = 1<<1,
        SCALE       = 1<<2
    };

    inline void setFlag(bool can, Flag flag)
    {
        // if (f) mUpdateFlag |= POSITION; else mUpdateFlag &= ~POSITION; 
        mUpdateFlag ^= (-((int)can) ^ mUpdateFlag) & flag;
    }
    inline bool isFlagSet(Flag flag) const
    {
        return !!(mUpdateFlag & flag);
    }

    /// Indicate if the attribute needs to be updated(1) or not(0)
    unsigned int mUpdateFlag;
};

///////////////////////////////////////////////////////////////////////////
/// @class ObjetAnimable
/// @brief Reprensente l'interface a implementer pour faire en sorte qu'un objet est animable
///        mferris: Attention le meme objet ne peut etre animer de 2 facon différente, car l'animation overwrite ses infos
///                 refactor needed si besoin d'animer de plusieur facon.
/// @author Mathieu Parent
/// @date 2012-03-16
///////////////////////////////////////////////////////////////////////////
class ObjetAnimable
{
public:
	/// Constructeur par défaut
	ObjetAnimable(void);
	/// Destructeur virtuel
	virtual ~ObjetAnimable(void);
	/// Méhode d'animation
	virtual void appliquerAnimation( const ObjectAnimationParameters& pAnimationResult ) = 0;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////





