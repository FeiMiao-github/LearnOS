/**
* reference: http://byterunner.com/16550.html
*/

#ifndef UART_H__
#define UART_H__

#include <stdint.h>

#define PLIC_UART0                          (10)

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

// (Receive holding register interrupt) enable the receiver ready interrupt.
#define IER_RHRI_ENABLE                     (0x1)
// (Transmit holding register interrupt) enable the transmitter empty interrupt.
#define IER_THRI_ENABLE                     (0x2)
// (Receive line status interrupt) enable the receiver line status interrupt.
#define IER_RISI_ENABLE                     (0x4)
// (modem status interrupt) enable the modem status register interrupt.
#define IER_MSI_ENABLE                      (0x8)

#define FCR_FIFO_ENABLE                         (0x1)
// Receiver FIFO Reset
#define FCR_R_FIFO_RESET                        (0x2)
// transmit FIFO Reset
#define FCR_T_FIFO_RESET                        (0x4)
// DMA mode select
#define FCR_DMA_MODE_SELECT                     (0x8)
#define FCR_R_TRIGGER_LSB                       (0x40)
#define FCR_R_TRIGGER_MSB                       (0x80)

#define ISR_INTERRUPT_STATUS                    (0x1)
// TODO: 3bit
#define ISR_INTERRUPT_BIT_PRIOR

#define LCR_WORD_LENGTH_5                       (0x0)
#define LCR_WORD_LENGTH_6                       (0x1)
#define LCR_WORD_LENGTH_7                       (0x2)
#define LCR_WORD_LENGTH_8                       (0x3)
#define LCR_STOP_BIT                            (0x4) 
#define LCR_PARITY_ENABLE                       (0x8)
#define LCR_EVEN_PARITY_ENABLE                  (0x10)
#define LCR_SET_PARITY                          (0x20)
#define LCR_SET_BREAK                           (0x40)
#define LCR_DIVISOR_LATCH_ENABLE                (0x80)

#define MCR_DTR                                 (0x01)
#define MCR_RTS                                 (0x02)
#define MCR_OP1                                 (0x4)
#define MCR_OP2                                 (0x8)
#define MCR_LOOP_BACK                           (0x10)

#define LSR_RECEIVE_DATA_READY                  (0x1)
#define LSR_OVERRUN_ERROR                       (0x2)
#define LSR_PARITY_ERROR                        (0x4)
#define LSR_FRAMING_ERROR                       (0x8)
#define LSR_BREAK_INTERRUPT                     (0x10)
#define LSR_TRANSMIT_HOLDING_EMPTY              (0x20)
#define LSR_TRANSMIT_EMPTY                      (0x40)
#define LSR_FIFO_ERROR                          (0x80)

#define MSR_DELTA_CTS                           (0x1)
#define MSR_DELTA_DSR                           (0x2)
#define MSR_DELTA_RI                            (0x4)
#define MSR_DELTA_CD                            (0x8)
#define MSR_CTS                                 (0x10)
#define MSR_DSR                                 (0x20)
#define MSR_RI                                  (0x40)
#define MSR_CD                                  (0x80)

typedef struct {
    // uint8_t RBR_THR_DLL; // 0x00, Receiver Buffer Register/Transmitter Holding Register/Divisor Latch LSB
    union {
        uint8_t RBR;
        uint8_t THR;
        uint8_t DLL;
    };
    union {
        uint8_t IER; // 0x01, Interrupt Enable Register
        uint8_t DLM; // 0x01, MSB of Divisor Latch
    };
    union {
        uint8_t FCR; // 0x02, FIFO Control Register
        uint8_t ISR; // 0x02, Interrupt Status Register
    };
    uint8_t LCR; // 0x03, Line Control Register
    uint8_t MCR; // 0x04, Modem Control Register
    uint8_t LSR; // 0x05, Line Status Register
    uint8_t MSR; // 0x06, Modem Status Register
    uint8_t SRR; // 0x07, Scratch Register Read/Write Register
} uart_16550a_reg_t;

#endif // UART_H__