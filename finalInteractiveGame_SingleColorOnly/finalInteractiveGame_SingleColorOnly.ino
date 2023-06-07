#include <AsyncDelay.h>
#include <Adafruit_CircuitPlayground.h>

// mode control & ISR global vars 
int modeControl = 1;
int leftPin = 4; 
int rightPin = 5; 
int switchPin = 7;
int accPin = 27;
volatile bool leftFlag = 0;
volatile bool rightFlag = 0; 
volatile bool switchFlag = 0;
volatile bool sensorFlag = 0;
bool switchState = 0;


// Serial Monitor messages
String levelOneText = "You currently have level one selected. Flip the switch once to start the level!";
String levelTwoText = "You currently have level two selected. Flip the switch once to start the level!";
String levelThreeText = "You currently have level three selected. Flip the switch once to start the level!";
String startLevelText = "Starting the level!";


// game global vars 
int delayTime;
int count;
String userInput; 
int myPixel = 0;

// timer global vars 
AsyncDelay delay_10s;

void setup() 
{
  CircuitPlayground.begin();
  CircuitPlayground.setAccelTap(1, 120);
  randomSeed(analogRead(0));
  Serial.begin(9600);
  while(!Serial);

  attachInterrupt(digitalPinToInterrupt(leftPin), decrease, RISING);
  attachInterrupt(digitalPinToInterrupt(rightPin), increase, RISING);
  attachInterrupt(digitalPinToInterrupt(switchPin), startLevel, CHANGE);

  switchState = digitalRead(switchPin);

  delay_10s.start(10000, AsyncDelay::MILLIS);
}

void loop() 
{

  // decreasing level difficulty 
  if(leftFlag)
  {
    delay(5);
    if(modeControl == 1)
    {
      modeControl = 3; 
    }
    else 
    {
      modeControl--;
    }

    if(modeControl == 1)
    {
      Serial.println(levelOneText);
    }
    else if(modeControl == 2)
    {
      Serial.println(levelTwoText);
    }
    else
    {
      Serial.println(levelThreeText);
    }
    leftFlag = 0;

    // switch(modeControl)
    // {
    //   case 1: 
    //   {
    //     Serial.println(levelOneText);
    //   }

    //     break;

    //   case 2: 
    //   {
    //     Serial.println(levelTwoText);
    //   }
      
    //     break;

    //   case 3:
    //   {
    //     Serial.println(levelThreeText);
    //   }
    //     break;

    //   default: 
    //   break; 

    }

  

  // increasing level difficulty 
 if(rightFlag)
  {
    delay(5);
    if(modeControl == 3)
    {
      modeControl = 1; 
      Serial.println(levelOneText);
    }
    else 
    {
      modeControl++;
    }
    rightFlag = 0;

        if(modeControl == 1)
    {
      Serial.println(levelOneText);
    }
    else if(modeControl == 2)
    {
      Serial.println(levelTwoText);
    }
    else
    {
      Serial.println(levelThreeText);
    }

    // switch(modeControl)
    // {
    //   case 1: 
    //   {
    //     Serial.println(levelOneText);
    //   }
    //   break;

    //   case 2: 
    //   {
    //     Serial.println(levelTwoText);
    //   }
      
    //   break;

    //   case 3:
    //   {
    //     Serial.println(levelThreeText);
    //   }
    //   break;
    //   default: 
    //   break; 

    // }
  }



  switch(modeControl)           
  {
    case 1: 
    {
      if(switchFlag)
      {
        readSwitch();
        Serial.println(startLevelText);
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
        Serial.println(startLevelText);
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
        Serial.println(startLevelText);
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

int sparkle(int level) 
{
  count = 0;
  delay_10s.restart();

  if(level == 1)
    {
      delayTime = random(350, 401);
      Serial.println(delayTime);
    }
    if(level == 2)
    {
      delayTime = random(250, 301);
      Serial.println(delayTime);
    }
    if(level == 3)
    {
      delayTime = random(200, 251);
      Serial.println(delayTime);
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
  Serial.println(count);
}


void readSwitch()
{
  for(int i = 0; i < 10; i++)
    CircuitPlayground.setPixelColor(i, 0XFFFFFF);
    delay(5000);
  CircuitPlayground.clearPixels();

  switchFlag = 0;
}

void getUserInput()
{
  Serial.println("Please input your guess");
  delay(10000);
  userInput = Serial.readStringUntil('\n');
  Serial.println("Your guess is: " + userInput +".");

  if(count == (userInput.toInt()))
  {
  Serial.println("You guessed correctly!");
  }
  else
  {
  Serial.println("You guessed incorrectly....");
  }
}

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