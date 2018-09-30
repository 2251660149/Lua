#ifndef __USRDEF_H
#define __USRDEF_H
#define USR_REG_NUMBER( L, name, val ) lua_pushnumber( L, val );lua_setfield( L, -2, name )
#endif
