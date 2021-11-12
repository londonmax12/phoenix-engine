#include "phxpch.h"
#include "UUID.h"

#include <random>

namespace phx {

	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistrobution;

	UUID::UUID()
		: m_UUID(s_UniformDistrobution(s_Engine))
	{

	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}
}