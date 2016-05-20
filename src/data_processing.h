void dp_exec(void);

struct dp_instr {
	cond: 4
	imm_op: 1
	op_code: 4
	set_cond: 1
	op1: 4
	dest: 4
	op2: 12
}
