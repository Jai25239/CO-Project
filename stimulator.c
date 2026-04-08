#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int PC = 0;
typedef struct {
    char name[6];     
    char encoding[6]; 
    int value;
}Register;

typedef struct {
    char opcode[9];
    char inst[6];
} Opcode;

typedef struct {
    int decimal_address;
    char hex_address[12];
    int value;
} Memory;

Opcode InsList[] = {
    {"0110011", "R"}, {"0100011", "S"}, {"1100011", "B"}, {"0110111", "U"},{"0010111","U"},
    {"1101111", "J"},
    {"0000011", "Lw"}, {"0010011", "Addi"}, {"0010011", "Sltiu"}, {"1100111", "Jalr"}
};

Memory MemList[] = {
    {0,  "0x0001_0000", 0},{1,  "0x0001_0004", 0},{2,  "0x0001_0008", 0},{3,  "0x0001_000C", 0},{4,  "0x0001_0010", 0},{5,  "0x0001_0014", 0},
    {6,  "0x0001_0018", 0},{7,  "0x0001_001C", 0},{8,  "0x0001_0020", 0},{9,  "0x0001_0024", 0},{10, "0x0001_0028", 0},{11, "0x0001_002C", 0},
    {12, "0x0001_0030", 0},{13, "0x0001_0034", 0},{14, "0x0001_0038", 0},{15, "0x0001_003C", 0},{16, "0x0001_0040", 0},{17, "0x0001_0044", 0},
    {18, "0x0001_0048", 0},{19, "0x0001_004C", 0},{20, "0x0001_0050", 0},{21, "0x0001_0054", 0},{22, "0x0001_0058", 0},{23, "0x0001_005C", 0},
    {24, "0x0001_0060", 0},{25, "0x0001_0064", 0},{26, "0x0001_0068", 0},{27, "0x0001_006C", 0},{28, "0x0001_0070", 0},{29, "0x0001_0074", 0},
    {30, "0x0001_0078", 0},
    {31, "0x0001_007C", 0}
};

Register RegList[] = {
        {"zero", "00000", 0},
        {"ra", "00001", 0},
        {"sp", "00010", 0},
        {"gp", "00011", 0},
        {"tp", "00100", 0},
        {"t0", "00101", 0}, {"t1", "00110", 0}, {"t2", "00111", 0},
        {"s0", "01000", 0}, {"fp", "01000", 0},
        {"s1", "01001", 0},
        {"a0", "01010", 0}, {"a1", "01011", 0},
        {"a2", "01100", 0}, {"a3", "01101", 0}, {"a4", "01110", 0}, {"a5", "01111", 0}, {"a6", "10000", 0}, {"a7", "10001", 0},
        {"s2", "10010", 0}, {"s3", "10011", 0}, {"s4", "10100", 0}, {"s5", "10101", 0}, {"s6", "10110", 0},
        {"s7", "10111", 0}, {"s8", "11000", 0}, {"s9", "11001", 0}, {"s10", "11010", 0}, {"s11", "11011", 0},
        {"t3", "11100", 0}, {"t4", "11101", 0}, {"t5", "11110", 0}, {"t6", "11111", 0}
    };

int bin_to_dec(char* bin, int bit){
    int dec =0;

    for(int i=0; i<bit;i++){
        char b = *(bin+bit-1-i);
        if(b == '1'){
            dec = dec+ (1<<i);
        }
        else { continue;
        }
    }
    if(*bin == '1'){
        dec -= (1<<bit);
    }
    return dec;
}

Memory* find_memory(int address){
    for(int i=0; i<32; i++){
        if(MemList[i].decimal_address == address){
            return &MemList[i];
        }
    }
}

char* find_inst_from_opcode(char given_inst_opcode[]){
    for (int i = 0; i<10; i++){
        if (strcmp(given_inst_opcode, InsList[i].opcode) == 0){
            return InsList[i].inst;
        }
    }
    return NULL;
}

//Find the register from Reglist and return it's pointer, because we need the changes to be made in RegList itself.
Register* find_register(char address[]){
    for (int i = 0; i<33; i++){
        if (strcmp(address, RegList[i].encoding) == 0){
            return &RegList[i];
        }
    }
    return NULL;
}

//Decoder specifically for R-type
int R_decoder(char whole_inst[], int index){
    //Spliting the 32 bits instruction based on R-type format
    char funct7[8];
    char rs2_address[6];
    char rs1_address[6];
    char funct3[4];
    char rd_address[6];
    strncpy(funct7, whole_inst, 7);
    funct7[7] = '\0';
    strncpy(rs2_address, whole_inst + 7, 5);
    rs2_address[5] = '\0';
    strncpy(rs1_address, whole_inst + 12, 5);
    rs1_address[5] = '\0';
    strncpy(funct3, whole_inst + 17, 3);
    funct3[3] = '\0';
    strncpy(rd_address, whole_inst + 20, 5);
    rd_address[5] = '\0';

    //Finding those registers in RegList
    Register* rs2 = find_register(rs2_address);
    Register* rs1 = find_register(rs1_address);
    Register* rd = find_register(rd_address);

    //ADD
    if ((strcmp("000", funct3) == 0) && (strcmp("0000000", funct7) == 0)){
        rd->value = rs1->value + rs2->value;
        return 0;
    }
    //SUB
    else if ((strcmp("000", funct3) == 0) && (strcmp("0100000", funct7) == 0)){
        rd->value = rs1->value - rs2->value;
        return 0;
    }
    //SLL
    else if (strcmp("001", funct3) == 0){
        if(rs2->value >= 0){
            rd->value = (rs1->value)<<(rs2->value);
        }
        else{
            rd->value = (rs1->value)<<((rs2->value)*(-1));
        }
        return 0;
    }
    //SLT
    else if (strcmp("010", funct3) == 0){
        if((rs1->value) < (rs2->value)){
            rd->value = 1;
        }
        else{
            rd->value = 0;
        }
        return 0;
    }
    //SLTU
    else if (strcmp("011", funct3) == 0){
        if ((unsigned int)(rs1->value) < (unsigned int)(rs2->value)){
        rd->value = 1;
        } 
        else {
        rd->value = 0;
        }
        return 0;
    }
    //XOR
    else if (strcmp("100", funct3) == 0){
        rd->value = (rs1->value)^(rs2->value);
        return 0;
    }
    //SRL
    else if (strcmp("101", funct3) == 0){
        if(rs2->value >= 0){
            rd->value = (rs1->value)>>(rs2->value);
        }
        else{
            rd->value = (rs1->value)>>((rs2->value)*(-1));
        }
        return 0;
    }
    //OR
    else if (strcmp("110", funct3) == 0){
        rd->value = (rs1->value)|(rs2->value);
        return 0;
    }
    //AND
    else if (strcmp("111", funct3) == 0){
        rd->value = rs1->value & rs2->value; 
        return 0;
    }
    else {
        printf("\nError in line %d", index + 1);
        return -1;}
}

int S_decoder(char whole_inst[], int index){
    char funct3[4];
    strncpy(funct3,whole_inst + 17,3);
    funct3[3] = '\0';
    //First checking if the funct3 is 010 for store or not.
    if(strcmp(funct3,"010")==0){

        // Breaking the 32 bit instruction based on S type instruction.
        char imm[13];
        char rs2_address[6];
        char rs1_address[6];
        strncpy(imm,whole_inst+20,5);
        strncpy(imm+5,whole_inst,7);
        imm[12]='\0';
        strncpy(rs2_address, whole_inst+7,5);
        rs2_address[5]='\0';
        strncpy(rs1_address,whole_inst+12,5);
        rs1_address[5]='\0';

        //Finding the registers.
        Register* r2 = find_register(rs1_address);
        Register* r1 = find_register(rs2_address);

        //Searching for the memory address given and putting in it.
        Memory* m1 = find_memory(bin_to_dec(imm,12)+(r1->value));
        m1->value = r2->value;
        return 0;
    }

    //Printing error if funct3 is not 010.
    printf("\nError in line %d", index + 1);
    return -1;
}

int B_decoder(char whole_inst[], int index){
    char rs2_address[6];
    char rs1_address[6];
    char funct3[4];
    char imm[14];  // 13 bits + null terminator
    
    //rs2 [24:20] → bits 7-11 in whole_inst 
    strncpy(rs2_address, whole_inst + 7, 5);
    rs2_address[5] = '\0';
    
    //rs1 [19:15] → bits 12-16 in whole_inst
    strncpy(rs1_address, whole_inst + 12, 5);
    rs1_address[5] = '\0';
    
    //funct3 [14:12] → bits 17-19 in whole_inst
    strncpy(funct3, whole_inst + 17, 3);
    funct3[3] = '\0';
    
    //imm[12] is bit 0 of whole_inst (bit 31 of the instruction)
    imm[0] = whole_inst[0];
    
    //imm[10:5] are bits 1-6 of whole_inst (bits 30:25)
    strncpy(imm + 1, whole_inst + 1, 6);
    
    //imm[4:1] are bits 20-23 of whole_inst (bits 11:8)
    strncpy(imm + 7, whole_inst + 20, 4);
    
    //imm[11] is bit 24 of whole_inst (bit 7)
    imm[11] = whole_inst[24];
    
    //imm[0] is always 0 (branch targets are 2-byte aligned)
    imm[12] = '0';
    imm[13] = '\0';
    
    Register* rs1 = find_register(rs1_address);
    Register* rs2 = find_register(rs2_address);

    //beq
    if (strcmp(funct3, "000")== 0){
        if (rs1->value == rs2->value){
            return bin_to_dec(imm, 13)/4 + index;
        }
        else return index + 1;
    }
    //bne
    else if (strcmp(funct3, "001")== 0){
        if (rs1->value != rs2->value){
            return bin_to_dec(imm, 13)/4 + index;
        }
        else return index + 1;
    }
    //blt
    else if (strcmp(funct3, "100")== 0){
        if (rs1->value < rs2->value){
            return bin_to_dec(imm, 13)/4 + index;
        }
        else return index + 1;
    }
    //bge
    else if (strcmp(funct3, "101")== 0){
        if (rs1->value >= rs2->value){
            return bin_to_dec(imm, 13)/4 + index;
        }
        else return index + 1;
    }
    //bltu
    else if (strcmp(funct3, "110")== 0){
        if ((unsigned int)(rs1->value) < (unsigned int)(rs2->value)){
            return bin_to_dec(imm, 13)/4 + index;
        }
        else return index + 1;
    }
    //bgeu
    else if (strcmp(funct3, "111")== 0){
        if ((unsigned int)(rs1->value) >= (unsigned int)(rs2->value)){
            return bin_to_dec(imm, 13)/4 + index;
        }
        else return index + 1;
    }
    else {
        printf("\nError in line %d", index + 1);
        return -1;}
}

int U_decoder(char whole_inst[], int index){
    //Spliting the 32 bits instruction based on R-type format
    char opcode[8];
    char imm[21];
    char rd_address[6];
    strncpy(imm,whole_inst,20);
    imm[20]='\0';
    strncpy(rd_address,whole_inst+20,5);
    rd_address[5] = '\0';
    strncpy(opcode,whole_inst+25,7);
    opcode[7] = '\0';
    

    Register* rd = find_register(rd_address);

    if(strcmp(opcode,"0110111")==0){
        rd->value = bin_to_dec(imm,20)<<12;
        printf("rd have %d ",rd->value);
        return 0;
    }
    else if(strcmp(opcode,"0010111")==0){
        //rd->value = PC + (bin_to_dec(imm,20)<<12); 
        return 0;
    }
    else {
        printf("\nError in line %d", index + 1);
        return -1;
    }
}

int J_decoder(char whole_inst[], int index){
    char imm[22];        //21 bits + null fucking terminator
    char rd_address[6];
    
    //rd [11:7] - bits 20-24 in whole_inst
    strncpy(rd_address, whole_inst + 20, 5);
    rd_address[5] = '\0';
    
    //imm[20] is bit 0 of whole_inst (bit 31 of instruction)
    imm[0] = whole_inst[0];
    
    //imm[10:1] are bits 1-10 of whole_inst (bits 30:21)
    strncpy(imm + 1, whole_inst + 1, 10);
    
    //imm[11] is bit 11 of whole_inst (bit 20)
    imm[11] = whole_inst[11];
    
    // imm[19:12] are bits 12-19 of whole_inst (bits 19:12)
    strncpy(imm + 12, whole_inst + 12, 8);
    
    // imm[0] is always 0 
    imm[20] = '0';
    
    imm[21] = '\0';
    
    Register* rd = find_register(rd_address);
    
    rd->value = (index + 1)*4; // Save return address
    return index + bin_to_dec(imm, 21)/4;
}

int Lw_decoder(char whole_inst[], int index){
    char funct3[4];
    strncpy(funct3,whole_inst + 17,3);
    funct3[3] = '\0';
    //First checking if the funct3 is 010 for store or not.
    if(strcmp(funct3,"010")==0){

        // Breaking the 32 bit instruction based on S type instruction.
        char imm[13];
        char rd_address[6];
        char rs1_address[6];
        strncpy(imm,whole_inst,12);
        imm[12]='\0';
        strncpy(rs1_address,whole_inst+12,5);
        rs1_address[5]='\0';      
        strncpy(rd_address,whole_inst+20,5);
        rd_address[5]='\0';

        //Finding the registers.
        Register* rd = find_register(rd_address);
        Register* r1 = find_register(rs1_address);

        //Searching for the memory address given and putting in it.
        Memory* m1 = find_memory((r1->value)+bin_to_dec(imm,12));
        rd->value = m1->value;
        printf("rd have %d ",rd->value);
        return 0;
    }
    printf("\nError in line %d", index + 1);
    return -1;
}

int Addi_decoder(char whole_inst[], int index){
    char imm[13];        // 12 bits + fucking null terminator
    char rs1_address[6];
    char funct3[4];
    char rd_address[6];
    
    //imm[11:0] -bits 0-11 in whole_inst (bits 31:20 of instruction)
    strncpy(imm, whole_inst, 12);
    imm[12] = '\0';
    
    //rs1 [19:15] - bits 12-16 in whole_inst
    strncpy(rs1_address, whole_inst + 12, 5);
    rs1_address[5] = '\0';
    
    //funct3 [14:12] - bits 17-19 in whole_inst
    strncpy(funct3, whole_inst + 17, 3);
    funct3[3] = '\0';
    
    //rd [11:7] - bits 20-24 in whole_inst
    strncpy(rd_address, whole_inst + 20, 5);
    rd_address[5] = '\0';
    
    Register* rs1 = find_register(rs1_address);
    Register* rd = find_register(rd_address);
    
    if (strcmp(funct3, "000") == 0) {
        rd->value = rs1->value + bin_to_dec(imm, 12);
        return 0;
    }
    else {
        printf("\nError in line %d", index + 1);
        return -1;}
}

int Sltui_decoder(char whole_inst[], int index){
    char imm[13];        // 12 bits + fucking null terminator
    char rs1_address[6];
    char funct3[4];
    char rd_address[6];
    
    //imm[11:0] -bits 0-11 in whole_inst (bits 31:20 of instruction)
    strncpy(imm, whole_inst, 12);
    imm[12] = '\0';
    
    //rs1 [19:15] - bits 12-16 in whole_inst
    strncpy(rs1_address, whole_inst + 12, 5);
    rs1_address[5] = '\0';
    
    //funct3 [14:12] - bits 17-19 in whole_inst
    strncpy(funct3, whole_inst + 17, 3);
    funct3[3] = '\0';
    
    //rd [11:7] - bits 20-24 in whole_inst
    strncpy(rd_address, whole_inst + 20, 5);
    rd_address[5] = '\0';
    
    Register* rs1 = find_register(rs1_address);
    Register* rd = find_register(rd_address);

    if (strcmp(funct3, "011") == 0){
        if ((unsigned int)(rs1->value) < (unsigned int)(bin_to_dec(imm, 12))){
            rd->value = 1;
            return 0;
        }
        else {
            rd->value = 0;
            return 0; 
        }
    }
    printf("\nError in line %d", index + 1);
    return -1;
}

int Jalr_decoder(char whole_inst[], int index){
    char imm[13];        // 12 bits + fucking null terminator
    char rs1_address[6];
    char funct3[4];
    char rd_address[6];
    
    //imm[11:0] -bits 0-11 in whole_inst (bits 31:20 of instruction)
    strncpy(imm, whole_inst, 12);
    imm[12] = '\0';
    
    //rs1 [19:15] - bits 12-16 in whole_inst
    strncpy(rs1_address, whole_inst + 12, 5);
    rs1_address[5] = '\0';
    
    //funct3 [14:12] - bits 17-19 in whole_inst
    strncpy(funct3, whole_inst + 17, 3);
    funct3[3] = '\0';
    
    //rd [11:7] - bits 20-24 in whole_inst
    strncpy(rd_address, whole_inst + 20, 5);
    rd_address[5] = '\0';
    
    Register* rs1 = find_register(rs1_address);
    Register* rd = find_register(rd_address);

    if (strcmp(funct3, "000")== 0){
        rd->value = (index + 1)*4;
        return (rs1->value + bin_to_dec(imm, 12))/4;
    }
    else {
        printf("\nError in line %d", index + 1);
        return -1;}
}

int Master_decoder(char whole_inst[], int index){
    //Opcode extraction
    char given_opcode[8];
    strncpy(given_opcode, whole_inst + 25, 7);
    given_opcode[7] = '\0';

    //find which instruction it is
    char* inst_name = find_inst_from_opcode(given_opcode);

    if (strcmp(inst_name, "R") == 0){
        printf("%c", 'R');
        int result = R_decoder(whole_inst, index);
        if (result == -1) return -1;
        return index + 1;
    }

    if (strcmp(inst_name, "S") == 0){
        printf("%c", 'S');
        int result = S_decoder(whole_inst, index);
        if (result == -1) return -1;
        return index + 1;
    }

    if (strcmp(inst_name, "B") == 0){
        printf("%c", 'B');
        return B_decoder(whole_inst, index);
    }

    if (strcmp(inst_name, "U") == 0){
        printf("%c", 'U');
        int result = U_decoder(whole_inst, index);
        if (result == -1) return -1;
        return index + 1;
    }

    if (strcmp(inst_name, "J") == 0){
        printf("%c", 'J');
        return J_decoder(whole_inst, index);
    }

    if (strcmp(inst_name, "Lw") == 0){
        printf("Lw");
        int result = Lw_decoder(whole_inst, index);
        if (result == -1) return -1;
        return index + 1;
    }

    if (strcmp(inst_name, "Addi") == 0){
        printf("Addi");
        int result = Addi_decoder(whole_inst, index);
        if (result == -1) return -1;
        return index + 1;
    }

    if (strcmp(inst_name, "Sltiu") == 0){
        printf("Sltiu");
        int result = Sltui_decoder(whole_inst, index);
        if (result == -1) return -1;
        return index + 1;
    }

    if (strcmp(inst_name, "Jalr") == 0){
        printf("Jalr");
        return Jalr_decoder(whole_inst, index);
    }

    return -1;
}

void stimulator(FILE* input, FILE* output){
    int VHalt = 0;
    int index = 0;
    char instructions[1000][33];  
    int instruction_count = 0;
    char halt_inst[] = "00000000000000000000000001100011";

    while(fgets(instructions[instruction_count], 100, input) != NULL){
        instructions[instruction_count][strcspn(instructions[instruction_count], "\r\n")] = '\0';
        instruction_count++;
    }

    while (index < instruction_count){
        if (strcmp(instructions[index], halt_inst) == 0){
            printf("\nVirtual halt detected. Terminating simulation.\n");
            VHalt = 1;
            break;
        }
        index = Master_decoder(instructions[index], index);
        RegList[0].value = 0;
        if (index == -1){
            return;
        }
        fprintf(output, "\n%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
        index*4, RegList[0].value, RegList[1].value, RegList[2].value, RegList[3].value, RegList[4].value, RegList[5].value, RegList[6].value, RegList[7].value, RegList[8].value, RegList[9].value, RegList[10].value, RegList[11].value, RegList[12].value, RegList[13].value, RegList[14].value, RegList[15].value, RegList[16].value, RegList[17].value, RegList[18].value, RegList[19].value, RegList[20].value, RegList[21].value, RegList[22].value, RegList[23].value, RegList[24].value, RegList[25].value, RegList[26].value, RegList[27].value, RegList[28].value, RegList[29].value, RegList[29].value, RegList[30].value, RegList[31].value
        );

    }
    if (VHalt == 1){
        for (int i = 0; i<32; i++){
            fprintf(output, "\n%s: %d", MemList[i].hex_address, MemList[i].value);
        }
    }
}


int main(int argc, char* argv[]){
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    if(input == NULL){
        fprintf(stderr, "Error: could not open input file\n");
        exit(1);
    }

    if(output == NULL){
        fprintf(stderr, "Error: could not open output file '%s'\n", argv[2]);
        exit(1);
    }

    stimulator(input, output);
    return 0;
}