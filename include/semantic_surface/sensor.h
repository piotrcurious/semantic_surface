#ifndef SENSOR_H
#define SENSOR_H

#include <semantic_surface/semantic_surface.h>

namespace SemanticSurface {

class Sensor : public SemanticElement {
private:
    String id;
    int pin;
    float lastValue;
    String unit;

public:
    Sensor(const String& id, int pin, const String& unit = "")
        : id(id), pin(pin), lastValue(0.0f), unit(unit) {
        pinMode(pin, INPUT);
    }

    String getID() const override { return id; }
    String getType() const override { return "Sensor"; }

    float read() {
        // In mock, this returns a fixed value, but could be extended
        lastValue = (float)analogRead(pin);
        return lastValue;
    }

    void update(const JsonObject& data) override {
        // Sensors might not have many controllable properties,
        // but maybe we can set simulation values or thresholds
        if (data.containsKey("simulationValue")) {
            lastValue = data["simulationValue"].as<float>();
        }
    }

    void render(JsonObject& output) const override {
        output["type"] = getType();
        output["id"] = id;
        output["value"] = lastValue;
        if (unit.length() > 0) {
            output["unit"] = unit;
        }
    }
};

} // namespace SemanticSurface

#endif
