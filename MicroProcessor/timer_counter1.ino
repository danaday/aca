#define F_CPU 160000000UL
#include <avr/io.h>
#include<avr/interrupt.h>

uint8_t count;
ISR(TIMER0_OVF_vect)	//1/64초에 한번씩
{
  if(++count >= 32)
  {
    count = 0;
 	PORTB ^= 1 << PB5;
  }
}

void timer0_init()
{
  //분주비 설정 : 1024(타/카 동작시작)
  TCCR0B |= 0b101<<CS00;
  TIMSK0 |= 1<<TOIE0;	//오버플로우 인터럽트 활성화
  sei();	//전역인터럽트 활성화 
}

int main()
{
  DDRB |= 1 << PB5;	//내장 LED
  timer0_init();
  
  while(1)
  {
  }
  
}