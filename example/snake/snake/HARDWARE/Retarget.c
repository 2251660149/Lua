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
//������Ƿ�Ϊ�ն�
int _sys_istty(FILEHANDLE fh)
{
	return 0;
}
int _sys_seek(FILEHANDLE fh, long pos)
{
	return mmc_lseek(fh, pos, SEEK_SET);
}
//ˢ�¾�������Ļ�����
int _sys_ensure(FILEHANDLE fh)
{
	return 0;
}

//�����ļ���ǰ����
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
//��һ���ַ�д�����̨
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
