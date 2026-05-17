CXX = g++
CXXFLAGS = -I./include -I./include/external -I./mock_arduino -std=c++14

all: examples tests

examples: led_demo gui_demo sensor_hub_demo automation_demo

led_demo: examples/led_demo.cpp mock_arduino/Arduino.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

gui_demo: examples/gui_demo.cpp mock_arduino/Arduino.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

sensor_hub_demo: examples/sensor_hub_demo.cpp mock_arduino/Arduino.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

automation_demo: examples/automation_demo.cpp mock_arduino/Arduino.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

tests: test_suite
	./test_suite

test_suite: tests/test_semantic_surface.cpp mock_arduino/Arduino.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f led_demo gui_demo sensor_hub_demo automation_demo test_suite

.PHONY: all examples tests clean
