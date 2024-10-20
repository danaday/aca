#define F_CPU 160000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600

void uart_init()	//설정
{
  UCSR0A |= 1 << U2X0;	//2배속 모드
  UBRR0 = F_CPU/(8*BAUD)-1;
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
}


void uart_tx(uint8_t data)	//송신
{
  	//송신버퍼가 비어있지 않으면 대기
  while (!(UCSR0A & (1 << UDRE0))) {}
  	//송신버퍼가 비어있음
  UDR0 =data; //송신버퍼에 데이터 전달(송신)
}

uint8_t uart_rx()	//수신
{
  //수신되지 않으면 대기
  while (!(UCSR0A & (1 << RXC0))) {} //그동인 수신됨?
  //수신됨
  return UDR0;	//수신 레지스터(데이터)를 읽어 리턴함
}

void uart_tx_string(char * str)	//송신
{
  while(*str)	//str이 가리키는 문자가 널이 아니면 반복
  {
    uart_tx(*str);	//해당문자 전송
    str++;	//다음문자
  }
}

int main()
{
  DDRB |= 1 << PB5;	//내장 LED
  uart_init();
  while(1)
  {
   	uint8_t c = uart_rx();	//수신된 데이터를
    if (c =='1')
    {
      PORTB |= 1<< PB5; //LED 켜기
    }
    else if(c=='0')
    {
      PORTB &=  ~(1 << PB5);
    }
  } 
}

int main_tx()
{
  DDRB |= 1 << PB5;	//내장 LED
  uart_init();
  while(1)
  {
   	uart_tx_string("HELLO!\n");
    _delay_ms(1000);
    
  }
  
}
