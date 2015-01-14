#ifndef MotorizedPot_h
#define MotorizedPot_h

class MotorizedPot
{
    public:
        MotorizedPot(char* name, int feedbackPin, int phasePin, int enablePin);
        void loop();
        void setup();
        int  getValue();
        void sendStatus();
        void gotoValue(int value);
        void onChange(callback fn);

    private:
        int _phasePin;
        int _enablePin;
        int _feedbackPin;

        int _currentValue;
        int _gotoValue;

        bool _busy;
        char* _name;

        callback _onChange;

};

MotorizedPot::MotorizedPot(char* name, int feedbackPin, int phasePin, int enablePin) {

    _name = name;

    _phasePin = phasePin;
    _enablePin = enablePin;
    _feedbackPin = feedbackPin;

    _busy = false;
    _currentValue = 0;

    _onChange = noop;

    pinMode(_feedbackPin, INPUT);
    pinMode(_enablePin, OUTPUT);
    pinMode(_phasePin,  OUTPUT);

}

void MotorizedPot::setup() {}

void MotorizedPot::loop() {
    int value = analogRead(_feedbackPin);
    if (value != _currentValue) {
        _onChange();
        _currentValue = value;
    }
}

void MotorizedPot::onChange(callback fn) {
    _onChange = fn;
}

void MotorizedPot::gotoValue(int value) {

}

int MotorizedPot::getValue() {
    return _currentValue;
}

void MotorizedPot::sendStatus() {
    if (IS_CONNECTED) {
        char value[5] = "";
        itoa(_currentValue, value, 10);
        mqttStatus("motorpot/value", value);
    }
}

#endif
