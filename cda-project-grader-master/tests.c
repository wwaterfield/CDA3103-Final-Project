#include "project.c"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"

void assertEquals_special (const char * callerName, const char * message, unsigned a, unsigned b) {
	if (a != b) {
		printf(
			RED "✕ %s -> " RESET "%s " YELLOW "(%u != %u)" RESET "\n",
			callerName, message, a, b);
	} else {
		// printf(
		// 	GREEN "✓ %s -> " RESET "%s\n",
		// 	callerName, message);
	}
}

#define assertEquals(message, a, b) assertEquals_special(__FUNCTION__, message, a, b)

void instructionFetchTest () {
	unsigned memory[] = { 0, 1, 2, 3 };
	unsigned instruction = -1;
	unsigned pc = 12;

	int result = instruction_fetch(3, memory, &instruction);
	assertEquals("Halts on non-word-aligned PC value", result, 1);

	result = instruction_fetch(pc, memory, &instruction);
	assertEquals("Doesn't halt on word-aligned value", result, 0);
	assertEquals("Fetches PC value from memory", instruction, memory[pc / 4]);
}

void instructionPartitionTest () {
	unsigned rType = 0b10000110001100111011110001100001;

	unsigned op = 0, r1 = 0, r2 = 0, r3 = 0, funct = 0, offset = 0, jsec = 0;

	instruction_partition(rType, &op, &r1, &r2, &r3, &funct, &offset, &jsec);
	assertEquals("Extracts op", op, 0b100001);
	assertEquals("Extracts rs", r1, 0b10001);
	assertEquals("Extracts rt", r2, 0b10011);
	assertEquals("Extracts rd", r3, 0b10111);
	assertEquals("Extracts funct", funct, 0b100001);
	assertEquals("Extracts offset", offset, 0b1011110001100001);
	assertEquals("Extracts jsec", jsec, 0b10001100111011110001100001);
}

void signExtendTest () {
	unsigned positiveOffset = 0b00000000000000000111111111111111;
	unsigned expectedPositive = 0b00000000000000000111111111111111;

	unsigned negativeOffset = 0b00000000000000001111111111111111;
	unsigned expectedNegative = 0b11111111111111111111111111111111;

	sign_extend(positiveOffset, &positiveOffset);
	sign_extend(negativeOffset, &negativeOffset);

	assertEquals("Doesn't extend zeroes for positive value", positiveOffset, expectedPositive);
	assertEquals("Extends with ones for negative values", negativeOffset, expectedNegative);
}

void writeRegisterTest () {
	unsigned r2 = 2;
	unsigned r3 = 3;
	unsigned registerFile[] = { 1, 2, 3, 4, 5 };

	int memData = 50;
	int aluResult = 75;

	write_register(0, 0, memData, aluResult, 0, 0, 0, registerFile);
	assertEquals("Register isn't modified when RegWrite is deasserted", registerFile[0], 1);

	write_register(r2, r3, memData, aluResult, 1, 0, 0, registerFile);
	assertEquals("Writes ALUresult -> rs when appropriate", registerFile[r2], aluResult);

	write_register(r2, r3, memData, aluResult, 1, 0, 1, registerFile);
	assertEquals("Writes memdata -> rs when appropriate", registerFile[r2], memData);

	write_register(r2, r3, memData, aluResult, 1, 1, 0, registerFile);
	assertEquals("Writes ALUresult -> rt when appropriate", registerFile[r3], aluResult);

	write_register(r2, r3, memData, aluResult, 1, 1, 1, registerFile);
	assertEquals("Writes memdata -> rt when appropriate", registerFile[r3], memData);
}

void pcUpdateTest () {
	unsigned pc = 4000;

	PC_update(0, 0, 0, 0, 0, &pc);
	assertEquals("PC gets incremented by 4", pc, 4004);

	pc = 4000;
	PC_update(0, 3, 1, 0, 1, &pc);
	assertEquals("Uses PC-relative addressing when branching", pc, 4016);

	pc = 4000;
	PC_update(0, 3, 1, 0, 0, &pc);
	assertEquals("Uses incremental addressing when branch isn't taken", pc, 4004);

	pc = 4000;
	PC_update(0, 3, 1, 0, 0, &pc);
	assertEquals("Uses incremental addressing when branch isn't taken", pc, 4004);

	pc = 0b11110000000000000000000000000000;
	unsigned jsec = 0b00000000000000001111111111;
	unsigned expected = 0b11110000000000000000111111111100;

	PC_update(jsec, 0, 0, 1, 0, &pc);
	assertEquals("Uses pseudo-direct addressing when jumping", pc, expected);
}

int main () {
	instructionFetchTest();
	instructionPartitionTest();
	signExtendTest();
	writeRegisterTest();
	pcUpdateTest();

	return 0;
}
