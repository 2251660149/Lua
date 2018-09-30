/*
 * File: mmcfs.c 
 * Change Logs:
 * Date           Author       Notes
 * 2012-12-26     dzcn      first version
 */
#include <string.h>
#include <stdio.h>
#include "ff.h"
#include "mmcfs.h"
#include <rt_sys.h>
#define MMCFS_MAX_FDS   4
static FIL mmcfs_fd_table[ MMCFS_MAX_FDS ];
static int mmcfs_num_fd;
static FATFS mmc_fs;
static FIL mmc_fileObject;
#define PATH_BUF_SIZE   40
static char mmc_pathbuf[PATH_BUF_SIZE];
int mmcfs_find_empty_fd( void )
{
  int i;

  for (i = 0; i < MMCFS_MAX_FDS; i ++)
    if (mmcfs_fd_table[i].fs == NULL)
      return i;
  return -1;
}

int mmc_open(const char * filename, int mode)
{

 
  int fd;
  int mmc_mode;
  if (mmcfs_num_fd == MMCFS_MAX_FDS)
  {
    return -1;
  }
  mmc_pathbuf[0] = 0;
  if (strchr(filename, '/') == NULL)
    strcat(mmc_pathbuf, "/");
  strcat(mmc_pathbuf, filename);
	switch(mode)
	{
		case 0:mmc_mode=FA_OPEN_EXISTING|FA_READ;break;		//r
		case 2:mmc_mode=FA_OPEN_EXISTING|FA_READ|FA_WRITE;break;	    //r+
		case 3:mmc_mode=FA_OPEN_ALWAYS|FA_READ;break;	    //rb+
		case 4:mmc_mode=FA_CREATE_ALWAYS|FA_WRITE;	break;		//w
		case 5:mmc_mode=FA_OPEN_ALWAYS|FA_WRITE;break;	    //wb
		case 6:mmc_mode=FA_CREATE_ALWAYS|FA_READ|FA_WRITE;break;	    //w+
		case 7:mmc_mode=FA_OPEN_ALWAYS|FA_READ|FA_WRITE;break;		//wb+
		case 8:mmc_mode=FA_OPEN_ALWAYS|FA_WRITE;break;		//a
		case 0xa:mmc_mode=FA_OPEN_ALWAYS|FA_READ|FA_WRITE;break;		 //a+
		case 0xb:mmc_mode=FA_OPEN_ALWAYS|FA_READ|FA_WRITE;break;		 //ab+
		case 18:mmc_mode=FA_OPEN_ALWAYS|FA_READ|FA_WRITE;break;		 //at+
	}

 
  // Open the file for reading
  if (f_open(&mmc_fileObject, mmc_pathbuf, mmc_mode) != FR_OK)
  {
    return -1;
  }
  if (mode & OPEN_A)
    mmc_fileObject.fptr = mmc_fileObject.fsize;
  fd = mmcfs_find_empty_fd();
  memcpy(mmcfs_fd_table + fd, &mmc_fileObject, sizeof(FIL));
  mmcfs_num_fd ++;
  return fd; 	 
}
int mmc_close(int handle)
{
	FRESULT res;
  FIL* pfile = mmcfs_fd_table + handle;
  res=f_close( pfile );
	if(res==FR_OK)
	{
	
		memset(pfile, 0, sizeof(FIL));
		mmcfs_num_fd --;
		}
  return 0;
}
size_t mmc_write(int handle, const unsigned char * buffer, size_t size)
{
  UINT bytes_written;
  if (f_write(mmcfs_fd_table + handle,  buffer, size, &bytes_written) != FR_OK)
  {
    return 0;
  }
  return (size_t) bytes_written;
}
size_t mmc_read(int handle, unsigned char * buffer, size_t size)
{
  UINT bytes_read;
  if (f_read(mmcfs_fd_table + handle, buffer, size, &bytes_read) != FR_OK)
  {
    return 0;
  }
  return (size_t) bytes_read;
}
long mmc_lseek(int handle, long offset, int whence)
{
  
  FIL* pfile = mmcfs_fd_table + handle;
  long newpos = 0;

  switch( whence )
  {
    case SEEK_SET:
      // seek from beginning of file
      newpos = offset;
      break;

    case SEEK_CUR:
      // seek from current position
      newpos = pfile->fptr + offset;
      break;

    case SEEK_END:
      // seek from end of file
      newpos = pfile->fsize + offset;
      break;

    default:
      return -1;
  }
  if (f_lseek (pfile, newpos) != FR_OK)
    return -1;
  return newpos;
}
long mmc_file_len(int handle)
{
	FIL* pfile = mmcfs_fd_table + handle;
	return pfile->fsize;
}
int mmcfs_init(void)
{
  // Mount the MMC file system using logical disk 0
  if ( f_mount( 0, &mmc_fs ) != FR_OK )
    return -1;
  else 
    return 0;
}
