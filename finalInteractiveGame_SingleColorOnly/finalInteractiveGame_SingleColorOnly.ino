#include <AsyncDelay.h>
#include <Adafruit_CircuitPlayground.h>

// mode control & ISR global vars 
int modeControl = 1;
int leftPin = 4; 
int rightPin = 5; 
int switchPin = 7;
volatile bool leftFlag = 0;
volatile bool rightFlag = 0; 
volatile bool switchFlag = 0;
bool switchState = 0;


// Serial Monitor messages
String levelOneText = "Level one selected.";
String levelTwoText = "Level two selected.";
String levelThreeText = "Level three selected.";
String ruleText = "Welcome to Remember the Lights! Please refer the README.md file for a more detailed description and set of rules for this game.";
String guideText = "To see the different levels, click on one of the two buttons on the board.";


// game global vars 
int delayTime;
int count;
int myPixel = 0;
int userDiff;
String userInput; 

// timer global vars 
AsyncDelay delay_10s;

void setup() 
{
  CircuitPlayground.begin();
  randomSeed(analogRead(0));
  Serial.begin(9600);
  while(!Serial);

  attachInterrupt(digitalPinToInterrupt(leftPin), decrease, HIGH);
  attachInterrupt(digitalPinToInterrupt(rightPin), increase, HIGH);
  attachInterrupt(digitalPinToInterrupt(switchPin), startLevel, CHANGE);

  switchState = digitalRead(switchPin);

  delay_10s.start(10000, AsyncDelay::MILLIS);

  Serial.println(ruleText);
  Serial.println(guideText);
}

void loop() 
{
  // decreasing level difficulty 
  if(leftFlag)
  {
    delay(10);
    if(modeControl == 1)
    {
      modeControl = 3; 
    }
    else 
    {
      modeControl--;
    }
    leftFlag = 0;
    levelTextOutput();
  }

  // increasing level difficulty 
 if(rightFlag)
  {
    delay(10);
    if(modeControl == 3)
    {
      modeControl = 1; 
    }
    else 
    {
      modeControl++;
    }
    rightFlag = 0;
    levelTextOutput();
  }

  // trigger different light sequences depending on the value of mode control 
  switch(modeControl)           
  {
    case 1: 
    {
      if(switchFlag)
      {
        readSwitch();
        sparkle(1);
        getUserInput();
      }
    }
      break;

    case 2: 
    {
      if(switchFlag)
      {
        readSwitch();
        sparkle(2);
        getUserInput();
      }
    }
      break;

    case 3: 
    {
      if(switchFlag)
      {
        readSwitch();
        sparkle(3);
        getUserInput();
      }
    }
      break; 

    default:
      break;
  }
}


// function for making LEDs sparkle 
// takes integer argument that affects how fast the lights will sparkle 
int sparkle(int level) 
{
  count = 0;
  delay_10s.restart();

  if(level == 1)
    {
      delayTime = random(350, 401);
    }
    if(level == 2)
    {
      delayTime = random(250, 301);
    }
    if(level == 3)
    {
      delayTime = random(200, 251);
    }
  while(!(delay_10s.isExpired()))
  {
    myPixel = random(10);

    CircuitPlayground.setPixelColor(myPixel, 0xFFFFFF); // Set random pixel white
    delay(delayTime);
    CircuitPlayground.setPixelColor(myPixel, 0x000000); // Set random pixel off
    delay(delayTime);

    count++;
  } 
  Serial.println("The game has finished");
  delay(5);
}

// read if switch has been flipped and if so, turn all LEDs on to signify start of level
void readSwitch()
{
  for(int i = 0; i < 10; i++)
    CircuitPlayground.setPixelColor(i, 0XFFFFFF);
    delay(5000);
  CircuitPlayground.clearPixels();
  switchFlag = 0;
}

// prompts the user for input and outputs whether their guess was right or wrong + how much they were off by 
void getUserInput()
{
  Serial.println("Please input your guess");
  delay(10000);
  userInput = Serial.readStringUntil('\n');
  Serial.println("Your guess is: " + userInput +".");

  if(count == userInput.toInt())
  {
    Serial.println("You guessed correctly!");
  }
  else
  {
    userDiff = count - userInput.toInt();
    userDiff = abs(userDiff);
    Serial.println("You guessed incorrectly. The right answer was: " + String(count) + ". You were off by " + String(userDiff));
  }
}

// output which level has been selected only when the buttons are pressed
void levelTextOutput()
{
  switch(modeControl)
    {
      case 1:
      {
        Serial.println(levelOneText);
      }
        break;
      case 2:
      {
        Serial.println(levelTwoText);
      }
        break;
      case 3:
      {
        Serial.println(levelThreeText);
      }
        break;
      default:
        break;
    }
}


// ISR functions 


void decrease()
{
  leftFlag = 1;
}

void increase()
{
  rightFlag = 1; 
}

void startLevel()
{
  switchFlag = 1;
}
