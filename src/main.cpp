#include <Arduino.h>
#include "colour.h"

enum Colours
{
  RED,
  GREEN,
  BLUE,
  YELLOW
};

const char *colourNames[] = {"RED", "GREEN", "BLUE", "YELLOW"};

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

void flashLeds()
{
  for (int colour = RED; colour <= YELLOW; colour++)
  {
    colours[colour]->ledOn();
    delay(50);
    colours[colour]->ledOff();
  }
}

void ledsOn()
{
  for (int colour = RED; colour <= YELLOW; colour++)
  {
    colours[colour]->ledOn();
  }
}

void ledsOff()
{
  for (int colour = RED; colour <= YELLOW; colour++)
  {
    colours[colour]->ledOff();
  }
}

void generate_sequence()
{
  randomSeed(millis());

  for (int i = 0; i < MAX_LEVEL; i++)
  {
    sequence[i] = random(RED, YELLOW + 1);
  }

  flashLeds();
}

void goBeep()
{
  tone(SOUND, 500);
  delay(200);
  noTone(SOUND);
  delay(200);
}

void show_sequence()
{
  ledsOff();

  Serial.println("Showing Sequence - seq");

  for (int cIndex = 0; cIndex < level; cIndex++)
  {
    Colour *c = colours[sequence[cIndex]];

    c->ledOn();
    tone(SOUND, c->getNote());
    delay(velocity);
    c->ledOff();
    noTone(SOUND);
    delay(200);
  }
  goBeep();
}

void right_sequence()
{
  if (level < MAX_LEVEL)
  {
    level++;
  }

  velocity -= 50;
  goBeep();
}

void wrong_sequence()
{
  for (int blinkCounter = 0; blinkCounter < 3; blinkCounter++)
  {
    ledsOn();
    tone(SOUND, 233);
    delay(500);

    ledsOff();
    noTone(SOUND);
    delay(250);
  }

  level = 1;
  velocity = 1000;
}

void get_sequence()
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
          c->ledOn();
          tone(SOUND, c->getNote());
          delay(velocity);
          noTone(SOUND);
          gamer_sequence[levelIndex] = colour;
          flag = 1;
          delay(200);

          if (gamer_sequence[levelIndex] != sequence[levelIndex])
          {
            wrong_sequence();
            return;
          }
          c->ledOff();
        }
      }
    }
  }
  right_sequence();
}

void setup()
{
  Serial.begin(115200);

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
    generate_sequence();
  }

  //start button == first button in the list
  if (colours[RED]->isBtnPressed() || level != 1)
  {
    if (level == 1)
    {
      delay(200);
    }
    show_sequence();
    get_sequence();
  }
}
