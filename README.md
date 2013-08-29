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
In <b>InputLine.h</b><br>
Include the libraries you use with your LCD and KeyPad.<br>
On <b>Line: 9</b> Update SCREENCOLUMNSIZE to match your LCD screen's column number.<br>
On <b>Line: 49</b> Update the paramaters' types in the constructor definition according to your LCD and KeyPad Classes.<br>
On <b>Line 71</b> and <b>72</b> update keypad and lcd types according to your LCD and Keypad Classes.<br>
<br>
In <b>InputLine.cpp</b><br>
On <b>Line 14</b>: Update the constructor according to changes you made to Line 49 on InputLine.h<br>
Update the implementation of these functions according to methods avaliable in your LCD, KeyPad classes:<br>
  <b>InputLine::void lcdSetCursor(int col, int row)<br>
  InputLine::	void lcdPrint(char c)<br>
  InputLine::	char keyPadReadKey()</b><br>


Usage & Public Methods
=================
<b>Example usage can be seen in InputLineExample.ino file.</b><br><br>
<b>Constructor: Initializes a new input line</b><br>
InputLine(int inputRow, bool cyclicRotation, LiquidCrystal *lcdRef, Keypad_I2C *kpd)<br>
inputRow: Which line of LCD should be used for input line<br>
cyclicRotation: When set to true enables cursor to jump from last input field to first one and vice versa<br>
*lcdRef: Reference to LCD instance<br>
*kpd: Reference to KeyPad instance<br>

<b>addLabel: Adds a new static label to input line, returns true if succeeds.</b><br>
bool addLabel(char* text)<br>
bool addLabel(int start, char* text)<br>
start: Start position of label on LCD. If omitted label is added to first avaliable space.<br>
text: A char* paramater and adds the label to first avaliable space.<br>

<b>addInputField: Adds a new input field to input line, returns true if succeeds.</b><br>
bool addInputField(int length, int inputMode)<br>
bool addInputField(int start, int length, int inputMode)<br>
start: Start position of input field on LCD. If omitted field is added to first avaliable space.<br>
length: Length of input field, please note starting element is included in length as well.<br>
inputMode: Input mode for input field. Possible values: 0=Numeric, 1=Alphabetic<br>

<b>Adding AlphaNumeric Input Fields:</b><br>
bool addInputField(int length, int inputMode, int defaultInputMode, bool smartInputType)<br>
bool addInputField(int start, int length, int inputMode, int defaultInputMode, bool smartInputType)<br>
start: Start position of input field on LCD. If omitted field is added to first avaliable space.<br>
length: Length of input field, please note starting element is included in length as well.<br>
inputMode: While adding alphanumeric input fields inputMode parameter must be 3. <br>
defaultInputMode: Initial input mode of input field can be set to 0 for numeric or 1 for alphabetic input.
smartInputType: If set to true, remembers the input mode the user letf the field and when returned continues from that mode.<br>

<b>readInput: Reads keypad and prints to lcd. Should be called in the loop() body of arduino program.</b><br>

<b>enableInput: Enables/Disables reading input from user.</b><br>
void enableInput(bool enable)<br>
enable: If set to true, enables reading input from user.<br>

<b>getFieldAsCharArr: Return char* from the specified input field.</b><br>
char* getInputFieldAsCharArr(int fieldNo)<br>
fieldNo: Input field's number. Input Fields can be accessed in the same order they are added. First input field is 1.<br><br>

<b>getFieldAsInt: Returns integer from the specified input field. If field contains non-numeric characters returns null.</b><br>
int getInputFieldAsInt(int fieldNo)<br>
fieldNo: Input field's number. Input Fields can be accessed in the same order they are added. First input field is 1.

Notes
=================
I have developed this library to make it easier to develop more user oriented applications using LCD and KeyPad with Arduino.<br>
I have tested the library on Arduino Uno R3, it should work on other versions as well.<br>
If you have found any bugs, have some improvement ideas or need help setting/porting the library drop a comment on the repository.<br>
Enjoy, happy developing :)
