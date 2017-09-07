#include <stdio.h>
#include "module.h"

void bail(lua_State *L, char *msg){
	fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n",
		msg, lua_tostring(L, -1));
	exit(1);
}

int main()
{
	lua_State *L;

	L = luaL_newstate();
	luaL_openlibs(L);
	RegisterLibraryWrapper(L);

	if (luaL_loadfile(L, "callfuncscript.lua")) /* Load but don't run the Lua script */
		bail(L, "luaL_loadfile() failed");      /* Error out if file can't be read */

	if (lua_pcall(L, 0, 0, 0))                  /* PRIMING RUN. FORGET THIS AND YOU'RE TOAST */
		bail(L, "lua_pcall() failed");          /* Error out if Lua file has an error */

	lua_close(L);
	printf("Hello World!\n");
	return 0;
}
