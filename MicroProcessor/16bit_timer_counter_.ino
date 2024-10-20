#define F_CPU 160000000UL
#include <avr/io.h>

void timer1_init()
{
  //분주비 설정 : 1024(타/카 동작시작)
  TCCR1B |= 0b101<<CS10;
  OCR1A = 0x500;			//비교대상
  TCCR1A |= 1<<COM1A0;		//토글 파형 출력
  TCCR1B |= 1<<WGM12;		//CTC : 일치시 클리어

}

int main()
{
  DDRB |= 1 << PB1;	//내장 LED
  timer1_init();
  
  while(1) {}
  
}