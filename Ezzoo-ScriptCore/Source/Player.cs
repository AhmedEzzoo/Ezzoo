﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using Ezzoo;
namespace Sandbox
{
    class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate: {ID} ");
        }

        void OnUpdate (float ts)
        {
            InternalCalls.Entity_GetTranslation(ID);
        }


    }
}