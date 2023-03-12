/*
 ============================================================================
 Name        : letters.c
 Author      : InfinitePain
 License     : Apache License 2.0
 Description : Source file for Snake-by-InfinitePain
 ============================================================================
 */

#include "letters.h"
#include <stdlib.h>
#include <setjmp.h>

extern jmp_buf jmp_buffer1;
jmp_buf jmp_buffer2;

List* create_letter_A() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_B() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_C() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
	}
	else
	{
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_D() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_E() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_F() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_G() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_H() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_I() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 2, 2));
		add_element_to_tail(pList, create_element(3, 3, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(3, 5, 2));
		add_element_to_tail(pList, create_element(3, 6, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_J() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_K() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(3, 3, 2));
		add_element_to_tail(pList, create_element(3, 5, 2));
		add_element_to_tail(pList, create_element(4, 2, 2));
		add_element_to_tail(pList, create_element(4, 6, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_L() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_M() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 2, 2));
		add_element_to_tail(pList, create_element(3, 3, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(4, 2, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_N() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 2, 2));
		add_element_to_tail(pList, create_element(3, 3, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_O() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_P() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_Q() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 5, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 6, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_R() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_S() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_T() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 2, 2));
		add_element_to_tail(pList, create_element(3, 3, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(3, 5, 2));
		add_element_to_tail(pList, create_element(3, 6, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_U() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_V() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(2, 6, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 6, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_W() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(1, 4, 2));
		add_element_to_tail(pList, create_element(1, 5, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 6, 2));
		add_element_to_tail(pList, create_element(3, 5, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(4, 6, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 4, 2));
		add_element_to_tail(pList, create_element(5, 5, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_X() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 3, 2));
		add_element_to_tail(pList, create_element(2, 5, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(4, 3, 2));
		add_element_to_tail(pList, create_element(4, 5, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 6, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_Y() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 2, 2));
		add_element_to_tail(pList, create_element(1, 3, 2));
		add_element_to_tail(pList, create_element(2, 4, 2));
		add_element_to_tail(pList, create_element(3, 5, 2));
		add_element_to_tail(pList, create_element(3, 6, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 4, 2));
		add_element_to_tail(pList, create_element(5, 3, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}
List* create_letter_Z() {
	List* pList = create_list();
	if (pList == NULL) {
		longjmp(jmp_buffer1, 1);
		return NULL;
	}
	if (setjmp(jmp_buffer2) == 0)
	{
		add_element_to_tail(pList, create_element(1, 1, 2));
		add_element_to_tail(pList, create_element(1, 6, 2));
		add_element_to_tail(pList, create_element(1, 7, 2));
		add_element_to_tail(pList, create_element(2, 1, 2));
		add_element_to_tail(pList, create_element(2, 5, 2));
		add_element_to_tail(pList, create_element(2, 7, 2));
		add_element_to_tail(pList, create_element(3, 1, 2));
		add_element_to_tail(pList, create_element(3, 4, 2));
		add_element_to_tail(pList, create_element(3, 7, 2));
		add_element_to_tail(pList, create_element(4, 1, 2));
		add_element_to_tail(pList, create_element(4, 3, 2));
		add_element_to_tail(pList, create_element(4, 7, 2));
		add_element_to_tail(pList, create_element(5, 1, 2));
		add_element_to_tail(pList, create_element(5, 2, 2));
		add_element_to_tail(pList, create_element(5, 7, 2));
	}
	else {
		longjmp(jmp_buffer1, 1);
	}
	return pList;
}