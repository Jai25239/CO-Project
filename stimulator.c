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

Opcode InsList[] = {
    {"0110011", "R"}, {"0100011", "S"}, {"1100011", "B"}, {"0110111", "U"},
    {"1101111", "J"},
    {"0000011", "Lw"}, {"0010011", "Addi"}, {"0010011", "Sltiu"}, {"1100111", "Jalr"}
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
    for (int i = 0; i<32; i++){
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