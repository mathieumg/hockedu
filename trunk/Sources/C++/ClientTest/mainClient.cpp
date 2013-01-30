


#include <stdlib.h>
#include <stdio.h>
#include "Reseau\GestionnaireReseau.h"
#include <iostream>
#include "Reseau\PaquetHandlers\PacketHandlerString.h"
#include "Reseau\UsinePaquets\UsinePaquetString.h"
#include <sstream>
#include "Reseau\Paquets\PaquetTest.h"

using namespace std;

int main(void)  {

	
	printf("client\n");
	GestionnaireReseau::obtenirInstance();
    while(true)
    {
        std::string testString("asd"), readString;
        uint64_t test64b = 4 << 20, read64b;
        uint32_t test32b = 4 << 15, read32b;
        uint16_t test16b = 4 << 8, read16b;
        uint8_t  test8b  = 4, read8b;
        double testDouble = 0.121321861351681351891361654986154651894135489615494865189432198451, readDouble;
        float testFloat = 0.236185413219816f, readFloat;
        
        PacketBuilder pString, p64b, p32b, p16b, p8b, pDouble, pFloat;
        pString << testString ;
        PacketReader prString(pString.getPacketString());

        int wArraySize = prString.readInteger();
        uint8_t* wBuffer = new uint8_t[wArraySize];
        prString.readString(wBuffer, wArraySize);
        readString = std::string((char*)wBuffer);
        delete wBuffer;

        p64b << test64b;
        PacketReader pr64b(p64b.getPacketString());
        read64b = pr64b.read64bInteger();

        p32b << test32b;
        PacketReader pr32b(p32b.getPacketString());
        read32b = pr32b.readInteger();

        p16b << test16b;
        PacketReader pr16b(p16b.getPacketString());
        read16b = pr16b.readShort();

        p8b << test8b;
        PacketReader pr8b(p8b.getPacketString());
        read8b = pr8b.readUnsignedChar();

        pDouble << testDouble;
        PacketReader prDouble(pDouble.getPacketString());
        readDouble = prDouble.readDouble();

        pFloat << testFloat;
        PacketReader prFloat(pFloat.getPacketString());
        readFloat = prFloat.readFloat();

        std::cout << "One at a time:" << std::endl;
        std::cout << "String: " << testString << "=" << readString << std::endl;
        std::cout << "64b: "    << test64b    << "=" << read64b    << std::endl;
        std::cout << "32b: "    << test32b    << "=" << read32b    << std::endl;
        std::cout << "16b: "    << test16b    << "=" << read16b    << std::endl;
        std::cout << "8b: "     << test8b     << "=" << read8b     << std::endl;
        std::cout << "Float: "  << testFloat  << "=" << readFloat  << std::endl;
        std::cout << "Double: " << testDouble << "=" << readDouble << std::endl;

        PacketBuilder pb;
        pb << testString << test64b << test32b << test16b << test8b << testDouble << testFloat;
        PacketReader pr(pb.getPacketString());

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

        std::cout << "All at once:" << std::endl;
        std::cout << "String: " << testString << "=" << readString << std::endl;
        std::cout << "64b: "    << test64b    << "=" << read64b    << std::endl;
        std::cout << "32b: "    << test32b    << "=" << read32b    << std::endl;
        std::cout << "16b: "    << test16b    << "=" << read16b    << std::endl;
        std::cout << "8b: "     << test8b     << "=" << read8b     << std::endl;
        std::cout << "Float: "  << testFloat  << "=" << readFloat  << std::endl;
        std::cout << "Double: " << testDouble << "=" << readDouble << std::endl;
    }
	system("pause");
	return 0;
}