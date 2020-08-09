#ifndef FLAGS_H
#define FLAGS_H

//flags(dest)
#define     A_WITH_OPERANDS          (1 << 0)
#define     DIRECT_WITH_OPERANDS     (1 << 1)
#define     INDIRECT_WITH_OPERANDS   (1 << 2)
#define     REGISTER_WITH_OPERANDS   (1 << 3)
#define     C_WITH_OPERANDS          (1 << 4)

//flags(DIRECT)(src)
#define     DIRECT_A_LOGICAL         (1 << 5)
#define     DIRECT_A_OTHER           (1 << 6)
#define     DIRECT_REG               (1 << 7)
#define     DIRECT_DIRECT            (1 << 8)
#define     DIRECT_IMM_LOGICAL       (1 << 9)
#define     DIRECT_IMM_OTHER         (1 << 10)
#define     DIRECT_INDIRECT          (1 << 11)

//flags(A)(src)
#define     A_WITH_REGISTER          (1 << 12)
#define     A_WITH_INDIRECT          (1 << 13)
#define     A_WITH_IMMEDIATE         (1 << 14)
#define     A_WITH_DIRECT            (1 << 15)

//flags(REG)(src)
#define     REG_WITH_A               (1 << 16)
#define     REG_WITH_IMM             (1 << 17)
#define     REG_WITH_DIRECT          (1 << 18)

//flags(INDIRECT)(src)
#define     INDIRECT_WITH_A          (1 << 19)
#define     INDIRECT_DIRECT          (1 << 20)
#define     INDIRECT_IMM             (1 << 21)

//flags(C)(src)
#define     C_WITH_BIT               (1 << 22)
#define     C_WITH_BARBIT            (1 << 23)

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
