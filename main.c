#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

int ticks;
volatile int sending = 0;
volatile int bit_id = 0;
volatile char to_send_byte;
char *text = "hello world\n";
volatile int idx = 0;
void begin();
void send();
int main(void) {
    DDRD |= (1 << PD1); 
    DDRB |= (1 << DDB5);
    int baud_rate = 9600;
    ticks = 207;
    begin();
    char *abcd = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int len = 40;
    int idx = 0;
    while (1) {
        send();
    }
}


ISR(TIMER1_COMPA_vect) {
    if (!sending)
        return;
    bit_id++;
    if (bit_id == 1) {
        PORTD &= ~(1 << PD1);
        return;
    }
    if (bit_id <= 9) {
        uint8_t bit = ((to_send_byte >> (bit_id - 2)) & 1);
        if (bit) {
            PORTD |= (1 << PD1);   // HIGH
        } else {
            PORTD &= ~(1 << PD1);  // LOW
        }
    } else if (bit_id == 10) {
        PORTD |= (1 << PD1); 
    } else {
        sending = 0;
        bit_id = 0;
        PORTB = PORTB & ~(1 << PORTB5);
    }
    TCNT1 = 0;
}



void send() {
    if (sending)
        return;
    if (idx > 11) {
        idx = 0;
        _delay_ms(1000);
    }
    to_send_byte = text[idx++];
    sending = 1;
    PORTB = PORTB | (1 << PORTB5);
}

void init_timer() {

    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11); // set prescaler
    TIMSK1 = (1 << OCIE1A); // enable overflow interupt
    OCR1A = 207;
    sei();
} 

void begin() {
    PORTD |= (1 << PD1);   // HIGH
    init_timer();
}