.file "init_uart.s"

.section ".text"
.global init_uart

.include "uart_defs.inc"

.align 4*32

.type init_uart, #function
.proc 04
.ctlbits 128 0

! Function: initialize UART
! Parameters:
!     none
! Returns:
!     none
! Damages:
!     %tl1 - APBUART1_BASE
!     %tl4 - temporary storage of config constants

init_uart:
    set     APBUART1_BASE, %tl1

    mov     SCALER_RELOAD_VALUE, %tl4
    st      %tl4, [%tl1+APBUART_PRESCALER_OFFSET]  ! non-cacheable memory access

    mov     ENABLE_RX, %tl4
    or      %tl4, ENABLE_TX, %tl4
    st      %tl4, [%tl1+APBUART_CONTROL_OFFSET]  ! non-cacheable memory access

init_uart_end:
    retl
    nop

.size init_uart, .-init_uart
