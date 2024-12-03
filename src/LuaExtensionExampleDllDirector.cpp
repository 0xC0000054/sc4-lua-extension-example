////////////////////////////////////////////////////////////////////////
//
// This file is part of sc4-lua-extensions-example, a DLL Plugin for
// SimCity 4 that demonstrates adding new C++ functions for the game's
// Lua system to call.
//
// Copyright (c) 2024 Nicholas Hayes
//
// This file is licensed under terms of the MIT License.
// See LICENSE.txt for more information.
//
////////////////////////////////////////////////////////////////////////

#include "cGZPersistResourceKey.h"
#include "cIGZFrameWork.h"
#include "cISC4AdvisorSystem.h"
#include "cISC4City.h"
#include "cIGZMessageServer2.h"
#include "cIGZMessageTarget2.h"
#include "cIGZString.h"
#include "cRZMessage2COMDirector.h"
#include "cRZMessage2Standard.h"
#include "cRZBaseString.h"
#include "GZServPtrs.h"
#include "Logger.h"
#include "LuaExtensions.h"
#include "SCLuaUtil.h"
#include "version.h"

#include <filesystem>
#include <Windows.h>
#include "wil/resource.h"
#include "wil/filesystem.h"

static constexpr uint32_t kSC4MessagePostCityInit = 0x26D31EC1;

static constexpr uint32_t kLuaExtensionExamplePluginDirectorID = 0xD034A68B;

static constexpr std::string_view PluginLogFileName = "SC4LuaExtensionExample.log";

namespace
{
	std::filesystem::path GetDllFolderPath()
	{
		wil::unique_cotaskmem_string modulePath = wil::GetModuleFileNameW(wil::GetModuleInstanceHandle());

		std::filesystem::path temp(modulePath.get());

		return temp.parent_path();
	}

	void RegisterLuaFunction(
		cISC4AdvisorSystem* pAdvisorSystem,
		const char* tableName,
		const char* functionName,
		lua_CFunction callback)
	{
		Logger& logger = Logger::GetInstance();

		SCLuaUtil::RegisterLuaFunctionStatus status = SCLuaUtil::RegisterLuaFunction(
			pAdvisorSystem,
			tableName,
			functionName,
			callback);

		if (status == SCLuaUtil::RegisterLuaFunctionStatus::Ok)
		{
			logger.WriteLineFormatted(
				LogLevel::Info,
				"Registered the %s.%s function",
				tableName,
				functionName);
		}
		else
		{
			if (status == SCLuaUtil::RegisterLuaFunctionStatus::NullParameter)
			{
				logger.WriteLineFormatted(
					LogLevel::Info,
					"Failed to register the %s.%s function. "
					"One or more SCLuaUtil::RegisterLuaFunction parameters were NULL.",
					tableName,
					functionName);
			}
			else if (status == SCLuaUtil::RegisterLuaFunctionStatus::TableWrongType)
			{
				logger.WriteLineFormatted(
					LogLevel::Info,
					"Failed to register the %s.%s function. The %s object is not a Lua table.",
					tableName,
					functionName,
					tableName);
			}
			else
			{
				logger.WriteLineFormatted(
					LogLevel::Info,
					"Failed to register the %s.%s function. "
					"Is LuaExtensionExample.dat in the plugins folder?",
					tableName,
					functionName);
			}
		}
	}
}

class LuaExtensionExampleDllDirector : public cRZMessage2COMDirector
{
public:

	LuaExtensionExampleDllDirector()
	{
		std::filesystem::path dllFolder = GetDllFolderPath();

		std::filesystem::path logFilePath = dllFolder;
		logFilePath /= PluginLogFileName;

		Logger& logger = Logger::GetInstance();

		logger.Init(logFilePath, LogLevel::Error);
		logger.WriteLogFileHeader("SC4LuaExtensionExample v" PLUGIN_VERSION_STR);
	}

	uint32_t GetDirectorID() const
	{
		return kLuaExtensionExamplePluginDirectorID;
	}

	void PostCityInit(cIGZMessage2Standard* pStandardMsg)
	{
		cISC4City* pCity = reinterpret_cast<cISC4City*>(pStandardMsg->GetIGZUnknown());

		if (pCity)
		{
			cISC4AdvisorSystem* pAdvisorSystem = pCity->GetAdvisorSystem();

			if (pAdvisorSystem)
			{
				const char* szTableName = "lua_extensions_example";

				RegisterLuaFunction(
					pAdvisorSystem,
					szTableName,
					"log2",
					LuaExtensions::Log2);
				RegisterLuaFunction(
					pAdvisorSystem,
					szTableName,
					"nam_version_string",
					LuaExtensions::NamVersionString);
			}
		}
	}

	bool DoMessage(cIGZMessage2* pMessage)
	{
		switch (pMessage->GetType())
		{
		case kSC4MessagePostCityInit:
			PostCityInit(static_cast<cIGZMessage2Standard*>(pMessage));
			break;
		}

		return true;
	}

	bool PostAppInit()
	{
		Logger& logger = Logger::GetInstance();

		cIGZMessageServer2Ptr pMsgServ;

		if (!pMsgServ || !pMsgServ->AddNotification(this, kSC4MessagePostCityInit))
		{
			logger.WriteLine(LogLevel::Error, "Failed to subscribe to the required notifications.");
			return false;
		}

		return true;
	}

	bool OnStart(cIGZCOM* pCOM)
	{
		mpFrameWork->AddHook(this);
		return true;
	}
};

cRZCOMDllDirector* RZGetCOMDllDirector() {
	static LuaExtensionExampleDllDirector sDirector;
	return &sDirector;
}