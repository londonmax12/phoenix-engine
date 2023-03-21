using System;
using System.Runtime.CompilerServices;

namespace Phoenix
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);


        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static bool Input_IsKeyDown(Key key);
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static float Input_GetRelMousePositionX();
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static float Input_GetRelMousePositionY();
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static ulong Input_GetHoveredEntity();

        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void TransformComponent_GetRotation(ulong entityID, out Vector3 scale);
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void TransformComponent_SetRotation(ulong entityID, ref Vector3 scale);
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void TransformComponent_GetScale(ulong entityID, out Vector3 scale);
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void TransformComponent_SetScale(ulong entityID, ref Vector3 scale);

        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void SpriteRenderComponent_GetColor(ulong entityID, out Color color);
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void SpriteRenderComponent_SetColor(ulong entityID, ref Color color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void Rigidbody2DComponent_ApplyLinearImpulse(ulong entityID, ref Vector2 impulse, ref Vector2 point, bool wake);
        [MethodImplAttribute(MethodImplOptions.InternalCall)] internal extern static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(ulong entityID, ref Vector2 impulse, bool wake);
    }
}