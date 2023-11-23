/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#include "generator.h"
#include "global_variables.h"
#include "dynamic_string.h"

static bool firstGenerated = false;
static bool entryGenerated = false;

extern char _binary_substring_code_start[];

void genSubstring(void) {
  printf("%s\n", _binary_substring_code_start);
}

void genFirst(void) {
  if (!firstGenerated) {
    printf("JUMP main\n");
    firstGenerated = true;
  }
}

void genEntry(void) {
  if (!entryGenerated) {
    printf("LABEL main\n");
    entryGenerated = true;
  }
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
        printf("WRITE string@%s\n", stringCStr(&params[i].name));
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
  if (strcmp(idname, "readString") == 0
    || strcmp(idname, "readInt") == 0
    || strcmp(idname, "readDouble") == 0) {
      printf("CREATEFRAME\n");
      printf("DEFVAR A\n");
      printf("READ TF@A\n");
      printf("PUSHS TF@A");
  }
  else if (strcmp(idname, "write") == 0) {
    _genWrite(params, paramCount);
  }
  else if (strcmp(idname, "Int2Double") == 0) {
    printf("PUSHS LF@%s\n", stringCStr(&params[0].name));
    printf("INT2FLOATS\n");
  }
  else if (strcmp(idname, "Double2Int") == 0) {
    printf("PUSHS LF@%s\n", stringCStr(&params[0].name));
    printf("FLOAT2INT\n");
  }
  else if (strcmp(idname, "length") == 0) {
    const char* l = global_isLocal(stringCStr(&params->name)) ? "LF" : "GF";
    printf("STRLEN %s@%s\n", l, stringCStr(&params[0].name));
  }
  else if (strcmp(idname, "substring") == 0) {
    printf("CALL substring");
  }
  else if (strcmp(idname, "ord") == 0) {
    printf("STR2INTS LF@c int@0\n");
  }
  else if (strcmp(idname, "chr") == 0) {
    printf("INT2CHARS LF@c\n");
  }
}

void genCall(const char* idname, Param* params, unsigned paramCount) {
  symtableItem* it = symtableSearch(global_table, idname);

  if (it->data.flags & FN_BUILTIN) {
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
        printf("MOVE TF@%s string@%s\n", stringCStr(&funcParam.name), stringCStr(&callParam.name));
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
      printf("PUSHS string@%s\n", e.value.constValue.value.STR_VAL);
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
    printf("POPS TF@A\n");
    printf("POPS TF@B\n");
    printf("POPS TF@C\n");
    printf("CONCAT TF@A TF@B TF@C\n");
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
  case op_DEFAULT:
    printf("not implemented\n");
    break;
  case op_UNWRAP:
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
    break;
  }
}
