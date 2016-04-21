#include "spimcore.h"
/*
Tyler did all of this
*/


/* ALU */
/* 10 Points */
/* :3 */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
  	//Read the control number 0-7
  if(ALUControl == 0){ //Addition
    *ALUresult = A + B;
  }
  else if(ALUControl == 1){ //Subtraction
    *ALUresult = A - B
  }
  else if(ALUControl == 2){ //Set less than (signed)
    if((int)A < (int)B){ //Set to int so that it converts as signed binary
      *ALUresult = 1;
    }
    else{
      *ALUresult = 0;
    }

  }
  else if(ALUControl == 3){ //Set less than (unsigned)
     if(A < B){
      *ALUresult = 1;
    }
    else{
      *ALUresult = 0;
    }
  }
  else if(ALUControl == 4){ //AND operation
  	*ALUresult = A&B;

  }
  else if(ALUControl == 5){ //OR operation
  	*ALUresult = A|B;
  }
  else if(ALUControl == 6){ //Shift left
    B>>16; //Makes the leftmost 16 bits B

  }
  else if(ALUControl == 7){ //NOT operation
    *ALUresult = ~A;
  }

  //Evaluate if zero is 0 or 1
  if(*ALUresult == 0){
    *Zero = 1;
  }
  else{
    *Zero = 0;
  }
}

/* instruction fetch */
/* 10 Points */
/* :3 */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
  	// If Mem Halts.
    if(PC % 4 == 0 && PC >> 2 < MEMSIZE)
    {
        *instruction = Mem[PC >> 2];
        return 0;
    }
    return 1;
}


/* instruction partition */
/* 10 Points */
/* :3 */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	// Partition instruction into 7 parts
  	// Apparently there is something important in spimcore.c on lines 41-47

	// Partition the Op Code
  	*op  = (instruction & 0xFC000000) >> 26;

  	// Partition the first source register (r1).
  	*r1 = (instruction & 0x3E00000) >> 21;

  	// Partition the second source register (r2).https://github.com/JamesBedont/MipsSimulation/blob/master/project.c
  	*r2 = (instruction & 0x1F0000) >> 16;

  	// Partition the destination register (r3).
  	*r3 = (instruction & 0xF800) >> 11;

  	// Partition the function.
  	*funct = (instruction & 0x3F);

  	// Partition the offset.
  	*offset = (instruction & 0xFFFF);

  	// Partition the jump address (jsec).
  	*jsec = (instruction & 0x3FFFFFF);

}



/* instruction decode */
/* 15 Points */
/* JASPER DIDN'T WRITE THE SMILEY FACE */
int instruction_decode(unsigned op,struct_controls *controls)
{

  switch(op)
  {
    //R-Type
    case 0:
    	controls->RegDst = 1;
        controls->Jump = 0;
      	controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0x111;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 1;
    	break;
    //Lw
    case 35:
    	controls->RegDst = 1;
        controls->Jump = 0;
      	controls->Branch = 0;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->ALUOp = 0x000;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    	break;
    //Sw
    case 43:
    	controls->RegDst = 2;
        controls->Jump = 0;
      	controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 2;
        controls->ALUOp = 0x000;
        controls->MemWrite = 1;
        controls->ALUSrc = 1;
        controls->RegWrite = 0;
    	break;
    //beq
    case 4:
    	controls->RegDst = 2;
        controls->Jump = 0;
      	controls->Branch = 1;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0x001;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->RegWrite = 0;
    	break;
    //addi
    case 8:
    	controls->RegDst = 0;
        controls->Jump = 0;
      	controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0x000;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    	break;
    //lui
    case 15:
    	controls->RegDst = 1;
        controls->Jump = 0;
      	controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0x110;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    	break;
    //slti
    case 10:
    	controls->RegDst = 0;
        controls->Jump = 0;
      	controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0x010;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    	break;
    //sltui
    case 7:
    	controls->RegDst = 0;
        controls->Jump = 0;
      	controls->Branch = 0;
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->ALUOp = 0x011;
        controls->MemWrite = 0;
        controls->ALUSrc = 1;
        controls->RegWrite = 1;
    	break;
    defualt: return 0;
  }
  return 1;
}

/* Read Register */
/* 5 Points */
/* :3 */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

  	// Take the values found in data1 and data2 and place
  	// them in their respective spots in the register.
  	*data1 = Reg[r1];
  	*data2 = Reg[r2];

}


/* Sign Extend */
/* 10 Points */
/* :3 */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	// Assign the sign-extended value of offset to extended_value.
  	if (offset >> 15 == 1) *extended_value = offset | 0xFFFF0000;
  	else *extended_value = offset;
}

/* ALU operations */
/* 10 Points */
/* :3 */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
  if(ALUOp == 0x111)
  {
    switch(funct)
    {
      //add
      case 32:
      ALU(data1, data2, 0x000, ALUresult, Zero);
      break;
      //sub
      case 34:
      ALU(data1, data2, 0x001, ALUresult, Zero);
      break;
      //and
      case 36:
      ALU(data1, data2, 0x100, ALUresult, Zero);
      break;
      //or
      case 37:
      ALU(data1, data2, 0x101, ALUresult, Zero);
      break;
      //slt
      case 42:
      ALU(data1, data2, 0x010, ALUresult, Zero);
      break;
      //sltu
      case 43:
      ALU(data1, data2, 0x011, ALUresult, Zero);
      break;
      //other
      default: return 1;
    }
    return 0;
  }
  switch(ALUOp)
  {
    //addi
    case 0:
    ALU(data1, extended_value, 0x000, ALUresult, Zero);
    break;
    //beq
    case 1:
    ALU(data1, extended_value, 0x001, ALUresult, Zero);
    break;
    //slti
    case 2:
    ALU(data1, extended_value, 0x010, ALUresult, Zero);
    break;
    //sltui
    case 3:
    ALU(data1, extended_value, 0x011, ALUresult, Zero);
    break;
    //and
    case 4:
    ALU(data1, extended_value, 0x100, ALUresult, Zero);
    break;
    //or
    case 5:
    ALU(data1, extended_value, 0x101, ALUresult, Zero);
    break;
    //lui
    case 6:
    ALU(data1, extended_value, 0x110, ALUresult, Zero);
    break;
    default: return 1;
  }
	return 0;



	//ALCSrc will operate on either data1, data2, or extended_value
  	//apply to ALU
  	//output to ALUresult


}

/* Read / Write Memory */
/* 10 Points */
/* :3 */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

  // Check to make sure the alignment of the ALUresult is appropriate.
  int alignment = ALUresult % 4;

  if (MemRead == 1)
  {
  	// If there is anything wrong with the ALUresult, halt.
  	if (alignment != 0) return 1;

    // Read data from memory.
    *memdata  = Mem[ALUresult >> 2];
  }

  if (MemWrite == 1)
  {
    // If there is anything wrong with the ALUresult, halt.
   	if (alignment != 0) return 1;

    // Write data from data2 into memory.
    Mem[ALUresult >> 2] = data2;
  }
}


/* Write Register */
/* 10 Points */
/* :3 */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
  int x;
  if(RegWrite){ //If RegWrite is 1, write to the register. If not, do nothing

      //If RegDest is 1, write to 15-11 (which is r3), else 20-16 (r2)
    x = (RegDest) ? r3 : r2;

    if(MemtoReg){//If MemtoReg is 1, then read the data from memory
      Reg[x] = memdata;
    }
    else{ //If it's not, read the data from the ALUresult
      Reg[x] = ALUresult;
    }
  }
}

/* PC update */
/* 10 Points */
/* :3 */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

  	// Updates the program counter.
  	if(Jump == 1)
      *PC = (*PC & 0xF0000000) + jsec * 4;
  	else if(Branch == 1 && Zero == 1)
      *PC = *PC + 4 + extended_value * 4;
    else *PC += 4;
