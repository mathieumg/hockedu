

#include <iostream>
#include "ControllerServeurJeu.h"
#include <sstream>
/*
#include "..\Reseau\Paquets\PaquetTest.h"
#include "..\Reseau\GestionnaireReseau.h"
#include "..\Reseau\PacketBuilder.h"
#include "..\Reseau\PacketReader.h"
#include "Utilitaire.h"

#define PRINT 0*/

int main(void)  {

    std::cout << "--------- Serveur Jeu ---------" << std::endl << std::endl;
    /*
    GestionnaireReseauServeurJeu::obtenirInstance();
    GestionnaireReseau* wGestReseau = GestionnaireReseau::obtenirInstance();

    
    GestionnaireReseau::obtenirInstance()->ajouterOperationReseau("ChatMessage", new PacketHandlerChatMessage, new UsinePaquetChatMessage);
    GestionnaireReseau::obtenirInstance()->setController(new ControllerServeurJeu());
    */

    /*
    GestionnaireReseau::setNetworkMode(CLIENT);
	GestionnaireReseau::obtenirInstance();
    for(int i = 0; i < 1000000; ++i)
    {
        std::string testString("asd"), readString;
        uint64_t test64b = ((uint64_t)i) << 40, read64b;
        uint32_t test32b = i << 15, read32b;
        uint16_t test16b = i << 8, read16b;
        uint8_t  test8b  = i % 255, read8b;
        double testDouble = (i/1000000000000.0), readDouble;
        float testFloat = (i/100000.0f), readFloat;

#if PRINT
        std::cout << "---------------- BIG ENDIAN ----------------" << std::endl;
#endif
        
        PacketBuilder pString, p64b, p32b, p16b, p8b, pDouble, pFloat;
        pString << BIG_ENDIAN << testString ;
        PacketReader prString(pString.getPacketString());
        prString.setCurrentByteOrder(BIG_ENDIAN);
        int wArraySize = prString.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        prString.readString(wBuffer, wArraySize);
        readString = std::string((char*)wBuffer);
        delete wBuffer;

        p64b << BIG_ENDIAN << test64b;
        PacketReader pr64b(p64b.getPacketString());
        pr64b.setCurrentByteOrder(BIG_ENDIAN);
        read64b = pr64b.read64bInteger();

        p32b << BIG_ENDIAN << test32b;
        PacketReader pr32b(p32b.getPacketString());
        pr32b.setCurrentByteOrder(BIG_ENDIAN);
        read32b = pr32b.readInteger();

        p16b << BIG_ENDIAN << test16b;
        PacketReader pr16b(p16b.getPacketString());
        pr16b.setCurrentByteOrder(BIG_ENDIAN);
        read16b = pr16b.readShort();

        p8b << BIG_ENDIAN << test8b;
        PacketReader pr8b(p8b.getPacketString());
        pr8b.setCurrentByteOrder(BIG_ENDIAN);
        read8b = pr8b.readUnsignedChar();

        pDouble << BIG_ENDIAN << testDouble;
        PacketReader prDouble(pDouble.getPacketString());
        prDouble.setCurrentByteOrder(BIG_ENDIAN);
        readDouble = prDouble.readDouble();

        pFloat << BIG_ENDIAN << testFloat;
        PacketReader prFloat(pFloat.getPacketString());
        prFloat.setCurrentByteOrder(BIG_ENDIAN);
        readFloat = prFloat.readFloat();

#if PRINT
        std::cout << "One at a time:" << std::endl;
        std::cout << "String: " << testString << "=" << readString << std::endl;
        std::cout << "64b: "    << test64b    << "=" << read64b    << std::endl;
        std::cout << "32b: "    << test32b    << "=" << read32b    << std::endl;
        std::cout << "16b: "    << test16b    << "=" << read16b    << std::endl;
        std::cout << "8b: "     << test8b     << "=" << read8b     << std::endl;
        std::cout << "Float: "  << testFloat  << "=" << readFloat  << std::endl;
        std::cout << "Double: " << testDouble << "=" << readDouble << std::endl << std::endl;
#endif

        checkf(testString == readString, "STRING PAS PAREILLE %s != %s", testString, readString);
        checkf(test64b    == read64b   , "ENTIER 64B PAS PAREIL %d != %d", test64b, read64b);
        checkf(test32b    == read32b   , "ENTIER 32B PAS PAREIL %d != %d", test32b, read32b);
        checkf(test16b    == read16b   , "ENTIER 16B PAS PAREIL %d != %d", test16b, read16b);
        checkf(test8b     == read8b    , "ENTIER  8B PAS PAREIL %c != %c", test8b, read8b);
        checkf(testFloat  == readFloat , "FLOAT PAS PAREIL %f != %f", testFloat, readFloat);
        checkf(testDouble == readDouble, "DOUBLE PAS PAREIL %f != %f", testDouble, readDouble);

        PacketBuilder pb;
        pb << BIG_ENDIAN << testString << test64b << test32b << test16b << test8b << testDouble << testFloat;
        PacketReader pr(pb.getPacketString());

        pr.setCurrentByteOrder(BIG_ENDIAN);
        wArraySize = pr.readInteger();
        wBuffer = new uint8_t[wArraySize];
        pr.readString(wBuffer, wArraySize);
        readString = std::string((char*)wBuffer);
        delete wBuffer;
        read64b    = pr.read64bInteger();
        read32b    = pr.readInteger();
        read16b    = pr.readShort();
        read8b     = pr.readUnsignedChar();
        readDouble = pr.readDouble();
        readFloat  = pr.readFloat();

#if PRINT
        std::cout << "One at a time:" << std::endl;
        std::cout << "String: " << testString << "=" << readString << std::endl;
        std::cout << "64b: "    << test64b    << "=" << read64b    << std::endl;
        std::cout << "32b: "    << test32b    << "=" << read32b    << std::endl;
        std::cout << "16b: "    << test16b    << "=" << read16b    << std::endl;
        std::cout << "8b: "     << test8b     << "=" << read8b     << std::endl;
        std::cout << "Float: "  << testFloat  << "=" << readFloat  << std::endl;
        std::cout << "Double: " << testDouble << "=" << readDouble << std::endl << std::endl;
#endif

        checkf(testString == readString, "STRING PAS PAREILLE %s != %s", testString, readString);
        checkf(test64b    == read64b   , "ENTIER 64B PAS PAREIL %d != %d", test64b, read64b);
        checkf(test32b    == read32b   , "ENTIER 32B PAS PAREIL %d != %d", test32b, read32b);
        checkf(test16b    == read16b   , "ENTIER 16B PAS PAREIL %d != %d", test16b, read16b);
        checkf(test8b     == read8b    , "ENTIER  8B PAS PAREIL %c != %c", test8b, read8b);
        checkf(testFloat  == readFloat , "FLOAT PAS PAREIL %f != %f", testFloat, readFloat);
        checkf(testDouble == readDouble, "DOUBLE PAS PAREIL %f != %f", testDouble, readDouble);
        
#if PRINT
        std::cout << "---------------- LITTLE ENDIAN ----------------" << std::endl;
#endif

        pString.clearBuffer();
        pString << LITTLE_ENDIAN << testString ;
        prString = PacketReader(pString.getPacketString());
        prString.setCurrentByteOrder(LITTLE_ENDIAN);
        wArraySize = prString.readInteger();
        wBuffer = new uint8_t[wArraySize];
        prString.readString(wBuffer, wArraySize);
        readString = std::string((char*)wBuffer);
        delete wBuffer;

        p64b.clearBuffer();
        p64b << LITTLE_ENDIAN << test64b;
        pr64b = PacketReader(p64b.getPacketString());
        pr64b.setCurrentByteOrder(LITTLE_ENDIAN);
        read64b = pr64b.read64bInteger();

        p32b.clearBuffer();
        p32b << LITTLE_ENDIAN << test32b;
        pr32b = PacketReader(p32b.getPacketString());
        pr32b.setCurrentByteOrder(LITTLE_ENDIAN);
        read32b = pr32b.readInteger();

        p16b.clearBuffer();
        p16b << LITTLE_ENDIAN << test16b;
        pr16b = PacketReader(p16b.getPacketString());
        pr16b.setCurrentByteOrder(LITTLE_ENDIAN);
        read16b = pr16b.readShort();

        p8b.clearBuffer();
        p8b << LITTLE_ENDIAN << test8b;
        pr8b = PacketReader(p8b.getPacketString());
        pr8b.setCurrentByteOrder(LITTLE_ENDIAN);
        read8b = pr8b.readUnsignedChar();

        pDouble.clearBuffer();
        pDouble << LITTLE_ENDIAN << testDouble;
        prDouble = PacketReader(pDouble.getPacketString());
        prDouble.setCurrentByteOrder(LITTLE_ENDIAN);
        readDouble = prDouble.readDouble();

        pFloat.clearBuffer();
        pFloat << LITTLE_ENDIAN << testFloat;
        prFloat = PacketReader(pFloat.getPacketString());
        prFloat.setCurrentByteOrder(LITTLE_ENDIAN);
        readFloat = prFloat.readFloat();

#if PRINT
        std::cout << "One at a time:" << std::endl;
        std::cout << "String: " << testString << "=" << readString << std::endl;
        std::cout << "64b: "    << test64b    << "=" << read64b    << std::endl;
        std::cout << "32b: "    << test32b    << "=" << read32b    << std::endl;
        std::cout << "16b: "    << test16b    << "=" << read16b    << std::endl;
        std::cout << "8b: "     << test8b     << "=" << read8b     << std::endl;
        std::cout << "Float: "  << testFloat  << "=" << readFloat  << std::endl;
        std::cout << "Double: " << testDouble << "=" << readDouble << std::endl << std::endl;
#endif

        checkf(testString == readString, "STRING PAS PAREILLE %s != %s", testString, readString);
        checkf(test64b    == read64b   , "ENTIER 64B PAS PAREIL %d != %d", test64b, read64b);
        checkf(test32b    == read32b   , "ENTIER 32B PAS PAREIL %d != %d", test32b, read32b);
        checkf(test16b    == read16b   , "ENTIER 16B PAS PAREIL %d != %d", test16b, read16b);
        checkf(test8b     == read8b    , "ENTIER  8B PAS PAREIL %c != %c", test8b, read8b);
        checkf(testFloat  == readFloat , "FLOAT PAS PAREIL %f != %f", testFloat, readFloat);
        checkf(testDouble == readDouble, "DOUBLE PAS PAREIL %f != %f", testDouble, readDouble);

        pb.clearBuffer();
        pb << LITTLE_ENDIAN << testString << test64b << test32b << test16b << test8b << testDouble << testFloat;

        pr = PacketReader(pb.getPacketString());
        pr.setCurrentByteOrder(LITTLE_ENDIAN);
        wArraySize = pr.readInteger();
        wBuffer = new uint8_t[wArraySize];
        pr.readString(wBuffer, wArraySize);
        readString = std::string((char*)wBuffer);
        delete wBuffer;
        read64b    = pr.read64bInteger();
        read32b    = pr.readInteger();
        read16b    = pr.readShort();
        read8b     = pr.readUnsignedChar();
        readDouble = pr.readDouble();
        readFloat  = pr.readFloat();

#if PRINT
        std::cout << "One at a time:" << std::endl;
        std::cout << "String: " << testString << "=" << readString << std::endl;
        std::cout << "64b: "    << test64b    << "=" << read64b    << std::endl;
        std::cout << "32b: "    << test32b    << "=" << read32b    << std::endl;
        std::cout << "16b: "    << test16b    << "=" << read16b    << std::endl;
        std::cout << "8b: "     << test8b     << "=" << read8b     << std::endl;
        std::cout << "Float: "  << testFloat  << "=" << readFloat  << std::endl;
        std::cout << "Double: " << testDouble << "=" << readDouble << std::endl << std::endl;
#endif

        checkf(testString == readString, "STRING PAS PAREILLE %s != %s", testString, readString);
        checkf(test64b    == read64b   , "ENTIER 64B PAS PAREIL %d != %d", test64b, read64b);
        checkf(test32b    == read32b   , "ENTIER 32B PAS PAREIL %d != %d", test32b, read32b);
        checkf(test16b    == read16b   , "ENTIER 16B PAS PAREIL %d != %d", test16b, read16b);
        checkf(test8b     == read8b    , "ENTIER  8B PAS PAREIL %c != %c", test8b, read8b);
        checkf(testFloat  == readFloat , "FLOAT PAS PAREIL %f != %f", testFloat, readFloat);
        checkf(testDouble == readDouble, "DOUBLE PAS PAREIL %f != %f", testDouble, readDouble);
    }
    system("pause");*/


    // To prevend the server to exit without killing it
    while(true)
    {
        getchar(); // Pause until enter is pressed
    }

    return 0;
}
