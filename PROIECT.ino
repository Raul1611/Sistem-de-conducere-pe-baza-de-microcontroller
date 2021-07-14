#define val_aprindere (35)
#define HISTEREZIS (0.5)
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

unsigned short int sec, sec2;
unsigned short int i,j;
/*
void timer0_PWM_init();
void timer1_init();
void adc_init();
uint16_t read_adc(uint8_t channel);
void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
void ledPWM();
void seven_seg();
void temp();
void led_com_seriala();
*/

int main(void){
  
  DDRB |= (1<<PB5) | (1<<PB0) | (1<<PB1) | (1<<PB2);//pini 7seg
  DDRB |= (1<<PB3) | (1<<PB4);//pin led temperatura, pin led comunicatie seriala
  DDRD |= (1<<PD6); //pin pwm
  DDRD |= (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD5) | (1<<PD7); //pini 7seg
  DDRC &= ~(1<<PC5); //pin intrare pentru lm35

  timer1_init();
  timer0_PWM_init();
  USART_Init(MYUBRR);
  sei();//activare intreruperi
  
  while(1)
  { 
    ledPWM();
    seven_seg();
    led_com_seriala();
  }
}

void timer1_init()
{
  //timer1
  TCCR1A = 0; 
  TCCR1B |= (1<< CS10) | (1<<CS12) | (1<<WGM12); //prescale /1024, CTC
  OCR1A = 15625;// output compare value
  TIMSK1 = 1<<OCIE1A; //compare intrerrupt
}

void timer0_PWM_init()
{
  //timer0
  TCCR0A |= (1<<COM0A1);//Clear OC0A on compare match, set OC0A at BOTTOM,(non-inverting mode).
  TCCR0A |= (1<<WGM00) | (1<<WGM01); //fast PWM
  TCCR0B |= (1<<CS00) | (1<<CS02); //prescale /1024
}

ISR(TIMER1_COMPA_vect){ // timer 1 intrerupt (output compare match A)
  PORTB ^= (1<<PB5); //toggle Port B, pin 13
  sec++; sec2++;
  if(sec==4) sec=0;
  if(sec2==7) sec2=1;

  temp();
}

void adc_init()//adc initialization
{
 ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));//set division factor between system clock frequency and the input clock to the ADC- 128
 ADMUX |= (1<<REFS0);//AVcc with external capacitor at Aref pin 
 ADCSRA |= (1<<ADEN);//enable ADC
 ADCSRA |= (1<<ADSC);//ADC start conversion
}

uint16_t read_adc(uint8_t channel)
{
 ADMUX &= 0xFF;//set input AO to A8
 ADMUX |= channel;//select chanel AO to A8
 ADCSRA |= (1<<ADSC);//start conversion
 if(!(ADCSRA & (1<<ADSC)));//wait while adc conversion are not updated 
 return ADCW;//read and return voltage
}

void USART_Init(unsigned int ubrr)
{
/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/*Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
 while(!(UCSR0A & (1<<UDRE0)));
 {
 /* Put data into buffer, sends the data */
 UDR0 = data;
 }
}

void ledPWM()
{
    if(sec==0)
    for(i=0; i<256; i++)
    {
      OCR0A=i; 
      _delay_ms(30);
      if(sec==1) break;
    }
    if(sec==1) OCR0A=i;
    if(sec==2)
    for(j=i; j>0; j--)
    {
       OCR0A=j; 
       _delay_ms(30);
       if(sec==3) break;
    }
    if(sec==3) OCR0A=0;
}

void seven_seg()
{ 
     delay(5);
     switch(sec2)
  {
     case 1: //R (Raul) - pozitionarea segmentelor nu permite afisarea literei R, asa ca se afiseaza litera A, fiindca se aseamana.
            PORTD |= (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD7);
            PORTB |= (1<<PB0) | (1<<PB1) | (1<<PB2);
            break;

    case 2: PORTD &= ~(1<<PD2) & ~(1<<PD3) & ~(1<<PD4) & ~(1<<PD7);
            PORTB &= ~(1<<PB0) & ~(1<<PB1) & ~(1<<PB2);
            break;

    case 3: // P(Petrisor)
            PORTD |= (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD7);
            PORTB |= (1<<PB0) | (1<<PB2);
            break;

    case 4: PORTD &= ~(1<<PD2) & ~(1<<PD3) & ~(1<<PD4) & ~(1<<PD7);
            PORTB &= ~(1<<PB0) & ~(1<<PB2);
            break;

    case 5: //D (Dumitrache) - pozitionarea segmentelor nu permite afisarea literei D, asa ca se afiseaza litera d
            PORTD |= (1<<PD2) | (1<<PD4) | (1<<PD7) | (1<<PD5);
            PORTB |= (1<<PB1) | (1<<PB2);
            break;

   case 6: PORTD &= ~(1<<PD2) & ~(1<<PD4) & ~(1<<PD7) & ~(1<<PD5);
           PORTB &= ~(1<<PB1) & ~(1<<PB2);
           break;

   default: PORTD &= ~(1<<PD2) & ~(1<<PD4) & ~(1<<PD7) & ~(1<<PD5) & ~(1<<PD3);
            PORTB &= ~(1<<PB1) & ~(1<<PB2) & ~(1<<PB0);
  }
}

void temp()
{
 adc_init ();

 unsigned int numar_CAN=read_adc(5);
 float milivolt=(numar_CAN*5)/1023.0;
 //float volt=milivolt*1000;
 //float temp=volt/10; //10 mv --> 1 grad C
 float temp = milivolt/0.01;

 short int int_temp = (short int)(100*temp);

 //se aprinde ledul cand temp trece de 35 de grade, cu histerezis de 0.5 grade
 if(temp > (val_aprindere + HISTEREZIS))
   PORTB |= (1<<PB3);
 if(temp < (val_aprindere - HISTEREZIS))
   PORTB &= ~(1<<PB3);
 
   char buff[100];
   memset(buff, 0, sizeof(buff));
   sprintf(buff, "Temperatura: %hd.%hd°C\n", int_temp/100, int_temp%100);

   for(int i=0; i<strlen(buff); i++)
      USART_Transmit(buff[i]);
 
}

void led_com_seriala()
{
  if(UDR0==65)//A
    PORTB |= (1<<PB4);
  if(UDR0==83)//S
    PORTB &= ~(1<<PB4);
}
