#pragma once
#include <stddef.h>
typedef struct DList DList;
struct DList {
    DList *next;
    DList *prev;
};

void dlist_init(DList *dlist);
void dlist_insert_after(DList *d1, DList *d2);
void dlist_insert_before(DList *d1, DList *d2);
void dlist_remove(DList *d);
void dlist_push(DList **d1p, DList *d2);
DList *dlist_pop(DList **dp);
void dlist_remove_from(DList **d1p, DList *d2);

#define CONTAINER(C, l, v) ((C *)(((char *)v) - (intptr_t) & (((C *)0)->l)))
#define OFFSETOF(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

#define DLIST_INIT(v, l) dlist_init(&v->l)

#define DLIST_REMOVE_FROM(h, d, l)                 \
    {                                              \
        typeof(**h) **h_ = h, *d_ = d;             \
        DList *head = &(*h_)->l;                   \
        dlist_remove_from(&head, &d_->l);          \
        if (head == NULL)                          \
        {                                          \
            *h_ = NULL;                            \
        }                                          \
        else                                       \
        {                                          \
            *h_ = CONTAINER(typeof(**h), l, head); \
        }                                          \
    }

#define DLIST_PUSH(h, v, l)                   \
    {                                         \
        typeof(*v) **h_ = h, *v_ = v;         \
        DList *head = &(*h_)->l;              \
        if (*h_ == NULL)                      \
            head = NULL;                      \
        dlist_push(&head, &v_->l);            \
        *h_ = CONTAINER(typeof(*v), l, head); \
    }

#define DLIST_POP(h, l)                                \
    (                                                  \
        {                                              \
            typeof(**h) **h_ = h;                      \
            DList *head = &(*h_)->l;                   \
            DList *res = dlist_pop(&head);             \
            if (head == NULL)                          \
            {                                          \
                *h_ = NULL;                            \
            }                                          \
            else                                       \
            {                                          \
                *h_ = CONTAINER(typeof(**h), l, head); \
            }                                          \
            CONTAINER(typeof(**h), l, res);            \
        })

#define DLIST_ITERATOR_BEGIN(h, l, it)                                  \
    {                                                                   \
        typeof(*h) *h_ = h;                                             \
        DList *last_##it = h_->l.prev, *iter_##it = &h_->l, *next_##it; \
        do                                                              \
        {                                                               \
            if (iter_##it == last_##it)                                 \
            {                                                           \
                next_##it = NULL;                                       \
            }                                                           \
            else                                                        \
            {                                                           \
                next_##it = iter_##it->next;                            \
            }                                                           \
            typeof(*h) *it = CONTAINER(typeof(*h), l, iter_##it);

#define DLIST_ITERATOR_END(it)      \
    }                               \
    while ((iter_##it = next_##it)) \
        ;                           \
    }

#define DLIST_ITERATOR_REMOVE_FROM(h, it, l) DLIST_REMOVE_FROM(h, iter_##it, l)