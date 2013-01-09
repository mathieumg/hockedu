//////////////////////////////////////////////////////////////////////////////
/// @file UtilitaireINF2990.cpp
/// @author Martin Bisson
/// @date 2007-05-22
/// @version 1.0 
///
/// @addtogroup inf2990 INF2990
/// @{
//////////////////////////////////////////////////////////////////////////////
#include "UtilitaireINF2990.h"


namespace utilitaire {


   ////////////////////////////////////////////////////////////////////////
   ///
   /// @fn string obtenirChaineISO(JNIEnv* env, jstring chaine)
   ///
   /// Cette fonction permet d'obtenir la valeur d'une cha�ne Java (String
   /// java ou jstring) sous la forme d'une cha�ne C++ (string STL) encod�
   /// ISO-8859-1.
   ///
   /// @param[in] env    : L'environnement Java.
   /// @param[in] chaine : La cha�ne � convertir.
   ///
   /// @return La cha�ne C++.
   ///
   ////////////////////////////////////////////////////////////////////////
   std::string obtenirChaineISO(JNIEnv* env, jstring chaine)
   {
      jclass classeString = env->FindClass("java/lang/String");

      jmethodID getBytes = env->GetMethodID(
         classeString, "getBytes", "(Ljava/lang/String;)[B"
         );
      jbyteArray byteArray = (jbyteArray)env->CallObjectMethod(
         chaine, getBytes, env->NewStringUTF("ISO-8859-1")
         );

      return std::string((const char*)env->GetByteArrayElements(
         byteArray, 0), env->GetArrayLength(byteArray)
         );
   }



}


///////////////////////////////////////////////////////////////////////////////
/// @}
///////////////////////////////////////////////////////////////////////////////
