#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//초음파 센서 관련 매크로
#define ECHO (PIND & (1 << PD2))
#define TRIGGER_HIGH() PORTD |= 1<<PD4
#define TRIGGER_LOW() PORTD &= ~(1<<PD4)

ISR(PCINT2_vect)
{
  if(ECHO)	//상승
  {  
    TCNT1 = 0;	//초기화
    
  }
  else		//하강
  {
    uint16_t t = TCNT1;
    uint16_t t1 = t;
  }
 
}

void init_port()
{
  
  DDRB |= 1 <<PB5;
  DDRD &=~(1<<PD2);
  //PORTD |= (1<<PD2);	//풀업
  
  //trigger
  
  DDRD |= 1<<PD4;	//출력핀
  
}


void init_timer1()
{
  TCCR1B |= 0b010 << CS10;	// 분주비 8 : 타이머 동작 
  // 1 => 0.5us
}

void init_pcint2()
{
  PCICR = 1<<PCIE2;
  PCMSK2 = 1 << PCINT18;	//PD2
  sei();
}

void start_measure()
{
  TRIGGER_HIGH();
  _delay_us(10);
  TRIGGER_LOW();
}

int main()
{
  init_port();
  init_pcint2();
  init_timer1();
  
  while(1){
    start_measure();
    _delay_ms(50);
  }
}