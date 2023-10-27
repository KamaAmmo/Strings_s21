#include "s21_smart_calc.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>


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
        case '%':
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
        case '%':
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
    for (char *ch  = str; *ch != '\0'; ++ch){
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
                    printChAndSpace(&ptr, top);
                } 
                else if (top == '(') break;    
            }
        } else if (isOpenScope(cur)){
            push(&st, cur);
        } else if (isOperator(cur)){

            if (isEmpty(st)) 
                push(&st, cur);
            else {
                while (!isEmpty(st)){
                    char top = pop(&st);
                    if (isOpenScope(top)) {
                        break;
                    }
                    else if (getPriority(top) < getPriority(cur)) {
                        push(&st, top);
                        break;
                    }   
                    else if (getPriority(top) >= getPriority(cur)) {
                        printChAndSpace(&ptr, top);
                    }
                }
                push(&st, cur);
            } 
        }
    }
    while (!isEmpty(st)){
        char top = pop(&st);
        printChAndSpace(&ptr, top);
    }
    *ptr = '\0';
    return result;
}



double s21_Compute(char *str){
    stack st = NULL;
    for (char *p = str; *p; p++){
        if (isdigit(*p)){
            double number = s21_convertStrToNum(p);
            push(&st, num);
        }
        
    }
}

double s21_convertStrToNum(const char *str){
    char *p = (char *)str;
    for (; isdigit(*p) || *p == '.'; p++){}
    return strtod(str, &p);
}

void printChAndSpace(char **str, char top){
    **str = top;
    (*str)++;
    **str = ' ';
    (*str)++;
}

int isComplexFun(char *str, stack *st){
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

