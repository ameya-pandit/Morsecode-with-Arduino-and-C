# Morsecode-with-Arduino-and-C

# Summary #
This C Program interprets Morse Code messages with the use of an Arduino. The user can push a button on the Arduino to resemble a dot and a dash (a dot is a click and a dash is a click and a hold for a second), and the corresponding message is outputted on the LCD display. The combination of dots and dashes resemble different letters in the English alphabet and the different combinations can be used to conjure different messages. 

Click the link here for a demo: https://classes.soe.ucsc.edu/cmpe013/Spring17/Labs/Lab8/Lab8_demo.mp4

# Technologies Used #
This program uses trees to store the characters. Here is a link to a tree graph resembling the location of all the letters in the tree structure: https://external-preview.redd.it/s-3wLlEyAnwI9oyTmGgMRyGxjwTQlIdJPeEEm_BEVP4.jpg?s=acc20e3e8b4567f616b6d9be7349f65030349e47. 

The program interprets the user command (of clicking the button quickly or pressing the button down for a second or longer) to determine whether to go left or right on the tree. Going for the left child resembles a dot, and going for the right child resembles the dash. It is possible to create a message with 26 alphabets, numbers 0-9, and a few symbols. 

The button clicks indicate what node is selected and this node is then interpreted to be a certain character which is outputted to the LED display. 

# Files #
Morse.c handles the "Tree" portion of this program. It creates the tree by reading an array and creating a node for each element in that array. It analyzes how long each button press was and acts accordingly to the user input. It also decodes the message that is received (the button input) and traverses the tree to find the location of the character that is requested by the user. 

morseCodeLab.c handles the display portion of this program. It interprets the data received by the button press and calls functions in Morse.c to do the subsequent task related to each button press. It also updates the LCD display to show either a dot or a dash, or a space if there is an adequate wait between letter inputs. Once the traversal is complete and a letter, number, or symbol is retrieved, it updates the LCD display to show the converted string. Overall, the top part of the LCD shows the morse code input, and the bottom part shows the English/Numerical/Symbolic output.


