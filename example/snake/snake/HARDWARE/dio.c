/*
 * File: dio.c 
 * Change Logs:
 * Date           Author       Notes
 * 2013-01-08     dzcn      first version
 */

#include "dio.h"

static int dio_pin_mode(lua_State *L)
{
	int op,total,i,v;
	total=lua_gettop(L);//获取元素的个数
	if((total-1)>NUM_DIO)
	{
		return luaL_error( L, "invalid DIO" );
	}
	op= luaL_checkinteger( L, 1 );//获取模式
	if((op!=PLATFORM_DIO_IN)&&(op!=PLATFORM_DIO_OUT))
	{
		return luaL_error( L, "invalid direction" );
	}
	for(i=2;i<=total;i++)
	{
		v=luaL_checkinteger( L, i );  //获取IO引脚 0--21
		if(v>=NUM_DIO)
		{
			return luaL_error(L,"invalid DIO");
		}
		platform_dio_op(v,op);
	}
	return 1;
}
static int dio_write(lua_State *L)
{
	int total,i,v,value;
	total=lua_gettop(L);//获取要写的DIO引脚总数
	if(total>=NUM_DIO)
	{
		return luaL_error( L, "invalid DIO" );
	}
	value= luaL_checkinteger( L, 1 );
	if((value!=0)&&(value!=1))
	{
		return luaL_error( L, "invalid value" );
	}
	for(i=2;i<=total;i++)
	{
		v=luaL_checkinteger( L, i );
		if(v>=NUM_DIO)
		{
			return luaL_error(L,"invalid DIO");
		}
		platform_dio_op(v,value==1 ? PLATFORM_DIO_SET : PLATFORM_DIO_CLR);
	}
	return 1;
}
static int dio_read(lua_State *L)
{
	int total,i,v,value;
	total=lua_gettop(L);
	if(total>=NUM_DIO)
	{
		return luaL_error( L, "invalid DIO" );
	}
	for(i=1;i<=total;i++)
	{
		v=luaL_checkinteger( L, i );
		if(v>=NUM_DIO)
		{
			return luaL_error(L,"invalid DIO");
		}
		value=platform_dio_op(v,PLATFORM_DIO_GET);
		lua_pushinteger(L,value);
	}
    return total;
}

const luaL_reg dio_map[]=
{
	{"mode",dio_pin_mode},
	{"write",dio_write},
	{"read",dio_read},
	{NULL, NULL}
};

LUALIB_API int luaopen_dio( lua_State *L )
{
	luaL_register( L, "dio", dio_map );
	USR_REG_NUMBER(L,"INPUT",PLATFORM_DIO_IN);
	USR_REG_NUMBER(L,"OUTPUT",PLATFORM_DIO_OUT);
	USR_REG_NUMBER(L,"LOW",0);
	USR_REG_NUMBER(L,"HIGH",1);
	return 0;
}
