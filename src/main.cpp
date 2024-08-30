#include <Arduino.h>
#include "pinout.h"
#include <Joystick_ESP32S2.h>
#include <EncButton.h>
#include <GyverHC595.h>
#include <GyverFilters.h>
#include <USBHIDKeyboard.h>
// #define debugToSerial true // Send debug into Serial

#ifdef debugToSerial
#define debugPrint(x) Serial.println(x)
#else
#define debugPrint(x)
#endif

#define KeyboardPressDelayMS 5
#define JoystickPressDelayMS 15

#define KeyboardMode 0
#define JoystickMode 1
uint8_t outputMode = KeyboardMode;

USBHIDKeyboard usbKeyboard;
// Init
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_MULTI_AXIS, 15, 0,
                   true, true, true, true, true, true,
                   false, false, false, false, false);
Button button_1 = Button(B1_PIN);
Button button_2 = Button(B2_PIN);
Button button_3 = Button(B3_PIN);
EncButton enc_0 = EncButton(E02_PIN, E01_PIN, E0B_PIN, INPUT_PULLUP);
EncButton enc_1 = EncButton(E12_PIN, E11_PIN, E1B_PIN, INPUT_PULLUP);
Encoder enc_2 = Encoder(E21_PIN, E22_PIN, INPUT_PULLUP);
Encoder enc_3 = Encoder(E31_PIN, E32_PIN, INPUT_PULLUP);

GyverHC595<1, HC_PINS> led_reg(latchPin, dataPin, clockPin);

// Encoder Interrupts
IRAM_ATTR void isr_enc0()
{
    enc_0.tickISR();
}

IRAM_ATTR void isr_enc1()
{
    enc_1.tickISR();
}

IRAM_ATTR void isr_enc2()
{
    enc_2.tickISR();
}

IRAM_ATTR void isr_enc3()
{
    enc_3.tickISR();
}

void setup()
{
    usbKeyboard.begin();
    debugPrint("Initialisation Started...");
    // Joystick settings
    Joystick.begin(false);
    Joystick.setXAxisRange(0, 1200);
    Joystick.setRxAxisRange(0, 1200);
    Joystick.setYAxisRange(0, 1200);
    Joystick.setRyAxisRange(0, 1200);
    Joystick.setZAxisRange(0, 1200);
    Joystick.setRzAxisRange(0, 1200);

    // Set potentiometers modes
    pinMode(R1_PIN, INPUT);
    pinMode(R2_PIN, INPUT);
    pinMode(R3_PIN, INPUT);
    pinMode(R4_PIN, INPUT);
    pinMode(R5_PIN, INPUT);
    pinMode(R6_PIN, INPUT);
    pinMode(modeSwitchPin, INPUT_PULLUP);

    enc_0.setFastTimeout(255);

    // enc 2 and 3 have another signal types
    enc_2.setEncType(EB_STEP2);
    enc_3.setEncType(EB_STEP2);

    // led test (init OK)s
    led_reg.setAll();
    led_reg.update();
    delay(1000);
    led_reg.clearAll();
    led_reg.update();

    attachInterrupt(E02_PIN, isr_enc0, CHANGE);
    attachInterrupt(E01_PIN, isr_enc0, CHANGE);
    enc_0.setEncISR(true);

    attachInterrupt(E12_PIN, isr_enc1, CHANGE);
    attachInterrupt(E11_PIN, isr_enc1, CHANGE);
    enc_1.setEncISR(true);

    attachInterrupt(E22_PIN, isr_enc2, CHANGE);
    attachInterrupt(E21_PIN, isr_enc2, CHANGE);
    enc_2.setEncISR(true);

    attachInterrupt(E32_PIN, isr_enc3, CHANGE);
    attachInterrupt(E31_PIN, isr_enc3, CHANGE);
    enc_3.setEncISR(true);

    debugPrint("Initialisation complete!\n");
}
int value = 0;
byte byteToSend = 0;

void readAxies()
{
    if (analogRead(R1_PIN) > 250)
    {
        led_reg.set(7);
    }
    else
    {
        led_reg.clear(7);
    }
    if (analogRead(R2_PIN) > 250)
    {
        led_reg.set(6);
    }
    else
    {
        led_reg.clear(6);
    }
    if (analogRead(R3_PIN) > 250)
    {
        led_reg.set(5);
    }
    else
    {
        led_reg.clear(5);
    }
    if (analogRead(R4_PIN) > 250)
    {
        led_reg.set(4);
    }
    else
    {
        led_reg.clear(4);
    }
    if (analogRead(R5_PIN) > 250)
    {
        led_reg.set(3);
    }
    else
    {
        led_reg.clear(3);
    }
    if (analogRead(R6_PIN) > 250)
    {
        led_reg.set(2);
    }
    else
    {
        led_reg.clear(2);
    }

    Joystick.setXAxis(analogRead(R6_PIN) >> 2);
    Joystick.setRxAxis(analogRead(R5_PIN) >> 2);
    Joystick.setYAxis(analogRead(R4_PIN) >> 2);
    Joystick.setRyAxis(analogRead(R3_PIN) >> 2);
    Joystick.setZAxis(analogRead(R2_PIN) >> 2);
    Joystick.setRzAxis(analogRead(R1_PIN) >> 2);
    led_reg.update();
    Joystick.sendState();
}

void readButtons()
{
    if (button_1.press())
    {
        debugPrint("b1_press");
        Joystick.setButton(0, 1);
    }
    if (button_1.release())
    {
        Joystick.setButton(0, 0);
        debugPrint("b1_release");
    }
    if (button_2.press())
    {
        Joystick.setButton(1, 1);
        debugPrint("b2_press");
    }
    if (button_2.release())
    {
        Joystick.setButton(1, 0);
        debugPrint("b2_release");
    }
    if (button_3.press())
    {
        Joystick.setButton(2, 1);
        debugPrint("b3_press");
    }
    if (button_3.release())
    {
        Joystick.setButton(2, 0);
        debugPrint("b3_release");
    }
    Joystick.sendState();
}

void readEncButtons()
{
    if (enc_0.press())
    {
        led_reg.set(0);
        led_reg.update();
        Joystick.setButton(3, 1);
        debugPrint("e0_press");
    }
    if (enc_0.release())
    {
        led_reg.clear(0);
        led_reg.update();
        Joystick.setButton(3, 0);
        debugPrint("e0_release");
    }

    if (enc_1.press())
    {
        led_reg.set(1);
        led_reg.update();
        Joystick.setButton(4, 1);
        debugPrint("e1_press");
    }
    if (enc_1.release())
    {
        led_reg.clear(1);
        led_reg.update();
        Joystick.setButton(4, 0);
        debugPrint("e1_release");
    }

    Joystick.sendState();
}

void readEncRotations()
{
    // Read enc_0
    if (enc_0.dir() == 1 && enc_0.turn())
    {
        Joystick.pressButton(5);
        Joystick.sendState();
        vTaskDelay(pdMS_TO_TICKS(JoystickPressDelayMS));
        Joystick.releaseButton(5);
        Joystick.sendState();
        debugPrint("e0 turn R");
        debugPrint(enc_0.counter);
    }
    else if (enc_0.dir() == -1 && enc_0.turn())
    {
        Joystick.pressButton(6);
        Joystick.sendState();
        vTaskDelay(pdMS_TO_TICKS(JoystickPressDelayMS));
        Joystick.releaseButton(6);
        Joystick.sendState();
        debugPrint("e0 turn L");
        debugPrint(enc_0.counter);
    }

    // Read enc_1
    if (enc_1.dir() == 1 && enc_1.turn())
    {
        Joystick.pressButton(7);
        Joystick.sendState();
        vTaskDelay(pdMS_TO_TICKS(JoystickPressDelayMS));
        Joystick.releaseButton(7);
        Joystick.sendState();
        debugPrint("e1 turn R");
        debugPrint(enc_1.counter);
    }
    else if (enc_1.dir() == -1 && enc_1.turn())
    {
        Joystick.pressButton(8);
        Joystick.sendState();
        vTaskDelay(pdMS_TO_TICKS(JoystickPressDelayMS));
        Joystick.releaseButton(8);
        Joystick.sendState();
        debugPrint("e1 turn L");
        debugPrint(enc_1.counter);
    }

    // Read enc_2
    if (enc_2.dir() == 1 && enc_2.turn())
    {
        Joystick.pressButton(9);
        Joystick.sendState();
        vTaskDelay(pdMS_TO_TICKS(JoystickPressDelayMS));
        Joystick.releaseButton(9);
        Joystick.sendState();
        debugPrint("e2 turn R");
        debugPrint(enc_2.counter);
    }
    else if (enc_2.dir() == -1 && enc_2.turn())
    {
        Joystick.pressButton(10);
        Joystick.sendState();
        vTaskDelay(pdMS_TO_TICKS(JoystickPressDelayMS));
        Joystick.releaseButton(10);
        Joystick.sendState();
        debugPrint("e2 turn L");
        debugPrint(enc_2.counter);
    }

    // Read enc_3
    if (enc_3.dir() == 1 && enc_3.turn())
    {
        Joystick.pressButton(11);
        Joystick.sendState();
        vTaskDelay(pdMS_TO_TICKS(JoystickPressDelayMS));
        Joystick.releaseButton(11);
        Joystick.sendState();
        debugPrint("e3 turn R");
        debugPrint(enc_3.counter);
    }
    else if (enc_3.dir() == -1 && enc_3.turn())
    {
        Joystick.pressButton(12);
        Joystick.sendState();
        vTaskDelay(pdMS_TO_TICKS(JoystickPressDelayMS));
        Joystick.releaseButton(12);
        Joystick.sendState();
        debugPrint("e3 turn L");
        debugPrint(enc_3.counter);
    }
}

void readEncKeyboard()
{
    // Read enc_0

    if (enc_0.turnH())
    {
        led_reg.set(0);
        led_reg.update();
        if (enc_0.dir() == 1)
        {
            usbKeyboard.press(KEY_LEFT_ALT);
            usbKeyboard.press(KEY_F14);
            delay(KeyboardPressDelayMS);
            usbKeyboard.release(KEY_F14);
            usbKeyboard.release(KEY_LEFT_ALT);
            debugPrint("e0 turn R");
            debugPrint(enc_0.counter);
        }
        else if (enc_0.dir() == -1)
        {
            usbKeyboard.press(KEY_LEFT_ALT);
            usbKeyboard.press(KEY_F13);
            delay(KeyboardPressDelayMS);
            usbKeyboard.release(KEY_F13);
            usbKeyboard.release(KEY_LEFT_ALT);
            debugPrint("e0 turn L");
            debugPrint(enc_0.counter);
        }
    }
    else
    {
        led_reg.clear(0);
        led_reg.update();
        if (enc_0.turn())
        {
            if (enc_0.dir() == 1)
            {
                usbKeyboard.press(KEY_F14);
                delay(KeyboardPressDelayMS);
                usbKeyboard.release(KEY_F14);
                debugPrint("e0 turn R");
                debugPrint(enc_0.counter);
            }
            else if (enc_0.dir() == -1)
            {
                usbKeyboard.press(KEY_F13);
                delay(KeyboardPressDelayMS);
                usbKeyboard.release(KEY_F13);
                debugPrint(enc_0.counter);
            }
        }
    }

    // Read enc_1
    if (enc_1.turnH())
    {
        led_reg.set(1);
        led_reg.update();
        if (enc_1.dir() == 1)
        {
            usbKeyboard.press(KEY_LEFT_ALT);
            usbKeyboard.press(KEY_F16);
            delay(KeyboardPressDelayMS);
            usbKeyboard.release(KEY_F16);
            usbKeyboard.release(KEY_LEFT_ALT);
            debugPrint("e1 turnH R");
            debugPrint(enc_1.counter);
        }
        else if (enc_1.dir() == -1)
        {
            usbKeyboard.press(KEY_LEFT_ALT);
            usbKeyboard.press(KEY_F15);
            delay(KeyboardPressDelayMS);
            usbKeyboard.release(KEY_F15);
            usbKeyboard.release(KEY_LEFT_ALT);
            debugPrint("e1 turnH L");
            debugPrint(enc_1.counter);
        }
    }
    else
    {
        led_reg.clear(1);
        led_reg.update();
        if (enc_1.turn())
        {
            if (enc_1.dir() == 1)
            {
                usbKeyboard.press(KEY_F16);
                delay(KeyboardPressDelayMS);
                usbKeyboard.release(KEY_F16);
                debugPrint("e1 turn R");
                debugPrint(enc_1.counter);
            }
            else if (enc_1.dir() == -1)
            {
                usbKeyboard.press(KEY_F15);
                delay(KeyboardPressDelayMS);
                usbKeyboard.release(KEY_F15);
                debugPrint(enc_1.counter);
            }
        }
    }

    // Read enc_2
    if (enc_2.dir() == 1 && enc_2.turn())
    {
        usbKeyboard.press(KEY_F18);
        delay(KeyboardPressDelayMS);
        usbKeyboard.release(KEY_F18);
        debugPrint("e2 turn R");
        debugPrint(enc_2.counter);
    }
    else if (enc_2.dir() == -1 && enc_2.turn())
    {
        usbKeyboard.press(KEY_F17);
        delay(KeyboardPressDelayMS);
        usbKeyboard.release(KEY_F17);
        debugPrint("e2 turn L");
        debugPrint(enc_2.counter);
    }

    // Read enc_3
    if (enc_3.dir() == 1 && enc_3.turn())
    {
        usbKeyboard.press(KEY_F20);
        delay(KeyboardPressDelayMS);
        usbKeyboard.release(KEY_F20);
        debugPrint("e3 turn R");
        debugPrint(enc_3.counter);
    }
    else if (enc_3.dir() == -1 && enc_3.turn())
    {
        usbKeyboard.press(KEY_F19);
        delay(KeyboardPressDelayMS);
        usbKeyboard.release(KEY_F19);
        Joystick.sendState();
        debugPrint("e3 turn L");
        debugPrint(enc_3.counter);
    }
}

void tickAll()
{
    button_1.tick();
    button_2.tick();
    button_3.tick();
    enc_0.tick();
    enc_1.tick();
    enc_2.tick();
    enc_3.tick();
}

void modeCheck()
{
    static uint8_t prevousModeSwitchState = -1;
    if (digitalRead(modeSwitchPin) != prevousModeSwitchState)
    {
        if (digitalRead(modeSwitchPin) == 0)
        {
            outputMode = KeyboardMode;
            led_reg.clearAll();
            led_reg.update();
            delay(200);
            led_reg.set(7);
            led_reg.update();
            delay(200);
            led_reg.clearAll();
            led_reg.update();
            delay(200);
            led_reg.set(7);
            led_reg.update();
            delay(200);
            led_reg.clearAll();
            led_reg.update();
        }
        else
        {
            outputMode = JoystickMode;
            led_reg.clearAll();
            led_reg.update();
            delay(200);
            led_reg.set(2);
            led_reg.update();
            delay(200);
            led_reg.clearAll();
            led_reg.update();
            delay(200);
            led_reg.set(2);
            led_reg.update();
            delay(200);
            led_reg.clearAll();
            led_reg.update();
        }
        prevousModeSwitchState = digitalRead(modeSwitchPin);
    }
}

void loop()
{
    tickAll();
    modeCheck();
    if (outputMode == JoystickMode)
    {
        readEncButtons();
        readEncRotations();
    }
    if (outputMode == KeyboardMode)
    {
        readEncKeyboard();
    }
    readAxies();
    readButtons();
}