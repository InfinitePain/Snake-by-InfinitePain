/*
 ============================================================================
 Name        : test_element.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Test file for Snake-by-InfinitePain
=============================================================================
*/

#include <check.h>
#include <stdlib.h>
#include "../include/element.h"

START_TEST(test_create_element)
{
	Element* pElement = create_element(1, 1);
	ck_assert_ptr_nonnull(pElement);
	ck_assert_int_eq(pElement->pos.posx, 1);
	ck_assert_int_eq(pElement->pos.posy, 1);
	ck_assert_int_eq(pElement->index, -1);
	ck_assert_ptr_eq(pElement->next, NULL);
	free(pElement);
}
END_TEST

Suite* element_suite()
{
	Suite* s = suite_create("Element Test Suite");

	TCase* tc_element = tcase_create("Element");
	tcase_add_test(tc_element, test_create_element);
	suite_add_tcase(s, tc_element);

	return s;
}

int main(void)
{
	int number_failed = 0;

	{
		Suite* s = element_suite();
		SRunner* sr = srunner_create(s);
		srunner_run_all(sr, CK_NORMAL);
		number_failed += srunner_ntests_failed(sr);
		srunner_free(sr);
	}

	return (number_failed == 0) ? 0 : 1;
}
