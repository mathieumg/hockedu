#pragma once
// Evitez le plus possible de mettre des includes ici

#ifndef NULL
#define NULL 0
#endif

#include <string>

template<class T>
class Vecteur3D;

#ifdef WIN32
#include "tinyxml\include\tinyxml.h"
typedef TiXmlElement XmlElement;
typedef TiXmlText XmlText;
typedef TiXmlNode XmlNode;
typedef TiXmlDocument XmlDocumentNode;
#elif __APPLE__
#include "tinyxml_mac/tinyxml.h"
typedef TiXmlElement XmlElement;
typedef TiXmlText XmlText;
typedef TiXmlNode XmlNode;
typedef TiXmlDocument XmlDocumentNode;
#else
typedef int XmlElement;

typedef int XmlText;

typedef int XmlNode;

typedef int XmlDocumentNode;

// mettre les typedef et class pour une librairie xml dans MAC
#endif

// Permet de s'assurer que la memoire est bien liberé lorsqu'une
// variable document sort du scope.
class XmlDocument
{
public:
    XmlDocument()
    {
        mNode = new XmlDocumentNode();
    }
    ~XmlDocument()
    {
        delete mNode;
    }
    XmlElement* GetElem() const
    {
        return (XmlElement*)mNode;
    }
    XmlDocumentNode* mNode;
};

namespace XMLUtils
{
    static const std::string XmlFieldVersion = "3.1";
    
    ///private use
    const char* MakeName(const char* name, int index);
    void FreeName(const char* createdName);
    
    
    
    /// Creates an xml element with the given name
    XmlElement* createNode(const char* name);
    /// Creates a text node with the given name
    XmlText* createTextNode(const char* name);

    
    /// Writes an attribute in the xml element with the given name tag
    template<class T> void writeAttribute(XmlElement* element, const char* name, const T& attribute);
    void writeString(XmlElement* element, const char* name, const char* attribute);
    /// Read an attribute from the xml element with the given name tag
    template<class T> bool readAttribute(const XmlElement* element, const char* name, T& attribute);
    /// retrieve the text for this node's tag
    const char* GetNodeTag(const XmlElement* element);

    /// Appends an element to another element
    void LinkEndChild(XmlNode* parent, XmlNode* child);
    void LinkEndChild(XmlDocument& parent, XmlNode* child);
    /// Search an element with the tag
    const XmlElement* FirstChildElement( const XmlElement* element, const char* childName = NULL);
    const XmlElement* FirstChildElement( const XmlDocument& document, const char* childName = NULL);
    /// Search an element with the tag
    XmlElement* FirstChildElement( XmlElement* element, const char* childName = NULL);
    /// first child of the element
    const XmlNode* FirstChild( const XmlElement* element, const char* childName = NULL );
    /// first child of the element
    XmlNode* FirstChild( XmlElement* element, const char* childName = NULL );
    /// retrieves the sibling of the current node
    const XmlElement* NextSibling( const XmlElement* child );

    /// creates an xml document
    void CreateDocument(XmlDocument& document);
    /// Loads a document from the file, returns null if not found
    bool LoadDocument(XmlDocument& document, const char* fileName);
    bool SaveDocument(XmlDocument& document, const char* fileName);

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

}


