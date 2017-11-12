/* EmbeddedPrinter class
 * file name: EmbeddedPrinter.h
 * author   : 5km
 * email    : 5km@smslit.cn
 * descr    : geyi embedded printer driver -> EH405
 * */
#ifndef EMBEDDEDPRINTER_H_
#define EMBEDDEDPRINTER_H_
#include <Arduino.h>

enum Alignment {
    Left, Center, Right
};

union CharMode {
    uint8_t value;
    struct {
        uint8_t type:1;
        uint8_t reverse:1;
        uint8_t updown:1;
        uint8_t bold:1;
        uint8_t doubleHeight:1;
        uint8_t doubleWidth:1;
        uint8_t strikeout:1;
        uint8_t res:1;
    };
};

union CNCharMode {
    uint8_t value;
    struct {
        uint8_t :2;
        uint8_t widthTimes:1;
        uint8_t heightTimes:1;
        uint8_t :3;
        uint8_t underline:1;
    };
};

union CharBigTimes {
    uint8_t value;
    struct {
        uint8_t height:4;
        uint8_t width:4;
    };
};

union PrinterStatus {
    uint8_t value;
    struct {
        uint8_t link:1;
        uint8_t :1;
        uint8_t paper:1;
        uint8_t volatage:1;
        uint8_t :2;
        uint8_t temperature:1;
        uint8_t timeout:1;
    };
};

class EmbeddedPrinter {
private:
    HardwareSerial *serial;
public:
    EmbeddedPrinter(HardwareSerial *serialPtr);
    void nextline(void);
    void tab(void);
    void setTab(uint8_t *d, uint8_t num);
    void printblank(uint8_t n);
    void printlines(uint8_t n);
    void on(void);
    void off(void);
    void linespace(void);
    void linespace(uint8_t space);
    void align(Alignment pos);
    void big(bool flag);
    void margin(uint16_t n);
    void cursor(uint16_t n);
    void indent(uint8_t n);
    void setMode(CharMode mode);
    void big(CharBigTimes bigTimes);
    void big(uint8_t times);
    void reverse(bool flag);
    void rotate(bool flag);
    PrinterStatus status(void);
    void overprint(bool flag);
    void bold(bool flag);
    void space(uint8_t n);
    void updown(bool flag);
    void underline(uint8_t flag);
    void usercharset(bool flag);
    void useCN(bool flag);
    void setMode(CNCharMode mode);
    void print(char *str);
    void print(int num);
    void print(double num);
};

#endif
