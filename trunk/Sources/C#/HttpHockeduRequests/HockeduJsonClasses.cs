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
}
