//================================ INPUT =========================================
#define IP_LS1 ~PIND.7   // left sensor 1
#define IP_LS2 ~PIND.6   // left sensor 2
#define IP_LS3 ~PIND.5   // left sensor 3
#define IP_LS4 ~PIND.4   // left sensor 4

#define IP_RS1 ~PINA.6   // right sensor 1
#define IP_RS2 ~PINA.5   // right sensor 2
#define IP_RS3 ~PINA.4   // right sensor 3
#define IP_RS4 ~PINA.3   // right sensor 4

#define IP_EMERGENCY  PIND.3   // emergency input
#define IP_OVERLOAD   PIND.2   // overload input

#define AUTO_RIGHT ~PINC.6    // Auto right input
#define AUTO_MANUAL_SIGNAL ~PINC.4    // Auto manual signal
#define AUTO_LEFT ~PINC.2    // Auto left input

#define LMOTION_KEY ~PIND.0   // left sensor key 
#define LRST_KEY   ~PIND.1   // left RST key

#define RMOTION_KEY ~PINB.3   // right sensor key 
#define RRST_KEY   ~PINB.2   // right RST key

#define LINK PINB.5        // Movement mode selection 0=inch, 1=run

//================================ OUTPUT =========================================

#define LMOTION_GLED PORTB.6   //left motion green led
#define LALARM_RLED  PORTB.7   //left alarm red led

#define RMOTION_GLED PORTB.1   //right motion green led
#define RALARM_RLED  PORTA.0   //right alarm red led

#define BUZZER PORTB.4         // buzzer

#define MANUAL_FORWARD PORTA.1  // Manual forward output
#define MANUAL_REVERSE PORTA.2  // Manual reverse output

#define EMERGENCY_OP PORTC.0      // EMERGENCY OUTPUT

#define PB_L PORTA.7   
#define PB_R PORTC.7
#define LS_L PORTC.5
#define LS_R PORTC.1

#define DUMMY PORTC.3           // DUMMY output

#define inch 0
#define run  1
#define SENSE_DELAY      50
#define KEY_BEEP_TIME    50
#define IP_SENSE_DELAY   50
#define KEY_SENSE_DELAY  20
//=================================================================

#define ON  1
#define OFF 0

#define LED_ON  0                   //LED Status
#define LED_OFF 1

#define MANUAL  0
#define AUTO    1

enum buz_code
     {
        BUZZER_OFF = 0,
        BUZZER_ONCE,
        BUZZER_LMOTION,
        BUZZER_RMOTION,
        BUZZER_LERROR,
        BUZZER_RERROR,
        BUZZER_ERROR,
        BUZZER_CONT
     };

enum rled_code
     {
        RLED_OFF = 0,
        RLED_ONCE,
        RLED_RMOTION,
        RLED_RERROR,
        RLED_ERROR,
        RLED_CONT
     };

enum lled_code
     {
        LLED_OFF = 0,
        LLED_ONCE,
        LLED_LMOTION,
        LLED_LERROR,
        LLED_ERROR,
        LLED_CONT
     };
     
enum pb_l_code
     {
        PB_L_OFF = 0,
        PB_L_ONCE
     };  

enum pb_r_code
     {
        PB_R_OFF = 0,
        PB_R_ONCE
     };
     
enum ls_l_code
     {
        LS_L_OFF = 0,
        LS_L_ONCE
     };  
  
enum ls_r_code
     {
        LS_R_OFF = 0,
        LS_R_ONCE
     };  
     
enum a_req
{
  NO_REQUEST = 0,
  LEFT_REQUEST,
  RIGHT_REQUEST,
  EMG_STOP_REQUEST
};
enum a_STATUS
{
  BOTH_ALLOW = 0,
  LEFT_ALLOW,
  RIGHT_ALLOW,
  NOT_ALLOW
};



//============================= global variable ============================
unsigned int   LEFT_ERROR_FLAG, RIGHT_ERROR_FLAG, Error_flag, \
BUZZER_ON_TIME, BUZZER_TOTAL_TIME, BUZZER_COUNTER, BUZZER_ENABLE,\
RLED_ON_TIME, RLED_TOTAL_TIME, RLED_COUNTER, RLED_ENABLE, \
LLED_ON_TIME, LLED_TOTAL_TIME, LLED_COUNTER, LLED_ENABLE, i, M_MODE,\
LS1,LS2,LS3,LS4,RS1,RS2,RS3,RS4,EMERGENCY,OVERLOAD,\
L_move,L_rst,R_move,R_rst,lmove_counter,rmove_counter,lrst_counter,rrst_counter,key_sense,\
req_flag,req_on_time,op_mode,aright_counter,aleft_counter,A_R,A_L,auto_ol,auto_status,auto_request,\
ol_flag,ol_error; 

unsigned char ls1_on_time,ls1_off_time,ls2_on_time,ls2_off_time,ls3_on_time,ls3_off_time,\
ls4_on_time,ls4_off_time,rs1_on_time,rs1_off_time,rs2_on_time,rs2_off_time,\
rs3_on_time,rs3_off_time,rs4_on_time,rs4_off_time,emg_on_time,emg_off_time,ol_on_time,ol_off_time;

unsigned char ucCount,ucSec, ucMin, ucHour;




//============================= sub functions =============================

void  error_mode(void);
void left_error_mode(void);
void right_error_mode(void);
void reverse_movement(void);
void forward_movement(void);
void reverse_stop(void);
void forward_stop(void);
void check_inputs(void);

void auto_error_mode(void);
void auto_left_error_mode(void);
void auto_right_error_mode(void);
void auto_reverse_movement(void);
void auto_forward_movement(void);
void auto_reverse_stop(void);
void auto_forward_stop(void);
void auto_left_request(void);
void auto_right_request(void);
/*****************************************************************/
unsigned char EEPROM_read(unsigned int uiAddress);
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
