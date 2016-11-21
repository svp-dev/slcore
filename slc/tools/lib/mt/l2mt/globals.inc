	.file "globals.s"
! PSEUDO OUTPUT DEVICES
        .section ".bss"
        .common __pseudo_stdout,4,4
        .common __pseudo_stderr,4,4
        .common __exit_code,4,4
        .section        ".rodata"
        .align 8
.LLC1:
        .asciz  "a.out"
        .section        ".data"
        .align 4
        .type   __pseudo_argv, #object
        .size   __pseudo_argv, 8
__pseudo_argv:
        .long   .LLC1
        .long   0
        .global __argv_ptr
        .align 4
        .type   __argv_ptr, #object
        .size   __argv_ptr, 4
__argv_ptr:
        .long   __pseudo_argv
        .global __argc
        .align 4
        .type   __argc, #object
        .size   __argc, 4
__argc:
        .long   1
        .common __main_place_id,4,4
        .common environ,4,4
