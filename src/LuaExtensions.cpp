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

#include "LuaExtensions.h"
#include "cIGZString.h"
#include "cRZAutoRefCount.h"
#include "SCLuaUtil.h"
#include "StringResourceKey.h"
#include "StringResourceManager.h"
#include <math.h>

int32_t LuaExtensions::Log2(lua_State* pState)
{
	// First we must get a reference to the game's Lua type from the function state.
	// This type is used for retrieving function parameters and setting return values.

	cRZAutoRefCount<cISCLua> pLua = SCLuaUtil::GetISCLuaFromFunctionState(pState);

	int parameterCount = pLua->GetTop();

	// The method takes 1 number parameter.
	// The negative arguments retrieve the function parameters that are passed
	// on the stack.
	// The first parameter is at -1, the second parameter is at -2, and so on.

	if (parameterCount == 1
		&& pLua->Type(-1) == cIGZLua5Thread::LuaTypeNumber)
	{
		double value = pLua->ToNumber(-1);

		// The C/C++ log2 function doesn't support negative numbers or 0, so we only
		// send it values greater than 1.0.
		// The log2 of 1 is 0, so our error behavior of returning 0 is correct for
		// that case.

		if (value > 1.0)
		{
			// Return the result and indicate that we are returning 1 value.
			pLua->PushNumber(log2(value));
			return 1;
		}
	}

	pLua->PushNumber(0.0);
	return 1;
}

int32_t LuaExtensions::NamVersionString(lua_State* pState)
{
	// First we must get a reference to the game's Lua type from the function state.
	// This type is used for retrieving function parameters and setting return values.

	cRZAutoRefCount<cISCLua> pLua = SCLuaUtil::GetISCLuaFromFunctionState(pState);

	// This function doesn't take any parameters.

	cRZAutoRefCount<cIGZString> pVersionString;

	if (StringResourceManager::GetString(
		StringResourceKey(0x123006aa, 0x6a47ffff),
		pVersionString.AsPPObj()))
	{
		// Return the result and indicate that we are returning 1 value.

		pLua->PushLString(pVersionString->ToChar(), pVersionString->Strlen());
		return 1;
	}

	pLua->PushString("");
	return 1;
}
