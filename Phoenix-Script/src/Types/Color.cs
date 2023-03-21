using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phoenix
{
    public struct Color
    {
        public float r, g, b, a;

        public Color(float scalar = 0)
        {
            r = scalar;
            g = scalar;
            b = scalar;
            a = scalar;
        }
        public Color(float X, float Y, float Z, float A = 255)
        {
            r = X;
            g = Y;
            b = Z;
            a = A;
        }


        public static Color operator +(Color vector, float scalar)
        {
            return new Color(vector.r + scalar, vector.g + scalar, vector.b + scalar);
        }
        public static Color operator -(Color vector, float scalar)
        {
            return new Color(vector.r - scalar, vector.g - scalar, vector.b - scalar);
        }
        public static Color operator *(Color vector, float scalar)
        {
            return new Color(vector.r * scalar, vector.g * scalar, vector.b * scalar);
        }
        public static Color operator /(Color vector, float scalar)
        {
            return new Color(vector.r / scalar, vector.g / scalar, vector.b / scalar);
        }

        public static Color operator +(Color vector, Vector3 vector2)
        {
            return new Color(vector.r + vector2.x, vector.g + vector2.y, vector.b + vector2.z);
        }
        public static Color operator -(Color vector, Vector3 vector2)
        {
            return new Color(vector.r - vector2.x, vector.g - vector2.y, vector.b - vector2.z);
        }
        public static Color operator *(Color vector, Vector3 vector2)
        {
            return new Color(vector.r * vector2.x, vector.g * vector2.y, vector.b * vector2.z);
        }
        public static Color operator /(Color vector, Vector3 vector2)
        {
            return new Color(vector.r / vector2.x, vector.g / vector2.y, vector.b / vector2.z);
        }

        public static Color operator +(Color vector, Color vector2)
        {
            return new Color(vector.r + vector2.r, vector.g + vector2.g, vector.b + vector2.b);
        }
        public static Color operator -(Color vector, Color vector2)
        {
            return new Color(vector.r - vector2.r, vector.g - vector2.g, vector.b - vector2.b);
        }
        public static Color operator *(Color vector, Color vector2)
        {
            return new Color(vector.r * vector2.r, vector.g * vector2.g, vector.b * vector2.b);
        }
        public static Color operator /(Color vector, Color vector2)
        {
            return new Color(vector.r / vector2.r, vector.g / vector2.g, vector.b / vector2.b);
        }
    }
}
