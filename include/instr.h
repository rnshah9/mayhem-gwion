#ifndef __INSTR
#define __INSTR
#define PUSH_MEM(a, b) a->mem += b;
#define POP_MEM(a, b)  a->mem -= b;
#define PUSH_REG(a, b) a->reg += b;
#define POP_REG(a, b)  a->reg -= b;

#define MEM(a) (shred->mem + (a))
#define REG(a) (shred->reg + (a))
#define INSTR(a) __attribute__((hot))\
ANN2(1) void a(const VM_Shred shred __attribute__((unused)), const Instr instr  __attribute__((unused)))

enum Kind {
  KIND_INT,
  KIND_FLOAT,
  KIND_OTHER,
  KIND_ADDR
};

typedef struct Instr_     * Instr;
typedef void (*f_instr)(const VM_Shred, const Instr);
struct Instr_ {
  m_bit opcode;
//  union {
//    m_float f;
    m_uint m_val;
//  };
  m_uint m_val2;
  m_bit ptr[SZ_MINVAL];
  void (*execute)(const VM_Shred shred, const Instr instr);
};

INSTR(EOC);
INSTR(DTOR_EOC);
INSTR(DtorReturn);

INSTR(RegPushMe);
INSTR(RegPushMaybe);

/* branching */
INSTR(BranchSwitch);
INSTR(SwitchIni);
INSTR(SwitchEnd);

INSTR(ComplexReal);
INSTR(ComplexImag);

/* function */
INSTR(DtorReturn);

/* array */
INSTR(ArrayTop);
INSTR(ArrayBottom);
INSTR(ArrayPost);
INSTR(ArrayInit);
INSTR(ArrayAlloc);
INSTR(ArrayAccess);
INSTR(ArrayAccessMulti);
INSTR(ArrayAppend);

/* vararg */
INSTR(VarargIni);
INSTR(VarargEmpty);
INSTR(VarargTop);
INSTR(VarargEnd);
INSTR(VarargMember);

INSTR(MemberFunction);
INSTR(VecMember);
INSTR(PopArrayClass);

INSTR(AutoLoopStart);
INSTR(AutoLoopEnd);
INSTR(DotTmpl);
// optimizations
#ifdef OPTIMIZE
INSTR(PutArgsInMem);
INSTR(ConstPropSet);
INSTR(ConstPropGet);
#endif
#include "opcode.h"
#endif
