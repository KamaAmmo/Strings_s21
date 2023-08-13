#include <check.h>

#include "s21_smart_calc.h"

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
  push(&st, 10);
  push(&st, 20);
  push(&st, 30);
  destroy(&st);
}

Suite *s21_smart_calc_tests_create() {
  Suite *s21_smart_calc = suite_create("s21_smart_calc");
  TCase *s21_smart_calc_tests = tcase_create("S21_SMART_CALC");

  tcase_add_test(s21_smart_calc_tests, s21_stack_test);

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
