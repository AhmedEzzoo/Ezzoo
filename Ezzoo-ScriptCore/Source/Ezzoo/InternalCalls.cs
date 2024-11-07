using System;
using System.Runtime.CompilerServices;


namespace Ezzoo
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string str, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float NativeVectorLog(ref Vector3 vec3);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ref Vector3 NativeVectorCross(ref Vector3 vec3, ref Vector3 rVec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_IsKeyDown(KeyCode keyCode);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(ulong id, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(ulong id, out Vector3 val);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong id, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_ApplyLinearImpulse(ulong id, ref Vector2 impulse, ref Vector2 point, bool wake);
    }

}
