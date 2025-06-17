/**
 * @file hello_gmock.c
 */

#include "hello_gmock.h"

int CheckGetData(void)
{
    return GetData();
}

int CheckGetByPointer(void)
{
    int data = 0;
    GetByPointer(&data);
    return data;
}

int CheckGetByPointerAndReturnValue(int *data)
{
    return GetByPointerAndReturnValue(data);
}

void CheckGetDataStructureByPointer(MyDataType *data)
{
    GetDataStructureByPointer(data);
}

void CheckGetDataStructureArray(MyDataType *data)
{
    GetDataStructureArray(data);
}

void CheckSetData(int data)
{
    SetData(data);
}

void CheckSetDataByPointer(int data)
{
    SetDataByPointer(&data);
}
