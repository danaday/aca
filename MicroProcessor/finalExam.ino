#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// PORTB (PB3..0) : 7SEG DATA1 (0:off / 1:on)
// PORTD(PD7..4) : 7SEG DATA   (0:off / 1:on)
// PORTC(PC3..0) : 7SEG SELECT (1:off / 0:on)

//7세그 출력 관련 매크로
#define SET_7SEG_DIGIT(v)    PORTD = pattern[v]
#define SET_7SEG_DIGIT1(v)   PORTB = pattern1[v]
#define SELECT_7SEG(v)       PORTC &= ~(1<<v)
#define CLEAR_7SEG()         PORTC |= 0x0f
#define SHOW_DOT()           PORTD |= (1<<4)
#define BLANK                (10)
#define LOADING_7SEG_DIGIT(v)	PORTD = loading_pattern[v]
#define LOADING_7SEG_DIGIT1(v)	PORTB = loading_pattern1[v]
#define LOADING_BLANK		(7)


// 초음파 센서 관련 매크로
#define ECHO (PINC & (1<< PC5))

#define TRIGGER_HIGH()       PORTC |= (1<<PC4)
#define TRIGGER_LOW()        PORTC &= ~(1<<PC4)

//#define soundSPEED 343.0 // SPEED of sound in m/s

//로딩 7세그먼트 출력 관련
const uint8_t loading_pattern1[]=
{
  //0000abcd
  0b00000000, //0 0
  0b00001000, //a 1
  0b00000100, //b 2
  0b00000010, //c 3
  0b00000001, //d 4
  0b00000000, //e 5
  0b00000000, //f 6
  0b00000000, // BLANK
};

const uint8_t loading_pattern[]=
{
//efgp0000
0b00000000, //0 0
0b00000000, //a 1
0b00000000, //b 2
0b00000000, //c 3
0b00000000, //d 4
0b10000000, //e 5
0b01000000, //f 6
0b00000000, // BLANK
};

const int loading_count[] =
{
  1000,	//0
  100,	//1
  10,	//2
  1,	//3
  2,	//4
  3,	//5
  4,	//6
  40,	//7
  400,	//8
  4000,	//9
  5000,	//10
  6000,	//11
  
};

//7세그먼트 출력 관련
const uint8_t pattern1[] = {
    //0000abcd
    0b00001111, // 0
    0b00000110, // 1
    0b00001101, // 2
    0b00001111, // 3
    0b00000110, // 4
    0b00001011, // 5
    0b00001011, // 6
    0b00001110, // 7
    0b00001111, // 8
    0b00001111, // 9
    0b00000000, // BLANK
};

const uint8_t pattern[] = {
    //efgp0000
    0b11000000, // 0
    0b00000000, // 1
    0b10100000, // 2
    0b00100000, // 3
    0b01100000, // 4
    0b01100000, // 5
    0b11100000, // 6
    0b01000000, // 7
    0b11100000, // 8
    0b01100000, // 9
    0b00000000, // BLANK
};


//포트설정

void init_port() {
    DDRB = 0X0f; // 7SEG DATA1 (OUTPUT)
    PORTB = 0X00; // ALL OFF1

    DDRD = 0xf0; // 7SEG DATA (OUTPUT)
    PORTD = 0x00; // ALL OFF

    DDRC = 0x0f; // 7SEG SELECT (OUTPUT)
    CLEAR_7SEG(); // ALL OFF
}

//7세그 출력 설정
uint8_t digits[] = {0, 0, 0, 0};

void set_7seg_value(uint16_t v) {
    digits[0] = v % 10000 / 1000;
    digits[1] = v % 1000 / 100;
    digits[2] = v % 100 / 10;
    digits[3] = v % 10;

    for(uint8_t i=0; i<3; i++) {
        if(digits[i] == 0)
            digits[i] = BLANK;
        else
            break;
    }
}

void loading_7seg_value(int v)
{
	digits[0] = v % 10000 / 1000;
	digits[1] = v % 1000 / 100;
	digits[2] = v % 100 / 10;
	digits[3] = v % 10;

	for(uint8_t i=0;i<3;i++)
	{
		if(digits[i] == 0)
			digits[i] = LOADING_BLANK;
		else
			break;
	}
}


uint8_t index_7seg = 0;	
uint8_t dot_position = 2; //2번 세그먼트에 dp 활성화


void disp_7seg_loading() {
    CLEAR_7SEG(); // 전부 끄기

    LOADING_7SEG_DIGIT(digits[index_7seg]); // 데이터 주기
    LOADING_7SEG_DIGIT1(digits[index_7seg]);

 

    SELECT_7SEG(index_7seg); // 켜기
    index_7seg = (index_7seg + 1) % 4; // 인덱스 증가
}

uint8_t loading_cnt = 0;
uint16_t loading_time_cnt = 0;

void loading()	//돌아가는 패턴
{   
  loading_time_cnt++;
  
  if (loading_time_cnt >= 100) // 패턴 바뀌는 시간
  {
    loading_time_cnt=0;
  
    
   		loading_7seg_value(loading_count[loading_cnt]);
    
    	loading_cnt++;
    	
    	if (loading_cnt >= 12)
          loading_cnt=0;
  }
  
}

void disp_7seg() {
    CLEAR_7SEG(); // 전부 끄기

    SET_7SEG_DIGIT(digits[index_7seg]); // 데이터 주기
    SET_7SEG_DIGIT1(digits[index_7seg]);

    // 점 찍기
    if(index_7seg == dot_position)
        SHOW_DOT();

    SELECT_7SEG(index_7seg); // 켜기
    index_7seg = (index_7seg + 1) % 4; // 인덱스 증가
}

//초음파/PWM 관련 설정
volatile uint16_t distance = 0; // 측정 시간->거리
volatile uint8_t rising_edge = 0;
volatile uint16_t startTIME = 0;
volatile uint16_t endTIME = 0;

uint16_t prev_distance = 0;
uint16_t distance_cnt = 0;
uint16_t top = 0;
uint16_t height = 0;

ISR(PCINT1_vect) // 인터럽트 수정
{
    if (ECHO) // 상승 에지
    {
        if (!rising_edge)
        {
            TCNT1 = 0; // 타이머 초기화
            rising_edge = 1;
        }
    }
    else if (rising_edge) // 하강 에지
    {
        endTIME = TCNT1;
        rising_edge = 0;

        distance = 0.363*(endTIME/4); // 거리 계산해주기
        //set_7seg_value(distance); // 7세그에 값 넣기
      
      if (top==0)	//전체 거리 계산
      {
        if ((distance>=2300)&&(prev_distance == distance))
        {
          distance_cnt++;
          if (distance_cnt>=50)
          {
            top=distance;	//천장-바닥 사이 거리
            
            piezo_yun();	//피에조 카운트 설정
            distance_cnt = 0;	//측정 시간 초기화
          }
        }
        else
        {
          distance_cnt=0;
        }
        prev_distance = distance;	//비교를 위한 변수 prev_distance에 값 저장 
      }
      
      else
      {
        uint16_t prev_height = height;
        if(distance >= top)
        {
          height=0;	//높이 초기화
          return;
        }
        
        height = top-distance;	//키
        
        
        
        if(height>=600)
        {
          if(prev_height==0)
            piezo_yun();			//피에조 카운트 설정
          set_7seg_value(height);	//키 7세그에 출력
        }
        else 
          height = 0;
      }
      
    }
  	
}

void init_port1() {		//초음파 포트설정
    DDRB |= 1<<PB5;
    DDRC &= ~(1<<PC5); // 입력핀

    // trigger  
    DDRC |= 1<<PC4;  // 출력핀
}

void init_timer2() {
    TCCR1A = 0; // Clear Timer1 settings
    TCCR1B |= (1<<CS11); // Prescaler 8 : 타이머 동작 (1 tick = 0.5us)
}

void init_pcint1() {
    PCICR = 1<<PCIE1; // PCINT1 인터럽트 활성화
    PCMSK1 = 1<< PCINT13; // PC5 핀의 핀 변화 인터럽트 허용
    sei(); // 전역적으로 인터럽트 허용
}

void start_measure() {	//측정
    TRIGGER_HIGH();
    _delay_us(10);
    TRIGGER_LOW();
}

// 피에조 관련

void piezo() //피에조 설정
{
 	TCCR2B |= (1<<CS12); // 분주비 256로 설정
  	OCR2B=0x20; // 비교일치 기준값
  
  	//비교일치 인터럽트 발생 시 OC2B 핀의 출력을 반전
  	TCCR2A |= (1<<COM2B0);
  	TCCR2B |= (1<<WGM22); // CTC 모드 선택
  
  	//DDRD |= (1<<PD3); // OC2B 핀(PD3 핀)을 출력으로 설정
}

void piezo_on()	//소리on
{
  
  DDRD |= (1<<PD3);
}

void piezo_off()	//소리off
{
   DDRD &= ~(1<<PD3);
}

uint16_t time_cnt = 0;
uint16_t piezo_cnt = 0;

void run_piezo()	//피에조 실행/종료
{
  if (piezo_cnt > 0)
  {
    piezo_on();
  	piezo_cnt--;
  }
    
  else
     piezo_off();
  
}

void piezo_yun()
{
 	 piezo_cnt = 20;
}

int main() {
    init_port();
    init_port1();
    init_pcint1();
  	
  	piezo();
  
    init_timer2();
  

    while (1) {
      	 
      
      	if (height==0)
        {
          loading();
          disp_7seg_loading();
        }
      	else
      		disp_7seg();
      
      	run_piezo();	//피에조 실행
     
      	time_cnt++;
      
      	if (time_cnt >= 100) //초당10번 측정하기
        {
          time_cnt=0;
          start_measure();
          //piezo_yun();
          
        }
      	_delay_ms (1);
      	
    }
}
