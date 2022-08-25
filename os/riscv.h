#ifndef RISCV_H__
#define RISCV_H__

// mip: Machine interrupt-pending register
// external interrupt-pending
#define MIP_MEIP 0x800
#define MIP_HEIP 0x400
#define MIP_SEIP 0x200
#define MIP_UEIP 0x100
// timer interrupt-pending
#define MIP_MTIP 0x080
#define MIP_HTIP 0x040
#define MIP_STIP 0x020
#define MIP_UTIP 0x010
// software interrupt-pending
#define MIP_MSIP 0x008
#define MIP_HSIP 0x004
#define MIP_SSIP 0x002
#define MIP_USIP 0x001

// mie: Machine interrupt-enable register
// external interrupt-enable
#define MIE_MEIE 0x800
#define MIE_HEIE 0x400
#define MIE_SEIE 0x200
#define MIE_UEIE 0x100
// timer interrupt-enable
#define MIE_MTIE 0x080
#define MIE_HTIE 0x040
#define MIE_STIE 0x020
#define MIE_UTIE 0x010
// software interrupt-enable
#define MIE_MSIE 0x008
#define MIE_HSIE 0x004
#define MIE_SSIE 0x002
#define MIE_USIE 0x001

#define PMP_R 0x01
#define PMP_W 0x02
#define PMP_X 0x04
#define PMP_A 0x18
#define PMP_L 0x80
#define PMP_SHIFT 2

#define PMP_TOR 0x08
#define PMP_NA4 0x10
#define PMP_NAPOT 0x18

#define MSTATUS_MIE (1 << 3)

// TRAP
// if 1, interrupt; otherwise exception
#define INTERRUPT_BIT (~((0x0lu - 1) >> 1)) 

#endif // RISCV_H__
