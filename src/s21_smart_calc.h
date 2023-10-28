#ifndef S21_SMART_CALC_H
#define S21_SMART_CALC_H
#include <stdbool.h>

typedef struct node {
  char data;
  double num;
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
void pushNum(stack *top, double info);
double popNum(stack *st);
//RPN 
bool isOperand(char ch);
bool isOperator(char ch);
bool isOpenScope(char ch);
bool isCloseScope(char ch);
int isComplexFun(char *str);
int getPriority(char ch);
char *s21_parser(char *str);
double s21_compute(char *str);
void computeOper(char op, stack *st);
char getToken(char *str);
double s21_calculate(char *str);
//helpers
void printChAndSpace(char **str, char top);
void printFunAndSpace(char **ptr, char token);
double convertStrToNum(char **str);
#endif