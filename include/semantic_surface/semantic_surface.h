#ifndef SEMANTIC_SURFACE_H
#define SEMANTIC_SURFACE_H

#include "Arduino.h"
#include <external/ArduinoJson.h>
#include <vector>
#include <memory>

namespace SemanticSurface {

// Base interface for all semantic elements
class SemanticElement {
public:
    virtual ~SemanticElement() = default;
    virtual String getID() const = 0;
    virtual String getType() const = 0;
    virtual void update(const JsonObject& data) = 0;
    virtual void render(JsonObject& output) const = 0;
};

// GUI Component Base
class GUIComponent : public SemanticElement {
public:
    virtual ~GUIComponent() = default;
};

// Slider Component
class Slider : public GUIComponent {
private:
    String id;
    int value;
    int minValue, maxValue;

public:
    Slider(const String& id, int minValue, int maxValue, int initialValue)
        : id(id), minValue(minValue), maxValue(maxValue), value(initialValue) {}

    String getID() const override { return id; }
    String getType() const override { return "Slider"; }

    void update(const JsonObject& data) override {
        if (data.containsKey("value")) {
            value = constrain(data["value"].as<int>(), minValue, maxValue);
        }
    }

    void render(JsonObject& output) const override {
        output["type"] = getType();
        output["id"] = id;
        output["value"] = value;
        output["min"] = minValue;
        output["max"] = maxValue;
    }

    int getValue() const { return value; }
};

// Button Component
class Button : public GUIComponent {
private:
    String id;
    bool pressed;

public:
    Button(const String& id) : id(id), pressed(false) {}

    String getID() const override { return id; }
    String getType() const override { return "Button"; }

    void update(const JsonObject& data) override {
        if (data.containsKey("pressed")) {
            pressed = data["pressed"].as<bool>();
        }
    }

    void render(JsonObject& output) const override {
        output["type"] = getType();
        output["id"] = id;
        output["pressed"] = pressed;
    }

    bool isPressed() const { return pressed; }
};

// Window Component
class Window : public GUIComponent {
private:
    String id;
    float value;
    int x, y, width, height;
    int screenWidth, screenHeight;

public:
    Window(const String& id, float initialValue, int x, int y, int width, int height, int screenWidth, int screenHeight)
        : id(id), value(initialValue), x(x), y(y), width(width), height(height), screenWidth(screenWidth), screenHeight(screenHeight) {}

    String getID() const override { return id; }
    String getType() const override { return "Window"; }

    void update(const JsonObject& data) override {
        if (data.containsKey("value")) {
            value = data["value"].as<float>();
        }
        if (data.containsKey("x") && data.containsKey("y")) {
            x = constrain(data["x"].as<int>(), 0, screenWidth - width);
            y = constrain(data["y"].as<int>(), 0, screenHeight - height);
        }
    }

    void render(JsonObject& output) const override {
        output["type"] = getType();
        output["id"] = id;
        output["value"] = value;
        JsonObject pos = output.createNestedObject("position");
        pos["x"] = x;
        pos["y"] = y;
        JsonObject size = output.createNestedObject("size");
        size["width"] = width;
        size["height"] = height;
    }
};

// Manager for Semantic Elements
class SemanticManager {
private:
    std::vector<std::unique_ptr<SemanticElement>> elements;

public:
    void addElement(std::unique_ptr<SemanticElement> element) {
        elements.push_back(std::move(element));
    }

    void handleUpdate(const String& input) {
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, input);

        if (!error && doc.containsKey("update")) {
            JsonObject updateData = doc["update"];
            String targetID = updateData["id"];
            for (auto& element : elements) {
                if (element->getID() == targetID) {
                    element->update(updateData);
                    break;
                }
            }
        }
    }

    void renderAll(JsonArray& output) const {
        for (const auto& element : elements) {
            JsonObject obj = output.createNestedObject();
            element->render(obj);
        }
    }

    String getFullState() const {
        StaticJsonDocument<1024> doc;
        JsonArray array = doc.createNestedArray("elements");
        renderAll(array);
        String output;
        serializeJson(doc, output);
        return output;
    }
};

} // namespace SemanticSurface

#endif
