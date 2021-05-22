#include <Arduino.h>
#include "colour.h"

enum Colours
{
  RED,
  GREEN,
  BLUE,
  YELLOW
};

#define LED_RED 15
#define LED_GREEN 16
#define LED_BLUE 17
#define LED_YELLOW 18

#define BTN_RED 4
#define BTN_GREEN 5
#define BTN_BLUE 6
#define BTN_YELLOW 7

#define SOUND 11

const int MAX_LEVEL = 100;
Colour *colours[4];

int sequence[MAX_LEVEL];
int gamer_sequence[MAX_LEVEL];

int level = 1;
int velocity = 1000;

void allLedsOn()
{
  for (int colour = RED; colour <= YELLOW; colour++)
  {
    colours[colour]->ledOn();
  }
}

void allLedsOff()
{
  for (int colour = RED; colour <= YELLOW; colour++)
  {
    colours[colour]->ledOff();
  }
}

void flashAllLedsAndBeep(int note)
{
  allLedsOn();
  tone(SOUND, note);
  delay(500);

  allLedsOff();
  noTone(SOUND);
  delay(250);
}

void flashSingleLedAndBeep(int colour, int duration)
{
  Colour *c = colours[colour];

  c->ledOn();
  tone(SOUND, c->getNote());
  delay(duration);

  c->ledOff();
  noTone(SOUND);
  delay(200);
}

void generateSequence()
{
  randomSeed(millis());

  for (int i = 0; i < MAX_LEVEL; i++)
  {
    sequence[i] = random(RED, YELLOW + 1);
  }

  for (int colour = RED; colour <= YELLOW; colour++)
  {
    colours[colour]->ledOn();
    delay(50);
    colours[colour]->ledOff();
  }
}

void readyBeep()
{
  tone(SOUND, 500);
  delay(200);
  noTone(SOUND);
  delay(200);
}

void showSequence()
{
  allLedsOff();

  for (int cIndex = 0; cIndex < level; cIndex++)
  {
    flashSingleLedAndBeep(sequence[cIndex], velocity);
  }

  readyBeep();
}

void sequenceCorrect()
{
  if (level < MAX_LEVEL)
  {
    level++;
  }

  velocity -= 50;

  readyBeep();
}

void sequenceError()
{
  for (int blinkCounter = 0; blinkCounter < 3; blinkCounter++)
  {
    flashAllLedsAndBeep(233);
  }

  for (int levelCounter = 0; levelCounter < level - 1; levelCounter++)
  {
    flashAllLedsAndBeep(600);
  }

  level = 1;
  velocity = 1000;
}

void readSequence()
{
  int flag = 0;

  for (int levelIndex = 0; levelIndex < level; levelIndex++)
  {
    flag = 0;

    while (flag == 0)
    {
      for (int colour = RED; colour <= YELLOW; colour++)
      {
        if (flag)
        {
          continue;
        }

        Colour *c = colours[colour];

        if (c->isBtnPressed())
        {
          flashSingleLedAndBeep(colour, velocity);

          gamer_sequence[levelIndex] = colour;
          flag = 1;

          if (gamer_sequence[levelIndex] != sequence[levelIndex])
          {
            sequenceError();
            return;
          }
          c->ledOff();
        }
      }
    }
  }
  sequenceCorrect();
}

void setup()
{
  colours[RED] = new Colour(LED_RED, BTN_RED, 349);
  colours[GREEN] = new Colour(LED_GREEN, BTN_GREEN, 329);
  colours[BLUE] = new Colour(LED_BLUE, BTN_BLUE, 293);
  colours[YELLOW] = new Colour(LED_YELLOW, BTN_YELLOW, 261);

  pinMode(SOUND, OUTPUT);
}

void loop()
{
  if (level == 1)
  {
    generateSequence();
  }

  //start button == first button in the list
  if (colours[RED]->isBtnPressed() || level != 1)
  {
    if (level == 1)
    {
      delay(200);
    }
    showSequence();
    readSequence();
  }
}
