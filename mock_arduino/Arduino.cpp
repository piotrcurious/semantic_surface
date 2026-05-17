#include "Arduino.h"
#include <thread>

void pinMode(int pin, int mode) {}
void analogWrite(int pin, int value) {
    // std::cout << "[Mock] analogWrite pin " << pin << " value " << value << std::endl;
}
int analogRead(int pin) { return 512; }
int digitalRead(int pin) { return LOW; }

void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

unsigned long millis() {
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
}

void SerialMock::begin(int baud) {}
void SerialMock::println(const String& s) { std::cout << s << std::endl; }
void SerialMock::println() { std::cout << std::endl; }
void SerialMock::print(const String& s) { std::cout << s; }
bool SerialMock::available() { return false; }
String SerialMock::readStringUntil(char terminator) { return ""; }

SerialMock Serial;

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int constrain(int x, int a, int b) {
    if (x < a) return a;
    if (x > b) return b;
    return x;
}
