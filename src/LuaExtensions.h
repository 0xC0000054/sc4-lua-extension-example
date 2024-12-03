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

#pragma once
#include "cISCLua.h"

namespace LuaExtensions
{
	int32_t Log2(lua_State* pState);
	int32_t NamVersionString(lua_State* pState);
}