#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    {"0110011", "R"}, {"0100011", "S"}, {"1100011", "B"}, {"0110111", "U"},
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
    for (int i = 0; i<9; i++){
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
void R_decoder(char whole_inst[]){
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
        return;
    }
    //SUB
    if ((strcmp("000", funct3) == 0) && (strcmp("0100000", funct7) == 0)){
        rd->value = rs1->value - rs2->value;
        return;
    }
    //SLL
    if (strcmp("001", funct3) == 0){

        return;
    }
    //SLT
    if (strcmp("010", funct3) == 0){

        return;
    }
    //SLTU
    if (strcmp("011", funct3) == 0){

        return;
    }
    //XOR
    if (strcmp("100", funct3) == 0){

        return;
    }
    //SRL
    if (strcmp("101", funct3) == 0){

        return;
    }
    //OR
    if (strcmp("110", funct3) == 0){

        return;
    }
    //AND
    if (strcmp("111", funct3) == 0){

        return;
    }
}

void S_decoder(char whole_inst[]){
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
        return;
    }

    //Printing error if funct3 is not 010.
    printf("error");
    return;
}

void B_decoder(char whole_list[]){

}

void U_decoder(char whole_list[]){

}

void J_decoder(char whole_list[]){

}

void Lw_decoder(char whole_list[]){

}

void Addi_decoder(char whole_list[]){

}

void Sltui_decoder(char whole_list[]){

}

void Jalr_decoder(char whole_list[]){

}

void Master_decoder(char whole_inst[]){
    //Opcode extraction
    char given_opcode[8];
    strncpy(given_opcode, whole_inst + 25, 7);
    given_opcode[7] = '\0';

    //find which instruction it is
    char* inst_name = find_inst_from_opcode(given_opcode);

    if (strcmp(inst_name, "R") == 0){
        printf("%c", 'R');
        R_decoder(whole_inst);
        return;
    }

    if (strcmp(inst_name, "S") == 0){
        printf("%c", 'S');
        S_decoder(whole_inst);
        return;
    }

    if (strcmp(inst_name, "B") == 0){
        printf("%c", 'B');
        B_decoder(whole_inst);
        return;
    }

    if (strcmp(inst_name, "U") == 0){
        printf("%c", 'U');
        U_decoder(whole_inst);
        return;
    }

    if (strcmp(inst_name, "J") == 0){
        printf("%c", 'J');
        J_decoder(whole_inst);
        return;
    }

    if (strcmp(inst_name, "Lw") == 0){
        printf("Lw");
        Lw_decoder(whole_inst);
        return;
    }

    if (strcmp(inst_name, "Addi") == 0){
        printf("Addi");
        Addi_decoder(whole_inst);
        return;
    }

    if (strcmp(inst_name, "Sltiu") == 0){
        printf("Sltiu");
        Sltui_decoder(whole_inst);
        return;
    }

    if (strcmp(inst_name, "Jalr") == 0){
        printf("Jalr");
        Jalr_decoder(whole_inst);
        return;
    }

    return;
}

void stimulator(FILE* input, FILE* output){
    char whole_inst[100];
    int PC = 0; 
    int line_no = 1;
    int HALT = 0;

    while(fgets(whole_inst, 100, input) != NULL){   //Reads one line at a time. Do everything INSIDE THIS LOOP.
        whole_inst[strcspn(whole_inst, "\r\n")] = '\0';
        //SKIP EMPTY LINES
        if (strlen(whole_inst) == 0){
            continue;
        }
        Master_decoder(whole_inst);
        PC = PC + 4;
        line_no++;
    }
}


int main(int argc, char* argv[]){
    /*/FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    if(input == NULL){
        fprintf(stderr, "Error: could not open input file\n");
        exit(1);
    }

    if(output == NULL){
        fprintf(stderr, "Error: could not open output file '%s'\n", argv[2]);
        exit(1);
    }

    stimulator(input, output);/*/

}