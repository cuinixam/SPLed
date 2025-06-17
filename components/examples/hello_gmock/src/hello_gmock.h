#ifndef HELLO_GMOCK_H
#define HELLO_GMOCK_H

extern int GetData(void);
extern int CheckGetData(void);

extern void GetByPointer(int *data);
extern int CheckGetByPointer(void);

extern int GetByPointerAndReturnValue(int *data);
extern int CheckGetByPointerAndReturnValue(int *data);

extern void GetByPointer(int *data);
extern int CheckGetByPointer(void);

typedef struct
{
    int a;
    unsigned char b;
} MyDataType;

extern void GetDataStructureByPointer(MyDataType *data);
extern void CheckGetDataStructureByPointer(MyDataType *data);

extern void GetDataStructureArray(MyDataType *data);
extern void CheckGetDataStructureArray(MyDataType *data);

extern void SetData(int data);
extern void CheckSetData(int data);

extern void SetDataByPointer(int *data);
extern void CheckSetDataByPointer(int data);

#endif /* HELLO_GMOCK_H */
