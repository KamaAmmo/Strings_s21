#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "s21_smart_calc.h"


void display(stack top) {
  while (top != NULL) {
    putchar((*top).data);
    top = (*top).next;
  }
  putchar(' ');
}

char isEmpty(stack top) {
  if (top == NULL) return true;
  return false;
}

void push(stack *top, char info) {
  node *newnode = (node *)malloc(sizeof(node));
  newnode->data = info;
  newnode->next = *top;
  *top = newnode;
}

void pushNum(stack *top, double info){
  node *newnode = (node *)malloc(sizeof(node));
  newnode->num = info;
  newnode->next = *top;
  *top = newnode;
}

char pop(stack *top) {
  if (isEmpty(*top)) {
    // printf("Stack is empty");
    return -1;
  }
  char result = (*top)->data;
  node *temp = *top;
  *top = (*top)->next;
  free(temp);
  return result;
}

double popNum(stack *top) {
  if (isEmpty(*top)) {
    // printf("Stack is empty");
    return -1;
  }
  double result = (*top)->num;
  node *temp = *top;
  *top = (*top)->next;
  free(temp);
  return result;
}

char peak(stack top) { return (*top).data; }

void destroy(stack *top) {
  while (*top != NULL) {
    node *temp = (*top);
    *top = (*top)->next;
    free(temp);
  }
}

void pushStrToStack(stack *st, char *str){
    int len = strlen(str);
    for (int i = 0; i < len; i++){
        push(st, str[len - 1 - i]);
    }
}
