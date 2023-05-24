/*
 ============================================================================
 Name        : test_direction_buffer.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Test file for Snake-by-InfinitePain
=============================================================================
*/

#include <check.h>
#include "../include/direction_buffer.h"

START_TEST(test_buffer_init) {
	DirectionBuffer buffer;
	buffer_init(&buffer);
	ck_assert_int_eq(buffer.head, 0);
	ck_assert_int_eq(buffer.tail, 0);
}
END_TEST

START_TEST(test_buffer_is_empty) {
	DirectionBuffer buffer;
	buffer_init(&buffer);
	ck_assert(buffer_is_empty(&buffer));

	buffer_push(&buffer, 1);
	ck_assert(!buffer_is_empty(&buffer));
}
END_TEST

START_TEST(test_buffer_push) {
	DirectionBuffer buffer;
	buffer_init(&buffer);
	buffer_push(&buffer, 1);
	ck_assert_int_eq(buffer.buffer[buffer.tail], 1);
}
END_TEST

Suite* direction_buffer_suite(void) {
	Suite* s;
	TCase* tc_core;

	s = suite_create("DirectionBuffer");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_buffer_init);
	tcase_add_test(tc_core, test_buffer_is_empty);
	tcase_add_test(tc_core, test_buffer_push);

	suite_add_tcase(s, tc_core);

	return s;
}

int main(void) {
	int number_failed = 0;

	{
		Suite* s = direction_buffer_suite();
		SRunner* sr = srunner_create(s);
		srunner_run_all(sr, CK_NORMAL);
		number_failed += srunner_ntests_failed(sr);
		srunner_free(sr);
	}
	
	return (number_failed == 0) ? 0 : 1;
}