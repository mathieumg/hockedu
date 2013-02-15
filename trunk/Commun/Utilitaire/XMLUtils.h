#pragma once

template<class T>
class Vecteur3D;

#ifdef WIN32
class TiXmlElement;
class TiXmlText;
typedef TiXmlElement XmlElement;
typedef TiXmlText XmlText;
#else 
// mettre les typedef et class pour une librairie xml dans MAC
#endif

namespace XMLUtils
{
    XmlElement* creerNoeud(const char* name);
    XmlText* creerNoeudText(const char* name);
    void ecrireVecteur3Dxml(const Vecteur3D<float>* vecteur, XmlElement* element, const char* name);
    bool lectureVecteur3Dxml(Vecteur3D<float>* vecteur, const XmlElement* element, const char* name);
    template<class T>
    void ecrireAttribute(XmlElement* element, const char* name, const T& attribute);
    template<class T>
    bool LireAttribute(const XmlElement* element, const char* name, T& attribute);

    void LinkEndChild(XmlElement* parent, XmlElement* child);

}


