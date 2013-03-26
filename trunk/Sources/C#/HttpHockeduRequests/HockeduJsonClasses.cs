using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace HttpHockeduRequests
{
    public class UserData
    {
        public string username; // Attention, le nom doit etre le meme que dans le JSON. Ex: dans ce cas-ci, il va chercher le field "username". Case sensitive!!!
    }

    public class UserInfos
    {
        public string text;
        public UserData user;
    }

    public class UserList
    {
        public UserInfos[] data;
    }



    // Classes pour la liste des maps


    public class MapListJSON
    {
        public string error;                    // Nom de l'erreur, null sinon
        public UserMapDetailedJSON[] maps;  // Liste des maps

    }

    public class UserMapDetailedJSON
    {
        public int      id;
        public string   name;
        public string   description;
        public string   rating_average;
        public string   rating_count;
        public bool     is_public;
        public string   creation_time;
        public string   last_modified_time;
        public override string ToString() { return name; }
    }


    // Classes pour le getMap

    public class UserMapLightJSON
    {
        public string   error;       // Error message
        public string   name;    // Map name
        public string   content;    // XML content
    }


}
