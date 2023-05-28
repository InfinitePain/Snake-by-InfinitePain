/*
 ============================================================================
 Name        : test_input.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Test file for Snake-by-InfinitePain
=============================================================================
*/

#include <check.h>
#include "../include/input.h"
#include "../include/config.h"
#include "../include/appdata.h"

START_TEST(test_read_input)
{
	Config config = {
		.configs = {
			0, 0, 0, 0, 0,
			PLAYER_1_UP, PLAYER_1_LEFT, PLAYER_1_RIGHT, PLAYER_1_DOWN,
			PLAYER_2_UP, PLAYER_2_LEFT, PLAYER_2_RIGHT, PLAYER_2_DOWN,
			0, 0, 0, 0, 0, 0, 0
		}
    };
    appArgs.pConfig = &config;

    ck_assert_int_eq(read_input(PLAYER_1_UP), MOVE_UP);
    ck_assert_int_eq(read_input(PLAYER_1_LEFT), MOVE_LEFT);
    ck_assert_int_eq(read_input(PLAYER_1_RIGHT), MOVE_RIGHT);
    ck_assert_int_eq(read_input(PLAYER_1_DOWN), MOVE_DOWN);

    ck_assert_int_eq(read_input(PLAYER_2_UP), MOVE_UP);
    ck_assert_int_eq(read_input(PLAYER_2_LEFT), MOVE_LEFT);
    ck_assert_int_eq(read_input(PLAYER_2_RIGHT), MOVE_RIGHT);
    ck_assert_int_eq(read_input(PLAYER_2_DOWN), MOVE_DOWN);

    ck_assert_int_eq(read_input(-1), -1);
}
END_TEST

START_TEST(test_differentiator)
{
	Config config = {
		.configs = {
			0, 0, 0, 0, 0,
			PLAYER_1_UP, PLAYER_1_LEFT, PLAYER_1_RIGHT, PLAYER_1_DOWN,
			PLAYER_2_UP, PLAYER_2_LEFT, PLAYER_2_RIGHT, PLAYER_2_DOWN,
			0, 0, 0, 0, 0, 0, 0
		}
    };
    appArgs.pConfig = &config;

    ck_assert_int_eq(differentiator(PLAYER_1_UP), 1);
    ck_assert_int_eq(differentiator(PLAYER_1_LEFT), 1);
    ck_assert_int_eq(differentiator(PLAYER_1_RIGHT), 1);
    ck_assert_int_eq(differentiator(PLAYER_1_DOWN), 1);

    ck_assert_int_eq(differentiator(PLAYER_2_UP), 2);
    ck_assert_int_eq(differentiator(PLAYER_2_LEFT), 2);
    ck_assert_int_eq(differentiator(PLAYER_2_RIGHT), 2);
    ck_assert_int_eq(differentiator(PLAYER_2_DOWN), 2);

    ck_assert_int_eq(differentiator(-1), -1);
}
END_TEST

Suite* input_suite(void)
{
    Suite* s;
    TCase* tc_core;

    s = suite_create("Input");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_read_input);
    tcase_add_test(tc_core, test_differentiator);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
	int number_failed = 0;

	{
		Suite* s = input_suite();
		SRunner* sr = srunner_create(s);
		srunner_run_all(sr, CK_NORMAL);
		number_failed = srunner_ntests_failed(sr);
		srunner_free(sr);
	}
	
	return (number_failed == 0) ? 0 : 1;
}
