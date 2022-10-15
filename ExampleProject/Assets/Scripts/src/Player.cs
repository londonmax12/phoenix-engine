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
            Console.WriteLine($"Player.OnCreate:{entityID}");
        }
        void OnUpdate(float DeltaTime)
        {
            float speed = 2.0f;
            Vector3 velocity = new Vector3();

            if (Input.IsKeyDown(Key.W))
                velocity.y = 1.0f;
            else if (Input.IsKeyDown(Key.S))
                velocity.y = -1.0f;

            if (Input.IsKeyDown(Key.D))
                velocity.x = 1.0f;
            else if (Input.IsKeyDown(Key.A))
                velocity.x = -1.0f;

            velocity *= speed;

            Vector3 translation = Translation;
            translation.x += velocity.x * DeltaTime;
            translation.y += velocity.y * DeltaTime;
            Translation = translation;

        }
    }
}
