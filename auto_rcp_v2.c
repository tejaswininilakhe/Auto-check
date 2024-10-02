#include <mega32.h>
#include <delay.h>
#include <main.h>
#include <mem.h> 

// Declare your global variables here
// Timer0 overflow interrupt service routine 1Sec
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
     //ucSec++;               
}
/************************************************************************/

// Timer1 overflow interrupt service routine 100mSec
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
 {
    TCNT1 = 64286;     // again load value for 10ms  
    ucCount++;
    if(ucCount>103) 
    {
      ucSec++;  
      ucCount = 0;
    } 
    if(ucSec>59)
     {
        ucSec = 0;
        ucMin++; 
        //pokeb(0x00,ucMin);   
        //LMOTION_GLED =~LED_ON;
        EEPROM_write(0x00, ucMin);
     }
     if(ucMin>59)
     { 
        ucMin = 0;
        ucHour++;
        //pokeb(0x02,ucHour); 
        //LMOTION_GLED =~LED_ON; 
        EEPROM_write(0x02, ucHour);
     } 
          

    if (BUZZER_ENABLE&&(!key_sense))
    {
        if (++BUZZER_COUNTER >= BUZZER_TOTAL_TIME)       
        {
            BUZZER_COUNTER = 0;
            if (BUZZER_ENABLE == BUZZER_ONCE)        BUZZER_ENABLE = OFF;
        }
        else if (BUZZER_COUNTER >= BUZZER_ON_TIME)        BUZZER = OFF;      
        else                                              BUZZER = ON;
    }
   
    if (RLED_ENABLE)
    {
        if (++RLED_COUNTER >= RLED_TOTAL_TIME)       
        {
            RLED_COUNTER = 0;
            if (RLED_ENABLE == RLED_ONCE)        RLED_ENABLE = OFF;
        }
        else if (RLED_COUNTER >= RLED_ON_TIME)        RALARM_RLED = OFF;      
        else                                          RALARM_RLED = ON;
    }
   
    if (LLED_ENABLE)
    {
        if (++LLED_COUNTER >= LLED_TOTAL_TIME)       
        {
         LLED_COUNTER = 0;
            if (LLED_ENABLE == LLED_ONCE)        LLED_ENABLE = OFF;
        }
        else if (LLED_COUNTER >= LLED_ON_TIME)        LALARM_RLED = OFF;      
        else                                          LALARM_RLED = ON;
    }
    if(A_R && A_L)
    {
         if(++ol_flag>=50)  ol_error = 1;
    }
    else  {ol_flag = 0;ol_error = 0;}
    if((op_mode == AUTO)&&(req_on_time))
    {
        if(req_flag)    
        {
            LS_L = ON;      //Request condition. Update of all 3 ops 
            if(auto_request==NO_REQUEST)            {PB_R = OFF;PB_L = OFF;}
            else if(auto_request==LEFT_REQUEST)     {PB_R = OFF;PB_L = ON;}
            else if(auto_request==RIGHT_REQUEST)    {PB_R = ON;PB_L = OFF;}
            else if(auto_request==EMG_STOP_REQUEST) {PB_R = ON;PB_L = ON;}
        }
        req_on_time--;
    } 
    else if(op_mode == AUTO)              
    {
         auto_request = NO_REQUEST;
         LS_L = OFF;    //Status condition. Update of all 3 ops
         if(auto_status== BOTH_ALLOW)        {PB_R = ON;PB_L = ON;}
         else if(auto_status== LEFT_ALLOW)   {PB_R = OFF;PB_L = ON;}
         else if(auto_status== RIGHT_ALLOW)  {PB_R = ON;PB_L = OFF;}
         else if(auto_status== NOT_ALLOW)    {PB_R = OFF;PB_L = OFF;}
    }
 }

// Timer2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
 {
//TCNT2=0x00;                              // again load value for 1ms
   check_inputs();  
   
/* Left Motion key sensing with sensing delay filter */
    if(LMOTION_KEY)
    {
     if(++lmove_counter==KEY_SENSE_DELAY)      key_sense = KEY_BEEP_TIME; 
     else if(lmove_counter>=KEY_SENSE_DELAY)   {lmove_counter=KEY_SENSE_DELAY;L_move = 1;}    
    } 
    else 
    {
     if(lmove_counter)  --lmove_counter;
     else              L_move = 0;
    }

/* Left Reset key sensing with sensing delay filter */
    if(LRST_KEY)
    {
      if(++lrst_counter==KEY_SENSE_DELAY)      key_sense = KEY_BEEP_TIME; 
      else if(lrst_counter>=KEY_SENSE_DELAY)   {lrst_counter=KEY_SENSE_DELAY;L_rst = 1;}    
    } 
    else 
    {
     if(lrst_counter)  --lrst_counter;
     else              L_rst = 0;
    }

/* Right Motion key sensing with sensing delay filter */
    
    if(RMOTION_KEY)
    {
      if(++rmove_counter==KEY_SENSE_DELAY)      key_sense = KEY_BEEP_TIME; 
      if(rmove_counter>=KEY_SENSE_DELAY)   {rmove_counter=KEY_SENSE_DELAY;R_move = 1;}    
    } 
    else 
    {
     if(rmove_counter)  --rmove_counter;
     else              R_move = 0;
    }
/* Right Reset key sensing with sensing delay filter */
    if(RRST_KEY)
    {
      if(++rrst_counter==KEY_SENSE_DELAY)      key_sense = KEY_BEEP_TIME; 
      if(rrst_counter>=KEY_SENSE_DELAY)   {rrst_counter=KEY_SENSE_DELAY;R_rst = 1;}    
    } 
    else 
    {
     if(rrst_counter)  --rrst_counter;
     else              R_rst = 0;
    }  
    
    if(key_sense)
    {
      BUZZER = ON; 
      if(key_sense==1)      BUZZER = OFF;
      key_sense--;    
    }
    if(AUTO_RIGHT)
    {
      if(++aright_counter>=IP_SENSE_DELAY)   {aright_counter=IP_SENSE_DELAY;A_R = 1;}    
    } 
    else 
    {
     if(aright_counter)  --aright_counter;
     else                  A_R = 0;
    }  
    if(AUTO_LEFT)
    {
      if(++aleft_counter>=IP_SENSE_DELAY)   {aleft_counter=IP_SENSE_DELAY;A_L = 1;}    
    } 
    else 
    {
     if(aleft_counter)  --aleft_counter;
     else                  A_L = 0;
    }  
    
 }

void main(void)
{
// Declare your local variables here

// Reset Source checking
if (MCUCSR & (1<<PORF))
   {
   // Power-on Reset
   MCUCSR&=~((1<<JTRF) | (1<<WDRF) | (1<<BORF) | (1<<EXTRF) | (1<<PORF));
   // Place your code here

   }
else if (MCUCSR & (1<<EXTRF))
   {
   // External Reset
   MCUCSR&=~((1<<JTRF) | (1<<WDRF) | (1<<BORF) | (1<<EXTRF) | (1<<PORF));
   // Place your code here

   }
else if (MCUCSR & (1<<BORF))
   {
   // Brown-Out Reset
   MCUCSR&=~((1<<JTRF) | (1<<WDRF) | (1<<BORF) | (1<<EXTRF) | (1<<PORF));
   // Place your code here

   }
else if (MCUCSR & (1<<WDRF))
   {
   // Watchdog Reset
   MCUCSR&=~((1<<JTRF) | (1<<WDRF) | (1<<BORF) | (1<<EXTRF) | (1<<PORF));
   // Place your code here  
    //    BUZZER = ON;
    //    BUZZER_ON_TIME = 10;        // BUZZER_ON_TIME = 10 *(100ms)
   }
else if (MCUCSR & (1<<JTRF))
   {
   // JTAG Reset
   MCUCSR&=~((1<<JTRF) | (1<<WDRF) | (1<<BORF) | (1<<EXTRF) | (1<<PORF));
   // Place your code here

   }


  

// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=Out Bit6=In Bit5=In Bit4=In Bit3=In Bit2=Out Bit1=Out Bit0=Out 
DDRA=(1<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (1<<DDA2) | (1<<DDA1) | (1<<DDA0);
// State: Bit7=0 Bit6=P Bit5=P Bit4=P Bit3=P Bit2=0 Bit1=0 Bit0=0 
PORTA=(0<<PORTA7) | (1<<PORTA6) | (1<<PORTA5) | (1<<PORTA4) | (1<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (1<<PORTA0);

// Port B initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=In Bit2=In Bit1=Out Bit0=Out 
DDRB=(1<<DDB7) | (1<<DDB6) | (0<<DDB5) | (1<<DDB4) | (0<<DDB3) | (0<<DDB2) | (1<<DDB1) | (1<<DDB0);
// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=P Bit2=P Bit1=0 Bit0=0 
PORTB=(1<<PORTB7) | (1<<PORTB6) | (1<<PORTB5) | (0<<PORTB4) | (1<<PORTB3) | (1<<PORTB2) | (1<<PORTB1) | (1<<PORTB0);

// Port C initialization
// Function: Bit7=Out Bit6=In Bit5=Out Bit4=In Bit3=Out Bit2=In Bit1=Out Bit0=Out 
DDRC=(1<<DDC7) | (0<<DDC6) | (1<<DDC5) | (0<<DDC4) | (1<<DDC3) | (0<<DDC2) | (1<<DDC1) | (1<<DDC0);
// State: Bit7=0 Bit6=P Bit5=0 Bit4=P Bit3=0 Bit2=P Bit1=0 Bit0=0 
PORTC=(0<<PORTC7) | (1<<PORTC6) | (0<<PORTC5) | (1<<PORTC4) | (0<<PORTC3) | (1<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=P Bit6=P Bit5=P Bit4=P Bit3=P Bit2=P Bit1=P Bit0=P 
PORTD=(1<<PORTD7) | (1<<PORTD6) | (1<<PORTD5) | (1<<PORTD4) | (1<<PORTD3) | (1<<PORTD2) | (1<<PORTD1) | (1<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
TCCR0=(0<<WGM00) | (1<<COM01) | (0<<COM00) | (1<<WGM01) | (1<<CS02) | (0<<CS01) | (1<<CS00);
TCNT0=0x00;
OCR0=0x97;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 125.000 kHz
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer Period: 0.52429 s
// Timer1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (1<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;
TCNT1 = 64286;                            // again load value for 100ms


// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 250.000 kHz
// Mode: Normal top=0xFF
// OC2 output: Disconnected
// Timer Period: 1.024 ms
ASSR=0<<AS2;
TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (1<<CS21) | (1<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (1<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (1<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);

// USART initialization
// USART disabled
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
SFIOR=(0<<ACME);

// ADC initialization
// ADC disabled
ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);
/*******************************************************/
ucSec = 0;
ucMin = 0;
ucHour = 0;
/******************************************************/
 ucMin = EEPROM_read(0x00);  //peekb(0x00); 
 ucHour = EEPROM_read(0x02);//peekb(0x02);
delay_ms(3000);
/*
if((RMOTION_KEY)&&(LMOTION_KEY))
{
    delay_ms(3000);  
    if((RMOTION_KEY)&&(LMOTION_KEY))
    {  
        ucMin  = 0;
        ucHour = 0;
        EEPROM_write(0x00, ucMin);
        EEPROM_write(0x02, ucHour);
    }
}  */
  
// Globally enable interrupts
#asm("sei")

BUZZER = ON;
delay_ms(1000);
BUZZER = OFF;
for (i=0;i<3;i++)
    {
        LMOTION_GLED = LED_ON;
        RMOTION_GLED = LED_ON;
        delay_ms(1000);
        LMOTION_GLED = LED_OFF;
        RMOTION_GLED = LED_OFF;
        delay_ms(1000);
    } 
    

// Watchdog Timer initialization
// Watchdog Timer Prescaler: OSC/1024k
//WDTCR=(0<<WDTOE) | (1<<WDE) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);

while (1)
 {  
      if(ucHour>100)
       {  
         /*for (i=0;i<3;i++)
         {
           LMOTION_GLED = LED_ON;
           RMOTION_GLED = LED_ON;
           delay_ms(1000);
           LMOTION_GLED = LED_OFF;
           RMOTION_GLED = LED_OFF;
           delay_ms(1000);
         } */    
         for (;;)     
         {
            RIGHT_ERROR_FLAG = OFF;
            MANUAL_FORWARD = OFF; 
            RMOTION_GLED = LED_OFF; 
            RALARM_RLED = LED_OFF;
            LEFT_ERROR_FLAG = OFF;
            MANUAL_REVERSE = OFF;
            LMOTION_GLED = LED_OFF; 
            LALARM_RLED = LED_OFF; 
            BUZZER_ENABLE = BUZZER_OFF;
         }
        } 
    //#asm("wdr")     // watchdog timer reset    
    
    if(AUTO_MANUAL_SIGNAL)     op_mode = AUTO;
    else                       op_mode = MANUAL;
    if(op_mode==MANUAL)    /************* manual mode ***************/
    {
    if (LINK)                               M_MODE = run;
    else                                    M_MODE = inch;
    if(Error_flag)                          error_mode();
    else if(EMERGENCY || OVERLOAD)   {Error_flag = ON;error_mode();}
    else
    {
        if((LEFT_ERROR_FLAG)||(!(LS1 && LS2 && LS3)))                       left_error_mode(); 
        if((RIGHT_ERROR_FLAG)||(!(RS1 && RS2 && RS3)))                      right_error_mode();
        if((!LEFT_ERROR_FLAG)&&(L_move) &&(LS4))                            reverse_movement();
        else if (((!LEFT_ERROR_FLAG)&&(M_MODE == inch))||(!LS4))            reverse_stop(); 
        if((!RIGHT_ERROR_FLAG)&&(R_move)&&(RS4))                            forward_movement();
        else if (((!RIGHT_ERROR_FLAG)&&(M_MODE == inch))||(!RS4))           forward_stop(); 

        if(L_rst) 
        {   
          if((LS1 && LS2 && LS3 ))   //C
          {
            LEFT_ERROR_FLAG = OFF;
            MANUAL_REVERSE = OFF;
            LMOTION_GLED = LED_OFF; 
            LALARM_RLED = LED_OFF;
            LLED_ENABLE = LLED_OFF;
            // 100ms buzzer
            BUZZER_ON_TIME = 10;
            BUZZER_TOTAL_TIME = 20;
            BUZZER_COUNTER = 0;
            if ((BUZZER_ENABLE == BUZZER_ERROR)||(BUZZER_ENABLE == BUZZER_LMOTION)||(BUZZER_ENABLE == BUZZER_LERROR)||(!BUZZER_ENABLE))                
            {
               if (!MANUAL_FORWARD)     BUZZER_ENABLE = BUZZER_OFF;
               else                     BUZZER_ENABLE = BUZZER_RMOTION;
            }
          }
          else  ;              
        }
        if(R_rst) 
        {
          if((RS1 && RS2 && RS3 ))    //D
          {
            RIGHT_ERROR_FLAG = OFF;
            MANUAL_FORWARD = OFF;
            RMOTION_GLED = LED_OFF; 
            RALARM_RLED = LED_OFF;
            RLED_ENABLE = RLED_OFF;
            // 100ms buzzer
            BUZZER_ON_TIME = 10;
            BUZZER_TOTAL_TIME = 20;
            BUZZER_COUNTER = 0;
            if ((BUZZER_ENABLE == BUZZER_ERROR)||(BUZZER_ENABLE == BUZZER_RMOTION)||(BUZZER_ENABLE == BUZZER_RERROR)||(!BUZZER_ENABLE))    
            {
               if (!MANUAL_REVERSE)     BUZZER_ENABLE = BUZZER_OFF;
               else                     BUZZER_ENABLE = BUZZER_LMOTION;                                 
            }
         }
         else ;  
        }             
    }
  }
  else    /************************************** Auto mode *************************************/
  { 
    if(ol_error)        auto_ol = 1;
    else                auto_ol = 0;
    
    if(Error_flag)                          auto_error_mode();
    else if(EMERGENCY ||(auto_ol))   {Error_flag = ON;auto_error_mode();}
    else
    {
        if((LEFT_ERROR_FLAG)||(!(LS1 && LS2 && LS3)))                       auto_left_error_mode(); 
        if((RIGHT_ERROR_FLAG)||(!(RS1 && RS2 && RS3)))                      auto_right_error_mode();
        if((!LEFT_ERROR_FLAG)&&(A_L) &&(LS4))                               auto_reverse_movement();
        else                                                                auto_reverse_stop(); 
        if((!RIGHT_ERROR_FLAG)&&(A_R)&&(RS4))                               auto_forward_movement();
        else                                                                auto_forward_stop(); 
        if((!LEFT_ERROR_FLAG)&&(L_move) )                                   auto_left_request();
        else if((!RIGHT_ERROR_FLAG)&&(R_move))                              auto_right_request();
        if(L_rst) 
        {   
          if((LS1 && LS2 && LS3 ))   //C
          {
            LEFT_ERROR_FLAG = OFF;
            MANUAL_REVERSE = OFF;
            LMOTION_GLED = LED_OFF; 
            LALARM_RLED = LED_OFF;
            LLED_ENABLE = LLED_OFF;
            // 100ms buzzer
            BUZZER_ON_TIME = 10;
            BUZZER_TOTAL_TIME = 20;
            BUZZER_COUNTER = 0;
            if ((BUZZER_ENABLE == BUZZER_ERROR)||(BUZZER_ENABLE == BUZZER_LMOTION)||(BUZZER_ENABLE == BUZZER_LERROR)||(!BUZZER_ENABLE))                
            {
               if (!MANUAL_FORWARD)     BUZZER_ENABLE = BUZZER_OFF;
               else                     BUZZER_ENABLE = BUZZER_RMOTION;
            }
          }
          else  ;    
        }
        if(R_rst) 
        {
          if((RS1 && RS2 && RS3 ))    //D
          {
            RIGHT_ERROR_FLAG = OFF;
            MANUAL_FORWARD = OFF;
            RMOTION_GLED = LED_OFF; 
            RALARM_RLED = LED_OFF;
            RLED_ENABLE = RLED_OFF;
            // 100ms buzzer
            BUZZER_ON_TIME = 10;
            BUZZER_TOTAL_TIME = 20;
            BUZZER_COUNTER = 0;
            if ((BUZZER_ENABLE == BUZZER_ERROR)||(BUZZER_ENABLE == BUZZER_RMOTION)||(BUZZER_ENABLE == BUZZER_RERROR)||(!BUZZER_ENABLE))    
            {
               if (!MANUAL_REVERSE)     BUZZER_ENABLE = BUZZER_OFF;
               else                     BUZZER_ENABLE = BUZZER_LMOTION;
            }
         }
         else  ;  
        }
    }
    if(!RIGHT_ERROR_FLAG && !LEFT_ERROR_FLAG && !Error_flag && RS4 && LS4)  auto_status = BOTH_ALLOW;
    else if((LEFT_ERROR_FLAG && RIGHT_ERROR_FLAG)||Error_flag)              auto_status = NOT_ALLOW;
    else if(!RIGHT_ERROR_FLAG && RS4)                                       auto_status = RIGHT_ALLOW;
    else if(!LEFT_ERROR_FLAG && LS4)                                        auto_status = LEFT_ALLOW;
    else                                                                    auto_status = NOT_ALLOW;
  }
 }
}
/****************************manual funtions*****************************************/
void error_mode()
{
    MANUAL_REVERSE = OFF;
    MANUAL_FORWARD = OFF;
    RMOTION_GLED = LED_OFF;
    LMOTION_GLED = LED_OFF;
    // led and buzzer blink at 500ms FOR EMERGENCY INPUT
    if (EMERGENCY)
    {
         LLED_ON_TIME = 50;
         LLED_TOTAL_TIME = 100;
         LLED_ENABLE = RLED_RERROR;      
        
         RLED_ON_TIME = 50;
         RLED_TOTAL_TIME = 100;
         RLED_ENABLE = RLED_RERROR;

         BUZZER_ON_TIME = 50;
         BUZZER_TOTAL_TIME = 100;
         BUZZER_ENABLE = BUZZER_ERROR;
     }
         // led and buzzer blink at 1000ms FOR OVERLOAD INPUT
     else if (OVERLOAD)
     {
         LLED_ON_TIME = 100;
         LLED_TOTAL_TIME = 200;
         LLED_ENABLE = RLED_RERROR; 
        
         RLED_ON_TIME = 100;
         RLED_TOTAL_TIME = 200;
         RLED_ENABLE = RLED_RERROR;

         BUZZER_ON_TIME = 100;
         BUZZER_TOTAL_TIME = 200;
         BUZZER_ENABLE = BUZZER_ERROR;    
     }
   //
   if(EMERGENCY || OVERLOAD)   Error_flag = ON;
   else if((R_rst)||(L_rst))
   {
     Error_flag = OFF;
     LALARM_RLED = LED_OFF;
     LLED_ENABLE = LLED_OFF;
     RALARM_RLED = LED_OFF;
     RLED_ENABLE = RLED_OFF;
     BUZZER = OFF; 
     BUZZER_ENABLE = BUZZER_ONCE;    
   } 
}
void left_error_mode()
{
    LEFT_ERROR_FLAG = ON;
    MANUAL_REVERSE = OFF;
    LMOTION_GLED = LED_OFF; 
    // Toggle Alarm led with 500ms buzzer 500ms On & 4500 ms Off.
    LLED_ON_TIME = 50;
     LLED_TOTAL_TIME = 100;
     LLED_ENABLE = RLED_RERROR;
     
     BUZZER_ON_TIME = 50;
     BUZZER_TOTAL_TIME = 500;
     BUZZER_ENABLE = BUZZER_LERROR;
    //BUZZER = ON;
   //
}
void right_error_mode()
{
    RIGHT_ERROR_FLAG = ON;
    MANUAL_FORWARD = OFF;
    RMOTION_GLED = LED_OFF; 
    // Toggle Alarm led with 500ms buzzer 500ms On & 4500 ms Off.
     BUZZER_ON_TIME = 50;
     BUZZER_TOTAL_TIME = 500;
     BUZZER_ENABLE = BUZZER_RERROR;
     
     RLED_ON_TIME = 50;
     RLED_TOTAL_TIME = 100;
     RLED_ENABLE = RLED_RERROR;

    //BUZZER = ON;
}
void reverse_movement()
{
    if (!MANUAL_FORWARD)
    {
        LALARM_RLED = LED_OFF;
        LLED_ENABLE = LLED_OFF;
        LMOTION_GLED = LED_ON; 
        MANUAL_REVERSE = ON;
        //Contineously toggle buzzer with 100ms 
        BUZZER_ON_TIME = 10;
        BUZZER_TOTAL_TIME = 20;
        BUZZER_ENABLE = BUZZER_LMOTION;   
    }
}
void forward_movement()
{
    if (!MANUAL_REVERSE)
    {
        RALARM_RLED = LED_OFF;
        RLED_ENABLE = RLED_OFF;
        RMOTION_GLED = LED_ON; 
        MANUAL_FORWARD = ON;
        //Contineously toggle buzzer with 100ms   
        BUZZER_ON_TIME = 10;
        BUZZER_TOTAL_TIME = 20;
        BUZZER_ENABLE = BUZZER_RMOTION;        
    }
}

void forward_stop()
{
        RMOTION_GLED = LED_OFF; 
        MANUAL_FORWARD = OFF;
        if(BUZZER_ENABLE == BUZZER_RMOTION)    BUZZER_ENABLE = BUZZER_ONCE;
}

void reverse_stop()
{
        LMOTION_GLED = LED_OFF; 
        MANUAL_REVERSE = OFF;
        if(BUZZER_ENABLE == BUZZER_LMOTION)    BUZZER_ENABLE = BUZZER_ONCE;
}

/************************************* auto funtions ****************************************/
void auto_error_mode()
 {
    MANUAL_REVERSE = OFF;
    MANUAL_FORWARD = OFF;
    RMOTION_GLED = LED_OFF;
    LMOTION_GLED = LED_OFF;
    // led and buzzer blink at 500ms FOR EMERGENCY INPUT
    if (EMERGENCY)
    {
         LLED_ON_TIME = 50;
         LLED_TOTAL_TIME = 100;
         LLED_ENABLE = RLED_RERROR;      
        
         RLED_ON_TIME = 50;
         RLED_TOTAL_TIME = 100;
         RLED_ENABLE = RLED_RERROR;

         BUZZER_ON_TIME = 50;
         BUZZER_TOTAL_TIME = 100;
         BUZZER_ENABLE = BUZZER_ERROR;
         if(auto_request!= EMG_STOP_REQUEST)    {req_flag = 1;req_on_time = 200;auto_request = EMG_STOP_REQUEST;}
         EMERGENCY_OP = ON;
    }
         // led and buzzer blink at 1000ms FOR OVERLOAD INPUT
    else if (auto_ol)
    {
         LLED_ON_TIME = 100;
         LLED_TOTAL_TIME = 200;
         LLED_ENABLE = RLED_RERROR; 
        
         RLED_ON_TIME = 100;
         RLED_TOTAL_TIME = 200;
         RLED_ENABLE = RLED_RERROR;

         BUZZER_ON_TIME = 100;
         BUZZER_TOTAL_TIME = 200;
         BUZZER_ENABLE = BUZZER_ERROR;    
    }
    if(EMERGENCY || auto_ol)   Error_flag = ON;
    else if((R_rst)||(L_rst))
    {
     Error_flag = OFF; 
     EMERGENCY_OP = OFF;
     LALARM_RLED = LED_OFF;
     LLED_ENABLE = LLED_OFF;
     RALARM_RLED = LED_OFF;
     RLED_ENABLE = RLED_OFF;
     BUZZER = OFF; 
     BUZZER_ENABLE = BUZZER_ONCE;
    } 
 }
 
 void auto_right_request()
 {
    if(auto_request!= RIGHT_REQUEST)    {req_flag = 1;req_on_time = 200;auto_request = RIGHT_REQUEST;}
 }

 void auto_left_request()
 {
    if(auto_request!= LEFT_REQUEST)    {req_flag = 1;req_on_time = 200;auto_request = LEFT_REQUEST;}
 }

void auto_left_error_mode()
 {
    LEFT_ERROR_FLAG = ON;
    MANUAL_REVERSE = OFF;
    LMOTION_GLED = LED_OFF; 
    // Toggle Alarm led with 500ms buzzer 500ms On & 4500 ms Off.
    LLED_ON_TIME = 50;
    LLED_TOTAL_TIME = 100;
    LLED_ENABLE = RLED_RERROR;
     
    BUZZER_ON_TIME = 50;
    BUZZER_TOTAL_TIME = 500;
    BUZZER_ENABLE = BUZZER_LERROR;
 }
 
 void auto_right_error_mode()
 {
    RIGHT_ERROR_FLAG   = ON;
    MANUAL_FORWARD     = OFF;
    RMOTION_GLED       = LED_OFF; 
    // Toggle Alarm led with 500ms buzzer 500ms On & 4500 ms Off.
     BUZZER_ON_TIME    = 50;
     BUZZER_TOTAL_TIME = 500;
     BUZZER_ENABLE     = BUZZER_RERROR;
     
     RLED_ON_TIME      = 50;
     RLED_TOTAL_TIME   = 100;
     RLED_ENABLE       = RLED_RERROR;    
    //BUZZER = ON;
 }
void auto_reverse_movement()
 {
    if (!MANUAL_FORWARD)
    {
        LALARM_RLED    = LED_OFF;
        LLED_ENABLE    = LLED_OFF;
        LMOTION_GLED   = LED_ON; 
        MANUAL_REVERSE = ON;
        //Contineously toggle buzzer with 100ms 
        if(!RIGHT_ERROR_FLAG)
        {
            BUZZER_ON_TIME     = 10;
            BUZZER_TOTAL_TIME  = 20;
            BUZZER_ENABLE      = BUZZER_LMOTION;
        }   
    }
 }
void auto_forward_movement()
 {
    if (!MANUAL_REVERSE)
    {
        RALARM_RLED = LED_OFF;
        RLED_ENABLE = RLED_OFF;
        RMOTION_GLED = LED_ON; 
        MANUAL_FORWARD = ON;
        //Contineously toggle buzzer with 100ms   
        if(!LEFT_ERROR_FLAG)
        {
            BUZZER_ON_TIME = 10;
            BUZZER_TOTAL_TIME = 20;
            BUZZER_ENABLE = BUZZER_LMOTION;
        }        
    }
 }

void auto_forward_stop()
 {
    RMOTION_GLED     =  LED_OFF; 
    MANUAL_FORWARD   =  OFF;
    if(BUZZER_ENABLE == BUZZER_RMOTION)    
    BUZZER_ENABLE    =  BUZZER_ONCE;
 }

void auto_reverse_stop()
 {
    LMOTION_GLED   = LED_OFF; 
    MANUAL_REVERSE = OFF;
    if(BUZZER_ENABLE == BUZZER_LMOTION)    BUZZER_ENABLE = BUZZER_ONCE;
 }

void check_inputs()
 {
   if(IP_LS1)  {ls1_off_time = 0;if(++ls1_on_time>=SENSE_DELAY)  {ls1_on_time = SENSE_DELAY;LS1 = ON;}}
   else        {ls1_on_time = 0;if(++ls1_off_time>=SENSE_DELAY)  {ls1_off_time = SENSE_DELAY;LS1 = OFF;}}

   if(IP_LS2)  {ls2_off_time = 0;if(++ls2_on_time>=SENSE_DELAY)  {ls2_on_time = SENSE_DELAY;LS2 = ON;}}
   else        {ls2_on_time = 0;if(++ls2_off_time>=SENSE_DELAY)  {ls2_off_time = SENSE_DELAY;LS2 = OFF;}}

   if(IP_LS3)  {ls3_off_time = 0;if(++ls3_on_time>=SENSE_DELAY)  {ls3_on_time = SENSE_DELAY;LS3 = ON;}}
   else        {ls3_on_time = 0;if(++ls3_off_time>=SENSE_DELAY)  {ls3_off_time = SENSE_DELAY;LS3 = OFF;}}

   if(IP_LS4)  {ls4_off_time = 0;if(++ls4_on_time>=SENSE_DELAY)  {ls4_on_time = SENSE_DELAY;LS4 = ON;}}
   else        {ls4_on_time = 0;if(++ls4_off_time>=SENSE_DELAY)  {ls4_off_time = SENSE_DELAY;LS4 = OFF;}}

   if(IP_RS1)  {rs1_off_time = 0;if(++rs1_on_time>=SENSE_DELAY)  {rs1_on_time = SENSE_DELAY;RS1 = ON;}}
   else        {rs1_on_time = 0;if(++rs1_off_time>=SENSE_DELAY)  {rs1_off_time = SENSE_DELAY;RS1 = OFF;}}

   if(IP_RS2)  {rs2_off_time = 0;if(++rs2_on_time>=SENSE_DELAY)  {rs2_on_time = SENSE_DELAY;RS2 = ON;}}
   else        {rs2_on_time = 0;if(++rs2_off_time>=SENSE_DELAY)  {rs2_off_time = SENSE_DELAY;RS2 = OFF;}}

   if(IP_RS3)  {rs3_off_time = 0;if(++rs3_on_time>=SENSE_DELAY)  {rs3_on_time = SENSE_DELAY;RS3 = ON;}}
   else        {rs3_on_time = 0;if(++rs3_off_time>=SENSE_DELAY)  {rs3_off_time = SENSE_DELAY;RS3 = OFF;}}

   if(IP_RS4)  {rs4_off_time = 0;if(++rs4_on_time>=SENSE_DELAY)  {rs4_on_time = SENSE_DELAY;RS4 = ON;}}
   else        {rs4_on_time = 0;if(++rs4_off_time>=SENSE_DELAY)  {rs4_off_time = SENSE_DELAY;RS4 = OFF;}}

   if(IP_EMERGENCY)  {emg_off_time = 0;if(++emg_on_time>=SENSE_DELAY)  {emg_on_time = SENSE_DELAY;EMERGENCY = ON;}}
   else              {emg_on_time = 0;if(++emg_off_time>=SENSE_DELAY)  {emg_off_time = SENSE_DELAY;EMERGENCY = OFF;}}

   if(IP_OVERLOAD)  {ol_off_time = 0;if(++ol_on_time>=SENSE_DELAY)  {ol_on_time = SENSE_DELAY;OVERLOAD = ON;}}
   else              {ol_on_time = 0;if(++ol_off_time>=SENSE_DELAY)  {ol_off_time = SENSE_DELAY;OVERLOAD = OFF;}}
 }
 
 void EEPROM_write(unsigned int uiAddress, unsigned char ucData)
{
 /* Wait for completion of previous write */
 while(EECR & (1<<EEWE))
 ;
 /* Set up address and data registers */
 EEAR = uiAddress;
 EEDR = ucData;
 /* Write logical one to EEMWE */
 EECR |= (1<<EEMWE);
 /* Start eeprom write by setting EEWE */
 EECR |= (1<<EEWE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
 /* Wait for completion of previous write */
 while(EECR & (1<<EEWE))
 ;
 /* Set up address register */
 EEAR = uiAddress;
 /* Start eeprom read by writing EERE */
 EECR |= (1<<EERE);
 /* Return data from data register */
 return EEDR;
}
