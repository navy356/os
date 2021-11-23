#include "list.h"

void dlist_init(DList *dlist)
{
    dlist->next = dlist;
    dlist->prev = dlist;
}

void dlist_insert_after(DList *d1, DList *d2)
{
    DList *n1 = d1->next;
    DList *e2 = d2->prev;
    d1->next = d2;
    d2->prev = d1;
    e2->next = n1;
    n1->prev = e2;
}

void dlist_insert_before(DList *d1, DList *d2)
{
    DList *e1 = d1->prev;
    DList *e2 = d2->prev;

    e1->next = d2;
    d2->prev = e1;
    e2->next = d1;
    d1->prev = e2;
}

void dlist_remove(DList *d)
{
    d->prev->next = d->next;
    d->next->prev = d->prev;
    d->next = d;
    d->prev = d;
}

void dlist_push(DList **d1p, DList *d2)
{
    if (*d1p != NULL)
    {
        dlist_insert_before(*d1p, d2);
    }
    *d1p = d2;
}

DList *dlist_pop(DList **dp)
{
    DList *d1 = *dp;
    DList *d2 = d1->next;
    dlist_remove(d1);
    if (d1 == d2)
    {
        *dp = NULL;
    }
    else
    {
        *dp = d2;
    }
    return d1;
}

void dlist_remove_from(DList **d1p, DList *d2)
{
    if (*d1p == d2)
    {
        dlist_pop(d1p);
    }
    else
    {
        dlist_remove(d2);
    }
}
