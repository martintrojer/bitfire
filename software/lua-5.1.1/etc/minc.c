#include <stdlib.h>
#include <stdio.h>

/* Include the Lua API header files. */
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int main(void)
{
  /* Declare a Lua State, open the Lua State and load the libraries (see above). */
  lua_State *l;
    init();
  l = lua_open();
  luaL_openlibs(l);
//  luaL_openlib(l,"base");

  printf("This line in directly from C\n\n");
  luaL_dostring(l, "print 'hej'");
  printf("\nBack to C again\n\n");

  /* Remember to destroy the Lua State */
  lua_close(l);

  return 0;
}
