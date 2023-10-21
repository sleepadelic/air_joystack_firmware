#include <Arduino.h>
#include "pinout.h"
#include <Joystick_ESP32S2.h>
#include <EncButton.h>
#include <GyverHC595.h>

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

void setup()
{
    // Joystick settings
    Joystick.begin();
    Joystick.setXAxisRange(0, 600);
    Joystick.setRxAxisRange(0, 600);
    Joystick.setYAxisRange(0, 600);
    Joystick.setRyAxisRange(0, 600);
    Joystick.setZAxisRange(0, 600);
    Joystick.setRzAxisRange(0, 600);
    // Set potentiometers modes
    pinMode(R1_PIN, INPUT);
    pinMode(R2_PIN, INPUT);
    pinMode(R3_PIN, INPUT);
    pinMode(R4_PIN, INPUT);
    pinMode(R5_PIN, INPUT);
    pinMode(R6_PIN, INPUT);

    // enc 2 and 3 have another signal types
    enc_2.setEncType(EB_STEP2);
    enc_3.setEncType(EB_STEP2);

    // led test (init OK)
    led_reg.setAll();
    led_reg.update();
    delay(800);
    led_reg.clearAll();
    led_reg.update();
}
int value = 0;
byte byteToSend = 0;
void loop()
{
    // Buttons and enc read
    button_1.tick();
    button_2.tick();
    button_3.tick();
    enc_0.tick();
    enc_1.tick();
    enc_2.tick();
    enc_3.tick();

    if (button_1.press())
    {
        Serial.println("b1_press");
        Joystick.setButton(0, 1);
    }
    if (button_1.release())
    {
        Joystick.setButton(0, 0);
        Serial.println("b1_release");
    }
    if (button_2.press())
    {
        Joystick.setButton(1, 1);
        Serial.println("b2_press");
    }
    if (button_2.release())
    {
        Joystick.setButton(1, 0);
        Serial.println("b2_release");
    }
    if (button_3.press())
    {
        Joystick.setButton(2, 1);
        Serial.println("b3_press");
    }
    if (button_3.release())
    {
        Joystick.setButton(2, 0);
        Serial.println("b3_release");
    }
    if (enc_0.press())
    {
        led_reg.set(0);
        Serial.println("e0_press");
    }
    if (enc_0.release())
    {
        led_reg.clear(0);
        Serial.println("e0_release");
    }
    if (enc_0.turn())
    {
        Serial.println("e0 turn");
        Serial.println(enc_0.dir());
    }

    if (enc_1.press())
    {
        led_reg.set(1);

        Serial.println("e1_press");
    }
    if (enc_1.release())
    {
        led_reg.clear(1);
        Serial.println("e1_release");
    }
    if (enc_1.turn())
    {
        Serial.println("e1 turn");
        Serial.println(enc_1.dir());
    }

    // Enc 2,3
    if (enc_2.turn())
    {
        // Serial.println("e2 turn");
        // Serial.println(enc_2.dir());
        if (enc_2.dir() == 1)
        {
            Joystick.pressButton(3);
            Joystick.sendState();
            Joystick.releaseButton(3);
            Joystick.sendState();
        }
        else
        {
            Joystick.pressButton(4);
            Joystick.sendState();
            Joystick.releaseButton(4);
            Joystick.sendState();
        }
    }
    if (enc_3.turn())
    {
        // Serial.println("e3 turn");
        // Serial.println(enc_3.dir());
        if (enc_3.dir() == 1)
        {
            Joystick.pressButton(5);
            Joystick.sendState();
            Joystick.releaseButton(5);
            Joystick.sendState();
        }
        else
        {
            Joystick.pressButton(6);
            Joystick.sendState();
            Joystick.releaseButton(6);
            Joystick.sendState();
        }
    }
    if (analogRead(R1_PIN) > 200)
    {
        led_reg.set(7);
    }
    else
    {
        led_reg.clear(7);
    }
    if (analogRead(R2_PIN) > 200)
    {
        led_reg.set(6);
    }
    else
    {
        led_reg.clear(6);
    }
    if (analogRead(R3_PIN) > 200)
    {
        led_reg.set(5);
    }
    else
    {
        led_reg.clear(5);
    }
    if (analogRead(R4_PIN) > 200)
    {
        led_reg.set(4);
    }
    else
    {
        led_reg.clear(4);
    }
    if (analogRead(R5_PIN) > 200)
    {
        led_reg.set(3);
    }
    else
    {
        led_reg.clear(3);
    }
    if (analogRead(R6_PIN) > 200)
    {
        Serial.println(analogRead(R6_PIN) >> 3);
        led_reg.set(2);
    }
    else
    {
        led_reg.clear(2);
    }
    Joystick.setXAxis(analogRead(R6_PIN) >> 3);
    Joystick.setRxAxis(analogRead(R5_PIN) >> 3);
    Joystick.setYAxis(analogRead(R4_PIN) >> 3);
    Joystick.setRyAxis(analogRead(R3_PIN) >> 3);
    Joystick.setZAxis(analogRead(R2_PIN) >> 3);
    Joystick.setRzAxis(analogRead(R1_PIN) >> 3);
    led_reg.update();
}