#include "BOARD.h"
#include "Morse.h"
#include "Oled.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries

#define CLEAR "                     "       //I used this macro (which has 21 spaces) in substitution of manually printing 21 spaces

#define BUTTON4_STATE_FLAG (1 << 7)

int morseEvent;                             //gets the value of the MorseCheckEvents() function in the timer
int morseFlag;                              //if an event has happened, flag has been triggered - corresponding usage of dots and dashes proceeds

static char display[100];                   //this is the char array I use to print
static char firstStr[21];                   //I use this array for appending the Top (the dots and dashes)
static char secondStr[63];                  //I use this array for appending the Bottom (chars)

//these are my append counters
int appendIncTop = 0;                       
int appendIncBot = 0;

char returnData;                            //this is the char variable I use to get the value of the tree, which I will print

static void Top(char append);               //utilized to clear top line of OLED
static void Bottom(char append);            //utilized to clear bottom line(s) of OLED
static void Clear();                        //utilized to CLEAR the screen/line

int main()
{
    BOARD_Init();


    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    //Initializes the functions needed in the lab
    MorseInit();                            
    OledInit();
    
    if (MorseInit() == STANDARD_ERROR)  {
        OledDrawString("ERROR: MorseInit() Failed");
        OledUpdate();
        FATAL_ERROR();
    }
    
    while(1)    {
        if (morseFlag)  {                                           //if an event has occurred (this flag has been initialized in the timer if an event has occurred)
            if (morseEvent == MORSE_EVENT_DOT)  {                   //if the event is a dot
                Top('.');                                           //print DOT
                returnData = MorseDecode(MORSE_CHAR_DOT);           //get the data from MorseDecode for when the input is a dot
                morseFlag = 0;                                      //flag is reset to 0
                morseEvent = MORSE_EVENT_NONE;                      //event is set to 0
            } else if (morseEvent == MORSE_EVENT_DASH)  {           //if the event is a dash
                Top('-');                                           //print DASH
                returnData = MorseDecode(MORSE_CHAR_DASH);          //get the data from MorseDecode for when the input is a dash
                morseFlag = 0;                                      //flag is reset to 0
                morseEvent = MORSE_EVENT_NONE;                      //event is reset to 0
            } else if (morseEvent == MORSE_EVENT_INTER_LETTER)  {   
                Clear();                                            //calling CLEAR function to clear the dots and dashes from before
                MorseDecode(MORSE_CHAR_END_OF_CHAR);                //getting END OF CHAR and RESET
                MorseDecode(MORSE_CHAR_DECODE_RESET);
                morseFlag = 0;                                      //resetting flag to 0
                morseEvent = MORSE_EVENT_NONE;                      //resetting event to 0
                Bottom(returnData);                                 //print out the data you get from the decoding - when the state machine is inter letter
            } else if (morseEvent == MORSE_EVENT_INTER_WORD)    {    
                Clear();                                            //calling CLEAR function to clear the dots and dashes from before
                Bottom(returnData);                                 //print out the data from decoding
                Bottom(' ');                                        //print out a space, as the next input is a new word
                MorseDecode(MORSE_CHAR_END_OF_CHAR);                //getting END OF CHAR and RESET
                MorseDecode(MORSE_CHAR_DECODE_RESET);
                morseFlag = 0;                                      //resetting flag to 0
                morseEvent = MORSE_EVENT_NONE;                      //resetting event to 0
            }
        }
    }

    while (1);
}

void __ISR(_TIMER_2_VECTOR, IPL4AUTO) TimerInterrupt100Hz(void)
{
    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
   
    morseEvent = MorseCheckEvents();                        //morseEvent is an int that gets the input from the state machine in regards to what the event is
    if (morseEvent != MORSE_EVENT_NONE) {                   //if an event has occurred...
        morseFlag = 1;                                      //event has occurred - flag has been set to 1 (true)
    }
}

static void Top (char append)  {                            //appending the top of the screen
    if (appendIncTop < 21)    {                             //if the incrementation is less than 21 (pixels)
        firstStr[appendIncTop] = append;                    //keep moving ahead in firstStr string which is 21 spaces big
        appendIncTop++;                                     //incrementing
    } else {    
        appendIncTop = 0;                                   //reset
    }
    sprintf(display, "%s\n%s", firstStr, secondStr);        //printing the firstStr along with the secondStr (set to nothing right now)
    OledDrawString(display);                                //print the display string, update the display
    OledUpdate();
}

static void Bottom (char append)   {                        //appending the bottom of the screen
    if (appendIncBot < 63)  {                               //if the incrementation is less than 63 (meaning 21 x 3 - 3 lines)
        if (append != '\0') {                               //if the input doesn't go to a NULL character...
            secondStr[appendIncBot] = append;                   //keep appending
        } else {                                            //otherwise, print out the NULL character
            secondStr[appendIncBot] = '#';
        }
        appendIncBot++;                                     //incrementing the bottom counter
    } else {
        appendIncBot = 0;                                   //otherwise, set to 0
    }
    appendIncTop = 0;                                       //setting the TOP counter to 0 as well
    sprintf(display, "%s\n%s", CLEAR, secondStr);           //you CLEAR the top (meaning the dots and dashes) and print the string with the secondStr (which has the char values)
    OledDrawString(display);                                //print the display string, update the display
    OledUpdate();
}

static void Clear()    {                                    //clear function
    sprintf(firstStr, "%s", CLEAR);                         //CLEARING the string if need be
    OledUpdate();                                           //update
}

