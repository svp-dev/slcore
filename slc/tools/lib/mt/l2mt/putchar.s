.file "putchar.s"

.section ".text"
.global uart_putchar

.include "uart_defs.inc"

.align 4*32

.type uart_putchar, #function
.proc 04
.ctlbits 128 0

! Function: write char to UART
! Parameters:
!     %o0 - character
! Returns:
!     none
! Damages:
!     %tl1 - APBUART1_BASE
!     %tl4 - UART status, internal loop ctl
!     %tl5 - delay loop ctl

uart_putchar:
    and     %tl3, 255, %tl3
    set     APBUART1_BASE, %tl1
0:
    ld      [%tl1+APBUART_STATUS_OFFSET], %tl4  ! non-cacheable memory access
    andcc   %tl4, STAT_TH, %tl4
    mov     10, %tl5                            ! delay(1ms)
    bne     putchar_rdy
!    nop

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

putchar_rdy:
    st      %o0, [%tl1+APBUART_DATA_OFFSET]
0:
    ld      [%tl1+APBUART_STATUS_OFFSET], %tl4  ! non-cacheable memory access
    andcc   %tl4, STAT_TH, %tl4
    mov     10, %tl5                            ! delay(1ms)
    bne     putchar_end
!    nop
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

putchar_end:
    retl
    nop

.size uart_putchar, .-uart_putchar
