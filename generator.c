/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */

#include "generator.h"
#include "global_variables.h"
#include "dynamic_string.h"
#include "istack.h"
#include "substring.h"

typedef struct WhileVariable {
    char* idname;
    unsigned id;
    bool local;
} WhileVariable;

static unsigned cmain = 0;
static bool inWhile = false;

// stack for uniquely undentifying block scopes
IStack* blockStack;

static unsigned whileVariablesCapacity = 8;
static WhileVariable* whileVariables;
static unsigned whileVariableCount = 0;
static unsigned outerWhileId = 0;

// get unique udentifier
unsigned uid(void) {
  static unsigned state = 0;
  return state++;
}

void genWhileDeclaration(const char* idname) {
    // grow array if necesarry
    if (whileVariableCount >= whileVariablesCapacity - 1) {
        whileVariablesCapacity *= 2;
        whileVariables = realloc(whileVariables, whileVariablesCapacity * sizeof(WhileVariable));
        CHECK_MEMORY_ALLOC(whileVariables);
        memset(whileVariables + whileVariableCount * sizeof(WhileVariable), 0,
                (whileVariablesCapacity - whileVariableCount) * sizeof(WhileVariable));
    }

    // copy the string
    if (whileVariables[whileVariableCount].idname == NULL) {
        whileVariables[whileVariableCount].idname = malloc(strlen(idname) + 1);
        CHECK_MEMORY_ALLOC(whileVariables[whileVariableCount].idname);
    }
    strcpy(whileVariables[whileVariableCount].idname, idname);

    // add symtable stack id
    unsigned id;
    global_symbolSearch(idname, &id);
    whileVariables[whileVariableCount].id = id;

    // add locality info
    whileVariables[whileVariableCount].local = global_isLocal(idname);

    whileVariableCount++;
}

void genInit(void) {
    printf(".IFJcode23\n");
    blockStack = istackInit();
    whileVariables = calloc(whileVariablesCapacity, sizeof(WhileVariable));
    CHECK_MEMORY_ALLOC(whileVariables);
    genSubstring();
}

void genDeinit(void) {
    istackFree(blockStack);

    for (unsigned i = 0; i < whileVariablesCapacity; i++) {
        free(whileVariables[i].idname);
    }
    free(whileVariables);
}

void genMainJump(void) {
  printf("JUMP main%i\n", cmain);
}

void genMainLabel(void) {
  printf("LABEL main%i\n", cmain);
  cmain++;
}

void genSubstring(void) {
    genMainJump();
    printf("%s\n", _binary_substring_code_start);
    genMainLabel();
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
    // variable
    else {
      const char* l = global_isLocal(stringCStr(&params[i].name)) ? "LF" : "GF";
      unsigned id;
      symtableItem* it = global_symbolSearch(stringCStr(&params[i].name), &id);
      if (it->data.flags & symbol_flag_PARAM) {
          printf("WRITE %s@$%i\n", l, it->data.paramCount);
      }
      else {
          printf("WRITE %s@%s%i\n", l, stringCStr(&params[i].name), id);
      }
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
      symtableItem* it = global_symbolSearch(stringCStr(&params->name), &id);
      if (it->data.flags & symbol_flag_PARAM) {
          printf("PUSHS %s@$%i\n", l, it->data.paramCount);
      }
      else {
          printf("PUSHS %s@%s%i\n", l, stringCStr(&params[0].name), id);
      }
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
      symtableItem* it = global_symbolSearch(stringCStr(&params[0].name), &id);
      if (it->data.flags & symbol_flag_PARAM) {
          printf("PUSHS LF@$%i\n", it->data.paramCount);
      }
      else {
          printf("PUSHS %s@%s%i\n", l, stringCStr(&params[0].name), id);
      }
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
      symtableItem* it = global_symbolSearch(stringCStr(&params[0].name), &id);
      if (it->data.flags & symbol_flag_PARAM) {
          printf("STRLEN TF@temp_A LF@$%i\n", it->data.paramCount);
      }
      else {
          printf("STRLEN TF@temp_A %s@%s%i\n", l, stringCStr(&params[0].name), id);
      }
    }

    printf("PUSHS TF@temp_A\n");
  }
  else if (strcmp(idname, "substring") == 0) {
    for (int i = 0; i < 3; i++) { // for each parameter
        if (params[i].isConst) {
            if (params[i].type.base == 'I') {
                printf("PUSHS int@%i\n", params[i].intVal);
            }
            else if (params[i].type.base == 'S') {
                printf("PUSHS string@");
                _printEscaped(stringCStr(&params[i].name));
                putchar('\n');
            }
            else {
                EXIT_WITH_MESSAGE(INTERNAL_ERROR);
            }
        }
        // param is variable
        else {
            const char* l = global_isLocal(stringCStr(&params[i].name)) ? "LF" : "GF";
            unsigned id;
            symtableItem* it = global_symbolSearch(stringCStr(&params[i].name), &id);
            if (it->data.flags & symbol_flag_PARAM) {
                printf("PUSHS LF@$%i\n", it->data.paramCount);
            }
            else {
                printf("PUSHS %s@%s%i\n", l, stringCStr(&params[i].name), id);
            }
        }
    }
    printf("CREATEFRAME\n");
    printf("DEFVAR TF@s-1\n");
    printf("DEFVAR TF@i-1\n");
    printf("DEFVAR TF@j-1\n");
    printf("POPS TF@j-1\n");
    printf("POPS TF@i-1\n");
    printf("POPS TF@s-1\n");
    printf("PUSHFRAME\n");
    printf("CALL func__substring\n");
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
      symtableItem* it = global_symbolSearch(stringCStr(&params[0].name), &id);
      if (it->data.flags & symbol_flag_PARAM) {
          printf("PUSHS LF@$%i\n", it->data.paramCount);
      }
      else {
          printf("PUSHS %s@%s%i\n", l, stringCStr(&params[0].name), id);
      }
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
#ifdef DEBUG
    printf("# call `%s`\n", idname);
#endif

    symtableItem* it = symtableSearch(global_table, idname);
    if (it != NULL && it->data.flags & symbol_flag_BUILTIN) {
        _genBuiltinCall(idname, params, paramCount);
        return;
    }

  printf("CREATEFRAME\n");

  for (unsigned i = 0; i < paramCount; i++) {
    Param callParam = params[i];

    printf("DEFVAR TF@$%i\n", i);

    if (callParam.isConst) {
      if (callParam.type.base == 'I') {
        printf("MOVE TF@$%i int@%i\n", i, callParam.intVal);
      }
      else if (callParam.type.base == 'D') {
        printf("MOVE TF@$%i float@%a\n", i, callParam.floatVal);
      }
      else if (callParam.type.base == 'S') {
        printf("MOVE TF@$%i string@", i);
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
      symtableItem* it = global_symbolSearch(stringCStr(&callParam.name), &varId);
      if (it->data.flags & symbol_flag_PARAM) {
          printf("MOVE TF@$%i LF@$%i\n", i, it->data.paramCount);
      }
      else {
          printf("MOVE TF@$%i %s@%s%i\n", i, l, stringCStr(&callParam.name), varId);
      }
    }
  }

  printf("PUSHFRAME\n");
  printf("CALL func__%s\n", idname);
  printf("POPFRAME\n");
#ifdef DEBUG
  printf("# end call `%s`\n", idname);
#endif
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

#ifdef DEBUG
    printf("# variable definition\n");
#endif
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
#ifdef DEBUG
    printf("# variable assignment\n");
#endif
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
#ifdef DEBUG
    printf("# variable assignment from variable\n");
#endif
  const char* ll = global_isLocal(left) ? "LF" : "GF";
  const char* lr = global_isLocal(left) ? "LF" : "GF";
  unsigned leftId, rightId;
  global_symbolSearch(left, &leftId);
  symtableItem* it = global_symbolSearch(right, &rightId);
  if (it->data.flags & symbol_flag_PARAM) {
      printf("MOVE %s@%s%i LF@$%i\n", ll, left, leftId, it->data.paramCount);
  }
  else {
      printf("MOVE %s@%s%i %s@%s%i\n", ll, left, leftId, lr, right, rightId);
  }
}

void _printEscaped(const char* str) {
  while (*str != '\0') {
    const char* fmt = "%c";
    if (*str <= ' ' || *str == '#' || *str == '\\') {
      fmt = "\\%03i";
    }

    printf(fmt, *str);
    str++;
  }
}

void genExprOperand(ExprItem e) {
  if (e.type == expr_ID) {
    const char* l = global_isLocal(e.value.idName) ? "LF" : "GF";
    unsigned id;
    symtableItem* it = global_symbolSearch(e.value.idName, &id);
    if (it->data.flags & symbol_flag_PARAM) {
        printf("PUSHS LF@$%i\n", it->data.paramCount);
    }
    else {
        printf("PUSHS %s@%s%i\n", l, e.value.idName, id);
    }
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
    else if (e.value.constValue.type.base == 'B') {
      printf("PUSHS bool@%s\n", e.value.constValue.value.INT_VAL ? "true" : "false");
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

void genSwitchStackTop(void) {
  printf("CREATEFRAME\n");
  printf("DEFVAR TF@temp_A\n");
  printf("DEFVAR TF@temp_B\n");
  printf("POPS TF@temp_A\n");
  printf("POPS TF@temp_B\n");
  printf("PUSHS TF@temp_A\n");
  printf("PUSHS TF@temp_B\n");
}

void genExprOperator(OperatorType optype) {
    switch (optype) {
    case op_PLUS:
#ifdef DEBUG
        printf("# plus\n");
#endif
        printf("ADDS\n");
        break;
    case op_MINUS:
#ifdef DEBUG
        printf("# minus\n");
#endif
        printf("SUBS\n");
        break;
    case op_MUL:
#ifdef DEBUG
        printf("# mul\n");
#endif
        printf("MULS\n");
        break;
    case op_DIV:
#ifdef DEBUG
        printf("# div\n");
#endif
        printf("DIVS\n");
        break;
    case op_IDIV:
#ifdef DEBUG
        printf("# int div\n");
#endif
        printf("IDIVS\n");
        break;
    case op_CONCAT:
#ifdef DEBUG
        printf("# concat\n");
#endif
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
#ifdef DEBUG
        printf("# equals\n");
#endif
        printf("EQS\n");
        break;
    case op_NEQ:
#ifdef DEBUG
        printf("# not equals\n");
#endif
        printf("EQS\n");
        printf("NOTS\n");
        break;
    case op_LESS:
#ifdef DEBUG
        printf("# less\n");
#endif
        printf("LTS\n");
        break;
  case op_MORE:
#ifdef DEBUG
        printf("# more\n");
#endif
        printf("GTS\n");
        break;
  case op_LESS_EQ:
#ifdef DEBUG
        printf("# less or equals\n");
#endif
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
#ifdef DEBUG
        printf("# more or equals\n");
#endif
        printf("CREATEFRAME\n");
        printf("DEFVAR TF@temp_A\n");
        printf("DEFVAR TF@temp_B\n");
        printf("DEFVAR TF@temp_C\n");
        printf("POPS TF@temp_B\n");
        printf("POPS TF@temp_A\n");
        printf("GT TF@temp_C TF@temp_A TF@temp_B\n");
        printf("PUSHS TF@temp_C\n");
        printf("EQ TF@temp_C TF@temp_A TF@temp_B\n");
        printf("PUSHS TF@temp_C\n");
        printf("ORS\n");
        break;
  case op_DEFAULT: {
#ifdef DEBUG
        printf("# default\n");
#endif
        unsigned id = uid();

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

#ifdef DEBUG
    printf("# if begin\n");
#endif
    printf("PUSHS bool@true\n");
    printf("JUMPIFEQS if%04X\n", id);
    printf("JUMP if_else%04X\n", id);
}

void genIfBlock(void) {
#ifdef DEBUG
    printf("# if block\n");
#endif
    printf("LABEL if%04X\n", istackTop(blockStack));
}

void genIfElse(void) {
#ifdef DEBUG
    printf("# if else\n");
#endif
    printf("JUMP if_end%04X\n", istackTop(blockStack));
    printf("LABEL if_else%04X\n", istackTop(blockStack));
}

void genIfEnd(void) {
#ifdef DEBUG
    printf("# if end\n");
#endif
    printf("LABEL if_end%04X\n", istackTop(blockStack));

    istackPop(blockStack);
}

void genWhileBegin(void) {
    unsigned id = uid();
    istackPush(blockStack, id);

    if (!inWhile) {
        outerWhileId = id;
        inWhile = true;
        whileVariableCount = 0;
    }

#ifdef DEBUG
    printf("# while begin\n");
#endif

    if (outerWhileId == id) {
        printf("JUMP while_declaration%04X\n", istackTop(blockStack));
    }
    printf("LABEL while_condition%04X\n", id);
}

void genWhileStats(void) {
#ifdef DEBUG
    printf("# while condition\n");
#endif
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS while_end%04X\n", istackTop(blockStack));
}

void genWhileEnd(void) {
#ifdef DEBUG
    printf("# while end\n");
#endif
    printf("JUMP while_condition%04X\n", istackTop(blockStack));

    if (istackTop(blockStack) == outerWhileId) {
        printf("LABEL while_declaration%04X\n", istackTop(blockStack));

        for (unsigned i = 0; i < whileVariableCount; i++) {
            const char* l = whileVariables[i].local ? "LF" : "GF";
            printf("DEFVAR %s@%s%i\n", l, whileVariables[i].idname, whileVariables[i].id);
        }

        printf("JUMP while_condition%04X\n", istackTop(blockStack));
    }

    printf("LABEL while_end%04X\n", istackTop(blockStack));

    istackPop(blockStack);
}

void genIfLet(const char* idname) {
#ifdef DEBUG
    printf("# if let\n");
#endif
    const char* l = global_isLocal(idname) ? "LF" : "GF";
    unsigned id;
    symtableItem* it = global_symbolSearch(idname, &id);
    if (it->data.flags & symbol_flag_PARAM) {
        printf("PUSHS LF@$%i\n", it->data.paramCount);
    }
    else {
        printf("PUSHS %s@%s%i\n", l, idname, id);
    }
    printf("PUSHS nil@nil\n");
    printf("EQS\n");
    printf("NOTS\n");
}

void genIfLetShadow(const char* idname, unsigned oldId, unsigned newId) {
#ifdef DEBUG
    printf("# if let shadowing\n");
#endif

    const char* l = global_isLocal(idname) ? "LF" : "GF";
    if (inWhile) {
        genWhileDeclaration(idname);
    }
    else {
        printf("DEFVAR %s@%s%i\n", l, idname, newId);
    }

    symtableItem* it = global_symbolSearch(idname, NULL);
    if (it->data.flags & symbol_flag_PARAM) {
        printf("MOVE %s@%s%i LF@$%i\n", l, idname, newId, it->data.paramCount);
    }
    else {
        printf("MOVE %s@%s%i %s@%s%i\n", l, idname, newId, l, idname, oldId);
    }
}
