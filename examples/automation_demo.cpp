#include <Arduino.h>
#include <semantic_surface/semantic_surface.h>
#include <semantic_surface/sensor.h>
#include <semantic_surface/led_control.h>

using namespace SemanticSurface;

// A simple automation concept: Thermostat
// If temperature > threshold, turn on "Cooling" (represented by LED)
class ThermostatSurface : public SemanticElement {
private:
    float threshold;
    bool cooling;
    String id;

public:
    ThermostatSurface(const String& id, float threshold)
        : id(id), threshold(threshold), cooling(false) {}

    String getID() const override { return id; }
    String getType() const override { return "Thermostat"; }

    void check(float currentTemp, LEDControl& cooler) {
        if (currentTemp > threshold) {
            cooling = true;
            cooler.setBrightness(255);
        } else {
            cooling = false;
            cooler.setBrightness(0);
        }
    }

    void update(const JsonObject& data) override {
        if (data.containsKey("threshold")) {
            threshold = data["threshold"].as<float>();
        }
    }

    void render(JsonObject& output) const override {
        output["type"] = getType();
        output["id"] = id;
        output["threshold"] = threshold;
        output["cooling_active"] = cooling;
    }
};

int main() {
    Serial.begin(9600);

    auto tempSensor = std::make_unique<Sensor>("temp1", A0, "C");
    auto coolerLed = std::make_unique<LEDControl>("cooler", 13);
    auto thermostat = std::make_unique<ThermostatSurface>("thermo1", 25.0f);

    SemanticManager manager;
    // We can't easily share pointers with the manager and use them locally without raw pointers or shared_ptr.
    // For this example, let's just use them directly to show the interaction.

    std::cout << "Automation Demo: Thermostat logic" << std::endl;

    float temperatures[] = { 22.0, 24.0, 26.0, 28.0, 23.0 };

    for (float t : temperatures) {
        std::cout << "--- Temp is now " << t << " ---" << std::endl;

        // Update sensor
        StaticJsonDocument<128> sd;
        sd["simulationValue"] = t;
        tempSensor->update(sd.as<JsonObject>());

        // Automation logic
        thermostat->check(t, *coolerLed);

        // Render states
        StaticJsonDocument<512> state;
        JsonArray arr = state.to<JsonArray>();

        JsonObject o1 = arr.createNestedObject();
        tempSensor->render(o1);

        JsonObject o2 = arr.createNestedObject();
        thermostat->render(o2);

        JsonObject o3 = arr.createNestedObject();
        coolerLed->render(o3);

        serializeJson(state, std::cout);
        std::cout << std::endl;
    }

    return 0;
}
