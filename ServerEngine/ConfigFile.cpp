#include "CommonConvert.h"
#include "ConfigFile.h"
#include "Log.h"

namespace ServerEngine
{
	ConfigFile::ConfigFile(void)
	{

	}

	ConfigFile::~ConfigFile(void)
	{

	}

	bool ConfigFile::Load(std::string strFileName)
	{
		FILE* pFile = NULL;
		auto err = fopen_s(&pFile, strFileName.c_str(), "r+");

		if (pFile != 0)
		{
			return false;
		}

		char szBuff[256] = { 0 };

		do
		{
			fgets(szBuff, 256, pFile);

			if (szBuff[0] == ';')
			{
				continue;
			}

			char* pChar = strchr(szBuff, '=');
			if (pChar == NULL)
			{
				continue;
			}

			std::string strName;
			strName.assign(szBuff, pChar - szBuff);
			std::string strValue = pChar + 1;

			ServerEngine::StringTrim(strName);
			ServerEngine::StringTrim(strValue);

			m_Values.insert(std::make_pair(strName, strValue));

		} while (!feof(pFile));

		fclose(pFile);


		return true;
	}


	ConfigFile* ConfigFile::GetInstancePtr()
	{
		static ConfigFile ConfigFile;

		return &ConfigFile;
	}

	std::string ConfigFile::GetStringValue(std::string strName)
	{
		auto itor = m_Values.find(strName);
		if (itor != m_Values.end())
		{
			return itor->second;
		}

		CLog::GetInstancePtr()->LogError("잘못된 구성 옵션: [%s]", strName.c_str());

		return "";
	}

	int32_t ConfigFile::GetIntValue(std::string VarName)
	{
		return atoi(GetStringValue(VarName).c_str());
	}

	float ConfigFile::GetFloatValue(std::string VarName)
	{
		return (float)atof(GetStringValue(VarName).c_str());
	}

	double ConfigFile::GetDoubleValue(std::string VarName)
	{
		return atof(GetStringValue(VarName).c_str());
	}


}