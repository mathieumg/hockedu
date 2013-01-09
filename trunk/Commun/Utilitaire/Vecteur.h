///////////////////////////////////////////////////////////////////////////////
/// @file Vecteur.h
/// @author Alex Duru
/// @author Ionut Alexandrescu
/// @author Martin Bisson
/// @author Jean-Fran�ois P�russe
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
///        D�finition d'une classe g�n�rique pour les vecteurs.
///
///        Cette classe g�n�rique implante des vecteurs en n dimensions.
///        Le type du vecteur est aussi g�n�rique (double, float, int,...).
///        Les fonctions communes d�finies sont:
///        - [] pour la lecture et l'�criture
///        - conversion double/float ou int/float
///        - norme du vecteur
///        - si le vecteur est nul
///        - op�rateur =
///        - op�rateur ==
///        - op�rateur +=
///        - op�rateur -=
///        - normalisation du vecteur
///        - op�rateur *= (multiplication par un scalaire)
///        - op�rateur /= (division par un scalaire)
///        - op�rateur +
///        - op�rateur -
///        - op�rateur * (multiplication par un scalaire)
///        - op�rateur / (division par un scalaire)
///        - produit scalaire
///        - la r�flexion du vecteur @a u par rapport � l'espace vectoriel orthogonal
///        - projection du vecteur @a u sur l'espace vectoriel orthogonal
///        - projection du vecteur @a u sur une droite
///        - op�rateur << (sortie texte)
///        - op�rateur >> (entr�e texte)
///        - op�rateur <  (sortie binaire)
///        - op�rateur >  (entr�e binaire)
///
/// @author Alex Duru
/// @author Ionut Alexandrescu
/// @author Martin Bisson
/// @author Jean-Fran�ois P�russe.
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
	/// Constructeur par d�faut qui ne fait rien.
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
	/// Constructeur "copie" � partir d'un autre type de vecteur.
	///
	/// @param[in] u : Le vecteur � copier.
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
	/// Constructeur "copie" � partir d'un vecteur de taille differente
	///
	/// @param[in] u : Le vecteur � copier.
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
	/// Op�rateur de lecture d'une composante ( @a x: 0, @a y: 1, @a z: 2 ...)
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
	/// Op�rateur de lecture d'une composante sans la modifier ( @a x: 0, @a y: 1, @a z: 2 ...)
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
	/// Cette fonction calcule le carr� de la norme du vecteur
	/// ( ( x*x + y*y +...) ).
	///
	/// @return Le carr� de la norme du vecteur.
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
	/// Surcharge de l'op�rateur d'assignation.
	///
	/// @param[in] v : Le vecteur � assigner.
	///
	/// @return La r�f�rence vers l'objet assign� afin de respecter la
	///         s�mantique de l'op�rateur d'assignation C++.
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
	/// Surcharge de l'op�rateur d'assignation pour des vecteur de taille differentes.
	///
	/// @param[in] v : Le vecteur � assigner.
	///
	/// @return La r�f�rence vers l'objet assign� afin de respecter la
	///         s�mantique de l'op�rateur d'assignation C++.
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
	/// Surcharge de l'op�rateur d'�galit�.
	///
	/// @param[in] v : Le vecteur dont on v�rifie l'�galit�.
	///
	/// @return Vrai si les vecteurs sont �gaux, faux autrement.
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
	/// Surcharge de l'op�rateur d'ajout � un vecteur.
	///
	/// @param[in] u : Le vecteur � ajouter.
	///
	/// @return La r�f�rence vers l'objet assign� afin de respecter la
	///         s�mantique de l'op�rateur d'ajout C++.
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
	/// Surcharge de l'op�rateur de retrait d'un vecteur.
	///
	/// @param[in] u : Le vecteur � retirer.
	///
	/// @return La r�f�rence vers l'objet assign� afin de respecter la
	///         s�mantique de l'op�rateur de retrait C++.
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
	/// Surcharge de l'op�rateur de mise � l'�chelle par un facteur.
	///
	/// @param[in] k : Le facteur de mise � l'�chelle.
	///
	/// @return La r�f�rence vers l'objet assign� afin de respecter la
	///         s�mantique de l'op�rateur de mise � l'�chelle C++.
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
	/// Surcharge de l'op�rateur de division par un facteur.
	///
	/// @param[in] k : Le facteur de division.
	///
	/// @return La r�f�rence vers l'objet assign� afin de respecter la
	///         s�mantique de l'op�rateur de division C++.
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
	/// Surcharge de l'op�rateur d'addition.
	///
	/// @param[in] v : Le vecteur � additionner.
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
	/// Surcharge de l'op�rateur de soustraction.
	///
	/// @param[in] v : Le vecteur � soustraire.
	///
	/// @return Le vecteur � soustraire.
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
	/// Surcharge de l'op�rateur de multiplication par un scalaire.
	///
	/// @param[in] k : Le facteur de multiplication.
	///
	/// @return Le vecteur multipli� par le scalaire.
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
	/// Surcharge de l'op�rateur de multiplication d'un scalaire par un vecteur.
	///
	/// @param[in] k : Le facteur de multiplication.
	/// @param[in] v : Le vecteur � multiplier.
	///
	/// @return Le vecteur multipli� par un scalaire.
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
	/// Surcharge de l'op�rateur de division par un scalaire.
	///
	/// @param[in] k : Facteur de division.
	///
	/// @return Le vecteur divis� par le scalaire.
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
	/// Surcharge de l'op�rateur de projection d'un vecteur sur un autre.
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
	/// Cette fonction calcule la r�flexion du vecteur @a u par rapport �
	/// l'espace vectoriel orthogonal � @a n (@a n doit �tre normalis�).
	///
	/// @param[in] u : Le vecteur � r�fl�chir.
	/// @param[in] n : La normale par rapport � laquelle faire la r�flexion.
	///
	/// @return Le vecteur r�fl�chi.
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
	/// vectoriel orthogonal � @a n (@a n doit �tre normalis�).
	///
	/// @param[in] u : Le vecteur � projeter.
	/// @param[in] n : La normale par rapport � laquelle faire la projection.
	///
	/// @return Le vecteur projet�.
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
	/// une droite ne passant pas par l'origine, calculer d'abord la diff�rence.
	///
	/// @param[in] u   : Le vecteur � projeter.
	/// @param[in] dir : La direction de la droite sur laquelle projeter. Doit etre normalis�
	///
	/// @return Le vecteur projet�.
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
	/// Surcharge de l'op�rateur d'extraction de flux de sortie pour
	/// l'affichage.
	///
	/// @param[in] out : Le flux de sortie.
	/// @param[in] v   : Le vecteur � afficher.
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
	/// Surcharge de l'op�rateur d'extraction de flux d'entr�e.
	///
	/// @param[in] in : Le flux d'entr�e.
	/// @param[in] v  : Le vecteur lu.
	///
	/// @return Le flux d'entr�e.
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
	/// Surcharge de l'op�rateur de sortie dans un fichier binaire.
	///
	/// @param[in] out : Le fichier binaire de sortie.
	/// @param[in] r   : Le vecteur � �crire.
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
	/// Surcharge de l'op�rateur de lecture dans un fichier binaire.
	///
	/// @param[in] in : Le fichier binaire d'entr�e.
	/// @param[in] r  : Le vecteur � lire.
	///
	/// @return Le fichier binaire d'entr�e.
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
	/// Surcharge de l'op�rateur unaire de n�gation.
	///
	/// @return L'oppos� du vecteur.
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
	/// Cette fonction convertit entre les vecteurs de diff�rentes dimensions.
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
	/// Cette fonction convertit entre les vecteurs de diff�rentes dimensions
	/// et de diff�rents types.
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
	/// Le vecteur g�n�rique de dimension @a N.
	T vect[N];


};



///////////////////////////////////////////////////////////////////////////
/// @class Vecteur2D
///
///        Classe g�n�rique pour implanter des vecteurs en 2D.
///
///        Cette classe g�n�rique est d�finie pour les vecteurs de dimension 2.
///        Les op�rations n�cessaires sont d�finies, soient:
///        - le constructeur de la classe ;
///        - op�rateur = ;
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
	/// @param[in] u : Le vecteur � copier dans le nouveau vecteur.
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
	/// Constructeur par param�tres.
	///
	/// @param[in] x : La coordonn�e en @a x.
	/// @param[in] y : La coordonn�e en @a y.
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
	/// @param[in] v : Le vecteur � copier dans le nouveau vecteur.
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
	/// Surcharge de l'op�rateur d'assignation.
	///
	/// @param[in] v : Le vecteur � copier.
	///
	/// @return La r�f�rence vers l'objet assign� afin de respecter la
	///         s�mantique de l'op�rateur d'assignation C++.
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
	/// Cette fonction calcule l'angle du vecteur par rapport � l'axe des X
	/// positifs dans le sens anti-horaire.
	///
	/// @return L'angle du vecteur par rapport � l'axe des X dans le sens
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
	/// Cette fonction retourne un nouveau vecteur ayant effectu� une rotation
	/// d'un angle donn� dans le sens anti-horaire.
	///
	/// @param[in] angle : L'angle duquel faire tourner le vecteur.
	///
	/// @return Le vecteur apr�s la rotation.
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
	/// Cette fonction retourne un nouveau vecteur ayant effectu� une rotation
	/// d'un angle de Pi sur 2 dans le sens anti-horaire.
	///
	/// @return Le vecteur apr�s la rotation.
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
	/// Cette fonction retourne un nouveau vecteur ayant effectu� une rotation
	/// d'un angle de Pi sur 2 dans le sens horaire.
	///
	/// @return Le vecteur apr�s la rotation.
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
///        Classe g�n�rique pour implanter des vecteurs en 3D.
///
///        Cette classe g�n�rique est d�finie pour les vecteurs de dimension 3.
///        Les op�rations n�cessaires sont d�finies, soient:
///        - les constructeurs ;
///        - op�rateur = ;
///        - calcul de l'angle de rotation ;
///        - calcul de l'angle de �l�vation ;
///        - transformation du vecteur en coordon�es sph�riques ;
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
	/// @param[in] u : Le vecteur � copier dans le nouveau vecteur.
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
	/// Constructeur par param�tres.
	///
	/// @param[in] x : La coordonn�e en @a x.
	/// @param[in] y : La coordonn�e en @a y.
	/// @param[in] z : La coordonn�e en @a z.
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
	/// Constructeur � partir de deux points.  Le vecteur construit est dirig�
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
	/// @param[in] v : Le vecteur � copier dans le nouveau vecteur.
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
	/// Surcharge de l'op�rateur d'assignation.
	///
	/// @param[in] v : Le vecteur � copier.
	///
	/// @return La r�f�rence vers l'objet assign� afin de respecter la
	///         s�mantique de l'op�rateur d'assignation C++.
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
	/// Cette fonction calcule l'angle d'�l�vation du vecteur 3D.  Le vecteur
	/// dirig� vers l'axe des Z positifs, k, (0,0,1), aura comme angle 0.
	///
	/// @return L'angle d'�l�vation du vecteur 3D.
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
	/// Cette fonction retourne les coordonn�es sph�riques du vecteur via les
	/// param�tres pass�s en r�f�rence.
	///
	/// @param[out] r     : Le rayon des coordonn�es sph�riques.
	/// @param[out] phi   : L'angle d'�l�vation des coordonn�es sph�riques.
	/// @param[out] theta : L'angle de rotation des coordonn�es sph�riques.
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
	/// Cette fonction assigne les coordonn�es du vecteur qui sont donn�es en
	/// coordonn�es sph�riques.
	///
	/// @param[in] r     : Le rayon des coordonn�es sph�riques.
	/// @param[in] phi   : L'angle d'�l�vation des coordonn�es sph�riques.
	/// @param[in] theta : L'angle de rotation des coordonn�es sph�riques.
	///
	/// @return Aucune.
	///
	///////////////////////////////////////////////////////////////////////////////
	inline void assignerCoordSpheriques ( const T r, const T phi, const T theta )
	{
		// Calcul de la position � partir des coordonn�es sph�riques
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
	/// @return Le vecteur apr�s la rotation.
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
///        Classe g�n�rique pour implanter des vecteurs en 4D.
///
///        Cette classe g�n�rique est d�finie pour les vecteurs de dimension 4.
///        Les op�rations n�cessaires sont d�finies, soient:
///        - les constucteurs
///        - op�rateur d'�galit�
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
	/// @param[in] u : Le vecteur � copier dans le nouveau vecteur.
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
	/// Constructeur par param�tres.
	///
	/// @param[in] x : La coordonn�e en @a x.
	/// @param[in] y : La coordonn�e en @a y.
	/// @param[in] z : La coordonn�e en @a z.
	/// @param[in] w : La coordonn�e en @a w.
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
	/// @param[in] v : Le vecteur � copier dans le nouveau vecteur.
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
	/// Surcharge de l'op�rateur d'assignation.
	///
	/// @param[in] v : Le vecteur � copier.
	///
	/// @return La r�f�rence vers l'objet assign� afin de respecter la
	///         s�mantique de l'op�rateur d'assignation C++.
	///
	////////////////////////////////////////////////////////////////////////////
	inline const Vecteur4D& operator= ( const Vecteur<T, 4>& v )
	{
		Vecteur<T, 4>::operator= ( v );
		return *this;
	}


};




/// D�finition de type de vecteur � 2 coordonn�es de type double.
typedef Vecteur2D<double> Vecteur2;
/// D�finition de type de vecteur � 3 coordonn�es de type double.
typedef Vecteur3D<double> Vecteur3;
/// D�finition de type de vecteur � 4 coordonn�es de type double.
typedef Vecteur4D<double> Vecteur4;

/// D�finition de type de vecteur � 2 coordonn�es de type float.
typedef Vecteur2D<float> Vecteur2f;
/// D�finition de type de vecteur � 3 coordonn�es de type float.
typedef Vecteur3D<float> Vecteur3f;
/// D�finition de type de vecteur � 4 coordonn�es de type float.
typedef Vecteur4D<float> Vecteur4f;

/// D�finition de type de vecteur � 2 coordonn�es de type int.
typedef Vecteur2D<int> Vecteur2i;
/// D�finition de type de vecteur � 3 coordonn�es de type int.
typedef Vecteur3D<int> Vecteur3i;
/// D�finition de type de vecteur � 4 coordonn�es de type int.
typedef Vecteur4D<int> Vecteur4i;



enum vecteurPos{VX,VY,VZ,VW};

#endif // __UTILITAIRE_VECTEUR_H__


///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////
