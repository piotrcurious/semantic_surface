#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "semantic_surface.h"

namespace SemanticSurface {

class LEDControl : public SemanticElement {
private:
    int pin;
    int brightness;
    String id;

public:
    LEDControl(const String& id, int ledPin) : id(id), pin(ledPin), brightness(0) {
        pinMode(pin, OUTPUT);
    }

    String getID() const override { return id; }
    String getType() const override { return "LEDControl"; }

    void setBrightness(int level) {
        brightness = constrain(level, 0, 255);
        analogWrite(pin, brightness);
    }

    int getBrightness() const {
        return brightness;
    }

    void toggle() {
        if (brightness > 0) {
            setBrightness(0);
        } else {
            setBrightness(255);
        }
    }

    void update(const JsonObject& data) override {
        if (data.containsKey("brightness")) {
            setBrightness(data["brightness"].as<int>());
        }
        if (data.containsKey("toggle") && data["toggle"].as<bool>()) {
            toggle();
        }
    }

    void render(JsonObject& output) const override {
        output["type"] = getType();
        output["id"] = id;
        output["pin"] = pin;
        output["brightness"] = brightness;
        output["status"] = (brightness > 0 ? "ON" : "OFF");
    }
};

} // namespace SemanticSurface

#endif
