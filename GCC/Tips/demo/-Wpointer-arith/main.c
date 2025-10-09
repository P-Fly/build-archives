// main.c
#include <stdio.h>
#include <stddef.h>

#define container_of(ptr, type, member) ({          \
    void *__mptr = (void *)(ptr);                   \
    ((type *)(__mptr - offsetof(type, member))); })

typedef struct
{
    int a;
    int b;
    int c;
} foo_t;

int main(void)
{
    foo_t foo;
    foo_t* foo_p;
    int* p = &foo.b;

    foo_p = container_of(p, foo_t, b);

    printf("foo_p = %p, foo = %p.\n", (void*)foo_p, (void*)&foo);

    return 0;
}
