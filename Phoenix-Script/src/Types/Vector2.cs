using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phoenix
{
    public class Vector2
    {
        public float x, y;

        public Vector2(float scalar = 0)
        {
            x = scalar;
            y = scalar;
        }

        public Vector2(float X, float Y)
        {
            x = X;
            y = Y;
        }


        public static Vector2 operator +(Vector2 vector, float scalar)
        {
            return new Vector2(vector.x + scalar, vector.y + scalar);
        }
        public static Vector2 operator -(Vector2 vector, float scalar)
        {
            return new Vector2(vector.x - scalar, vector.y - scalar);
        }
        public static Vector2 operator *(Vector2 vector, float scalar)
        {
            return new Vector2(vector.x * scalar, vector.y * scalar);
        }
        public static Vector2 operator /(Vector2 vector, float scalar)
        {
            return new Vector2(vector.x / scalar, vector.y / scalar);
        }

        public static Vector2 operator +(Vector2 vector, Vector2 vector2)
        {
            return new Vector2(vector.x + vector2.x, vector.y + vector2.y);
        }
        public static Vector2 operator -(Vector2 vector, Vector2 vector2)
        {
            return new Vector2(vector.x - vector2.x, vector.y - vector2.y);
        }
        public static Vector2 operator *(Vector2 vector, Vector2 vector2)
        {
            return new Vector2(vector.x * vector2.x, vector.y * vector2.y);
        }
        public static Vector2 operator /(Vector2 vector, Vector2 vector2)
        {
            return new Vector2(vector.x / vector2.x, vector.y / vector2.y);
        }
    }
}
