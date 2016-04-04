// MTFFS (Martin Trojer Flash File System)
// A very simple and unsafe filesystem for NAND flash devices.
// Copyright (c) 2007 Martin Trojer (martin.trojer@gmail.com)
// $WCREV$
//
// Written for eCos OS, 32bit
// No support for directoires, links, permission flags.
// unmount to write out the fat to disk!
// NOT thread safe, only one mount instance!

#ifndef __MTFFS_H
#define __MTFFS_H

//===========================================================================
// MTFFS Configs
//===========================================================================
#define MAGIC_STRING "MTFFS"
#define VERSION 0

#define FILENAME_SIZE 32

#ifndef CYGOPT_FS_MTFFS_USE_IO_FLASH
// These are also defined in flash.h
#define BLOCK_NUMBER 256
#define BLOCK_SIZE 0x1000
#define FLASH_START_ADDRESS 0x0
#endif

//===========================================================================
// eCOs includes
//===========================================================================
#include <pkgconf/system.h>
#include <pkgconf/hal.h>
#include <pkgconf/kernel.h>
#include <pkgconf/io_fileio.h>
#include <pkgconf/fs_mtffs.h>

#include <cyg/kernel/ktypes.h>         // base kernel types
#include <cyg/infra/cyg_trac.h>        // tracing macros
#include <cyg/infra/cyg_ass.h>         // assertion macros
#include <cyg/io/io.h>
#include <cyg/io/config_keys.h>

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>

#include <stdlib.h>
#include <string.h>

#include <cyg/fileio/fileio.h>

#include <cyg/kernel/kapi.h>
#include <cyg/infra/diag.h>

#ifdef USE_CYGPKG_IO_FLASH
#include <cyg/io/flash.h>
#endif

//===========================================================================
// Internal FS structures
//===========================================================================

//#define FAT_BLOCKS (1+(FAT_SIZE / BLOCK_SIZE))
//#define FILE_NUMBER BLOCK_NUMBER-FAT_BLOCKS
//#define FAT_SIZE FILE_NUMBER*(FILENAME_SIZE+4)+6

typedef struct
{
	char magic[5];
	unsigned char version;
	unsigned int fatsize;
	unsigned short fatblocks;
	unsigned short filenumber;
	// mtffs_fileentry files[mtffs_filenumber];	// File enries goes here
} mtffs_fat;

typedef struct
{
	char filename[FILENAME_SIZE];
	unsigned int filesize;
	short nextblock;		// block number
} mtffs_fileentry;

#define MAX_FAT_SIZE sizeof(mtffs_fat) + BLOCK_NUMBER*sizeof(mtffs_fileentry)

typedef struct
{
	cyg_io_handle_t dev;
	unsigned int blocksize;
	unsigned int blocknumber;
	unsigned int offset;
} mtffs_devinfo;

typedef struct
{
	unsigned short mtffs_fatblocks;
	unsigned short mtffs_filenumber;
	unsigned int mtffs_fatsize;
	unsigned char mtffs_initialized;
	unsigned char mtffs_mounted;
	mtffs_devinfo devinfo;
	unsigned char mtffs_fatbuffer[MAX_FAT_SIZE];
	unsigned char block_cache[BLOCK_SIZE];
} mtffs_fsstatus;

typedef struct
{
	short nextblock;		// 0: No more blocks -1: empty
	// Data goes here
} mtffs_block;

// These are globals, so no NOT thread safe!
mtffs_fsstatus mtffs_status;

// Local help functions
static int mtffs_init(void);
static int mtffs_mkfs(void);
static int mtffs_readfat(unsigned char *data);
static int mtffs_writefat(unsigned char *data);
static mtffs_fileentry *mtffs_findfile(char *filename);
static mtffs_fileentry *mtffs_emptyfe(void);
static int mtffs_emptyblock(unsigned short *blocknr);
static int mtffs_emptyblockcount(void);
static int mtffs_updateblock(mtffs_block *block, unsigned short blocknr, unsigned char *data);
static int mtffs_newfile(char *filename);
static int mtffs_allocnewblock(mtffs_fileentry *fe, unsigned short *newblocknr);
static int mtffs_pathconf( struct cyg_pathconf_info *info );

// Flash interface

int mtffs_flash_read(unsigned char *addr, unsigned char *data, unsigned int length);
int mtffs_flash_write(unsigned char *addr, unsigned char *data, unsigned int length);
int mtffs_flash_erase(unsigned short sector);

//===========================================================================
// eCos API
//===========================================================================

// Filesystem operations
static int mtffs_mount    ( cyg_fstab_entry *fste, cyg_mtab_entry *mte );
static int mtffs_umount   ( cyg_mtab_entry *mte );
static int mtffs_open     ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, int mode,  cyg_file *fte );
static int mtffs_unlink   ( cyg_mtab_entry *mte, cyg_dir dir, const char *name );
static int mtffs_mkdir    ( cyg_mtab_entry *mte, cyg_dir dir, const char *name );
static int mtffs_rmdir    ( cyg_mtab_entry *mte, cyg_dir dir, const char *name );
static int mtffs_rename   ( cyg_mtab_entry *mte, cyg_dir dir1, const char *name1, cyg_dir dir2, const char *name2 );
static int mtffs_link     ( cyg_mtab_entry *mte, cyg_dir dir1, const char *name1, cyg_dir dir2, const char *name2, int type );
static int mtffs_opendir  ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, cyg_file *fte );
static int mtffs_chdir    ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, cyg_dir *dir_out );
static int mtffs_stat     ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, struct stat *buf);
static int mtffs_getinfo  ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, int key, void *buf, int len );
static int mtffs_setinfo  ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, int key, void *buf, int len );

// File operations
static int mtffs_fo_read      (struct CYG_FILE_TAG *fp, struct CYG_UIO_TAG *uio);
static int mtffs_fo_write     (struct CYG_FILE_TAG *fp, struct CYG_UIO_TAG *uio);
static int mtffs_fo_lseek     (struct CYG_FILE_TAG *fp, off_t *pos, int whence );
static int mtffs_fo_ioctl     (struct CYG_FILE_TAG *fp, CYG_ADDRWORD com,
			       CYG_ADDRWORD data);
static int mtffs_fo_fsync     (struct CYG_FILE_TAG *fp, int mode );        
static int mtffs_fo_close     (struct CYG_FILE_TAG *fp);
static int mtffs_fo_fstat     (struct CYG_FILE_TAG *fp, struct stat *buf );
static int mtffs_fo_getinfo   (struct CYG_FILE_TAG *fp, int key, void *buf, int len );
static int mtffs_fo_setinfo   (struct CYG_FILE_TAG *fp, int key, void *buf, int len );

// Directory operations
static int mtffs_fo_dirread      (struct CYG_FILE_TAG *fp, struct CYG_UIO_TAG *uio);
static int mtffs_fo_dirlseek     (struct CYG_FILE_TAG *fp, off_t *pos, int whence );

//==========================================================================
// Filesystem table entries

// -------------------------------------------------------------------------
// Fstab entry.
// This defines the entry in the filesystem table.
// For simplicity we use _FILESYSTEM synchronization for all accesses since
// we should never block in any filesystem operations.

FSTAB_ENTRY( mtffs_fste, "mtffs", 0,
             CYG_SYNCMODE_FILE_FILESYSTEM|CYG_SYNCMODE_IO_FILESYSTEM,
             mtffs_mount,
             mtffs_umount,
             mtffs_open,
             mtffs_unlink,
             mtffs_mkdir,
             mtffs_rmdir,
             mtffs_rename,
             mtffs_link,
             mtffs_opendir,
             mtffs_chdir,
             mtffs_stat,
             mtffs_getinfo,
             mtffs_setinfo);

// -------------------------------------------------------------------------
// File operations.
// This set of file operations are used for normal open files.

static cyg_fileops mtffs_fileops =
{
    mtffs_fo_read,
    mtffs_fo_write,
    mtffs_fo_lseek,
    mtffs_fo_ioctl,
    cyg_fileio_seltrue,
    mtffs_fo_fsync,
    mtffs_fo_close,
    mtffs_fo_fstat,
    mtffs_fo_getinfo,
    mtffs_fo_setinfo
};

// -------------------------------------------------------------------------
// Directory file operations.
// This set of operations are used for open directories. Most entries
// point to error-returning stub functions. Only the read, lseek and
// close entries are functional.

static cyg_fileops mtffs_dirops =
{
    mtffs_fo_dirread,
    (cyg_fileop_write *)cyg_fileio_enosys,
    mtffs_fo_dirlseek,
    (cyg_fileop_ioctl *)cyg_fileio_enosys,
    cyg_fileio_seltrue,
    (cyg_fileop_fsync *)cyg_fileio_enosys,
    mtffs_fo_close,
    (cyg_fileop_fstat *)cyg_fileio_enosys,
    (cyg_fileop_getinfo *)cyg_fileio_enosys,
    (cyg_fileop_setinfo *)cyg_fileio_enosys
};

#endif
