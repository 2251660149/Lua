/*
 * File: delay.c 
 * Change Logs:
 * Date           Author       Notes
 * 2013-01-09     dzcn      first version
 */
#include "ddelay.h"
#include "delay.h"
static int _delay_ms(lua_State *L)
{
	int total,value;
	total=lua_gettop(L);
	if(total>1)
	{
		return luaL_error(L,"invalid value");
	}
	value= luaL_checkinteger( L, 1 );
	delay_ms(value);
	return 0;
}
const luaL_reg delay_map[]=
{
	{"ms",_delay_ms},
  {NULL,NULL}
};
LUALIB_API int luaopen_delay( lua_State *L )
{
	luaL_register( L, "delay", delay_map );
	return 0;
}
