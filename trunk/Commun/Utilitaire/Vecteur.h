///////////////////////////////////////////////////////////////////////////////
/// @file Vecteur.h
/// @author Alex Duru
/// @author Ionut Alexandrescu
/// @author Martin Bisson
/// @author Jean-François Pérusse
/// @date 2005-06-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#ifndef __UTILITAIRE_VECTEUR_H__
#define __UTILITAIRE_VECTEUR_H__

#include <string>

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

///////////////////////////////////////////////////////////////////////////
/// @class Vecteur
///
///        Définition d'une classe générique pour les vecteurs.
///
///        Cette classe générique implante des vecteurs en n dimensions.
///        Le type du vecteur est aussi générique (double, float, int,...).
///        Les fonctions communes définies sont:
///        - [] pour la lecture et l'écriture
///        - conversion double/float ou int/float
///        - norme du vecteur
///        - si le vecteur est nul
///        - opérateur =
///        - opérateur ==
///        - opérateur +=
///        - opérateur -=
///        - normalisation du vecteur
///        - opérateur *= (multiplication par un scalaire)
///        - opérateur /= (division par un scalaire)
///        - opérateur +
///        - opérateur -
///        - opérateur * (multiplication par un scalaire)
///        - opérateur / (division par un scalaire)
///        - produit scalaire
///        - la réflexion du vecteur @a u par rapport à l'espace vectoriel orthogonal
///        - projection du vecteur @a u sur l'espace vectoriel orthogonal
///        - projection du vecteur @a u sur une droite
///        - opérateur << (sortie texte)
///        - opérateur >> (entrée texte)
///        - opérateur <  (sortie binaire)
///        - opérateur >  (entrée binaire)
///
/// @author Alex Duru
/// @author Ionut Alexandrescu
/// @author Martin Bisson
/// @author Jean-François Pérusse.
/// @date 2006-02-23
///////////////////////////////////////////////////////////////////////////
template <class T = double, const int N = 3>
class Vecteur
{
public:
	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur()
	///
	/// Constructeur par défaut qui ne fait rien.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur()
	{
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur ( const Vecteur<T2, N>& u )
	///
	/// Constructeur "copie" à partir d'un autre type de vecteur.
	///
	/// @param[in] u : Le vecteur à copier.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	template<class T2>
	inline Vecteur ( const Vecteur<T2, N>& u )
	{
		for ( int i = 0; i < N; i++ ) {
			vect[i] = static_cast<T>(u[i]);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur ( const Vecteur<T2, N>& u )
	///
	/// Constructeur "copie" à partir d'un vecteur de taille differente
	///
	/// @param[in] u : Le vecteur à copier.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	template<const int N2>
	inline Vecteur ( const Vecteur<T, N2>& u )
	{
		for ( int i = 0; i < N; i++ ) {
			if(i>=N2)
				vect[i] = 0;	// Assigne a zero aux dimensions que u ne possede pas
			else
				vect[i] = u[i];
		}
		// ignore les dimensions superieures
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline T& operator[] ( const int& i )
	///
	/// Opérateur de lecture d'une composante ( @a x: 0, @a y: 1, @a z: 2 ...)
	///
	/// @param[in] i : ( @a x: 0, @a y: 1, @a z: 2 ...)
	///
	/// @return La composante ( @a x ou @a y ou @a z ...)
	///
	////////////////////////////////////////////////////////////////////////////
	inline T& operator[] ( const int& i )
	{
		return vect[i];
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const T& operator[] ( const int& i ) const
	///
	/// Opérateur de lecture d'une composante sans la modifier ( @a x: 0, @a y: 1, @a z: 2 ...)
	///
	/// @param[in] i : ( @a x: 0, @a y: 1, @a z: 2 ...)
	///
	/// @return La composante ( @a x ou @a y ou @a z ...)
	///
	////////////////////////////////////////////////////////////////////////////
	inline const T& operator[] ( const int& i ) const
	{
		return vect[i];
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline T norme2() const
	///
	/// Cette fonction calcule le carré de la norme du vecteur
	/// ( ( x*x + y*y +...) ).
	///
	/// @return Le carré de la norme du vecteur.
	///
	////////////////////////////////////////////////////////////////////////////
	inline T norme2() const
	{
		T n2 = 0;
		T pow2;
		for ( int i = 0; i < N; i++ ) {
			pow2  = vect[i];
			pow2 *= pow2;
			n2   += pow2;
		}

		return ( n2 );
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline T norme() const
	///
	/// Cette fonction calcule la norme du vecteur ( sqrt ( x*x + y*y +...) ).
	///
	/// @return La norme du vecteur.
	///
	////////////////////////////////////////////////////////////////////////////
	inline T norme() const
	{
		return sqrt( norme2() );
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline bool estNul() const
	///
	/// Cette fonction teste si le vecteur est nul.
	///
	/// @return @a true si le vecteur est nul, @a false dans le cas contraire.
	///
	////////////////////////////////////////////////////////////////////////////
	inline bool estNul() const
	{
		return (norme() == 0);
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const Vecteur<T, N>& operator= ( const Vecteur<T, N>& v )
	///
	/// Surcharge de l'opérateur d'assignation.
	///
	/// @param[in] v : Le vecteur à assigner.
	///
	/// @return La référence vers l'objet assigné afin de respecter la
	///         sémantique de l'opérateur d'assignation C++.
	///
	////////////////////////////////////////////////////////////////////////////
	inline const Vecteur<T, N>& operator= ( const Vecteur<T, N>& v )
	{
		for ( int i = 0; i < N; i++ ) {
			vect[i] = v.vect[i];
		}
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const Vecteur<T, N>& operator= ( const Vecteur<T, N2>& v )
	///
	/// Surcharge de l'opérateur d'assignation pour des vecteur de taille differentes.
	///
	/// @param[in] v : Le vecteur à assigner.
	///
	/// @return La référence vers l'objet assigné afin de respecter la
	///         sémantique de l'opérateur d'assignation C++.
	///
	////////////////////////////////////////////////////////////////////////////
	template<const int N2>
	inline const Vecteur<T, N>& operator= ( const Vecteur<T, N2>& v )
	{
		for ( int i = 0; i < N; i++ ) {
			if(i>=N2)
				vect[i] = 0;	// Assigne a zero aux dimensions que u ne possede pas
			else
				vect[i] = u[i];
		}
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline bool operator == ( const Vecteur<T, N>& v ) const
	///
	/// Surcharge de l'opérateur d'égalité.
	///
	/// @param[in] v : Le vecteur dont on vérifie l'égalité.
	///
	/// @return Vrai si les vecteurs sont égaux, faux autrement.
	///
	////////////////////////////////////////////////////////////////////////////
	inline bool operator == ( const Vecteur<T, N>& v ) const
	{
		for ( int i = 0; i < N; i++ ) {
			if ( vect[i] != v.vect[i] )
				return false;
		}
		return true;
	}

	inline bool operator != ( const Vecteur<T, N>& v ) const
	{
		for ( int i = 0; i < N; i++ ) {
			if ( vect[i] != v.vect[i] )
				return true;
		}
		return false;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const Vecteur<T, N>& operator += ( const Vecteur<T, N>& u )
	///
	/// Surcharge de l'opérateur d'ajout à un vecteur.
	///
	/// @param[in] u : Le vecteur à ajouter.
	///
	/// @return La référence vers l'objet assigné afin de respecter la
	///         sémantique de l'opérateur d'ajout C++.
	///
	////////////////////////////////////////////////////////////////////////////
	inline const Vecteur<T, N>& operator += ( const Vecteur<T, N>& u )
	{
		for ( int i = 0; i < N; i++ ) {
			vect[i] += u.vect[i];
		}
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const Vecteur<T, N>& operator -= ( const Vecteur<T, N>& u )
	///
	/// Surcharge de l'opérateur de retrait d'un vecteur.
	///
	/// @param[in] u : Le vecteur à retirer.
	///
	/// @return La référence vers l'objet assigné afin de respecter la
	///         sémantique de l'opérateur de retrait C++.
	///
	////////////////////////////////////////////////////////////////////////////
	inline const Vecteur<T, N>& operator -= ( const Vecteur<T, N>& u )
	{
		for ( int i = 0; i < N; i++ ) {
			vect[i] -= u.vect[i];
		}
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline void normaliser()
	///
	/// Cette fonction normalise le vecteur.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////////
	inline void normaliser()
	{
		const T n = norme();
		if ( n != 0 )
			( *this ) /= n;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const Vecteur<T, N>& operator*= ( const T k )
	///
	/// Surcharge de l'opérateur de mise à l'échelle par un facteur.
	///
	/// @param[in] k : Le facteur de mise à l'échelle.
	///
	/// @return La référence vers l'objet assigné afin de respecter la
	///         sémantique de l'opérateur de mise à l'échelle C++.
	///
	////////////////////////////////////////////////////////////////////////////
	inline const Vecteur<T, N>& operator*= ( const T k )
	{
		for ( int i = 0; i < N; i++ ) {
			vect[i] *= k;
		}
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const Vecteur<T, N>& operator /= ( const T k )
	///
	/// Surcharge de l'opérateur de division par un facteur.
	///
	/// @param[in] k : Le facteur de division.
	///
	/// @return La référence vers l'objet assigné afin de respecter la
	///         sémantique de l'opérateur de division C++.
	///
	////////////////////////////////////////////////////////////////////////////
	inline const Vecteur<T, N>& operator /= ( const T k )
	{
		for ( int i = 0; i < N; i++ ) {
			vect[i] /= k;
		}
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur<T, N> operator+ ( const Vecteur<T, N>& v ) const
	///
	/// Surcharge de l'opérateur d'addition.
	///
	/// @param[in] v : Le vecteur à additionner.
	///
	/// @return La somme des deux vecteurs.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur<T, N> operator+ ( const Vecteur<T, N>& v ) const
	{
		Vecteur<T, N> r ( *this );
		r += v;
		return r;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur<T, N> operator- ( const Vecteur<T, N>& v ) const
	///
	/// Surcharge de l'opérateur de soustraction.
	///
	/// @param[in] v : Le vecteur à soustraire.
	///
	/// @return Le vecteur à soustraire.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur<T, N> operator- ( const Vecteur<T, N>& v ) const
	{
		Vecteur<T, N> r ( *this );
		r -= v;
		return r;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur<T, N> operator* ( const T k ) const
	///
	/// Surcharge de l'opérateur de multiplication par un scalaire.
	///
	/// @param[in] k : Le facteur de multiplication.
	///
	/// @return Le vecteur multiplié par le scalaire.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur<T, N> operator* ( const T k ) const
	{
		Vecteur<T, N> r ( *this );
		r *= k;
		return r;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend Vecteur<T, N> operator* ( const T k , const Vecteur<T,N>& v)
	///
	/// Surcharge de l'opérateur de multiplication d'un scalaire par un vecteur.
	///
	/// @param[in] k : Le facteur de multiplication.
	/// @param[in] v : Le vecteur à multiplier.
	///
	/// @return Le vecteur multiplié par un scalaire.
	///
	////////////////////////////////////////////////////////////////////////////
	inline friend Vecteur<T, N> operator* ( const T k , const Vecteur<T,N>& v)
	{
		return v * k;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur<T, N> operator / ( const T k ) const
	///
	/// Surcharge de l'opérateur de division par un scalaire.
	///
	/// @param[in] k : Facteur de division.
	///
	/// @return Le vecteur divisé par le scalaire.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur<T, N> operator / ( const T k ) const
	{
		Vecteur<T, N> r ( *this );
		r /= k;
		return r;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur<T, N> operator >> ( const Vecteur<T,N>& dir ) const
	///
	/// Surcharge de l'opérateur de projection d'un vecteur sur un autre.
	///
	/// @param[in] dir : La direction sur laquelle projeter.
	///
	/// @return La projection du vecteur sur l'autre.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur<T, N> operator >> ( const Vecteur<T,N>& dir ) const
	{
		T k = produitScalaire(*this, dir) / produitScalaire(dir, dir);
		Vecteur<T, N> r ( dir );
		r *= k;
		return r;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend T produitScalaire ( const Vecteur<T, N>& u, const Vecteur<T, N>& v )
	///
	/// Cette fonction calcule le produit scalaire entre deux vecteurs.
	///
	/// @param[in] u : Le premier vecteur du produit scalaire.
	/// @param[in] v : Le second vecteur du produit scalaire.
	///
	/// @return Le produit scalaire des deux vecteurs.
	///
	////////////////////////////////////////////////////////////////////////////
	inline friend T produitScalaire ( const Vecteur<T, N>& u, const Vecteur<T, N>& v )
	{
		T r = 0;
		for ( int i = 0; i < N; i++ ) {
			r += u.vect[i] * v.vect[i];
		}
		return r;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend Vecteur<T, N> calculerReflexion( const Vecteur<T, N>& u, const Vecteur<T, N>& n )
	///
	/// Cette fonction calcule la réflexion du vecteur @a u par rapport à
	/// l'espace vectoriel orthogonal à @a n (@a n doit être normalisé).
	///
	/// @param[in] u : Le vecteur à réfléchir.
	/// @param[in] n : La normale par rapport à laquelle faire la réflexion.
	///
	/// @return Le vecteur réfléchi.
	///
	////////////////////////////////////////////////////////////////////////////
	inline friend Vecteur<T, N> calculerReflexion(
		const Vecteur<T, N>& u, const Vecteur<T, N>& n
		)
	{
		return u - n * ( 2 * produitScalaire ( u, n ) );
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend Vecteur<T, N> calculerProjectionPlan(const Vecteur<T, N>& u, const Vecteur<T, N>& n)
	///
	/// Cette fonction calcule la projection du vecteur @a u sur l'espace
	/// vectoriel orthogonal à @a n (@a n doit être normalisé).
	///
	/// @param[in] u : Le vecteur à projeter.
	/// @param[in] n : La normale par rapport à laquelle faire la projection.
	///
	/// @return Le vecteur projeté.
	///
	////////////////////////////////////////////////////////////////////////////
	inline friend Vecteur<T, N> calculerProjectionPlan(
		const Vecteur<T, N>& u, const Vecteur<T, N>& n
		)
	{
		return u - n * ( produitScalaire ( u, n ) );
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend Vecteur<T, N> calculerProjectionDroite(const Vecteur<T, N>& u, const Vecteur<T, N>& dir)
	///
	/// Cette fonction calcule la projection du vecteur @a u sur une droite de
	/// direction @a dir passant par l'origine.  Pour calculer la projection sur
	/// une droite ne passant pas par l'origine, calculer d'abord la différence.
	///
	/// @param[in] u   : Le vecteur à projeter.
	/// @param[in] dir : La direction de la droite sur laquelle projeter. Doit etre normalisé
	///
	/// @return Le vecteur projeté.
	///
	////////////////////////////////////////////////////////////////////////////
	inline friend Vecteur<T, N> calculerProjectionDroite(
		const Vecteur<T, N>& u, const Vecteur<T, N>& dir
		)
	{
		return dir * ( produitScalaire ( dir, u ) );
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend std::ostream& operator<< ( std::ostream& out, const Vecteur<T, N>& v )
	///
	/// Surcharge de l'opérateur d'extraction de flux de sortie pour
	/// l'affichage.
	///
	/// @param[in] out : Le flux de sortie.
	/// @param[in] v   : Le vecteur à afficher.
	///
	/// @return Le flux de sortie.
	///
	////////////////////////////////////////////////////////////////////////////
	inline friend std::ostream& operator<< (
		std::ostream& out, const Vecteur<T, N>& v
		)
	{
		for ( int i = 0; i < N; i++ ) {
			out << v[i];
			if ( i < N - 1 ) out << ' ';
		}
		return out;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend std::istream& operator>> ( std::istream& in, Vecteur<T, N>& v )
	///
	/// Surcharge de l'opérateur d'extraction de flux d'entrée.
	///
	/// @param[in] in : Le flux d'entrée.
	/// @param[in] v  : Le vecteur lu.
	///
	/// @return Le flux d'entrée.
	///
	////////////////////////////////////////////////////////////////////////////
	inline friend std::istream& operator>> (
		std::istream& in, Vecteur<T, N>& v
		)
	{
		for ( int a = 0; a < N; a++ ) {
			in >> v[a];
		}
		return in;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend CEcritureFichierBinaire& operator< ( CEcritureFichierBinaire& out, const Vecteur<T, N>& r )
	///
	/// Surcharge de l'opérateur de sortie dans un fichier binaire.
	///
	/// @param[in] out : Le fichier binaire de sortie.
	/// @param[in] r   : Le vecteur à écrire.
	///
	/// @return Le fichier binaire de sortie.
	///
	////////////////////////////////////////////////////////////////////////////
// 	inline friend CEcritureFichierBinaire& operator< (
// 		CEcritureFichierBinaire& out, const Vecteur<T, N>& r
// 		)
// 	{
// 		for ( int a = 0; a < N; a++ ) {
// 			out < r[a];
// 		}
// 		return out;
// 	}



	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend CLectureFichierBinaire& operator> (CLectureFichierBinaire& in, Vecteur<T, N>& r)
	///
	/// Surcharge de l'opérateur de lecture dans un fichier binaire.
	///
	/// @param[in] in : Le fichier binaire d'entrée.
	/// @param[in] r  : Le vecteur à lire.
	///
	/// @return Le fichier binaire d'entrée.
	///
	////////////////////////////////////////////////////////////////////////////
// 	inline friend CLectureFichierBinaire& operator> (
// 		CLectureFichierBinaire& in, Vecteur<T, N>& r
// 		)
// 	{
// 		for ( int a = 0; a < N; a++ ) {
// 			in > r[a];
// 		}
// 		return in;
// 	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur<T, N> operator- () const
	///
	/// Surcharge de l'opérateur unaire de négation.
	///
	/// @return L'opposé du vecteur.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur<T, N> operator- () const
	{
		Vecteur<T, N> r;
		for ( int i = 0; i < N; i++ ) {
			r[i] = - ( *this ) [i];
		}
		return r;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn template <const int N2> inline Vecteur<T, N2> convertir() const
	///
	/// Cette fonction convertit entre les vecteurs de différentes dimensions.
	///
	/// @return Le vecteur converti.
	///
	////////////////////////////////////////////////////////////////////////////
	template <const int N2>
	inline Vecteur<T, N2> convertir() const
	{
		return convertir<T, N2>();
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn template <class T2, const int N2> inline Vecteur<T2, N2> convertir() const
	///
	/// Cette fonction convertit entre les vecteurs de différentes dimensions
	/// et de différents types.
	///
	/// @return Le vecteur converti.
	///
	////////////////////////////////////////////////////////////////////////////
	template <class T2, const int N2>
	inline Vecteur<T2, N2> convertir() const
	{
		Vecteur<T2, N2> nouveauVecteur;
		if (N2 <= N) {
			for (int i = 0 ; i < N2; ++i)
				nouveauVecteur[i] = static_cast<T2>(vect[i]);
		}
		else {
			int i;
			for (i = 0; i < N; ++i)
				nouveauVecteur[i] = static_cast<T2>(vect[i]);
			for (i = N; i < N2; ++i)
				nouveauVecteur[i] = 0;
		}

		return nouveauVecteur;
	}

	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur2D tournerMoinsPiSur2() const
	///
	/// tri de 2 vecteur en x, le point le plus a gauche est dans le vecteur appelant
	///
	/// @return void
	///
	////////////////////////////////////////////////////////////////////////////
	inline void triEnX( Vecteur<T, N>& v )
	{
		if(vect[VX] > v[VX])
		{
			for ( int i = 0; i < N; i++ ) {
				swap(vect[i],v[i]);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn void Vecteur<T, N>::remetAZero(  )
	///
	/// fonction pour remettre un vecteur a zero sans avoir a passer par un constructeur
	///	l'operateur = 
	///
	/// @return void
	///
	////////////////////////////////////////////////////////////////////////
	inline void remetAZero()
	{
		for ( int i = 0; i < N; i++ ) 
		{
			vect[i] = 0;
		}
	}



protected:
	/// Le vecteur générique de dimension @a N.
	T vect[N];


};



///////////////////////////////////////////////////////////////////////////
/// @class Vecteur2D
///
///        Classe générique pour implanter des vecteurs en 2D.
///
///        Cette classe générique est définie pour les vecteurs de dimension 2.
///        Les opérations nécessaires sont définies, soient:
///        - le constructeur de la classe ;
///        - opérateur = ;
///        - calcul de l'angle de rotation ;
///        - rotation du vecteur .
///
/// @author Alex Duru
/// @author Ionut Alexandrescu
/// @author Martin Bisson
/// @date 2006-02-23
///////////////////////////////////////////////////////////////////////////
template<class T>
class Vecteur2D: public Vecteur<T, 2>
{
public:
	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur2D( const Vecteur2D<T2>& u )
	///
	/// Constructeur avec conversion.
	///
	/// @param[in] u : Le vecteur à copier dans le nouveau vecteur.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	template<class T2>
	inline Vecteur2D( const Vecteur2D<T2>& u )
		: Vecteur<T, 2> ( u )
	{
	}

	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur2D( const T x = 0, const T y = 0 )
	///
	/// Constructeur par paramètres.
	///
	/// @param[in] x : La coordonnée en @a x.
	/// @param[in] y : La coordonnée en @a y.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur2D( const T x = 0, const T y = 0 )
	{
		vect[0] = x;
		vect[1] = y;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur2D ( const Vecteur<T, 2>& v )
	///
	/// Constructeur copie.
	///
	/// @param[in] v : Le vecteur à copier dans le nouveau vecteur.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur2D ( const Vecteur<T, 2>& v )
		: Vecteur<T, 2> ( v )
	{
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const Vecteur2D& operator= ( const Vecteur<T, 2>& v )
	///
	/// Surcharge de l'opérateur d'assignation.
	///
	/// @param[in] v : Le vecteur à copier.
	///
	/// @return La référence vers l'objet assigné afin de respecter la
	///         sémantique de l'opérateur d'assignation C++.
	///
	////////////////////////////////////////////////////////////////////////////
	inline const Vecteur2D& operator= ( const Vecteur<T, 2>& v )
	{
		Vecteur<T, 2>::operator= ( v );
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline T angle() const
	///
	/// Cette fonction calcule l'angle du vecteur par rapport à l'axe des X
	/// positifs dans le sens anti-horaire.
	///
	/// @return L'angle du vecteur par rapport à l'axe des X dans le sens
	///         anti-horaire.
	///
	////////////////////////////////////////////////////////////////////////////
	inline T angle() const
	{
		return atan2 ( vect[1], vect[0] );
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur2D tourner(const T angle) const
	///
	/// Cette fonction retourne un nouveau vecteur ayant effectué une rotation
	/// d'un angle donné dans le sens anti-horaire.
	///
	/// @param[in] angle : L'angle duquel faire tourner le vecteur.
	///
	/// @return Le vecteur après la rotation.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur2D tourner(const T angle) const
	{
		const T cosAngle = cos(angle);
		const T sinAngle = sin(angle);
		return Vecteur2D(
			vect[0] * cosAngle + vect[1] * -sinAngle,
			vect[0] * sinAngle + vect[1] * cosAngle
			);
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur2D tournerPiSur2() const
	///
	/// Cette fonction retourne un nouveau vecteur ayant effectué une rotation
	/// d'un angle de Pi sur 2 dans le sens anti-horaire.
	///
	/// @return Le vecteur après la rotation.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur2D tournerPiSur2() const
	{
		return Vecteur2D(-(*this)[1], (*this)[0]);
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur2D tournerMoinsPiSur2() const
	///
	/// Cette fonction retourne un nouveau vecteur ayant effectué une rotation
	/// d'un angle de Pi sur 2 dans le sens horaire.
	///
	/// @return Le vecteur après la rotation.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur2D tournerMoinsPiSur2() const
	{
		return Vecteur2D((*this)[1], -(*this)[0]);
	}

};




///////////////////////////////////////////////////////////////////////////
/// @class Vecteur3D
///
///        Classe générique pour implanter des vecteurs en 3D.
///
///        Cette classe générique est définie pour les vecteurs de dimension 3.
///        Les opérations nécessaires sont définies, soient:
///        - les constructeurs ;
///        - opérateur = ;
///        - calcul de l'angle de rotation ;
///        - calcul de l'angle de élévation ;
///        - transformation du vecteur en coordonées sphériques ;
///        - le produit vectoriel.
///
/// @author Alex Duru
/// @author Ionut Alexandrescu
/// @author Martin Bisson
/// @date 2006-02-23
///////////////////////////////////////////////////////////////////////////
template<class T>
class Vecteur3D: public Vecteur<T, 3>
{
public:
	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur3D ( const Vecteur3D<T2>& u )
	///
	/// Constructeur avec conversion.
	///
	/// @param[in] u : Le vecteur à copier dans le nouveau vecteur.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	template<class T2>
	inline Vecteur3D ( const Vecteur3D<T2>& u )
		: Vecteur<T, 3> ( u )
	{
	}

	template<class T2>
	inline Vecteur3D ( const Vecteur2D<T2>& u )
	{
		vect[0] = u[0];
		vect[1] = u[1];
		vect[2] = 0;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur3D ( const T x = 0, const T y = 0, const T z = 0 )
	///
	/// Constructeur par paramètres.
	///
	/// @param[in] x : La coordonnée en @a x.
	/// @param[in] y : La coordonnée en @a y.
	/// @param[in] z : La coordonnée en @a z.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur3D ( const T x = 0, const T y = 0, const T z = 0 )
	{
		vect[0] = x;
		vect[1] = y;
		vect[2] = z;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur3D ( const Vecteur<T, 3>& v1, const Vecteur<T, 3>& v2 )
	///
	/// Constructeur à partir de deux points.  Le vecteur construit est dirigé
	/// du premier vers le second.
	///
	/// @param[in] v1 : Le premier vecteur (source).
	/// @param[in] v2 : Le second vecteur (destination).
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur3D ( const Vecteur<T, 3>& v1, const Vecteur<T, 3>& v2 )
		: Vecteur<T, 3> ( v2 - v1 )
	{
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur3D ( const Vecteur<T, 3>& v )
	///
	/// Constructeur copie.
	///
	/// @param[in] v : Le vecteur à copier dans le nouveau vecteur.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur3D ( const Vecteur<T, 3>& v )
		: Vecteur<T, 3> ( v )
	{
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const Vecteur3D& operator= ( const Vecteur<T, 3>& v )
	///
	/// Surcharge de l'opérateur d'assignation.
	///
	/// @param[in] v : Le vecteur à copier.
	///
	/// @return La référence vers l'objet assigné afin de respecter la
	///         sémantique de l'opérateur d'assignation C++.
	///
	////////////////////////////////////////////////////////////////////////////
	inline const Vecteur3D& operator= ( const Vecteur<T, 3>& v )
	{
		Vecteur<T, 3>::operator= ( v );
		return *this;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline T lireTheta() const
	///
	/// Cette fonction calcule l'angle de rotation (@a xy) du vecteur 3D.
	///
	/// @return L'angle de rotation du vecteur 3D.
	///
	////////////////////////////////////////////////////////////////////////////
	inline T lireTheta() const
	{
		Vecteur2D<T> xy ( vect[0], vect[1] );
		return xy.angle();
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline T lirePhi() const
	///
	/// Cette fonction calcule l'angle d'élévation du vecteur 3D.  Le vecteur
	/// dirigé vers l'axe des Z positifs, k, (0,0,1), aura comme angle 0.
	///
	/// @return L'angle d'élévation du vecteur 3D.
	///
	////////////////////////////////////////////////////////////////////////////
	inline T lirePhi() const
	{
		Vecteur2D<T> xy ( vect[0], vect[1] );
		Vecteur2D<T> zr ( vect[2], xy.norme() );
		return zr.angle();
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline void lireCoordSpheriques ( T& r, T& phi, T& theta ) const
	///
	/// Cette fonction retourne les coordonnées sphériques du vecteur via les
	/// paramètres passés en référence.
	///
	/// @param[out] r     : Le rayon des coordonnées sphériques.
	/// @param[out] phi   : L'angle d'élévation des coordonnées sphériques.
	/// @param[out] theta : L'angle de rotation des coordonnées sphériques.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////////
	inline void lireCoordSpheriques ( T& r, T& phi, T& theta ) const
	{
		r     = norme();
		theta = lireTheta();
		phi   = lirePhi();
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline void assignerCoordSpheriques ( const T r, const T phi, const T theta )
	///
	/// Cette fonction assigne les coordonnées du vecteur qui sont données en
	/// coordonnées sphériques.
	///
	/// @param[in] r     : Le rayon des coordonnées sphériques.
	/// @param[in] phi   : L'angle d'élévation des coordonnées sphériques.
	/// @param[in] theta : L'angle de rotation des coordonnées sphériques.
	///
	/// @return Aucune.
	///
	///////////////////////////////////////////////////////////////////////////////
	inline void assignerCoordSpheriques ( const T r, const T phi, const T theta )
	{
		// Calcul de la position à partir des coordonnées sphériques
		vect[0] = r * sin(phi) * cos(theta);
		vect[1] = r * sin(phi) * sin(theta);
		vect[2] = r * cos(phi);
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline friend Vecteur3D produitVectoriel ( const Vecteur3D& A, const Vecteur3D& B )
	///
	/// Cette fonction calcule le produit vectoriel des deux vecteurs.
	///
	/// @param[in] A : Le premier vecteur du produit.
	/// @param[in] B : Le second vecteur du produit.
	///
	/// @return Le produit vectoriel des deux vecteurs.
	///
	////////////////////////////////////////////////////////////////////////////
	inline friend Vecteur3D produitVectoriel (
		const Vecteur3D& A, const Vecteur3D& B
		)
	{
		return Vecteur3D(
			A[1] * B[2] - A[2] * B[1],
			A[2] * B[0] - A[0] * B[2],
			A[0] * B[1] - A[1] * B[0]
		);
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur3D tourner(const T angle, const Vecteur3D& axe) const
	///
	/// Cette fonction calcule la rotation d'un vecteur autour d'un axe
	/// arbitraire.
	///
	/// @param[in] angle : L'angle de rotation, en radians.
	/// @param[in] axe   : L'axe de rotation (vecteur unitaire).
	///
	/// @return Le vecteur après la rotation.
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur3D tourner(const T angle, const Vecteur3D& axe) const
	{
		const T cosAngle = cos(angle);
		const T sinAngle = sin(angle);
		const T unMoinsCosAngle = 1 - cosAngle;

		return Vecteur3D(
			// Calcul de la composante X
			(cosAngle + unMoinsCosAngle * axe[0] * axe[0])          * vect[0] +
			(unMoinsCosAngle * axe[0] * axe[1] - axe[2] * sinAngle) * vect[1] +
			(unMoinsCosAngle * axe[0] * axe[2] + axe[1] * sinAngle) * vect[2],
			// Calcul de la composante Y
			(unMoinsCosAngle * axe[0] * axe[1] + axe[2] * sinAngle) * vect[0] +
			(cosAngle + unMoinsCosAngle * axe[1] * axe[1])          * vect[1] +
			(unMoinsCosAngle * axe[1] * axe[2] - axe[0] * sinAngle) * vect[2],
			// Calcul de la composante Z
			(unMoinsCosAngle * axe[0] * axe[2] - axe[1] * sinAngle) * vect[0] +
			(unMoinsCosAngle * axe[1] * axe[2] + axe[0] * sinAngle) * vect[1] +
			(cosAngle + unMoinsCosAngle * axe[2] * axe[2])          * vect[2]
		);
	}

};




///////////////////////////////////////////////////////////////////////////
/// @class Vecteur4D
///
///        Classe générique pour implanter des vecteurs en 4D.
///
///        Cette classe générique est définie pour les vecteurs de dimension 4.
///        Les opérations nécessaires sont définies, soient:
///        - les constucteurs
///        - opérateur d'égalité
///
/// @author Alex Duru
/// @author Ionut Alexandrescu
/// @date 2006-02-23
///////////////////////////////////////////////////////////////////////////
template<class T>
class Vecteur4D: public Vecteur<T, 4>
{
public:
	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur4D ( const Vecteur4D<T2>& u )
	///
	/// Constructeur avec conversion.
	///
	/// @param[in] u : Le vecteur à copier dans le nouveau vecteur.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	template<class T2>
	inline Vecteur4D ( const Vecteur4D<T2>& u )
		: Vecteur<T, 4> ( u )
	{
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur4D ( const T x = 0, const T y = 0, const T z = 0, const T w = 0 )
	///
	/// Constructeur par paramètres.
	///
	/// @param[in] x : La coordonnée en @a x.
	/// @param[in] y : La coordonnée en @a y.
	/// @param[in] z : La coordonnée en @a z.
	/// @param[in] w : La coordonnée en @a w.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur4D ( const T x = 0, const T y = 0, const T z = 0, const T w = 0 )
	{
		vect[0] = x;
		vect[1] = y;
		vect[2] = z;
		vect[3] = w;
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline Vecteur4D ( const Vecteur<T, 4>& v )
	///
	/// Constructeur copie.
	///
	/// @param[in] v : Le vecteur à copier dans le nouveau vecteur.
	///
	/// @return Aucune (constructeur).
	///
	////////////////////////////////////////////////////////////////////////////
	inline Vecteur4D ( const Vecteur<T, 4>& v )
		: Vecteur<T, 4> ( v )
	{
	}


	////////////////////////////////////////////////////////////////////////////
	///
	/// @fn inline const Vecteur4D& operator= ( const Vecteur<T, 4>& v )
	///
	/// Surcharge de l'opérateur d'assignation.
	///
	/// @param[in] v : Le vecteur à copier.
	///
	/// @return La référence vers l'objet assigné afin de respecter la
	///         sémantique de l'opérateur d'assignation C++.
	///
	////////////////////////////////////////////////////////////////////////////
	inline const Vecteur4D& operator= ( const Vecteur<T, 4>& v )
	{
		Vecteur<T, 4>::operator= ( v );
		return *this;
	}


};




/// Définition de type de vecteur à 2 coordonnées de type double.
typedef Vecteur2D<double> Vecteur2;
/// Définition de type de vecteur à 3 coordonnées de type double.
typedef Vecteur3D<double> Vecteur3;
/// Définition de type de vecteur à 4 coordonnées de type double.
typedef Vecteur4D<double> Vecteur4;

/// Définition de type de vecteur à 2 coordonnées de type float.
typedef Vecteur2D<float> Vecteur2f;
/// Définition de type de vecteur à 3 coordonnées de type float.
typedef Vecteur3D<float> Vecteur3f;
/// Définition de type de vecteur à 4 coordonnées de type float.
typedef Vecteur4D<float> Vecteur4f;

/// Définition de type de vecteur à 2 coordonnées de type int.
typedef Vecteur2D<int> Vecteur2i;
/// Définition de type de vecteur à 3 coordonnées de type int.
typedef Vecteur3D<int> Vecteur3i;
/// Définition de type de vecteur à 4 coordonnées de type int.
typedef Vecteur4D<int> Vecteur4i;



enum vecteurPos{VX,VY,VZ,VW};

#endif // __UTILITAIRE_VECTEUR_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
