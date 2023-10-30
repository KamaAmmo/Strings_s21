#include <check.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_smart_calc.h"

START_TEST(s21_stack_test) {
  stack st = NULL;
  char *str = NULL;
  push(&st, "+");
  str = pop(&st);
  ck_assert_str_eq(str, "+");
  free(str);

  push(&st, "-");
  str = pop(&st);
  ck_assert_str_eq(str, "-");
  free(str);

  push(&st, "cos");
  str = pop(&st);
  ck_assert_str_eq(str, "cos");
  free(str);

  push(&st, "sqrt");
  str = pop(&st);
  ck_assert_str_eq(str, "sqrt");
  free(str);

  destroy(&st);
  pushNum(&st, 42.5);
  double val = popNum(&st);
  ck_assert_double_eq(val, 42.5);
  destroy(&st);
}

START_TEST(s21_convertStrToNum_test) {
  char *str = "255";
  double res = convertStrToNum(&str);
  ck_assert_double_eq(res, 255);
  str = "1";
  res = convertStrToNum(&str);
  ck_assert_double_eq(res, 1);
  str = "12467";
  res = convertStrToNum(&str);
  ck_assert_double_eq(res, 12467);
  str = "120.567tza";
  res = convertStrToNum(&str);
  ck_assert_double_eq(res, 120.567);
}

START_TEST(s21_isComplexFun_test) {
  char *str = "cos";
  int len = isFun(str);
  ck_assert_int_eq(len, 3);

  str = "sqrt";
  len = isFun(str);
  ck_assert_int_eq(len, 4);

  str = "sin";
  len = isFun(str);
  ck_assert_int_eq(len, 3);

  str = "log";
  len = isFun(str);
  ck_assert_int_eq(len, 3);

  str = "ln";
  len = isFun(str);
  ck_assert_int_eq(len, 2);

  str = "atan";
  len = isFun(str);
  ck_assert_int_eq(len, 4);

  
}

START_TEST(s21_parser_for_elem_test) {
  char *str = "1+2-3";
  char *res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 + 3 - ");
  free(res);

  str = "1*2/3";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 * 3 / ");
  free(res);

  str = "1+2*3";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 * + ");
  free(res);

  str = "1*2+3";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 * 3 + ");
  free(res);

  str = "1*2+3*4";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 * 3 4 * + ");
  free(res);

  str = "1*(2+3)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 + * ");
  free(res);

  str = "(1+2)*(3-4)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 + 3 4 - * ");
  free(res);

  str = "((1+2)*3)-4";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 + 3 * 4 - ");
  free(res);

  str = "1+2*(3-4/(10+6))";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 4 10 6 + / - * + ");
  free(res);

  str = "2.5*2";
  res = s21_parser(str);
  ck_assert_str_eq(res, "2.5 2 * ");
  free(res);

  str = "2*(12*2*3+25)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "2 12 2 * 3 * 25 + * ");
  free(res);

  str = "x+2";
  res = s21_parser(str);
  ck_assert_str_eq(res, "x 2 + ");
  free(res);

  str = "2*(12*x*x+25)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "2 12 x * x * 25 + * ");
  free(res);
}

START_TEST(s21_parser_for_complex_test) {
  char *str;
  char *res;

  str = "1+cos(2)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 cos + ");
  free(res);

  str = "cos(sqrt(2+5)/3)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "2 5 + sqrt 3 / cos ");
  free(res);

  str = "1+atan(3)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 3 atan + ");
  free(res);

  str = "1+ln(2+3)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 + ln + ");
  free(res);

  str = "1+ln(2+3*4)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 4 * + ln + ");
  free(res);

  str = "sqrt(12*2)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "12 2 * sqrt ");
  free(res);

  str = "sqrt(12.5*2*3+25)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "12.5 2 * 3 * 25 + sqrt ");
  free(res);

  str = "sin(log(3)+4)/(97*log(7))";
  res = s21_parser(str);
  ck_assert_str_eq(res, "3 log 4 + sin 97 7 log * / ");
  free(res);

  str = "sin(log(x)+4)/(x*log(7))";
  res = s21_parser(str);
  ck_assert_str_eq(res, "x log 4 + sin x 7 log * / ");
  free(res);

  str = "2*(97*log(7))";
  res = s21_parser(str);
  ck_assert_str_eq(res, "2 97 7 log * * ");
  free(res);

  str = "(2*(97*log(7)))^3";
  res = s21_parser(str);
  ck_assert_str_eq(res, "2 97 7 log * * 3 ^ ");
  free(res);

  str = "log(3)+4";
  res = s21_parser(str);
  ck_assert_str_eq(res, "3 log 4 + ");
  free(res);

  str = "sqrt(12.5*2*3+25)/2";
  res = s21_parser(str);
  ck_assert_str_eq(res, "12.5 2 * 3 * 25 + sqrt 2 / ");
  free(res);

  str = "sqrt(x*2*3+x)/2";
  res = s21_parser(str);
  ck_assert_str_eq(res, "x 2 * 3 * x + sqrt 2 / ");
  free(res);

  str = "sin(log(3)+4)/(97*log(7))/x*12";
  res = s21_parser(str);
  ck_assert_str_eq(res, "3 log 4 + sin 97 7 log * / x / 12 * ");
  free(res);
}

START_TEST(s21_compute_test) {
  char *str = "1 2 + ";
  double res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 3);

  str = "1 2 + 3 -";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 0);

  str = "3 2 * 3 / ";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 2);

  str = "1 2 3 * + ";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 7);

  str = "1 2 * 3 + ";
  res = s21_compute(str, NULL);
  ;
  ck_assert_double_eq(res, 5);

  str = "1 2 * 3 4 * + ";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 14);

  str = "1 2 3 + * ";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 5);

  str = "1 2 + 3 4 - * ";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, -3);

  str = "1 2 + 3 * 4 - ";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 5);

  str = "1 2 3 4 10 6 + / - * + ";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 6.5);

  str = "5 -";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, -5);

  str = "2 3 ^";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 8);

  str = "2.5 2 * ";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 5);
}

START_TEST(s21_compute_functions_test) {
  char *str = "2 cos ";
  double res = s21_compute(str, NULL);
  ck_assert_double_eq(res, cos(2));

  str = "1 3 cos +";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 1 + cos(3));

  str = "1 3 atan +";
  res = s21_compute(str, NULL);
  ;
  ck_assert_double_eq(res, 1 + atan(3));

  str = "1 2 3 + ln +";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 1 + log(2 + 3));

  str = "1 2 3 4 * + ln +";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, 1 + log(2 + 3 * 4));

  str = "3 ln 4 + sin 97 7 ln * / ";
  res = s21_compute(str, NULL);
  ck_assert_double_eq(res, sin(log(3) + 4) / (97 * log(7)));
}

START_TEST(s21_compute_with_x_test) {
  char *str;
  double res, num = 10.253;
  double *x = &num;

  str = "x cos ";
  res = s21_compute(str, x);
  ck_assert_double_eq(res, cos(*x));

  str = "1 x cos +";
  res = s21_compute(str, x);
  ck_assert_double_eq(res, 1 + cos(*x));

  str = "x x atan +";
  res = s21_compute(str, x);
  ;
  ck_assert_double_eq(res, *x + atan(*x));

  str = "1 x x + ln +";
  res = s21_compute(str, x);
  ck_assert_double_eq(res, 1 + log(*x + *x));

  str = "1 x x x * + ln +";
  res = s21_compute(str, x);
  ck_assert_double_eq(res, 1 + log(*x + *x * *x));

  str = "x ln 4 + sin x x ln * / ";
  res = s21_compute(str, x);
  ck_assert_double_eq(res, sin(log(*x) + 4) / (*x * log(*x)));
}

START_TEST(s21_final_test) {
  char *str = "1+2*3";
  double res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, 1 + 2 * 3);

  str = "5*32/2";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, 5 * 32 / 2);

  str = "sin(14/7)";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, sin(14 / 7));

  str = "2^3 * 2";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, pow(2, 3) * 2);

  str = "(2*(97*log(7)))^3";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, pow(2 * (97 * log10(7)), 3));

  str = "150 mod 20";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, 150 % 20);

  str = "150.2 mod 20.1";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, fmod(150.2, 20.1));

  str = "sqrt(12.5*2)";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, sqrt(12.5 * 2));

  str = "sqrt(12.5*2*3+25)";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, sqrt(12.5 * 2 * 3 + 25));

  str = "cos(sqrt(12.5*2*3+25)/2)*4.5";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, cos(sqrt(12.5 * 2 * 3 + 25) / 2) * 4.5);

  str = "sin(ln(3)+4)/(97*ln(7))";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, sin(log(3) + 4) / (97 * log(7)));

  str = "acos(1) + asin(0)";
  res = s21_smart_calc(str, NULL);
  ck_assert_double_eq(res, acos(1) + asin(0));
}

START_TEST(s21_final_with_x_test) {
  char *str;
  double res, num = 420.69;
  double *x = &num;

  str = "1+x*x";
  res = s21_smart_calc(str, x);
  ck_assert_double_eq(res, 1 + *x * *x);

  str = "5*x/2";
  res = s21_smart_calc(str, x);
  ck_assert_double_eq(res, 5 * *x / 2);

  str = "sin(x/7)";
  res = s21_smart_calc(str, x);
  ck_assert_double_eq(res, sin(*x / 7));

  str = "sqrt(12.5*x)";
  res = s21_smart_calc(str, x);
  ck_assert_double_eq(res, sqrt(12.5 * *x));

  str = "sqrt(12.5*2*x+x)";
  res = s21_smart_calc(str, x);
  ck_assert_double_eq(res, sqrt(12.5 * 2 * *x + *x));

  str = "cos(sqrt(12.5*x*3+x)/2)*4.5/x";
  res = s21_smart_calc(str, x);
  ck_assert_double_eq(res, cos(sqrt(12.5 * *x * 3 + *x) / 2) * 4.5 / *x);

  str = "sin(ln(x)+4)/(x*ln(7))";
  res = s21_smart_calc(str, x);
  ck_assert_double_eq(res, sin(log(*x) + 4) / (*x * log(7)));

  str =
      "x*cos(3 * x * 3.14) / 2 + sin(9 * x *3.14) / 4 + cos(27 * x * 3.14) / 8 "
      "+ tan(81 * x * 3.14) / 16)";
  res = s21_smart_calc(str, x);
  ck_assert_double_eq(res,
                      *x * cos(3 * *x * 3.14) / 2 + sin(9 * *x * 3.14) / 4 +
                          cos(27 * *x * 3.14) / 8 + tan(81 * *x * 3.14) / 16);
}

START_TEST(s21_correct_input_test){
  char *str = " 2 * at";
  bool res = s21_isCorrectInput(str);
  ck_assert_int_eq(res, 0);

  str = "2*sqrt(x)";
  res = s21_isCorrectInput(str);
  ck_assert_int_eq(res, 1);

  str = "(1+2)(";
  res = s21_isCorrectInput(str);
  ck_assert_int_eq(res, 0);

  str = ")(1+2)";
  res = s21_isCorrectInput(str);
  ck_assert_int_eq(res, 0);

}

START_TEST(s21_edge_cases_test){
  char *str;
  double res;

  str = " 1 / 0";
  res = s21_smart_calc(str, NULL);
  ck_assert_int_eq(isnan(res), 1);

  str =  "ln(-2)";
  res = s21_smart_calc(str, NULL);
  ck_assert_int_eq(isnan(res), 1);

  str = "sqrt(-1)";
  res = s21_smart_calc(str, NULL);
  ck_assert_int_eq(isnan(res), 1);

  str = "log(-5)";
  res = s21_smart_calc(str, NULL);
  ck_assert_int_eq(isnan(res), 1);

  str = "acos(-5)";
  res = s21_smart_calc(str, NULL);
  ck_assert_int_eq(isnan(res), 1);

  str = "asin(5)";
  res = s21_smart_calc(str, NULL);
  ck_assert_int_eq(isnan(res), 1);

}

Suite *s21_smart_calc_tests_create() {
  Suite *s21_smart_calc = suite_create("s21_smart_calc");
  TCase *s21_smart_calc_tests = tcase_create("S21_SMART_CALC");

  tcase_add_test(s21_smart_calc_tests, s21_stack_test);
  tcase_add_test(s21_smart_calc_tests, s21_parser_for_elem_test);
  tcase_add_test(s21_smart_calc_tests, s21_parser_for_complex_test);
  tcase_add_test(s21_smart_calc_tests, s21_convertStrToNum_test);
  tcase_add_test(s21_smart_calc_tests, s21_isComplexFun_test);
  tcase_add_test(s21_smart_calc_tests, s21_compute_test);
  tcase_add_test(s21_smart_calc_tests, s21_compute_functions_test);
  tcase_add_test(s21_smart_calc_tests, s21_compute_with_x_test);
  tcase_add_test(s21_smart_calc_tests, s21_final_test);
  tcase_add_test(s21_smart_calc_tests, s21_final_with_x_test);
  tcase_add_test(s21_smart_calc_tests, s21_edge_cases_test);
  tcase_add_test(s21_smart_calc_tests, s21_correct_input_test);

  
  suite_add_tcase(s21_smart_calc, s21_smart_calc_tests);

  return s21_smart_calc;
}

int main() {
  Suite *s21_smart_calc = s21_smart_calc_tests_create();
  SRunner *s21_smart_calc_runner = srunner_create(s21_smart_calc);
  srunner_run_all(s21_smart_calc_runner, CK_NORMAL);
  srunner_ntests_failed(s21_smart_calc_runner);
  srunner_free(s21_smart_calc_runner);

  return 0;
}
