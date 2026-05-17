#include <Arduino.h>
#include <semantic_surface/semantic_surface.h>
#include <semantic_surface/led_control.h>
#include <cassert>
#include <iostream>

using namespace SemanticSurface;

void test_slider() {
    Slider slider("s1", 0, 100, 50);
    assert(slider.getID() == "s1");
    assert(slider.getValue() == 50);

    StaticJsonDocument<128> doc;
    doc["value"] = 75;
    slider.update(doc.as<JsonObject>());
    assert(slider.getValue() == 75);

    doc["value"] = 150;
    slider.update(doc.as<JsonObject>());
    assert(slider.getValue() == 100); // Constrained

    std::cout << "test_slider passed" << std::endl;
}

void test_button() {
    Button button("b1");
    assert(!button.isPressed());

    StaticJsonDocument<128> doc;
    doc["pressed"] = true;
    button.update(doc.as<JsonObject>());
    assert(button.isPressed());

    std::cout << "test_button passed" << std::endl;
}

void test_led_control() {
    LEDControl led("l1", 13);
    assert(led.getBrightness() == 0);

    led.toggle();
    assert(led.getBrightness() == 255);

    StaticJsonDocument<128> doc;
    doc["brightness"] = 128;
    led.update(doc.as<JsonObject>());
    assert(led.getBrightness() == 128);

    std::cout << "test_led_control passed" << std::endl;
}

void test_manager() {
    SemanticManager manager;
    manager.addElement(std::make_unique<Slider>("s1", 0, 100, 10));

    manager.handleUpdate("{\"update\":{\"id\":\"s1\",\"value\":20}}");

    String state = manager.getFullState();
    assert(state.find("\"value\":20") != String::npos);

    std::cout << "test_manager passed" << std::endl;
}

int main() {
    test_slider();
    test_button();
    test_led_control();
    test_manager();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
