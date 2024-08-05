/* 
 * File:   application.c
 * Author: Abdelrahman Mansour
 *
 * Created on July 29, 2024, 2:53 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f4620.h>
#include "../pic18.h"
#include "application.h"

void tmr0_isr(void);
 void Int0_APP_ISR(void);
Std_ReturnType ret = E_NOT_OK;
Std_ReturnType retpad = E_NOT_OK;
uint8 seconds = 0;
uint8 temp_seconeds =0;
uint8 minutes = 0;
uint8 temp_minutes =0;
uint8 hours = 0;
uint8 temp_hours =0;
uint8 keybad_value = 99;
uint8 first_digit,second_digit;
uint8 value = 0 ;
uint8 pressed = 0 ;
const uint8 smile[] = {0x00, 0x0A, 0x0A, 0x00, 0x11, 0x0E, 0x00, 0x00};
const uint8 heart[] = {0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00};
lcd_8bit_t lcd_obj = {
  .lcd_rs.port = PORTA_INDEX,
  .lcd_rs.pin = PIN5,
  .lcd_rs.direction = GPIO_OUTPUT ,
  .lcd_rs.direction = LOW ,
  .lcd_en.port = PORTA_INDEX,
  .lcd_en.pin = PIN6,
  .lcd_en.direction = GPIO_OUTPUT ,
  .lcd_en.direction = LOW ,
  .lcd_data[0].port = PORTD_INDEX,
  .lcd_data[0].pin = PIN0,
  .lcd_data[0].direction = GPIO_OUTPUT,
  .lcd_data[0].logic = LOW,
  .lcd_data[1].port = PORTD_INDEX,
  .lcd_data[1].pin = PIN1,
  .lcd_data[1].direction = GPIO_OUTPUT,
  .lcd_data[1].logic = LOW,
  .lcd_data[2].port = PORTD_INDEX,
  .lcd_data[2].pin = PIN2,
  .lcd_data[2].direction = GPIO_OUTPUT,
  .lcd_data[2].logic = LOW,
  .lcd_data[3].port = PORTD_INDEX,
  .lcd_data[3].pin = PIN3,
  .lcd_data[3].direction = GPIO_OUTPUT,
  .lcd_data[3].logic = LOW,
  .lcd_data[4].port = PORTD_INDEX,
  .lcd_data[4].pin = PIN4,
  .lcd_data[4].direction = GPIO_OUTPUT,
  .lcd_data[4].logic = LOW,
  .lcd_data[5].port = PORTD_INDEX,
  .lcd_data[5].pin = PIN5,
  .lcd_data[5].direction = GPIO_OUTPUT,
  .lcd_data[5].logic = LOW,
  .lcd_data[6].port = PORTD_INDEX,
  .lcd_data[6].pin = PIN6,
  .lcd_data[6].direction = GPIO_OUTPUT,
  .lcd_data[6].logic = LOW,
  .lcd_data[7].port = PORTD_INDEX,
  .lcd_data[7].pin = PIN7,
  .lcd_data[7].direction = GPIO_OUTPUT,
  .lcd_data[7].logic = LOW,
};

keypad_t keypad_obj={
.keypad_r_pins[0].port =PORTC_INDEX,
.keypad_r_pins[0].pin = PIN0,
.keypad_r_pins[0].direction = GPIO_OUTPUT,
.keypad_r_pins[0].logic =LOW,
.keypad_r_pins[1].port =PORTC_INDEX,
.keypad_r_pins[1].pin = PIN1,
.keypad_r_pins[1].direction = GPIO_OUTPUT,
.keypad_r_pins[1].logic =LOW,
.keypad_r_pins[2].port =PORTC_INDEX,
.keypad_r_pins[2].pin = PIN2,
.keypad_r_pins[2].direction = GPIO_OUTPUT,
.keypad_r_pins[2].logic =LOW,
.keypad_r_pins[3].port =PORTC_INDEX,
.keypad_r_pins[3].pin = PIN3,
.keypad_r_pins[3].direction = GPIO_OUTPUT,
.keypad_r_pins[3].logic =LOW,
.keypad_c_pins[0].port =PORTC_INDEX,
.keypad_c_pins[0].pin = PIN4,
.keypad_c_pins[0].direction = GPIO_INPUT,
.keypad_c_pins[0].logic =LOW,
.keypad_c_pins[1].port =PORTC_INDEX,
.keypad_c_pins[1].pin = PIN5,
.keypad_c_pins[1].direction = GPIO_INPUT,
.keypad_c_pins[1].logic =LOW,
.keypad_c_pins[2].port =PORTC_INDEX,
.keypad_c_pins[2].pin = PIN6,
.keypad_c_pins[2].direction = GPIO_INPUT,
.keypad_c_pins[2].logic =LOW,
.keypad_c_pins[3].port =PORTC_INDEX,
.keypad_c_pins[3].pin = PIN7,
.keypad_c_pins[3].direction = GPIO_INPUT,
.keypad_c_pins[3].logic =LOW,
};

segment_t seven_seg_obj = {
.segment_type = SEGMENT_COMMON_ANODE,
.segment_pin[0].port = PORTA_INDEX,
.segment_pin[0].pin = PIN0,
.segment_pin[0].direction = GPIO_OUTPUT,
.segment_pin[0].logic = LOW,
.segment_pin[1].port = PORTA_INDEX,
.segment_pin[1].pin = PIN1,
.segment_pin[1].direction = GPIO_OUTPUT,
.segment_pin[1].logic = LOW,
.segment_pin[2].port = PORTA_INDEX,
.segment_pin[2].pin = PIN2,
.segment_pin[2].direction = GPIO_OUTPUT,
.segment_pin[2].logic = LOW,
.segment_pin[3].port = PORTA_INDEX,
.segment_pin[3].pin = PIN3,
.segment_pin[3].direction = GPIO_OUTPUT,
.segment_pin[3].logic = LOW,
};

 timer0_t timer0_obj ={
 .Timer0_InterruptHandler = tmr0_isr,
 .prescaler_enable = TIMER0_PRESCALER_ENABLE,
 .prescaler_val = TIMER0_PRESCALER_VALUE_DIV_BY_32,
 .timer0_mode = TIMER0_TIMER_MODE,
 .timer0_reg_size = TIMER0_REGESTER_SIZE_16BIT,
 .timer0_preload_val = 3036,
 
 };

 

 
int main() {
    ret = lcd_8bit_init(&lcd_obj);

    ret = keypad_init(&keypad_obj);
    ret = seven_segment_init(&seven_seg_obj);
    ret = Timer0_Init(&timer0_obj);
    ret = gpio_port_direction_init(PORTB_INDEX , GPIO_OUTPUT);
    ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"Press 1 To Set Clock");
    while(1){
         
            ret = gpio_port_write_logic(PORTB_INDEX, 0x01); // 0011 1110
            ret = gpio_port_write_logic(PORTA_INDEX, ((uint8)(hours/10)));
            __delay_ms(5);
            ret = gpio_port_write_logic(PORTB_INDEX, 0x02); // 0011 1101
            ret = gpio_port_write_logic(PORTA_INDEX, ((uint8)(hours%10)));
            __delay_ms(5);
            ret = gpio_port_write_logic(PORTB_INDEX, 0x04); // 0011 1011
            ret = gpio_port_write_logic(PORTA_INDEX, ((uint8)(minutes/10)));
            __delay_ms(5);
            ret = gpio_port_write_logic(PORTB_INDEX, 0x08); // 0011 0111
            ret = gpio_port_write_logic(PORTA_INDEX, ((uint8)(minutes%10)));
            __delay_ms(5);
            ret = gpio_port_write_logic(PORTB_INDEX, 0x10); // 0010 1111
            ret = gpio_port_write_logic(PORTA_INDEX, ((uint8)(seconds/10)));
            __delay_ms(5);
            ret = gpio_port_write_logic(PORTB_INDEX, 0x20); // 0001 1111
            ret = gpio_port_write_logic(PORTA_INDEX, ((uint8)(seconds%10)));
            __delay_ms(5);
            if(seconds == 60){
                seconds = 0;
                minutes++;
            }
            else{/*Nothing*/}
            if(minutes == 60){
                minutes = 0;
                hours++;
            }
            else{/*Nothing*/}
            if(hours == 24){
                hours=0;
            }
            else{/*Nothing*/}       
           temp_seconeds = seconds;
           temp_minutes = minutes;
           temp_hours = hours;
            
            value = keypad_get_value(&keypad_obj , &keybad_value);
            if(keybad_value == '1'){
                retpad = 1;
            }

            if(retpad){
                if(value == '1'){
                ret = lcd_8bit_send_command(&lcd_obj , LCD_CLEAR);
                ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"hours=--");
                first_digit = 'u';
                keybad_value = 99;
				__delay_ms(500);
				do
				{
					first_digit=keypad_get_value(&keypad_obj , &keybad_value);
					if((first_digit == '1') || (first_digit == '2') || (first_digit == '3') || (first_digit == '4') || (first_digit == '5')
                            || (first_digit == '6') || (first_digit == '7') || (first_digit == '8') || (first_digit == '9') || (first_digit == '0')){
                        pressed = 1;
                    }
                    else{
                        pressed = 0;
                    }
				} while (!(pressed));
				lcd_8bit_send_char_data_pos(&lcd_obj,1,7,first_digit);
                pressed = 0;
                keybad_value = 99;
				__delay_ms(300);
				do
				{   
                    
					second_digit=keypad_get_value(&keypad_obj , &keybad_value);
					if((second_digit == '1') || (second_digit == '2') || (second_digit == '3') || (second_digit == '4') || (second_digit == '5')
                            || (second_digit == '6') || (second_digit == '7') || (second_digit == '8') || (second_digit == '9') || (second_digit == '0')){
                        pressed = 1;
                    }
                    else{
                        pressed = 0;
                    }
				} while (!(pressed));
				lcd_8bit_send_char_data_pos(&lcd_obj,1,8,second_digit);
				__delay_ms(300);
               
                
                hours=(second_digit-48)+10*(first_digit-48);
                ret = lcd_8bit_send_command(&lcd_obj , LCD_CLEAR);
                ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"minutes=--");
                
                 first_digit = 'u';
                keybad_value = 99;
				__delay_ms(500);
				do
				{
					first_digit=keypad_get_value(&keypad_obj , &keybad_value);
					if((first_digit == '1') || (first_digit == '2') || (first_digit == '3') || (first_digit == '4') || (first_digit == '5')
                            || (first_digit == '6') || (first_digit == '7') || (first_digit == '8') || (first_digit == '9') || (first_digit == '0')){
                        pressed = 1;
                    }
                    else{
                        pressed = 0;
                    }
				} while (!(pressed));
				lcd_8bit_send_char_data_pos(&lcd_obj,1,9,first_digit);
                pressed = 0;
                keybad_value = 99;
				__delay_ms(300);
				do
				{   
                    
					second_digit=keypad_get_value(&keypad_obj , &keybad_value);
					if((second_digit == '1') || (second_digit == '2') || (second_digit == '3') || (second_digit == '4') || (second_digit == '5')
                            || (second_digit == '6') || (second_digit == '7') || (second_digit == '8') || (second_digit == '9') || (second_digit == '0')){
                        pressed = 1;
                    }
                    else{
                        pressed = 0;
                    }
				} while (!(pressed));
				lcd_8bit_send_char_data_pos(&lcd_obj,1,10,second_digit);
				__delay_ms(300);
				minutes=(second_digit-48)+10*(first_digit-48);
                
                ret = lcd_8bit_send_command(&lcd_obj , LCD_CLEAR);
                ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"seconds=--");
                
                 first_digit = 'u';
                keybad_value = 99;
				__delay_ms(500);
				do
				{
					first_digit=keypad_get_value(&keypad_obj , &keybad_value);
					if((first_digit == '1') || (first_digit == '2') || (first_digit == '3') || (first_digit == '4') || (first_digit == '5')
                            || (first_digit == '6') || (first_digit == '7') || (first_digit == '8') || (first_digit == '9') || (first_digit == '0')){
                        pressed = 1;
                    }
                    else{
                        pressed = 0;
                    }
				} while (!(pressed));
				lcd_8bit_send_char_data_pos(&lcd_obj,1,9,first_digit);
                pressed = 0;
                keybad_value = 99;
				__delay_ms(300);
				do
				{   
                    
					second_digit=keypad_get_value(&keypad_obj , &keybad_value);
					if((second_digit == '1') || (second_digit == '2') || (second_digit == '3') || (second_digit == '4') || (second_digit == '5')
                            || (second_digit == '6') || (second_digit == '7') || (second_digit == '8') || (second_digit == '9') || (second_digit == '0')){
                        pressed = 1;
                    }
                    else{
                        pressed = 0;
                    }
				} while (!(pressed));
				lcd_8bit_send_char_data_pos(&lcd_obj,1,10,second_digit);
				__delay_ms(300);
				seconds=(second_digit-48)+10*(first_digit-48);
                ret = lcd_8bit_send_command(&lcd_obj , LCD_CLEAR);
               
                
                if((hours > 23) || (minutes > 59) || (seconds > 59)){
                  
                ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"Invalid Number");
                __delay_ms(1000);
                ret = lcd_8bit_send_command(&lcd_obj , LCD_CLEAR);
                ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"Press 1 To Set Clock");
                hours = temp_hours;
                minutes = temp_minutes;
                seconds = temp_seconeds;

                }
                else{
				 ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"Your Clock Has Been Set");
                ret = lcd_8bit_send_custom_char(&lcd_obj,1,25,smile,0);
                ret = lcd_8bit_send_custom_char(&lcd_obj,1,26,heart,1);
                __delay_ms(1000);
                ret = lcd_8bit_send_command(&lcd_obj , LCD_CLEAR);
                ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"Press 1 To Set Clock");
                retpad = 0;
                keybad_value = 'u';
                }
                }
                
                
            }
            else{
                if((keybad_value == '2') || (keybad_value == '3') || (keybad_value == '4') || (keybad_value == '5') || (keybad_value == '6')
                        || (keybad_value == '7') || (keybad_value == '8') || (keybad_value == '9') || (keybad_value == '0') || (keybad_value == '#')
                        || (keybad_value == '=') || (keybad_value == '+') || (keybad_value == '-') || (keybad_value == '*') || (keybad_value == '/')){
                ret = lcd_8bit_send_command(&lcd_obj , LCD_CLEAR);
                ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"Wrong Choice");
                __delay_ms(1000);
                keybad_value = 99;
                ret = lcd_8bit_send_command(&lcd_obj , LCD_CLEAR);
                ret = lcd_8bit_send_string_pos(&lcd_obj,1,1,"Press 1 To Set Clock");
                }
                else{/*Nothing*/}
            }
            
      
    }
    
    return (EXIT_SUCCESS);
}

 void tmr0_isr(void){
     seconds++;
 
 }
