#include <lib/log.h>

#include <drivers/serial.h>

void log_init(void) {
    serial_init();
}

void log_print(const char *str) {
    serial_print(str);
}

void log_println(const char *str) {
    serial_println(str);
}

void log_info(const char *str) {
    serial_print("[INFO]  ");
    serial_println(str);
}

void log_warn(const char *str) {
    serial_print("[WARN]  ");
    serial_println(str);
}

void log_error(const char *str) {
    serial_print("[ERROR] ");
    serial_println(str);
}
