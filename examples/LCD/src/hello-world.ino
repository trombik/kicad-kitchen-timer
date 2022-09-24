#include <Wire.h>

/*
 * hd44780
 * https://github.com/duinoWitchery/hd44780
 */

/* include hd44780 driver */
#include <hd44780.h>

/* include the transport, I2C driver */
#include <hd44780ioClass/hd44780_I2Cexp.h>

/* the size of LCD: 2 lines of 16 characters */
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

/* create lcd instance */
hd44780_I2Cexp lcd;

void
setup()
{
    int err;

    Serial.begin(9600);
    Serial.println("Started");

    err = lcd.begin(LCD_COLS, LCD_ROWS);
    if (err != 0) {
        Serial.println("begin(): failed");
        goto fail;
    }

    /* clear the display */
    lcd.clear();

    /* set the cursor position */
    lcd.setCursor(0, 0);
    lcd.print("Hello world");
    delay(1000);
    lcd.clear();
    return;

fail:

    /* XXX hd44780::fatalError() never returns */
    hd44780::fatalError(err);
}

void
loop()
{
    /* unsigned int: between 0 and 65,535 (0xffff, 2 bytes) */
    unsigned int uptime_min;
    unsigned int uptime_sec;
    unsigned int uptime_ms;

    /* unsigned int: between 0 and 4,294,967,295 (0xffffffff, 4 bytes) */
    unsigned long now_ms;

    /* possible text on the display: 1234:02:01 + null */
    const size_t time_str_len = 4 + 1 + 2 + 1 + 2 + 1;

    /* char: between 0 to 255 (0xff) */
    char time_str[time_str_len];

    /* millis() returns the number of milliseconds passed since the Arduino
     * board began running the current program. the return value is unsigned
     * long.
     */
    now_ms = millis();
    uptime_min = (now_ms / 1000) / 60;

    /* "%" returns the remainder when one integer is divided by another.
     *
     * 10 % 3 = 1
     * 5 % 3 = 2
     */
    uptime_sec = (now_ms / 1000) % 60;
    uptime_ms  = now_ms % 1000;

    /* create the text to display. see man 3 snprintf */
    snprintf(time_str, time_str_len, "%4i:%02i:%02i", uptime_min, uptime_sec, uptime_ms);

    /* set the cursor position */
    lcd.setCursor(0, 0);

    /* print the text on the display */
    lcd.print(time_str);
}
