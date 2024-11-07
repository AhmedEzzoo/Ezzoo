using System;


using Ezzoo;

namespace Sandbox
{
    public class Camera : Entity
    {
        public float YAW = 1.67f;

        void OnUpdate(float ts)
        {

            const float speed = 1.0f;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.EZZOO_KEY_UP))
            {
                velocity.Y = 1.0f;
            }
            else if (Input.IsKeyDown(KeyCode.EZZOO_KEY_DOWN))
            {
                velocity.Y = -1.0f;
            }
           if (Input.IsKeyDown(KeyCode.EZZOO_KEY_LEFT))
            {
                velocity.X = 1.0f;
            }
            else if (Input.IsKeyDown(KeyCode.EZZOO_KEY_RIGHT))
            {
                velocity.X = -1.0f;
            }

            velocity *= speed;

            
            Vector3 translation = Translation;
            translation += velocity * ts;
           Translation = translation;

        }


    }


}

