#include <check.h>
#include <stdio.h>
#include "s21_smart_calc.h"
#include <stdlib.h>

START_TEST(s21_stack_test) {
  stack st = NULL;
  int value;
  push(&st, 1);
  value = peak(st);
  ck_assert_int_eq(value, 1);
  push(&st, 2);
  value = pop(&st);
  ck_assert_int_eq(value, 2);
  ck_assert_int_eq(isEmpty(st), 0);
  value = pop(&st);
  ck_assert_int_eq(value, 1);
  ck_assert_int_eq(isEmpty(st), 1);
  value = pop(&st);
  ck_assert_int_eq(value, -1);
  push(&st, 10);
  push(&st, 20);
  push(&st, 30);
  destroy(&st);

  pushNum(&st, 42.5);
  double val = popNum(&st);
  ck_assert_double_eq(val, 42.5);
}

START_TEST(s21_parser_for_elem_test){
  char *str = "1+2-3";
  char *res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 + 3 - ");

  str = "1*2/3";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 * 3 / ");

  str = "1+2*3";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 * + ");

  str = "1*2+3";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 * 3 + ");

  str = "1*2+3*4";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 * 3 4 * + ");

  str = "1*(2+3)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 + * ");

  str = "(1+2)*(3-4)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 + 3 4 - * ");

  str = "((1+2)*3)-4";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 + 3 * 4 - ");

  str = "1+2*(3-4/(10+6))";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 4 10 6 + / - * + ");

  free(res);
}

START_TEST(s21_compute_test){
  char *str = "1 2 + ";
  double res = s21_Compute(str);
  ck_assert_double_eq(res, 3);

  str = "1 2 + 3 -";
  res = s21_Compute(str);
  ck_assert_double_eq(res, 0);

  str = "3 2 * 3 / ";
  res = s21_Compute(str);
  ck_assert_double_eq(res, 2);

  str = "1 2 3 * + ";
  res = s21_Compute(str);
  ck_assert_double_eq(res, 7);

  str = "1 2 * 3 + ";
  res = s21_Compute(str);
  ck_assert_double_eq(res, 5);

  str = "1 2 * 3 4 * + ";
  res = s21_Compute(str);
  ck_assert_double_eq(res, 14);

  str = "1 2 3 + * ";
  res = s21_Compute(str);
  ck_assert_double_eq(res, 5);

  str = "1 2 + 3 4 - * ";
  res = s21_Compute(str);
  ck_assert_double_eq(res, -3);

  str = "1 2 + 3 * 4 - ";
  res = s21_Compute(str);
  ck_assert_double_eq(res, 5);

  str = "1 2 3 4 10 6 + / - * + ";
  res = s21_Compute(str);
  ck_assert_double_eq(res, 6.5);

  str = "5 -";
  res = s21_Compute(str);
  ck_assert_double_eq(res, -5);

  str = "2 3 ^";
  res = s21_Compute(str);
  ck_assert_double_eq(res, 8);

}

START_TEST(s21_convertStrToNum_test){
  char *str = "255";
  double res = s21_convertStrToNum(&str);
  ck_assert_double_eq(res, 255);
  str = "1";
  res = s21_convertStrToNum(&str);
  ck_assert_double_eq(res, 1);
  str = "12467";
  res = s21_convertStrToNum(&str);
  ck_assert_double_eq(res, 12467);
  str = "120.567tza";
  res = s21_convertStrToNum(&str);
  ck_assert_double_eq(res, 120.567);
}

START_TEST(s21_isComplexFun_test){
  char *str = "cos";
  int len = isComplexFun(str);
  ck_assert_int_eq(len, 3);

  str = "sqrt";
  len = isComplexFun(str);
  ck_assert_int_eq(len, 4);

  str = "sin";
  len = isComplexFun(str);
  ck_assert_int_eq(len, 3);

  str = "log";
  len = isComplexFun(str);
  ck_assert_int_eq(len, 3);

  str = "ln";
  len = isComplexFun(str);
  ck_assert_int_eq(len, 2);

  str = "atan";
  len = isComplexFun(str);
  ck_assert_int_eq(len, 4);
}


START_TEST(s21_parser_for_complex_test){
  char *str;
  char *res;
  str = "1+cos(2)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 cos + ");
  str = "1+atan(3)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 3 atan + ");
  str = "1+ln(2+3)";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 + ln + ");
}

Suite *s21_smart_calc_tests_create() {
  Suite *s21_smart_calc = suite_create("s21_smart_calc");
  TCase *s21_smart_calc_tests = tcase_create("S21_SMART_CALC");

  tcase_add_test(s21_smart_calc_tests, s21_stack_test);
  tcase_add_test(s21_smart_calc_tests, s21_parser_for_elem_test);
  tcase_add_test(s21_smart_calc_tests, s21_parser_for_complex_test);
  tcase_add_test(s21_smart_calc_tests, s21_convertStrToNum_test);
  tcase_add_test(s21_smart_calc_tests, s21_compute_test);
  tcase_add_test(s21_smart_calc_tests, s21_isComplexFun_test);


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
