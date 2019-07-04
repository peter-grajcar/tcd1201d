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
byte ccd_data[80] = { 2,79,147,236,249,62,207,147,228,251 };

int
main(void)
{
    data_index = 0;
    for(int i = 0; i < 10; i++)
        insert(i*255);
    
    for(int i = 0; i < 5; i++)
        printf("%d ", extract(i));

    printf("\n");
}

void
insert(int value)
{
    int byte_index = 10 * data_index / 8;
    int byte_offset = 10 * data_index % 8;

    /* 8 high bits */
    byte high = (value & 0x3FC) >> 2;
    /* 8 low bits */
    byte low = value & 0x0FF; 

    ccd_data[byte_index]     |= high >>  byte_offset;
    ccd_data[byte_index + 1] |= low  << (6 - byte_offset);

    data_index++;
}

int
extract(int i)
{
    int byte_index = 10 * i / 8;
    int byte_offset = 10 * i % 8;

    byte high = ccd_data[byte_index]     <<  byte_offset;
    byte low =  ccd_data[byte_index + 1] >> (6 - byte_offset);
    int value = (high << 2) | low;
    
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