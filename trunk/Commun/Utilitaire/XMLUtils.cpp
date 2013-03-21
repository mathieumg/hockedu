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
#if WIN32 || __APPLE__
    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn const char* MakeName( const char* name, int index )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * name
    /// @param[in] int index
    ///
    /// @return const char*
    ///
    ////////////////////////////////////////////////////////////////////////
    const char* MakeName( const char* name, int index )
    {
        char* createdName = new char[64];
        if(sprintf(createdName,"%s%d",name,index) == -1)
        {
            
            delete[] createdName;
            return NULL;
        }
        return createdName;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void FreeName( const char* createdName )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * createdName
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    void FreeName( const char* createdName )
    {
        delete[] createdName;
    }



    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn writeAttribute<double>( XmlElement* element, const double* attribute )
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
    void writeAttribute<double>( XmlElement* element, const char* name, const double& attribute )
    {
        element->SetDoubleAttribute(name,attribute);
    }

	////////////////////////////////////////////////////////////////////////
    ///
    /// @fn writeAttribute<float>( XmlElement* element, const float* attribute )
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
    void writeAttribute<float>( XmlElement* element, const char* name, const float& attribute )
    {
		double buffer = (double)attribute;
        element->SetDoubleAttribute(name,buffer);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn writeAttribute
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
    void writeAttribute<int>( XmlElement* element, const char* name, const int& attribute )
    {
        element->SetAttribute(name,attribute);
    }
    template<>
    void writeAttribute<bool>( XmlElement* element, const char* name, const bool& attribute )
    {
        element->SetAttribute(name,attribute);
    }
    template<>
    void writeAttribute<char>( XmlElement* element, const char* name, const char& attribute )
    {
        element->SetAttribute(name,attribute);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn writeAttribute
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
    void writeAttribute<std::string>( XmlElement* element, const char* name, const std::string& attribute )
    {
        element->SetAttribute(name,attribute.c_str());
    }


    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn bool utilitaire::readAttribute<double>( const XmlElement* element, const char* name, double* attribute )
    ///
    /// Permet la lecture d'un noeud xml dans un attribut de type float
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] const XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à lire
    /// @param[in] double * attribute : double attribute to read
    ///
    /// @return bool : retourne vrai si la lecture a réussi 
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    bool readAttribute<double>( const XmlElement* element, const char* name, double& attribute )
    {
        return element->QueryDoubleAttribute(name, &attribute) == TIXML_SUCCESS;
    }

	////////////////////////////////////////////////////////////////////////
    ///
    /// @fn bool utilitaire::readAttribute<float>( const XmlElement* element, const char* name, double* attribute )
    ///
    /// Permet la lecture d'un noeud xml dans un attribut de type float
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] const XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à lire
    /// @param[in] float * attribute : float attribute to read
    ///
    /// @return bool : retourne vrai si la lecture a réussi 
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    bool readAttribute<float>( const XmlElement* element, const char* name, float& attribute )
    {
        return element->QueryFloatAttribute(name, &attribute) == TIXML_SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn bool utilitaire::readAttribute<int>( const XmlElement* element, const char* name, int* attribute )
    ///
    /// Permet la lecture d'un noeud xml dans un attribut de type int
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] const XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à lire
    /// @param[in] int * attribute : int attribute to read
    ///
    /// @return bool : retourne vrai si la lecture a réussi 
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    bool readAttribute<int>( const XmlElement* element, const char* name, int& attribute )
    {
        return element->QueryIntAttribute(name, &attribute) == TIXML_SUCCESS;
    }
    template<>
    bool readAttribute<char>( const XmlElement* element, const char* name, char& attribute )
    {
        int temp;
        auto res = element->QueryIntAttribute(name, &temp);
        attribute = (char)temp;
        return res == TIXML_SUCCESS;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn bool utilitaire::readAttribute<int>( const XmlElement* element, const char* name, int* attribute )
    ///
    /// Permet la lecture d'un noeud xml dans un attribut de type int
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] const XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à lire
    /// @param[in] string * attribute : string attribute to read
    ///
    /// @return bool : retourne vrai si la lecture a réussi 
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    bool readAttribute<std::string>( const XmlElement* element, const char* name, std::string& attribute )
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
    /// @fn bool XMLUtils<>::readAttribute( const XmlElement* element, const char* name, const char*& attribute )
    ///
    /// Permet la lecture d'un noeud xml dans un attribut de type int
    /// Tous les pointeurs doivent être validé avant les appels à cette fonction
    ///
    /// @param[in] const XmlElement * element : make sure to valid the pointer before calling
    /// @param[in] const char * name : nom du nom à lire
    /// @param[in] const char * & attribute : string attribute to read
    ///
    /// @return bool
    ///
    ////////////////////////////////////////////////////////////////////////
    template<>
    bool readAttribute<const char*>( const XmlElement* element, const char* name, const char*& attribute )
    {
        attribute = element->Attribute(name);
        return !!attribute;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlElement* createNode( const char* name )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * name
    ///
    /// @return XmlElement*
    ///
    ////////////////////////////////////////////////////////////////////////
    XmlElement* createNode( const char* name )
    {
        return new XmlElement(name);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlText* createTextNode( const char* name )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * name
    ///
    /// @return XmlText*
    ///
    ////////////////////////////////////////////////////////////////////////
    XmlText* createTextNode( const char* name )
    {
        return new XmlText(name);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void LinkEndChild( XmlNode* parent, XmlNode* child )
    ///
    /// /*Description*/
    ///
    /// @param[in] XmlNode * parent
    /// @param[in] XmlNode * child
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    void LinkEndChild( XmlNode* parent, XmlNode* child )
    {
        parent->LinkEndChild(child);
    }

    void LinkEndChild(XmlDocument& parent, XmlNode* child)
    {
        LinkEndChild(parent.mNode,child);
    }


    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlElement* FirstChildElement( XmlElement* element, const char* childName )
    ///
    /// /*Description*/
    ///
    /// @param[in] XmlElement * element
    /// @param[in] const char * childName
    ///
    /// @return XmlElement*
    ///
    ////////////////////////////////////////////////////////////////////////
    const XmlElement* FirstChildElement( const XmlElement* element, const char* childName )
    {
        if(childName)return element->FirstChildElement(childName);
        return element->FirstChildElement();
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlElement* FirstChildElement( XmlElement* element, const char* childName )
    ///
    /// /*Description*/
    ///
    /// @param[in] XmlElement * element
    /// @param[in] const char * childName
    ///
    /// @return XmlElement*
    ///
    ////////////////////////////////////////////////////////////////////////
    XmlElement* FirstChildElement( XmlElement* element, const char* childName )
    {
        return const_cast<XmlElement*>(FirstChildElement((const XmlElement*)element,childName));
    }
    const XmlElement* FirstChildElement( const XmlDocument& document, const char* childName)
    {
        return FirstChildElement(document.GetElem(),childName);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlDocument LoadDocument( const char* fileName )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * fileName
    ///
    /// @return XmlDocument
    ///
    ////////////////////////////////////////////////////////////////////////
    bool LoadDocument( XmlDocument& document, const char* fileName )
    {
        return document.mNode->LoadFile ( fileName );
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlDocument CreateDocument( const char* _version,const char* _encoding,const char* _standalone )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * _version
    /// @param[in] const char * _encoding
    /// @param[in] const char * _standalone
    ///
    /// @return XmlDocument
    ///
    ////////////////////////////////////////////////////////////////////////
    void CreateDocument( XmlDocument& document, const char* _version,const char* _encoding,const char* _standalone )
    {
        // Écrire la déclaration XML standard...
        TiXmlDeclaration* declaration = new TiXmlDeclaration( _version, _encoding, _standalone );
        LinkEndChild(document,declaration);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void SaveDocument( XmlDocument document, const char* fileName )
    ///
    /// /*Description*/
    ///
    /// @param[in] XmlDocument * document
    /// @param[in] const char * fileName
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    bool SaveDocument( XmlDocument& document, const char* fileName )
    {
        return document.mNode->SaveFile(fileName);
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn const XmlElement* FirstChild( const XmlElement* element )
    ///
    /// /*Description*/
    ///
    /// @param[in] const XmlElement * element
    ///
    /// @return const XmlElement*
    ///
    ////////////////////////////////////////////////////////////////////////
    const XmlNode* FirstChild( const XmlElement* element, const char* childName )
    {
        if(childName) return (const XmlNode*)element->FirstChild(childName);
        return (const XmlNode*)element->FirstChild();
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlElement* FirstChild( XmlElement* element )
    ///
    /// /*Description*/
    ///
    /// @param[in] XmlElement * element
    ///
    /// @return XmlElement*
    ///
    ////////////////////////////////////////////////////////////////////////
    XmlNode* FirstChild( XmlElement* element, const char* childName )
    {
        if(childName) return (XmlNode*)element->FirstChild(childName);
        return (XmlNode*)element->FirstChild();
    }


    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn const XmlElement* NextSibling( const XmlElement* child )
    ///
    /// retrieves the sibling of the current node
    ///
    /// @param[in] const XmlElement * child
    ///
    /// @return const XmlElement*
    ///
    ////////////////////////////////////////////////////////////////////////
    const XmlElement* NextSibling( const XmlElement* child )
    {
        return child->NextSiblingElement();
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn const char* GetNodeTag( const XmlElement* element )
    ///
    /// retrieve the text for this node's tag
    ///
    /// @param[in] const XmlElement * element
    ///
    /// @return const char*
    ///
    ////////////////////////////////////////////////////////////////////////
    const char* GetNodeTag( const XmlElement* element )
    {
        return element->Value();
    }
    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn const char* GetVersion( XmlDocument document )
    ///
    /// Retrieves the version from the document
    ///
    /// @param[in] XmlDocument document
    ///
    /// @return const char*
    ///
    ////////////////////////////////////////////////////////////////////////
    const char* GetVersion( XmlDocument& document )
    {
        XmlNode* n = document.mNode->FirstChild();
        if(n)
        {
            TiXmlDeclaration* dec = n->ToDeclaration();
            if(dec)
            {
                return dec->Version();
            }
        }
        return NULL;
    }

#else
////////////////////////////////////////////////////////////////////////
///
/// @fn writeAttribute<double>( XmlElement* element, const double* attribute )
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
void writeAttribute<double>( XmlElement* element, const char* name, const double& attribute )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn writeAttribute<float>( XmlElement* element, const float* attribute )
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
void writeAttribute<float>( XmlElement* element, const char* name, const float& attribute )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn writeAttribute
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
void writeAttribute<int>( XmlElement* element, const char* name, const int& attribute )
{
}
template<>
void writeAttribute<bool>( XmlElement* element, const char* name, const bool& attribute )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn writeAttribute
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
void writeAttribute<std::string>( XmlElement* element, const char* name, const std::string& attribute )
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn bool utilitaire::readAttribute<double>( const XmlElement* element, const char* name, double* attribute )
///
/// Permet la lecture d'un noeud xml dans un attribut de type float
/// Tous les pointeurs doivent être validé avant les appels à cette fonction
///
/// @param[in] const XmlElement * element : make sure to valid the pointer before calling
/// @param[in] const char * name : nom du nom à lire
/// @param[in] double * attribute : double attribute to read
///
/// @return bool : retourne vrai si la lecture a réussi 
///
////////////////////////////////////////////////////////////////////////
template<>
bool readAttribute<double>( const XmlElement* element, const char* name, double& attribute )
{
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool utilitaire::readAttribute<float>( const XmlElement* element, const char* name, double* attribute )
///
/// Permet la lecture d'un noeud xml dans un attribut de type float
/// Tous les pointeurs doivent être validé avant les appels à cette fonction
///
/// @param[in] const XmlElement * element : make sure to valid the pointer before calling
/// @param[in] const char * name : nom du nom à lire
/// @param[in] float * attribute : float attribute to read
///
/// @return bool : retourne vrai si la lecture a réussi 
///
////////////////////////////////////////////////////////////////////////
template<>
bool readAttribute<float>( const XmlElement* element, const char* name, float& attribute )
{
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool utilitaire::readAttribute<int>( const XmlElement* element, const char* name, int* attribute )
///
/// Permet la lecture d'un noeud xml dans un attribut de type int
/// Tous les pointeurs doivent être validé avant les appels à cette fonction
///
/// @param[in] const XmlElement * element : make sure to valid the pointer before calling
/// @param[in] const char * name : nom du nom à lire
/// @param[in] int * attribute : int attribute to read
///
/// @return bool : retourne vrai si la lecture a réussi 
///
////////////////////////////////////////////////////////////////////////
template<>
bool readAttribute<int>( const XmlElement* element, const char* name, int& attribute )
{
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool utilitaire::readAttribute<int>( const XmlElement* element, const char* name, int* attribute )
///
/// Permet la lecture d'un noeud xml dans un attribut de type int
/// Tous les pointeurs doivent être validé avant les appels à cette fonction
///
/// @param[in] const XmlElement * element : make sure to valid the pointer before calling
/// @param[in] const char * name : nom du nom à lire
/// @param[in] string * attribute : string attribute to read
///
/// @return bool : retourne vrai si la lecture a réussi 
///
////////////////////////////////////////////////////////////////////////
template<>
bool readAttribute<std::string>( const XmlElement* element, const char* name, std::string& attribute )
{
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn bool XMLUtils<>::readAttribute( const XmlElement* element, const char* name, const char*& attribute )
///
/// Permet la lecture d'un noeud xml dans un attribut de type int
/// Tous les pointeurs doivent être validé avant les appels à cette fonction
///
/// @param[in] const XmlElement * element : make sure to valid the pointer before calling
/// @param[in] const char * name : nom du nom à lire
/// @param[in] const char * & attribute : string attribute to read
///
/// @return bool
///
////////////////////////////////////////////////////////////////////////
template<>
bool readAttribute<const char*>( const XmlElement* element, const char* name, const char*& attribute )
{
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* createNode( const char* name )
///
/// /*Description*/
///
/// @param[in] const char * name
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* createNode( const char* name )
{
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlText* createTextNode( const char* name )
///
/// /*Description*/
///
/// @param[in] const char * name
///
/// @return XmlText*
///
////////////////////////////////////////////////////////////////////////
XmlText* createTextNode( const char* name )
{
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void LinkEndChild( XmlNode* parent, XmlNode* child )
///
/// /*Description*/
///
/// @param[in] XmlNode * parent
/// @param[in] XmlNode * child
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
void LinkEndChild( XmlNode* parent, XmlNode* child )
{
}

void LinkEndChild(XmlDocument& parent, XmlNode* child)
{
}


////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* FirstChildElement( XmlElement* element, const char* childName )
///
/// /*Description*/
///
/// @param[in] XmlElement * element
/// @param[in] const char * childName
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
const XmlElement* FirstChildElement( const XmlElement* element, const char* childName )
{
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* FirstChildElement( XmlElement* element, const char* childName )
///
/// /*Description*/
///
/// @param[in] XmlElement * element
/// @param[in] const char * childName
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlElement* FirstChildElement( XmlElement* element, const char* childName )
{
    return NULL;
}
const XmlElement* FirstChildElement( const XmlDocument& document, const char* childName)
{
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlDocument LoadDocument( const char* fileName )
///
/// /*Description*/
///
/// @param[in] const char * fileName
///
/// @return XmlDocument
///
////////////////////////////////////////////////////////////////////////
bool LoadDocument( XmlDocument& document, const char* fileName )
{
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlDocument CreateDocument( const char* _version,const char* _encoding,const char* _standalone )
///
/// /*Description*/
///
/// @param[in] const char * _version
/// @param[in] const char * _encoding
/// @param[in] const char * _standalone
///
/// @return XmlDocument
///
////////////////////////////////////////////////////////////////////////
void CreateDocument( XmlDocument& document, const char* _version,const char* _encoding,const char* _standalone )
{
}

////////////////////////////////////////////////////////////////////////
///
/// @fn void SaveDocument( XmlDocument document, const char* fileName )
///
/// /*Description*/
///
/// @param[in] XmlDocument * document
/// @param[in] const char * fileName
///
/// @return void
///
////////////////////////////////////////////////////////////////////////
bool SaveDocument( XmlDocument& document, const char* fileName )
{
    return false;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const XmlElement* FirstChild( const XmlElement* element )
///
/// /*Description*/
///
/// @param[in] const XmlElement * element
///
/// @return const XmlElement*
///
////////////////////////////////////////////////////////////////////////
const XmlNode* FirstChild( const XmlElement* element, const char* childName )
{
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn XmlElement* FirstChild( XmlElement* element )
///
/// /*Description*/
///
/// @param[in] XmlElement * element
///
/// @return XmlElement*
///
////////////////////////////////////////////////////////////////////////
XmlNode* FirstChild( XmlElement* element, const char* childName )
{
    return NULL;
}


////////////////////////////////////////////////////////////////////////
///
/// @fn const XmlElement* NextSibling( const XmlElement* child )
///
/// retrieves the sibling of the current node
///
/// @param[in] const XmlElement * child
///
/// @return const XmlElement*
///
////////////////////////////////////////////////////////////////////////
const XmlElement* NextSibling( const XmlElement* child )
{
    return NULL;
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const char* GetNodeTag( const XmlElement* element )
///
/// retrieve the text for this node's tag
///
/// @param[in] const XmlElement * element
///
/// @return const char*
///
////////////////////////////////////////////////////////////////////////
const char* GetNodeTag( const XmlElement* element )
{
    return "";
}

////////////////////////////////////////////////////////////////////////
///
/// @fn const char* GetVersion( XmlDocument document )
///
/// Retrieves the version from the document
///
/// @param[in] XmlDocument document
///
/// @return const char*
///
////////////////////////////////////////////////////////////////////////
const char* GetVersion( XmlDocument& document )
{
    return NULL;
}

    const char* MakeName( const char* name, int index )
    {
        return NULL;
    }
    
    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void FreeName( const char* createdName )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * createdName
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    void FreeName( const char* createdName )
    {
        
    }

#endif
    
}