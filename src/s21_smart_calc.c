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
            res = 4;
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            res = 3;
            break;
        case '*':
        case '/':
        case '^':
            res = 2;
            break;
        case '+':
        case '-':
            res = 1;
            break;
    }
    return res;
}

double s21_calculate(char *str) {
    char *postfix = s21_parser(str);
    double res = s21_compute(postfix);
    free(postfix);
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

            if ( *(ch + 1) && !(isdigit(*(ch + 1))) && (*(ch + 1) != '.') ){
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
                        printFunAndSpace(&ptr, top);
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
                char fun[len + 1];
                strlcpy(fun, ch, len + 1);
                cur =  getToken(fun);
                ch += len - 1;
            }
            if (isEmpty(st)) 
                push(&st, cur);
            else {
                while (!isEmpty(st)){
                    char top = pop(&st);
                    if (isOpenScope(top)) {
                        push(&st, top);
                        break;
                    }
                    else if (getPriority(top) < getPriority(cur)) {
                        push(&st, top);
                        break;
                    }   
                    else if (getPriority(top) >= getPriority(cur)) {
                        if (isdigit(top)){
                            printFunAndSpace(&ptr, top);
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
            printFunAndSpace(&ptr, top);
        } else {
            printChAndSpace(&ptr, top);
        }
    }
    *ptr = '\0';
    return result;
}

double s21_compute(char *str){
    stack st = NULL;
    double result = 0;
    for (char *p = str; *p; p++){
        if (isdigit(*p)){
            double number = s21_convertStrToNum(&p);
            pushNum(&st, number);
        } else if (isOperator(*p)){
            if (!(isEmpty(st))){
                computeOper(*p, &st);
            } 
        } else if (isComplexFun(p)){
            int len = isComplexFun(p);
            char fun[len + 1];
            strlcpy(fun, p, len + 1);
            p += len - 1;
            char token = getToken(fun);
            if  (!isEmpty(st)){
                computeOper(token, &st);
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
            c = pow(a, b);
            break;
        case '1':
            c = cos(b);
            break;
        case '2':
            c = sin(b);
            break;
        case '3':
            c = tan(b);
            break;
        case '4':
            c = acos(b);
            break;
        case '5':
            c = asin(b);
            break;
        case '6':
            c = atan(b);
            break;
        case '7':
            c = sqrt(b);
            break;
        case '8':
            c = log(b);
            break;
        case '9':
            c = log10(b);
            break;
    }
    if (isdigit(op)){ //is Unary operator
        pushNum(st, a);
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
        res = 4; 
    } else if (*str =='c' || *str == 't' || (*str == 's' && *(str + 1) == 'i') || (*str == 'l' && *(str + 1) == 'o')){
        res = 3; //cos tan sin log
    } else if (*str == 'l' && *(str + 1) == 'n'){
        res = 2;
    } 
    return res;
}


char getToken(char *str){
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
    return token;
}

void printFunAndSpace(char **ptr, char token){
    **ptr = '\0';
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
    if (token >= '4' && token <= '7'){
        *ptr += 5;
    } else if (token == '8'){
        *ptr += 3;
    } else if ((token >= '1' && token <= '3') || token == '9'){
        *ptr += 4;
    }
}
