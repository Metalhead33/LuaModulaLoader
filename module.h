#ifndef MODULE_H
#define MODULE_H
#include <lua.h>
#include <lauxlib.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
typedef int (*lua_CFunction) (lua_State *L);
typedef struct TLIB {
	void* lib;
	luaL_Reg* funcs;
	size_t funcnum;
} module;
typedef module* MOD;

int lCreateModule(lua_State* L);
int lDestroyModule(lua_State* L);
MOD lCheckModule(lua_State* L, int index);
int lRegisterLib(lua_State* L);
int lMetaRegisterLib(lua_State* L);
void RegisterLibraryWrapper(lua_State *L);

void outputDebugInfo(FILE* output,MOD modula);
int lDebugInfo(lua_State *L);

// extern const luaL_Reg* LuaLibFunctions;

#endif // MODULE_H
