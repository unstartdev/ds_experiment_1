/*
The MIT License (MIT)

Copyright © 2025 <unstartdev>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This version is for datastructure class.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ordered_list.h"

struct series {
        int val;
        struct series *next;
};

struct test_sample {
        struct series *samples;
        struct test_sample *next;
};

struct test_sample *newTestSample()
{
        struct test_sample *t = calloc(1, sizeof(struct test_sample));
        if (!t)
                exit(OVERFLOW);
        t->samples = calloc(1, sizeof(struct series));
        if (!t->samples)
                exit(OVERFLOW);
        return t;
}

void print_series(struct series *s)
{
        while (s = s->next) {
                printf("%d ", s->val);
        }
        putchar('\n');
}

struct test_sample *test_sample_get(struct test_sample *shead, size_t index)
{
        for (size_t i = 0; i < index + 1; i++) {
                shead = shead->next;
        }
        return shead;
}

Status test_sample_load(struct test_sample *t, struct ordered_list *l)
{
        struct series *scur = t->samples;
        for (size_t i = 1; scur = scur->next; i++) {
                if (OrderedListInsert(l, i, scur->val) != OK) {
                        return Error;
                }
        }
        return OK;
}

int main()
{
        FILE *fp = fopen("sample.txt", "r");

        char line[2048];
        char nbuffer[1024];

        struct test_sample *shead = newTestSample();
        struct test_sample *tcur = shead;

        while (fgets(line, 2048, fp)) {
                char *pline = line;
                tcur->next = newTestSample();
                tcur = tcur->next;
                struct series *scur = tcur->samples;
                while (*pline) {
                        for (size_t i = 0; i < 1024; i++) {
                                if (*pline == ' ' || *pline == '\n') {
                                        nbuffer[i] = 0;
                                        pline++;
                                        break;
                                }
                                if (*pline == 0) {
                                        nbuffer[i] = 0;
                                        break;
                                }
                                nbuffer[i] = *pline++;
                        }
                        if (nbuffer[0] == 0)
                                continue;
                        scur->next = calloc(1, sizeof(struct series));
                        if (!scur)
                                exit(OVERFLOW);
                        scur = scur->next;
                        scur->val = atoi(nbuffer);
                        memset(nbuffer, 0, 1024);

                }
        }

        fclose(fp);

        printf("Sucessfully read all test samples as follows:\n");

        while (1) {
                system("cls");
                tcur = shead;
                size_t i = 0;

                for (; tcur = tcur->next; i++) {
                        printf("Sample %zu: ", i);
                        print_series(tcur->samples);
                }

                printf("Sample %zu: exit\n", i);

                int sel;
                printf("Please select a sample: ");
                scanf_s("%d", &sel);
                if (sel == i)
                        break;


                tcur = test_sample_get(shead, sel);



                struct ordered_list testobj;
                if (InitOrderedList(&testobj) != OK) {
                        printf("Init ordered_list error.");
                        break;
                }

                if (test_sample_load(tcur, &testobj) != OK) {
                        printf("Error happened when loading elements into testobj.");
                        break;
                }
                while (1) {
                        system("cls");
                        printf("Current sample: ");
                        print_series(tcur->samples);
                        printf("Current ordered_list: ");
                        OrderedListTraversal(&testobj, printf_it);
                        putchar('\n');

                        printf("0. Delete\n1. Insert\n2. Locate\n3. Order\n4. Merge\n5. Traversal(printf)\n6. Back to main menu\nPlease select a test function: ");
                        scanf_s("%d", &sel);
                        if (sel == 6) {
                                OrderedListDestory(&testobj);
                                break;
                        }
                        int pos;
                        int val;
                        switch (sel) {
                        case 0:
                                printf("Input the position: ");
                                scanf_s("%d", &pos);
                                if (OrderedListDelete(&testobj, pos, &val) != OK)
                                        printf("Delete error.");
                                printf("Successfully deleted the element which value is %d\n", val);
                                break;

                        case 1:
                                printf("Input the position: ");
                                scanf_s("%d", &pos);
                                printf("Input the value: ");
                                scanf_s("%d", &val);
                                if (OrderedListInsert(&testobj, pos, val) != OK)
                                        printf("Insert error.");
                                printf("Successfully inserted the element.");
                                break;
                        case 2:
                                printf("Input the value: ");
                                scanf_s("%d", &val);
                                printf("The position is %zu", OrderedListLocate(&testobj, val, int_equal));
                                break;
                        case 3:
                                if (OrderedListOrder(&testobj, int_compare) != OK)
                                        printf("Order error.");
                                OrderedListTraversal(&testobj, printf_it);
                                break;
                        case 4:
                                tcur = shead;
                                for (size_t i = 0; tcur = tcur->next; i++) {
                                        printf("Sample %zu: ", i);
                                        print_series(tcur->samples);
                                }

                                printf("Please select the another sample: ");
                                scanf_s("%d", &sel);

                                struct ordered_list testobj2;
                                InitOrderedList(&testobj2);

                                tcur = test_sample_get(shead, sel);
                                test_sample_load(tcur, &testobj2);

                                OrderedListOrder(&testobj, int_compare);
                                OrderedListOrder(&testobj2, int_compare);

                                struct ordered_list mg;
                                if (OrderedListMerge(&testobj, &testobj2, &mg, int_compare) != OK)
                                        printf("Merge failed.");
                                printf("Merged result is ");
                                OrderedListTraversal(&mg, printf_it);
                                OrderedListDestory(&testobj2);
                                break;
                        case 5:
                                if (OrderedListTraversal(&testobj, printf_it) != OK)
                                        printf("Traversal error.");
                                printf("\nTraversal finished.");
                                break;
                        default:
                                break;
                        }
                        system("pause");
                }
        }

end:
        while (shead != NULL) {
                tcur = shead;
                while (tcur->samples != NULL) {
                        struct series *scur = tcur->samples;
                        tcur->samples = tcur->samples->next;
                        free(scur);
                }
                shead = shead->next;
                free(tcur);
        }
}