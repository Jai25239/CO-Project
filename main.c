#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[10];    
    char funct7[8];   
    char funct3[4];   
    char opcode[8];   
} RTypeInstruction;

typedef struct {
    char name[3];
    char funct3[4];
    char opcode[8];
}STypeInstruction;

// For registers
typedef struct {
    char name[6];     
    char encoding[6]; 
} Register;


Register RegList[] = {
        {"zero", "00000"},
        {"ra", "00001"},
        {"sp", "00010"},
        {"gp", "00011"},
        {"tp", "00100"},
        {"t0", "00101"}, {"t1", "00110"}, {"t2", "00111"},
        {"s0", "01000"}, {"fp", "01000"},
        {"s1", "01001"},
        {"a0", "01010"}, {"a1", "01011"},
        {"a2", "01100"}, {"a3", "01101"}, {"a4", "01110"}, {"a5", "01111"}, {"a6", "10000"}, {"a7", "10001"},
        {"s2", "10010"}, {"s3", "10011"}, {"s4", "10100"}, {"s5", "10101"}, {"s6", "10110"},
        {"s7", "10111"}, {"s8", "11000"}, {"s9", "11001"}, {"s10", "11010"}, {"s11", "11011"},
        {"t3", "11100"}, {"t4", "11101"}, {"t5", "11110"}, {"t6", "11111"}
    };

    RTypeInstruction Rtype[] = {
        {"add", "0000000", "000", "0110011"},
        {"sub", "0100000", "000", "0110011"},
        {"sll", "0000000", "001", "0110011"},
        {"slt", "0000000", "010", "0110011"},
        {"sltu","0000000", "011", "0110011"},
        {"xor", "0000000", "100", "0110011"},
        {"srl", "0000000", "101", "0110011"},
        {"or",  "0000000", "110", "0110011"},
        {"and", "0000000", "111", "0110011"},
    };
    int Rsize = sizeof(Rtype)/(sizeof(RTypeInstruction));

    STypeInstruction Stype[] = {
        {"sw", "010", "0100011"}
    };
    int Ssize = sizeof(Stype)/sizeof(STypeInstruction);



char* imm_to_bin(int num, int bits, char* result){
    if (num <0){
        num = num + (1<<bits);
    }
    for (int i = bits-1; i>=0; i--){
        result[i] = (num%2) + '0';
        num = num/2;
    }
    result[bits] = '\0';
    return result;
}

Register* find_reg(char* name, Register* Reg_list){
    for (int i = 0; i<32; i++){
        if (strcmp(Reg_list[i].name, name) == 0){
            return &Reg_list[i];
        }
    }
    return NULL;
}

RTypeInstruction* find_Rinst(char* name, RTypeInstruction* Rlist, int size){
    for (int i = 0; i<size; i++){
        if (strcmp(Rlist[i].name, name) == 0){
            return &Rlist[i];
        }
    }
    return NULL;
}

STypeInstruction* find_Sinst(char* name, STypeInstruction* Slist, int size){
    for (int i = 0; i<size; i++){
        if (strcmp(Slist[i].name, name) == 0){
            return &Slist[i];
        }
    }
    return NULL;
}


//Master find instruction function
char find_inst(char* name,RTypeInstruction* Rlist, int Rsize, STypeInstruction* Slist, int Ssize){
    if (find_Rinst(name, Rlist, Rsize) != NULL){
        return 'R';
    }
    if (find_Sinst(name, Slist, Ssize) != NULL){
        return 'S';
    }
    return '?';
}


void encoder(FILE* input, FILE* output){
    char line[100];

    while(fgets(line, 100, input) != NULL){
        line[strcspn(line, "\r\n")] = '\0';

        //SKIP EMPTY LINES
        if (strlen(line) == 0){
            continue;
        }

        //Parsing, ie splitting the line into tokens.
        char* tokens[10];
        int count = 0;

        char* elements = strtok(line, " ,()");
        while (elements != NULL){
            tokens[count] = elements;
            count++;
            elements = strtok(NULL, " ,()");
        }

        //RTYPE INSTRUNCTION ENCODING
        if (find_inst(tokens[0], Rtype, Rsize, Stype, Ssize) == 'R') {
            RTypeInstruction* Instruct = find_Rinst(tokens[0], Rtype, Rsize);
            Register* rd = find_reg(tokens[1], RegList);
            Register* r1 = find_reg(tokens[2], RegList);
            Register* r2 = find_reg(tokens[3], RegList);
            
            if (Instruct == NULL || rd == NULL || r1 == NULL || r2 == NULL){
                printf("Incorrect instruction format.\nFor Rtype intruction, the format is: Instruction Name rd, r1, r2");
                return;
            } else {
                fprintf(output, "%s%s%s%s%s%s\n", Instruct->funct7, r2->encoding, r1->encoding, Instruct->funct3, rd->encoding, Instruct->opcode);
            }
        }

        //STYPE INSTRUNCTION ENCODING
        else if (find_inst(tokens[0], Rtype, Rsize, Stype, Ssize) == 'S'){
            STypeInstruction* Instruct = find_Sinst(tokens[0], Stype, Ssize);
            Register* rs2 = find_reg(tokens[1], RegList);
            Register* rs1 = find_reg(tokens[3], RegList);
            char imm[12];
            imm_to_bin(atoi(tokens[2]), 12, imm);
            //UPPER IMM
            char upper[8];
            strncpy(upper, imm, 7);
            upper[7] = '\0';

            //LOWER IMM
            char lower[6];
            strncpy(lower, imm+7, 5);
            lower[5] = '\0';

            if (Instruct == NULL || rs2 == NULL || rs1 == NULL){
                printf("Incorrect instruction format.\nFor Stype intruction, the format is: Instruction Name rs2, imm(rs1)");
                return;
            } else{
                fprintf(output, "%s%s%s%s%s%s\n", upper, rs2->encoding, rs1->encoding, Instruct->funct3, lower, Instruct->opcode);
            }
        } 
        
        else {
            printf("ERROR!");
        }
    }
}

int main(int argc, char* argv[]){
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    encoder(input, output);
    return 0;
}