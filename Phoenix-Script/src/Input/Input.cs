using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phoenix
{
    public class Input
    {
        public static bool IsKeyDown(Key key)
        {
            return InternalCalls.Input_IsKeyDown(key);
        }
    }
}
