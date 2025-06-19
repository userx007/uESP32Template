#include "uart_access.h"
#include <stdint.h>

/* ================================================
                    defines
==================================================*/

#define UART_FIFO_REG  (*(volatile uint32_t *)0x3FF40000)
#define UART_TXD_READY (*(volatile uint32_t *)0x3FF40018)
#define UART_RXD_READY (*(volatile uint32_t *)0x3FF4001C)
#define UART_RX_FIFO   (*(volatile uint32_t *)0x3FF40078)


/* ================================================
            private interfaces declaration
==================================================*/


static void print_int(int value, int width, char pad);
static void print_hex(unsigned int value, int width, char pad);
static void print_int_to_buf(char *buf, int *pos, int maxlen, int value, int width, char pad);
static void print_hex_to_buf(char *buf, int *pos, int maxlen, unsigned int value, int width, char pad);


/* ================================================
            public interfaces ddefinition
==================================================*/

/*--------------------------------------------------*/
void uart_setup(void)
{

}

/*--------------------------------------------------*/
int uart_getchar(void)
{
    // Wait until RX FIFO has data
    while (!(UART_RXD_READY & (1 << 0))) { }
    return (int)(UART_RX_FIFO & 0xFF);
}

/*--------------------------------------------------*/
void uart_putchar(char c)
{
    while (!(UART_TXD_READY & (1 << 1))) { }
    UART_FIFO_REG = c;
}

/*--------------------------------------------------*/
void uart_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            char pad = ' ';
            int width = 0;
            if (*fmt == '0') {
                pad = '0';
                fmt++;
            }
            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt - '0');
                fmt++;
            }
            switch (*fmt) {
                case 's': {
                    const char *s = va_arg(args, const char *);
                    while (*s) {
                        uart_putchar(*s++);
                    }
                    break;
                }
                case 'd':
                    print_int(va_arg(args, int), width, pad);
                    break;
                case 'x':
                    print_hex(va_arg(args, unsigned int), width, pad);
                    break;
                case 'c':
                    uart_putchar((char)va_arg(args, int));
                    break;
                default:
                    uart_putchar('%');
                    uart_putchar(*fmt);
                    break;
            }
        } else {
            uart_putchar(*fmt);
        }
        fmt++;
    }
    va_end(args);
}



/*--------------------------------------------------*/
int uart_snprintf(char *buf, int maxlen, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int pos = 0;

    while (*fmt && pos < maxlen - 1) {
        if (*fmt == '%') {
            fmt++;
            char pad = ' ';
            int width = 0;
            if (*fmt == '0') {
                pad = '0';
                fmt++;
            }
            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt - '0');
                fmt++;
            }
            switch (*fmt) {
                case 's': {
                    const char *s = va_arg(args, const char *);
                    while (*s && pos < maxlen - 1) {
                        buf[pos++] = *s++;
                    }
                    break;
                }
                case 'd':
                    print_int_to_buf(buf, &pos, maxlen, va_arg(args, int), width, pad);
                    break;
                case 'x':
                    print_hex_to_buf(buf, &pos, maxlen, va_arg(args, unsigned int), width, pad);
                    break;
                case 'c':
                    buf[pos++] = (char)va_arg(args, int);
                    break;
                default:
                    buf[pos++] = '%';
                    buf[pos++] = *fmt;
                    break;
            }
        } else {
            buf[pos++] = *fmt;
        }
        fmt++;
    }

    buf[pos] = '\0';
    va_end(args);
    return pos;
}


/* ================================================
            private interfaces ddefinition
==================================================*/


/*--------------------------------------------------*/
static void print_int(int value, int width, char pad)
{
    char buffer[12];
    int i = 0;
    if (value < 0) {
        uart_putchar('-');
        value = -value;
        width--;
    }
    do {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    } while (value);
    while (width-- > i) {
        uart_putchar(pad);
    }
    while (i--) {
        uart_putchar(buffer[i]);
    }
}



/*--------------------------------------------------*/
static void print_hex(unsigned int value, int width, char pad)
{
    const char *hex = "0123456789ABCDEF";
    char buffer[8];
    int i = 0;
    do {
        buffer[i++] = hex[value & 0xF];
        value >>= 4;
    } while (value);
    while (width-- > i + 2) {
        uart_putchar(pad);
    }
    uart_putchar('0');
    uart_putchar('x');
    while (i--) {
        uart_putchar(buffer[i]);
    }
}



/*--------------------------------------------------*/
static void print_int_to_buf(char *buf, int *pos, int maxlen, int value, int width, char pad)
{
    char tmp[12];
    int i = 0;
    if (value < 0) {
        if (*pos < maxlen - 1) buf[(*pos)++] = '-';
        value = -value;
        width--;
    }
    do {
        tmp[i++] = '0' + (value % 10);
        value /= 10;
    } while (value);
    while (width-- > i && *pos < maxlen - 1) {
        buf[(*pos)++] = pad;
    }
    while (i-- && *pos < maxlen - 1) {
        buf[(*pos)++] = tmp[i];
    }
}



/*--------------------------------------------------*/
static void print_hex_to_buf(char *buf, int *pos, int maxlen, unsigned int value, int width, char pad)
{
    const char *hex = "0123456789ABCDEF";
    char tmp[8];
    int i = 0;
    do {
        tmp[i++] = hex[value & 0xF];
        value >>= 4;
    } while (value);
    if (*pos < maxlen - 2) {
        buf[(*pos)++] = '0';
        buf[(*pos)++] = 'x';
    }
    while (width-- > i + 2 && *pos < maxlen - 1) {
        buf[(*pos)++] = pad;
    }
    while (i-- && *pos < maxlen - 1) {
        buf[(*pos)++] = tmp[i];
    }
}


