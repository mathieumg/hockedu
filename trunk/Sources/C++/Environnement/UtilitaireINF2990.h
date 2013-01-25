///////////////////////////////////////////////////////////////////////////
/// @file UtilitaireINF2990.h
/// @author Martin Bisson
/// @date 2007-05-22
/// @version 1.0
///
/// @addtogroup inf2990 INF2990
/// @{
///////////////////////////////////////////////////////////////////////////
#ifndef __ENVIRONNEMENT_UTILITAIREINF2990_H__
#define __ENVIRONNEMENT_UTILITAIREINF2990_H__

#define BOX2D_INTEGRATED 0
#define BOX2D_DEBUG 1
#include <string>

namespace utilitaire {
	
   /// Effectue une conversion entre une chaîne Java et une chaîne C++.
   std::string obtenirChaineISO(void* env, void* chaine);
   
}


#endif // __ENVIRONNEMENT_UTILITAIREINF2990_H__


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
