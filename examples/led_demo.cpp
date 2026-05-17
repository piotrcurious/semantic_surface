#include <Arduino.h>
#include <semantic_surface/led_control.h>

using namespace SemanticSurface;

void setup_led_demo(LEDControl& led) {
    Serial.begin(9600);
    Serial.println("LED Demo Initialized");
}

void loop_led_demo(LEDControl& led) {
    static unsigned long lastUpdate = 0;
    static int direction = 5;

    if (millis() - lastUpdate > 50) {
        int current = led.getBrightness();
        if (current >= 255 || current <= 0) direction = -direction;
        led.setBrightness(current + direction);

        StaticJsonDocument<256> doc;
        JsonObject obj = doc.to<JsonObject>();
        led.render(obj);
        String output;
        serializeJson(doc, output);
        Serial.println(output);

        lastUpdate = millis();
    }
}

int main() {
    LEDControl myLed("led1", 9);
    setup_led_demo(myLed);
    for (int i = 0; i < 100; ++i) {
        loop_led_demo(myLed);
        delay(10);
    }
    return 0;
}
