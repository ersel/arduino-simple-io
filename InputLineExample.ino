/*
InputLine Library Example Usage
Ersel Aker
29/08/2013
*/

/* Include libraries required for your LCD and Keypad */
/* below are LCD KEYPAD libraries I am using */
/* remove or change them according to your setup */
#include <Wire.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

/* declarations for my test LCD, Keypad Environment */
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
byte rowPins[ROWS] = {0,1,2,3}; 
byte colPins[COLS] = {4,5,6,7};
int i2caddress = 0x20;

Keypad_I2C kpd = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, i2caddress );
LiquidCrystal lcd(12, 11, 5, 4, 3, 6);
/* END OF MY ENVIRONMENT SETUP */

/* include InputLine library */
#include <InputLine.h>

/* declaring labels */
char* nickLabel = "Nick:";
char* ageLabel = "Age:";

/* declare a char pointer to hold data coming from nick field */
char* nickField;

/* declare int to hold data coming from age field */
int ageField;
/* create input line instance */
/* Row:0, Smart Input: On, passing the references of LCD and Keypad instances */
InputLine input = InputLine(0, true, &lcd, &kpd);

void setup(){
  
  /* initialize serial for testing */
  Serial.begin(9600);
  
  /* initializing my keypad */
  kpd.begin();
  
  /* initializing my LCD */ 
  lcd.begin(16, 2);
  
  /* Lets add Nick label */
  if(input.addLabel(nickLabel)){
    /*if label is added print message on serial outpu t*/
    Serial.println("Nick label added");
  };
 
  /* Add alphabetic input field of 4 character for reading nick */
  /* length: 3, input mode: 3 (alphanumeric), Default Input Mode: 1 (alphabetic), smart input: true (remembers the input mode user has left the field) */
  if(input.addInputField(4, 3, 1, true)){
    /*if input field is added print message on serial output */
    Serial.println("Nick input field added");
  };
  
  
  /* Lets add Age label starting from 11th position on LCD */
  if(input.addLabel(11, ageLabel)){
    /*if label is added print message on serial output */
    Serial.println("Age label added");
  };
 
  /* Add numeric input field of 2 character for reading age */
  /* length: 2, input mode: 0 (numeric) */
  if(input.addInputField(2, 0)){
    /*if input field is added print message on serial output */
    Serial.println("Age input field added");
  };
  
}


void loop(){
  
    /* read keys */ 
    input.readInput();
    
    /* get the values from input fields                        */
    /* you can get data as char* or int                        */
    /* you can access input fields in the order you added them */
    /* nick field was added first so it is accessed by 1       */
    nickField = input.getInputFieldAsCharArr(1);
    Serial.print("Nick: ");
    for(int i = 0; i < strlen(nickField); i++){
      Serial.print(nickField[i]);
    }
    
    Serial.print(" Age: ");
    ageField = input.getInputFieldAsInt(2);
    Serial.println(ageField);
    
    /* delay is essential, input reading is based on polling                */
    /* I use 10ms change it to whatever you feel is comfortable             */
    /* update cursorblinkrate and cursormovetime in InputLine.h accordingly */
    delay(10); 
}