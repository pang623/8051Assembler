#ifndef FLAGS_H
#define FLAGS_H

//flags(DIRECT)(src)
#define     DIR_A                 (1 << 0)
#define     DIR_IMM               (1 << 1)

//flags(A)(src)
#define     A_REG                 (1 << 2)
#define     A_IND                 (1 << 3)
#define     A_IMM                 (1 << 4)
#define     A_DIR                 (1 << 5)

//flags(C)(src)
#define     C_BIT                 (1 << 6)
#define     C_BARBIT              (1 << 7)

//single operand flags
#define     OPERAND_A             (1 << 8)
#define     OPERAND_A_ROT         (1 << 9)
#define     OPERAND_AB            (1 << 10)
#define     OPERAND_REG           (1 << 11)
#define     OPERAND_IND           (1 << 12)
#define     OPERAND_DIR           (1 << 13)
#define     OPERAND_DIR_STACK     (1 << 14)
#define     OPERAND_C             (1 << 15)
#define     OPERAND_DPTR          (1 << 16)
#define     OPERAND_DIR16         (1 << 17)
#define     OPERAND_DIR11         (1 << 18)
#define     OPERAND_BIT           (1 << 19)

#endif // FLAGS_H
