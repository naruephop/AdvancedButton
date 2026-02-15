#include "AdvancedButton.h"

AdvancedButton::AdvancedButton(const Config& cfg)
  : _cfg(cfg),
    _lastRead(true),
    _stable(true),
    _prevStable(true),
    _lastChangeMs(0),
    _pressStartMs(0),
    _long1Fired(false),
    _long2Fired(false),
    _long3Fired(false),
    _clickCount(0),
    _clickDeadlineMs(0),
    _pending(Event::None) {}

void AdvancedButton::begin() {
  pinMode(_cfg.pin, _cfg.usePullup ? INPUT_PULLUP : INPUT);
  _lastRead = digitalRead(_cfg.pin);
  _stable = _lastRead;
  _prevStable = _stable;
  _lastChangeMs = millis();
}

bool AdvancedButton::rawPressed(bool rawLevel) const {
  return _cfg.usePullup ? (rawLevel == LOW) : (rawLevel == HIGH);
}

bool AdvancedButton::isPressed() const {
  return rawPressed(_stable);
}

void AdvancedButton::setEvent(Event e) {
  if (_pending == Event::None)
    _pending = e;
}

AdvancedButton::Event AdvancedButton::getEvent() {
  Event e = _pending;
  _pending = Event::None;
  return e;
}

void AdvancedButton::update(uint32_t nowMs) {
  bool raw = digitalRead(_cfg.pin);

  if (raw != _lastRead) {
    _lastRead = raw;
    _lastChangeMs = nowMs;
  }

  if (nowMs - _lastChangeMs >= _cfg.debounceMs) {
    _stable = _lastRead;
  }

  if (_stable != _prevStable) {
    _prevStable = _stable;

    if (isPressed()) {
      _pressStartMs = nowMs;
      _long1Fired = _long2Fired = _long3Fired = false;
      setEvent(Event::Press);
    } else {
      setEvent(Event::Release);

      if (!_long1Fired) {
        _clickCount++;
        _clickDeadlineMs = nowMs + _cfg.clickGapMs;
      } else {
        _clickCount = 0;
        _clickDeadlineMs = 0;
      }
    }
  }

  if (isPressed()) {
    uint32_t held = nowMs - _pressStartMs;

    if (!_long1Fired && held >= _cfg.long1Ms) { _long1Fired = true; setEvent(Event::Long1); }
    if (!_long2Fired && held >= _cfg.long2Ms) { _long2Fired = true; setEvent(Event::Long2); }
    if (!_long3Fired && held >= _cfg.long3Ms) { _long3Fired = true; setEvent(Event::Long3); }

    if (_long1Fired) {
      _clickCount = 0;
      _clickDeadlineMs = 0;
    }
  }

  if (!isPressed() && _clickDeadlineMs != 0 && nowMs >= _clickDeadlineMs) {
    if (_clickCount == 1) setEvent(Event::Click1);
    else if (_clickCount == 2) setEvent(Event::Click2);
    else if (_clickCount >= 3) setEvent(Event::Click3);

    _clickCount = 0;
    _clickDeadlineMs = 0;
  }
}

const char* AdvancedButton::eventName(Event e) {
  switch (e) {
    case Event::Press:   return "Press";
    case Event::Release: return "Release";
    case Event::Click1:  return "Click1";
    case Event::Click2:  return "Click2";
    case Event::Click3:  return "Click3";
    case Event::Long1:   return "Long1";
    case Event::Long2:   return "Long2";
    case Event::Long3:   return "Long3";
    default:             return "None";
  }
}