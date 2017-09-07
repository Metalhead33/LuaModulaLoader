#include "module.h"
#include <string.h>
int lCreateModule(lua_State* L)
{
	/* Let's pre-declare the variables, as done in C */
	const char* name;
	size_t funcnums,i;
	module** udata;
	const char* temp_sref;

	name = luaL_checkstring(L, 1);
	funcnums = lua_objlen(L, 2);
	udata = (module**)lua_newuserdata(L, sizeof(module*));
	*udata = (module*)malloc(sizeof(module));
	(*udata)->lib = dlopen(name, RTLD_NOW | RTLD_LOCAL);
	if(!(*udata)->lib)
	{
		lua_pushstring(L, dlerror());
		lua_error(L);
		return 0;
	}
	(*udata)->funcnum = funcnums;
	(*udata)->funcs = (luaL_Reg*)malloc(sizeof(luaL_Reg) * (funcnums+1));
	/*
	  Stack:
	  1 / -3 - name/path (string)
	  2 / -2 - function list (table)
	  3 / -1 - the module's pointer (userdata)
	*/
	for(i=1;i<=funcnums;++i)
	{
		lua_rawgeti(L,2,i);
		/*
		  Stack:
		  1 / -4-i - name/path (string)
		  2 / -3-i - function list (table)
		  3 / -2-i - udata (user data)
		  3+i / -1 - the string
		*/
		// (*udata)->funcs[i-1].name = lua_tostring(L,-1);
		temp_sref = lua_tostring(L,-1);
		(*udata)->funcs[i-1].name = (const char*)malloc(sizeof(char) * strlen(temp_sref));
		strcpy((char*)(*udata)->funcs[i-1].name,temp_sref);
		lua_settop(L,3); /* Pop the unneeded string after copying*/
	}
	(*udata)->funcs[funcnums].name = 0;
	(*udata)->funcs[funcnums].func = 0;
	for(i=0;i<funcnums;++i)
	{
		(*udata)->funcs[i].func = (lua_CFunction)dlsym((*udata)->lib,(*udata)->funcs[i].name);
	}
	/*
		  Stack:
		  1 / -3 - name/path (string)
		  2 / -2 - function list (table)
		  3 / -1 - udata (user data)
	*/
	luaL_getmetatable(L, "_extend");
	/*
		  Stack:
		  1 / -4 - name/path (string)
		  2 / -3 - function list (table)
		  3 / -2 - udata (user data)
		  4 / -1 - meta
	*/
	lua_setmetatable(L, -2);
	/*
		  Stack:
		  1 / -3 - name/path (string)
		  2 / -2 - function list (table)
		  3 / -1 - meta
	*/
	return 1;
}
int lDestroyModule(lua_State* L)
{
	size_t i;
	MOD modula = lCheckModule(L,1);
	dlclose(modula->lib);
	for(i = 0; i < modula->funcnum;++i)
	{
		free(modula->funcs[i].name);
	}
	free(modula->funcs);
	free(modula);
	return 0;
}
MOD lCheckModule(lua_State* L, int index)
{
	return *((MOD*)(luaL_checkudata(L,index,"_extend")));
}
int lRegisterLib(lua_State* L)
{
	MOD modula = lCheckModule(L,1);
	lua_newtable(L);
	luaL_register(L, NULL, modula->funcs);
	return 1;
}
int lMetaRegisterLib(lua_State* L)
{
	MOD modula;
	const char* name;
	modula = lCheckModule(L,1);
	name = luaL_checkstring(L,2);
	luaL_newmetatable(L,name);
	luaL_register(L, NULL, modula->funcs);
	lua_pushvalue(L, -1);
	lua_setfield(L, -1, "__index");
	return 0;
}

void outputDebugInfo(FILE* output,MOD modula)
{
	size_t i;
	fprintf(output, "Module address: %p\n", (void*)modula);
	fprintf(output, "Module real library address: %p\n", modula->lib);
	fprintf(output, "Number of functions: %zu\n", modula->funcnum);
	for(i = 0;i < modula->funcnum;++i)
	{
		fprintf(output, "Function[%zu]: %s, %p\n", i, modula->funcs[i].name, (void*)modula->funcs[i].func);
	}
}
int lDebugInfo(lua_State *L)
{
	MOD modula = lCheckModule(L,1);
	outputDebugInfo(stdout,modula);
	return 0;
}

const luaL_Reg LuaLibFunctions[] = {
	{ "new", lCreateModule },
	{ "__call", lCreateModule },
	{ "__gc", lDestroyModule },
	{ "register", lRegisterLib },
	{ "metaRegister", lMetaRegisterLib },
	{ "debugInfo", lDebugInfo },
	{ NULL, NULL }
};

void RegisterLibraryWrapper(lua_State *L)
{
	luaL_newmetatable(L, "_extend");
	luaL_register(L, NULL, LuaLibFunctions);
	lua_pushvalue(L, -1);
	lua_setfield(L, -1, "__index");
	lua_setglobal(L, "extend");
}
