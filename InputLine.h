/*
Class to create a new input line
with labels and input fields
input fields can be numeric, alphabetic or alphanumeric
*/

/*  LCD screen column size */
#ifndef SCREENCOLUMNSIZE
#define SCREENCOLUMNSIZE 16
#endif

/* input cursor blink rate in milliseconds */
#ifndef CURSORBLINKRATE
#define CURSORBLINKRATE 20
#endif
/* waiting time before jumping to next space in alphanumeric input mode  (milliseconds) */
#ifndef CURSORMOVETIME
#define CURSORMOVETIME 200
#endif

/* include libraries for LCD and Keypad */
#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

// Arduino versioning.
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/* InputField */
struct InputField{
	int start;
    int finish;
    int inputMode;
	int defaultInputMode; //if field can switch between different modes
	bool smartInput; // if field can switch between modes, remember the last mode user switched to
	struct InputField *next;
	struct InputField *previous;
};

//class Keypad : public Key, public HAL_obj {
class InputLine{
public:

	InputLine(int inputRows, bool cyclicRotation, LiquidCrystal *lcdRef, Keypad_I2C *kpd);

	bool addLabel(char* text);
	bool addLabel(int start, char* text);
	
	bool addInputField(int length, int inputMode);	
	bool addInputField(int start, int length, int inputMode);
	
	bool addInputField(int length, int inputMode, int defaultInputMode, bool smartInputType);
	bool addInputField(int start, int length, int inputMode, int defaultInputMode, bool smartInputType);
	
	void readInput();
	
	/* enable/disable reading input */
	void enableInput(bool enable);
	
	/* return methods */
	char* getInputFieldAsCharArr(int fieldNo);
	int getInputFieldAsInt(int fieldNo);
	

private:

	LiquidCrystal *lcd;
	Keypad_I2C *kpd;
	
	int row;
	int noOfInputFields;
	int cursorPoint;
	int currentReadMode;
	int cursorWait;
	int keyCount;
	char lastChar;
	int charCount;
	bool cursorActive;
	bool cursorMove;
	int cursorMoveTimer;
	bool cyclicRotation; //jump from last inputfield to first or reverse
	bool inputActive;
	
	int checkSpace(int start, int length);
	int findSpace(int length);
	void lcdSetCursor(int col, int row);
	void lcdPrint(char c);
	char keyPadReadKey();
	void displayInput();
	void cursorBlinker();
	void readNumbers(char key);
	void readLetters(char key);
	void moveCursorForward();
	void moveCursorBackward();
	void removeCurrentElementMoveForward();
	void removeCurrentElementMoveBackward();
	void switchInputMode();
	//jump to next or previous input field
	bool jumpField(bool jumpNext, bool doCyclicRotation, bool resetCursorMove);
	
	struct InputField *inputs;
	struct InputField *active;
	char inputLine[SCREENCOLUMNSIZE];
	bool avaliable[SCREENCOLUMNSIZE];
	char* textToReturn;
};


