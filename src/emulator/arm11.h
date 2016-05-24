#ifndef ARM11_H
#define ARM11_H

#include <stdint.h>
#include <stdlib.h>

#define MEMORY_SIZE (1 << 16)
#define NUM_OF_REGISTERS 17
#define BYTE 8
#define WORD_SIZE 4
#define SIGN_BIT (1 << 31)

// TODO: remove after CPSR utils are complete
/* CPSR flags (temporary)
 * ----------
 * Used to check conditions for certain ARM instructions. The flags are stored
 * in register 16. For the purposes of this project, we are only concerned with
 * the upper 4 bits of the register.
 */
#define CPSR_REG 16
#define CPSR_N (1 << 31)
#define CPSR_Z (1 << 30)
#define CPSR_C (1 << 29)
#define CPSR_V (1 << 28)

/* Instruction types
 * -----------------
 * The 4 following structs represent the 4 instruction types supported by the
 * emulator: data processing, multiply, single data transfer, and branch.
 *
 * | The common fields are described in the table below
 * |--------------------------------------------------------------------------|
 * |Field name | Size | Position| Description                                 |
 * |-----------|------|---------|---------------------------------------------|
 * | cond      | 4    | 31-28   | Condition code to determine if the operation|
 * |           |      |         | should be executed or not based on the CPSR.|
 * |-----------|------|---------|---------------------------------------------|
 * | imm_op    | 1    | 25      | If set indicates that op2 is an immediate   |
 * |           |      |         | constant, otherwise a shifted register.     |
 * |-----------|------|---------|---------------------------------------------|
 * | set_cond  | 1    | 20      | If set indicates that CPSR should be updated|
 * |-----------|------|---------|---------------------------------------------|
 * | _id       | 2    | 27-26   | Used for decoding only                      |
 * |--------------------------------------------------------------------------|
 */

 /*
* Struct: dp_instr
* -----------------
* A bit field representation of the data processing instruction type.
*/
struct dp_instr {
    uint32_t op2      : 12;
    uint32_t rd       : 4;
    uint32_t rn       : 4;
    uint32_t set_cond : 1;
    uint32_t op_code  : 4;
    uint32_t imm_op   : 1;
    uint32_t _id      : 2;
    uint32_t cond     : 4;
};

/*
 * Struct: mul_instr
 * -----------------
 * A bit field representation of the multiply instruction type.
 */
struct mul_instr {
<<<<<<< HEAD
    uint32_t rm       : 4;
    uint32_t _mul4    : 1;
    uint32_t          : 2;
    uint32_t _mul7    : 1;
    uint32_t rs       : 4;
    uint32_t rn       : 4;
    uint32_t rd       : 4;
    uint32_t set_cond : 1;
    uint32_t acc      : 1;
    uint32_t          : 4;
    uint32_t _id      : 2;
    uint32_t cond     : 4;
=======
    uint32_t cond : 4;
    uint32_t acc: 1;
    uint32_t set_cond : 1;
    uint32_t dest : 4;
    uint32_t rn : 4;
    uint32_t rs : 4;
    uint32_t rm : 4;
>>>>>>> 890097f69587d6db7d2aef2f161830435d93d673
};

/*
* Struct: sdt_instr
* -----------------
* A bit field representation of the single data transfer instruction type.
* Fields:
*          cond  a 4-bit condition code
*/
struct sdt_instr {
    uint32_t offset     : 12;
    uint32_t rd         : 4;
    uint32_t rn         : 4;
    uint32_t load_store : 1;
    uint32_t            : 2;
    uint32_t up         : 1;
    uint32_t index_bit  : 1;
    uint32_t imm_off    : 1;
    uint32_t _id        : 2;
    uint32_t cond       : 4;
};

/*
* Struct: br_instr
* -----------------
* A bit field representation of the branch instruction type.
*/
struct br_instr {
    uint32_t offset : 24;
    uint32_t        : 2;
    uint32_t _id    : 2;
    uint32_t cond   : 4;
};

/* Memory representation
 * ---------------------
 * The unit of memory is a union which provides access to the whole word as a
 * 32-bit unsigned integer in big-endian, the 4 bytes that form a word, and an
 * instruction union, which can be treated as one of the 4 instruction types.
 */

/*
 * Union: decoded_instr
 * --------------------
 * Bit fields per instruction type
 */
union decoded_instr {
    struct mul_instr mul;
    struct br_instr br;
    struct dp_instr dp;
    struct sdt_instr sdt;
};

/*
 * Struct: word
 * ------------
 * Byte-addressing of memory words
 */
struct word {
    uint32_t lsb: 8;
    uint32_t b1: 8;
    uint32_t b2: 8;
    uint32_t msb: 8;
};

/*
 * Union: instruction
 * ------------------
 * This is the data type used internally to represent memory. It offers a
 * choice between the 3 representations described above - instructions, and
 * data as words or bytes
 */
union instruction {
    uint32_t bin;
    struct word raw;
    union decoded_instr decoded;
};

/* API functions
* -------------
* Things to take into account when using the exposed interface:
*
*  + The address space of memory in the current implementation fits in 16 bits
*    However, an address parameter is of type uint32_t which is consistent
*    with the word size and makes it easier to extend the memory size or
*    address other types of storage. An invalid address would probably cause
*    an assertion to fail if there is no error-handling mechanism specified
*
*  + All general purpose registers can be accessed by index (inluding the
*    special ones such as SP(13), LR(14), PC(15), CPSR(16)). There will be
*    an extension to this interface exposing the special registers as separate
*    functions and also giving read/write access to the flags in CPSR.
*/

/*
* Function : initialize
* Usage    : initialize()
* -----------------------
* This procedure must be called before using the following access procedures.
* It alocates space for the memory and registers of the emulated
* architecture on the heap and prepares it for use.
*/
void initialize(void);

/*
* Function : get_instr
* Usage    : get_instr(0x00ffaa224)
* ---------------------------------
* This function takes an address in memory and returns a pointer to the
* instruction starting at that address. See the instruction union type.
*
* PRE:  address is a valid address of the byte-addressable memory
*
* POST: a decoded instruction is pointed to by the returned pointer
*/
union instruction *get_instr(uint32_t address);

/*
* Function : set_register
* Usage    : set_register(0x00ffaa224)
* ------------------------------------
* Set the contents of the register indentified by the given index
*
* PRE:  index is a valid register identifier
*
* POST: the contents of the register equal the given value
*       no endiannes conversion is performed
*/
void set_register(uint32_t index, uint32_t value);

/*
* Function : get_register
* Usage    : get_register(0x00ffaa224)
* ------------------------------------
* Get the contents of the register indentified by the given index
*
* PRE:  index is a valid register identifier
*
* POST: the contents of the register equal the returned value
*       no endiannes conversion is performed
*/
uint32_t get_register(uint32_t index);

/*
* Function : set_byte
* Usage    : set_byte(0x0000aa00, 0xfa)
* -------------------------------------
* Write a single byte of memory at a given address
*
* PRE:  address is a valid address of the byte-addressable memory
*
* POST: the contents of memory at the given address equal the give value
*/
void set_byte(uint32_t address, uint8_t value);

/*
* Function : get_byte
* Usage    : get_byte(0xfa)
* -------------------------
* Read the contents of memory at a the given address
*
* PRE:  address is a valid address of the byte-addressable memory
*
* POST: the contents of memory at the given address equal the returned value
*/
uint8_t get_byte(uint32_t address);

/*
* Function          : set_word
*
* Usage             : set_word(0x0000aa00, 0x00fa23ab)
*
* Address           : 0x0000aa00 0x0000aa01 0x0000aa02 0x0000aa03
* State (POST)      : 0xab       0x23       0xfa       0x00
* ----------------------------------------
* This procedure takes an address in memory and a 32-bit big-endian value
* and it writes the value at the desired address after converting it to
* little-endian
*/
void set_word(uint32_t address, uint32_t value);

/*
* Function : get_word
*
* Usage    : get_word(0x0000aa00)
*
* Address  : 0x0000aa00 0x0000aa01 0x0000aa02 0x0000aa03
* State    : 0xab       0x23       0xfa       0x00
*
* Returns  : 0x00fa23ab
* ----------------------------------------
* This function takes an address in memory andit returns the word at the
* desired address after converting it to big endian
*/
uint32_t get_word(uint32_t address);

#endif
