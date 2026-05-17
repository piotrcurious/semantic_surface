#ifndef ARDUINO_H
#define ARDUINO_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>

#define OUTPUT 1
#define INPUT 0
#define HIGH 1
#define LOW 0

#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

typedef std::string String;

void pinMode(int pin, int mode);
void analogWrite(int pin, int value);
int analogRead(int pin);
int digitalRead(int pin);

void delay(unsigned long ms);
unsigned long millis();

class SerialMock {
public:
    void begin(int baud);
    void println(const String& s);
    void println();
    void print(const String& s);
    bool available();
    String readStringUntil(char terminator);
};

extern SerialMock Serial;

int map(int x, int in_min, int in_max, int out_min, int out_max);
int constrain(int x, int a, int b);

#endif
