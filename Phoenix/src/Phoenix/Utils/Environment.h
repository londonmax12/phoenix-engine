#include <string>

namespace phx {
	class Environment {
	public:
		static bool HasEnvironmentalVariable(const std::string& key);
		static bool SetEnvironmentalVariable(const std::string& key, const std::string& value);
		static std::string GetEnvironmentalVariable(const std::string& key);
	};
}