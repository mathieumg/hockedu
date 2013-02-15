#include "XMLUtils.h"
#ifdef WIN32
#include "tinyxml\include\tinyxml.h"
#else
// mac librairie
#endif
#include "Vecteur.h"
#include <sstream>
#include <string>


namespace XMLUtils
{

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void utilitaire::ecrireVecteur3Dxml( Vecteur* vecteur, XmlElement* element, const std::string* nom )
    ///
    /// Permet d'ecrire un vecteur dans un element XML
    ///
    /// @param[in] Vecteur3D<float> * vecteur : vecteur à écrire dans le noeud xml
    /// @param[in] XmlElement * element : noeud xml
    /// @param[in] const char* name : nom de ce vecteur a etre ecrit dans le fichier XML, s'assurer qu'il soit unique
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    void ecrireVecteur3Dxml( const Vecteur3* vecteur, XmlElement* element, const char* name )
    {
        if(vecteur && element)
        {
            for ( int i = 0; i < 3; i++ ) 
            {
                std::ostringstream nameAttribute;
                nameAttribute << name;
                nameAttribute << i;
                element->SetDoubleAttribute(nameAttribute.str().c_str(),vecteur->operator[](i));
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void utilitaire::lectureVecteur3Dxml( Vecteur3D<float>* vecteur, const XmlElement* element, const std::string& nom )
    ///
    /// Permet de lire un element XML dans un vecteur 
    ///
    /// @param[in] Vecteur3D<float> * vecteur : vecteur à écrire dans le noeud xml
    /// @param[in] XmlElement * element : noeud xml
    /// @param[in] const char* name : nom de ce vecteur a etre ecrit dans le fichier XML, s'assurer qu'il soit unique
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    bool lectureVecteur3Dxml( Vecteur3* vecteur, const XmlElement* element, const char* name )
    {
        if(vecteur && element)
        {
            for ( int i = 0; i < 3; i++ ) 
            {
                float item;
                std::ostringstream nameAttribute;
                nameAttribute << name;
                nameAttribute << i;
                if( element->QueryFloatAttribute(nameAttribute.str().c_str(),&item) != TIXML_SUCCESS)
                    return false;
                vecteur->operator[](i) = item;
            }
            return true;
        }
        return false;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn ecrireAttribute<double>( XmlElement* element, const double* attribute )
    ///
    /// écrit un attribut de type float dans le noeud xml
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    /// 
    /// @param[in] XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à écrire
    /// @param[in] const float & attribute : float attribute to write
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    void ecrireAttribute<double>( XmlElement* element, const char* name, const double& attribute )
    {
        element->SetDoubleAttribute(name,attribute);
    }

	////////////////////////////////////////////////////////////////////////
    ///
    /// @fn ecrireAttribute<float>( XmlElement* element, const float* attribute )
    ///
    /// écrit un attribut de type float dans le noeud xml
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    /// 
    /// @param[in] XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à écrire
    /// @param[in] const float & attribute : float attribute to write
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    void ecrireAttribute<float>( XmlElement* element, const char* name, const float& attribute )
    {
		double buffer = (double)attribute;
        element->SetDoubleAttribute(name,buffer);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn ecrireAttribute
    ///
    /// écrit un attribut de type float dans le noeud xml
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à écrire
    /// @param[in] const int& attribute : float attribute to write
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    void ecrireAttribute<int>( XmlElement* element, const char* name, const int& attribute )
    {
        element->SetAttribute(name,attribute);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn ecrireAttribute
    ///
    /// écrit un attribut de type float dans le noeud xml
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à écrire
    /// @param[in] const string& attribute : float attribute to write
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    void ecrireAttribute<std::string>( XmlElement* element, const char* name, const std::string& attribute )
    {
        element->SetAttribute(name,attribute.c_str());
    }


    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn bool utilitaire::LireAttribute<double>( const XmlElement* element, const char* name, double* attribute )
    ///
    /// Permet la lecture d'un noeud xml dans un attribut de type float
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] const XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à lire
    /// @param[in] double * attribute : float attribute to read
    ///
    /// @return bool : retourne vrai si la lecture a réussi 
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    bool LireAttribute<double>( const XmlElement* element, const char* name, double& attribute )
    {
        return element->QueryDoubleAttribute(name, &attribute) == TIXML_SUCCESS;
    }

	////////////////////////////////////////////////////////////////////////
    ///
    /// @fn bool utilitaire::LireAttribute<float>( const XmlElement* element, const char* name, double* attribute )
    ///
    /// Permet la lecture d'un noeud xml dans un attribut de type float
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] const XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à lire
    /// @param[in] double * attribute : float attribute to read
    ///
    /// @return bool : retourne vrai si la lecture a réussi 
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    bool LireAttribute<float>( const XmlElement* element, const char* name, float& attribute )
    {
        return element->QueryFloatAttribute(name, &attribute) == TIXML_SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn bool utilitaire::LireAttribute<int>( const XmlElement* element, const char* name, int* attribute )
    ///
    /// Permet la lecture d'un noeud xml dans un attribut de type int
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] const XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à lire
    /// @param[in] int * attribute : float attribute to read
    ///
    /// @return bool : retourne vrai si la lecture a réussi 
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    bool LireAttribute<int>( const XmlElement* element, const char* name, int& attribute )
    {
        return element->QueryIntAttribute(name, &attribute) == TIXML_SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn bool utilitaire::LireAttribute<int>( const XmlElement* element, const char* name, int* attribute )
    ///
    /// Permet la lecture d'un noeud xml dans un attribut de type int
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] const XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à lire
    /// @param[in] int * attribute : float attribute to read
    ///
    /// @return bool : retourne vrai si la lecture a réussi 
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    bool LireAttribute<std::string>( const XmlElement* element, const char* name, std::string& attribute )
    {
        const char* val = element->Attribute(name);
        if(val)
        {
            attribute = val;
        }
        return !!val;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlElement* creerNoeud( const char* name )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * name
    ///
    /// @return XmlElement*
    ///
    ////////////////////////////////////////////////////////////////////////
    XmlElement* creerNoeud( const char* name )
    {
        return new XmlElement(name);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlText* creerNoeudText( const char* name )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * name
    ///
    /// @return XmlText*
    ///
    ////////////////////////////////////////////////////////////////////////
    XmlText* creerNoeudText( const char* name )
    {
        return new XmlText(name);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void LinkEndChild( TiXmlNode* parent, TiXmlNode* child )
    ///
    /// /*Description*/
    ///
    /// @param[in] TiXmlNode * parent
    /// @param[in] TiXmlNode * child
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    void LinkEndChild( XmlElement* parent, XmlElement* child )
    {
        parent->LinkEndChild(child);
    }

}