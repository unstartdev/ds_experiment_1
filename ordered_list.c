/*
The MIT License (MIT)

Copyright © 2025 <unstartdev>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This version is for datastructure class.
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ordered_list.h"

#define LIST_INIT_SIZE 10
#define LISTINC 10

Status int_equal(int a, int b)
{
        return a == b ? OK : Error;
}

char int_compare(int a, int b)
{
        if (a > b)
                return '>';
        if (a == b)
                return '=';
        if (a < b)
                return '<';
        return 0;
}

Status printf_it(int t)
{
        printf("%d ", t);
        return OK;
}

/*
* @brief Create a new ordered list
* @param[in, out] l
*/
Status InitOrderedList(struct ordered_list *l)
{
        if ((l->elem = calloc(LIST_INIT_SIZE, sizeof(ElemType))) == NULL)
                exit(OVERFLOW);
        l->length = 0;
        l->list_size = LIST_INIT_SIZE;
        return OK;
}

/*
* @brief Check whether the pos is in the list. Return Error if it is out of length.
*/
Status assert_pos(struct ordered_list *l, size_t pos)
{
        if (pos - l->length > 0)
                return Error;
        return OK;
}


/*
* @brief Insert an element into a existed list.
* @param[in] l The existed list
* @param pos The position of element after the element will be inserted.
* @param e The element will be inserted.
*/
Status OrderedListInsert(struct ordered_list *l, size_t pos, ElemType e)
{
        if (pos < 1 || pos > l->length + 1)
                return Error;
        if (l->length >= l->list_size) {
                ElemType *t = realloc(l->elem, (l->list_size + LISTINC) * sizeof(ElemType));
                if (t == NULL)
                        exit(OVERFLOW);
                l->elem = t;
                l->list_size += LISTINC;
        }
        ElemType *t = &(l->elem[pos - 1]);
        for (ElemType *ptr = &(l->elem[l->length - 1]); ptr >= t; ptr--) {
                *(ptr + 1) = *ptr;        
        }        
        l->elem[pos - 1] = e;
        l->length++;
        return OK;
}

/*
* @brief Delete the element at selected position.
* @param[in] l The targeted list.
* @param pos The positon of the element which will be deleted.
* @param[out] e The value of the element.
*/
Status OrderedListDelete(struct ordered_list *l, size_t pos, ElemType *e)
{
        if (pos < 1 || pos > l->length)
                return Error;
        ElemType* del_ptr = &(l->elem[pos - 1]);
        *e = *del_ptr;
        for (ElemType *ptr = del_ptr; ptr < &(l->elem[l->length - 1]); ptr++) {
                *ptr = *(ptr + 1);
        }
        l->length--;
        return OK;
}

/*
* @brief Locate the first appeared index of e.
* @param[in] l The targeted list.
* @param e The value of the element to be searched.
* @param equal A function that can check whether two elements of ElemType is equal.
*/
size_t OrderedListLocate(struct ordered_list *l, ElemType e, Status (*equal)(ElemType, ElemType))
{
        for (ElemType *ptr = l->elem; ptr <= &(l->elem[l->length - 1]); ptr++) {
                if (equal(*ptr, e) == OK)
                        return ptr - l->elem + 1;
        }
        return 0;
}

/*
* @brief Traversal a list using function it.
* @param[in] l The targeted list.
* @param it A function that will receive each element in the list.
*/
Status OrderedListTraversal(struct ordered_list *l, Status (*it)(ElemType))
{
        for (size_t i = 0; i < l->length; i++) {
                if (it(l->elem[i]) == Error)
                        return Error;
        }
        return OK;
}

/*
* @brief Simply make the list in order from small to big.
* @param[in] l The targeted list.
* @param compare A function that can compare two elements of ElemType returning '>', '<' or '='.
*/
Status OrderedListOrder(struct ordered_list *l, char (*compare)(ElemType, ElemType))
{
        if (l->length < 2)
                return OK;
        char is_unordered = 0;
        do {
                is_unordered = 0;
                for (size_t i = 0; i < l->length - 1 ; i++) {
                        ElemType t;
                        switch (compare(l->elem[i], l->elem[i+1])) {
                        case '>':
                                t = l->elem[i];
                                l->elem[i] = l->elem[i+1];
                                l->elem[i+1] = t;
                                is_unordered = 1;
                                break;
                        case '=':
                                break;
                        case '<':
                                break;
                        default:
                                return Error;
                                break;
                        }
                }
        } while (is_unordered);
        return OK;
}

/*
* @brief Merge list a and list b into one list out and allow element repeated.
* @param[in] a The targeted list a.
* @param[in] b The targeted list b.
* @param[in,out] out The merged list.
* @param compare A function that can compare two elements of ElemType returning '>', '<' or '='.
*/
Status OrderedListMerge(struct ordered_list *a, struct ordered_list *b, struct ordered_list *out, char (*compare)(ElemType, ElemType))
{
        // pre allocation
        out->length = a->length + b->length;
        out->elem = calloc(out->length, sizeof(ElemType));
        if (out->elem == NULL)
                exit(OVERFLOW);
        ElemType *pa = a->elem;
        ElemType *pb = b->elem;
        ElemType *t = out->elem;
        ElemType *ta = pa + a->length - 1;
        ElemType *tb = pb + b->length - 1;
        while (pa <= ta && pb <= tb) {
                switch (compare(*pa, *pb)) {
                case '=':
                case '<':
                        *t++ = *pa++;
                        break;
                case '>':
                        *t++ = *pb++;
                        break;
                default:
                        return Error;
                        break;
                }
        }
        while (pa <= ta) {
                *t++ = *pa++;
        }
        while (pb <= tb) {
                *t++ = *pb++;
        }
        return OK;
}

void OrderedListDestory(struct ordered_list *l)
{
        free(l->elem);
}