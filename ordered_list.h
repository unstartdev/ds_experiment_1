#pragma once
typedef int ElemType;

enum Status {
        OK,
        Error,
        OVERFLOW
};

typedef enum Status Status;

Status int_equal(int a, int b);

char int_compare(int a, int b);

Status printf_it(int t);

struct ordered_list {
        ElemType *elem;
        size_t length;
        size_t list_size;
};

Status InitOrderedList(struct ordered_list *l);

Status OrderedListInsert(struct ordered_list *l, size_t pos, ElemType e);

Status OrderedListDelete(struct ordered_list *l, size_t pos, ElemType *e);

size_t OrderedListLocate(struct ordered_list *l, ElemType e, Status(*equal)(ElemType, ElemType));

Status OrderedListTraversal(struct ordered_list *l, Status(*it)(ElemType));

Status OrderedListOrder(struct ordered_list *l, char (*compare)(ElemType, ElemType));

Status OrderedListMerge(struct ordered_list *a, struct ordered_list *b, struct ordered_list *out, char (*compare)(ElemType, ElemType));