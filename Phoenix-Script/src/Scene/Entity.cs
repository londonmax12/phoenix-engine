using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phoenix
{
    public class Entity
    {
        protected Entity() { entityID = 0; }
        internal Entity(ulong entityId)
        {
            entityID = entityId;
        }

        public readonly ulong entityID;

        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(entityID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(entityID, ref value);
            }
        }
    }
}
