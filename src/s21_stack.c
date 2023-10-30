#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "s21_smart_calc.h"


// void display(stack top) {
//   while (top != NULL) {
//     putchar((*top).data);
//     top = (*top).next;
//   }
//   putchar(' ');
// }

char isEmpty(stack top) {
  if (top == NULL) return true;
  return false;
}

void push(stack *top, char *info) {
  node *newnode = (node *)malloc(sizeof(node));
  // newnode->data = info;
  int i = 0;
  for (; i < strlen(info); ++i){
    newnode->data[i] = info[i];
  }
  newnode->data[i] = '\0';
  newnode->next = *top;
  *top = newnode;
}

void pushNum(stack *top, double info){
  node *newnode = (node *)malloc(sizeof(node));
  newnode->num = info;
  newnode->next = *top;
  *top = newnode;
}

char* pop(stack *top) {
  char *result = (char *)malloc(sizeof((*top)->data) * strlen((*top)->data));
  strcpy(result, (*top)->data);
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

void destroy(stack *top) {
  while (*top != NULL) {
    node *temp = (*top);
    *top = (*top)->next;
    free(temp);
  }
}

