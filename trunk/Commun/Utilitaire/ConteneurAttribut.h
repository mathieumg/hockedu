///////////////////////////////////////////////////////////////////////////////
/// @file ConteneurAttribut.h
/// @author Michael Ferris
/// @date 2012-02-15
/// @version 1.0
///
/// @addtogroup utilitaire Utilitaire
/// @{
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <ctime>

/// Type d'attribut géré par le conteneur
enum TypeAttribut{INT_TYPE,BOOL_TYPE,DOUBLE_TYPE,STRING_TYPE};

///////////////////////////////////////////////////////////////////////////
/// @class ConteneurAttribut
/// @brief Conteneur d'attribut pour faciliter les modifications 
///			d'attributs ayant un type non connu d'avance
///
/// @author Michael Ferris
/// @date 2012-02-15
///////////////////////////////////////////////////////////////////////////
class ConteneurAttribut
{
public:
	ConteneurAttribut(void);
	~ConteneurAttribut(void);

	bool boolElem;
	int intElem;
	double doubleElem;
	std::string stringElem;

	/// Affecte une valeur aléatoire à un attribut
	void randomizeAttribut(const TypeAttribut& type);


	/// Surcharge de l'opérateur== avec un booléen
	inline bool operator==(const bool& elem)
	{
		return boolElem == elem;
	}
	/// Surcharge de l'opérateur== avec un int
	inline bool operator==(const int& elem)
	{
		return intElem == elem;
	}
	/// Surcharge de l'opérateur== avec un float
	inline bool operator==(const float& elem)
	{
		// Permet de comparer 2 float jusqu'à une précision de 5 décimal
		return (long)(doubleElem*10000) == (long)(elem*10000);
	}
	/// Surcharge de l'opérateur== avec un string
	inline bool operator==(const std::string& elem)
	{
		return stringElem == elem;
	}
	/// Surcharge de l'opérateur!= avec un booléen
	inline bool operator!=(const bool& elem)
	{
		return boolElem != elem;
	}
	/// Surcharge de l'opérateur!= avec un int
	inline bool operator!=(const int& elem)
	{
		return intElem != elem;
	}
	/// Surcharge de l'opérateur!= avec un float
	inline bool operator!=(const float& elem)
	{
		// Permet de comparer 2 float jusqu'à une précision de 5 décimal
		return (long)(doubleElem*10000) != (long)(elem*10000);
	}
	/// Surcharge de l'opérateur!= avec un strin
	inline bool operator!=(const std::string& elem)
	{
		return stringElem != elem;
	}

	/// Comparaison entre 2 tyoe concret de 2 conteneurs
	bool estEgal(const ConteneurAttribut& ca, const TypeAttribut& type)
	{
		switch(type)
		{
		case INT_TYPE: return intElem == ca.intElem;
		case BOOL_TYPE: return boolElem == ca.boolElem;
		case DOUBLE_TYPE: 
			// Permet de comparer 2 float jusqu'à une précision de 5 décimal
			return (long)(doubleElem*10000) == (long)(ca.doubleElem*10000);
		case STRING_TYPE: return stringElem == ca.stringElem;
		default: throw ("wtf");
		}
	}

	/// Modification du booléen du conteneur
	void modifier( const bool& Elem)
	{
		boolElem = Elem;
	}
	/// Modification du int du conteneur
	void modifier( const int& Elem)
	{
		intElem = Elem;
	}
	/// Modification du float du conteneur
	void modifier( const float& Elem)
	{
		doubleElem = Elem;
	}
	/// Modification du string du conteneur
	void modifier( const std::string& Elem)
	{
		stringElem = Elem;
	}
	/// Accesseur au booléen du conteneur
	void obtenir(bool& Elem)
	{
		Elem = boolElem;
	}
	/// Accesseur au int du conteneur
	void obtenir(int& Elem)
	{
		Elem = intElem;
	}
	/// Accesseur au float du conteneur
	void obtenir(double& Elem)
	{
		Elem = doubleElem;
	}
	/// Accesseur au string du conteneur
	void obtenir(std::string& Elem)
	{
		Elem = stringElem;
	}

};

