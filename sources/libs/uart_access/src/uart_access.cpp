#include "uart_access.h"
#include <stdint.h>

// Full Bare-Metal UART1 Setup (ESP32-S2)

#define DR_REG_UART1_BASE       0x60010000
#define DR_REG_IO_MUX_BASE      0x60009000
#define DR_REG_GPIO_BASE        0x60004000
#define DPORT_PERIP_CLK_EN0_REG 0x3F400000  // Adjust if you're in IDF, else not used here

// UART Registers
#define UART_FIFO(u)            (*(volatile uint32_t *)((u) + 0x0))
#define UART_INT_CLR(u)         (*(volatile uint32_t *)((u) + 0x10))
#define UART_CLKDIV(u)          (*(volatile uint32_t *)((u) + 0x14))
#define UART_CONF0(u)           (*(volatile uint32_t *)((u) + 0x20))
#define UART_CONF1(u)           (*(volatile uint32_t *)((u) + 0x24))
#define UART_STATUS(u)          (*(volatile uint32_t *)((u) + 0x1C))

// GPIO function select: MUX settings for GPIO17 (TX1) and GPIO21 (RX1)
#define IO_MUX_GPIO17_REG       (DR_REG_IO_MUX_BASE + 0x44)
#define IO_MUX_GPIO21_REG       (DR_REG_IO_MUX_BASE + 0x58)

#define UART1                   DR_REG_UART1_BASE
#define UART_BAUD_DIV           115200

static void print_int(int value, int width, char pad);
static void print_hex(unsigned int value, int width, char pad);
static void print_int_to_buf(char *buf, int *pos, int maxlen, int value, int width, char pad);
static void print_hex_to_buf(char *buf, int *pos, int maxlen, unsigned int value, int width, char pad);



/*--------------------------------------------------*/
void uart_setup(void) 
{
    // Configure UART1 baud rate: clk_div = APB_CLK / (baud * 16)
    // APB_CLK = 80 MHz → 80,000,000 / (115200 * 16) ≈ 43
    UART_CLKDIV(UART1) = 43 << 0;

    // Configure 8N1 format (8 data, no parity, 1 stop)
    UART_CONF0(UART1) = 0;

    // Enable TX and RX FIFO, default FIFO thresholds
    UART_CONF1(UART1) = 0x60;  // TXFIFO empty threshold = 96

    // Clear all pending interrupts
    UART_INT_CLR(UART1) = 0xFFFFFFFF;

    // Configure GPIO17 (TXD1)
    *(volatile uint32_t *)IO_MUX_GPIO17_REG = 0x10;  // Function 2 = UART1 TX
    *(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x90 + 0x04 * 17) = (2 << 0); // Set GPIO function 2

    // Configure GPIO21 (RXD1)
    *(volatile uint32_t *)IO_MUX_GPIO21_REG = 0x10;  // Function 2 = UART1 RX
    *(volatile uint32_t *)(DR_REG_GPIO_BASE + 0x90 + 0x04 * 21) = (2 << 0); // Set GPIO function 2
}

/*--------------------------------------------------*/
int uart_getchar(void) 
{
    while (!(UART_STATUS(UART1) & (0xFF << 0))) {}  // Wait for RX FIFO to have data
    return UART_FIFO(UART1) & 0xFF;
}


/*--------------------------------------------------*/
void uart_putchar(char c) 
{
    while (!(UART_STATUS(UART1) & (1 << 1))) {}     // Wait for TX FIFO not full
    UART_FIFO(UART1) = c;
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


