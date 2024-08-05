# Real-Time-Clock
This project is an Pic18f4620 that uses its timer0 (Internal Hardware) to count seconds which is displayed on two 7 Segments using Concept of Multiplexing (for more details go to my project of multiplexing two 7 Segments Common Anode) and program can run when timer0 is operating, then when timer0 increased by a value equal to 1 second, timer0 requests an interrupt to change a counter of seconds (Global Variable) in main program, then after 60 seconds, there are if conditions, these if conditions make counter of seconds equal to zero if it reaches its maximum value (59) and adjust counter of minutes and counter of hours according to value of counter of seconds
## Devices used:
+ Pic18f4620 (where we put our code to do specific functions)
+ LCD 20x4 (for displaying Temperature in Celsius)
+ 7Segments (for displaying numbers)
+ 7447 (for multplexing the 7 segments)
+ Keypad (to set time if 1 is Pressed)

## tools used:

+ Proteus 8.9 Professional
+ MPLAB X IDE

