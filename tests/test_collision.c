/*
 ============================================================================
 Name        : test_collision.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
=============================================================================
*/

#include <check.h>
#include "../include/collision.h"
#include "../include/snake.h"

START_TEST(test_is_snake_collided)
{
	Snake* snake = create_snake(3, 1, MOVE_RIGHT, 5, 5);
	add_element_to_head(snake->pos_snake, create_element(6, 5));
	add_element_to_head(snake->pos_snake, create_element(7, 5));
	add_element_to_head(snake->pos_snake, create_element(7, 6));
	add_element_to_head(snake->pos_snake, create_element(6, 6));
	ck_assert(!is_snake_collided(snake));
	delete_snake(snake);
}
END_TEST

START_TEST(test_is_snake_collided_with_wall)
{
	Snake* snake = create_snake(3, 1, MOVE_RIGHT, 0, 5);
	ck_assert(is_snake_collided_with_wall(snake, 20, 20));
	snake->pos_snake->head->pos.posx = 5;
	snake->pos_snake->head->pos.posy = 0;
	ck_assert(is_snake_collided_with_wall(snake, 20, 20));
	snake->pos_snake->head->pos.posx = 19;
	snake->pos_snake->head->pos.posy = 5;
	ck_assert(is_snake_collided_with_wall(snake, 20, 20));
	snake->pos_snake->head->pos.posx = 5;
	snake->pos_snake->head->pos.posy = 19;
	ck_assert(is_snake_collided_with_wall(snake, 20, 20));
	snake->pos_snake->head->pos.posx = 5;
	snake->pos_snake->head->pos.posy = 5;
	ck_assert(!is_snake_collided_with_wall(snake, 20, 20));
	delete_snake(snake);
}
END_TEST

START_TEST(test_is_snake_collided_with_snake)
{
	Snake* snake1 = create_snake(3, 1, MOVE_RIGHT, 5, 5);
	Snake* snake2 = create_snake(3, 2, MOVE_RIGHT, 5, 5);
	ck_assert(is_snake_collided_with_snake(snake1, snake2));
	snake1->pos_snake->head->pos.posx = 5;
	snake1->pos_snake->head->pos.posy = 5;
	snake2->pos_snake->head->pos.posx = 7;
	snake2->pos_snake->head->pos.posy = 5;
	ck_assert(!is_snake_collided_with_snake(snake1, snake2));
	delete_snake(snake1);
	delete_snake(snake2);
}
END_TEST

Suite* collision_suite(void)
{
	Suite* s;
	TCase* tc_core;

	s = suite_create("Collision");
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_is_snake_collided);
	tcase_add_test(tc_core, test_is_snake_collided_with_wall);
	tcase_add_test(tc_core, test_is_snake_collided_with_snake);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void)
{
	int number_failed;

	{
		Suite* s = collision_suite();
		SRunner* sr = srunner_create(s);
		srunner_run_all(sr, CK_NORMAL);
		number_failed = srunner_ntests_failed(sr);
		srunner_free(sr);
	}

	return (number_failed == 0) ? 0 : 1;
}
