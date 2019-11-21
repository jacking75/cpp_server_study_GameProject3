#pragma once

#include "Platform.h"

#include <unordered_map>
#include <string>


namespace ServerEngine
{
	class ConfigFile
	{
	private:
		ConfigFile(void);
		~ConfigFile(void);

	public:
		static ConfigFile* GetInstancePtr();

	public:
		bool Load(std::string strFileName);

		std::string GetStringValue(std::string strName);

		int32_t GetIntValue(std::string VarName);

		float GetFloatValue(std::string VarName);

		double GetDoubleValue(std::string VarName);

	private:
		std::unordered_map<std::string, std::string> m_Values;
	};

}