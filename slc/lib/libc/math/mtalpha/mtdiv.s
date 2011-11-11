   .file "div.s"
   .arch ev6
   .text

#
# unsigned 64-bit integer division
#
# $d0 = dividend
# $d1 = divisor
# $s0 = remainder
# $s1 = quotient
#
   .globl __divmodqu
   .ent __divmodqu
__divmodqu:
   .registers 0 2 3 0 0 0

   allocate 8, $l1

   # Find the highest set bit
   ctlz    $d1, $l2; swch
   sll     $d1, $l2, $l0      # $l0 = divisor
   addq    $l2, 1, $l2        # $l2 = shiftcount

   setlimit $l1, $l2; swch
   cred    $l1,__divmodqu_loop
   puts    $d0, $l1, 0          ; swch # $d0 = dividend
   putg    $l0, $l1, 0
   puts    0, $l1, 1
   sync    $l1, $l2             
   mov     $l2, $31             ; swch
   gets    $l1, 0, $l0
   gets    $l1, 1, $l2
   release $l1
   mov     $l0, $s0             ; swch
   mov     $l2, $s1
   end
   .end __divmodqu


   .ent __divmodqu_loop
#
# $g0     = divisor
# $d0/$s0 = dividend
# $d1/$s1 = answer
#
__divmodqu_loop:
   .registers 1 2 2 0 0 0
   srl     $g0, $l0, $l0       # d = divisor >> index
   cmpule  $l0, $d0, $l1; swch
   beq     $l1, 1f;       swch
   subq    $d0, $l0, $s0       # $s0 = dividend - d
   sll     $d1,   1, $l0; swch # $l0 = answer << 1
   addq    $l0,   1, $s1       # $s1 = (answer << 1) + 1;
   end
1:  mov     $d0, $s0
   sll     $d1,   1, $s1; swch
   end
   .end __divmodqu_loop

#
# signed 64-bit integer division
#
# $d0 = dividend
# $d1 = divisor
# $s0 = modulo
# $s1 = divide
#
   .globl __divmodqs
   .ent __divmodqs
__divmodqs:
   .registers 0 2 6 0 0 0

   allocate 8, $l5

   negq      1, $l3            # $l3 = invsign
   mov     $d1, $l0; swch      # $l0 = divisor
   cmple   $l0, $31, $l2
   bne     $l2, 1f
   negq    $l3, $l3
   negq    $l0, $l0
1:
   mov       1, $l4            # $l4 = dendsign
   mov     $d0, $l1; swch      # $l1 = dividend
   cmple   $l1, $31, $l2
   bne     $l2, 2f
   negq    $l4, $l4
   negq    $l3, $l3
   negq    $l1, $l1
2:

   # Find the highest unset bit.
   # At this point, divisor ($l0) is guaranteed negative.
   negq    $l0, $l2
   ctlz    $l2, $l2           # $l2 = shiftcount

   setlimit $l5, $l2; swch

   subq    $l2,   1, $l2
   sll     $l0, $l2, $l0      # $l0 = divisor
   cred    $l5,__divmodqs_loop
   putg    $l0, $l5, 0          ; swch
   puts    0, $l5, 1
   puts    $l1, $l5, 0
   sync    $l5, $l1
   mov     $l1, $31; swch
   gets    $l5, 0, $l1
   gets    $l5, 1, $l2
   release $l5
   mulq    $l1, $l4, $s0        ; swch
   mulq    $l2, $l3, $s1
   end
   .end __divmodqs


   .ent __divmodqs_loop
#
# $g0     = divisor
# $d0/$s0 = dividend
# $d1/$s1 = answer
#
__divmodqs_loop:
   .registers 1 2 2 0 0 0
   sra     $g0, $l0, $l0       # d = divisor >> index
   cmplt   $l0, $d0, $l1; swch
   bne     $l1, 1f;       swch
   subq    $d0, $l0, $s0       # $s0 = dividend - d
   sll     $d1,   1, $l0; swch # $l0 = answer << 1
   subq    $l0,   1, $s1       # $s1 = (answer << 1) - 1;
   end
1:  mov     $d0, $s0
   sll     $d1,   1, $s1; swch
   end
   .end __divmodqs_loop
