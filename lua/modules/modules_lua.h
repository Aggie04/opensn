#pragma once

#include "framework/lua.h"

namespace chi_modules
{
namespace lua_utils
{
void LoadRegisteredLuaItems();
void RegisterLuaEntities(lua_State* L);
} // namespace lua_utils
} // namespace chi_modules