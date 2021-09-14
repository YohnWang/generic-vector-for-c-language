#ifndef VECTOR_H
#define VECTOR_H

#include<stddef.h>
#include<stdlib.h>
#include<string.h>

#include<stdio.h>
#include<stdbool.h>

#if defined(DEBUG) || defined(_DEBUG)
#ifndef VECTOR_DEBUG
#define VECTOR_DEBUG
#endif
#endif

// vector相关的调试项
#ifdef  VECTOR_DEBUG
#define vector_debug(format,...) fprintf(stderr,"[file: %s][line: %d]"format,__FILE__,__LINE__,##__VA_ARGS__)
#define vector_assert(condition,...) do{ if((condition)==false) {vector_debug("assert error: %s\n",#condition ": " __VA_ARGS__);exit(1);}}while(0)
#else
#define vector_debug(format,...)  
#define vector_assert(condition,...)  
#endif

#define $(str1,str2) str1##_##str2
#define $$(str)  $(__name_to_convert,str)

#define vector(type) $(vector,type)

#define vector_def(type)                                                            \
struct vector(type)                                                                 \
{                                                                                   \
    ssize_t size;                                                                   \
    ssize_t capacity;                                                               \
    type *a;                                                                        \
};                                                                                  \
typedef struct vector(type) vector(type);                                           \
                                                                                    \
static inline void    $(vector_init,type)(vector(type) *v);                         \
static inline void    $(vector_del,type)(vector(type) *v);                          \
static inline ssize_t $(vector_size,type)(vector(type) *v);                         \
static inline ssize_t $(vector_capacity,type)(vector(type) *v);                     \
static inline type*   $(vector_at,type)(vector(type) *v,ssize_t index);             \
static inline type    $(vector_get,type)(vector(type) *v,ssize_t index);            \
static inline type*   $(vector_data,type)(vector(type) *v);                         \
static inline void    $(vector_clear,type)(vector(type) *v);                        \
static inline void    $(vector_reserve,type)(vector(type) *v,ssize_t new_capacity); \
static inline void    $(vector_resize,type)(vector(type) *v,ssize_t size);          \
static inline void    $(vector_shrink_to_fit,type)(vector(type) *v);                \
static inline void    $(vector_assign,type)(vector(type) *t,vector(type) *v);       \
static inline void    $(vector_move,type)(vector(type) *t,vector(type) *v);         \
static inline type*   $(vector_divert,type)(vector(type) *v);                       \
static inline void    $(vector_push_back,type)(vector(type) *v,type e);             \
static inline void    $(vector_pop_back,type)(vector(type) *v);                     \
                                                                                    \
static inline void $(vector_init,type)(vector(type) *v)                                         \
{                                                                                               \
    v->size=0;                                                                                  \
    v->capacity=0;                                                                              \
    v->a=NULL;                                                                                  \
}                                                                                               \
static inline void $(vector_del,type)(vector(type) *v)                                          \
{                                                                                               \
    free($(vector_divert,type)(v));                                                             \
}                                                                                               \
static inline ssize_t $(vector_size,type)(vector(type) *v)                                      \
{                                                                                               \
    return v->size;                                                                             \
}                                                                                               \
static inline ssize_t $(vector_capacity,type)(vector(type) *v)                                  \
{                                                                                               \
    return v->capacity;                                                                         \
}                                                                                               \
static inline type* $(vector_at,type)(vector(type) *v,ssize_t index)                            \
{                                                                                               \
    vector_assert(index>=0 && index<$(vector_size,type)(v),"vector access out of range\n");     \
    return &v->a[index];                                                                        \
}                                                                                               \
static inline type $(vector_get,type)(vector(type) *v,ssize_t index)                            \
{                                                                                               \
    return *$(vector_at,type)(v,index);                                                         \
}                                                                                               \
static inline type* $(vector_data,type)(vector(type) *v)                                        \
{                                                                                               \
    return v->a;                                                                                \
}                                                                                               \
static inline void $(vector_clear,type)(vector(type) *v)                                        \
{                                                                                               \
    v->size=0;                                                                                  \
}                                                                                               \
static inline void $(vector_reserve,type)(vector(type) *v,ssize_t new_capacity)                 \
{                                                                                               \
    ssize_t capacity=$(vector_capacity,type)(v);                                                \
    ssize_t size=$(vector_size,type)(v);                                                        \
    if(new_capacity>capacity)                                                                   \
    {                                                                                           \
        type *p=malloc(new_capacity*sizeof(*p));                                                \
        if(p==NULL)                                                                             \
        {                                                                                       \
            vector_debug("vector memory alloc failed\n");                                       \
            exit(1);                                                                            \
        }                                                                                       \
        if(size>0)                                                                              \
        {                                                                                       \
            memcpy(p,v->a,size*sizeof(*p));                                                     \
        }                                                                                       \
        free(v->a);                                                                             \
        v->capacity=new_capacity;                                                               \
        v->a=p;                                                                                 \
    }                                                                                           \
    else                                                                                        \
    {                                                                                           \
        vector_debug("vector capacity is bigger equal than this new capacity\n");               \
    }                                                                                           \
}                                                                                               \
static inline void $(vector_resize,type)(vector(type) *v,ssize_t new_size)                      \
{                                                                                               \
    ssize_t size=$(vector_size,type)(v);                                                        \
    ssize_t capacity=$(vector_capacity,type)(v);                                                \
    if(new_size > size)                                                                         \
    {                                                                                           \
        if(new_size > capacity)                                                                 \
            $(vector_reserve,type)(v,new_size);                                                 \
        ssize_t i=size;                                                                         \
        while(((new_size-i)&7)!=0)                                                              \
            v->a[i++]=(type){0};                                                                \
        while(i+7<new_size)                                                                     \
        {                                                                                       \
            v->a[i++]=(type){0};                                                                \
            v->a[i++]=(type){0};                                                                \
            v->a[i++]=(type){0};                                                                \
            v->a[i++]=(type){0};                                                                \
            v->a[i++]=(type){0};                                                                \
            v->a[i++]=(type){0};                                                                \
            v->a[i++]=(type){0};                                                                \
            v->a[i++]=(type){0};                                                                \
        }                                                                                       \
    }                                                                                           \
    v->size=new_size;                                                                           \
}                                                                                               \
static inline void $(vector_shrink_to_fit,type)(vector(type) *v)                                \
{                                                                                               \
    if(v->capacity - v->size > (ssize_t)((size_t)v->size/2))                                    \
    {                                                                                           \
        vector(type) t={0};                                                                     \
        $(vector_assign,type)(&t,v);                                                            \
        $(vector_move,type)(v,&t);                                                              \
        $(vector_del,type)(&t);                                                                 \
    }                                                                                           \
}                                                                                               \
static inline void $(vector_assign,type)(vector(type) *t,vector(type) *v)                           \
{                                                                                                   \
    $(vector_del,type)(t);                                                                          \
    if($(vector_size,type)(v)>0)                                                                    \
    {                                                                                               \
        $(vector_reserve,type)(t,$(vector_size,type)(v));                                           \
        t->size=$(vector_size,type)(v);                                                             \
        memcpy($(vector_data,type)(t),$(vector_data,type)(v),$(vector_size,type)(t)*sizeof(type));  \
    }                                                                                               \
}                                                                                                   \
static inline void $(vector_move,type)(vector(type) *t,vector(type) *v)                         \
{                                                                                               \
    vector(type) p=*t;                                                                          \
    *t=*v;                                                                                      \
    *v=p;                                                                                       \
}                                                                                               \
static inline type* $(vector_divert,type)(vector(type) *v)                                      \
{                                                                                               \
    type *ret=v->a;                                                                             \
    v->size=0;                                                                                  \
    v->capacity=0;                                                                              \
    v->a=NULL;                                                                                  \
    return ret;                                                                                 \
}                                                                                               \
static inline void $(vector_push_back,type)(vector(type) *v,type e)                             \
{                                                                                               \
    ssize_t size=$(vector_size,type)(v);                                                        \
    ssize_t capacity=$(vector_capacity,type)(v);                                                \
    if(size==capacity)                                                                          \
        $(vector_reserve,type)(v,size+size/2+1);                                                \
    v->a[v->size++]=e;                                                                          \
}                                                                                               \
static inline void $(vector_pop_back,type)(vector(type) *v)                                     \
{                                                                                               \
    vector_assert(v->size>0,"vector empty but pop\n");                                          \
    v->size--;                                                                                  \
}                                                                                               \

struct _vector
{
    ssize_t size;
    ssize_t capacity;
    void    *a;
};

static inline void vector_init(void *v)
{
    struct _vector *vptr=v;
    vptr->size=0;
    vptr->capacity=0;
    vptr->a=NULL;
}

static inline void vector_del(void *v)
{
    struct _vector *vptr=v;
    free(vptr->a);
    vptr->size=0;
    vptr->capacity=0;
    vptr->a=NULL;
}

static inline void vector_clear(void *v)
{
    struct _vector *vptr=v;
    vptr->size=0;
}

static inline void vector_pop_back(void *v)
{
    struct _vector *vptr=v;
    vector_assert(vptr->size>0);
    vptr->size--;
}

static inline ssize_t vector_size(void *v)
{
    struct _vector *vptr=v;
    return vptr->size;
}

static inline ssize_t vector_capacity(void *v)
{
    struct _vector *vptr=v;
    return vptr->capacity;
}

// 移动赋值
static inline void vector_move(void *t,void *v)
{
    struct _vector *tptr=t,*vptr=v,tmp;
    tmp=*tptr;
    *tptr=*vptr;
    *vptr=tmp;
}

#define vector_at(vptr,index) ({&(vptr)->a[index];})
#define vector_data(vptr) ({(vptr)->a;})
#define vector_get(vptr,index) ({(vptr)->a[index];})

#define vector_divert(vptr)         \
({                                  \
    __auto_type $$(v)=vptr;         \
    __auto_type $$(ret)=$$(v)->a;   \
    $$(v)->size=0;                  \
    $$(v)->capacity=0;              \
    $$(v)->a=NULL;                  \
    $$(ret);                        \
})

#define vector_reserve(vptr,new_capacity_in) \
({                                                                                              \
    __auto_type $$(vv)=vptr;                                                                    \
    ssize_t $$(new_capacity)=new_capacity_in;                                                   \
    {                                                                                           \
        ssize_t capacity=vector_capacity($$(vv));                                               \
        ssize_t size=vector_size($$(vv));                                                       \
        if($$(new_capacity)>capacity)                                                           \
        {                                                                                       \
            typeof($$(vv)->a[0]) *p=malloc($$(new_capacity)*sizeof(*p));                        \
            if(p==NULL)                                                                         \
            {                                                                                   \
                vector_debug("vector memory alloc failed\n");                                   \
                exit(1);                                                                        \
            }                                                                                   \
            if(size>0)                                                                          \
            {                                                                                   \
                memcpy(p,$$(vv)->a,size*sizeof(*p));                                            \
            }                                                                                   \
            free($$(vv)->a);                                                                    \
            $$(vv)->capacity=$$(new_capacity);                                                  \
            $$(vv)->a=p;                                                                        \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            vector_debug("vector capacity is bigger than this new capacity\n");                 \
        }                                                                                       \
    }                                                                                           \
})

#define vector_resize(vptr,new_size_in) \
({                                                                                              \
    __auto_type $$(v1)=vptr;                                                                    \
    ssize_t $$(new_size)=new_size_in;                                                           \
    {                                                                                           \
        ssize_t size=vector_size($$(v1));                                                       \
        ssize_t capacity=vector_capacity($$(v1));                                               \
        if($$(new_size) > size)                                                                 \
        {                                                                                       \
            if($$(new_size) > capacity)                                                         \
                vector_reserve($$(v1),$$(new_size));                                            \
            ssize_t i=size;                                                                     \
            while((($$(new_size)-i)&7)!=0)                                                      \
                $$(v1)->a[i++]=(typeof($$(v1)->a[0])){0};                                       \
            while(i+7<$$(new_size))                                                             \
            {                                                                                   \
                $$(v1)->a[i++]=(typeof($$(v1)->a[0])){0};                                       \
                $$(v1)->a[i++]=(typeof($$(v1)->a[0])){0};                                       \
                $$(v1)->a[i++]=(typeof($$(v1)->a[0])){0};                                       \
                $$(v1)->a[i++]=(typeof($$(v1)->a[0])){0};                                       \
                $$(v1)->a[i++]=(typeof($$(v1)->a[0])){0};                                       \
                $$(v1)->a[i++]=(typeof($$(v1)->a[0])){0};                                       \
                $$(v1)->a[i++]=(typeof($$(v1)->a[0])){0};                                       \
                $$(v1)->a[i++]=(typeof($$(v1)->a[0])){0};                                       \
            }                                                                                   \
        }                                                                                       \
        $$(v1)->size=$$(new_size);                                                              \
    }                                                                                           \
})

#define vector_assign(tptr,vptr)                                                                    \
({                                                                                                  \
    __auto_type $$(ta)=tptr;                                                                        \
    typeof($$(ta)) $$(va)=vptr;                                                                     \
    vector_del($$(ta));                                                                             \
    if(vector_size($$(va))>0)                                                                       \
    {                                                                                               \
        vector_reserve($$(ta),vector_size($$(va)));                                                 \
        $$(ta)->size=vector_size($$(va));                                                           \
        memcpy(vector_data($$(ta)),vector_data($$(va)),vector_size($$(va))*sizeof($$(va)->a[0]));   \
    }                                                                                               \
})

#define vector_shrink_to_fit(vptr) \
({                                                                                              \
    __auto_type $$(v)=vptr;                                                                     \
    if($$(v)->capacity - $$(v)->size > (ssize_t)((size_t)$$(v)->size/2))                        \
    {                                                                                           \
        typeof(*$$(v)) $$(t)={0};                                                               \
        vector_assign(&$$(t),$$(v));                                                            \
        vector_move($$(v),&$$(t));                                                              \
        vector_del(&$$(t));                                                                     \
    }                                                                                           \
})

#define vector_push_back(vptr,e_in) \
({                                                  \
    __auto_type $$(v)=vptr;                         \
    typeof($$(v)->a[0]) $$(e)=e_in;                 \
    {                                               \
        ssize_t size=vector_size($$(v));            \
        ssize_t capacity=vector_capacity($$(v));    \
        if(size==capacity)                          \
            vector_reserve($$(v),size+size/2+1);    \
        $$(v)->a[$$(v)->size++]=$$(e);              \
    }                                               \
})

#endif