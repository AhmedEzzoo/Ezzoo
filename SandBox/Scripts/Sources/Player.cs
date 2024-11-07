using System;


using Ezzoo;
namespace Sandbox
{
    public class Player : Entity
    {

        private TransformComponent m_TransformComponent;
        private RigidBodyComponent m_RigidbodyComponent;


        public float Speed ;
        public float Time ;


        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate: {ID} ");

            m_TransformComponent = GetComponent<TransformComponent>();
            m_RigidbodyComponent = GetComponent<RigidBodyComponent>();
        }

        void OnUpdate(float ts)
        {

            Time += ts;

            float speed = Speed;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.EZZOO_KEY_W))
            {
                velocity.Y = 1.0f;
            }
            else if (Input.IsKeyDown(KeyCode.EZZOO_KEY_S))
            {
                velocity.Y = -1.0f;
            }
            else if (Input.IsKeyDown(KeyCode.EZZOO_KEY_D))
            {
                velocity.X = 1.0f;
            }
            else if (Input.IsKeyDown(KeyCode.EZZOO_KEY_A))
            {
                velocity.X = -1.0f;
            }

            velocity *= speed;

            m_RigidbodyComponent.Entity_ApplyImpulse(velocity.XY, Vector2.Zero, true);

           // Vector3 translation = m_TransformComponent.Translation;
            //translation += velocity * ts;
            //m_TransformComponent.Translation = translation;

        }


    }
}
