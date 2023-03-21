using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Phoenix;

namespace Sandbox
{
    public class Player : Entity
    {
        void OnCreate()
        {
        }
        void OnUpdate(float DeltaTime)
        {
            float speed = 30.0f;
            float velocity = 0.0f;

            if (Input.IsKeyDown(Key.D))
                velocity += 1.0f;
            velocity *= speed;

            Console.WriteLine(velocity);
            GetComponent<Rigidbody2DComponent>().ApplyLinearImpulse(new Vector2(0, velocity), false);
        }
    }
}
