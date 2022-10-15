using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phoenix
{
    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float scalar = 0)
        {
            x = scalar;
            y = scalar;
            z = scalar;
        }

        public Vector3(float X, float Y, float Z)
        {
            x = X;
            y = Y;
            z = Z;
        }


        public static Vector3 operator +(Vector3 vector, float scalar)
        {
            return new Vector3(vector.x + scalar, vector.y + scalar, vector.z + scalar);
        }
        public static Vector3 operator -(Vector3 vector, float scalar)
        {
            return new Vector3(vector.x - scalar, vector.y - scalar, vector.z - scalar);
        }
        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
        }
        public static Vector3 operator /(Vector3 vector, float scalar)
        {
            return new Vector3(vector.x / scalar, vector.y / scalar, vector.z / scalar);
        }

        public static Vector3 operator +(Vector3 vector, Vector3 vector2)
        {
            return new Vector3(vector.x + vector2.x, vector.y + vector2.y, vector.z + vector2.z);
        }
        public static Vector3 operator -(Vector3 vector, Vector3 vector2)
        {
            return new Vector3(vector.x - vector2.x, vector.y - vector2.y, vector.z - vector2.z);
        }
        public static Vector3 operator *(Vector3 vector, Vector3 vector2)
        {
            return new Vector3(vector.x * vector2.x, vector.y * vector2.y, vector.z * vector2.z);
        }
        public static Vector3 operator /(Vector3 vector, Vector3 vector2)
        {
            return new Vector3(vector.x / vector2.x, vector.y / vector2.y, vector.z / vector2.z);
        }
    }
}
