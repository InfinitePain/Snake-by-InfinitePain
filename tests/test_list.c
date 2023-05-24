/*
 ============================================================================
 Name        : test_list.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Test file for Snake-by-InfinitePain
=============================================================================
*/

#include <check.h>
#include <stdlib.h>
#include "../include/list.h"

START_TEST(test_create_list)
{
	List* pList = create_list();
	ck_assert_ptr_nonnull(pList);
	ck_assert_ptr_eq(pList->head, NULL);
	ck_assert_int_eq(pList->size, 0);
	delete_list(pList);
}
END_TEST

START_TEST(test_add_element_to_head)
{
	List* pList = create_list();
	Element* e1 = create_element(1, 1);
	Element* e2 = create_element(2, 2);

	add_element_to_head(pList, e1);
	ck_assert_ptr_eq(pList->head, e1);
	ck_assert_int_eq(pList->size, 1);

	add_element_to_head(pList, e2);
	ck_assert_ptr_eq(pList->head, e2);
	ck_assert_ptr_eq(e2->next, e1);
	ck_assert_int_eq(pList->size, 2);

	delete_list(pList);
}
END_TEST

START_TEST(test_get_element_at_index)
{
	List* pList = create_list();
	Element* e1 = create_element(1, 1);
	Element* e2 = create_element(2, 2);

	add_element_to_head(pList, e1);
	add_element_to_head(pList, e2);

	Element* e = get_element_at_index(pList, 1);
	ck_assert_ptr_eq(e, e1);

	e = get_element_at_index(pList, 0);
	ck_assert_ptr_eq(e, e2);

	delete_list(pList);
}
END_TEST

START_TEST(test_delete_last_element)
{
	List* pList = create_list();
	Element* e1 = create_element(1, 1);
	Element* e2 = create_element(2, 2);

	add_element_to_head(pList, e1);
	add_element_to_head(pList, e2);

	delete_last_element(pList);
	ck_assert_int_eq(pList->size, 1);
	ck_assert_ptr_eq(pList->head, e2);
	ck_assert_ptr_eq(pList->head->next, NULL);

	delete_list(pList);
}
END_TEST

START_TEST(test_make_list_from)
{
	List* source_list = create_list();
	List* target_list = create_list();
	Element* e1 = create_element(1, 1);
	Element* e2 = create_element(2, 2);
	Element* e3 = create_element(3, 3);

	add_element_to_head(source_list, e1);
	add_element_to_head(source_list, e2);
	add_element_to_head(source_list, e3);

	make_list_from(source_list, target_list, 2);

	ck_assert_int_eq(target_list->size, 2);
	ck_assert_ptr_eq(target_list->head, e2);

	delete_list(source_list);
	free(target_list);
}
END_TEST

Suite* list_suite()
{
	Suite* s = suite_create("List Test Suite");

	TCase* tc_list = tcase_create("List");
	tcase_add_test(tc_list, test_create_list);
	tcase_add_test(tc_list, test_add_element_to_head);
	tcase_add_test(tc_list, test_get_element_at_index);
	tcase_add_test(tc_list, test_delete_last_element);
	tcase_add_test(tc_list, test_make_list_from);
	suite_add_tcase(s, tc_list);

	return s;
}

int main(void)
{
	int number_failed;
	
	{
		Suite* s = list_suite();
		SRunner* sr = srunner_create(s);
		srunner_run_all(sr, CK_NORMAL);
		number_failed += srunner_ntests_failed(sr);
		srunner_free(sr);
	}
	
	return (number_failed == 0) ? 0 : 1;
}
