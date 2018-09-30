#ifndef __DDELAY_H
#define __DDELAY_H
#include "platform.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
LUALIB_API int luaopen_delay( lua_State *L );
#endif
