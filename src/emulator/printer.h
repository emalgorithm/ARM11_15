#ifndef PRINTER_H
#define PRINTER_H

/*
 * Function : print_registers
 * -----------------------
 * Prints register contents to stdout in the following format
 *     $register_index : 0d value 0x value
 *     PC: 0d value 0x value
 *     CPSR: 0d value 0x value
 */
void print_registers(void);

/*
 * Function : print_memory
 * -----------------------
 * Prints non-zero memory cells to stdout in the following format
 *     0x address: 0x value
 */
void print_memory(void);

#endif
