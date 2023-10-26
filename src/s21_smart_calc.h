#ifndef S21_SMART_CALC_H
#define S21_SMART_CALC_H
#include <stdbool.h>

typedef struct node {
  char data;
  struct node *next;
} node;

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
int getPriority(char ch);
char *s21_parser(char *str);

//helpers
#endif