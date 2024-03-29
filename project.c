/* 
Class Project: The logical conclusion (v1.1)
CSCI-2500 Spring 2021
Prof. Slota 
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define BIT type as a char (i.e., one byte)
typedef char BIT;
#define TRUE 1
#define FALSE 0
#define UNDEF -1

// useful constants
BIT ONE[32] = {TRUE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT ZERO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};
BIT REG_THIRTY_ONE[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
BIT THIRTY_TWO[32] = {FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE, FALSE, 
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE,
  FALSE};


/******************************************************************************/
/* Function prototypes */
/******************************************************************************/
BIT not_gate(BIT A);
BIT or_gate(BIT A, BIT B);
BIT or_gate3(BIT A, BIT B, BIT C);
BIT and_gate(BIT A, BIT B);
BIT and_gate3(BIT A, BIT B, BIT C);
BIT xor_gate(BIT A, BIT B);
BIT nor_gate(BIT A, BIT B);
BIT nand_gate(BIT A, BIT B);

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3);
BIT multiplexor2(BIT S, BIT I0, BIT I1);
void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output);
BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3);

void copy_bits(BIT* A, BIT* B);
void print_binary(BIT* A);
void convert_to_binary(int a, BIT* A, int length);
void convert_to_binary_char(int a, char* A, int length);
int binary_to_integer(BIT* A);

int get_instructions(BIT Instructions[][32]);

void Instruction_Memory(BIT* ReadAddress, BIT* Instruction);
void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* Immediate, BIT* JumpLink, BIT* JumpReturn);
void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2);
void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData);
void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl);
void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result);
void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData);
void Extend_Sign16(BIT* Input, BIT* Output);
void updateState();
  
/******************************************************************************/
/* Functions provided for your convenience */
/******************************************************************************/
BIT not_gate(BIT A)
{
  if (A)
    return FALSE;
  else
    return TRUE;
}

BIT or_gate(BIT A, BIT B)
{
  if (A || B)
    return TRUE;
  else
    return FALSE;
}

BIT or_gate3(BIT A, BIT B, BIT C)
{
  return or_gate(A, or_gate(B, C));
}

BIT and_gate(BIT A, BIT B)
{
  if (A && B)
    return TRUE;
  else
    return FALSE;
}

BIT and_gate3(BIT A, BIT B, BIT C)
{
  return and_gate(A, and_gate(B, C));
}

BIT xor_gate(BIT A, BIT B)
{
  if (A ^ B)
    return TRUE;
  else
    return FALSE;
}

BIT nor_gate(BIT A, BIT B)
{
  return not_gate(or_gate(A, B));
}

BIT nand_gate(BIT A, BIT B)
{
  return not_gate(and_gate(A, B));
}

void decoder2(BIT I0, BIT I1, BIT* O0, BIT* O1, BIT* O2, BIT* O3)
{
  // Note: The input -> output mapping is slightly modified from before
  *O0 = and_gate(not_gate(I1), not_gate(I0));
  *O1 = and_gate(not_gate(I1), I0);
  *O2 = and_gate(I1, not_gate(I0));
  *O3 = and_gate(I1, I0);
  
  return;
}
void decoder3(BIT* I, BIT EN, BIT* O)
{
  O[0] = and_gate3(not_gate(I[2]), not_gate(I[1]), not_gate(I[0]));
  O[1] = and_gate3(not_gate(I[2]), not_gate(I[1]), I[0]);
  O[2] = and_gate3(not_gate(I[2]), I[1], not_gate(I[0]));
  O[3] = and_gate3(not_gate(I[2]), I[1], I[0]);
  O[4] = and_gate3(I[2], not_gate(I[1]), not_gate(I[0]));
  O[5] = and_gate3(I[2], not_gate(I[1]), I[0]);
  O[6] = and_gate3(I[2], I[1], not_gate(I[0]));
  O[7] = and_gate3(I[2], I[1], I[0]);
  
  O[0] = and_gate(EN, O[0]);
  O[1] = and_gate(EN, O[1]);
  O[2] = and_gate(EN, O[2]);
  O[3] = and_gate(EN, O[3]);
  O[4] = and_gate(EN, O[4]);
  O[5] = and_gate(EN, O[5]);
  O[6] = and_gate(EN, O[6]);
  O[7] = and_gate(EN, O[7]);
  
  return;
}
void decoder5(BIT* I, BIT* O)
{
   BIT EN[4] = {FALSE};
   decoder2(I[3], I[4], &EN[0], &EN[1], &EN[2], &EN[3]);
   decoder3(I, EN[3], &O[24]);
   decoder3(I, EN[2], &O[16]);
   decoder3(I, EN[1], &O[8]);
   decoder3(I, EN[0], &O[0]);
}

BIT multiplexor2(BIT S, BIT I0, BIT I1)
{
  BIT x0 = and_gate(not_gate(S), I0);
  BIT x1 = and_gate(S, I1);
  return or_gate(x0, x1);  
}

void multiplexor2_32(BIT S, BIT* I0, BIT* I1, BIT* Output)
{
  for (int i = 0; i < 32; ++i) {
    BIT x0 = and_gate(not_gate(S), I0[i]);
    BIT x1 = and_gate(S, I1[i]);
    Output[i] = or_gate(x0, x1);
  }
}

BIT multiplexor4(BIT S0, BIT S1, BIT I0, BIT I1, BIT I2, BIT I3)
{
  BIT x0, x1, x2, x3 = FALSE;
  decoder2(S0, S1, &x0, &x1, &x2, &x3);
  
  BIT y0 = and_gate(x0, I0);
  BIT y1 = and_gate(x1, I1);
  BIT y2 = and_gate(x2, I2);
  BIT y3 = and_gate(x3, I3);
  
  BIT z0 = or_gate(y0, y1);
  BIT z1 = or_gate(y2, y3);
  
  return or_gate(z0, z1);  
}


/******************************************************************************/
/* Helper functions */
/******************************************************************************/
void copy_bits(BIT* A, BIT* B)
{
  for (int i = 0; i < 32; ++i)
    B[i] = A[i]; 
}

void print_binary(BIT* A)
{
  for (int i = 31; i >= 0; --i)
    printf("%d", A[i]); 
}

void convert_to_binary(int a, BIT* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? TRUE : FALSE);
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? TRUE : FALSE);
      a /= 2;
    }
  }
}

void convert_to_binary_char(int a, char* A, int length)
{
  if (a >= 0) {
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 1 ? '1' : '0');
      a /= 2;
    }
  } else {
    a += 1;
    for (int i = 0; i < length; ++i) {
      A[i] = (a % 2 == 0 ? '1' : '0');
      a /= 2;
    }
  }
}
int binary_to_integer(BIT* A)
{
  unsigned a = 0;
  unsigned mult = 1;
  
  for (int i = 0; i < 32; ++i) {
    //printf("BIT A at index %d is %d\n", i, A[i]);
    a += A[i]*mult;
    mult *= 2;
  }
  
  return (int)a;
}



/******************************************************************************/
/* Parsing functions */
/******************************************************************************/

// TODO: Implement any helper functions to assist with parsing

void set_register(char* input, char* output) {
  char tmp[256] = {0};

  if(strcmp(input, "zero") == 0) //0
    convert_to_binary_char(0, tmp, 5);
  else if(strcmp(input, "v0") == 0) //2
    convert_to_binary_char(2, tmp, 5);
  else if(strcmp(input, "a0") == 0)  //4
    convert_to_binary_char(4, tmp, 5);
  else if(strcmp(input, "t0") == 0)  //8
    convert_to_binary_char(8, tmp, 5);
  else if(strcmp(input, "t1") == 0)  //9
    convert_to_binary_char(9, tmp, 5);
  else if(strcmp(input, "s0") == 0)  //16
    convert_to_binary_char(16, tmp, 5);
  else if(strcmp(input, "s1") == 0)  //17
    convert_to_binary_char(17, tmp, 5);
  else if(strcmp(input, "sp") == 0)  //29
    convert_to_binary_char(29, tmp, 5);
  else if(strcmp(input, "ra") == 0)  //31
    convert_to_binary_char(31, tmp, 5);
  
  strcpy(output, tmp);
}

int get_instructions(BIT Instructions[][32])
{
  char line[256] = {0};
  int instruction_count = 0;
  while (fgets(line, 256, stdin) != NULL) {        
    // TODO: perform conversion of instructions to bianry
    // Input: coming from stdin via: ./a.out < input.txt
    // Output: Convert instructions to binary in the instruction memory
    // Return: Total number of instructions
    // Note: you are free to use if-else and external libraries here
    // Note: you don't need to implement circuits for saving to inst. mem.
    // My approach:
    // - Use sscanf on line to get strings for instruction and registers
    // - Use instructions to determine op code, funct, and shamt fields
    // - Convert immediate field and jump address field to binary
    // - Use registers to get rt, rd, rs fields
    // Note: I parse everything as strings, then convert to BIT array at end

    //Setting up input and output variables
    BIT output[32] = {FALSE};
    char inst[256] = {0};
    char op1[256] = {0};
    char op2[256] = {0};
    char op3[256] = {0};
    //loading the instruction into variables
    sscanf(line, "%s %s %s %s", inst, op1, op2, op3);

    char tmp_out[33] = {0};
    char rs[6] = {0};
    char rt[6] = {0};
    char rd[6] = {0};
    char imm[17] = {0};
    char address[27] = {0};

    //I-Type:
    //    Instr: op rt rs imm
    //    Bin: op rs rt imm 
    //R-Type: 
    //    Instr: op rd rs rt 
    //    Bin: op rs rt rd shamt funct 
    //J-Type: 
    //    Bin/Instr: op address

    //figure out the type of instruction
    if(strcmp(inst, "lw") == 0 || strcmp(inst, "sw") == 0 ||
       strcmp(inst, "beq") == 0 || strcmp(inst, "addi") == 0) 
    {  //I-Type Bin: op rs rt imm 
        convert_to_binary_char(atoi(op3), imm, 16);
        if(strcmp(inst, "beq") == 0) {
          set_register(op2, rt);
          set_register(op1, rs);
        } else {
          set_register(op1, rt);
          set_register(op2, rs);
        }
        memcpy(&tmp_out[0], imm, 16);
        memcpy(&tmp_out[16], rt, 5);
        memcpy(&tmp_out[21], rs, 5); 

      if(strcmp(inst, "lw") == 0)
        memcpy(&tmp_out[26], "110001", 6);
      else if(strcmp(inst, "sw") == 0)
        memcpy(&tmp_out[26], "110101", 6);
      else if(strcmp(inst, "beq") == 0)
        memcpy(&tmp_out[26], "001000", 6);
      else if(strcmp(inst, "addi") == 0)
        memcpy(&tmp_out[26], "000100", 6);
      
    }
    else if(strcmp(inst, "and") == 0 || strcmp(inst, "or") == 0 ||
            strcmp(inst, "add") == 0 || strcmp(inst, "sub") == 0 ||
            strcmp(inst, "slt") == 0 || strcmp(inst, "jr") == 0)
    { //R_Type Bin: op rs rt rd shamt funct 
      set_register(op1, rd);
      set_register(op2, rs);
      set_register(op3, rt);

      if(strcmp(inst, "jr") == 0)
        strcpy(rs, rd);
      
      memcpy(&tmp_out[6], "00000", 5);
      memcpy(&tmp_out[11], rd, 5);
      memcpy(&tmp_out[16], rt, 5);
      memcpy(&tmp_out[21], rs, 5);

      if(strcmp(inst, "and") == 0) 
        memcpy(&tmp_out[0], "001001", 6);
      else if(strcmp(inst, "or") == 0) 
        memcpy(&tmp_out[0], "101001", 6);
      else if(strcmp(inst, "add") == 0) 
        memcpy(&tmp_out[0], "000001", 6);
      else if(strcmp(inst, "sub") == 0) 
        memcpy(&tmp_out[0], "010001", 6);
      else if(strcmp(inst, "slt") == 0) 
        memcpy(&tmp_out[0], "010101", 6);
      else if(strcmp(inst, "jr") == 0) 
        memcpy(&tmp_out[0], "000100", 6);
      
      memcpy(&tmp_out[26], "000000", 6); 
    }
    else if(strcmp(inst, "j") == 0 || strcmp(inst, "jal") == 0)
    { //J-Type Bin: op address
      convert_to_binary_char(atoi(op1), address, 26);
      memcpy(&tmp_out[0], address, 26);

      if(strcmp(inst, "j") == 0)
        memcpy(&tmp_out[26], "010000", 6);  
      else if(strcmp(inst, "jal") == 0)
        memcpy(&tmp_out[26], "110000", 6);  
    }

    for (int i = 0; i < 32; ++i)
      output[i] = (tmp_out[i] == '1' ? TRUE : FALSE); 

    for (int i = 0; i < 32; ++i)
      Instructions[instruction_count][i] = output[i]; 
    ++instruction_count;
  }
  
  return instruction_count;
}


/******************************************************************************/
/* Program state - memory spaces, PC, and control */
/******************************************************************************/
BIT PC[32]                  = {FALSE};
BIT MEM_Instruction[32][32] = {FALSE};
BIT MEM_Data[32][32]        = {FALSE};
BIT MEM_Register[32][32]    = {FALSE};

BIT RegDst     = FALSE;
BIT Jump       = FALSE;
BIT Branch     = FALSE;
BIT MemRead    = FALSE;
BIT MemToReg   = FALSE;
BIT ALUOp[2]   = {FALSE};
BIT MemWrite   = FALSE;
BIT ALUSrc     = FALSE;
BIT RegWrite   = FALSE;
BIT Zero       = FALSE;
BIT Immediate  = FALSE;
BIT JumpLink   = FALSE;
BIT JumpReturn = FALSE;
BIT ALUControl[4] = {FALSE};
BIT ReadDataMemory[32];

void print_instruction()
{
  unsigned pc = binary_to_integer(PC);
  printf("PC: %d\n", pc);
  printf("Instruction: ");
  print_binary(MEM_Instruction[pc]);
  printf("\n");
}

void print_state()
{
  printf("Data: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Data[i]));
  } 
  printf("\n");  
  
  printf("Register: ");
  for (int i = 0; i < 32; ++i) {
    printf("%d ", binary_to_integer(MEM_Register[i]));
  } 
  printf("\n");
}


/******************************************************************************/
/* Functions that you will implement */
/******************************************************************************/
void Instruction_Memory(BIT* ReadAddress, BIT* Instruction)
{
  // TODO: Implement instruction memory
  // Input: 32-bit instruction address
  // Output: 32-bit binary instruction
  // Note: Useful to use a 5-to-32 decoder here
  BIT index[32];
  decoder5(ReadAddress, index);
  for (int i = 0; i < 32; ++i) {
    for (int j = 0; j < 32; ++j) {
      Instruction[j] = multiplexor2(index[i], Instruction[j], MEM_Instruction[i][j]);
    }
  }
}

void adder1(BIT A, BIT B, BIT CarryIn, BIT* CarryOut, BIT* Sum)
{
  // TODO: implement a 1-bit adder
  // Note: you can probably copy+paste this from your (or my) Lab 5 solution
  // *Sum = xor_gate(xor_gate(A,B),CarryIn);
  // *CarryOut = or_gate(and_gate(A,B),and_gate(CarryIn,xor_gate(A,B)));
  BIT x0 = xor_gate(A, B);
  *Sum = xor_gate(CarryIn, x0);
  BIT y0 = and_gate(x0, CarryIn);
  BIT y1 = and_gate(A, B);
  *CarryOut = or_gate(y0, y1);
  
}

void Control(BIT* OpCode,
  BIT* RegDst, BIT* Jump, BIT* Branch, BIT* MemRead, BIT* MemToReg,
  BIT* ALUOp, BIT* MemWrite, BIT* ALUSrc, BIT* RegWrite, BIT* Immediate, BIT* JumpLink, BIT* JumpReturn)
{
  // OpCode Shorthand
  BIT A = OpCode[5];
  BIT B = OpCode[4];
  BIT C = OpCode[3];
  BIT D = OpCode[2];
  BIT E = OpCode[1];
  BIT F = OpCode[0];
  //printf("OpCODE: %d%d%d%d%d%d", F, E, D, C, B, A);
  
  BIT is_lw = and_gate(and_gate3(A, not_gate(B), not_gate(C)), and_gate3(not_gate(D), E, F));
  BIT is_sw = and_gate(and_gate3(A,not_gate(B), C), and_gate3(not_gate(D), E ,F));
  BIT is_beq = and_gate(and_gate3(not_gate(A), not_gate(B), not_gate(C)), and_gate3(D, not_gate(E), not_gate(F)));
  BIT is_addi = and_gate(and_gate3(not_gate(A), not_gate(B), C), and_gate3(not_gate(D), not_gate(E), not_gate(F)));

  BIT is_j = and_gate(and_gate3(not_gate(A), not_gate(B), not_gate(C)), and_gate3(not_gate(D), E, not_gate(F)));
  BIT is_jal = and_gate(and_gate3(not_gate(A), not_gate(B), not_gate(C)), and_gate3(not_gate(D), E, F));
  BIT is_jr = and_gate(and_gate3(not_gate(A), not_gate(B), not_gate(C)), and_gate3(not_gate(D), not_gate(E), not_gate(F)));


  BIT is_and = and_gate(and_gate3(not_gate(A), not_gate(B), not_gate(C)), and_gate3(not_gate(D), not_gate(E), not_gate(F)));
  BIT is_or =  and_gate(and_gate3(not_gate(A), not_gate(B), not_gate(C)), and_gate3(not_gate(D), not_gate(E), not_gate(F)));
  BIT is_add = and_gate(and_gate3(not_gate(A), not_gate(B), not_gate(C)), and_gate3(not_gate(D), not_gate(E), not_gate(F)));
  BIT is_sub = and_gate(and_gate3(not_gate(A), not_gate(B), not_gate(C)), and_gate3(not_gate(D), not_gate(E), not_gate(F)));
  BIT is_slt = and_gate(and_gate3(not_gate(A), not_gate(B), not_gate(C)), and_gate3(not_gate(D), not_gate(E), not_gate(F)));



  // Determine format type
  BIT is_r_format = or_gate(or_gate3(is_and, is_or, is_add), or_gate3(is_sub, is_slt, is_jr));
  // Commented out for now:
  //BIT is_j_format = or_gate(is_j, is_jal);
  //BIT is_i_format = or_gate(or_gate3(is_lw, is_sw, is_beq), is_addi);

  // NOTES:
  // How to set zero, and ALUSrc? Zero is result from ALU and How to tell if second input is addi?
  // immidate --> lw, sw, addi, beq --> set ALUsrc
  // zero wont be set within control -> work with ALU part, zero as additional input to ALU function
  // Additional Lines for: (add input parameters)
  // jal determine if input to PC register
  // jr

  // Set ALUOp:
  ALUOp[1] = or_gate(is_r_format, is_addi);
  ALUOp[0] = is_beq;

  //SOP Representation:
  *JumpReturn = is_jr;
  *JumpLink = is_jal;
  *Immediate = is_addi;
  *ALUSrc = or_gate3(is_lw, is_sw, or_gate(is_addi, is_beq));
  *RegDst = is_r_format;
  *Jump = is_j;
  *Branch = is_beq;
  *MemRead = is_lw; // maybe not supposed to be set the same??
  *MemToReg = is_lw;
  *MemWrite = is_sw;
  *RegWrite = or_gate3(or_gate3(is_and, is_or, is_add), or_gate3(is_add, is_sub, is_slt), or_gate(is_lw, is_addi)); //include addi
  
 
  // TODO: Set control bits for everything
  // Input: opcode field from the instruction
  // OUtput: all control lines get set 
  // Note: Can use SOP or similar approaches to determine bits
  
}

void Read_Register(BIT* ReadRegister1, BIT* ReadRegister2,
  BIT* ReadData1, BIT* ReadData2)
{
  // TODO: Implement register read functionality
  // Input: two 5-bit register addresses
  // Output: the values of the specified registers in ReadData1 and ReadData2
  // Note: Implementation will be very similar to instruction memory circuit
  BIT index1[32];
  BIT index2[32];

  decoder5(ReadRegister1,index1);
  decoder5(ReadRegister2,index2);
  for(int i = 0;i < 32; i++) {
  for (int j = 0; j < 32; ++j){
      ReadData1[j] = multiplexor2(index1[i], ReadData1[j], MEM_Register[i][j]);
      ReadData2[j] = multiplexor2(index2[i], ReadData2[j], MEM_Register[i][j]);
    }
  }  

  
}

void Write_Register(BIT RegWrite, BIT* WriteRegister, BIT* WriteData)
{
  BIT index[32];
  decoder5(WriteRegister, index);

  for(int i = 0;i < 32; i++) {
    for (int j = 0; j < 32; ++j){
      MEM_Register[i][j] = multiplexor2(and_gate(index[i], RegWrite), MEM_Register[i][j], WriteData[j]);
    }
  } 
}

  // TODO: Implement register write functionality
  // Input: one 5-bit register address, data to write, and control bit
  // Output: None, but will modify register file
  // Note: Implementation will again be similar to those above


void ALU_Control(BIT* ALUOp, BIT* funct, BIT* ALUControl)
{
  
  // Short Hand for ALUOP BITS:
  BIT A = ALUOp[1];
  BIT B = ALUOp[0];
  
  // Hardcode First bit - ALWAY FALSE:
  ALUControl[3] = FALSE;
  //printf("FUNCT CODE: %d%d%d%d%d%d\n", funct[5],funct[4], funct[3], funct[2], funct[1], funct[0]);
  
  BIT is_add_funct = and_gate(and_gate3(and_gate3(funct[5], not_gate(funct[4]), not_gate(funct[3])), not_gate(funct[2]), not_gate(funct[1])),not_gate(funct[0]));
  BIT is_sub_funct = and_gate(and_gate3(and_gate3(funct[5], not_gate(funct[4]), not_gate(funct[3])), not_gate(funct[2]), funct[1]),not_gate(funct[0]));
  //BIT is_and_funct = and_gate(and_gate3(and_gate3(funct[5], not_gate(funct[4]), not_gate(funct[3])), funct[2], not_gate(funct[1])),not_gate(funct[0]));
  BIT is_or_funct = and_gate(and_gate3(and_gate3(funct[5], not_gate(funct[4]), not_gate(funct[3])), funct[2], not_gate(funct[1])), funct[0]);
  BIT is_setless_funct = and_gate(and_gate3(and_gate3(funct[5], not_gate(funct[4]), funct[3]), not_gate(funct[2]), funct[1]),not_gate(funct[0]));
  
  // Second bit SOP:
  ALUControl[2] = or_gate3(and_gate(not_gate(A), B), and_gate3(A, not_gate(B), is_sub_funct), and_gate3(A, not_gate(B), is_setless_funct));

  // Third bit SOP:
  ALUControl[1] = or_gate3(or_gate(or_gate(and_gate(not_gate(A),not_gate(B)), and_gate(not_gate(A),B)), and_gate3(A, not_gate(B), is_add_funct)), and_gate3(A, not_gate(B), is_sub_funct), and_gate3(A, not_gate(B), is_setless_funct));

  // Forth bit SOp:
  ALUControl[0] = or_gate(and_gate3(A, not_gate(B), is_or_funct), and_gate3(A, not_gate(B), is_setless_funct));
  //printf("ALUControl_Output: ");
  //printf("%d%d%d%d\n", ALUControl[3], ALUControl[2], ALUControl[1], ALUControl[0]);

  // TODO: Implement ALU Control circuit
  // Input: 2-bit ALUOp from main control circuit, 6-bit funct field from the
  //        binary instruction
  // Output:4-bit ALUControl for input into the ALU
  // Note: Can use SOP or similar approaches to determine bits
}
void ALU1(BIT A, BIT B, BIT Binvert, BIT CarryIn, BIT Less, 
  BIT Op0, BIT Op1, BIT Op2, BIT* Result, BIT* CarryOut, BIT* Set)
{
  BIT x0 = multiplexor2(Binvert, B, not_gate(B));
  BIT y0 = and_gate(A, x0);
  BIT y1 = or_gate(A, x0);
  BIT y3 = and_gate(Less,*Set);
  BIT y2 = FALSE;
  adder1(A, x0, CarryIn, CarryOut, &y2);
  *Set = y2; 
   // *Result = multiplexor4(Op0,Op1,y0,y1,y2,y3);
  BIT Result2 = multiplexor2(Op1, y0,y2);
  BIT Result3 = multiplexor2(Op1,y1,y3);
  *Result = multiplexor2(Op0,Result2,Result3 );
  // BIT Result2 = multiplexor4(Op0,Op1,1,1,1,1);
  // *Result = multiplexor2(Op2,Result1,Result2);
  
}

void ALU(BIT* ALUControl, BIT* Input1, BIT* Input2, BIT* Zero, BIT* Result)
{ 
  BIT Set=FALSE;
  BIT carryout_temp = FALSE;
  BIT Binvert;
  Binvert = ALUControl[2];

  BIT CarryIn = Binvert;
  BIT copy = CarryIn;
  for(int i=0;i<32;i++)
  {
    ALU1(Input1[i],Input2[i],Binvert,CarryIn,FALSE, ALUControl[0],ALUControl[1],ALUControl[2], &Result[i],&carryout_temp,&Set);
    CarryIn = carryout_temp;//Setting carry ins to the previous carry out
  }
  ALU1(Input1[0],Input2[0],Binvert,copy,TRUE, ALUControl[0],ALUControl[1],ALUControl[2], &Result[0],&carryout_temp,&Set);
  *Zero = FALSE;
  for(int i = 0;i<32;i++)
  {
  *Zero = or_gate(*Zero,Result[i]);
  }
  *Zero = not_gate(*Zero);

}

void Data_Memory(BIT MemWrite, BIT MemRead, 
  BIT* Address, BIT* WriteData, BIT* ReadData)
{
  BIT shorten_address[5];
  BIT index[32];
  for (int i = 5; i >= 0; --i) {
    shorten_address[i] = Address[i];
  }
  decoder5(shorten_address, index);
  for(int i = 0; i < 32; i++) {
    for (int j = 0; j < 32; ++j){
      MEM_Data[i][j] = multiplexor2(and_gate(index[i], MemWrite), MEM_Data[i][j], WriteData[j]);
      ReadData[j] = multiplexor2(and_gate(index[i], MemRead), ReadData[j], MEM_Data[i][j]);
    }
  }
  // Made global variable ReadDataMemory, for some reason ReadData wasnt working when transfered to caller
  for (int i = 0; i < 32; ++i) {
    ReadDataMemory[i] = ReadData[i];
  }

  // TODO: Implement data memory
  // Input: 32-bit address, control flags for read/write, and data to write
  // Output: data read if processing a lw instruction
  // Note: Implementation similar as above
  
}

void Extend_Sign16(BIT* Input, BIT* Output)
{
  // TODO: Implement 16-bit to 32-bit sign extender
  // Copy Input to Output, then extend 16th Input bit to 17-32 bits in Output
  for(int i=0;i<16;i++)
  {
    Output[i] = Input[i];
  }
  BIT copy = Input[15];
  for(int i=16;i<32;i++)
  {
    Output[i] = copy;
  }
}

void updateState()
{
  // TODO: Implement the full datapath here
  // Essentially, you'll be figuring out the order in which to process each of 
  // the sub-circuits comprising the entire processor circuit. It makes it 
  // easier to consider the pipelined version of the process, and handle things
  // in order of the pipeline. The stages and operations are:
  // Fetch - load instruction from instruction memory
  // Decode - set control bits and read from the register file
  // Execute - process ALU
  // Memory - read/write data memory
  // Write Back - write to the register file
  // Update PC - determine the final PC value for the next instruction
  BIT Instruction[32];
  BIT funct[6];
  BIT ReadRegister1In[5];
  BIT ReadRegister2In[5];
  BIT WriteRegisterIn[5];
  BIT ReadData1[32];
  BIT ReadData2[32];
  BIT signExtendInput[16];
  BIT signExtendOutput[32];
  BIT OpCode[6];
  
    Instruction_Memory(PC, Instruction);
    for(int k = 0; k < 5; k++){
      //Indstruction [25 -21]
      ReadRegister1In[k] = Instruction[k + 21];

      //Instrucion [20-16]
      ReadRegister2In[k] = Instruction[k+ 16];

      //Instruction[15-11]
      WriteRegisterIn[k] = Instruction[k+11];

    }
    // Get OPcode
    for(int k=26;k<32;k++)
    {
      OpCode[k-26] = Instruction[k];
    }

    // Run Control function to set lines
    Control(OpCode,&RegDst, &Jump, &Branch, &MemRead, &MemToReg, ALUOp,  &MemWrite,  &ALUSrc,  &RegWrite, &Immediate, &JumpLink, &JumpReturn);

    // Read data from regisers
    Read_Register(ReadRegister1In,ReadRegister2In,ReadData1,ReadData2);

    // Determine func code for ALU
    for(int j = 0; j < 6; j++){
      funct[j] = Instruction[j];
    }

    for(int i = 0; i < 16; i++){
      signExtendInput[i] = Instruction[i];
    }
    BIT is_jr_funct = and_gate(and_gate3(and_gate3(not_gate(funct[5]), not_gate(funct[4]), funct[3]), not_gate(funct[2]), not_gate(funct[1])), not_gate(funct[0]));
    Extend_Sign16(signExtendInput, signExtendOutput);


    BIT ALUControlOuput[4];

    //Run ALU_Control Function to determine ALU control codes
    ALU_Control(ALUOp,funct, ALUControlOuput);

    // Mux to determine if immediate value (addi)
    ALUControlOuput[1] = multiplexor2(Immediate, ALUControlOuput[1], Immediate); 

    // Get the input for ALU
    BIT ALUResult[32];
    BIT muxOutput[32];
    multiplexor2_32(and_gate(ALUSrc,not_gate(Branch)), ReadData2, signExtendOutput, muxOutput);

    // print_binary(muxOutput);
    //printf("ReadData1: %d\n", binary_to_integer(ReadData1));
    //printf("ReadData2: %d\n", binary_to_integer(ReadData2));
    //printf("muxOutput: %d\n", binary_to_integer(muxOutput));

    ALU(ALUControlOuput, ReadData1, muxOutput, &Zero, ALUResult);

    //printf("ALU OUTPUT: %d\n", binary_to_integer(ALUResult));
    
    Data_Memory(MemWrite, MemRead, ALUResult, ReadData2, ReadDataMemory);
    BIT mux_output[5];
    //printf("RegDSt: %d\n", RegDst);
    //printf("ReadRegister2In: %d%d%d%d%d\n", ReadRegister2In[4], ReadRegister2In[3], ReadRegister2In[2], ReadRegister2In[1], ReadRegister2In[0]);
    //printf("WriteRegisterIn: %d%d%d%d%d\n", WriteRegisterIn[4], WriteRegisterIn[3], WriteRegisterIn[2], WriteRegisterIn[1], WriteRegisterIn[0]);
    for (int j = 0; j < 5; ++j) {
      mux_output[j] = multiplexor2(RegDst, ReadRegister2In[j], WriteRegisterIn[j]);
    }


    BIT temp[32];
    for (int j = 0; j < 32; ++j) {
      temp[j] = multiplexor2(MemToReg, ALUResult[j], ReadDataMemory[j]);
    }
    //printf("MUX_OUTPUT: %d%d%d%d%d\n", mux_output[4], mux_output[3], mux_output[2], mux_output[1], mux_output[0]);
    Write_Register(and_gate(RegWrite, not_gate(is_jr_funct)), mux_output, temp);


    BIT OpAlu[4] = {FALSE, TRUE, FALSE, FALSE};  //Add opcode for PC increment
    BIT Zero_temp[32]; 
    BIT PC_temp[32];
    BIT PC_branch[32];
    

    //Increment PC and store in PC_temp
    ALU(OpAlu, PC, ONE, Zero_temp, PC_temp);

    // Increment PC for beq opperation offsets:
    ALU(OpAlu, PC_temp, signExtendOutput, Zero_temp, PC_branch);
    
    // Determine if we should copy over RA to PC:
    BIT ReturnAddress[5] = {TRUE, TRUE, TRUE, TRUE, TRUE};
    Write_Register(JumpLink, ReturnAddress, PC_temp);
    
    //SOP representation to determine how to update PC
    //BIT determine_PC_update = or_gate3(and_gate(Zero,Branch), Jump, JumpLink);
  

    // Determine if PC will update to next instruction or to branch
    for(int i = 0; i < 32; i++){
      PC[i] = multiplexor2(and_gate(Zero,Branch), PC_temp[i], PC_branch[i]);
    } 
    
    //Check if jal:
    for(int i = 0; i < 32; i++){
      PC[i] = multiplexor2(or_gate(JumpLink, Jump), PC[i], signExtendOutput[i]);
    } 
   
    //Check if jr
    // Read ReturnRegister Address:
    BIT RA[32];
    Read_Register(ReturnAddress, ZERO, RA, Zero_temp);
    //001000
    //printf("ALUSRC: %d\n", ALUSrc);
    //printf("ALuCOntrol: %d%d%d%d\n", ALUControlOuput[3], ALUControlOuput[2], ALUControlOuput[1], ALUControlOuput[0]);
    //printf("BEFORE RA: %d\n", binary_to_integer(RA));
    //printf("JUMPRETURN: %d\n", and_gate(JumpReturn, is_jr_funct));
    for (int i = 0; i < 32; i++) {
      PC[i] = multiplexor2(and_gate(JumpReturn, is_jr_funct), PC[i], RA[i]);
    }
    //printf("AFTER RA: %d\n", binary_to_integer(RA));
    


    // COMMENTED THESE NOODLES OUT, IDK What they do??
    
    //BIT ALU_result[32];
    /*
    ALU(OpAlu, signExtendOutput,PC_temp, Zero_temp, ALU_result ); 
    BIT mux_out[32]; 

    multiplexor2_32(and_gate(Zero,Branch), PC_temp, ALU_result, mux_out);

    BIT mux_output_arr[32]; 
    multiplexor2_32(Jump, PC_temp ,mux_out, mux_output_arr); 
    */ 
  
}


/******************************************************************************/
/* Main */
/******************************************************************************/

int main()
{
  setbuf(stdout, NULL);
    
  // parse instructions into binary format
  int counter = get_instructions(MEM_Instruction);

  // load program and run
  copy_bits(ZERO, PC);
  copy_bits(THIRTY_TWO, MEM_Register[29]);
  
  while (binary_to_integer(PC) < counter) {

    print_instruction();
    updateState();
    print_state();
  }
  return 0;
}
