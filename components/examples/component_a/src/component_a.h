#ifndef COMPONENT_A_H
#define COMPONENT_A_H

#include "component_b.h"

extern int CheckReadSomeData();

extern void CheckWriteSomeData(int var);

extern int CheckReadSomeDataAndReturn();

extern int CheckReadMultipleDataAndReturn();

extern void CheckReadDataStructure(MyDataType *output);

#define MY_DATA_ARRAY_SIZE 3

extern void CheckReadDataStructureArray(MyDataType *output);

extern void CalculateSquare(void);

#endif /* COMPONENT_A_H */
