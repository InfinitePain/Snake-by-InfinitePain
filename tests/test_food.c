/*
 ============================================================================
 Name        : test_food.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Test file for Snake-by-InfinitePain
=============================================================================
*/

#include <check.h>
#include "../include/food.h"
#include "../include/appdata.h"

START_TEST(test_delete_available_positions)
{
    int width = 10;
    int height = 10;
    AvailablePositions* ap = create_available_positions(width, height);
    ck_assert_ptr_nonnull(ap);
    delete_available_positions(ap);
}
END_TEST

START_TEST(test_create_available_positions)
{
    int width = 10;
    int height = 10;
    AvailablePositions* ap = create_available_positions(width, height);
    ck_assert_ptr_nonnull(ap);
    ck_assert_ptr_nonnull(ap->positions);
    ck_assert_int_gt(ap->count, 0);
    ck_assert_int_eq(ap->count, (width - 2) * (height - 2));
    delete_available_positions(ap);
}
END_TEST

START_TEST(test_remove_position)
{
    int width = 10;
    int height = 10;
    AvailablePositions* ap = create_available_positions(width, height);
    int initial_count = ap->count;
    remove_position(ap, ap->positions[0].posx, ap->positions[0].posy);
    ck_assert_int_eq(ap->count, initial_count - 1);
    delete_available_positions(ap);
}
END_TEST

START_TEST(test_add_position)
{
    int width = 10;
    int height = 10;
    AvailablePositions* ap = create_available_positions(width, height);
    int initial_count = ap->count;
    Position pos = { 1, 1 };
    add_position(ap, pos.posx, pos.posy);
    ck_assert_int_eq(ap->count, initial_count + 1);
    delete_available_positions(ap);
}
END_TEST

START_TEST(test_food_adresser)
{
	List* pFood = create_list();
	Element* e = create_element(-1, -1);
	add_element_to_head(pFood, e);
	Element* addressed_element = food_adresser(pFood);
	ck_assert_ptr_eq(addressed_element, e);
	delete_list(pFood);
}
END_TEST

START_TEST(test_reset_food)
{
	List* pFood = create_list();
	Element* e = create_element(5, 5);
	add_element_to_head(pFood, e);
	reset_food(pFood);
	ck_assert_int_eq(e->pos.posx, -1);
	ck_assert_int_eq(e->pos.posy, -1);
	delete_list(pFood);
}
END_TEST

START_TEST(test_resize_foods)
{
	init_foods(3, 5);
	resize_foods(4, 6);
	ck_assert_int_eq(appArgs.pFood_Single_Player->size, 4);
	ck_assert_int_eq(appArgs.pFood_Multiplayer->size, 6);
	delete_foods();
}
END_TEST

START_TEST(test_init_foods)
{
	init_foods(3, 5);
	ck_assert_ptr_nonnull(appArgs.pFood_Main);
	ck_assert_ptr_nonnull(appArgs.pFood_Single_Player);
	ck_assert_ptr_nonnull(appArgs.pFood_Multiplayer);
	ck_assert_int_eq(appArgs.pFood_Single_Player->size, 3);
	ck_assert_int_eq(appArgs.pFood_Multiplayer->size, 5);
	delete_foods();
}
END_TEST

Suite* food_suite()
{
	Suite* s = suite_create("Food Test Suite");

	TCase* tc_available_positions = tcase_create("Available Positions");
	tcase_add_test(tc_available_positions, test_delete_available_positions);
	tcase_add_test(tc_available_positions, test_create_available_positions);
	tcase_add_test(tc_available_positions, test_remove_position);
	tcase_add_test(tc_available_positions, test_add_position);
	suite_add_tcase(s, tc_available_positions);

	TCase* tc_food = tcase_create("Food");
	tcase_add_test(tc_food, test_food_adresser);
	tcase_add_test(tc_food, test_reset_food);
	tcase_add_test(tc_food, test_resize_foods);
	tcase_add_test(tc_food, test_init_foods);
	suite_add_tcase(s, tc_food);

	return s;
}

int main(void)
{
	int number_failed = 0;

	{
		Suite* s = food_suite();
		SRunner* sr = srunner_create(s);
		srunner_run_all(sr, CK_NORMAL);
		number_failed += srunner_ntests_failed(sr);
		srunner_free(sr);
	}

	return (number_failed == 0) ? 0 : 1;
}
