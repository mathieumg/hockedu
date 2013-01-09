#pragma once

class TiXmlElement;
class TiXmlText;

template<class T>
class Vecteur3D;


namespace XMLUtils
{
    TiXmlElement* creerNoeud(const char* name);
    TiXmlText* creerNoeudText(const char* name);
    void ecrireVecteur3Dxml(const Vecteur3D<double>* vecteur, TiXmlElement* element, const char* name);
    bool lectureVecteur3Dxml(Vecteur3D<double>* vecteur, const TiXmlElement* element, const char* name);
    template<class T>
    void ecrireAttribute(TiXmlElement* element, const char* name, const T& attribute);
    template<class T>
    bool LireAttribute(const TiXmlElement* element, const char* name, T& attribute);

    void LinkEndChild(TiXmlElement* parent, TiXmlElement* child);

}


