#include <stdio.h>
#include <stdlib.h>
/*
    Basic dynamic pointer list of items
*/
struct list
{
    int *value;
    int items;
    int size;
};

void addItems(struct list *inputList, int item);

int main()
{

    struct list myList;
    if (myList.value == NULL)
    {
        printf("Mem allocated fail");
        return 1;
    }
    myList.items = 0;
    myList.size = 8;
    myList.value = malloc(myList.size * sizeof(int));

    int num = 34; // <- items to add to struct, any number.
    for (int i = 0; i < num; i++)
    {
        addItems(&myList, i);
    }
    for (int i = 0; i < myList.items; i++)
    {
        printf("%d ",myList.value[i]);
    }
    free(myList.value);
    return 0;
}

void addItems(struct list *inputList, int item)
{
    for (int i = 0; i < inputList->items; i++)
    {
        printf("%d ", inputList->value[i]);
    }
    printf("\n ============================================= \n");
    if (inputList->items == inputList->size)
    {
        printf("Resize: %d \n", inputList->size);
        printf("Items: %d \n", inputList->items);
        printf("Bytes: %d \n", inputList->size * sizeof(int));

        inputList->size += 10;
        inputList->value = realloc(inputList->value, inputList->size * sizeof(int));
        printf("Bytes after resize: %d \n", inputList->size * sizeof(int));
    }
    inputList->value[inputList->items] = item;
    inputList->items++;
}