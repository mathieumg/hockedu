#include "PacketReader.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>
#include "PacketHandler.h"
#include <queue>
#include "Paquet.h"
#include <windows.h>


int FreePacket(Paquet* p)
{
    delete p;
    return 0;
}

class PacketFile
{
public:
    PacketFile(const char* fileName, std::ofstream& pOutput, const char* friendlyName):
        mInput(mReader.mInputFile),mOuput(pOutput),mFriendlyName(friendlyName)
    {
        mInput.open(fileName,std::ios::binary|std::ios::in);
        mIsValid = !mInput.fail();
        mReader.mOutputFile = &mOuput;
    }
    ~PacketFile()
    {
        mInput.close();
    }

    bool getNextTimeStamp()
    {
        if(mIsValid)
        {
            mInput.read((char*)&mTimeStamp,8);
            mIsValid = !mInput.eof();
            return mIsValid;
        }
        return false;
    }
    bool readNextPacket()
    {
        if(mIsValid)
        {
            try
            {
                FILETIME ft_convert;
                ft_convert.dwHighDateTime = mTimeStamp>>32LL;
                ft_convert.dwLowDateTime = (DWORD)mTimeStamp;
                SYSTEMTIME t;
                FileTimeToSystemTime(&ft_convert,&t);
                mOuput << "[" << t.wHour << ":" << t.wMinute << ":" << t.wSecond << ":" << t.wMilliseconds << "] ";

                mOuput << mFriendlyName;
                char operationType;
                mInput.get(operationType);



                if(operationType == 0)
                {
                    mOuput << " <<=== ";
                } 
                else if(operationType == 1)
                {
                    mOuput << " ===>> ";
                }
                else
                {
                    mOuput << " <<???>> ";
                }

                mReader.mPrintingToOut = false;
                HeaderPaquet wPacketHeader = PacketHandler::handlePacketHeaderReception(mReader);
                mReader.mPrintingToOut = true;

                mOuput << ": ";
                if(wPacketHeader.type < NB_PACKET_TYPES)
                    mOuput << PaquetNamesArray[wPacketHeader.type] ;
                else if (wPacketHeader.type > PT_NONE && wPacketHeader.type < NB_PACKETDATATYPE)
                {
                    mOuput << GetPacketDataName((PacketDataTypes)wPacketHeader.type);
                }
                mOuput << std::endl <<"\t"; 

                bool wTaillePaquetValide = wPacketHeader.taillePaquet > 0;
                if(wTaillePaquetValide)
                {
                    PacketHandler* wPacketHandler = PacketHandler::GetHandler(wPacketHeader.type);
                    wPacketHandler->handlePacketReceptionSpecific( mReader , FreePacket );
                }
                mOuput << std::endl;

                mIsValid = !mInput.eof();
            }
            catch(...)
            {
                mIsValid = AttemptRecover();
                if(!mIsValid)
                {
                    mOuput << " Error while trying to recover\n";
                }
                mReader.mPrintingToOut = true;

            }
        }
        return mIsValid;
    }


    int64_t mTimeStamp;
private:
    PacketReader mReader;
    std::ifstream& mInput;
    std::ofstream& mOuput;
    std::string mFriendlyName;
    bool mIsValid;

    bool AttemptRecover()
    {
        bool recoverStatus = false;
        mReader.mPrintingToOut = false;
        try
        {
            while(!mInput.eof())
            {
                unsigned int seqId = mReader.readUnsignedInteger();
                if ( Paquet::sequenceIdentification == seqId)
                {
                    // back up the id(-4), the timestamp(-4) and the token(-1)
                    mInput.seekg(-4-4-1,std::ios::cur);
                    recoverStatus = true;
                }
            }
        }
        catch(...)
        {
            // error trying to recover
        }

        mReader.mPrintingToOut = true;
        return recoverStatus;
    }


};
struct FileQueueElem
{
    PacketFile* file;
    bool operator<(const FileQueueElem& p)const
    {
        // on retourne l'inverse car un priority queue positionne le plus gros en haut
        return file->mTimeStamp > p.file->mTimeStamp;
    }

};

int main(int argc, char** argv)
{
    std::ofstream output;
    output.open("DecryptionResult.txt", std::fstream::out);
    if(output.fail())
    {
        std::cout << "Error opening file DecryptionResult.txt" << std::endl;
        output.close();
        return -1;
    }

    /// conteneur avec les vrai references sur tous les fichiers
    std::vector<PacketFile*> allFiles;
    // conserve des references car on veut les trier par rapport au timestamp
    std::priority_queue<FileQueueElem> timeFileQueue;

    for(int i=1; i<argc; ++i)
    {
        std::string friendlyName = argv[i];
        int c = friendlyName.find_last_of('/');
        if(c == -1)
            c = friendlyName.find_last_of('\\');
        if(c != -1)
        {
            friendlyName = friendlyName.substr(c+1,friendlyName.size()-c-2);
        }
        int len = (int)friendlyName.size();
        for(int count = 0; count<len; ++count)
        {
            if(!isalpha(friendlyName[count]))
            {
                friendlyName.resize(count);
                break;
            }
        }

        PacketFile* file = new PacketFile(argv[i],output,friendlyName.c_str());
        allFiles.push_back(file);
        if(file->getNextTimeStamp())
        {
            FileQueueElem elem;
            elem.file = file;
            timeFileQueue.push(elem);
        }
    }

    while(!timeFileQueue.empty())
    {
        FileQueueElem nextElem = timeFileQueue.top();
        timeFileQueue.pop();

        PacketFile* nextFile = nextElem.file;
        if(nextFile->readNextPacket() && nextFile->getNextTimeStamp())
        {
            timeFileQueue.push(nextElem);
        }
    }

    STL_ITERATE(allFiles,it)
    {
        delete *it;
    }
    allFiles.clear();

    return 0;
}