/* moss linker script.
 *
 * TODO: Add a description of the image format.
 */

/* imag-regs.ld is a helper script that automatically assigns each unset
 * imaginary register to the address one after the register before it. That's
 * why we only need to define the first one.
 */
__rc0 = 0x00;
INCLUDE imag-regs.ld
ASSERT(__rc31 == 0x001f, "__rc31 should be at 0x001F")

MEMORY {
    zp : ORIGIN = __rc31 + 1, LENGTH = 0x100 - (__rc31 + 1)
    ram (rw) : ORIGIN = 0x0200, LENGTH = 0xFFF9 - 0x0200
}

REGION_ALIAS("c_readonly", ram)
REGION_ALIAS("c_writeable", ram)

/* According to https://llvm-mos.org/wiki/Porting, the linker automatically
 * places all sections in the ram region. I'm not sure if c.ld ensures that
 * or if it happens by default, but it's the behavior we want.
 */
SECTIONS { INCLUDE c.ld }

/* Set the initial software stack address to just above last memory address. */
__stack = 0xFFF9;

OUTPUT_FORMAT {
    SHORT(ORIGIN(ram))
    SHORT(_start)
    TRIM(ram)
}
