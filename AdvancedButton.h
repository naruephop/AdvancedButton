#pragma once
#include <Arduino.h>

class AdvancedButton {
public:
  enum class Event : uint8_t {
    None = 0,
    Press,
    Release,
    Click1,
    Click2,
    Click3,
    Long1,
    Long2,
    Long3
  };

  struct Config {
    uint8_t  pin;
    bool     usePullup = true;
    uint32_t debounceMs = 30;
    uint32_t clickGapMs = 350;
    uint32_t long1Ms = 2000;
    uint32_t long2Ms = 5000;
    uint32_t long3Ms = 10000;
  };

  explicit AdvancedButton(const Config& cfg);

  void begin();
  void update(uint32_t nowMs = millis());
  Event getEvent();
  bool  isPressed() const;

  static const char* eventName(Event e);

private:
  Config _cfg;

  bool _lastRead;
  bool _stable;
  bool _prevStable;
  uint32_t _lastChangeMs;

  uint32_t _pressStartMs;
  bool _long1Fired;
  bool _long2Fired;
  bool _long3Fired;

  uint8_t  _clickCount;
  uint32_t _clickDeadlineMs;

  Event _pending;

  bool rawPressed(bool rawLevel) const;
  void setEvent(Event e);
};