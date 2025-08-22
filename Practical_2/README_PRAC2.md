This Practical focuses on coding the STM32FO using Assembly. The following are the practical deliverables:
1. By default, the LEDs should increment by 1 every 0.7 seconds (with the count starting from 0).
2. While SW0 is being held down, the LEDs should change to increment by 2 every 0.7 seconds.
3. While SW1 is being held down, the increment timing should change to every 0.3 seconds.
4. While SW2 is being held down, the LED pattern should be set to 0xAA. Naturally, the pattern should stay at 0xAA until SW2 is released, at which point it will continue counting upwards from 0xAA.
5. While SW3 is being held down, the pattern should freeze, and then resume counting only when SW3 is released. 
6. All code must be done using Assembly
7. A report detailing the program development and practical results obtained
   
(Note that the events triggered by SW0 and SW1 can occur at the same time)

