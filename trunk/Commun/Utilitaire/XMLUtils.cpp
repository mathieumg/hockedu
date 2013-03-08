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
#if WIN32
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
    void LinkEndChild( XmlElement* parent, XmlElement* child )
    {
        parent->LinkEndChild(child);
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
        if(sprintf_s(createdName,64,"%s%d",name,index) == -1)
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
    /// @fn XmlDocument* LoadDocument( const char* fileName )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * fileName
    ///
    /// @return XmlDocument*
    ///
    ////////////////////////////////////////////////////////////////////////
    XmlDocument* LoadDocument( const char* fileName )
    {
        XmlDocument* document = new XmlDocument();
        if(document->LoadFile ( fileName ))
            return document;

        delete document;
        return NULL;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void FreeDocument( XmlDocument* document )
    ///
    /// /*Description*/
    ///
    /// @param[in] XmlDocument * document
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    void FreeDocument( XmlDocument* document )
    {
        if(document)
        {
            delete document;
        }
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn XmlDocument* CreateDocument( const char* _version,const char* _encoding,const char* _standalone )
    ///
    /// /*Description*/
    ///
    /// @param[in] const char * _version
    /// @param[in] const char * _encoding
    /// @param[in] const char * _standalone
    ///
    /// @return XmlDocument*
    ///
    ////////////////////////////////////////////////////////////////////////
    XmlDocument* CreateDocument( const char* _version,const char* _encoding,const char* _standalone )
    {
        XmlDocument* document = new XmlDocument();

        // Écrire la déclaration XML standard...
        TiXmlDeclaration* declaration = new TiXmlDeclaration( _version, _encoding, _standalone );
        document->LinkEndChild(declaration);

        return document;
    }

    ////////////////////////////////////////////////////////////////////////
    ///
    /// @fn void SaveDocument( XmlDocument* document, const char* fileName )
    ///
    /// /*Description*/
    ///
    /// @param[in] XmlDocument * document
    /// @param[in] const char * fileName
    ///
    /// @return void
    ///
    ////////////////////////////////////////////////////////////////////////
    void SaveDocument( XmlDocument* document, const char* fileName )
    {
        document->SaveFile(fileName);
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


#else
    ///private use
    const char* MakeName(const char* name, int index){return NULL;}
    void FreeName(const char* createdName){}
    
    /// Creates an xml element with the given name
    XmlElement* createNode(const char* name){return NULL;}
    /// Creates a text node with the given name
    XmlText* createTextNode(const char* name){return NULL;}
    
    /// Writes an attribute in the xml element with the given name tag
    template<> void writeAttribute<bool>(XmlElement* element, const char* name, const bool& attribute){}
    template<> void writeAttribute<int>(XmlElement* element, const char* name, const int& attribute){}
    template<> void writeAttribute<float>(XmlElement* element, const char* name, const float& attribute){}
    template<> void writeAttribute<std::string>(XmlElement* element, const char* name, const std::string& attribute){}
    /// Read an attribute from the xml element with the given name tag
    template<> bool readAttribute<bool>(const XmlElement* element, const char* name, bool& attribute){return false;}
    template<> bool readAttribute<int>(const XmlElement* element, const char* name, int& attribute){return false;}
    template<> bool readAttribute<float>(const XmlElement* element, const char* name, float& attribute){return false;}
    template<> bool readAttribute<std::string>(const XmlElement* element, const char* name, std::string& attribute){return false;}
    template<> bool readAttribute<char*>(const XmlElement* element, const char* name, char*& attribute){return false;}
    /// Appends an element to another element
    void LinkEndChild(XmlElement* parent, XmlElement* child){}
    /// Search an element with the tag
    const XmlElement* FirstChildElement( const XmlElement* element, const char* childName){return NULL;}
    /// Search an element with the tag
    XmlElement* FirstChildElement( XmlElement* element, const char* childName){return NULL;}
    /// first child of the element
    const XmlNode* FirstChild( const XmlElement* element, const char* childName ){return NULL;}
    /// first child of the element
    XmlNode* FirstChild( XmlElement* element, const char* childName ){return NULL;}
    
    /// creates an xml document
    XmlDocument* CreateDocument(const char* _version,const char* _encoding,const char* _standalone){return NULL;}
    /// Loads a document from the file, returns null if not found
    XmlDocument* LoadDocument(const char* fileName){return NULL;}
    void SaveDocument(XmlDocument* document, const char* fileName){}
    /// Free the document's memory
    void FreeDocument(XmlDocument* document){}

    /// retrieve the text for this node's tag
    const char* GetNodeTag(const XmlElement* element){return NULL;}
    /// retrieves the sibling of the current node
    const XmlElement* NextSibling( const XmlElement* child ) {return NULL;}
    
#endif
    
}