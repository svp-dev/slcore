.file "getchar.s"

.section ".text"
.global uart_getchar

.include "uart_defs.inc"

.align 4*32

.type uart_getchar, #function
.proc 04
.ctlbits 128 0

! Function: read char from UART
! Parameters:
!     none
! Returns:
!     %o0 - character
! Damages:
!     %tl1 - APBUART1_BASE
!     %tl4 - UART status, internal loop ctl
!     %tl5 - delay loop ctl

uart_getchar:
    set     APBUART1_BASE, %tl1
0:
    ld      [%tl1+APBUART_STATUS_OFFSET], %tl4  ! non-cacheable memory access
    andcc   %tl4, STAT_DR, %tl4
    mov     32, %tl5                            ! delay(10)
    bne     getchar_rdy
    nop

1:
    mov     625, %tl4  ! 100us @ 25MHz
2:
    subcc   %tl4, 1, %tl4
    nop
    bne     2b
    nop
    subcc   %tl5, 1, %tl5
    nop
    bne     1b
    nop
    ba      0b
    nop

getchar_rdy:
    ld      [%tl1+APBUART_DATA_OFFSET], %o0  ! non-cacheable memory access

getchar_end:
    retl
    nop

.size uart_getchar, .-uart_getchar
