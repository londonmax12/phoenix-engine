#include "phxpch.h"
#include "ProjectSerializer.h"

#include <yaml-cpp/yaml.h>

namespace phx {
	ProjectSerializer::ProjectSerializer(const Ref<Project>& project)
	{
		m_Project = project;
	}
	void ProjectSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value << m_Project->m_Name;
		out << YAML::Key << "AssetPath" << YAML::Value << m_Project->m_AssetPath;
		out << YAML::Key << "ActiveScene" << YAML::Value << m_Project->m_CurrentScene;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}
	Ref<Project> ProjectSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			return m_Project;
		}

		if (!data["Project"])
			return m_Project;

		std::string sceneName = data["Project"].as<std::string>();
		std::string activeScene = data["ActiveScene"].as<std::string>();
		std::string assetPath = data["AssetPath"].as<std::string>();

		m_Project = Project::Create(sceneName, filepath);
		m_Project->m_CurrentScene = activeScene;
		m_Project->m_AssetPath = assetPath;

		return m_Project;
	}
}