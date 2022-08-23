#ifndef UART_H__
#define UART_H__

#include <stdint.h>

#define UART0  (0x10000000LU)

// LSR: line status register(RO)
#define LSR_DATA_AVAILABLE                  (0x1)
#define LSR_OVERRUN_ERROR                   (0x2)
#define LSR_PARITY_ERROR                    (0x4)
#define LSR_FRAME_ERROR                     (0x8)
#define LSR_BREAK_SIGNAL_RECEIVED           (0x10)
#define LSR_THR_IS_EMPTY                    (0x20)
#define LSR_THR_IS_EMPTY_AND_LINE_IS_IDLE   (0x40)
#define LSR_ERRORNOUS_DATA_IN_FIFO          (0x80)

typedef struct {
    uint8_t RHRI:1; // Receive holding register interrupt
    uint8_t THRI:1; // Transmit holding register interrupt
    uint8_t RISI:1; // Receive line status interrupt
    uint8_t MSI:1; // modem status interrupt
    uint8_t OTHER: 4; // Reserved for future use
} IER_t;

typedef struct {
    uint8_t FIFO_ENABLE: 1; // FiFO Enable
    uint8_t R_FIFO_RESET: 1; // Receiver FIFO Reset
    uint8_t T_FIFO_RESET: 1; // transmit FIFO Reset
    uint8_t DMA_MODE_SELECT: 1; // DMA mode select
    uint8_t OTHER: 2; // Reserved for future use
    uint8_t R_TRIGGER_LSB: 1; // RCVR trigger LSB
    uint8_t R_TRIGGER_MSB: 1; // RCVR trigger MSB
} FCR_t;

typedef struct {
    uint8_t INTERRUPT_STATUS:1; // interrupt status
    uint8_t INTERRUPT_BIT_PRIOR:3; // interrupt
    uint8_t OTHER: 2; // 0
    uint8_t FIFO_ENABLED: 2; // FIFO enabled
} ISR_t;

typedef struct {
    uint8_t WORD_LENGTH:2; // word length
    uint8_t STOP_BIT:1; // stop bits
    uint8_t PARITY_ENABLE:1; // parity bits
    uint8_t EVEN_PARITY:1; // parity enable bits
    uint8_t SET_PARITY:1; // set parity bits
    uint8_t SET_BREAK:1; // set break bits
    uint8_t DIVISOR_LATCH_ENABLE:1; // divisor latch enable bits
} LCR_t;

typedef struct {
    uint8_t DTR:1; // DTR register
    uint8_t RTS:1; // RTS register
    uint8_t OP1:1; // OP1 register
    uint8_t OP2:1; // OP2 register
    uint8_t LOOP_BACK:1; // loop back register
    uint8_t _:3;
} MCR_t;

typedef struct {
    uint8_t RECEIVE_DATA_READY:1; // receive data ready
    uint8_t OVERRUN_ERROR:1; // overrun error
    uint8_t PARITY_ERROR:1; // parity error
    uint8_t FRAMING_ERROR:1; // framing error
    uint8_t BREAK_INTERRUPT:1; // break interrupt
    uint8_t TRANSMIT_HOLDING_EMPTY:1; // transmit holding empty
    uint8_t TRANSMIT_EMPTY:1; // transmit empty
    uint8_t FIFO_ERROR:1; // transmit error
} LSR_t;

typedef struct {
    uint8_t DELTA_CTS:1;
    uint8_t DELTA_DSR:1;
    uint8_t DELTA_RI:1;
    uint8_t DELTA_CD:1;
    uint8_t CTS:1;
    uint8_t DSR:1;
    uint8_t RI:1;
    uint8_t CD:1;
} MSR_t;

typedef struct {
    // uint8_t RBR_THR_DLL; // 0x00, Receiver Buffer Register/Transmitter Holding Register/Divisor Latch LSB
    union {
        uint8_t RBR;
        uint8_t THR;
        uint8_t DLL;
    };
    union {
        IER_t IER; // 0x01, Interrupt Enable Register
        uint8_t DLM; // 0x01, MSB of Divisor Latch
    };
    union {
        FCR_t FCR; // 0x02, FIFO Control Register
        ISR_t ISR; // 0x02, Interrupt Status Register
    };
    LCR_t LCR; // 0x03, Line Control Register
    MCR_t MCR; // 0x04, Modem Control Register
    LSR_t LSR; // 0x05, Line Status Register
    MSR_t MSR; // 0x06, Modem Status Register
    uint8_t SRR; // 0x07, Scratch Register Read/Write Register
} uart_16550a_reg_t;

#endif // UART_H__