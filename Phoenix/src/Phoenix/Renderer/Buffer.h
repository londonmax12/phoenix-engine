#pragma once
#include "Phoenix/Application/Core.h"

namespace phx {
	enum class ShaderDataType
	{
		None = 0, Float, vec2, vec3, vec4, mat3, mat4, Int, int2, int3, int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:return 4;
		case ShaderDataType::vec2: return 4 * 2;
		case ShaderDataType::vec3: return 4 * 3;
		case ShaderDataType::vec4: return 4 * 4;
		case ShaderDataType::mat3: return 4 * 3 * 3;
		case ShaderDataType::mat4: return 4 * 4 * 4;
		case ShaderDataType::Int:  return 4;
		case ShaderDataType::int2: return 4 * 2;
		case ShaderDataType::int3: return 4 * 3;
		case ShaderDataType::int4: return 4 * 4;
		case ShaderDataType::Bool: return 1;
		}
		return 0;
	}

	struct BufferElement
	{
		BufferElement() {}

		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:return 1;
			case ShaderDataType::vec2: return 2;
			case ShaderDataType::vec3: return 3;
			case ShaderDataType::vec4: return 4;
			case ShaderDataType::mat3: return 3 * 3;
			case ShaderDataType::mat4: return 4 * 4;
			case ShaderDataType::Int:  return 1;
			case ShaderDataType::int2: return 2;
			case ShaderDataType::int3: return 3;
			case ShaderDataType::int4: return 4;
			case ShaderDataType::Bool: return 1;
			}
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {  }
		BufferLayout(const std::initializer_list<BufferElement>& elements) 
			: m_Elements(elements) 
		{
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;

			for (auto& itr : m_Elements)
			{
				itr.Offset = offset;
				offset += itr.Size;
				m_Stride += itr.Size;
			}
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vetices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}