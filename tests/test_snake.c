/*
 ============================================================================
 Name        : test_snake.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Test file for Snake-by-InfinitePain
=============================================================================
*/

#include <check.h>
#include "snake.h"
#include "error_message.h"
#include "appdata.h"
#include "thread.h"
#include "terminal.h"
#include "timing_utils.h"
#include "app_status.h"

START_TEST(test_create_snake)
{
    int length = 5;
    int color = 2;
    int direction = MOVE_UP;
    int startx = 10;
    int starty = 10;

    Snake *pSnake = create_snake(length, color, direction, startx, starty);
    ck_assert_ptr_nonnull(pSnake);
    ck_assert_int_eq(pSnake->is_alive, true);
    ck_assert_int_eq(pSnake->point, 0);
    ck_assert_int_eq(pSnake->grow, length);
    ck_assert_int_eq(pSnake->color, color);
    ck_assert_int_eq(pSnake->pos_snake->head->pos.posx, startx);
    ck_assert_int_eq(pSnake->pos_snake->head->pos.posy, starty);

    delete_snake(pSnake);
}
END_TEST

START_TEST(test_set_snake_position)
{
    int length = 5;
    int color = 2;
    int direction = MOVE_UP;
    int startx = 10;
    int starty = 10;

    Snake *pSnake = create_snake(length, color, direction, startx, starty);
    ck_assert_ptr_nonnull(pSnake);

    int new_x = 12;
    int new_y = 15;
    set_snake_position(pSnake, new_x, new_y);
    ck_assert_int_eq(pSnake->pos_snake->head->pos.posx, new_x);
    ck_assert_int_eq(pSnake->pos_snake->head->pos.posy, new_y);

    delete_snake(pSnake);
}
END_TEST

START_TEST(test_set_snake_color)
{
    int length = 5;
    int color = 2;
    int direction = MOVE_UP;
    int startx = 10;
    int starty = 10;

    Snake *pSnake = create_snake(length, color, direction, startx, starty);
    ck_assert_ptr_nonnull(pSnake);

    int new_color = 3;
    set_snake_color(pSnake, new_color);
    ck_assert_int_eq(pSnake->color, new_color);

    delete_snake(pSnake);
}
END_TEST

START_TEST(test_is_key_reverse)
{
    ck_assert_int_eq(is_key_reverse(MOVE_UP, MOVE_DOWN), true);
    ck_assert_int_eq(is_key_reverse(MOVE_DOWN, MOVE_UP), true);
    ck_assert_int_eq(is_key_reverse(MOVE_LEFT, MOVE_RIGHT), true);
    ck_assert_int_eq(is_key_reverse(MOVE_RIGHT, MOVE_LEFT), true);
    ck_assert_int_eq(is_key_reverse(MOVE_UP, MOVE_LEFT), false);
    ck_assert_int_eq(is_key_reverse(MOVE_RIGHT, MOVE_DOWN), false);
}
END_TEST

Suite *snake_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Snake");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_create_snake);
    tcase_add_test(tc_core, test_set_snake_position);
    tcase_add_test(tc_core, test_set_snake_color);
    tcase_add_test(tc_core, test_is_key_reverse);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
	int number_failed = 0;

	{
		Suite* s = snake_suite();
		SRunner* sr = srunner_create(s);
		srunner_run_all(sr, CK_NORMAL);
		number_failed = srunner_ntests_failed(sr);
		srunner_free(sr);
	}

	return (number_failed == 0) ? 0 : 1;
}