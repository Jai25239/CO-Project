#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Defined seperate structs to store the instruction, register and their properties, encoding in a structured way.
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

typedef struct{
    char name[6];
    char funct3[4];
    char opcode[8];
}ITypeInstruction;

typedef struct{
    char name[6];
    char opcode[8];
}UTypeInstruction;

typedef struct{
    char name[6];
    char opcode[8];
}JTypeInstruction;

typedef struct{
    char name[6];
    char funct3[4];
    char opcode[8];
}BTypeInstruction;

// For registers
typedef struct {
    char name[6];     
    char encoding[6]; 
}Register;

//Storing the register and their encoding, in the same order we created its properties, for example, registers mein pehle "name", fir "encoding"
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

    //"name", "funct7", "funct3", "opcode"
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
    int Rsize = sizeof(Rtype)/(sizeof(RTypeInstruction));    //Number of R type instrunctions 

STypeInstruction Stype[] = {
        {"sw", "010", "0100011"}
    };
    int Ssize = sizeof(Stype)/sizeof(STypeInstruction);      //Number of S type instructions

    //"name","funct3","opcode"
ITypeInstruction Itype[]={
        {"lw","010","0000011"},
        {"addi","000","0010011"},
        {"sltiu","011","0010011"},
        {"jalr","000","1100111"}
    };
    int Isize = sizeof(Itype)/sizeof(ITypeInstruction);     //Number of I type instructions.

    //"name","opcode"
UTypeInstruction Utype[]={
        {"auipc","0010111"},
        {"lui","0110111"}
    };
    int Usize = sizeof(Utype)/sizeof(UTypeInstruction);     //Number of U type instructions.

    //"name","opcode"
JTypeInstruction Jtype[]={
        {"jal","1101111"}
    };
    int Jsize = sizeof(Jtype)/sizeof(JTypeInstruction);

BTypeInstruction Btype[] = {
        {"beq","000","1100011"},
        {"bne","001","1100011"},
        {"blt","100","1100011"},
        {"bge","101","1100011"},
        {"bltu","110","1100011"},
        {"bgeu","111","1100011"}
    };
    int Bsize = sizeof(Btype)/sizeof(BTypeInstruction);

//immediate will be in decimal, so this function converts decimal to binary (also do 2's complement for negetive decimals)
char* imm_to_bin(int num, int bits, char* result){  //num: What you want to convert. bits:Into how many bits. result: Where you want to store it.
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

//Finds the register with name "name" and returns that address where that register and its encoding is stored, otherwise gives NULL.
Register* find_reg(char* name, Register* Reg_list){
    for (int i = 0; i<33; i++){
        if (strcmp(Reg_list[i].name, name) == 0){
            return &Reg_list[i];
        }
    }
    return NULL;
}

//Finds the Rtype Instruction and returns that address where that instruction is stored, otherwise gives NULL.
RTypeInstruction* find_Rinst(char* name, RTypeInstruction* Rlist, int size){
    for (int i = 0; i<size; i++){
        if (strcmp(Rlist[i].name, name) == 0){
            return &Rlist[i];
        }
    }
    return NULL;
}

//Finds the Stype Instruction and returns that address where that instruction is stored, otherwise gives NULL.
STypeInstruction* find_Sinst(char* name, STypeInstruction* Slist, int size){
    for (int i = 0; i<size; i++){
        if (strcmp(Slist[i].name, name) == 0){
            return &Slist[i];
        }
    }
    return NULL;
}

//Finds the Itype Instruction and returns that address where that instruction is stored, otherwise gives NULL.
ITypeInstruction* find_Iinst(char* name, ITypeInstruction* Ilist,int size){
    for(int i = 0; i<size; i++){
        if(strcmp(Ilist[i].name,name)==0){
            return &Ilist[i];
        }
    }
    return NULL;
}
//Finds the Utype Instruction and returns that address where that instruction is stored, otherwise gives NULL.
UTypeInstruction* find_Uinst(char* name, UTypeInstruction* Ulist, int size){
    for(int i = 0; i< size; i++){
        if(strcmp(Ulist[i].name,name)==0){
            return &Ulist[i];
        }
    }
    return NULL;
}
//Finds the Jtype Instruction and returns that address where that instruction is stored, otherwise gives NULL.
JTypeInstruction* find_Jinst(char* name, JTypeInstruction* Jlist, int size){
    for(int i = 0; i<size; i++){
        if(strcmp(Jlist[i].name,name)==0){
            return &Jlist[i];
        }
    }
    return NULL;
}
//Finds the Btype Instruction and returns that address where that instruction is stored, otherwise gives NULL.
BTypeInstruction* find_Binst(char* name, BTypeInstruction* Blist, int size){
    for(int i=0;i<size;i++){
        if(strcmp(Blist[i].name,name)==0){
            return &Blist[i];
        }
    }
    return NULL;
}

//Master FIND INSTRUCTION function (find the type of instruction.)
char find_inst(char* name,RTypeInstruction* Rlist, int Rsize, STypeInstruction* Slist, int Ssize,ITypeInstruction* Ilist,int Isize, UTypeInstruction* Ulist, int Usize, JTypeInstruction* Jlist, int Jsize,BTypeInstruction* Blist, int Bsize){
    if (find_Rinst(name, Rlist, Rsize) != NULL){
        return 'R';
    }
    if (find_Sinst(name, Slist, Ssize) != NULL){
        return 'S';
    }
    if(find_Iinst(name,Ilist,Isize) != NULL){
        return 'I';
    }
    if(find_Uinst(name,Ulist,Usize) != NULL){
        return 'U';
    }
    if(find_Jinst(name,Jlist,Jsize) != NULL){
        return 'J';
    }
    if(find_Binst(name,Btype,Bsize) != NULL){
    return 'B';
    }
    return '?';
}

//This is the responsible function which breaks assembly written line into tokens one by one taking lines from the file.
//After breaking into tokens we find the encoding of opcode, funct3 or funct7 from the previous arrays and prints them into the sequence, which is encoding of the given assembly code. 
void encoder(FILE* input, FILE* output){
    char line[100];

    while(fgets(line, 100, input) != NULL){   //Reads one line at a time. Do everything INSIDE THIS LOOP, AFTER PARSING!
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

        //Example to understand what happened above: Given line = add t0, zero, t0
        //Tokens will become: tokens = {"add", "t0", "zero", "t0"}, very easy to handle now.
        //Another example: Given line = sw t0, 10(t1)
        //tokens = {"sw", "t0", "10", "t1"}

        //RTYPE INSTRUNCTION ENCODING
        if (find_inst(tokens[0], Rtype, Rsize, Stype, Ssize, Itype, Isize, Utype, Usize, Jtype, Jsize,Btype,Bsize) == 'R') {
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
        else if (find_inst(tokens[0], Rtype, Rsize, Stype, Ssize, Itype, Isize, Utype, Usize, Jtype, Jsize,Btype,Bsize) == 'S'){
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

        //I Type Instruction Encoding.
        else if(find_inst(tokens[0],Rtype,Rsize,Stype,Ssize,Itype,Isize,Utype,Usize, Jtype, Jsize,Btype,Bsize)=='I'){
            ITypeInstruction* Instruct = find_Iinst(tokens[0],Itype,Isize);
            Register* rd = find_reg(tokens[1],RegList);
            Register* rs1 = NULL;
            char imm[13];
            if(strcmp(Instruct->name,"lw")==0){
                rs1 = find_reg(tokens[3],RegList);
                imm_to_bin(atoi(tokens[2]),12,imm);
            }
            else{
                rs1 = find_reg(tokens[2],RegList);
                imm_to_bin(atoi(tokens[3]),12,imm);
            }
            if(rd == NULL || rs1 == NULL){
                printf("register name not found\n");
                return;
            }
            else{
                fprintf(output, "%s%s%s%s%s\n",imm, rs1->encoding, Instruct->funct3, rd->encoding, Instruct->opcode);
            }
        }

        //U Type Instruction Encoding.
        else if(find_inst(tokens[0],Rtype,Rsize,Stype,Ssize,Itype,Isize,Utype,Usize, Jtype, Jsize,Btype,Bsize)=='U'){
            UTypeInstruction* Instruct = find_Uinst(tokens[0],Utype,Usize);
            Register* rd = find_reg(tokens[1],RegList);
            char imm[21];
            imm_to_bin(atoi(tokens[2]),20,imm);
            if(rd == NULL){
                printf("register not found\n");
                return;
            }
            else{
                fprintf(output,"%s%s%s\n",imm,rd->encoding,Instruct->opcode);
            }
        }
        
       //J Type Instruction Encoding
       else if(find_inst(tokens[0],Rtype,Rsize,Stype,Ssize,Itype,Isize,Utype,Usize,Jtype,Jsize,Btype,Bsize)=='J'){
           JTypeInstruction* Instruct = find_Jinst(tokens[0],Jtype,Jsize);
           Register* rd = find_reg(tokens[1],RegList);
           char imm[21];
           imm_to_bin(atoi(tokens[2]),20,imm);
           if(rd == NULL){
               printf("register not found\n");
               return;
           }
           else{
               fprintf(output,"%s%s%s\n",imm,rd->encoding,Instruct->opcode);
           }
       }
      //U Type Instruction Encoding.
      else if(find_inst(tokens[0],Rtype,Rsize,Stype,Ssize,Itype,Isize,Utype,Usize,Jtype,Jsize,Btype,Bsize)=='B'){

    BTypeInstruction* Instruct = find_Binst(tokens[0],Btype,Bsize);

    Register* rs1 = find_reg(tokens[1],RegList);
    Register* rs2 = find_reg(tokens[2],RegList);

    int offset = atoi(tokens[3]);

    if(offset % 2 != 0){
        printf("Branch offset must be 2-byte aligned\n");
        return;
    }

    offset = offset >> 1;

    char imm[13];
    imm_to_bin(offset,12,imm);

    char bit12 = imm[0];
    char bit11 = imm[1];

    char bit10_5[7];
    strncpy(bit10_5,imm+2,6);
    bit10_5[6] = '\0';

    char bit4_1[5];
    strncpy(bit4_1,imm+8,4);
    bit4_1[4] = '\0';

    fprintf(output,"%c%s%s%s%s%s%c%s\n",
        bit12,
        bit10_5,
        rs2->encoding,
        rs1->encoding,
        Instruct->funct3,
        bit4_1,
        bit11,
        Instruct->opcode
    );
}
       else { 
           printf("ERROR!");
       }
    }
}
  

//This is the main function which takes assembly file and address of output file in which binary encoding has to be written.
int main(int argc, char* argv[]){        //for accessing input and output files in the command termial. First compile, then run .\main.exe input.txt output.txt 
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    encoder(input, output);
    return 0;
}
