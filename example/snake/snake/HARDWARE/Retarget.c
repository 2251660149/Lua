/*
 * File: retarget.c 
 * Change Logs:
 * Date           Author       Notes
 * 2013-01-06     dzcn      first version
 */

#include <stdio.h>
#include <time.h>
#include <rt_sys.h>
#include "mmcfs.h"
typedef int FILEHANDLE;

#pragma import(__use_no_semihosting_swi)
#pragma import(_main_redirection)
const char __stdin_name[150];
const char __stdout_name[150];
const char __stderr_name[150];
FILEHANDLE _sys_open(const char *name, int openmode)
{


	return mmc_open(name, openmode);
}
int _sys_close(FILEHANDLE fh)
{
	 return mmc_close(fh);
}
int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
{
	return mmc_write(fh, buf, len);
}
int _sys_read(FILEHANDLE fh, unsigned char *buf, unsigned len, int mode)
{
	return mmc_read(fh, buf, len);
}
//检查句柄是否为终端
int _sys_istty(FILEHANDLE fh)
{
	return 0;
}
int _sys_seek(FILEHANDLE fh, long pos)
{
	return mmc_lseek(fh, pos, SEEK_SET);
}
//刷新句柄关联的缓冲区
int _sys_ensure(FILEHANDLE fh)
{
	return 0;
}

//返回文件当前长度
long _sys_flen(FILEHANDLE fh)
{
	return mmc_file_len(fh);
}
void _sys_exit(int status)
{
	//while(1);

}
int _sys_tmpnam(char *name, int fileno, unsigned maxlength)
{
	return 0;
}
//将一个字符写入控制台
void _ttywrch(int ch)
{
}
int remove(const char *filename)
{
	return 0;
}
char *_sys_command_string(char *cmd, int len)
{
 return NULL;
}
