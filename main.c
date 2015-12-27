#define LUA_LIB
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "main.h"
//#include "ogl.hpp"

int check_file( char * filename ) {
	// only check for read permission,
	// use F_OK for all, W_OK for write, and X_OK for execute.
	if( access( filename, R_OK ) != -1 ) {
    	return 1;
	} else {
    	return 0;
	}
}

// Lua error message
void runtime_error(lua_State *L, char *msg){
	fprintf(stderr, "\nFATAL ERROR:\n  %s: %s\n\n",
		msg, lua_tostring(L, -1));
	exit(1);
}

char simple_table(lua_State *L, const char *name, char *argv[], int start, int exit ) {
	
	//printf("DEBUG: CREATE_TABLE simple_table Name [%s] Start [%d] End [%d] Num [%d]\n", name, start, exit, (exit-start));
	
	if ( exit <= start || exit <= 0 || start < 0 ) return 1;
		
	lua_createtable(L, (exit - start), 0);
	
	int i; // for iteration?
    
    for (i=0; i<=(exit - start); i++) {
    	//printf("DEBUG: CREATE_TABLE simple_table __- Index [%d] Value [%s]\n", i, argv[start + i]);
    	lua_pushnumber(L, i);
		lua_pushstring(L, argv[start + i]);
		lua_settable(L, -3);
    }
    	
    lua_setglobal(L, name);
    
    return 0;
}

static int lua_print_reset (lua_State *L) {
	lua_pushnil(L);
	lua_setglobal(L, "print");
	return 1;
}

static int secret_function (lua_State *L) {
	int d = lua_tonumber(L, 1);  /* get argument */
	lua_pushnumber(L, d);  /* push result */
	return 1;  /* number of results */
}

static int engine_log (lua_State *L) {
	//const char * arg = lua_tostring(L, 1);
	printf("[LOG] %s\n", lua_tostring(L, 1));
	return 0;
}

static int engine_init (lua_State *L) {
	if (lua_gettop(L) != 3) {
		return luaL_error(L, "Usage: <window_width> <window_height> <window_title>");
	}
	int w = lua_tonumber(L, 1);
	int h = lua_tonumber(L, 2);
	const char *t = lua_tostring(L, 3);
	return engine_display_init( w, h, t );
}

int main ( int argc, char *argv[] ) {

	printf("Application Loader 0.1-alpha\n");
	
	lua_State *L;
	L = luaL_newstate();

	luaL_openlibs(L);

	if ( argc < 2 )
    {
        /* We print argv[0] assuming it is the program name */
        printf( "Usage: %s filename arguments", argv[0] );
    }
    else 
    {
    	
    	if (luaL_loadfile(L, argv[1]))
			runtime_error(L, "Failed to load lua script");
		
		// Construct "arg" table for use in Lua
		if (simple_table(L, "arg", argv, 1, argc ))
    		runtime_error(L, "Failed to construct arguments table");
    	
    	// Custom Functions
    	
    	lua_print_reset(L); // reset print
    	
    	lua_pushcfunction(L, secret_function);
    	lua_setglobal(L, "placebo");
    	
    	lua_pushcfunction(L, engine_log);
    	lua_setglobal(L, "engine_log");
    	
    	lua_pushcfunction(L, engine_init);
    	lua_setglobal(L, "engine_init");
    	
    	// End of Custom Functions
		
		// First call!
		// Very very important!
		if (lua_pcall(L, 0, 0, 0))	//if (lua_pcall(L, argc - 2, 1, 1))
			runtime_error(L, "lua_pcall() failed");
		
		// Call function main() - init
		lua_getglobal(L, "main");
		
		if (lua_pcall(L, 0, 0, 0))
			runtime_error(L, "lua_pcall() failed");
		
    }
    
    lua_close(L);
    
    exit(EXIT_SUCCESS);
    return 0;
    
}
