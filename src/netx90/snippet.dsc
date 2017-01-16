Initialize a memory area with a fill-value in steps of 32 bytes.

A fill value is written to a memory area using 32bit Store-multiple (STM)
accesses. The STM uses 8 registers for one write access. 8 registers with
32bit each result in a total size of 8*32 bit = 256 bit = 32 bytes for each
STM. This makes the fill operation very fast, but rounds up the size of the
area to a multiple of 32 bytes.

Examples:

Fill the complete DDR_LO area with 0x00000000.

    <Snip artifact="initialize_memory" group="org.muhkuh.hboot.sniplib" version="1.0.0">
        <Parameter name="START">0x40000000</Parameter>
        <Parameter name="END">0x80000000</Parameter>
        <Parameter name="FILL">0x00000000</Parameter>
    </Snip>


Fill the area from 0x05080000 to 0x05080100 with 0x12345678. The last write
access goes to 0x050800fc.

    <Snip artifact="initialize_memory" group="org.muhkuh.hboot.sniplib" version="1.0.0">
        <Parameter name="START">0x05080000</Parameter>
        <Parameter name="END">0x05080100</Parameter>
        <Parameter name="FILL">0x12345678</Parameter>
    </Snip>


The area size in this example is not a multiple of 32 bytes. It is 16 bytes
and is rounded up to 32 bytes. It fills the area from 0x05080000 to 0x05080020
with 0x12345678.

    <Snip artifact="initialize_memory" group="org.muhkuh.hboot.sniplib" version="1.0.0">
        <Parameter name="START">0x05080000</Parameter>
        <Parameter name="END">0x05080010</Parameter>
        <Parameter name="FILL">0x12345678</Parameter>
    </Snip>
