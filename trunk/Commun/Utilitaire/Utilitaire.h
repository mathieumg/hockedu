//////////////////////////////////////////////////////////////////////////////
/// @file Utilitaire.h
/// @author Martin Bisson
/// @date 2007-03-11
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
//////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_UTILITAIRE_H__
#define __UTILITAIRE_UTILITAIRE_H__

#include "Vecteur.h"
#include <sstream>

// #define CheckTime(content) \
// {\
// clock_t t = clock();\
// content;\
// t = clock() - t;\
// {int ms = t, s = ms/1000, m = s/60;\
//     printf("%d:%d:%d\n", m, s%60, ms%1000);}}\
//
// // Permet de faire un breakpoint avec du code
// //#define appDebugBreak()     ( *((int*)3) = 13 )
// void appDebugBreak();
//
//
// // permet de connaitre la longueur d'un array
// #define ARRAY_COUNT( array ) \
//     ( sizeof(array) / sizeof((array)[0]) )
//
// // Méthode utilitaire
// typedef char *  va_list;
// int GetVarArgs( char* Dest, int DestSize, int Count, const char*& Fmt, va_list ArgPtr );
// #define GET_VARARGS(msg,msgsize,len,lastarg,fmt) { va_list ap; va_start(ap,lastarg);GetVarArgs(msg,msgsize,len,fmt,ap); }
//
// // Affiche un message d'assesrtion
// enum AssertHandleMode {
//     ASSERT_Break,
//     ASSERT_Ignore,
//     ASSERT_IgnoreAll,
// };
// AssertHandleMode __cdecl DisplayAssertMessage( const char *message );
//
// /** Failed assertion handler.  Warning: May be called at library startup time. */
// void __cdecl appFailAssertFunc( const char* Expr, const char* File, int Line, const char* Format="", ... );
//
// // Utilitaire
// #if __INTEL_COMPILER
// #define appFailAssert(expr,file,line,...)				{ appFailAssertFunc(expr, file, line, __VA_ARGS__); }
// #else
// #define appFailAssert(expr,file,line,...)				{ appFailAssertFunc(expr, file, line, ##__VA_ARGS__); }
// #endif
//
// // Fonction pour faire la verification
// #if !SHIPPING
// #define checkf(expr, ...)   { if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__, ##__VA_ARGS__ ); }
// #else
// #define checkf(expr, ...)
// #endif

namespace utilitaire {
	/// Définition d'un type d'entier non signé.
	typedef unsigned int uint;

	/// Constante pour la différence entre deux nombres réels.
	static const float EPSILON( 0.00000001f );
	/// Constante PI.
	static const double PI( 3.1415926535897932384626433832795 );

	/// Le vecteur i (dirigé vers l'axe des X positifs et unitaire).
	static Vecteur3 vi = Vecteur3( 1.0, 0.0, 0.0 );
	/// Le vecteur j (dirigé vers l'axe des Y positifs et unitaire).
	static Vecteur3 vj = Vecteur3( 0.0, 1.0, 0.0 );
	/// Le vecteur k (dirigé vers l'axe des Z positifs et unitaire).
	static Vecteur3 vk = Vecteur3( 0.0, 0.0, 1.0 );
    static const float ratioWorldToBox2D = 0.1f;

	/// Fonction globale pour l'affichage d'erreur.
	void afficherErreur(const std::string& message);

	/// Pour convertir les angles de radians en degrés.
	float RAD_TO_DEG( float AngleRad );
	/// Pour convertir les angles de degrés en radians.
	float DEG_TO_RAD( float AngleDeg );
#if BOX2D_INTEGRATED
    /// Pour convertir un vecteur de jeu en un vecteur pour Box2D
    void VEC3_TO_B2VEC( const Vecteur3& pVector, struct b2Vec2& pB2vector);
    void B2VEC_TO_VEC3( Vecteur3& pVector, const struct b2Vec2& pB2vector);
#endif

	/// Pour convertir les km/h en m/s.
	float KMH_TO_MS( float kmh );
	/// Pour convertir les m/s en km/h.
	float MS_TO_KMH( float ms );

	/// Fonction pour connaître le signe d'un nombre.
	float SIGN( float nombre );

	/// Fonction qui retourne la valeur absolue d'un nombre.
	float ABS( float nombre );

	/// Fonction pour savoir si un float est égal à zéro.
	bool EGAL_ZERO(float Nombre);

	/// Pour savoir si un nombre est dans l'intervalle: [BorneMin, BorneMax].
	bool DANS_INTERVALLE( float Valeur, float BorneMin, float BorneMax );
	/// Fonction pour vérifier si un Point 2d est dans un carré.
	bool DANS_LIMITESXY( float x, float xMin, float xMax,
		float y, float yMin, float yMax );

	/// Calcule le rapport d'écrasement
	float ecrasement( float a, float b);

	/// Vérification de l'existance d'un fichier
	bool fichierExiste( const std::string& nomDuFichier );

	/// Applique une matrice de transformation à un point.
	Vecteur3 appliquerMatrice( const Vecteur3& point, const float mat[] );

	// Foncteur permettant de supprimer chaque element avec un for_each d'un vecteur
	struct LibererVecteur
	{
		template <typename T>
		void operator()(const T& element) const
		{
			try
			{
				delete element;
			}
			catch (...)
			{

			}
		}
	};

	// Foncteur permettant de supprimer chaque element avec un for_each d'une mappe
	struct LibererMappe
	{
		template <typename T>
		void operator()(const T& element) const
		{
			try
			{
				delete element.second;
			}
			catch (...)
			{

			}
		}
	};

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void borneSuperieure( T& valeur, const T& borne )
	///
	/// Cette fonction s'assure que la valeur ne dépasse pas la borne.
	///
	/// @param[in,out] valeur : La valeur de l'expression à borner.
	/// @param[in]     borne  : La borne supérieure.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	template <class T>
	void borneSuperieure( T& valeur, const T& borne )
	{
		if ( valeur > borne )
			valeur = borne;
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void borneInferieure( T& valeur, const T& borne )
	///
	/// Cette fonction s'assure que la valeur dépasse la borne.
	///
	/// @param[in,out] valeur : La valeur de l'expression à borner.
	/// @param[in]     borne  : La borne inférieure.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	template <class T>
	void borneInferieure( T& valeur, const T& borne )
	{
		if ( valeur < borne )
			valeur = borne;
	}


	////////////////////////////////////////////////////////////////////////
	///
	/// @fn static std::string convertirEnChaine(const T& valeur)
	///
	/// @brief Conversion d'un type générique vers une chaîne de caractères.
	///
	/// Cette fonction convertit un type générique en une chaîne de
	/// caractères.  Comme elle se sert de l'opérateur << appliqué sur
	/// un ostringstream, elle devrait fonctionner pour toutes les classes
	/// qui définissent cet opérateur pour les ostream&.
	///
	/// @param[in] valeur    : Valeur template à convertir en chaîne de
	///                        caractères.
	/// @param[in] modifieur : Fonction utilisée pour la conversion de type
	///                        particulier, par exemple std::hex ou std::oct
	///                        pour les entiers.
	///
	/// @return Chaîne représentant le type.
	///
	////////////////////////////////////////////////////////////////////////
	template<typename T> static std::string convertirEnChaine(
		const T& valeur,
		std::ios_base& (*modifieur)(std::ios_base&) = 0
		)
	{
		std::ostringstream oss;

		// Application du modifieur s'il est spécifié.
		if (modifieur)
			oss << modifieur;

		oss << valeur;
		return oss.str();
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn static bool convertirEnType(const std::string& chaine, T& valeur, std::ios_base& (*modifieur)(std::ios_base&) = 0)
	///
	/// @brief Conversion d'une chaîne de caractères vers un type générique.
	///
	/// Cette fonction convertit une chaîne de caractères en un type générique.
	/// Comme elle se sert de l'opérateur >> appliqué sur un istringstream,
	/// elle devrait fonctionner pour toutes les classes qui définissent cet
	/// opérateur pour les istream&.
	///
	/// @param[in] chaine    : Chaîne à convertir.
	/// @param[in] valeur    : Valeur template qui contiendra la valeur contenu
	///                        dans la chaîne.  Ne sera pas modifiée si la
	///                        chaîne n'est pas valide.
	/// @param[in] modifieur : Fonction utilisée pour la conversion de type
	///                        particulier, par exemple std::hex ou std::oct
	///                        pour les entiers.
	///
	/// @return Vrai si la conversion a réussi.
	///
	////////////////////////////////////////////////////////////////////////
	template<typename T> static bool convertirEnType(
		const std::string& chaine,
		T& valeur,
		std::ios_base& (*modifieur)(std::ios_base&) = 0
		)
	{
		std::istringstream iss(chaine);

		// Application du modifieur s'il est spécifié.
		if (modifieur)
			iss >> modifieur;

		// Lecture de la valeur
		T temp;
		iss >> temp;

		// Pour que la conversion soit un succès, toute la chaîne doit avoir été
		// convertie.  On ajoute donc le second test pour éviter qu'une chaîne
		// comme "123bidon" puisse être acceptée et que toute la chaîne soit
		// utilisée pour que la conversion soit considérée comme réussie.
		const bool succes = ( (!iss.fail()) && (iss.peek() == EOF) );

		if (succes)
			valeur = temp;

		return succes;
	}


} // Fin du namespace utilitaire.


#endif // __UTILITAIRE_UTILITAIRE_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
