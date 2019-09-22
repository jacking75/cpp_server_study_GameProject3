#pragma once

#include <unordered_map>
#include <string>

#include "Platform.h"

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

		INT32 GetIntValue(std::string VarName);

		FLOAT GetFloatValue(std::string VarName);

		DOUBLE GetDoubleValue(std::string VarName);

	private:
		std::unordered_map<std::string, std::string> m_Values;
	};

}