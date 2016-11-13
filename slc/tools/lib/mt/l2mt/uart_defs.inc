.file "uart_defs.inc"


.equ    DISABLE,     0x0
.equ    ENABLE_RX,   0x1
.equ    ENABLE_TX,   0x2
.equ    RX_INT,      0x4
.equ    TX_INT,      0x8
.equ    EVEN_PARITY, 0x20
.equ    ODD_PARITY,  0x30

.equ    SCALER_RELOAD_VALUE_40,  0x80 /* 40MHz 38400bps */
.equ    SCALER_RELOAD_VALUE_100, 0x146  /* 100MHz 38400bps */
.equ    SCALER_RELOAD_VALUE_50,  0x036  /* 50MHz 115200bps */
.equ    SCALER_RELOAD_VALUE_51,  0x038  /* 51.2821MHz 115200bps */
.equ    SCALER_RELOAD_VALUE_25_115,     0x01A  /* 25MHz 115200bps */
.equ    SCALER_RELOAD_VALUE,     0x0145 /* 25MHz 9600bps */

.equ    LOOP_BACK,   0x80
.equ    FLOW_CONTROL,0x40

/* The following defines the bits in the LEON UART State Registers. */
.equ    STAT_DR, 0x00000001 /* Data Ready */
.equ    STAT_TS, 0x00000002 /* TX Send Register Empty */
.equ    STAT_TH, 0x00000004 /* TX Hold Register Empty */
.equ    STAT_BR, 0x00000008 /* Break Error */
.equ    STAT_OE, 0x00000010 /* RX Overrun Error */
.equ    STAT_PE, 0x00000020 /* RX Parity Error */
.equ    STAT_FE, 0x00000040 /* RX Framing Error */

.equ    STAT_ERR,0x00000078 /* Error Mask */

/* The following defines the bits in the LEON UART Ctrl Registers. */
.equ    CTRL_RE, 0x00000001 /* Receiver enable */
.equ    CTRL_TE, 0x00000002 /* Transmitter enable */
.equ    CTRL_RI, 0x00000004 /* Receiver interrupt enable */
.equ    CTRL_TI, 0x00000008 /* Transmitter interrupt enable */
.equ    CTRL_PS, 0x00000010 /* Parity select */
.equ    CTRL_PE, 0x00000020 /* Parity enable */
.equ    CTRL_FL, 0x00000040 /* Flow control enable */
.equ    CTRL_LB, 0x00000080 /* Loop Back enable */
.equ    CTRL_EC, 0x00000100 /* External clock enable */

.equ    APBUART1_BASE,      0x80000070
.equ    APBUART2_BASE,      0x80000080

.equ    APBUART_DATA_OFFSET,      0x0
.equ    APBUART_STATUS_OFFSET,    0x4
.equ    APBUART_CONTROL_OFFSET,   0x8
.equ    APBUART_PRESCALER_OFFSET, 0xc


