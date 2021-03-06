

HeaderMessage = "/// This file is autogenerated by BuildPacketsDefinitions.py\n"
HeaderMessage += "/// DO NOT MODIFY DIRECTLY\n\n"
HeaderMessage += "#pragma once\n"



HeaderFile = open("PacketsDefinitions.h",'w')
HeaderFile.write(HeaderMessage)

SourceFile = open("PacketsDefinitions.cpp",'w')
SourceFile.write(HeaderMessage)

SourceFile.write("#include \"{0}\"\n".format(HeaderFile.name))
SourceFile.write("#include \"PacketReader.h\"\n")
SourceFile.write("#include \"PacketBuilder.h\"\n")



DefFile = open("Packets.def",'r')

all = DefFile.read()

all = all.split("###")

# assuming the def file has  ### content to write to header ###  script
HeaderFile.write(all[1])


# writing necessary includes and declarations
HeaderFile.write("class PacketReader;\nclass PacketBuilder;\n")


all = all[-1]
all = all.split('{')
t = []
for i in all : 
    for s in i.split('}'):
        t.append(s)
all = t
t = []

## Reading Definitions ###############################################
readingName = True
name = ""
PacketsDefinitions = {}
for block in all:
    block = block.strip()
    if readingName:
        if len(block) > 0 :
            name = block
            PacketsDefinitions[name] = []
            readingName = False
    else:
        readingName = True
        comment = ""
        for line in block.split('\n') :
            line = line.strip()
            if len(line) > 2 :
                if line[0:2] == "//" :
                    comment += line[2:]
                else:
                    step=True
                    varPresent = False
                    for content in line.split('/'):
                        if step:
                            content.strip()
                            content = content.split(' ')
                            t = [content[0]]
                            for i in content[1:] : 
                                for s in i.split(','):
                                    if len(s)>0:
                                        t.append(s)
                            content = t
                            t = []
                            for i in content : 
                                for s in i.split(';'):
                                    if len(s)>0:
                                        t.append(s)
                            content = t
                            t = []

                            if len(content) > 0:
                                type = content[0]
                                vars = content[1:]
                                varPresent = True
                            step=False
                        else:
                            comment += content
                    if varPresent:
                        PacketsDefinitions[name].append((type,vars,comment))
                        comment = ""
##############################################################################################
        
## Writing Enum  ##################################################
enum = "enum PacketDataTypes\n{\n    PT_NONE=666,\n";
for dataType in PacketsDefinitions.keys():
    enum += "    PT_"+dataType.upper()+",\n"
enum += "    NB_PACKETDATATYPE,\n"
enum += "};\n\n"

HeaderFile.write(enum)
##############################################################################################

HeaderFile.write("\nclass PacketDataBase{\npublic:\n    virtual void ReceiveData(PacketReader& r) = 0;\n    virtual void SendData(PacketBuilder& b) = 0;\n    virtual PacketDataTypes GetType() = 0;\n };\n\n")


## Writing Classes ###########################################################################
for dataType in PacketsDefinitions.keys():
    dataClass = "class " + dataType + " : public PacketDataBase\n{\npublic:\n"
    sourceContent = ""
    for variable in PacketsDefinitions[dataType]:
        type = variable[0]
        names = variable[1]
        comments = variable[2]
        # add comments first
        dataClass += "    //"+comments+"\n"
        #print type
        dataClass += "    "+type+" "
        #print variable names
        for i in range(0,len(names)):
            dataClass += names[i]
            if i+1 < len(names):
                dataClass += ", "
            else:
                dataClass += ";\n\n"
    dataClass += "    void ReceiveData(PacketReader& r);\n"
    dataClass += "    void SendData(PacketBuilder& b);\n"
    dataClass += "    PacketDataTypes GetType(){ return PT_"+dataType.upper()+";}\n"
    dataClass += "};\n\n"
    HeaderFile.write(dataClass)


    sourceContent += "void "+dataType+"::ReceiveData(PacketReader& r)\n{\n"
    sourceContent += "    r >> "
    for i in range(0,len(PacketsDefinitions[dataType])):
        variable = PacketsDefinitions[dataType][i]
        type = variable[0]
        names = variable[1]
        for n in names :
            sourceContent += n+" >> "
    sourceContent = sourceContent[:-4]+";\n"
    sourceContent += "}\n\n"

    sourceContent += "void "+dataType+"::SendData(PacketBuilder& b)\n{\n"
    sourceContent += "    b << "
    for i in range(0,len(PacketsDefinitions[dataType])):
        variable = PacketsDefinitions[dataType][i]
        type = variable[0]
        names = variable[1]
        for n in names :
            sourceContent += n+" << "
    sourceContent = sourceContent[:-4]+";\n"
    sourceContent += "}\n\n"
    SourceFile.write(sourceContent)

##############################################################################################

generators =  "typedef PacketDataBase* (*PacketDataGenerator)();\n"
for dataType in PacketsDefinitions.keys():
    generators +=  "PacketDataBase* create"+dataType+"(){return new "+dataType+"();}\n"
generators +=  "const PacketDataGenerator generators["+str(len(PacketsDefinitions))+"] =            \n"
generators +=  "{                                                           \n"
for dataType in PacketsDefinitions.keys():
    generators +=  "    create"+dataType+",\n"
generators +=  "};                                                          \n"

generators +=  "PacketDataBase* CreatePacketData(PacketDataTypes t)             \n"
generators +=  "{                                                               \n"
generators +=  "    unsigned int i = t-PT_NONE-1;                                     \n"
generators +=  "    return i < (NB_PACKETDATATYPE-PT_NONE) ? generators[i]() : NULL;                                     \n"
generators +=  "}\n"

SourceFile.write(generators)
HeaderFile.write("PacketDataBase* CreatePacketData(PacketDataTypes t);\n")
print("{0} & {1} generated".format(HeaderFile.name,SourceFile.name))
