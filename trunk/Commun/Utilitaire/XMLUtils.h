#pragma once
// Evitez le plus possible de mettre des includes ici

// a enlever dès que le wrapper est terminé
#include "tinyxml\include\tinyxml.h"


template<class T>
class Vecteur3D;

#ifdef WIN32
class TiXmlElement;
typedef TiXmlElement XmlElement;

class TiXmlText;
typedef TiXmlText XmlText;

class TiXmlNode;
typedef TiXmlNode XmlNode;

class TiXmlDocument;
typedef TiXmlDocument XmlDocument;

#else 
// mettre les typedef et class pour une librairie xml dans MAC
#endif

namespace XMLUtils
{
    /// Creates an xml element with the given name
    XmlElement* createNode(const char* name);
    /// Creates a text node with the given name
    XmlText* createTextNode(const char* name);

    /// writes an array of element
    template<class T>
    void writeArray(const T* pArray, int size, XmlElement* element, const char* name)
    {
        for(int i=0; i<size; ++i)
        {
            const char* createdName = MakeName(name,i);
            if(createdName)
            {
                writeAttribute(element,createdName,pArray[i]);
                FreeName(createdName);
            }
        }
    }

    /// reads an array of element
    template<class T>
    bool readArray(T* pArray, int size, const XmlElement* element, const char* name)
    {
        for(int i=0; i<size; ++i)
        {
            const char* createdName = MakeName(name,i);
            if(!createdName)
            {
                return false;
            }

            if(!readAttribute(element,createdName,pArray[i]))
            {
                FreeName(createdName);
                return false;
            }
            FreeName(createdName);
        }
        return true;
    }

    /// Writes an attribute in the xml element with the given name tag
    template<class T> void writeAttribute(XmlElement* element, const char* name, const T& attribute);
    /// Read an attribute from the xml element with the given name tag
    template<class T> bool readAttribute(const XmlElement* element, const char* name, T& attribute);
    /// Appends an element to another element
    void LinkEndChild(XmlElement* parent, XmlElement* child);
    /// Search an element with the tag
    const XmlElement* FirstChildElement( const XmlElement* element, const char* childName);
    /// Search an element with the tag
    XmlElement* FirstChildElement( XmlElement* element, const char* childName);
    /// first child of the element
    const XmlNode* FirstChild( const XmlElement* element );
    /// first child of the element
    XmlNode* FirstChild( XmlElement* element );

    /// creates an xml document
    XmlDocument* CreateDocument(const char* _version,const char* _encoding,const char* _standalone);
    /// Loads a document from the file, returns null if not found
    XmlDocument* LoadDocument(const char* fileName);
    void SaveDocument(XmlDocument* document, const char* fileName);
    /// Free the document's memory
    void FreeDocument(XmlDocument* document);

///private use
    const char* MakeName(const char* name, int index);
    void FreeName(const char* createdName);
}


