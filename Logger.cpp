#include "Logger.h"

Logger::LogLevel Logger::logLevel = Logger::Info;

/*
 * Set the log level. Any output below the specified log level will be omitted.
 */
void Logger::setLoglevel(LogLevel level) {
    logLevel = level;
}

/*
 * Returns if debug log level is enabled. This can be used in time critical
 * situations to prevent unnecessary string concatenation (if the message won't
 * be logged in the end).
 *
 * Example:
 * if (Logger::isDebug()) {
 *    Logger::debug("current time: %d", millis());
 * }
 */
boolean Logger::isDebug() {
    return logLevel == Debug;
}

/*
 * Output a log message (called by log(), console())
 *
 * Supports printf() like syntax:
 *
 * %% - outputs a '%' character
 * %s - prints the next parameter as string
 * %d - prints the next parameter as decimal
 * %f - prints the next parameter as double float
 * %x - prints the next parameter as hex value
 * %X - prints the next parameter as hex value with '0x' added before
 * %b - prints the next parameter as binary value
 * %B - prints the next parameter as binary value with '0b' added before
 * %l - prints the next parameter as long
 * %c - prints the next parameter as a character
 * %t - prints the next parameter as boolean ('T' or 'F')
 * %T - prints the next parameter as boolean ('true' or 'false')
 */
void Logger::log(const char *message, ...) {
    va_list args;
    va_start(args, message);
    Logger::logMessage(message, args);
    va_end(args);
}

void Logger::logMessage(const char *format, va_list args) {
    if (!Logger::isDebug()) {
        return;
    }
    
    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            if (*format == '\0')
                break;
            if (*format == '%') {
                Serial.print(*format);
                continue;
            }
            if (*format == 's') {
                register char *s = (char *) va_arg( args, int );
                Serial.print(s);
                continue;
            }
            if (*format == 'd' || *format == 'i') {
                Serial.print(va_arg( args, int ), DEC);
                continue;
            }
            if (*format == 'f') {
                Serial.print(va_arg( args, double ), 2);
                continue;
            }
            if (*format == 'x') {
                Serial.print(va_arg( args, int ), HEX);
                continue;
            }
            if (*format == 'X') {
                Serial.print("0x");
                Serial.print(va_arg( args, int ), HEX);
                continue;
            }
            if (*format == 'b') {
                Serial.print(va_arg( args, int ), BIN);
                continue;
            }
            if (*format == 'B') {
                Serial.print("0b");
                Serial.print(va_arg( args, int ), BIN);
                continue;
            }
            if (*format == 'l') {
                Serial.print(va_arg( args, long ), DEC);
                continue;
            }

            if (*format == 'c') {
                Serial.print(va_arg( args, int ));
                continue;
            }
            if (*format == 't') {
                if (va_arg( args, int ) == 1) {
                    Serial.print("T");
                } else {
                    Serial.print("F");
                }
                continue;
            }
            if (*format == 'T') {
                if (va_arg( args, int ) == 1) {
                    Serial.print("true");
                } else {
                    Serial.print("false");
                }
                continue;
            }

        }
        Serial.print(*format);
    }
    Serial.println();
}



