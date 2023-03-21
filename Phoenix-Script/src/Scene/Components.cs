using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phoenix
{
	public abstract class Component
	{
		public Entity Entity { get; internal set; }
	}

	public class TransformComponent : Component
	{
		public Vector3 Translation
		{
			get
			{
				InternalCalls.TransformComponent_GetTranslation(Entity.entityID, out Vector3 translation);
				return translation;
			}
			set
			{
				InternalCalls.TransformComponent_SetTranslation(Entity.entityID, ref value);
			}
		}
		public Vector3 Rotation
		{
			get
			{
				InternalCalls.TransformComponent_GetRotation(Entity.entityID, out Vector3 scale);
				return scale;
			}
			set
			{
				InternalCalls.TransformComponent_SetRotation(Entity.entityID, ref value);
			}
		}
		public Vector3 Scale
		{
			get
			{
				InternalCalls.TransformComponent_GetScale(Entity.entityID, out Vector3 scale);
				return scale;
			}
			set
			{
				InternalCalls.TransformComponent_SetScale(Entity.entityID, ref value);
			}
		}
	}
	public class SpriteRendererComponent : Component
	{
		public Color Color
        {
			get
			{
				InternalCalls.SpriteRenderComponent_GetColor(Entity.entityID, out Color color);
				return color;
			}
			set
			{
				InternalCalls.SpriteRenderComponent_SetColor(Entity.entityID, ref value);
			}
		}
	}


	public class Rigidbody2DComponent : Component
	{

		public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.entityID, ref impulse, ref worldPosition, wake);
		}

		public void ApplyLinearImpulse(Vector2 impulse, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.entityID, ref impulse, wake);
		}

	}
}
