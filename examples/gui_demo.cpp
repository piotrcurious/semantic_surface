#include <Arduino.h>
#include <semantic_surface/semantic_surface.h>
#include <semantic_surface/led_control.h>

using namespace SemanticSurface;

SemanticManager manager;

void setup() {
    Serial.begin(9600);

    manager.addElement(std::make_unique<Slider>("slider1", 0, 100, 50));
    manager.addElement(std::make_unique<Button>("button1"));
    manager.addElement(std::make_unique<Window>("window1", 3.14f, 10, 10, 50, 30, 128, 64));
    manager.addElement(std::make_unique<LEDControl>("led1", 13));

    Serial.println("GUI Demo Initialized");
}

void loop() {
    // Simulate incoming updates
    static int counter = 0;
    counter++;

    if (counter == 10) {
        Serial.println("Sending update for slider1...");
        manager.handleUpdate("{\"update\":{\"id\":\"slider1\",\"value\":80}}");
    }

    if (counter == 20) {
        Serial.println("Sending update for window1...");
        manager.handleUpdate("{\"update\":{\"id\":\"window1\",\"x\":20,\"y\":25,\"value\":2.718}}");
    }

    if (counter == 30) {
        Serial.println("Toggling LED...");
        manager.handleUpdate("{\"update\":{\"id\":\"led1\",\"toggle\":true}}");
    }

    Serial.println("Current State:");
    Serial.println(manager.getFullState());
    delay(100);
}

int main() {
    setup();
    for (int i = 0; i < 40; ++i) {
        loop();
    }
    return 0;
}
