/* Entry Point */
ENTRY(main)

/* Sections */
SECTIONS
{
  . = 0x0 + SIZEOF_HEADERS;

  /* The program code and other data into "FLASH" Rom type memory */
  .text : ALIGN(4)
  {
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */

/*    KEEP (*(.init))*/
/*    KEEP (*(.fini))*/
/*    _etext = .;        /* define a global symbols at end of code */*/
  }

  /* Constant data into "FLASH" Rom type memory */
  .rodata : ALIGN(4)
  {
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
  }

/*  .preinit_array (READONLY) :*/
/*  {*/
/*    . = ALIGN(4);*/
/*    PROVIDE_HIDDEN (__preinit_array_start = .);*/
/*    KEEP (*(.preinit_array*))*/
/*    PROVIDE_HIDDEN (__preinit_array_end = .);*/
/*    . = ALIGN(4);*/
/*  }*/

/*  .init_array (READONLY) :*/
/*  {*/
/*    . = ALIGN(4);*/
/*    PROVIDE_HIDDEN (__init_array_start = .);*/
/*    KEEP (*(SORT(.init_array.*)))*/
/*    KEEP (*(.init_array*))*/
/*    PROVIDE_HIDDEN (__init_array_end = .);*/
/*    . = ALIGN(4);*/
/*  }*/

/*  .fini_array (READONLY) :*/
/*  {*/
/*    . = ALIGN(4);*/
/*    PROVIDE_HIDDEN (__fini_array_start = .);*/
/*    KEEP (*(SORT(.fini_array.*)))*/
/*    KEEP (*(.fini_array*))*/
/*    PROVIDE_HIDDEN (__fini_array_end = .);*/
/*    . = ALIGN(4);*/
/*  }*/

  /* Used by the startup to initialize data */
/*  _sidata = LOADADDR(.data);*/

  /* Initialized data sections into "RAM" Ram type memory */
  .data : ALIGN(4)
  {
/*    _sdata = .;        /* create a global symbol at data start */*/
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */
/*    _edata = .;        /* define a global symbol at data end */*/
  }

  /* Uninitialized data section into "RAM" Ram type memory */
  .bss : ALIGN(4)
  {
    /* This is used by the startup in order to initialize the .bss section */
/*    _sbss = .;         /* define a global symbol at bss start */*/
/*    __bss_start__ = _sbss;*/
    *(.bss)
    *(.bss*)
    *(COMMON)
/*    _ebss = .;         /* define a global symbol at bss end */*/
/*    __bss_end__ = _ebss;*/
  }

  .hash : { *(.hash) }
  .dynsym : { *(.dynsym) }
  .dynstr : { *(.dynstr) }
  .dynamic : { *(.dynamic) }
  .got : { *(.got*) }
  .plt : { *(.plt*) }
  .rel : { *(.rel*) }
  .symtab : { *(.symtab) }
  .strtab : { *(.strtab) }
  .shstrtab : { *(.shstrtab) }

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
    * (.comment)
    * (.ARM.attributes)
  }
}


