#include <avr/io.h>
#include <util/delay.h>

#define SSD_DATA PORTC
#define SSD_DATA_DDR DDRC
#define SSD_DIG_DDR DDRD
#define SSD_DIG PORTD
#define GET_ADC_DDR DDRA
#define GET_ADC  PORTA
#define RE_DDR DDRB
#define RES_VAL PORTB

#define DIG0 0
#define DIG1 1
#define DIG2 2
#define DIG3 3
#define DIG4 4
#define DIG5 5
#define DIG6 6
#define DIG7 7
 
void split_data( unsigned number);  
void ssd_display(void);                      
unsigned int get_adc(unsigned char cannel);
void init_newssd(void);
void init_adc(void);


unsigned char ssd[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char ssd_dig[]={0, 0, 0, 0};
float final;
float s;
float voltage;
int i;
int main(void){

	init_newssd();
	init_adc();
	float val;
	
	
	while(1){
	        
	      
		RE_DDR= ~(1<<DIG1 | 1<<DIG2);  
		val=get_adc(2);// channel 2 
        voltage=(val/1023)*5;
		final=((50000/voltage)-10000)/10;
		_delay_ms(12);
		float s=final;
		split_data(s);
		ssd_display();

	
		if(final<=100){
		
		     RE_DDR=~(1<<DIG0| 1<<DIG1);
			 RES_VAL=1<<DIG3;              
			 
			 for(i=0;i<100;i++){
			 
			    val=get_adc(2);// channel 2 
                voltage=(val/1023)*5;
		        final=(500/voltage)-100; 
			    _delay_ms(12);
				s=final;
				split_data(s);
				ssd_display();
				continue;
			}
		
		}
		
		else if((final>100) && (final<1000)){
		
		     RE_DDR=~(1<<DIG0| 1<<DIG2);
			 RES_VAL=1<<DIG4;
			 
			 while(1){
			 
			    val=get_adc(2);// channel 2 
                voltage=(val/1023)*5;
		        final=(5000/voltage)-1000; 
			    _delay_ms(12);
				 s=final;
				split_data(s);
				ssd_display();
			}
		
		}
		 

        if((final>=1000) && (final<10000)){
		
		     RE_DDR=~(1<<DIG1| 1<<DIG2);
			 RES_VAL=1<<DIG5;
			 
			 while(1){
			 
			    val=get_adc(2);// channel 2 
                voltage=(val/1023)*5;
		        final=(50000/voltage)-10000; 
			    _delay_ms(12);
				s=final;
				split_data(s);
				ssd_display();
			}
		}

	}
	return 0;
}

void ssd_display(void){
	     SSD_DATA=ssd[ssd_dig[0]];
	     SSD_DIG=1<<DIG0;
		 _delay_ms(25);
		 
		 SSD_DATA=ssd[ssd_dig[1]];
		 SSD_DIG=1<<DIG1;
		 _delay_ms(25);
		 
		 SSD_DATA=ssd[ssd_dig[2]];
		 SSD_DIG=1<<DIG2;
		 _delay_ms(25);
		 
		 SSD_DATA=ssd[ssd_dig[3]];
		 SSD_DIG=1<<DIG3;
		 _delay_ms(25);
	 }

void split_data( unsigned int number) {
	ssd_dig[0]=number/1000;
	ssd_dig[3]=number%10;
	ssd_dig[1]=(number%100)/10;
	ssd_dig[2]=(number%10)/10;
}

void init_newssd(void){
     
	 SSD_DATA_DDR = 0xff;
     SSD_DATA=0x00;
	 GET_ADC_DDR=0x00;
     RE_DDR=0xff;
	 SSD_DATA &=~(1<<DIG0 | 1<<DIG1 | 1<<DIG2 | 1<<DIG3| 1<<DIG4| 1<<DIG5| 1<<DIG6);
     SSD_DIG_DDR = SSD_DIG_DDR | (1<<DIG0 | 1<<DIG1 | 1<<DIG2 | 1<<DIG3 | 1<<DIG4 | 1<<DIG5 | 1<<DIG6);
}

void init_adc(void){
	ADMUX = 1<< REFS0;
	ADCSRA = 1<<ADEN | 1<<ADPS1 | 1<<ADPS0;
}

unsigned int get_adc(unsigned char channel){
	ADMUX = ADMUX | (channel & 0x0f);
	ADCSRA = ADCSRA | 1<<ADSC;
	  while(ADCSRA & (1<<ADSC)){
	 }
	return ADC;
}