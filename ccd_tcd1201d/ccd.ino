/*
 * Code for linear CCD - TCD1201D
 */
#define SH      PORTB0 /* digital pin 8  */
#define PHI1    PORTB1 /* digital pin 9  */
#define PHI2    PORTB2 /* digital pin 10 */
#define RS      PORTB3 /* digital pin 11 */
#define BT      PORTB4 /* digital pin 12 */
#define OS      A0     /* analog pin A0  */

/* 1024 values * 10 bit analog value / 8 bits per byte */
int data_index;
int read_value;
byte ccd_data[1280];

void setup()
{
    Serial.begin(115200);

    pinMode(OS, INPUT);
    /* set digital pins as OUTPUT */
    DDRB = B00011111;
    PORTB = B00000000;

    ADCSRA &= ~(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void loop()
{
    readCCD();

    for (int i = 0; i < 1280; i++) {
        Serial.write(ccd_data[i]);
        /* reset the value as insert function expects zeroes */
        ccd_data[i] = 0;
    }
}

inline void readCCD()
{
    noInterrupts();
    data_index = 0;

    PORTB = (0 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    PORTB = (1 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    PORTB = (1 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (1 << SH);
    
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    
    /* Dummy outputs */
    for (int i = 0; i < 16; i++) {
        readLow();
        readHigh();
    }

    /* Data */
    for (int i = 0; i < 1024; i++) {
        readLow();
        readHigh();
        //ccd_data[data_index] = read_value;
        insert(read_value);
    }

    /* Dummy outputs */
    for (int i = 0; i < 7; i++) {
        readLow();
        readHigh();
    }

    interrupts();
}

inline void readLow() {
    PORTB = (0 << RS) | (0 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
    read_value = analogRead(OS);
    PORTB = (1 << RS) | (0 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
}

inline void readHigh() {
    PORTB = (0 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    read_value = analogRead(OS);
    PORTB = (1 << RS) | (0 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (1 << RS) | (1 << BT) | (0 << PHI2) | (1 << PHI1) | (0 << SH);
    PORTB = (0 << RS) | (1 << BT) | (1 << PHI2) | (0 << PHI1) | (0 << SH);
}

void insert(int value) {
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

int extract(int i) {
    int byte_index = 10 * i / 8;
    int byte_offset = 10 * i % 8;

    byte high = ccd_data[byte_index]     <<  byte_offset;
    byte low =  ccd_data[byte_index + 1] >> (6 - byte_offset);
    int value = (high << 2) | low;
    
    return value;
}
