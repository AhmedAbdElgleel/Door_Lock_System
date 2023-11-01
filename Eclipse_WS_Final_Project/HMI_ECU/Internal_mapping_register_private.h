/*
 ================================================================================================================================
 * File Name   : Internal_mapping_register.h
 *
 * Module Name : Internal_mapping_register
 *
 * Created On  : Oct 14, 2023
 *
 * Author Name : Ahmed Abd Elgleel
 ================================================================================================================================
 */
#ifndef INTERNAL_MAPPING_REGISTER_PRIVATE_H_
#define INTERNAL_MAPPING_REGISTER_PRIVATE_H_

typedef union {
	uint8 Byte;
	struct{
		uint8 C_Bit:1;
		uint8 Z_Bit:1;
		uint8 N_Bit:1;
		uint8 V_Bit:1;
		uint8 S_Bit:1;
		uint8 H_Bit:1;
		uint8 T_Bit:1;
		uint8 I_Bit:1;
	}Bits;
}SREG_REG_Type;

#define SREG_REG  (*((volatile  SREG_REG_Type * const)0x005F))

#endif /* INTERNAL_MAPPING_REGISTER_PRIVATE_H_ */
