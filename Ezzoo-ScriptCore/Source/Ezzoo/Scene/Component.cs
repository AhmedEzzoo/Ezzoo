
namespace Ezzoo
{
    public class Component
    {
        public Entity entity { get; internal set; }

    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(entity.ID, out Vector3 translation);
                return translation;

            }

            set
            {
                InternalCalls.TransformComponent_SetTranslation(entity.ID, ref value);
            }
        }
    }

    public class RigidBodyComponent : Component
    {
        public void Entity_ApplyImpulse(Vector2 impulse, Vector2 point, bool wake)
        {
            InternalCalls.Entity_ApplyLinearImpulse(entity.ID, ref impulse, ref point, true);
        }
    }
}
