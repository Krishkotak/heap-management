#include <stdio.h>
#include <stddef.h>

#define MAX_SIZE 2048

typedef struct bltag
{
    size_t size;
    struct bltag *next;
    struct bltag *prev;
} block;

char memory[MAX_SIZE];
block *freelist;

void init()
{
    freelist = (block *)(memory);
    freelist->size = MAX_SIZE - sizeof(block);
    freelist->next = freelist->prev = NULL;
}

void *mem_alloc(size_t req_size)
{
    if (req_size == 0 || freelist == NULL)
        return NULL;

    block *ptr = freelist;

    while (ptr != NULL && ptr->size < req_size)
    {
        ptr = ptr->next;
    }

    if (ptr == NULL)
    {
        return NULL; // No suitable block found
    }

    if (ptr->size >= req_size)
    {
        if (ptr->size > req_size + sizeof(block))
        {
            block *newblock = (block *)((char *)ptr + sizeof(block) + req_size);
            newblock->size = ptr->size - req_size - sizeof(block);
            newblock->prev = ptr;
            newblock->next = ptr->next;

            if (ptr->next != NULL)
            {
                ptr->next->prev = newblock;
            }

            ptr->size = req_size;
            ptr->next = newblock;
        }

        if (ptr->prev == NULL)
        {
            freelist = ptr->next;
        }
        else
        {
            ptr->prev->next = ptr->next;
        }

        return (void *)ptr + sizeof(block);
    }
    else
    {
        return NULL; // No suitable block found
    }
}

void mem_free(void *ptr)
{
    block *newblock = (block *)(ptr - sizeof(block));

    // printf("(void*)ptr=%zu\n\n", sizeof((void *)ptr));
    // printf("sizeof(block)=%zu\n\n", sizeof(block));
    // printf("newblock->size=%zu\n\n", newblock->size);

    newblock->next = freelist;
    newblock->prev = NULL;
    freelist = newblock;
    ptr = NULL;

    if (newblock->next != NULL && (char *)newblock + newblock->size + sizeof(block) == (char *)newblock->next)
    {
        newblock->size += (newblock->next)->size + sizeof(block);
        newblock->next = (newblock->next)->next;
        if (newblock->next != NULL)
            (newblock->next)->prev = newblock;
    }
}


void display()
{
    printf("currently free blocks:\n");
    block *ptr = freelist;
    printf("address\t\t\t size\n");
    while (ptr != NULL)
    {
        printf("%p\t\t%zu\n", (void *)ptr, ptr->size);
        ptr = ptr->next;
    }
}

int main()
{
    init();
    display();
    int *arr = (int *)mem_alloc(200 * sizeof(int));
    char *srr=(char*)mem_alloc(200*sizeof(char));
    //float *t=(float*)mem_alloc(200*5);
    printf("Allocated memory at address: %p\n", (void *)arr);
    printf("Allocated memory at address: %p\n", (void *)srr);
   // printf("Allocated memory at address: %p\n", (void *)t);


    mem_free(arr);
    mem_free(srr);
    //mem_free(t);
    display();
    return 0;
}
