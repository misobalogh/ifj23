/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#include "generator.h"
#include "global_variables.h"
#include "dynamic_string.h"


void genFunction(const char* idname) {
  printf("LABEL function__%s\n", idname);
}

void genReturn(void) {
  printf("RETURN\n");
}

void genDef(const char* idname) {
  printf("DEFVAR LF@%s\n", idname);
}

void genAssign(const char* idname) {
  /* printf("# pop to %s\n", idname); */
  printf("POPS LF@%s\n", idname);
}

void genAssignId(const char* left, const char* right) {
  printf("MOVE LF@%s LF@%s\n", left, right);
}

void genExprOperand(ExprItem e) {
  if (e.type == expr_ID) {
    printf("PUSHS LF@%s\n", e.value.idName);
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
