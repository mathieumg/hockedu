﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace ExecuteUnitTests
{
    class Program
    {
        [DllImport(@"INF2990.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ExecuteUnitTest();

        static void Main(string[] args)
        {
            ExecuteUnitTest();

        }
    }
}
