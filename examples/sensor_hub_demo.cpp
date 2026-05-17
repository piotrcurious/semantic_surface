#include <Arduino.h>
#include <semantic_surface/semantic_surface.h>
#include <semantic_surface/sensor.h>

using namespace SemanticSurface;

SemanticManager hub;

void setup() {
    Serial.begin(9600);
    hub.addElement(std::make_unique<Sensor>("temp_sensor", A0, "C"));
    hub.addElement(std::make_unique<Sensor>("humidity_sensor", A1, "%"));
    hub.addElement(std::make_unique<Sensor>("light_sensor", A2, "lux"));

    Serial.println("Sensor Hub Initialized");
}

void loop() {
    // In a real app, sensors would read actual pins.
    // For the demo, we simulate data via the semantic surface interface.
    static float t = 20.0;
    t += 0.1;

    hub.handleUpdate("{\"update\":{\"id\":\"temp_sensor\",\"simulationValue\":" + std::to_string(t) + "}}");

    Serial.println(hub.getFullState());
    delay(500);
}

int main() {
    setup();
    for(int i=0; i<10; ++i) loop();
    return 0;
}
