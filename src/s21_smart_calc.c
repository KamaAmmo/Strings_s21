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

int getPriority(char *str){
    int res;
    if (strcmp(str, "(") == 0){
        res = 4;
    } else if (strcmp(str, "cos") == 0 || strcmp(str, "sin") == 0 || strcmp(str, "tan") == 0 
        || strcmp(str, "acos") == 0  || strcmp(str, "asin") == 0  || strcmp(str, "atan") == 0 
        || strcmp(str, "sqrt") == 0  || strcmp(str, "ln") == 0  || strcmp(str, "log") == 0  || strcmp(str, "pow") == 0 ){
            res = 3;
    } else if (strcmp(str, "*") == 0  || strcmp(str, "/") == 0  || strcmp(str, "mod") == 0 ){
        res = 2;
    } else if (strcmp(str, "+") == 0 || strcmp(str, "-") == 0 ){
        res = 1;
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
                char* top = pop(&st);
                if (strcmp(top, "(") != 0){
                    printFunAndSpace(&ptr, top);
                } 
                else if (strcmp(top, "(") == 0) {
                    free(top);  
                    break; 
                }
                free(top);   
            }
        } else if (isOpenScope(cur)){
            push(&st, "(");

        } else if (isOperator(cur) || isComplexFun(ch)){
            int len = isComplexFun(ch);
            char str[10];
            if (len){
                int i = 0;
                for (; i < len; ++i, ch++){
                    str[i] = *ch;
                }
                ch--;
                str[i] = '\0';
            } else {
                str[0] = cur;
                str[1] = '\0';
            }
            if (isEmpty(st)) 
                push(&st, str);
            else {
                while (!isEmpty(st)){
                    char *top = pop(&st);
                    if (isOpenScope(top[0])) {
                        push(&st, top);
                        free(top);
                        break;
                    }
                    else if (getPriority(top) < getPriority(str)) {
                        push(&st, top);
                        free(top);
                        break;
                    }   
                    else if (getPriority(top) >= getPriority(str)) {
                        printFunAndSpace(&ptr, top);
                        free(top);
                    }
                }
                push(&st, str);
            } 
        } 
    }
    while (!isEmpty(st)){
        char *top = pop(&st);
        printFunAndSpace(&ptr, top);
    }
    *ptr = '\0';
    return result;
}

double s21_compute(char *str){
    stack st = NULL;
    double result = 0;
    for (char *p = str; *p; p++){
        char str[5];
        str[0] = *p;
        str[1] = '\0';
        if (isdigit(*p)){
            double number = convertStrToNum(&p);
            pushNum(&st, number);
        } else if (isOperator(str[0])){
            if (!(isEmpty(st))){
                computeOper(str, &st);
            } 
        } else if (isComplexFun(p)){
            int len = isComplexFun(p);
            char fun[len + 1];
            strlcpy(fun, p, len + 1);
            p += len - 1;
            if  (!isEmpty(st)){
                computeOper(fun, &st);
            }
        }
    }
    result = popNum(&st);
    return result;
}

void computeOper(char *str, stack *st){
    double b = popNum(st); 
    double a = 0;
    bool is_unary = false;
    if (!(isEmpty(*st))){
        a = popNum(st);
    }
    double c;
    if (!strcmp(str, "+")){
        c = a + b;
    } else if (!strcmp(str, "-")){
        c = a - b;
    } else if (!strcmp(str, "*")){
        c = a * b;
    } else if (!strcmp(str, "/")){
        c = a / b;
    } else if (!strcmp(str, "mod")){
        c = fmod(a, b);
    } else if (!strcmp(str, "cos")){
        c = cos(b);
        is_unary = true;
    } else if (!strcmp(str, "sin")){
        c = sin(b);
        is_unary = true;
    } else if (!strcmp(str, "tan")){
        c = tan(b);
        is_unary = true;
    } else if (!strcmp(str, "acos")){
        c = acos(b);
        is_unary = true;
    } else if (!strcmp(str, "asin")){
        c = asin(b);
        is_unary = true;
    } else if (!strcmp(str, "atan")){
        c = atan(b);
        is_unary = true;
    } else if (!strcmp(str, "sqrt")){
        c = sqrt(b);
        is_unary = true;
    } else if (!strcmp(str, "ln")){
        c = log(b);
        is_unary = true;
    } else if (!strcmp(str, "log")){
        c = log10(b);
        is_unary = true;
    } else if (!strcmp(str, "pow")){
        c = pow(a, b);
    }

    if (is_unary){
        pushNum(st, a);
    }
    pushNum(st, c); 
}

double convertStrToNum(char **str){
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
    } else if (*str =='c' || *str == 't' || (*str == 's' && *(str + 1) == 'i') || (*str == 'l' && *(str + 1) == 'o') 
    || (*str == 'p' && *(str + 1) == 'o')){
        res = 3; 
    } else if (*str == 'l' && *(str + 1) == 'n'){
        res = 2;
    } 
    return res;
}

void printFunAndSpace(char **ptr, char *oper){
    **ptr = '\0';
    int len = strlen(oper);
    strcat(*ptr, oper);
    *ptr += len;
    **ptr = ' ';
    *ptr += 1; 
}
