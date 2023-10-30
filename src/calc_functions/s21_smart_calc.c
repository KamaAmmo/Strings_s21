#include "s21_smart_calc.h"

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool isOpenScope(char ch) { return true ? ch == '(' : false; }

bool isCloseScope(char ch) { return true ? ch == ')' : false; }

bool isOperator(char ch) {
  bool res;
  switch (ch) {
    case '+':
    case '-':
    case '^':
    case '*':
    case '/':
      res = true;
      break;
    default:
      res = false;
  }
  return res;
}

int getPriority(char *str) {
  int res = 0;
  if (strcmp(str, "cos") == 0 || strcmp(str, "sin") == 0 ||
      strcmp(str, "tan") == 0 || strcmp(str, "acos") == 0 ||
      strcmp(str, "asin") == 0 || strcmp(str, "atan") == 0 ||
      strcmp(str, "sqrt") == 0 || strcmp(str, "ln") == 0 ||
      strcmp(str, "log") == 0 || strcmp(str, "^") == 0) {
    res = 3;
  } else if (strcmp(str, "*") == 0 || strcmp(str, "/") == 0 ||
             strcmp(str, "mod") == 0) {
    res = 2;
  } else if (strcmp(str, "+") == 0 || strcmp(str, "-") == 0) {
    res = 1;
  }
  return res;
}

void printChar(char **write, char *read) {
  **write = *read;
  (*write)++;
  // если след не цифра и не . тогда ставим пробел // после числа должны быть
  // пробелы
  if (!(isdigit(*(read + 1))) && (*(read + 1) != '.')) {
    **write = ' ';
    (*write)++;
  }
}

void convertChToStr(char *dest, char **src, int len) {
  if (len) {
    strncpy(dest, *src, len);
    dest[len] = '\0';
    *src += len - 1;
  } else {
    dest[0] = **src;
    dest[1] = '\0';
  }
}

double convertStrToNum(char **str) {
  char *p = *str;
  int len = 0;
  for (; isdigit(*p) || *p == '.'; len++, p++) {
  }
  double result = strtod(*str, &p);
  *str = *(str) + len;
  return result;
}

int isFun(char *str) {
  int res = 0;
  if (!strncmp(str, "acos", 4) || !strncmp(str, "asin", 4) ||
      !strncmp(str, "atan", 4) || !strncmp(str, "sqrt", 4)) {
    res = 4;
  } else if (!strncmp(str, "cos", 3) || !strncmp(str, "sin", 3) ||
             !strncmp(str, "tan", 3) || !strncmp(str, "log", 3) ||
             !strncmp(str, "mod", 3)) {
    res = 3;
  } else if (!strncmp(str, "ln", 2)) {
    res = 2;
  }
  return res;
}

void printOper(char **ptr, char *oper) {
  **ptr = '\0';
  int len = strlen(oper);
  strcat(*ptr, oper);
  *ptr += len;
  **ptr = ' ';
  *ptr += 1;
}

void computeOper(char *str, stack *st, double a, double b) {
  bool is_unary = false;

  double c;
  if (!strcmp(str, "+")) {
    c = a + b;
  } else if (!strcmp(str, "-")) {
    c = a - b;
  } else if (!strcmp(str, "*")) {
    c = a * b;
  } else if (!strcmp(str, "/") && b != 0) {
    c = a / b;
  } else if (!strcmp(str, "mod") && b != 0) {
    c = fmod(a, b);
  } else if (!strcmp(str, "cos")) {
    c = cos(b);
    is_unary = true;
  } else if (!strcmp(str, "sin")) {
    c = sin(b);
    is_unary = true;
  } else if (!strcmp(str, "tan")) {
    c = tan(b);
    is_unary = true;
  } else if (!strcmp(str, "acos") && ((b >= -1) && (b <= 1))) {
    c = acos(b);
    is_unary = true;
  } else if (!strcmp(str, "asin") && ((b >= -1) && (b <= 1))) {
    c = asin(b);
    is_unary = true;
  } else if (!strcmp(str, "atan")) {
    c = atan(b);
    is_unary = true;
  } else if (!strcmp(str, "sqrt") && b >= 0) {
    c = sqrt(b);
    is_unary = true;
  } else if (!strcmp(str, "ln") && b >= 0) {
    c = log(b);
    is_unary = true;
  } else if (!strcmp(str, "log") && b >= 0) {
    c = log10(b);
    is_unary = true;
  } else if (!strcmp(str, "^")) {
    c = pow(a, b);
  } else {
    c = NAN;
  }

  if (is_unary) {
    pushNum(st, a);
  }
  pushNum(st, c);
  return;
}

double s21_compute(char *str, double *x) {
  stack st = NULL;
  double result = 0;
  for (char *p = str; *p; p++) {
    char ptr[5];
    ptr[0] = *p;
    ptr[1] = '\0';
    if (isdigit(*p)) {
      double number = convertStrToNum(&p);
      pushNum(&st, number);
    } else if (*p == 'x' && x != NULL) {
      pushNum(&st, *x);
    } else if (isOperator(ptr[0])) {
      if (!isCorrectVal(ptr, &st)) {
        break;
      }
    } else if (isFun(p)) {
      int len = isFun(p);
      char fun[len + 1];
      strncpy(fun, p, len);
      fun[len] = '\0';
      p += len - 1;
      if (!isCorrectVal(fun, &st)) {
        break;
      }
    }
  }
  result = popNum(&st);
  destroy(&st);
  return result;
}
bool isCorrectVal(char *str, stack *st) {
  if (!isEmpty(*st)) {
    doOperation(str, st);
  }
  bool result = true;
  if (isnan(peak(*st))) {
    result = false;
  }
  return result;
}
void doOperation(char *str, stack *st) {
  double b = popNum(st);
  double a = 0;
  if (!(isEmpty(*st))) {
    a = popNum(st);
  }
  computeOper(str, st, a, b);
}

char *s21_parser(char *str) {
  stack st = NULL;
  char *result = (char *)malloc(sizeof(char) * 255);
  char *ptr = result;
  for (char *ch = str; *ch != '\0'; ++ch) {
    char cur = *ch;
    if (isdigit(cur) || cur == '.' || cur == 'x') {
      printChar(&ptr, ch);
    } else if (isCloseScope(cur)) {
      while (!isEmpty(st)) {
        char *top = pop(&st);
        if (strcmp(top, "(") != 0) {
          printOper(&ptr, top);
        } else if (strcmp(top, "(") == 0) {
          free(top);
          break;
        }
        free(top);
      }
    } else if (isOpenScope(cur)) {
      push(&st, "(");

    } else if (isOperator(cur) || isFun(ch)) {
      int len = isFun(ch);
      char str[10];
      convertChToStr(str, &ch, len);
      if (isEmpty(st))
        push(&st, str);
      else {
        while (!isEmpty(st)) {
          char *top = pop(&st);
          if (isOpenScope(top[0])) {
            push(&st, top);
            free(top);
            break;
          } else if (getPriority(top) < getPriority(str)) {
            push(&st, top);
            free(top);
            break;
          } else if (getPriority(top) >= getPriority(str)) {
            printOper(&ptr, top);
          }
          free(top);
        }
        push(&st, str);
      }
    }
  }
  while (!isEmpty(st)) {
    char *top = pop(&st);
    printOper(&ptr, top);
    free(top);
  }
  *ptr = '\0';
  return result;
}

double s21_smart_calc(char *str, double *x) {
  char *postfix = s21_parser(str);
  double res;
  if (x == NULL) {
    res = s21_compute(postfix, NULL);
  } else {
    res = s21_compute(postfix, x);
  }
  free(postfix);
  return res;
}

bool s21_isCorrectInput(char *str) {
  bool result = true;
  int scopes = 0;
  int count_dots = 0;
  for (char *p = str; *p; p++) {
    if (!isdigit(*p) && *p != ' ' && !isOperator(*p) && !isOpenScope(*p) &&
        !isCloseScope(*p) && *p != 'x' && *p != '.') {
      int is_fun = isFun(p);
      if (!is_fun) {
        result = false;
        break;
      } else {
        p += is_fun - 1;
      }
    }
    if (*p == '.') {
      count_dots++;
      if (count_dots > 1 || !isdigit(*(p - 1))) {
        result = false;
        break;
      }
    } else if (!isdigit(*p) && *p != 'x') {
      count_dots = 0;
    }
    if (*p == 'x' && *(p + 1) == 'x') {
      result = false;
      break;
    }

    if (isOpenScope(*p)) {
      scopes++;
    } else if (isCloseScope(*p)) {
      scopes--;
    }
    if (scopes < 0) {
      result = false;
      break;
    }
  }
  if (scopes != 0) {
    result = false;
  }
  return result;
}
