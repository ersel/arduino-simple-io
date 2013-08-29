Arduino Simple Input Output Library
=================
A simple input output library to use Arduino with keypad and LCD.

Features
=================
Allows adding static labels and/or input fields to a specified line on LCD.<br>
Input Fields can be either numeric, alphabetic or both.<br>
Any number of labels/input fields can be added to an input line if they fit on the LCD screen.<br>
Labels/Input fields can be added to a specific position or to first avaliable space on screen.<br>
Input fields can be read as integers or char arrays.<br>
Reading input can be enabled/disabled at any time.<br>
Cyclic rotation allows jumping cursor between input fields. Such as last input field to first and vice versa.<br>
Smart input mode remembers the input mode user left the the input field while using alphanumeric input fields.

Configuration & Set-up
=================
In InputLine.h <br>
Include the libraries you use with your LCD and KeyPad.<br>
On Line: 49 Update the paramaters' types in the constructor definition according to your LCD and KeyPad Classes.<br>
On Line 71 and 72 update keypad and lcd types according to your LCD and Keypad Classes.<br>
<br>
In InputLine.cpp<br>
On Line 14: Update the constructor according to changes you made to Line 49 on InputLine.h<br>
Update the implementation of these functions according to methods avaliable in your LCD, KeyPad classes:<br>
  InputLine::void lcdSetCursor(int col, int row)<br>
  InputLine::	void lcdPrint(char c)<br>
  InputLine::	char keyPadReadKey()<br>
