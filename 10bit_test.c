/*
 * Storing 10 bit integers in byte array
 * to reduce used memory(proof of concept)
 */
#include <stdio.h>

typedef unsigned char byte;

void insert(int);
int extract(int);
void print_bin(int, int);

int data_index;
byte ccd_data[1280] = { 2,79,147,236,249,62,207,147,228,251 };

int
main(void)
{
    data_index = 0;
    /* for(int i = 0; i < 20; i++) {
        insert(i);
    }*/
    
    for(int i = 0; i < 5; i++) {
        int v = extract(i);
        printf("%d\n", v);
    }

    printf("\n");
}

void
insert(int value)
{
    int byte_index = 10 * data_index / 8;
    int byte_offset = 10 * data_index % 8;

    ccd_data[byte_index]     |= (value & 0x3FC) >> (2 + byte_offset);
    ccd_data[byte_index + 1] |= (value & 0x0FF) << (6 - byte_offset);

    data_index++;
}

int
extract(int i)
{
    int byte_index = 10 * i / 8;
    int byte_offset = 10 * i % 8;

    byte high = ccd_data[byte_index]     << (2 + byte_offset);
    byte low =  ccd_data[byte_index + 1] >> (6 - byte_offset);
    int value = high | low;

    ccd_data[byte_index] = 0;
    //ccd_data[byte_index + 1] = 0;
    
    return value;
}

void
print_bin(int n, int count)
{
    int mask = 1 << (count - 1);
    for (int i = 0; i < count; i++) {
        if (n & mask)
            printf("1");
        else
            printf("0");

        mask >>= 1;
    }
}