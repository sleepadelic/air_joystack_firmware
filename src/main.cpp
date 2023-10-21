#include <Arduino.h>
#include "pinout.h"
#include <Joystick_ESP32S2.h>
#include <EncButton.h>

// Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
//                    JOYSTICK_TYPE_MULTI_AXIS, 32, 0,
//                    true, true, false, false, false, false,
//                    true, true, false, false, false);
Button button_1 = Button(B1_PIN);
Button button_2 = Button(B2_PIN);
Button button_3 = Button(B3_PIN);
EncButton enc_0 = EncButton(E02_PIN, E01_PIN, E0B_PIN, INPUT_PULLUP);
EncButton enc_1 = EncButton(E12_PIN, E11_PIN, E1B_PIN, INPUT_PULLUP);
Encoder enc_2 = Encoder(E21_PIN, E22_PIN, INPUT_PULLUP);
Encoder enc_3 = Encoder(E31_PIN, E32_PIN, INPUT_PULLUP);

void setup()
{
    // pinMode(E02_PIN, INPUT_PULLUP);
    // pinMode(E01_PIN, INPUT_PULLUP);
    // pinMode(E0B_PIN, INPUT_PULLUP);
    // pinMode(LED_BUILTIN, OUTPUT);
    // pinMode(latchPin, OUTPUT);
    // pinMode(clockPin, OUTPUT);
    // pinMode(dataPin, OUTPUT);
    // ставим HIGH на "защёлку", чтобы регистр не принимал сигнал
    pinMode(R1_PIN, INPUT);
    pinMode(R2_PIN, INPUT);
    pinMode(R3_PIN, INPUT);
    pinMode(R4_PIN, INPUT);
    pinMode(R5_PIN, INPUT);
    pinMode(R6_PIN, INPUT);

    // enc_0.setEncType(EB_STEP1);
    enc_2.setEncType(EB_STEP2);
    enc_3.setEncType(EB_STEP2);
    digitalWrite(latchPin, HIGH);
}
int value = 0;
byte byteToSend = 0;
void loop()
{
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
    }
    if (button_1.release())
    {
        Serial.println("b1_release");
    }
    if (button_2.press())
    {
        Serial.println("b2_press");
    }
    if (button_2.release())
    {
        Serial.println("b2_release");
    }
    if (button_3.press())
    {
        Serial.println("b3_press");
    }
    if (button_3.release())
    {
        Serial.println("b3_release");
    }
    if (enc_0.press())
    {
        Serial.println("e0_press");
    }
    if (enc_0.release())
    {
        Serial.println("e0_release");
    }
    if (enc_0.turn())
    {
        Serial.println("e0 turn");
        Serial.println(enc_0.dir());
    }

    if (enc_1.press())
    {
        Serial.println("e1_press");
    }
    if (enc_1.release())
    {
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
        Serial.println("e2 turn");
        Serial.println(enc_2.dir());
    }
    if (enc_3.turn())
    {
        Serial.println("e3 turn");
        Serial.println(enc_3.dir());
    }
    // if (enc_3.right())
    // {
    //     Serial.println("e3_right");
    // }

    //     delay(100);
    //     for (byte bitPos = 0; bitPos < 8; bitPos++) {
    //       byteToSend = 0; // обнуляем байт, чтобы выключить светодиод
    //       bitWrite(byteToSend, bitPos, HIGH);
    //       digitalWrite(latchPin, LOW);                                        // ставим LOW на "защёлку"
    //       shiftOut(dataPin, clockPin, LSBFIRST, byteToSend);
    //       digitalWrite(latchPin, HIGH);                                       // ставим HIGH на "защёлку"
    //       delay(100);
    //    }
    //    for (byte bitPos = 0; bitPos < 8; bitPos++) {
    //       byteToSend = 0; // обнуляем байт, чтобы выключить светодиод
    //       bitWrite(byteToSend, bitPos, HIGH);
    //       digitalWrite(latchPin, LOW);                                        // ставим LOW на "защёлку"
    //       shiftOut(dataPin, clockPin, MSBFIRST, byteToSend);
    //       digitalWrite(latchPin, HIGH);                                       // ставим HIGH на "защёлку"
    //       delay(100);
    //    }
}