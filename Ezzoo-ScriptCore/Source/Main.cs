
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

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref Vector3 Entity_GetTranslation(ulong id);
    }

    public class Entity
    {

        protected Entity() { ID = 0; }

        internal Entity (ulong id)
        {
            ID = id;
        }


        public readonly ulong ID ;
 
    }
}