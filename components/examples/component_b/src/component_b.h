#ifndef COMPONENT_B_H
#define COMPONENT_B_H

extern void Read_MyInput(int *var);

extern void Write_MyInput(int *var);

extern void ReadSomeData(int *var);

extern unsigned char ReadSomeDataAndReturn(int *var);

extern unsigned char ReadMultipleDataAndReturn(int *var1, int *var2);

typedef struct
{
    int a;
    unsigned char b;
} MyDataType;

extern void ReadDataStructure(MyDataType *var);

extern void ReadDataStructureArray(MyDataType *var);

#endif /* COMPONENT_B_H */
