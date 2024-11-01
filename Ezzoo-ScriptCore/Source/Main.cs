
using System;
using System.Runtime.CompilerServices;

namespace Ezzoo {


    public struct Vector3
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

    }


    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string str, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float NativeVectorLog(ref Vector3 vec3);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref Vector3 NativeVectorCross(ref Vector3 vec3, ref Vector3 rVec);
    }

    public class Entity
    {
        public float MyPublicFloatVar = 5.0f;
        public int MyPublicIntVar = 5;

        internal Entity (ulong id)
        {
            ID = id;
        }

        public Entity() { }

        public readonly ulong ID ;

        public void PrintFloatVar()
        {
            Console.WriteLine("MyPublicFloatVar = {0:F}", MyPublicFloatVar);
        } 

        public void PrintIntVar(int x, int y)
        {
            Console.WriteLine("x = {0}, y = {1}", x, y);
        }

        public void PrintString (string str)
        {
            Console.WriteLine(str);
        }

        public void IncrementFloatVar(float value)
        {
            MyPublicFloatVar += value;
        }

        public void Log(string str, int parameter)
        {
            InternalCalls.NativeLog(str, parameter);
        }
        
        public void Log(Vector3 vec)
        {
            float x = InternalCalls.NativeVectorLog(ref vec);
            Console.WriteLine("{0:F}", x);
        }

        public void Log(Vector3 vec, Vector3 rVec)
        {
            InternalCalls.NativeVectorCross(ref vec, ref rVec);
            Console.WriteLine("{0:F}, {1:F}, {2:F}", rVec.X, rVec.Y, rVec.Z);
        }


 
    }
}