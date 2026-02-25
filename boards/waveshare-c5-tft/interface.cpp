#include "core/powerSave.h"
#include "core/utils.h"
#include <interface.h>

/***************************************************************************************
** Function name: _setup_gpio()
** Location: main.cpp
** Description:   initial setup for the device
***************************************************************************************/
void _setup_gpio() {

    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    pinMode(TFT_MOSI, OUTPUT);
    digitalWrite(TFT_MOSI, HIGH);
    pinMode(TFT_SCLK, OUTPUT);

    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
    pinMode(TFT_RST, OUTPUT);
    pinMode(TFT_DC, OUTPUT);
    digitalWrite(TFT_DC, HIGH);

#ifdef HAS_3_BUTTONS
    pinMode(UP_BTN, INPUT_PULLUP); // Sets the power btn as an INPUT
    pinMode(SEL_BTN, INPUT_PULLUP);
    pinMode(DW_BTN, INPUT_PULLUP);
#endif
    pinMode(NRF24_SS_PIN, OUTPUT);
    pinMode(CC1101_SS_PIN, OUTPUT);
    pinMode(SDCARD_CS, OUTPUT);
    pinMode(W5500_SS_PIN, OUTPUT);
    pinMode(TFT_CS, OUTPUT);

    digitalWrite(NRF24_SS_PIN, HIGH);
    digitalWrite(CC1101_SS_PIN, HIGH);
    digitalWrite(SDCARD_CS, HIGH);
    digitalWrite(W5500_SS_PIN, HIGH);
    digitalWrite(TFT_CS, HIGH);
}
/***************************************************************************************
** Function name: _post_setup_gpio()
** Location: main.cpp
** Description:   second stage gpio setup to make a few functions work
***************************************************************************************/
void _post_setup_gpio() {}

/***************************************************************************************
** Function name: getBattery()
** location: display.cpp
** Description:   Delivers the battery value from 1-100
***************************************************************************************/
int getBattery() { return 0; }

/***************************************************************************************
** Function name: isCharging()
** Description:   Default implementation that returns false
***************************************************************************************/
bool isCharging() { return false; }

/*********************************************************************
** Function: setBrightness
** location: settings.cpp
** set brightness value
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    if (brightval == 0) {
        analogWrite(TFT_BL, brightval);
    } else {
        int bl = MINBRIGHT + round(((255 - MINBRIGHT) * brightval / 100));
        analogWrite(TFT_BL, bl);
    }
}

/*********************************************************************
** Function: InputHandler
** Handles the variables PrevPress, NextPress, SelPress, AnyKeyPress and EscPress
**********************************************************************/
void InputHandler(void) {
    static unsigned long tm = 0;
    if (millis() - tm < 200 && !LongPress) return;

#ifdef HAS_3_BUTTONS
    bool upPressed = (digitalRead(UP_BTN) == LOW);
    bool selPressed = (digitalRead(SEL_BTN) == LOW);
    bool dwPressed = (digitalRead(DW_BTN) == LOW);

    bool anyPressed = upPressed || selPressed || dwPressed;
    if (anyPressed) tm = millis();
    if (anyPressed && wakeUpScreen()) return;

    AnyKeyPress = anyPressed;
    PrevPress = upPressed;
    EscPress = upPressed && dwPressed;
    NextPress = dwPressed;
    SelPress = selPressed;
#endif
}

/*********************************************************************
** Function: powerOff
** location: mykeyboard.cpp
** Turns off the device (or try to)
**********************************************************************/
void powerOff() {
    esp_sleep_enable_ext1_wakeup(
        (1ULL << UP_BTN) | (1ULL << DW_BTN),
        ESP_EXT1_WAKEUP_ANY_LOW
    );
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: checkReboot
** location: mykeyboard.cpp
** Btn logic to turnoff the device (name is odd btw)
**********************************************************************/
void checkReboot() {
    if (digitalRead(DW_BTN) == BTN_ACT &&
        digitalRead(UP_BTN) == BTN_ACT) {

        uint32_t start = millis();
        int count = 0;

        while (digitalRead(DW_BTN) == BTN_ACT &&
               digitalRead(UP_BTN) == BTN_ACT) {

            if (millis() - start > 500) {
                count = (millis() - start) / 1000 + 1;

                tft.setTextSize(1);
                tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);

                if (count <= 3) {
                    tft.drawCentreString(
                        "PWR OFF IN " + String(count) + "/3",
                        tftWidth / 2,
                        12,
                        1
                    );
                } else {
                    tft.fillScreen(bruceConfig.bgColor);
                    delay(200);
                    powerOff();
                }
            }
            delay(10);
        }

        drawStatusBar();
    }
}
