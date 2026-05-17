#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <semantic_surface/semantic_surface.h>

namespace SemanticSurface {

class ServoMotor : public SemanticElement {
private:
    String id;
    int pin;
    int angle;

public:
    ServoMotor(const String& id, int pin) : id(id), pin(pin), angle(90) {
        // In real Arduino we'd use Servo library, here we simulate
    }

    String getID() const override { return id; }
    String getType() const override { return "ServoMotor"; }

    void setAngle(int deg) {
        angle = constrain(deg, 0, 180);
        // analogWrite simulation or similar
    }

    void update(const JsonObject& data) override {
        if (data.containsKey("angle")) {
            setAngle(data["angle"].as<int>());
        }
    }

    void render(JsonObject& output) const override {
        output["type"] = getType();
        output["id"] = id;
        output["angle"] = angle;
    }
};

} // namespace SemanticSurface

#endif
