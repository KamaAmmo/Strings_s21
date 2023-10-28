#include "s21_smart_calc.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>


bool isOperand(char ch){
    bool result = false;
    if ((ch >= 48 && ch <= 57) || (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122)) 
        result = true;
    return result;
}

bool isOpenScope(char ch){
    return true ? ch == '(' : false; 
}

bool isCloseScope(char ch){
    return true ? ch == ')' : false; 
}

bool isOperator(char ch){
    bool res;
    switch (ch)
    { 
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


int getPriority(char ch){
    int res;
    switch (ch)
    {
        case '(': 
            res = 3;
            break;
        case '*':
        case '/':
        case '^':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            res = 2;
            break;
        case '+':
        case '-':
            res = 1;
            break;
    }
    return res;
}

char *s21_parser(char *str){
    stack st = NULL;
    char *result = (char *)malloc(sizeof(char) * 255);
    char *ptr = result;
    for (char *ch  = str; *ch != '\0'; ch++){
        char cur = *ch;
        if (isdigit(cur) || cur == '.'){
            *ptr = cur;
            ptr++;
            if (*(ch + 1) && !(isdigit(*(ch + 1)))){
                *ptr = ' ';
                ptr++;
            } else if (!*(ch + 1)){
                *ptr = ' ';
                ptr++;
            }
        } else if (isCloseScope(cur)){
            while (!isEmpty(st)){
                char top = pop(&st);
                if (top != '('){
                    if (isdigit(top)){
                        printFunAndSpace(&ptr, top, result);
                    } else{
                        printChAndSpace(&ptr, top);
                    }
                } 
                else if (top == '(') break;    
            }
        } else if (isOpenScope(cur)){
            push(&st, cur);

        } else if (isOperator(cur) || isComplexFun(ch)){
            int len = isComplexFun(ch);
            if (len){
                // printf("\n%s\n", ch);
                char fun[len + 1];
                strlcpy(fun, ch, len + 1);
                // printf("\n%s\n", fun);
                cur =  getFun(fun, &st);
                // cur  = 'f';
                ch += len - 1 ;
                // printf("\n\n%s\n", ch);
            }
            if (isEmpty(st)) 
                push(&st, cur);
            else {
                while (!isEmpty(st)){
                    // int token = -1;
                    char top = pop(&st);
                    // if (isdigit(top)){
                    //     token = (int)top - 48;
                    //     // top = 'f';
                    // }
                    if (isOpenScope(top)) {
                        break;
                    }
                    else if (getPriority(top) < getPriority(cur)) {
                        push(&st, top);
                        break;
                    }   
                    else if (getPriority(top) >= getPriority(cur)) {
                        if (isdigit(top)){
                            printFunAndSpace(&ptr, top, result);
                        } else {
                            printChAndSpace(&ptr, top);
                        }
                    }
                }
                push(&st, cur);
            } 
        } 
        
    }
    while (!isEmpty(st)){
        char top = pop(&st);
        if (isdigit(top)){
            printFunAndSpace(&ptr, top, result);
        } else {
            printChAndSpace(&ptr, top);
        }
    }
    *ptr = '\0';
    return result;
}

char getFun(char *str, stack *st){
    char token = '0';
    if (strcmp(str, "cos") == 0){
        token = '1';
    } else if (strcmp(str, "sin") == 0){
        token = '2';
    } else if (strcmp(str, "tan") == 0){
        token = '3';
    } else if (strcmp(str, "acos") == 0){
        token = '4';
    } else if (strcmp(str, "asin") == 0){
        token = '5';
    } else if (strcmp(str, "atan") == 0){
        token = '6';
    } else if (strcmp(str, "sqrt") == 0){
        token = '7';
    } else if (strcmp(str, "ln") == 0){
        token = '8';
    } else if (strcmp(str, "log") == 0){
        token = '9';
    }
    // push(st, token);
    return token;
}

void printFunAndSpace(char **ptr, char token, char *res){
    // printf("\nsuka:%c\n", res);
    if (token == '1'){
        strcat(*ptr, "cos ");    
    } else if (token == '2'){
        strcat(*ptr, "sin ");
    }  else if (token == '3'){
        strcat(*ptr, "tan ");
    }  else if (token == '4'){
        strcat(*ptr, "acos ");
    }  else if (token == '5'){
        strcat(*ptr, "asin ");
    }  else if (token == '6'){
        strcat(*ptr, "atan ");
    }  else if (token == '7'){
        strcat(*ptr, "sqrt ");
    }  else if (token == '8'){
        strcat(*ptr, "ln ");
    }  else if (token == '9'){
        strcat(*ptr, "log ");
    }
    // printf(" \n\n    %c     ", token);
    if (token >= '4' && token <= '7'){
        *ptr += 5;
    } else if (token == '8'){
        *ptr += 3;
    } else if (token >= '1' && token <= '3'){
        *ptr += 4;
    }
    
}


double s21_Compute(char *str){
    stack st = NULL;
    double result = 0;
    for (char *p = str; *p; p++){
        if (isdigit(*p)){
            double number = s21_convertStrToNum(&p);
            pushNum(&st, number);
        }
        if (isOperator(*p)){
            if (!(isEmpty(st))){
                computeOper(*p, &st);
            } 
        }
    }
    result = popNum(&st);
    return result;
}

void computeOper(char op, stack *st){
    double b = popNum(st); 
    double a = 0;
    if (!(isEmpty(*st))){
        a = popNum(st);
    }
    double c;
    switch (op)
    {
        case '+':
            c = a + b;
            break;
        case '-':
            c = a - b;
            break;
        case '*':
            c = a * b;
            break;
        case '/':
            c =  a / b;
            break;
        case '^':
            c = a * b;
            break;
    }
    pushNum(st, c); 
}



double s21_convertStrToNum(char **str){
    char *p = *str;
    int len = 0;
    for (; isdigit(*p) || *p == '.'; len++, p++){}
    double result = strtod(*str, &p);
    *str = *(str) + len;
    return result;
}

void printChAndSpace(char **str, char top){
    **str = top;
    (*str)++;
    **str = ' ';
    (*str)++;
}

int isComplexFun(char *str){
    int res = 0;
    if (*str == 'a' || (*str == 's' && *(str + 1) == 'q')){
        // if (*(str + 1) == 'c' && *(str + 1) == 'o' && *(str + 1) == 's') || 
        // (*(str + 1) == 's' && *(str + 1) == 'i' && *(str + 1) == 'n') ||
        // (*(str + 1) == 't' && *(str + 1) == 'a' && *(str + 1) == 'n') {
        //     res = 4
        // }
        res = 4; 
    } else if (*str =='c' || *str == 't' || (*str == 's' && *(str + 1) == 'i') || (*str == 'l' && *(str + 1) == 'o')){
        res = 3; //cos tan sin log
    } else if (*str == 'l' && *(str + 1) == 'n'){
        res = 2;
    } 
    return res;
}


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

int main(){
    char *str = "1+cos2";
    char *res = s21_parser(str);
    printf(res);
}