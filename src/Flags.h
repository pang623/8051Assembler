#ifndef FLAGS_H
#define FLAGS_H

//flags(DIRECT)(src)
#define     DIR_A         (1 << 0)
#define     DIR_IMM       (1 << 1)

//flags(A)(src)
#define     A_REG         (1 << 2)
#define     A_IND         (1 << 3)
#define     A_IMM         (1 << 4)
#define     A_DIR         (1 << 5)

//flags(C)(src)
#define     C_BIT         (1 << 6)
#define     C_BARBIT      (1 << 7)

//single operand flags
#define     ROTATE_A                 (1 << 24)
#define     OTHER_A                  (1 << 25)
#define     OPERAND_AB               (1 << 26)
#define     OPERAND_REG              (1 << 27)
#define     OPERAND_INDIRECT         (1 << 28)
#define     STACK_DIRECT             (1 << 29)
#define     OTHER_DIRECT             (1 << 30)
#define     OPERAND_C                (1 << 31)

#endif // FLAGS_H
