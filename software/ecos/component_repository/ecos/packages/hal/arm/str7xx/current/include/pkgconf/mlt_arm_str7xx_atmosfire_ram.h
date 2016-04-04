// eCos memory layout - Mon Jul 23 11:49:04 2001

// This is a generated file - do not edit

#ifndef __ASSEMBLER__
#include <cyg/infra/cyg_type.h>
#include <stddef.h>

#endif
#define CYGMEM_REGION_ram (0x62000000)
#define CYGMEM_REGION_ram_SIZE (0x100000)
#define CYGMEM_REGION_ram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)
//#define CYGMEM_REGION_iram (0x0)
//#define CYGMEM_REGION_iram_SIZE (0x10000)
//#define CYGMEM_REGION_iram_ATTR (CYGMEM_REGION_ATTR_R | CYGMEM_REGION_ATTR_W)
#define CYGMEM_REGION_rom (0x40000000)
#define CYGMEM_REGION_rom_SIZE (0x40000)
#define CYGMEM_REGION_rom_ATTR (CYGMEM_REGION_ATTR_R)
#ifndef __ASSEMBLER__
extern char CYG_LABEL_NAME (__heap1) [];
#endif
#define CYGMEM_SECTION_heap1 (CYG_LABEL_NAME (__heap1))
#define CYGMEM_SECTION_heap1_SIZE (0x62100000 - (size_t) CYG_LABEL_NAME (__heap1))
