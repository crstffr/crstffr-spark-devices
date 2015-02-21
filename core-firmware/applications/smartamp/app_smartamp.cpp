#include "application.h"
#include "../_inc/core-common.h"
#include "../_inc/component-led.h"
#include "../_inc/component-relay.h"
#include "../_inc/component-button.h"
#include "../_inc/component-motorized-pot.h"
#include "../_inc/lib-mcp4725.h"

// ******************************
// Definitions
// ******************************

int volLevel = 0;
int dacValue = 0;

// ******************************
// Function Prototype Definitions
// ******************************

void onConnect();
void onDisconnect();
void onUpBtnPress();
void onDnBtnPress();
void potChanged();
void ledReset();
void setVolume(int val);
void sendVolume();



// ******************************
// Class instantiations
// ******************************

Button upbtn("up-btn", D2, INPUT_PULLUP);
Button dnbtn("dn-btn", D4, INPUT_PULLUP);
MotorizedPot motorpot("motorpot", A2, D5, D6);
Relay rxrelay("rxrelay", D7);
Adafruit_MCP4725 dac(0x62);
LED led("led", A5, A6, A7);

void setup() {

    coreSetup();
    mqttSetup(DEVICE_TYPE_SMARTAMP, onConnect, onDisconnect);

    led.on();
    led.setMaxIntensity(MAX_LED_INTENSITY);
    led.intensity(DEFAULT_LED_INTENSITY);

    upbtn.onUp(ledReset);
    upbtn.onDown(onUpBtnPress);
    upbtn.onPress(ledReset);

    dnbtn.onUp(ledReset);
    dnbtn.onDown(onDnBtnPress);
    dnbtn.onPress(ledReset);

    ledReset();

    rxrelay.open();

}

// ******************************
// Main Loop
// ******************************

void loop() {

    mqttLoop();
    led.loop();
    upbtn.loop();
    dnbtn.loop();



}


// ******************************
// Connection State Callbacks
// ******************************

void onConnect() {
    led.color("blue");
}

void onDisconnect() {
    led.color("red");
}

void onAmpPowerOn() {
    //led.on();
}

void onAmpPowerOff() {
    //led.off();
}


// ******************************
// Button Press Handlers
// ******************************

void ledReset() {
    led.color("blue");
    motorpot.stop();
    motorpot.sendStatus();
}

void onUpBtnPress() {

    startMeasuringDAC();

    led.color("green");
    motorpot.moveUp();

}

void onDnBtnPress() {

    stopMeasuringDAC();

    led.color("red");
    motorpot.moveDown();

}


// ******************************
// Handle Incoming Messages
// ******************************

void mqttCustomMessageHandler(MqttMessage msg) {

    led.mqtt(msg);
    rxrelay.mqtt(msg);
    motorpot.mqtt(msg);

}