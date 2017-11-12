/* EmbeddedPrinter class
 * file name: EmbeddedPrinter.cpp
 * author   : 5km
 * email    : 5km@smslit.cn
 * descr    : geyi embedded printer driver -> EH405
 * */
#include "EmbeddedPrinter.h"

/* init method
 * with the point of HardwareSerial
 * */
EmbeddedPrinter::EmbeddedPrinter(HardwareSerial *serialPtr) {
    serial = serialPtr;
}

/* print LF and CR */
void EmbeddedPrinter::nextline(void) {
    serial->write(0x0A);
    serial->write(0x0D);
}

/* print tab */
void EmbeddedPrinter::tab(void) {
    serial->write(0x09);
}

/* set tab
 * the unit of d: 8 point
 * 1 <= d[i] <= 46; 1 <= k <= 16
 * */
void EmbeddedPrinter::setTab(uint8_t *d, uint8_t k) {
    if(k > 16) k = 16;
    for(int i = 0; i < k; i++) {
        d[i] = constrain(d[i], 1, 46);
    }
    serial->write(0x1B);
    serial->write(0x44);
    serial->write(d, k);
    serial->write(0x00);
}

/* print cache chars and pull paper with vertical blank
 * unit is one pointi(0.125mm)
 * 1 <= n <= 255
 * */
void EmbeddedPrinter::printblank(uint8_t n) {
    serial->write(0x1B);
    serial->write(0x4A);
    serial->write(n);
}

/* print cache chars and pull paper with lines
 * unit is one line
 * 1 <= n <= 255
 * */
void EmbeddedPrinter::printlines(uint8_t n) {
    serial->write(0x1B);
    serial->write(0x64);
    serial->write(n);
}

/* make printer online */
void EmbeddedPrinter::on(void) {
    serial->write(0x1B);
    serial->write(0x3d);
    serial->write(0x01);
}

/* make printer offline */
void EmbeddedPrinter::off(void) {
    serial->write(0x1B);
    serial->write(0x3d);
    serial->write(0x00);
}

/* set default line space: 30 point -> 30*0.125mm */
void EmbeddedPrinter::linespace(void) {
    serial->write(0x1B);
    serial->write(0x32);
}

/* set line space 
 * unit: 0.125mm
 * 0<= space <= 255
 * */
void EmbeddedPrinter::linespace(uint8_t space) {
    serial->write(0x1B);
    serial->write(0x32);
    serial->write(space);
}

/* alignment:
 * Left -> 0, 48
 * Center -> 1, 49
 * Right -> 2, 50
 * */
void EmbeddedPrinter::align(Alignment pos) {
    serial->write(0x1B);
    serial->write(0x61);
    serial->write((uint8_t)pos);
}

/* 2 times height and width of font */
void EmbeddedPrinter::big(bool flag) {
    uint8_t cmd = flag ? 0x0E : 0x14;
    serial->write(0x1B);
    serial->write(cmd);
}

/* margin left: n * 0.125mm */
void EmbeddedPrinter::margin(uint16_t n) {
    serial->write(0x1D);
    serial->write(0x4C);
    serial->write(lowByte(n));
    serial->write(highByte(n));
}

/* go to position on one line: n * 0.125mm */
void EmbeddedPrinter::cursor(uint16_t n) {
    serial->write(0x1B);
    serial->write(0x24);
    serial->write(lowByte(n));
    serial->write(highByte(n));
}

/* space left: 0 <= n <= 47 
 * unit: char
 * */
void EmbeddedPrinter::indent(uint8_t n) {
    serial->write(0x1B);
    serial->write(0x42);
    serial->write(constrain(n, 0, 47));
}

/* set the char type */
void EmbeddedPrinter::setMode(CharMode mode) {
    serial->write(0x1B);
    serial->write(0x21);
    serial->write(mode.value);
}

/* set font size: times include height anf width */
void EmbeddedPrinter::big(CharBigTimes bigTimes) {
    bigTimes.height = constrain(bigTimes.height, 0, 7);
    bigTimes.width = constrain(bigTimes.width, 0, 7);
    serial->write(0x1D);
    serial->write(0x21);
    serial->write(bigTimes.value);
}

/* font size */
void EmbeddedPrinter:big(uint8_t  times) {
    CharBigTimes sizeTimes;
    times = constrain(times, 0, 7);
    sizeTimes.height = times;
    sizeTimes.width = times;
    serial->write(0x1D);
    serial->write(0x21);
    serial->write(sizeTimes.value);
}

/* reverse color */
void EmbeddedPrinter::reverse(bool flag) {
    serial->write(0x1D);
    serial->write(0x42);
    serial->write((uint8_t)flag);
}

/* rotate char */
void EmbeddedPrinter::rotate(bool flag) {
    serial->write(0x1B);
    serial->write(0x56);
    serial->write((uint8_t)flag);
}

/* return printer status */
PrinterStatus EmbeddedPrinter::status(void) {
    uint32_t t;
    PrinterStatus st;
    serial->flush();
    serial->write(0x1B);
    serial->write(0x76);
    t = millis();
    while(!serial->available()) {
        if(millis() - t > 100)
        {
            st.timeout = 1;
            return st;
        }
    }
    st.value = serial->read();
    st.value &= 0xCD;
    return st;
}

/* over print */
void EmbeddedPrinter::overprint(bool flag) {
    serial->write(0x1B);
    serial->write(0x47);
    serial->write((uint8_t)flag);
}

/* bold char */
void EmbeddedPrinter::bold(bool flag) {
    serial->write(0x1B);
    serial->write(0x45);
    serial->write((uint8_t)flag);
}

/* space after char: n * 0.125mm */
void EmbeddedPrinter::space(uint8_t n) {
    serial->write(0x1B);
    serial->write(0x20);
    serial->write(n);
}

/* reverse the direction of chars vertical */
void updown(bool flag) {
    serial->write(0x1B);
    serial->write(0x7B);
    serial->write((uint8_t)flag);
}

/* underline: n
 * 0 -> no underline
 * 1 -> 1 point width
 * 2 -> 2 point width
 * */
void underline(uint8_t flag) {
    flag = constrain(flag, 0, 2);
    serial->write(0x1B);
    serial->write(0x2D);
    serial->write(flag);
}

/* use user char set or not */
void EmbeddedPrinter::usercharset(bool flag) {
    serial->write(0x1B);
    serial->write(0x25);
    serial->write((uint8_t)flag);
}

/* use chinese or not */
void EmbeddedPrinter::useCN(bool flag) {
    uint8_t cmd = flag ? 0x26 : 0x2E;
    serial->write(0x1C);
    serial->write(cmd);
}

/* set chinese char mode  */
void EmbeddedPrinter::setMode(CNCharMode mode) {
    serial->write(0x1C);
    serial->write(0x21);
    serial->write(mode.value);
}

/* print string */
void EmbeddedPrinter::print(char *str) {
    serial->print(str);
}

/* print integer num */
void EmbeddedPrinter::print(int num) {
    serial->print(num);
}

/* print float num */
void EmbeddedPrinter::print(double num) {
    serial->print(num);
}
