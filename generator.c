/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#include "generator.h"
#include "global_variables.h"
#include "dynamic_string.h"
#include "istack.h"

static unsigned cmain = 0;

static unsigned vertical_block = 0;
static unsigned horizontal_block = 0;

extern char _binary_substring_code_start[];

unsigned uid(void) {
  static unsigned state = 0;
  return state++;
}

void genInit(void) {
    printf(".IFJcode23\n");
}

void genDeinit(void) {
}

void genMainJump(void) {
  printf("JUMP main%i\n", cmain);
}

void genMainLabel(void) {
  printf("LABEL main%i\n", cmain);
  cmain++;
}

void genSubstring(void) {
  /* printf("%s\n", _binary_substring_code_start); */
}

void _genWrite(Param* params, unsigned paramCount) {
  for (unsigned i = 0; i < paramCount; i++) {
    if (params[i].isConst) {
      if (params[i].type.base == 'I') {
        printf("WRITE int@%i\n", params[i].intVal);
      }
      else if (params[i].type.base == 'D') {
        printf("WRITE float@%a\n", params[i].floatVal);
      }
      else if (params[i].type.base == 'S') {
        printf("WRITE string@");
        _printEscaped(stringCStr(&params[i].name));
        putchar('\n');
      }
      else {
        EXIT_WITH_MESSAGE(INTERNAL_ERROR);
      }
    }
    else {
      const char* l = global_isLocal(stringCStr(&params[i].name)) ? "LF" : "GF";
      printf("WRITE %s@%s\n", l, stringCStr(&params[i].name));
    }
  }
}

void _genBuiltinCall(const char* idname, Param* params, unsigned paramCount) {
  if (strcmp(idname, "readString") == 0) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@A\n");
    printf("READ TF@A string\n");
    printf("PUSHS TF@A\n");
  }
  else if (strcmp(idname, "readInt") == 0 ) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@A\n");
    printf("READ TF@A int\n");
    printf("PUSHS TF@A\n");
  }
  else if (strcmp(idname, "readDouble") == 0) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@A\n");
    printf("READ TF@A float\n");
    printf("PUSHS TF@A\n");
  }
  else if (strcmp(idname, "write") == 0) {
    _genWrite(params, paramCount);
  }
  else if (strcmp(idname, "Int2Double") == 0) {
    if (params[0].isConst) {
      printf("PUSHS int@%i\n", params[0].intVal);
    }
    else {
      const char* l = global_isLocal(stringCStr(&params->name)) ? "LF" : "GF";
      printf("PUSHS %s@%s\n", l, stringCStr(&params[0].name));
    }
    printf("INT2FLOATS\n");
  }
  else if (strcmp(idname, "Double2Int") == 0) {
    if (params[0].isConst) {
      printf("PUSHS float@%a\n", params[0].floatVal);
    }
    else {
      const char* l = global_isLocal(stringCStr(&params->name)) ? "LF" : "GF";
      printf("PUSHS %s@%s\n", l, stringCStr(&params[0].name));
    }
    printf("FLOAT2INTS\n");
  }
  else if (strcmp(idname, "length") == 0) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@A\n");

    if (params[0].isConst) {
      printf("STRLEN TF@A string@");
      _printEscaped(stringCStr(&params[0].name));
      putchar('\n');
    }
    else {
      const char* l = global_isLocal(stringCStr(&params->name)) ? "LF" : "GF";
      printf("STRLEN TF@A %s@%s\n", l, stringCStr(&params[0].name));
    }

    printf("PUSHS TF@A\n");
  }
  else if (strcmp(idname, "substring") == 0) {
    printf("CALL substring\n");
  }
  else if (strcmp(idname, "ord") == 0) {
    if (params->isConst) {
      printf("PUSHS string@");
      _printEscaped(stringCStr(&params[0].name));
      putchar('\n');
    }
    else {
      const char* l = global_isLocal(stringCStr(&params->name)) ? "LF" : "GF";
      printf("PUSHS %s@%s\n", l, stringCStr(&params[0].name));
    }
    printf("PUSHS int@0\n");
    printf("STRI2INTS\n");
  }
  else if (strcmp(idname, "chr") == 0) {
    if (params->isConst) {
      printf("PUSHS int@%i\n", params[0].intVal);
    }
    else {
      const char* l = global_isLocal(stringCStr(&params->name)) ? "LF" : "GF";
      printf("PUSHS %s@%s\n", l, stringCStr(&params[0].name));
    }
    printf("INT2CHARS\n");
  }
}

void genCall(const char* idname, Param* params, unsigned paramCount) {
    printf("# call `%s`\n", idname);
  symtableItem* it = symtableSearch(global_table, idname);

  if (it->data.flags & symbol_flag_BUILTIN) {
    _genBuiltinCall(idname, params, paramCount);
    return;
  }

  if (it == NULL || it->data.paramCount != paramCount) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  printf("CREATEFRAME\n");

  for (unsigned i = 0; i < paramCount; i++) {
    Param callParam = params[i];
    Param funcParam = it->data.params[i];

    printf("DEFVAR TF@%s\n", stringCStr(&funcParam.name));

    if (callParam.isConst) {
      if (callParam.type.base == 'I') {
        printf("MOVE TF@%s int@%i\n", stringCStr(&funcParam.name), callParam.intVal);
      }
      else if (callParam.type.base == 'D') {
        printf("MOVE TF@%s float@%a\n", stringCStr(&funcParam.name), callParam.floatVal);
      }
      else if (callParam.type.base == 'S') {
        printf("MOVE TF@%s string@", stringCStr(&funcParam.name));
        _printEscaped(stringCStr(&callParam.name));
        putchar('\n');
      }
      else {
        EXIT_WITH_MESSAGE(INTERNAL_ERROR);
      }
    }
    else {
      const char* l = global_isLocal(stringCStr(&callParam.name)) ? "LF" : "GF";
      printf("MOVE TF@%s %s@%s\n", stringCStr(&funcParam.name), l, stringCStr(&callParam.name));
    }
  }

  printf("PUSHFRAME\n");
  printf("CALL func__%s\n", idname);
  printf("POPFRAME\n");
  printf("# end call `%s`\n", idname);
}

void genFunction(const char* idname) {
  printf("LABEL func__%s\n", idname);
}

void genReturn(void) {
  printf("RETURN\n");
}

void genDef(const char* idname) {
  if (global_isLocal(idname)) {
    printf("DEFVAR LF@%s\n", idname);
  }
  else {
    printf("DEFVAR GF@%s\n", idname);
  }
}

void genAssign(const char* idname) {
  if (global_isLocal(idname)) {
    printf("POPS LF@%s\n", idname);
  }
  else {
    printf("POPS GF@%s\n", idname);
  }
}

void genAssignId(const char* left, const char* right) {
  const char* ll = global_isLocal(left) ? "LF" : "GF";
  const char* lr = global_isLocal(left) ? "LF" : "GF";
  printf("MOVE %s@%s %s@%s\n", ll, left, lr, right);
}

void _printEscaped(const char* str) {
  while (*str != '\0') {
    if (*str <= 32 || *str == 35 || *str == 92) {
      printf("\\%03i", *str);
    }
    else {
      putchar(*str);
    }

    str++;
  }
}

void genExprOperand(ExprItem e) {
  if (e.type == expr_ID) {
    const char* l = global_isLocal(e.value.idName) ? "LF" : "GF";
    printf("PUSHS %s@%s\n", l, e.value.idName);
  }
  else if (e.type == expr_CONST) {
    if (e.value.constValue.type.base == 'I') {
      printf("PUSHS int@%i\n", e.value.constValue.value.INT_VAL);
    }
    else if (e.value.constValue.type.base == 'D') {
      printf("PUSHS float@%a\n", e.value.constValue.value.FLOAT_VAL);
    }
    else if (e.value.constValue.type.base == 'S') {
      printf("PUSHS string@");
      _printEscaped(e.value.constValue.value.STR_VAL);
      putchar('\n');
    }
    else if (e.value.constValue.type.base == 'N') {
      printf("PUSHS nil@nil\n");
    }
    else {
      EXIT_WITH_MESSAGE(INTERNAL_ERROR);
    }
  }
  else if (e.type == expr_INTERMEDIATE) {
    return;
  }
  else {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }
}

void genExprOperator(OperatorType optype) {
  switch (optype) {
  case op_PLUS:
    printf("ADDS\n");
    break;
  case op_MINUS:
    printf("SUBS\n");
    break;
  case op_MUL:
    printf("MULS\n");
    break;
  case op_DIV:
    printf("DIVS\n");
    break;
  case op_CONCAT:
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@A\n");
    printf("DEFVAR TF@B\n");
    printf("DEFVAR TF@C\n");
    printf("POPS TF@B\n");
    printf("POPS TF@A\n");
    printf("MOVE TF@C string@\n");
    printf("CONCAT TF@C TF@A TF@B\n");
    printf("PUSHS TF@C\n");
    break;
  case op_EQ:
    printf("EQS\n");
    break;
  case op_NEQ:
    printf("EQS\n");
    printf("NOTS\n");
    break;
  case op_LESS:
    printf("LTS\n");
    break;
  case op_MORE:
    printf("GTS\n");
    break;
  case op_LESS_EQ:
    printf("LTS\n");
    printf("EQS\n");
    printf("ORS\n");
    break;
  case op_MORE_EQ:
    printf("GTS\n");
    printf("EQS\n");
    printf("ORS\n");
    break;
  case op_DEFAULT: {
    unsigned id = uid();
    printf("# default\n");
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@left\n");
    printf("DEFVAR TF@right\n");
    printf("POPS TF@left\n");
    printf("POPS TF@right\n");
    printf("JUMPIFNEQ default_not_null%i TF@left nil@nil\n", id);
    printf("PUSHS TF@right\n");
    printf("JUMP default_end%i\n", id);
    printf("LABEL default_end_null%i\n", id);
    printf("PUSHS TF@left\n");
    printf("LABEL default_end%i\n", id);
   }
    break;
  case op_UNWRAP:
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
    break;
  }
}


void genIfBegin(void) {
    horizontal_block++;

    printf("# if begin\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFEQS if%04X%04X\n", vertical_block, horizontal_block);
    printf("JUMP if_else%04X%04X\n", vertical_block, horizontal_block);
}

void genIfBlock(void) {
    printf("# if block\n");
    printf("LABEL if%04X%04X\n", vertical_block, horizontal_block);
}

void genIfElse(void) {
    printf("# if else\n");
    printf("JUMP if_end%04X%04X\n", vertical_block, horizontal_block);
    printf("LABEL if_else%04X%04X\n", vertical_block, horizontal_block);
}

void genIfEnd(void) {
    printf("# if end\n");
    printf("LABEL if_end%04X%04X\n", vertical_block, horizontal_block);

    horizontal_block--;
    vertical_block++;
}

void genWhileBegin(void) {
    horizontal_block++;

    printf("# while begin\n");
    printf("LABEL while_condition%04X%04X\n", vertical_block, horizontal_block);
}

void genWhileStats(void) {
    printf("# while condition\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS while_end%04X%04X\n", vertical_block, horizontal_block);
}

void genWhileEnd(void) {
    printf("# while end\n");
    printf("JUMP while_condition%04X%04X\n", vertical_block, horizontal_block);
    printf("LABEL while_end%04X%04X\n", vertical_block, horizontal_block);

    horizontal_block--;
    vertical_block++;
}

void genIfLet(const char* idname) {
    const char* l = global_isLocal(idname) ? "LF" : "GF";
    printf("PUSHS %s@%s\n", l, idname);
    printf("PUSHS nil@nil\n");
    printf("EQS\n");
    printf("NOTS\n");
}

