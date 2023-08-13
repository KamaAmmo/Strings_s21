#ifndef S21_SMART_CALC_H
#define S21_SMART_CALC_H

typedef struct node {
  char data;
  struct node *next;
} node;

typedef node *stack;

char isEmpty(stack top);
void push(stack *top, char data);
char pop(stack *top);
char peak(stack top);
void display(stack top);
void destroy(stack *top);

#endif