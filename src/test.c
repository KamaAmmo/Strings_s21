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
}

START_TEST(s21_parser_test){
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
  str = "1+2*(3-4/(5+6))";
  res = s21_parser(str);
  ck_assert_str_eq(res, "1 2 3 4 5 6 + / - * + ");
  free(res);
}

Suite *s21_smart_calc_tests_create() {
  Suite *s21_smart_calc = suite_create("s21_smart_calc");
  TCase *s21_smart_calc_tests = tcase_create("S21_SMART_CALC");

  tcase_add_test(s21_smart_calc_tests, s21_stack_test);
  tcase_add_test(s21_smart_calc_tests, s21_parser_test);

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
