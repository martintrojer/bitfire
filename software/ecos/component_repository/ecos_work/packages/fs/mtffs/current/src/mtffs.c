// MTFFS (Martin Trojer Flash File System)
// A very simple and unsafe filesystem for NAND flash devices
// Copyright (c) 2007 Martin Trojer (martin.trojer@gmail.com)
// $WCREV$

#include "mtffs.h"

#if 0
void check(void)
{
	char buffer[512];
	int i;
	for (i=33;i<36;i++)
		mtffs_flash_read(mtffs_status.devinfo.blocksize*i,buffer,512);
}
#endif

//===========================================================================
// eCos API
//===========================================================================

// Filesystem operations
static int mtffs_mount    ( cyg_fstab_entry *fste, cyg_mtab_entry *mte )
{
	Cyg_ErrNo err;
	cyg_uint32 len;
	
	int force=1;
	mtffs_fat fat;
	
#ifdef CYGOPT_FS_MTFFS_USE_IO_FLASH
	cyg_io_flash_getconfig_devsize_t ds;
	cyg_io_flash_getconfig_blocksize_t bs;
	cyg_io_handle_t t;
	
	err = cyg_io_lookup(mte->devname, &t);
	if (err != ENOERR)
		return -err;
	
	mtffs_status.devinfo.dev = t;
	
	len = sizeof(ds);
	err = cyg_io_get_config(mtffs_status.devinfo.dev, CYG_IO_GET_CONFIG_FLASH_DEVSIZE, &ds, &len);
	if (err != ENOERR) return err;
	len = sizeof(bs);
	bs.offset = 0;
	err = cyg_io_get_config(mtffs_status.devinfo.dev, CYG_IO_GET_CONFIG_FLASH_BLOCKSIZE, &bs, &len);
	if (err != ENOERR) return err;

	mtffs_status.devinfo.blocksize = bs.block_size;
	mtffs_status.devinfo.offset = bs.offset;
	mtffs_status.devinfo.blocknumber = ds.dev_size / bs.block_size;
#else
	mtffs_status.devinfo.dev = 0;
	mtffs_status.devinfo.blocksize = BLOCK_SIZE;
	mtffs_status.devinfo.offset = FLASH_START_ADDRESS;
	mtffs_status.devinfo.blocknumber = BLOCK_NUMBER;
#endif
	
	if (!mtffs_status.mtffs_initialized)
		mtffs_init();
	
	if (mtffs_status.mtffs_mounted)
		return EBUSY;
	
	while(1)
	{
		// Read FAT from flash.
		len = sizeof(mtffs_fat);
		err = mtffs_flash_read(0,(unsigned char *)&fat,sizeof(mtffs_fat));
		if (err != ENOERR) return err;
		
		// Valid FAT?
		if (strcmp(fat.magic,MAGIC_STRING)!=0 || fat.version!=VERSION) {
			if (force) {
				err = mtffs_mkfs();
				if (err != ENOERR) return err;
				force = 0;
				continue;
			} else
				return ENXIO;
		} else
			break;
	}
	
	mtffs_status.mtffs_fatblocks = fat.fatblocks;
	mtffs_status.mtffs_filenumber = fat.filenumber;
	mtffs_status.mtffs_fatsize = fat.fatsize;
	
	/*
	// Malloc FAT mem buffer for caching of entrire fat.
	mtffs_status.mtffs_fatbuffer = malloc(mtffs_status.mtffs_fatsize);
	if (mtffs_status.mtffs_fatbuffer==NULL)
		return ENOMEM;
	*/
		
	err = mtffs_readfat(mtffs_status.mtffs_fatbuffer);
	if (err != ENOERR) return err;
			
	mtffs_status.mtffs_mounted = -1;

	return ENOERR;
}
static int mtffs_umount   ( cyg_mtab_entry *mte )
{
	int err;
	err = mtffs_writefat(mtffs_status.mtffs_fatbuffer);
	if (err != ENOERR) return err;
	
	mtffs_status.mtffs_mounted = mtffs_status.mtffs_initialized = 0;
	mtffs_status.mtffs_fatblocks = mtffs_status.mtffs_filenumber = mtffs_status.mtffs_fatsize = 0;
	/*
	if (mtffs_status.mtffs_fatbuffer!=NULL)
		free(mtffs_status.mtffs_fatbuffer);
	mtffs_status.mtffs_fatbuffer = NULL;
	*/
	return ENOERR;
}
static int mtffs_open     ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, int mode,  cyg_file *file )
{
	Cyg_ErrNo err;
	mtffs_fileentry *fe = mtffs_findfile(name);
	if (fe==NULL)
	{
		if ((mode & O_CREAT)) {
			// Create a new file
			err = mtffs_newfile(name);
			if (err != ENOERR) return err;
		}
	}
	else
	{
		// The node exists. If the O_CREAT and O_EXCL bits are set, we
		// must fail the open
		if( (mode & (O_CREAT|O_EXCL)) == (O_CREAT|O_EXCL) )
			err = EEXIST;
	}
	// FIXME: Handle the O_TRUNC flag
	
	fe = mtffs_findfile(name);
	if (fe==NULL)
		return EIO;
	
	file->f_flag        |= mode & CYG_FILE_MODE_MASK;
	file->f_type        = CYG_FILE_TYPE_FILE;
	file->f_ops         = &mtffs_fileops;
	file->f_offset      = (mode&O_APPEND) ? fe->filesize : 0;
	file->f_data        = (CYG_ADDRWORD)fe;
	file->f_xops        = 0;
	
	return ENOERR;
}
static int mtffs_unlink   ( cyg_mtab_entry *mte, cyg_dir dir, const char *name )
{
	mtffs_fileentry empty_fe;
	mtffs_fileentry *fe = mtffs_findfile(name);
	mtffs_block b;
	int err;
	if (fe==NULL)
		return ENOENT;
	
	b.nextblock = fe->nextblock;
	// remove from FAT
	memset(empty_fe.filename,0,FILENAME_SIZE);
	empty_fe.filesize = 0;
	empty_fe.nextblock = 0;
	memcpy(fe,&empty_fe,sizeof(mtffs_fileentry));
	
	// Erase the blocks
	while(b.nextblock!=0) {
		mtffs_block nextblock;
		err = mtffs_flash_read((unsigned char*)(b.nextblock*mtffs_status.devinfo.blocksize),(unsigned char*)&nextblock,sizeof(mtffs_block));
		if (err != ENOERR) return err;
		err = mtffs_flash_erase(b.nextblock);
		if (err != ENOERR) return err;
 		b = nextblock;
	}
	
	err = mtffs_writefat(mtffs_status.mtffs_fatbuffer);
	if (err != ENOERR) return err;
	
	return ENOERR;
}
static int mtffs_mkdir    ( cyg_mtab_entry *mte, cyg_dir dir, const char *name )
{
	return EEXIST;
}
static int mtffs_rmdir    ( cyg_mtab_entry *mte, cyg_dir dir, const char *name )
{
	return EPERM;
}
static int mtffs_rename   ( cyg_mtab_entry *mte, cyg_dir dir1, const char *name1, cyg_dir dir2, const char *name2 )
{
	Cyg_ErrNo err;
	mtffs_fileentry *fe = mtffs_findfile(name1);
	if (fe==NULL)
		return ENOENT;
	if (strlen(name2)>FILENAME_SIZE)
		return ENAMETOOLONG;
	strcpy(fe->filename,name2);
		
	return ENOERR;
}
static int mtffs_link     ( cyg_mtab_entry *mte, cyg_dir dir1, const char *name1, cyg_dir dir2, const char *name2, int type )
{
	return EINVAL;
}
static int mtffs_opendir  ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, cyg_file *file )
{		
	file->f_type        = CYG_FILE_TYPE_FILE;
	file->f_ops         = &mtffs_dirops;
	file->f_offset      = 0;
	file->f_data        = 0;
	file->f_xops        = 0;
	return ENOERR;
}
static int mtffs_chdir    ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, cyg_dir *dir_out )
{
	return EINVAL;
}
static int mtffs_stat     ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, struct stat *buf)
{
	Cyg_ErrNo err;
	mtffs_fileentry *fe = mtffs_findfile(name);
	if (fe==NULL)
		return ENOENT;
		
    	// Fill in the status
	buf->st_mode        = 0;
	buf->st_ino         = 0;
	buf->st_dev         = 0;
	buf->st_nlink       = 0;
	buf->st_uid         = 0;
	buf->st_gid         = 0;
	buf->st_size        = fe->filesize;
	buf->st_atime       = 0;
	buf->st_mtime       = 0;
	buf->st_ctime       = 0;
	
	return ENOERR;
}
static int mtffs_getinfo  ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, int key, void *buf, int len )
{
	Cyg_ErrNo err;
	switch( key )
	{
		case FS_INFO_CONF:
			err = mtffs_pathconf( (struct cyg_pathconf_info *)buf );
			break;
#ifdef CYGSEM_FILEIO_INFO_DISK_USAGE
		case FS_INFO_DISK_USAGE:
		{
			struct cyg_fs_disk_usage *usage = (struct cyg_fs_block_usage *) buf;

			usage->total_blocks = mtffs_status.devinfo.blocknumber;
			usage->free_blocks = mtffs_emptyblockcount();
			usage->block_size = mtffs_status.devinfo.blocksize;
			return ENOERR;
		}
#endif
		default:
			err = EINVAL;
	}
	return err;
}
static int mtffs_setinfo  ( cyg_mtab_entry *mte, cyg_dir dir, const char *name, int key, void *buf, int len )
{
	return ENOERR;
}

// File operations
static int mtffs_fo_read      (struct CYG_FILE_TAG *fp, struct CYG_UIO_TAG *uio)
{
	Cyg_ErrNo err;
	mtffs_fileentry *fe = fp->f_data;
	unsigned char *addr;
	int i;
	off_t pos = fp->f_offset;
	ssize_t resid = uio->uio_resid;
	
	if (fe->nextblock==0)	// Nothing to read
		return ENOERR;
		
    // Loop over the io vectors until there are none left
	for( i = 0; i < uio->uio_iovcnt; i++ )
	{
		cyg_iovec *iov = &uio->uio_iov[i];
		unsigned char *buf = (char *)iov->iov_base;
		off_t len = iov->iov_len;
		
		// Loop over each vector filling it with data from the file.
		while( len > 0 && pos < fe->filesize )
		{			
			// Find the block in which pos points
			int tmppos = mtffs_status.devinfo.blocksize-sizeof(mtffs_block);
			int tlen = len;
			int skip_count=0;
			
			mtffs_block b;
			b.nextblock = fe->nextblock;

			while(tmppos<=pos) {
				if (b.nextblock==0)		// last block?
					return ESPIPE;
				// Read next block header
				addr = b.nextblock * mtffs_status.devinfo.blocksize;
				err = mtffs_flash_read(addr,(unsigned char*)&b,sizeof(mtffs_block));
				if (err != ENOERR) return err;				
				// block pointer sanity check
				if (b.nextblock==0 || b.nextblock>mtffs_status.devinfo.blocknumber)
					return ESPIPE;
				tmppos += mtffs_status.devinfo.blocksize-sizeof(mtffs_block);
				skip_count++;
			}			
			
			int block_offset = (pos - skip_count*mtffs_status.devinfo.blocksize) + ((1+skip_count)*sizeof(mtffs_block));
			// How much to read from this block?
			if (len>(mtffs_status.devinfo.blocksize-block_offset))
				tlen = mtffs_status.devinfo.blocksize-block_offset;
			
			if ((pos+tlen)>fe->filesize)
				tlen = fe->filesize - pos;
					
			addr = (b.nextblock * mtffs_status.devinfo.blocksize) + block_offset;
			err = mtffs_flash_read(addr,buf,tlen);
			if (err != ENOERR) return err;
			len -= tlen;
			buf += tlen;
			pos += tlen;
			resid -= tlen;
		}
	}
	
	uio->uio_resid = resid;
	fp->f_offset = pos;
	return ENOERR;
}
static int mtffs_fo_write     (struct CYG_FILE_TAG *fp, struct CYG_UIO_TAG *uio)
{
	mtffs_fileentry *fe = fp->f_data;
	off_t pos = fp->f_offset;
	ssize_t resid = uio->uio_resid;    
	int err = ENOERR;
	int i;
	unsigned char *addr;
   
	// If the APPEND mode bit was supplied, force all writes to
    // the end of the file.
	if( fp->f_flag & CYG_FAPPEND )
		pos = fp->f_offset = fe->filesize;
	
    // Now loop over the iovecs until they are all done, or
    // we get an error.
	for( i = 0; i < uio->uio_iovcnt; i++ )
	{
		cyg_iovec *iov = &uio->uio_iov[i];
		char *buf = (char *)iov->iov_base;
		off_t len = iov->iov_len;

        // loop over the vector writing it to the file until it has
        // all been done.
		while( len > 0 )
		{				
			// Find the block in which pos points
			int tmppos = mtffs_status.devinfo.blocksize-sizeof(mtffs_block);
			int tlen = len;
			int skip_count=0;			
			mtffs_block tb,b;
			
			b.nextblock = fe->nextblock;
			
			if (b.nextblock == 0) {
				// We need to allocate more space for this file
				err = mtffs_allocnewblock(fe,&b.nextblock);
				if (err != ENOERR) return err;
			}
			
			while(tmppos<=pos) 
			{
				addr = b.nextblock * mtffs_status.devinfo.blocksize;
				err = mtffs_flash_read(addr,(unsigned char*)&b,sizeof(mtffs_block));
				if (err != ENOERR) return err;				
					// block pointer sanity check
				if (b.nextblock>mtffs_status.devinfo.blocknumber)
					return ESPIPE;
				
				if (b.nextblock == 0) {
				// We need to allocate more space for this file
					err = mtffs_allocnewblock(fe,&b.nextblock);
					if (err != ENOERR) return err;
				}
				
				tmppos += mtffs_status.devinfo.blocksize-sizeof(mtffs_block);
				skip_count++;
			}			
			
			int block_offset = (pos - skip_count*mtffs_status.devinfo.blocksize) + ((1+skip_count)*sizeof(mtffs_block));
			// How much to write from this block?
			if (len>(mtffs_status.devinfo.blocksize-block_offset))
				tlen = mtffs_status.devinfo.blocksize-block_offset;
			
			// and what block does this block link to? Needed when updating it.
			addr = b.nextblock * mtffs_status.devinfo.blocksize;
			err = mtffs_flash_read(addr,(unsigned char*)&tb,sizeof(mtffs_block));
			if (err != ENOERR) return err;				
			if (tb.nextblock>mtffs_status.devinfo.blocknumber || tb.nextblock==-1)
				tb.nextblock=0;

			// Write actual data to block
			/*
			block_cache = malloc(mtffs_status.devinfo.blocksize);
			if (block_cache == NULL)
				return ENOMEM;
			*/
			addr = (b.nextblock * mtffs_status.devinfo.blocksize);
			err = mtffs_flash_read(addr,mtffs_status.block_cache,mtffs_status.devinfo.blocksize);
			if (err != ENOERR) return err;
			
			memcpy(mtffs_status.block_cache+block_offset,buf,tlen);
			err = mtffs_updateblock(&tb,b.nextblock,mtffs_status.block_cache);
			if (err != ENOERR) return err;

			len -= tlen;
			buf += tlen;
			pos += tlen;
			resid -= tlen;
		}	
	}
	// update FAT with filesize
	if (pos>fe->filesize)
		fe->filesize = pos;
	
	uio->uio_resid = resid;
	fp->f_offset = pos;

//	check();
	return err;
}
static int mtffs_fo_lseek     (struct CYG_FILE_TAG *fp, off_t *apos, int whence )
{
	mtffs_fileentry *fe = fp->f_data;
	off_t pos = *apos;

	switch( whence )
	{
		case SEEK_SET:
        // Pos is already where we want to be.
			break;

		case SEEK_CUR:
        // Add pos to current offset.
			pos += fp->f_offset;
			break;

		case SEEK_END:
        // Add pos to file size.
			pos += fe->filesize;
			break;

		default:
			return EINVAL;
	}
    
    // All OK, set fp offset and return new position.
	*apos = fp->f_offset = pos;
    
	return ENOERR;
}
static int mtffs_fo_ioctl     (struct CYG_FILE_TAG *fp, CYG_ADDRWORD com, CYG_ADDRWORD data)
{
	return EINVAL;
}
static int mtffs_fo_fsync     (struct CYG_FILE_TAG *fp, int mode )
{
	return ENOERR;
}
static int mtffs_fo_close     (struct CYG_FILE_TAG *fp)
{
	mtffs_fileentry *fe = fp->f_data;
	fp->f_data = 0;     // zero data pointer
	return ENOERR;
}
static int mtffs_fo_fstat     (struct CYG_FILE_TAG *fp, struct stat *buf )
{
	mtffs_fileentry *fe = fp->f_data;
    
	// Fill in the status
	buf->st_mode        = 0;
	buf->st_ino         = 0;
	buf->st_dev         = 0;
	buf->st_nlink       = 0;
	buf->st_uid         = 0;
	buf->st_gid         = 0;
	buf->st_size        = fe->filesize;
	buf->st_atime       = 0;
	buf->st_mtime       = 0;
	buf->st_ctime       = 0;
	return ENOERR;
}
static int mtffs_fo_getinfo   (struct CYG_FILE_TAG *fp, int key, void *buf, int len )
{
	mtffs_fileentry *fe = fp->f_data;
	int err;

	switch( key )
	{
		case FS_INFO_CONF:
			err = mtffs_pathconf( (struct cyg_pathconf_info *)buf );
			break;
        
		default:
			err = EINVAL;
	}
	return err;
}
static int mtffs_fo_setinfo   (struct CYG_FILE_TAG *fp, int key, void *buf, int len )
{
	return ENOERR;
}

// Directory operations
static int mtffs_fo_dirread      (struct CYG_FILE_TAG *fp, struct CYG_UIO_TAG *uio)
{
	struct dirent *ent = (struct dirent *)uio->uio_iov[0].iov_base;
	off_t pos = fp->f_offset;
	char *nbuf = ent->d_name;
	int nlen = sizeof(ent->d_name)-1;
	off_t len = uio->uio_iov[0].iov_len;
	
	void *fatend = (unsigned int)mtffs_status.mtffs_fatbuffer + mtffs_status.mtffs_fatsize;
	mtffs_fileentry *fe = (mtffs_fileentry*) pos;

	if( len < sizeof(struct dirent) )
		return EINVAL;    

	// pos points into fat at the next fileentry
	fe = (unsigned int)mtffs_status.mtffs_fatbuffer+sizeof(mtffs_fat)+pos;
	
	//find next file
	while ((void*)fe < fatend)
	{
		if (fe->filename[0]!=0)
			break;
		fe++;
	}
	
	if ((void *)fe!=fatend)
	{
		strcpy(nbuf,fe->filename);
		uio->uio_resid -= sizeof(struct dirent);
		fe++;
		fp->f_offset = (unsigned int)fe - (unsigned int)(mtffs_status.mtffs_fatbuffer+sizeof(mtffs_fat));
		
	}

	return ENOERR;
}
static int mtffs_fo_dirlseek     (struct CYG_FILE_TAG *fp, off_t *pos, int whence )
{
    // Only allow SEEK_SET to zero
    
	if( whence != SEEK_SET || *pos != 0)
		return EINVAL;

	*pos = fp->f_offset = 0;
    
	return ENOERR;
}

//===========================================================================
// Help Fuctions
//===========================================================================

// mtffs_init
// mainly used by mtffs_mkfs to calculate file numner, size of FAT
static int mtffs_init(void)
{
	// Calculate how many files we can support and size of the FAT.
	// Note that MTFFS only supports ont file per block, but the
	// both the FAT and files can span many blocks.
	int fesize = sizeof(mtffs_fileentry);
	int fatsize = sizeof(mtffs_fat);
		
	mtffs_status.mtffs_filenumber = mtffs_status.devinfo.blocknumber-1;
	mtffs_status.mtffs_fatsize = fatsize + mtffs_status.mtffs_filenumber*fesize;
	mtffs_status.mtffs_fatblocks = 1 + (mtffs_status.mtffs_fatsize / mtffs_status.devinfo.blocksize);
	
	while (mtffs_status.mtffs_fatblocks+mtffs_status.mtffs_filenumber > mtffs_status.devinfo.blocknumber) {
		mtffs_status.mtffs_filenumber--;
		mtffs_status.mtffs_fatsize = fatsize + mtffs_status.mtffs_filenumber*fesize;
		mtffs_status.mtffs_fatblocks = 1 + (mtffs_status.mtffs_fatsize / mtffs_status.devinfo.blocksize);
	}
	
#ifdef CYGOPT_FS_MTFFS_USE_M25PXX_FLASH
	{
		extern int flash_hwr_init(void);
		flash_hwr_init();
	}
#endif
	
	mtffs_status.mtffs_initialized = -1;
	
	return ENOERR;
}

// mtffs_mkffs				
// write out an empty filesystem on the flash
static int mtffs_mkfs(void)
{
	mtffs_fileentry fe;
	mtffs_fat fat;
	int i,err;
	
	if (!mtffs_status.mtffs_initialized) return ENXIO;
	
	// Make an empty fileentry
	memset(fe.filename,0,FILENAME_SIZE);
	fe.filesize = 0;
	fe.nextblock = 0;
	
	/*
	// Allocated the FAT buffer
	buf = malloc(mtffs_status.mtffs_fatsize);
	if (buf==NULL) return ENOMEM;
	*/
	
	// Initialize and empty FAT
	memcpy(fat.magic,MAGIC_STRING,5);
	fat.version = VERSION;
	fat.fatsize = mtffs_status.mtffs_fatsize;
	fat.fatblocks = mtffs_status.mtffs_fatblocks;
	fat.filenumber = mtffs_status.mtffs_filenumber;
	memcpy(mtffs_status.mtffs_fatbuffer,&fat,sizeof(mtffs_fat));
	for(i=0;i<mtffs_status.mtffs_filenumber;i++)
		memcpy(mtffs_status.mtffs_fatbuffer+sizeof(mtffs_fat)+i*sizeof(mtffs_fileentry),&fe,sizeof(mtffs_fileentry));
	
	printf("mkfs(): erasing flash...\n");
	// Erase entire device
	for (i=0;i<mtffs_status.devinfo.blocknumber;i++) {
		err = mtffs_flash_erase(i);
		if (err != ENOERR) return err;
	}
	
	// Write the empty FAT
	err = mtffs_writefat(mtffs_status.mtffs_fatbuffer);

	return err;
}

// mtffs_readfat
// Read the FAT into a memory buffer, this should only be done by mount.
// Make sure mtffs_fatsize is correct, mount/mkfs will update this.
static int mtffs_readfat(unsigned char *data)
{	
	if (!mtffs_status.mtffs_initialized) return ENXIO;
	
	return mtffs_flash_read(0, data, mtffs_status.mtffs_fatsize);
}

// mtffs_writefat
// Erase FAT blocks and write out the FAT to the flash device.
// Make sure mtffs_fatsize is correct, mount/mkfs will update this.
static int mtffs_writefat(unsigned char *data)
{
	int i;
	Cyg_ErrNo err;
	
	if (!mtffs_status.mtffs_initialized) return ENXIO;

	// Erase FAT blocks
	for (i=0;i<mtffs_status.mtffs_fatblocks;i++) {
		err = mtffs_flash_erase(i);
		if (err != ENOERR) return err;
	}
	
	return mtffs_flash_write(0, data, mtffs_status.mtffs_fatsize);
}

// mtffs_findfile
// Find a file by it's (unique) name.
static mtffs_fileentry *mtffs_findfile(char *filename)
{
	int i;
	// Loop the FAT and find the fileentry.
	mtffs_fileentry *addr = (unsigned int)mtffs_status.mtffs_fatbuffer + sizeof(mtffs_fat);
	for (i=0;i<mtffs_status.mtffs_filenumber;i++)
	{
		if (strcmp(addr->filename,filename)==0)
			return addr;
		addr++;
	}
	return NULL;
}

// mtffs_emptyfe
// Find next empty fileentry in the FAT.
static mtffs_fileentry *mtffs_emptyfe(void)
{
	mtffs_fileentry *addr = (unsigned int)mtffs_status.mtffs_fatbuffer + sizeof(mtffs_fat);
	int i;
	for (i=0;i<mtffs_status.mtffs_filenumber;i++)
	{
		if (addr->filename[0]==0)
			return addr;
		addr++;
	}
	return NULL;
}

// mtffs_emptyblock
// Find next empty block in the flash device.
static int mtffs_emptyblock(unsigned short *blocknr)
{
	unsigned char *addr = mtffs_status.mtffs_fatblocks*mtffs_status.devinfo.blocksize;
	*blocknr = mtffs_status.mtffs_fatblocks;
	mtffs_block bl;
	int i,err;
	for (i=0;i<mtffs_status.devinfo.blocknumber-mtffs_status.mtffs_fatblocks;i++) {
		err = mtffs_flash_read((unsigned char*)addr,(unsigned char*)&bl,sizeof(mtffs_block));
		if (err != ENOERR) return NULL;
		if (bl.nextblock==-1)
			return ENOERR;
		addr += mtffs_status.devinfo.blocksize;
		*blocknr = (*blocknr+1);
	}
	*blocknr = 0;
	return ENOERR;
}

// mtffs_emptyblock
// Find next empty block in the flash device.
static int mtffs_emptyblockcount(void)
{
	int count=0;
	unsigned char *addr = mtffs_status.mtffs_fatblocks*mtffs_status.devinfo.blocksize;
	mtffs_block bl;
	int i,err;
	for (i=0;i<mtffs_status.devinfo.blocknumber-mtffs_status.mtffs_fatblocks;i++) {
		err = mtffs_flash_read((unsigned char*)addr,(unsigned char*)&bl,sizeof(mtffs_block));
		if (err != ENOERR) return NULL;
		if (bl.nextblock==-1)
			count++;
		addr += mtffs_status.devinfo.blocksize;
	}
	return count;
}

// mtffs_updateblock
// Updates an entire block (erase + write).
// Skipping if blocks are in FAT
static int mtffs_updateblock(mtffs_block *block, unsigned short blocknr, unsigned char *data)
{
	int err;
	unsigned char *addr;
	if (blocknr<mtffs_status.mtffs_fatblocks)
		return ENOERR;
		   
	err = mtffs_flash_erase(blocknr);
	if (err != ENOERR) return err;
	memcpy(data,block,sizeof(mtffs_block));	// update the data with new index
	
	addr = blocknr * mtffs_status.devinfo.blocksize;
	err = mtffs_flash_write(addr,data,mtffs_status.devinfo.blocksize);
	return err;
}

// mtffs_newfile
// Allocate a new file in FAT with 0 size.
// Make sure to check if filename already exists before calling here! (No dup check)
static int mtffs_newfile(char *filename)
{
	mtffs_fileentry fe;
	mtffs_fileentry *empty = mtffs_emptyfe();
	int err;
	if (empty==NULL)
		return ENOSPC;
	if (strlen(filename)>FILENAME_SIZE)
		return ENAMETOOLONG;
	memset(fe.filename,0,FILENAME_SIZE);
	strcpy(fe.filename,filename);
	fe.filesize = 0;
	fe.nextblock = 0;
	
	// Update the FAT
	memcpy(empty,&fe,sizeof(mtffs_fileentry));
	
	return ENOERR;
}

// mtffs_allocnewblock
// Allocate a new block for the file.
// Will update FAT-mem cache and old blocks (if needed) to keep links intact
static int mtffs_allocnewblock(mtffs_fileentry *fe, unsigned short *newblocknr)
{
	mtffs_block b;
	unsigned char *addr;
	unsigned short lastblocknr;
	int err;
	
	b.nextblock = fe->nextblock;
	lastblocknr = 0;
	
	// Find last block
	while(b.nextblock!=0) {
		addr = b.nextblock * mtffs_status.devinfo.blocksize;
		lastblocknr = b.nextblock;
		err = mtffs_flash_read(addr,&b,sizeof(mtffs_block));
		if (err != ENOERR) return err;
	}
		
	mtffs_emptyblock(newblocknr);
	if (*newblocknr==0)
		return ENOSPC;
			
	// Is this the first block? Just update FAT
	if (fe->nextblock==0) {
		fe->nextblock = *newblocknr;
	}	
	else if (lastblocknr!=0) 	
	{
		// We need to link to this new block (update last block),
		mtffs_block b;
		b.nextblock = *newblocknr;
		/*
		block_cache = malloc(mtffs_status.devinfo.blocksize);
		if (block_cache == NULL)
			return ENOMEM;
		*/
		addr = (lastblocknr * mtffs_status.devinfo.blocksize);
		err = mtffs_flash_read(addr,mtffs_status.block_cache,mtffs_status.devinfo.blocksize);
		if (err != ENOERR) return err;
		err = mtffs_updateblock(&b,lastblocknr,mtffs_status.block_cache);
		if (err != ENOERR) return err;
	}
	return ENOERR;
}

static int mtffs_pathconf( struct cyg_pathconf_info *info )
{
	int err = ENOERR;
    
	switch( info->name )
	{
		case _PC_LINK_MAX:
			info->value = LINK_MAX;
			break;
        
		case _PC_MAX_CANON:
			info->value = -1;       // not supported
			err = EINVAL;
			break;
        
		case _PC_MAX_INPUT:
			info->value = -1;       // not supported
			err = EINVAL;        
			break;
        
		case _PC_NAME_MAX:
			info->value = NAME_MAX;
			break;
        
		case _PC_PATH_MAX:
			info->value = PATH_MAX;
			break;        

		case _PC_PIPE_BUF:
			info->value = -1;       // not supported
			err = EINVAL;        
			break;        

        
		case _PC_ASYNC_IO:
			info->value = -1;       // not supported
			err = EINVAL;        
			break;
        
		case _PC_CHOWN_RESTRICTED:
			info->value = -1;       // not supported
			err = EINVAL;
			break;
        
		case _PC_NO_TRUNC:
			info->value = 0;
			break;
        
		case _PC_PRIO_IO:
			info->value = 0;
			break;
        
		case _PC_SYNC_IO:
			info->value = 0;
			break;
        
		case _PC_VDISABLE:
			info->value = -1;       // not supported
			err = EINVAL;        
			break;
        
		default:
			err = EINVAL;
			break;
	}

	return err;
}


//===========================================================================
// Flash interface
//===========================================================================

#ifdef CYGOPT_FS_MTFFS_USE_IO_FLASH
int mtffs_flash_read(unsigned char *addr, unsigned char *data, unsigned int length)
{
	Cyg_ErrNo err;
	cyg_uint32 len = length;
	
	err = cyg_io_bread(mtffs_status.devinfo.dev, data, &len, mtffs_status.devinfo.offset+(unsigned int)addr);
	return ((err == ENOERR) ? ENOERR : -EIO);
}

int mtffs_flash_write(unsigned char *addr, unsigned char *data, unsigned int length)
{
	Cyg_ErrNo err;
	cyg_uint32 len = length;
	
	err = cyg_io_bwrite(mtffs_status.devinfo.dev, data, &len, mtffs_status.devinfo.offset+(unsigned int)addr);
	return ((err == ENOERR) ? ENOERR : -EIO);
}

int mtffs_flash_erase(unsigned short sector)
{
	Cyg_ErrNo err;
	cyg_io_flash_getconfig_erase_t e;
	void *err_addr;
	cyg_uint32 len = sizeof (e);
	
	e.offset = sector*mtffs_status.devinfo.blocksize;
	e.len = mtffs_status.devinfo.blocksize;
	e.err_address = &err_addr;
	
	err = cyg_io_get_config(mtffs_status.devinfo.dev, CYG_IO_GET_CONFIG_FLASH_ERASE, &e, &len);
	return (err != ENOERR || e.flasherr != 0);
}
#endif

#ifdef CYGOPT_FS_MTFFS_USE_M25PXX_FLASH
#include <cyg/io/flash.h>

extern int flash_erase_block_sub (volatile unsigned long block);
extern int flash_read_buf_byte(volatile unsigned char* addr, unsigned char* data, int len);
extern int flash_program_buf_byte(volatile unsigned char* addr, unsigned char* data, int len);

int mtffs_flash_read(unsigned char *addr, unsigned char *data, unsigned int length)
{
	int err;
	err = flash_read_buf_byte(addr,data,length);
	return ((err == FLASH_ERR_OK) ? ENOERR : -EIO);
}

int mtffs_flash_write(unsigned char *addr, unsigned char *data, unsigned int length)
{
	int err;
	err = flash_program_buf_byte(addr,data,length);
	return ((err == FLASH_ERR_OK) ? ENOERR : -EIO);
}

int mtffs_flash_erase(unsigned short sector)
{
	int err;
	err = flash_erase_block_sub(sector);
	return ((err == FLASH_ERR_OK) ? ENOERR : -EIO);
}
#endif

#ifdef CYGOPT_FS_MTFFS_USE_RAM_FLASH
#endif