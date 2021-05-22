#pragma once

#include <Arduino.h>

class Colour
{
private:
    const int btnPin;
    const int ledPin;
    const int note;

public:
    Colour(int ledPin, int btnPin, int note) : btnPin(btnPin), ledPin(ledPin), note(note)
    {
        pinMode(ledPin, OUTPUT);
        pinMode(btnPin, INPUT_PULLUP);
        ledOff();
    };
    const int getNote()
    {
        return note;
    }
    const void ledOn()
    {
        digitalWrite(ledPin, HIGH);
    }
    const void ledOff()
    {
        digitalWrite(ledPin, LOW);
    }
    bool isBtnPressed()
    {
        return digitalRead(btnPin) == LOW;
    }
};
