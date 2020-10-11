#include <stdint.h>
#include "BOARD.h"
#include "Tree.h"
#include "Morse.h"
#include "Buttons.h"

enum    {               //my ENUM for all the states of the state machine
    WAITING,
    DOT, 
    DASH, 
    INTER_LETTER
} Morse = WAITING;

Node *charCheck;
Node *charCheckTemp;

int MorseInit(void) {
    ButtonsInit();     //initializing the BUTTONS
    
    //this char array holds the values of the transversed MORSE codes
    char treeChars[63] = {'#', 'E', 'I', 'S', 'H', '5', '4', 'V', '#', '3', 'U', 'F', '#', '#', 
        '#', '#', '2', 'A', 'R', 'L', '#', '#', '#', '#', '#', 'W', 'P', '#', '#', 'J', 
        '#', '1', 'T', 'N', 'D', 'B', '6', '#', 'X', '#', '#', 'K', 'C', '#', '#', 'Y', 
        '#', '#', 'M', 'G', 'Z', '7', '#', 'Q', '#', '#', 'O', '#', '8', '#', '#', '9', '0'};
    
    charCheck = TreeCreate(6, treeChars);       //calling tree create with the appropriate levels to create the morse code "tree"
                                                    //charCheck pointer gets the pointer value of this TreeCreate output
    
    if (charCheck == NULL)  {                   //if the pointer points to nothing, return ERROR
        return STANDARD_ERROR;
    } else {
        charCheckTemp = charCheck;              //I set another pointer equal to this charCheck value, so I can manipulate this with less chance of error
        Morse = WAITING;                        //setting state machine event to WAITING
        return SUCCESS;
    }
}


char MorseDecode(MorseChar in)  {
    char decodeCharSave;                        //this is a char I use to save the data of the data pointed at by charCheckTemp
                                                    //which was initialized before
    
    if (in == MORSE_CHAR_DASH)   {              //if the event is a DASH
        if (charCheckTemp->rightChild == NULL)   {      //in the event of a dash, if the rightChild points to nothing - return STANDARD ERROR
            return STANDARD_ERROR;
        }
        charCheckTemp = charCheckTemp->rightChild;      //set rightChild to next value
        return charCheckTemp->data;                     //returning this value
    } else if (in == MORSE_CHAR_DOT)   {        //if the event is a DOT
        if (charCheckTemp->leftChild == NULL)   {       //in the event of a dash, if the rightChild points to nothing - return STANDARD ERROR
            return STANDARD_ERROR;
        }
        charCheckTemp = charCheckTemp->leftChild;       //set rightChild to next value
        return charCheckTemp->data;                     //returning this value
    } else if (in == MORSE_CHAR_END_OF_CHAR)    {       //if the event is a end of char event
        if (charCheckTemp->data != NULL)    {           //if the data pointed to isn't NULL, then the decodeCharSave char gets the value of the data
            decodeCharSave = charCheckTemp->data;
            return decodeCharSave;                      //return decodeCharSave - the char value
        } else  {
            return STANDARD_ERROR;                      //otherwise, return STANDARD_ERROR (if NULL)
        }
    } else if (in == MORSE_CHAR_DECODE_RESET)   {       //in the event is a reset event
        charCheckTemp = charCheck;                      //setting that original temporary variable which I manipulated to the value of charCheck which was the initial value
        return SUCCESS;
    } else {
        return STANDARD_ERROR;
    }
    return SUCCESS;
}


int buttonValue = BUTTON_EVENT_NONE;            //buttonValue is the int that gets the value of ButtonsCheckEvents();
int eventIncrement;                             //used to check if the event corresponds to dash or dot (based on how long it is held down)
                                                    //incremented in the timer (by calling MorseCheckEvents)

MorseEvent MorseCheckEvents(void)   {
    buttonValue = ButtonsCheckEvents();         //getting ButtonsCheckEvents();
    eventIncrement++;                           //incremented here
    
    switch (Morse)  {                           //switch based on Morse state
        case WAITING:
            if (buttonValue == BUTTON_EVENT_4DOWN)  {   //if machine waiting and button 4 down
                buttonValue = BUTTON_EVENT_NONE;        //reset event value
                Morse = DOT;                            //a DOT event has occurred, proceed
                eventIncrement = 0;                     //increment of the event set to 0
            } else {                                    //if nothing has happened, stay in WAITING
                Morse = WAITING;
            }
            break;
        case DOT:                                       //dot event
            if (eventIncrement >= MORSE_EVENT_LENGTH_DOWN_DASH) {   //if the button is held down for longer than the constant declared before
                                                                            //go to DASH
                Morse = DASH;
            }
            if (buttonValue == BUTTON_EVENT_4UP)  {     //when button4 released
                buttonValue = BUTTON_EVENT_NONE;        //button event value reset
                Morse = INTER_LETTER;                   //go to INTER_LETTER
                eventIncrement = 0;                     //increment of the event set to 0
                return MORSE_EVENT_DOT;                 
            }
            break;
        case DASH:                                      //dash event
            if (buttonValue == BUTTON_EVENT_4UP)    {   //when button4 released
                buttonValue = BUTTON_EVENT_NONE;    //button event value reset
                Morse = INTER_LETTER;               //go to INTER_LETTER
                eventIncrement = 0;                 //increment of the event set to 0
                return MORSE_EVENT_DASH;
            }
            break;
        case INTER_LETTER:                              //INTER_LETTER event
            if (eventIncrement >= MORSE_EVENT_LENGTH_UP_INTER_LETTER_TIMEOUT)  {    //if held down longer than constant defined before, go to WAITING
                Morse = WAITING;
                eventIncrement = 0;
                return MORSE_EVENT_INTER_WORD;
            }
            if (buttonValue == BUTTON_EVENT_4DOWN)  {   //when button4 pressed down
                if (eventIncrement >= MORSE_EVENT_LENGTH_UP_INTER_LETTER)   {   //if held down longer than this constant BUT less than the timeout one, go to DOT
                    buttonValue = BUTTON_EVENT_NONE;
                    Morse = DOT;
                    eventIncrement = 0;
                    return MORSE_EVENT_INTER_LETTER;
                } else {
                    Morse = DOT;
                    buttonValue = BUTTON_EVENT_NONE;
                    eventIncrement = 0;
                }
            } 
            break;
        }
    return MORSE_EVENT_NONE;
}
