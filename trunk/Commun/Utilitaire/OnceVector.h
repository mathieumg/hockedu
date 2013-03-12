///////////////////////////////////////////////////////////////////////////////
/// @file OnceVector.h
/// @author Michael Ferris
/// @date 2011-05-14
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include <vector>
#include <algorithm>

using namespace std;


///////////////////////////////////////////////////////////////////////////
/// @class OnceVector
/// @brief Vecteur qui ne peut contenir deux fois le m�me �l�ment.
///
/// @author Michael Ferris
/// @date 2011-05-14
///////////////////////////////////////////////////////////////////////////
template<typename T>
class OnceVector
{
public:
	/// Constructeur par d�faut
	OnceVector(int size=0):vec(0){};
	/// Desructeur vide
	~OnceVector(){};

	/////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	bool existe(const T& elem)
	///
	/// V�rifie l'existence d'un �l�ment dans le vecteur.
	///
	/// @param[in] const T& elem : L'�l�ment recherch�.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	bool existe(const T& elem)
	{
		return find(vec.begin(),vec.end(),elem) != vec.end();
	}

	/////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	void push_back(const T& elem)
	///
	/// Ajouter un �l�ment dans le vecteur.
	///
	/// @param[in] const T& elem : L'�l�ment � ajouter.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void push_back(const T& elem)
	{
		if(!existe(elem))
			vec.push_back(elem);
	}

	/////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	void erase(const T& elem)
	///
	/// Retirer un �l�ment du vecteur.
	///
	/// @param[in] const T& elem : L'�l�ment � retirer.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void erase(const T& elem)
	{
		auto pos = find(vec.begin(),vec.end(),elem);
		if(pos != vec.end())
		{
			vec.erase(pos);
		}
	}

	/////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	unsigned int size()
	///
	/// Accesseur � la taille du vecteur.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	unsigned int size() const
	{
		return (unsigned int)vec.size();
	}

	/////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	T& operator[](int i)
	///
	/// Op�rateur [] pour acc�der � un �l�ment du vecteur.
	///
	/// @param[in] int i : L'index de l'�l�ment.
	///
	/// @return T& : l'�l�ment en question
	///
	////////////////////////////////////////////////////////////////////////
	T& operator[](int i)
	{
		return vec[i];
	}

	////////////////////////////////////////////////////////////////////////
	///
	/// @fn T& OnceVector<T>::get( int i )
	///
	/// methode pour acc�der � un �l�ment, pareil que l'operateur []
	///
	/// @param[in] int i : L'index de l'�l�ment.
	///
	/// @return T& : l'�l�ment en question
	///
	////////////////////////////////////////////////////////////////////////
	const T& get(int i) const
	{
		return vec[i];
	}

	/////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	void clear()
	///
	/// Vider le vecteur.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void clear()
	{
		vec.clear();
	}

	/////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	void reserve(int i)
	///
	/// Appelle la m�thode reserve() du vecteur.
	///
	/// @param[in] int i : L'index � r�server.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void reserve(int i)
	{
		vec.reserve(i);
	}

	/////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	void resize(int i)
	///
	/// Redimensionner le vecteur.
	///
	/// @param[in] int i : La taille du redimensionnement.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void resize(int i)
	{
		vec.resize(i);
	}

	/////////////////////////////////////////////////////////////////////////
	///
	/// @fn 	void insert(const T& elem)
	///
	/// Ins�rer un �l�ment dans le vecteur.
	///
	/// @param[in] const T& elem : L'�l�ment � ins�rer.
	///
	/// @return Aucune.
	///
	////////////////////////////////////////////////////////////////////////
	void insert(const T& elem)
	{
		push_back(elem);
	}

private:
	/// Le vecteur
	vector<T> vec;
};

///////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////






