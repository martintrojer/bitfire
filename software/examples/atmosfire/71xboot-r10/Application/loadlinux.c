#include "platform.h"
#include "common.h"
#include "71x_lib.h"
#include "string.h"

/*
 * The new way of passing information: a list of tagged entries
 */

/* The list ends with an ATAG_NONE node. */
#define ATAG_NONE	0x00000000

struct tag_header {
	u32 size;
	u32 tag;
};

/* The list must start with an ATAG_CORE node */
#define ATAG_CORE	0x54410001

struct tag_core {
	u32 flags;		/* bit 0 = read-only */
	u32 pagesize;
	u32 rootdev;
};

/* it is allowed to have multiple ATAG_MEM nodes */
#define ATAG_MEM	0x54410002

struct tag_mem32 {
	u32	size;
	u32	start;	/* physical start address */
};

/* VGA text type displays */
#define ATAG_VIDEOTEXT	0x54410003

struct tag_videotext {
	u8		x;
	u8		y;
	u16		video_page;
	u8		video_mode;
	u8		video_cols;
	u16		video_ega_bx;
	u8		video_lines;
	u8		video_isvga;
	u16		video_points;
};

/* describes how the ramdisk will be used in kernel */
#define ATAG_RAMDISK	0x54410004

struct tag_ramdisk {
	u32 flags;	/* bit 0 = load, bit 1 = prompt */
	u32 size;	/* decompressed ramdisk size in _kilo_ bytes */
	u32 start;	/* starting block of floppy-based RAM disk image */
};

/* describes where the compressed ramdisk image lives (virtual address) */
/*
 * this one accidentally used virtual addresses - as such,
 * its depreciated.
 */
#define ATAG_INITRD	0x54410005

/* describes where the compressed ramdisk image lives (physical address) */
#define ATAG_INITRD2	0x54420005

struct tag_initrd {
	u32 start;	/* physical start address */
	u32 size;	/* size of compressed ramdisk image in bytes */
};

/* board serial number. "64 bits should be enough for everybody" */
#define ATAG_SERIAL	0x54410006

struct tag_serialnr {
	u32 low;
	u32 high;
};

/* board revision */
#define ATAG_REVISION	0x54410007

struct tag_revision {
	u32 rev;
};

/* initial values for vesafb-type framebuffers. see struct screen_info
 * in include/linux/tty.h
 */
#define ATAG_VIDEOLFB	0x54410008

struct tag_videolfb {
	u16		lfb_width;
	u16		lfb_height;
	u16		lfb_depth;
	u16		lfb_linelength;
	u32		lfb_base;
	u32		lfb_size;
	u8		red_size;
	u8		red_pos;
	u8		green_size;
	u8		green_pos;
	u8		blue_size;
	u8		blue_pos;
	u8		rsvd_size;
	u8		rsvd_pos;
};

/* command line: \0 terminated string */
#define ATAG_CMDLINE	0x54410009

struct tag_cmdline {
	char	cmdline[1];	/* this is the minimum size */
};


struct tag {
	struct tag_header hdr;
	union {
		struct tag_core		core;
		struct tag_mem32	mem;
		struct tag_videotext	videotext;
		struct tag_ramdisk	ramdisk;
		struct tag_initrd	initrd;
		struct tag_serialnr	serialnr;
		struct tag_revision	revision;
		struct tag_videolfb	videolfb;
		struct tag_cmdline	cmdline;

	} u;
};

#define tag_next(t)	((struct tag *)((u32 *)(t) + (t)->hdr.size))
#define tag_size(type)	((sizeof(struct tag_header) + sizeof(struct type)) >> 2)

#define LINUX_PARAM_OFFSET 0x100	//offset of embedded SRAM
static char linux_cmd[] = "noinitrd root=/dev/rom0";

static void setup_linux_param(u32 param_base)
{
	struct tag *params = (struct tag *)param_base;

	//Printf("Setup linux parameters at 0x%08lx\n", param_base);

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size(tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next(params);

	params->hdr.tag = ATAG_MEM;
	params->hdr.size = tag_size(tag_mem32);

	params->u.mem.start = 0x62000000; 
	params->u.mem.size = 0x400000;

	params = tag_next(params);

	/* set linux command line */
	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size = (sizeof(struct tag_header) + strlen(linux_cmd)+1+4)>>2;
	memcpy(params->u.cmdline.cmdline, linux_cmd, strlen(linux_cmd) + 1);
	params = tag_next(params);

	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

extern void CALL_KERNEL(unsigned int);

void LoadLinux(int argc, const char **argv)
{
	unsigned int srcaddr;
	unsigned int textaddr,textaddr1,endaddr;
	int errno = 0;

	if (argc == 1)
	{
		srcaddr = 0x60010000; //64K offset
	}
	else if (argc == 2)
	{
		if (!errno)
		if (!Str2Int(argv[1],&srcaddr)) errno = 1;
	}
	else
		errno = 1;

	if (errno ==1 )
	{
		PutString("Invalid Command Arguments!\r\n");
		return;
	}
	
	//zimage offset: 
	//0x24: 01 6f 28 18	--magic number
	//0x28: --text address (62 20 00 00	)
	//0x2c: --end address 
	
	if (*((volatile unsigned int *)(srcaddr+0x24)) != 0x016f2818)
	{
		Printf("Not Compressed zimage!\r\n");
		return;
	}
	
	textaddr = *((volatile unsigned int *)(srcaddr+0x28));
	textaddr1 = textaddr;
	endaddr = *((volatile unsigned int *)(srcaddr+0x2C));
	
	Printf("Copying...");
	while (textaddr <= endaddr)
	{
		*((volatile unsigned short *)textaddr) = *((volatile unsigned short*)srcaddr);
		textaddr += 2;
		srcaddr += 2;
	}
	Printf("end \r\n");

	/*set up linux parameters
	* linux kernel will find and parse these parameters if BOOT_PARAMS()is included in MACH_DESC.
	* Otherwise this pre-setting is not used.
	*/
 	setup_linux_param(0x0 + LINUX_PARAM_OFFSET);
	
	//run zimage
	CALL_KERNEL(textaddr1);
}

/*
   This function is for debug purpose.
   Developer may copy uncompressed image into RAM and then jump directory.
*/
void JumpTo(int argc, const char **argv)
{
	unsigned int jmpaddr;
	int errno = 0;

	if (argc == 2)
	{
		if (!errno)
		if (!Str2Int(argv[1],&jmpaddr)) errno = 1;
	}
	else
		errno = 1;

	if (errno ==1 )
	{
		PutString("Invalid Command Arguments!\r\n");
		return;
	}
 	setup_linux_param(0x0 + LINUX_PARAM_OFFSET);

	CALL_KERNEL(jmpaddr);
}