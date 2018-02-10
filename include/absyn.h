#ifndef __ABSYN
#define __ABSYN
#include <vm.h>

typedef struct Ast_       * Ast;
typedef struct Class_Def_ * Class_Def;
typedef struct Func_Def_  * Func_Def;
typedef struct Stmt_List_       * Stmt_List;
typedef struct Exp_ * Exp;
typedef struct Stmt_            * Stmt;
typedef struct Var_Decl_* Var_Decl;
typedef struct Var_Decl_List_* Var_Decl_List;
typedef struct Array_Sub_ * Array_Sub;
typedef struct Arg_List_ * Arg_List;

typedef struct {
  Exp base;
  Type t_base;
  S_Symbol xid;
  Exp self;
  int pos;
} Exp_Dot;

struct Array_Sub_ {
  m_uint depth;
  Exp exp_list;
  Type type;
  int pos;
};
Array_Sub new_array_sub(Exp exp, int pos);
Array_Sub prepend_array_sub(Array_Sub array, Exp exp);
void free_array_sub(Array_Sub array);

typedef struct {
  Exp base;
  Array_Sub indices;
  int pos;
} Array_Exp;

typedef struct {
  Exp base;
  Array_Sub indices;
  Exp self;
  int pos;
} Exp_Array;

Exp new_array(Exp base, Array_Sub indices, int pos);

struct Var_Decl_ {
  S_Symbol xid;
  Value value;
  Array_Sub array;
  void* addr;
  int pos;
};
Var_Decl new_var_decl(S_Symbol xid, Array_Sub array, int pos);

struct Var_Decl_List_ {
  Var_Decl self;
  Var_Decl_List next;
  int pos;
};
Var_Decl_List new_var_decl_list(Var_Decl decl, Var_Decl_List list, int pos);

typedef struct {
  ID_List xid;
  Array_Sub array;
  Exp_Dot* dot;
  Type_List types;
  ae_flag flag;
  int pos;
} Type_Decl;
Type_Decl* new_type_decl(ID_List name, int ref, int pos);
Type_Decl* new_type_decl2(ID_List name, int ref, int pos);
void free_type_decl(Type_Decl* a);
Type_Decl* add_type_decl_array(Type_Decl* a, Array_Sub array, int pos);

struct ID_List_    {
  S_Symbol xid;
  ID_List next;
  ID_List ref;
  int pos;
};
ID_List new_id_list(const S_Symbol xid, int pos);
ID_List prepend_id_list(const S_Symbol xid, ID_List list, int pos);
void free_id_list(ID_List a);

struct Type_List_  {
  Type_Decl* list;
  Type_List next;
  int pos;
};
Type_List new_type_list(Type_Decl* list, Type_List next, int pos);
void free_type_list(Type_List a);

typedef struct {
  Exp re;
  Exp im;
  int pos;
} Complex;
Complex* new_complex(Exp re, int pos);

typedef struct {
  Exp mod;
  Exp phase;
  int pos;
} Polar;
Polar* new_polar(Exp mod, int pos);

typedef struct {
  Exp args;
  m_uint numdims;
  int pos;
} Vec;
Vec* new_vec(Exp e, int pos);

struct Arg_List_ {
  Type_Decl* type_decl;
  Var_Decl var_decl;
  Type type;
  Arg_List  next;
  int pos;
};
Arg_List new_arg_list(Type_Decl* type_decl, Var_Decl var_decl, Arg_List arg_list, int pos);
void free_arg_list(Arg_List a);

typedef enum { ae_exp_decl, ae_exp_binary, ae_exp_unary, ae_exp_primary,
               ae_exp_cast, ae_exp_post, ae_exp_call, ae_exp_array,
               ae_exp_if, ae_exp_dot, ae_exp_dur
             } Exp_type;
typedef enum { ae_meta_var, ae_meta_value } ae_Exp_Meta;
typedef enum { ae_primary_id, ae_primary_num, ae_primary_float,
               ae_primary_str, ae_primary_array,
               ae_primary_hack, ae_primary_complex, ae_primary_polar, ae_primary_vec,
               ae_primary_char, ae_primary_nil
             } ae_Exp_Primary_Type;
typedef struct {
  Type_Decl* type;
  Type m_type;
  Var_Decl_List list;
  Class_Def base;
  Exp self;
  int pos;
} Exp_Decl;
typedef struct {
  ae_Exp_Primary_Type primary_type;
  Value value;
  union exp_primary_data {
    S_Symbol var;
    long num;
    m_float fnum;
    m_str chr;
    m_str str;
    Array_Sub array;
    Exp exp;
    Complex* cmp;
    Polar* polar;
    Vec* vec;
  } d;
  Exp self;
  int pos;
} Exp_Primary;

typedef struct {
  Type_List types;
  ID_List base;// hack for template
} Tmpl_Call;
Tmpl_Call* new_tmpl_call(Type_List);
typedef struct {
  Exp func;
  Exp args;
  Func m_func;
  Tmpl_Call* tmpl;
  Exp self;
  int pos;
} Exp_Func;
typedef struct {
  Type_Decl* type;
  Exp exp;
  Exp self;
  Nspc nspc;
  Func func;
  int pos;
} Exp_Cast;
typedef struct {
  Exp lhs, rhs;
  Operator op;
  Nspc nspc;
  Func func;
  Exp self;
  int pos;
} Exp_Binary;
typedef struct {
  Operator op;
  Nspc nspc;
  Exp exp;
  Exp self;
  int pos;
} Exp_Postfix;
typedef struct {
  Exp cond;
  Exp if_exp;
  Exp else_exp;
  Exp self;
  int pos;
} Exp_If;
typedef struct {
  Exp base;
  Exp unit;
  Exp self;
  int pos;
} Exp_Dur;
typedef struct {
  Operator op;
  Nspc nspc;
  Exp exp;
  Type_Decl* type;
//  Array_Sub array;
  Stmt code;
  m_uint code_depth;
  Exp self;
  int pos;
} Exp_Unary;

struct Exp_ {
  Exp_type exp_type;
  ae_Exp_Meta meta;
  Type type;
  Type cast_to;
  Exp next;
  union exp_data {
    Exp_Postfix   exp_post;
    Exp_Primary   exp_primary;
    Exp_Decl      exp_decl;
    Exp_Unary     exp_unary;
    Exp_Binary    exp_binary;
    Exp_Cast      exp_cast;
    Exp_Func      exp_func;
    Exp_If        exp_if;
    Exp_Dot       exp_dot;
    Exp_Array     exp_array;
    Exp_Dur       exp_dur;
  } d;
  int pos;
  m_bool emit_var;
};

Exp new_exp_prim_id(S_Symbol xid, int pos);
Exp new_exp_prim_int(long i, int pos);
Exp new_exp_prim_float(m_float num, int pos);
Exp new_exp_prim_string(m_str s, int pos);
Exp new_exp_prim_array(Array_Sub exp_list, int pos);
Exp new_exp_prim_hack(Exp exp, int pos);
Exp new_exp_prim_complex(Complex* exp, int pos);
Exp new_exp_prim_polar(Polar* exp, int pos);
Exp new_exp_prim_vec(Vec* a, int pos);
Exp new_exp_prim_char(m_str chr, int pos);
Exp new_exp_prim_nil(int pos);
Exp new_exp_decl(Type_Decl* type, Var_Decl_List list, int pos);
Exp new_exp_binary(Exp lhs, Operator op, Exp rhs, int pos);
Exp new_exp_post(Exp exp, Operator op, int pos);
Exp new_exp_call(Exp base, Exp args, int pos);
Exp new_exp_cast(Type_Decl* type, Exp exp, int pos);
Exp new_exp_if(Exp cond, Exp if_exp, Exp else_exp, int pos);
Exp new_exp_dur(Exp base, Exp unit, int pos);
Exp new_exp_dot(Exp base, S_Symbol xid, int pos);
Exp new_exp_unary(Operator oper, Exp exp, int pos);
Exp new_exp_unary2(Operator oper, Type_Decl* type, int pos);
Exp new_exp_unary3(Operator oper, Stmt code, int pos);
Exp prepend_exp(Exp exp, Exp next, int pos);
void free_exp(Exp exp);

typedef struct Decl_List_* Decl_List;
struct Decl_List_ {
  Exp self;
  Decl_List next;
};
Decl_List new_decl_list(Exp d, Decl_List l);

typedef enum { ae_stmt_exp, ae_stmt_while, ae_stmt_until, ae_stmt_for, ae_stmt_auto, ae_stmt_loop,
               ae_stmt_if, ae_stmt_code, ae_stmt_switch, ae_stmt_break,
               ae_stmt_continue, ae_stmt_return, ae_stmt_case, ae_stmt_gotolabel,
               ae_stmt_enum, ae_stmt_funcptr, ae_stmt_typedef, ae_stmt_union
             } ae_Stmt_Type;

typedef struct Stmt_Code_       * Stmt_Code;
typedef struct Stmt_Exp_        * Stmt_Return;
typedef struct Stmt_Basic_      * Stmt_Continue;
typedef struct Stmt_Basic_      * Stmt_Break;
typedef struct Stmt_Flow_       * Stmt_While;
typedef struct Stmt_Flow_       * Stmt_Until;
typedef struct Stmt_For_        * Stmt_For;
typedef struct Stmt_Auto_       * Stmt_Auto;
typedef struct Stmt_Loop_       * Stmt_Loop;
typedef struct Stmt_If_         * Stmt_If;
typedef struct Stmt_Switch_     * Stmt_Switch;
typedef struct Stmt_Exp_        * Stmt_Case;
typedef struct Stmt_Goto_Label_ * Stmt_Goto_Label;
typedef struct Stmt_Enum_       * Stmt_Enum;
typedef struct Stmt_Ptr_        * Stmt_Ptr;
typedef struct Stmt_Typedef_    * Stmt_Typedef;
typedef struct Stmt_Union_      * Stmt_Union;

struct Stmt_Basic_ {
  int pos;
};
struct Stmt_Exp_ {
  Exp val;
  int pos;
};
struct Stmt_Flow_ {
  int is_do;
  Exp cond;
  Stmt body;
  Stmt self;
  int pos;
};
struct Stmt_Code_ {
  Stmt_List stmt_list;
  int pos;
};
struct Stmt_For_ {
  Stmt c1;
  Stmt c2;
  Exp c3;
  Stmt body;
  Stmt self;
  int pos;
};
struct Stmt_Auto_ {
  S_Symbol sym;
  Exp exp;
  Stmt body;
  Value v; // is this useful ?
  m_bool is_ptr;
  int pos;
};
struct Stmt_Loop_ {
  Exp cond;
  Stmt body;
  Stmt self;
  int pos;
};
struct Stmt_If_ {
  Exp cond;
  Stmt if_body;
  Stmt else_body;
  int pos;
};
struct Stmt_Goto_Label_ {
  S_Symbol name;
  union stmt_goto_data {
    struct Vector_ v;
    Instr instr;
  } data;
  m_bool is_label;
  int pos;
};
struct Stmt_Switch_ {
  Exp val;
  Stmt stmt;
  Stmt self;
  int pos;
};
struct Stmt_Enum_ {
  ID_List list;
  S_Symbol xid;
  Type t;
  ae_flag flag;
  struct Vector_ values;
  int pos;
};

struct Stmt_Ptr_ {
  Type_Decl* type;
  Type       m_type;
  S_Symbol   xid;
  ae_flag    flag;
  Arg_List   args;
  Type       ret_type;
  Func       func;
  Value      value;
  int        pos;
};
struct Stmt_Typedef_ {
  Type_Decl* type;
  Type       m_type;
  S_Symbol   xid;
  int        pos;
};
struct Stmt_Union_ {
  Decl_List l;
  struct Vector_ v;
  S_Symbol xid;
  Value value;
  ae_flag flag;
  m_uint s;
  m_uint o;
  int pos;
};

struct Stmt_ {
  ae_Stmt_Type stmt_type;
  union stmt_data {
    struct Stmt_Exp_        stmt_exp;
    struct Stmt_Code_       stmt_code;
    struct Stmt_Flow_       stmt_while;
    struct Stmt_Flow_       stmt_until;
    struct Stmt_Loop_       stmt_loop;
    struct Stmt_For_        stmt_for;
    struct Stmt_Auto_       stmt_auto;
    struct Stmt_If_         stmt_if;
    struct Stmt_Basic_      stmt_break;
    struct Stmt_Basic_      stmt_continue;
    struct Stmt_Exp_        stmt_return;
    struct Stmt_Goto_Label_ stmt_gotolabel;
    struct Stmt_Switch_     stmt_switch;
    struct Stmt_Exp_        stmt_case;
    struct Stmt_Enum_       stmt_enum;
    struct Stmt_Ptr_        stmt_ptr;
    struct Stmt_Typedef_    stmt_type;
    struct Stmt_Union_      stmt_union;
  } d;
  int pos;
};

Stmt new_stmt_exp(Exp exp, int pos);
Stmt new_stmt_code(Stmt_List stmt_list, int pos);
Stmt new_stmt_while(Exp cond, Stmt body, m_bool is_do, int pos);
Stmt new_stmt_return(Exp exp, int pos);
Stmt new_stmt_break(int pos);
Stmt new_stmt_continue(int pos);
Stmt new_stmt_if(Exp cond, Stmt if_body, Stmt else_body, int pos);
Stmt new_stmt_until(Exp cond, Stmt body, m_bool is_do, int pos);
Stmt new_stmt_for(Stmt c1, Stmt c2, Exp c3, Stmt body, int pos);
Stmt new_stmt_auto(S_Symbol sym, Exp exp, Stmt body, int pos);
Stmt new_stmt_loop(Exp cond, Stmt body, int pos);
Stmt new_stmt_gotolabel(S_Symbol xid, m_bool is_label, int pos);
Stmt new_stmt_case(Exp exp, int pos);
Stmt new_stmt_enum(ID_List list, S_Symbol xid, int pos);
Stmt new_stmt_switch(Exp val, Stmt stmt, int pos);
Stmt new_stmt_union(Decl_List l, int pos);
Stmt new_func_ptr_stmt(ae_flag key, S_Symbol type, Type_Decl* decl, Arg_List args, int pos);
Stmt new_stmt_typedef(Type_Decl* decl, S_Symbol xid, int pos);
void free_stmt(Stmt a);
struct Stmt_List_ {
  Stmt stmt;
  Stmt_List next;
  int pos;
};
Stmt_List new_stmt_list(Stmt stmt, Stmt_List next, int pos);

typedef struct Class_Body_ * Class_Body;

typedef struct {
  ID_List list;
  m_bool  base;
} Tmpl_List;

struct Func_Def_ {
  Type_Decl* type_decl;
  Type ret_type;
  ae_flag flag;
  S_Symbol name;
  Arg_List arg_list;
  Stmt code;
  m_uint stack_depth;
  union func_def_data {
    Func func;
    void* dl_func_ptr;
  } d;
  Tmpl_List* tmpl;
  int pos;
};
Tmpl_List* new_tmpl_list(ID_List list, m_bool base);
void free_tmpl_list(Tmpl_List*);
const m_bool tmpl_list_base(const Tmpl_List*);
Func_Def new_func_def(ae_flag func_decl, Type_Decl* type_decl, S_Symbol xid, Arg_List arg_list, Stmt code, int pos);
void free_func_def(Func_Def def);

typedef enum { ae_section_stmt, ae_section_func, ae_section_class } ae_Section_Type;
typedef struct {
  ae_Section_Type section_type;
  union section_data {
    Stmt_List stmt_list;
    Class_Def class_def;
    Func_Def func_def;
  } d;
  int pos;
} Section;
Section* new_section_stmt_list(Stmt_List list, int pos);
Section* new_section_func_def(Func_Def func_def, int pos);

struct Class_Body_ {
  Section* section;
  Class_Body next;
  int pos;
};

typedef struct {
  Tmpl_List list;
  Type_List base;
} Tmpl_Class;
Tmpl_Class* new_tmpl_class(const ID_List, const m_bool);
const m_bool tmpl_class_base(const Tmpl_Class*);
void free_tmpl_class(Tmpl_Class*);
struct Class_Def_ {
  ae_flag flag;
  ID_List name;
  Type_Decl* ext;
  Class_Body body;
  Type type;
  Nspc home;
  Tmpl_Class*  tmpl;
  int pos;
};
Class_Def new_class_def(ae_flag class_decl, ID_List name,
                        Type_Decl* ext, Class_Body body, int pos);
void free_class_def(Class_Def a);
Class_Body new_class_body(Section* section, Class_Body body, int pos);
void free_class_body(Class_Body a);
Section* new_section_class_def(Class_Def class_def, int pos);

struct Ast_ {
  Section* section;
  Ast next;
  int pos;
};
Ast new_ast(Section* section, Ast next, int pos);
Ast parse(m_str);
void free_ast();
#endif
