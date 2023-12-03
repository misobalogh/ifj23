/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#include "generator.h"
#include "global_variables.h"
#include "dynamic_string.h"
#include "istack.h"

static unsigned cmain = 0;
static bool inWhile = false;

IStack* blockStack;

extern char _binary_substring_code_start[];

// TODO: rewrite this with linked list or dynamic array
static struct { char* idname; unsigned id; } whileVariables[256] = { 0 };
static unsigned whileVariableCount = 0;

unsigned uid(void) {
  static unsigned state = 0;
  return state++;
}

void genWhileDeclaration(const char* idname) {
    if (whileVariables[whileVariableCount].idname == NULL) {
        whileVariables[whileVariableCount].idname = malloc(strlen(idname) + 1);
        CHECK_MEMORY_ALLOC(whileVariables[whileVariableCount].idname);
    }
    strcpy(whileVariables[whileVariableCount].idname, idname);

    unsigned id;
    global_symbolSearch(idname, &id);
    whileVariables[whileVariableCount].id = id;

    whileVariableCount++;
}

void genInit(void) {
    printf(".IFJcode23\n");
    blockStack = istackInit();
}

void genDeinit(void) {
    istackFree(blockStack);

    for (unsigned i = 0; i < sizeof(whileVariables) / sizeof(whileVariables[0]); i++) {
        free(whileVariables[i].idname);
    }
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
      unsigned id;
      global_symbolSearch(stringCStr(&params[i].name), &id);
      printf("WRITE %s@%s%i\n", l, stringCStr(&params[i].name), id);
    }
  }
}

void _genBuiltinCall(const char* idname, Param* params, unsigned paramCount) {
  if (strcmp(idname, "readString") == 0) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@temp_A\n");
    printf("READ TF@temp_A string\n");
    printf("PUSHS TF@temp_A\n");
  }
  else if (strcmp(idname, "readInt") == 0 ) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@temp_A\n");
    printf("READ TF@temp_A int\n");
    printf("PUSHS TF@temp_A\n");
  }
  else if (strcmp(idname, "readDouble") == 0) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@temp_A\n");
    printf("READ TF@temp_A float\n");
    printf("PUSHS TF@temp_A\n");
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
      unsigned id;
      global_symbolSearch(stringCStr(&params->name), &id);
      printf("PUSHS %s@%s%i\n", l, stringCStr(&params[0].name), id);
    }
    printf("INT2FLOATS\n");
  }
  else if (strcmp(idname, "Double2Int") == 0) {
    if (params[0].isConst) {
      printf("PUSHS float@%a\n", params[0].floatVal);
    }
    else {
      const char* l = global_isLocal(stringCStr(&params[0].name)) ? "LF" : "GF";
      unsigned id;
      global_symbolSearch(stringCStr(&params[0].name), &id);
      printf("PUSHS %s@%s%i\n", l, stringCStr(&params[0].name), id);
    }
    printf("FLOAT2INTS\n");
  }
  else if (strcmp(idname, "length") == 0) {
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@temp_A\n");

    if (params[0].isConst) {
      printf("STRLEN TF@temp_A string@");
      _printEscaped(stringCStr(&params[0].name));
      putchar('\n');
    }
    else {
      const char* l = global_isLocal(stringCStr(&params->name)) ? "LF" : "GF";
      unsigned id;
      global_symbolSearch(stringCStr(&params[0].name), &id);
      printf("STRLEN TF@temp_A %s@%s%i\n", l, stringCStr(&params[0].name), id);
    }

    printf("PUSHS TF@temp_A\n");
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
      unsigned id;
      global_symbolSearch(stringCStr(&params[0].name), &id);
      printf("PUSHS %s@%s%i\n", l, stringCStr(&params[0].name), id);
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
      unsigned id;
      global_symbolSearch(stringCStr(&params[0].name), &id);
      printf("PUSHS %s@%s%i\n", l, stringCStr(&params[0].name), id);
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

    printf("DEFVAR TF@%s-1\n", stringCStr(&funcParam.name));

    if (callParam.isConst) {
      if (callParam.type.base == 'I') {
        printf("MOVE TF@%s-1 int@%i\n", stringCStr(&funcParam.name), callParam.intVal);
      }
      else if (callParam.type.base == 'D') {
        printf("MOVE TF@%s-1 float@%a\n", stringCStr(&funcParam.name), callParam.floatVal);
      }
      else if (callParam.type.base == 'S') {
        printf("MOVE TF@%s-1 string@", stringCStr(&funcParam.name));
        _printEscaped(stringCStr(&callParam.name));
        putchar('\n');
      }
      else {
        EXIT_WITH_MESSAGE(INTERNAL_ERROR);
      }
    }
    else {
      const char* l = global_isLocal(stringCStr(&callParam.name)) ? "LF" : "GF";
      unsigned varId;
      global_symbolSearch(stringCStr(&callParam.name), &varId);
      printf("MOVE TF@%s-1 %s@%s%i\n", stringCStr(&funcParam.name), l, stringCStr(&callParam.name), varId);
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
    if (inWhile) {
        genWhileDeclaration(idname);
        return;
    }

    printf("# variable definition\n");
    if (global_isLocal(idname)) {
        unsigned id;
        global_symbolSearch(idname, &id);
        printf("DEFVAR LF@%s%i\n", idname, id);
    }
    else {
        unsigned id;
        global_symbolSearch(idname, &id);
    printf("DEFVAR GF@%s%i\n", idname, id);
    }
}

void genAssign(const char* idname) {
    printf("# variable assignment\n");
  if (global_isLocal(idname)) {
      unsigned id;
      global_symbolSearch(idname, &id);
    printf("POPS LF@%s%i\n", idname, id);
  }
  else {
      unsigned id;
      global_symbolSearch(idname, &id);
    printf("POPS GF@%s%i\n", idname, id);
  }
}

void genAssignId(const char* left, const char* right) {
    printf("# variable assignment from variable\n");
  const char* ll = global_isLocal(left) ? "LF" : "GF";
  const char* lr = global_isLocal(left) ? "LF" : "GF";
  unsigned leftId, rightId;
  global_symbolSearch(left, &leftId);
  global_symbolSearch(right, &rightId);
  printf("MOVE %s@%s%i %s@%s%i\n", ll, left, leftId, lr, right, rightId);
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
    unsigned id;
    global_symbolSearch(e.value.idName, &id);
    printf("PUSHS %s@%s%i\n", l, e.value.idName, id);
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
    printf("DEFVAR TF@temp_A\n");
    printf("DEFVAR TF@temp_B\n");
    printf("DEFVAR TF@temp_C\n");
    printf("POPS TF@temp_B\n");
    printf("POPS TF@temp_A\n");
    printf("MOVE TF@temp_C string@\n");
    printf("CONCAT TF@temp_C TF@temp_A TF@temp_B\n");
    printf("PUSHS TF@temp_C\n");
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
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@temp_A\n");
    printf("DEFVAR TF@temp_B\n");
    printf("DEFVAR TF@temp_C\n");
    printf("POPS TF@temp_B\n");
    printf("POPS TF@temp_A\n");
    printf("LT TF@temp_C TF@temp_A TF@temp_B\n");
    printf("PUSHS TF@temp_C\n");
    printf("EQ TF@temp_C TF@temp_A TF@temp_B\n");
    printf("PUSHS TF@temp_C\n");
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
    printf("DEFVAR TF@temp_left\n");
    printf("DEFVAR TF@temp_right\n");
    printf("POPS TF@temp_left\n");
    printf("POPS TF@temp_right\n");
    printf("JUMPIFNEQ default_not_null%04X TF@temp_left nil@nil\n", id);
    printf("PUSHS TF@temp_right\n");
    printf("JUMP default_end%04X\n", id);
    printf("LABEL default_end_null%04X\n", id);
    printf("PUSHS TF@temp_left\n");
    printf("LABEL default_end%04X\n", id);
   }
    break;
  case op_UNWRAP:
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
    break;
  }
}


void genIfBegin(void) {
    unsigned id = uid();
    istackPush(blockStack, id);

    printf("# if begin\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFEQS if%04X\n", id);
    printf("JUMP if_else%04X\n", id);
}

void genIfBlock(void) {
    printf("# if block\n");
    printf("LABEL if%04X\n", istackTop(blockStack));
}

void genIfElse(void) {
    printf("# if else\n");
    printf("JUMP if_end%04X\n", istackTop(blockStack));
    printf("LABEL if_else%04X\n", istackTop(blockStack));
}

void genIfEnd(void) {
    printf("# if end\n");
    printf("LABEL if_end%04X\n", istackTop(blockStack));

    istackPop(blockStack);
}

void genWhileBegin(void) {
    inWhile = true;
    whileVariableCount = 0;
    unsigned id = uid();
    istackPush(blockStack, id);

    printf("# while begin\n");
    printf("JUMP while_declaration%04X\n", istackTop(blockStack));
    printf("LABEL while_condition%04X\n", id);
}

void genWhileStats(void) {
    printf("# while condition\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS while_end%04X\n", istackTop(blockStack));
}

void genWhileEnd(void) {
    printf("# while end\n");
    printf("JUMP while_condition%04X\n", istackTop(blockStack));
    printf("LABEL while_declaration%04X\n", istackTop(blockStack));

    for (unsigned i = 0; i < whileVariableCount; i++) {
        const char* l = global_isLocal(whileVariables[i].idname) ? "LF" : "GF";
        printf("DEFVAR %s@%s%i\n", l, whileVariables[i].idname, whileVariables[i].id);
    }

    printf("JUMP while_condition%04X\n", istackTop(blockStack));
    printf("LABEL while_end%04X\n", istackTop(blockStack));

    istackPop(blockStack);
}

void genIfLet(const char* idname) {
    printf("# if let\n");
    const char* l = global_isLocal(idname) ? "LF" : "GF";
    unsigned id;
    global_symbolSearch(idname, &id);
    printf("PUSHS %s@%s%i\n", l, idname, id);
    printf("PUSHS nil@nil\n");
    printf("EQS\n");
    printf("NOTS\n");
}

void genIfLetShadow(const char* idname, unsigned oldId, unsigned newId) {
    printf("# if let shadowing\n");
    const char* l = global_isLocal(idname) ? "LF" : "GF";
    printf("DEFVAR %s@%s%i\n", l, idname, newId);
    printf("MOVE %s@%s%i %s@%s%i\n", l, idname, newId, l, idname, oldId);
}
