#include <InputLine.h>

char alphaNumeric[9][3] = {
{'a','b','c'},
{'d','e','f'},
{'g','h','i'},
{'j','k','l'},
{'m','n','o'},
{'p','q','r'},
{'s','t','u'},
{'v','w','x'},
{'y','z','-'},
};

// <<constructor>> Allows custom keymap, pin configuration, and keypad sizes.
InputLine::InputLine(int inputRows, int noOfColumns, bool cyclicRotationChoice, LiquidCrystal *lcdRef, Keypad_I2C *kpdRef) {
	row = inputRows;
	screenColumnSize = noOfColumns;
	lcd = lcdRef;
	kpd = kpdRef;
	cyclicRotation = cyclicRotationChoice;
	
	//char* inputLine = (char *)malloc(noOfColumns*sizeof(char));
	//inputLine[0] = '\0';
	for(int i = 0; i < noOfColumns; i++){
		inputLine[i] = ' ';
    }

	//avaliable = (bool*)malloc(noOfColumns * sizeof(bool));
	//avaliable[0] = true;
	for(int i = 0; i < noOfColumns; i++){
			avaliable[i] = true;
	}
	
	keyCount = 0;
	noOfInputFields = 0;
	cursorWait = CURSORBLINKRATE;
	lastChar = '>';
	charCount = 0;
	cursorActive = true;
	cursorMove = false;
	cursorMoveTimer = CURSORMOVETIME;
	inputs = (InputField*)malloc(sizeof(InputField));
	inputs->next = NULL;
	active = (InputField*)malloc(sizeof(InputField));
	active->next = NULL;
	
}

int InputLine::checkSpace(int start, int length){
	//check if there is space for new label
	if(start+length<=screenColumnSize){	
		for(int i = 0; i < length; i++){
			if(!avaliable[start + i]){
				Serial.println(start+i);
						Serial.println("fail2");
				return 0; //false
			}
		}
	} else {
		Serial.println("fail1");
		return 0;
	}
	return 1; //true
}

void InputLine::displayInput(){
    for(int i = 0; i < screenColumnSize; i++){
      lcd->setCursor(i, row);
      lcd->print(inputLine[i]);
    }
}

void InputLine::cursorBlinker(){
		
	if(cursorWait < 0){
		if(cursorActive){
			lcd->print("_");
			cursorActive = false;
		  		
		} else {
			lcd->print(inputLine[cursorPoint]);
			cursorActive = true;		
		}
		cursorWait = CURSORBLINKRATE;
	} else {
		cursorWait--;
	}
}

int InputLine::findSpace(int length){
	int j;
	for(int i = 0; i + length <= screenColumnSize; i++){
		if(avaliable[i]){
			if(checkSpace(i, length) == 1){
				return i;
			} else {
				i = i + length;
			}
		}
	}
	return NULL;
}
bool InputLine::addLabel(char* text){
	int length = strlen(text);
	int start = findSpace(length) + 1;
	return addLabel(start, text);
}

bool InputLine::addLabel(int start, char* text){
    int length = strlen(text);
    bool spaceAvaliable = checkSpace(start-1, length);
    start--;
		
    // if space is avaliable set label text
	// mark corresponding spaces unavaliable
	if(spaceAvaliable){
		for(int i = 0; i < length; i++){
			avaliable[start + i] = false;
			inputLine[start + i] = text[i];
		}
		displayInput();
		return true;
	} else {
		return false;
	}
}

bool InputLine::addInputField(int length, int inputMode){
	int start = findSpace(length) + 1;
	Serial.println("test1");
	Serial.println(start);
	Serial.println("test1");
	return addInputField(start, length, inputMode);
}

bool InputLine::addInputField(int start, int length, int inputMode){
	 int spaceAvaliable = checkSpace(start-1, length);
	Serial.println(spaceAvaliable);
Serial.println("spaceavail");	
	 if(spaceAvaliable == 1){
		start--;
		 // try to allocate memory for new input field 
		struct InputField *newInputField = (struct InputField*)malloc(sizeof(struct InputField));
		if(newInputField == NULL){
			return false;
		}
		
		newInputField->start = start;
		newInputField->finish = start + length - 1;
		newInputField->inputMode = inputMode;
		newInputField->defaultInputMode = inputMode;
		newInputField->smartInput = false;
		newInputField->previous = NULL;
		newInputField->next = NULL;
		
		if(inputs->next == NULL){
			//insert new InputField at the head of InputField list
			inputs->next = newInputField;
			//first input field is default active input field 
			active = newInputField;
			cursorPoint = start;
			//set currentReadMode to fields input mode
			currentReadMode = inputMode;
		} else {
			//insert new InputField  at the end of InputField list
			struct InputField *iterator = inputs;
			while (true) { 
				if(iterator->next == NULL)
				{
					
					iterator->next = newInputField;
					newInputField->previous = iterator;
					break; 
				}
				iterator = iterator->next;
			};
		}
		
		noOfInputFields++;
		
		/* mark space as not avaliable */
		for(int i = 0; i < length; i++){
			avaliable[start + i] = false;
		}

		return true;
	 } else if(spaceAvaliable == 0){
		return false;
	 }
}

bool InputLine::addInputField(int length, int inputMode, int defaultInputMode, bool smartInputType){
	int start = findSpace(length) + 1;
	return addInputField(start, length, inputMode, defaultInputMode, smartInputType);
}

bool InputLine::addInputField(int start, int length, int inputMode, int defaultInputMode, bool smartInputType){
	 int spaceAvaliable = checkSpace(start-1, length);
		
	 if(spaceAvaliable == 1){
		start--;
		 // try to allocate memory for new input field 
		struct InputField *newInputField = (struct InputField*)malloc(sizeof(struct InputField));
		if(newInputField == NULL){
			return false;
		}
		
		newInputField->start = start;
		newInputField->finish = start + length - 1;
		newInputField->inputMode = inputMode;
		newInputField->defaultInputMode = defaultInputMode;
		newInputField->smartInput = smartInputType;
		newInputField->previous = NULL;
		newInputField->next = NULL;
		
		if(inputs->next == NULL){
			//insert new InputField at the head of InputField list
			inputs->next = newInputField;
			//first input field is default active input field 
			active = newInputField;
			cursorPoint = start;
			//set currentReadMode to fields input mode
			currentReadMode = defaultInputMode;
		} else {
			//insert new InputField  at the end of InputField list
			struct InputField *iterator = inputs;
			while (true) { 
				if(iterator->next == NULL)
				{
					
					iterator->next = newInputField;
					newInputField->previous = iterator;
					break; 
				}
				iterator = iterator->next;
			};
		}
		
		noOfInputFields++;
		
		/* mark space as not avaliable */
		for(int i = 0; i < length; i++){
			avaliable[start + i] = false;
		}

		return true;
	 } else if(spaceAvaliable == 0){
		return false;
	 }
}
 
 bool InputLine::jumpField(bool jumpNext, bool doCyclicRotation, bool resetCursorMove){
	if(jumpNext){
		if(active->next != NULL){
			active = active->next;
			cursorPoint = active->start;
			currentReadMode = active->defaultInputMode;
			if(resetCursorMove){
				cursorMove = false;
				cursorMoveTimer = CURSORMOVETIME;
			}
		} else if(doCyclicRotation && cyclicRotation){
			active = inputs->next;
			cursorPoint = active->start;
			currentReadMode = active->defaultInputMode;
			if(resetCursorMove){
				cursorMove = false;
				cursorMoveTimer = CURSORMOVETIME;
			}
		} else {
			return false;
		}
	} else {
		if(active->previous != NULL){
			active = active->previous;
			cursorPoint = active->finish;
			currentReadMode = active->defaultInputMode;
			if(resetCursorMove){
				cursorMove = false;
				cursorMoveTimer = CURSORMOVETIME;
			}
		} else if(doCyclicRotation && cyclicRotation){
			struct InputField *iterator = inputs;
			while (true) { 
				if(iterator->next == NULL)
				{
					active = iterator;
					break; 
				}
				iterator = iterator->next;
			};
			cursorPoint = active->finish;
			currentReadMode = active->defaultInputMode;
			if(resetCursorMove){
				cursorMove = false;
				cursorMoveTimer = CURSORMOVETIME;
			}
		} else {
			return false;
		}
	}
	
	return true;
 }
 
 char* InputLine::getInputFieldAsCharArr(int fieldNo){
	struct InputField *iterator = inputs;
	for(int i = 0; i < fieldNo; i++){
		if(iterator->next == NULL){
			return NULL;
		}
		iterator = iterator->next;
	}
	
	if(textToReturn != NULL){
		free(textToReturn);
	}
	
	int length = iterator->finish - iterator->start;
	
	textToReturn = (char*) malloc (length*sizeof(char));
	int index = 0;
	for(int i = iterator->start; i <= iterator->finish; i++){
		textToReturn[index++] = inputLine[i];
	}
	return textToReturn;
 }
 
 int InputLine::getInputFieldAsInt(int fieldNo){
	struct InputField *iterator = inputs;
	for(int i = 0; i < fieldNo; i++){
		if(iterator->next == NULL){
			return NULL;
		}
		iterator = iterator->next;
	}
	
	int currentNumber;
	int total = 0;
	int multiplier = 1;
	
	for(int i = iterator->finish; i >= iterator->start; i--){
		currentNumber = inputLine[i] - '0';
		if(currentNumber >= 0 && currentNumber <= 9){
			total = total + (currentNumber * multiplier);
			multiplier = multiplier * 10;
		} else {
			return NULL;
		}
	}

	return total;
 }
 
void InputLine::readInput(){
  lcd->setCursor(cursorPoint, row);
  cursorBlinker(); 
  
  char key = kpd->getKey();
  
  switch(currentReadMode){
    case 0: readNumbers(key); break;
    case 1: readLetters(key); break;
    default: break;
  }
  
}

void InputLine::readLetters(char key){
    
	if(cursorMove){
		if(cursorMoveTimer < 1){
		    if(cursorPoint < active->finish){
				cursorMove = false;
				cursorMoveTimer = CURSORMOVETIME;
				cursorPoint++;
				displayInput();
				lastChar = '/';
			} else {
				if(jumpField(true, false, true)){
					displayInput();
					lastChar = '/';
				}
			}
		} else {
			cursorMoveTimer--;      
		}
	}
	
	switch (key){
		
		case 'A': break;
		
		case 'B':
		    //move cursor forwards if there is still space in this field
			// else jump to next input field
			if(cursorPoint < active->finish){
				cursorPoint++;
				cursorMove = false;
				displayInput();
			} else {
				if(jumpField(true, true, true)){
					displayInput();
				}
			}
		break;
		
		case 'C':
		    //move cursor backwards if there is still space in this field
			// else jump to previous input field
			if(cursorPoint > active->start){
				cursorPoint--;
				cursorMove = false;
				displayInput();
			} else {
				if(jumpField(false, true, true)){
					displayInput();
				}
			}
		  break;
		  
		case 'D': 
			// if alphanumeric input is active, allow switching between letters/numbers
			if(active->inputMode == 3){
				if(currentReadMode == 1){
					currentReadMode = 0;
				} else {
					currentReadMode = 1;
				}
				if(active->smartInput){
					active->defaultInputMode = currentReadMode;
				}
				cursorMove = false;
				cursorMoveTimer = CURSORMOVETIME;
			}
		break;
		
		case '*':
			// remove current element from active input field
			// else jump to previous input field and remove last element
			if(cursorPoint > active->start){
				inputLine[cursorPoint] = ' ';
				cursorPoint--;
				displayInput();
				cursorMove = false;
			} else if(cursorPoint == active->start) {
				inputLine[cursorPoint] = ' ';
				jumpField(false, false, true);
				displayInput();
			}		
		break;
		case '#':
			// remove current element from active input field
			// else jump to next input field and remove first element
			if(cursorPoint < active->finish){
				inputLine[cursorPoint] = ' ';
				displayInput();
				cursorPoint++;
				cursorMove = false;
				cursorMoveTimer = CURSORMOVETIME;				
			} else if(cursorPoint == active->finish){
				inputLine[cursorPoint] = ' ';
				jumpField(true, false, true);
				displayInput();
			}
		break;
		default:
			//no key assigned to 0 while reading letters
			if(key && key != '0'){
				if(cursorPoint <= active->finish){
					if(lastChar == key){
						int pos = key - '0';
						if(charCount > 2){
							charCount = 0;
						}
						cursorMoveTimer = CURSORMOVETIME;
						inputLine[cursorPoint] = alphaNumeric[pos-1][charCount];
						charCount++;
						cursorMoveTimer = CURSORMOVETIME;
						displayInput();
					} else {
						int pos = key - '0';
						charCount = 0;
						if(cursorMove == true){
							if(cursorPoint < active->finish){
								cursorPoint++;
							}else{
								jumpField(true, false, true);
							}
						} 

						inputLine[cursorPoint] = alphaNumeric[pos-1][charCount];

						displayInput();	  
						cursorMove = true;
						cursorMoveTimer = CURSORMOVETIME;
						lastChar = key;
					}
				}
					
			} 
			
		
		break;
		}
}

void InputLine::readNumbers(char key){
	
	switch (key){
		
		case 'A': break;
		
		case 'B':
		    //move cursor forwards if there is still space in this field
			// else jump to next input field
			if(cursorPoint < active->finish){
				cursorPoint++;
				cursorMove = false;
				displayInput();
			} else {
				if(jumpField(true, true, true)){
					displayInput();
				}
			}
		break;
		
		case 'C':
		    //move cursor backwards if there is still space in this field
			// else jump to previous input field
			if(cursorPoint > active->start){
				cursorPoint--;
				cursorMove = false;
				displayInput();
			} else {
				if(jumpField(false, true, true)){
					displayInput();
				}
			}
		  break;
		  
		case 'D': 
			// if alphanumeric input is active, allow switching between letters/numbers
			if(active->inputMode == 3){
				if(currentReadMode == 1){
					currentReadMode = 0;
				} else {
					currentReadMode = 1;
				}
				if(active->smartInput){
					active->defaultInputMode = currentReadMode;
				}
				cursorMove = false;
				cursorMoveTimer = CURSORMOVETIME;
			}
		break;
		
		case '*':
			// remove current element from active input field
			// else jump to previous input field and remove last element
			if(cursorPoint > active->start){
				inputLine[cursorPoint] = ' ';
				cursorPoint--;
				displayInput();
				cursorMove = false;
			} else if(cursorPoint == active->start) {
				inputLine[cursorPoint] = ' ';
				jumpField(false, false, true);
				displayInput();
			}		
		break;
		case '#':
			// remove current element from active input field
			// else jump to next input field and remove first element
			if(cursorPoint < active->finish){
				inputLine[cursorPoint] = ' ';
				displayInput();
				cursorPoint++;
				cursorMove = false;
				cursorMoveTimer = CURSORMOVETIME;				
			} else if(cursorPoint == active->finish){
				inputLine[cursorPoint] = ' ';
				jumpField(true, false, true);
				displayInput();
			}
		break;
		default:
			if(key){
				if(cursorPoint <= active->finish){
					inputLine[cursorPoint] = key;
					if(cursorPoint == active->finish){
						jumpField(true, false, false);
					} else {
						cursorPoint++;
					}
					displayInput();	  
				} 	
			}
		break;
		}
}