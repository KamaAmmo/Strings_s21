#include "s21_smart_calc.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>



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
                    *ptr = top;
                    ptr++;
                    *ptr = ' ';
                    ptr++;
                } 
                else if (top == '(') break;
                
            }
        } else if (isOpenScope(cur)){
            push(&st, cur);
        } else if (isOperator(cur)){

            if (isEmpty(st)) push(&st, cur);
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
                        *ptr = top;
                        ptr++;
                        *ptr = ' ';
                        ptr++;
                    }
                    
                }
                push(&st, cur);
            } 
        }
    }
    while (!isEmpty(st)){
        char top = pop(&st);
        *ptr = top;
        ptr++;
        *ptr = ' ';
        ptr++;
    }
    *ptr = '\0';
    return result;
}

// int main(){
//     char *str = "1*2/3";
//     char *res = s21_parser(str);
//     printf(res);
// }