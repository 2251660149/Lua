#ifndef __DIO_H
#define __DIO_H
#include "platform.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "usrdef.h"
LUALIB_API int luaopen_dio( lua_State *L );
#endif
