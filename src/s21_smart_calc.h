#ifndef S21_SMART_CALC_H
#define S21_SMART_CALC_H
#include <stdbool.h>

typedef struct node {
  // struct storage data
  char data;
  struct node *next;
} node;

// struct storage {
//   char ch;
//   double num;
// }

typedef node *stack;
//stack
char isEmpty(stack top);
void push(stack *top, char data);
char pop(stack *top);
char peak(stack top);
void display(stack top);
void destroy(stack *top);

//RPN 
bool isOperand(char ch);
bool isOperator(char ch);
bool isOpenScope(char ch);
bool isCloseScope(char ch);
int isComplexFun(char *str, stack *st);
int getPriority(char ch);
char *s21_parser(char *str);

//helpers
void printChAndSpace(char **str, char top);
double s21_convertStrToNum(const char *str);
#endif