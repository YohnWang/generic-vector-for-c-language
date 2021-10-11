// This file <vector.h> is from https://github.com/YohnWang/generic-vector-for-c-language
#ifndef VECTOR_H
#define VECTOR_H

#include<stddef.h>
#include<stdlib.h>

#if defined(DEBUG) || defined(_DEBUG)
#ifndef VECTOR_DEBUG
#define VECTOR_DEBUG
#endif
#endif

// debug for vector
#ifdef  VECTOR_DEBUG
  #include<stdio.h>
  #include<stdbool.h>
  #define vector_debug(format,...) fprintf(stderr,"[file: %s][line: %d]"format,__FILE__,__LINE__,##__VA_ARGS__)
  #define vector_assert(condition,...) do{ if((condition)==false) {vector_debug("assert error: %s\n",#condition ": " __VA_ARGS__);exit(1);}}while(0)
  #define vector_check(condition) if(1) 
#else
  #define vector_debug(format,...)  
  #define vector_assert(condition,...)  
  #ifdef VECTOR_RELEASE_FAST
    #define vector_check(condition) if(1) 
  #else
    #define vector_check(condition) if(condition) 
    #define vector_check_failed_def_default_value_to_return_pointer(type) static type $(_loc,type) 
  #endif
#endif

#define $(str1,str2) str1##_##str2

#define vector(type) $(vector,type)
#define vector_raii __attribute__((cleanup(vector_del))) 

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
static inline void    $(vector_insert,type)(vector(type) *v,ssize_t index,type e);  \
static inline void    $(vector_erase,type)(vector(type) *v,ssize_t index);          \
                                                                                    \
static inline void $(vector_init,type)(vector(type) *v)                                         \
{                                                                                               \
    *v=(vector(type)){.size=0,.capacity=0,.a=NULL};                                             \
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
    if(new_capacity>capacity)                                                                   \
    {                                                                                           \
        type *p=realloc(v->a,(size_t)new_capacity*sizeof(*p));                                  \
        if(p!=NULL)                                                                             \
        {                                                                                       \
            v->a=p;                                                                             \
            v->capacity=new_capacity;                                                           \
        }                                                                                       \
        else                                                                                    \
        {                                                                                       \
            vector_debug("vector memory alloc failed\n");                                       \
            exit(1);                                                                            \
        }                                                                                       \
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
    if(v->capacity - v->size > 0)                                                               \
    {                                                                                           \
        vector(type) t={0};                                                                     \
        $(vector_assign,type)(&t,v);                                                            \
        $(vector_move,type)(v,&t);                                                              \
        $(vector_del,type)(&t);                                                                 \
    }                                                                                           \
}                                                                                               \
static inline void $(vector_assign,type)(vector(type) *restrict t,vector(type) *restrict v)           \
{                                                                                                     \
    vector(type) c={0};                                                                               \
    if($(vector_size,type)(v)>0)                                                                      \
    {                                                                                                 \
        $(vector_reserve,type)(&c,$(vector_size,type)(v));                                            \
        c.size=$(vector_size,type)(v);                                                                \
        ssize_t i=0;                                                                                  \
        while(((c.size-i)&7)!=0)                                                                      \
        {                                                                                             \
            c.a[i]=v->a[i];                                                                           \
            i++;                                                                                      \
        }                                                                                             \
        while(i+7<c.size)                                                                             \
        {                                                                                             \
            c.a[i]=v->a[i];i++;                                                                       \
            c.a[i]=v->a[i];i++;                                                                       \
            c.a[i]=v->a[i];i++;                                                                       \
            c.a[i]=v->a[i];i++;                                                                       \
            c.a[i]=v->a[i];i++;                                                                       \
            c.a[i]=v->a[i];i++;                                                                       \
            c.a[i]=v->a[i];i++;                                                                       \
            c.a[i]=v->a[i];i++;                                                                       \
        }                                                                                             \
    }                                                                                                 \
    $(vector_move,type)(t,&c);                                                                        \
    $(vector_del,type)(&c);                                                                           \
}                                                                                                     \
static inline void $(vector_move,type)(vector(type) *restrict t,vector(type) *restrict v)       \
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
static inline void $(_vector_push_back_none,type)(vector(type) *v)                              \
{                                                                                               \
    ssize_t size=$(vector_size,type)(v);                                                        \
    ssize_t capacity=$(vector_capacity,type)(v);                                                \
    if(size==capacity)                                                                          \
        $(vector_reserve,type)(v,size+size/2+1);                                                \
    v->size++;                                                                                  \
}                                                                                               \
static inline void $(vector_push_back,type)(vector(type) *v,type e)                             \
{                                                                                               \
    $(_vector_push_back_none,type)(v);                                                          \
    v->a[v->size-1]=e;                                                                          \
}                                                                                               \
static inline void $(vector_pop_back,type)(vector(type) *v)                                     \
{                                                                                               \
    vector_assert(v->size>0,"vector empty but pop\n");                                          \
    v->size--;                                                                                  \
}                                                                                               \
static inline void $(vector_insert,type)(vector(type) *v,ssize_t index,type e)                  \
{                                                                                               \
    vector_assert(index>=0 && index<=$(vector_size,type)(v),"vector insert out of range\n");    \
    $(_vector_push_back_none,type)(v);                                                          \
    for(ssize_t i=$(vector_size,type)(v)-1;i>index;i--)                                         \
        *$(vector_at,type)(v,i)=*$(vector_at,type)(v,i-1);                                      \
    *$(vector_at,type)(v,index)=e;                                                              \
}                                                                                               \
static inline void $(vector_erase,type)(vector(type) *v,ssize_t index)                          \
{                                                                                               \
    vector_assert(index>=0 && index<$(vector_size,type)(v),"vector erase out of range\n");      \
    for(ssize_t i=index;i<$(vector_size,type)(v)-1;i++)                                         \
        *$(vector_at,type)(v,i)=*$(vector_at,type)(v,i+1);                                      \
    $(vector_pop_back,type)(v);                                                                 \
}

struct _vector
{
    ssize_t size;
    ssize_t capacity;
    void    *a;
};

#define vector_init(vptr) \
({\
    inline void vector_init_helper(typeof(vptr) v)    \
    {                                                 \
        *v=(typeof(*v)){.size=0,.capacity=0,.a=NULL}; \
    }                                                 \
    vector_init_helper(vptr);                         \
})

static inline void vector_del(void *v)
{
    struct _vector *vptr=v;
    free(vptr->a);
    vptr->size=0;
    vptr->capacity=0;
    vptr->a=NULL;
}

#define vector_clear(vptr) \
({\
    inline void vector_clear_helper(typeof(vptr) v) \
    {                                               \
        v->size=0;                                  \
    }                                               \
    vector_clear_helper(vptr);                      \
})

#define vector_pop_back(vptr) \
({\
    inline void vector_pop_back_helper(typeof(vptr) v)  \
    {                                                   \
        vector_assert(v->size>0);                       \
        v->size--;                                      \
    }                                                   \
    vector_pop_back_helper(vptr);                       \
})

#define vector_size(vptr) \
({\
    inline ssize_t vector_size_helper(typeof(vptr) v) \
    {                                                 \
        return v->size;                               \
    }                                                 \
    vector_size_helper(vptr);                         \
})

#define vector_capacity(vptr) \
({\
    inline ssize_t vector_capacity_helper(typeof(vptr) v)   \
    {                                                       \
        return v->capacity;                                 \
    }                                                       \
    vector_capacity_helper(vptr);                           \
})

#define vector_at(vptr,index) \
({\
    inline typeof((vptr)->a) vector_at_helper(typeof(vptr) v,ssize_t i)         \
    {                                                                           \
        vector_assert(i>=0 && i<vector_size(v),"vector access out of range\n"); \
        return &v->a[i];                                                        \
    }                                                                           \
    vector_at_helper(vptr,index);                                               \
})

#define vector_data(vptr) \
({\
    inline typeof((vptr)->a) vector_data_helper(typeof(vptr) v) \
    {                                                           \
        return v->a;                                            \
    }                                                           \
    vector_data_helper(vptr);                                   \
})

#define vector_get(vptr,index) \
({\
    inline typeof((vptr)->a[0]) vector_get_helper(typeof(vptr) v,ssize_t i) \
    {                                                                       \
        return *vector_at(v,i);                                             \
    }                                                                       \
    vector_get_helper(vptr,index);                                          \
})

#define vector_divert(vptr) \
({\
    inline typeof((vptr)->a) vector_divert_helper(typeof(vptr) v)  \
    {                                                              \
        __auto_type ret=v->a;                                      \
        v->size=0;                                                 \
        v->capacity=0;                                             \
        v->a=NULL;                                                 \
        return ret;                                                \
    }                                                              \
    vector_divert_helper(vptr);                                    \
})

#define vector_reserve(vptr,new_capacity_in) \
({\
    inline void vector_reserve_helper(typeof(vptr) v,ssize_t new_capacity)              \
    {                                                                                   \
        ssize_t capacity=vector_capacity(v);                                            \
        if(new_capacity>capacity)                                                       \
        {                                                                               \
            typeof(v->a) p=realloc(v->a,(size_t)new_capacity*sizeof(*p));               \
            if(p!=NULL)                                                                 \
            {                                                                           \
                v->a=p;                                                                 \
                v->capacity=new_capacity;                                               \
            }                                                                           \
            else                                                                        \
            {                                                                           \
                vector_debug("vector memory alloc failed\n");                           \
                exit(1);                                                                \
            }                                                                           \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            vector_debug("vector capacity is bigger equal than this new capacity\n");   \
        }                                                                               \
    }                                                                                   \
    vector_reserve_helper(vptr,new_capacity_in);                                        \
})

#define vector_resize(vptr,new_size_in) \
({\
    inline void vector_resize_helper(typeof(vptr) v,ssize_t new_size)       \
    {                                                                       \
        typedef typeof(v->a[0]) type;                                       \
        ssize_t size=vector_size(v);                                        \
        ssize_t capacity=vector_capacity(v);                                \
        if(new_size > size)                                                 \
        {                                                                   \
            if(new_size > capacity)                                         \
                vector_reserve(v,new_size);                                 \
            ssize_t i=size;                                                 \
            while(((new_size-i)&7)!=0)                                      \
                v->a[i++]=(type){0};                                        \
            while(i+7<new_size)                                             \
            {                                                               \
                v->a[i++]=(type){0};                                        \
                v->a[i++]=(type){0};                                        \
                v->a[i++]=(type){0};                                        \
                v->a[i++]=(type){0};                                        \
                v->a[i++]=(type){0};                                        \
                v->a[i++]=(type){0};                                        \
                v->a[i++]=(type){0};                                        \
                v->a[i++]=(type){0};                                        \
            }                                                               \
        }                                                                   \
        v->size=new_size;                                                   \
    }                                                                       \
    vector_resize_helper(vptr,new_size_in);                                 \
})

#define vector_assign(tptr,vptr) \
({\
    typedef typeof(*(tptr)) vector_type_t;                                  \
    inline void vector_assign_helper(vector_type_t *t,vector_type_t *v)     \
    {                                                                       \
        vector_type_t c={0};                                                \
        if(vector_size(v)>0)                                                \
        {                                                                   \
            vector_reserve(&c,vector_size(v));                              \
            c.size=vector_size(v);                                          \
            ssize_t i=0;                                                    \
            while(((c.size-i)&7)!=0)                                        \
            {                                                               \
                c.a[i]=v->a[i];                                             \
                i++;                                                        \
            }                                                               \
            while(i+7<c.size)                                               \
            {                                                               \
                c.a[i]=v->a[i];i++;                                         \
                c.a[i]=v->a[i];i++;                                         \
                c.a[i]=v->a[i];i++;                                         \
                c.a[i]=v->a[i];i++;                                         \
                c.a[i]=v->a[i];i++;                                         \
                c.a[i]=v->a[i];i++;                                         \
                c.a[i]=v->a[i];i++;                                         \
                c.a[i]=v->a[i];i++;                                         \
            }                                                               \
        }                                                                   \
        vector_move(t,&c);                                                  \
        vector_del(&c);                                                     \
    }                                                                       \
    vector_assign_helper(tptr,vptr);                                        \
})

// 移动赋值
#define vector_move(tptr,vptr) \
({\
    typedef typeof(*(tptr)) vector_type_t; \
    inline void vector_move_helper(vector_type_t *restrict t,vector_type_t *restrict v)    \
    {                                                                                      \
        vector_type_t p=*t;                                                                \
        *t=*v;                                                                             \
        *v=p;                                                                              \
    }                                                                                      \
    vector_move_helper(tptr,vptr);\
})

#define vector_shrink_to_fit(vptr) \
({\
    inline void vector_shrink_to_fit_helper(typeof(vptr) v)  \
    {                                                        \
        if(v->capacity - v->size > 0)                        \
        {                                                    \
            typeof(*v) t={0};                                \
            vector_assign(&t,v);                             \
            vector_move(v,&t);                               \
            vector_del(&t);                                  \
        }                                                    \
    }                                                        \
    vector_shrink_to_fit_helper(vptr);                       \
})

#define _vector_push_back_none(vptr) \
({\
    inline void _vector_push_back_none_helper(typeof(vptr) v)   \
    {                                                           \
        ssize_t size=vector_size(v);                            \
        ssize_t capacity=vector_capacity(v);                    \
        if(size==capacity)                                      \
            vector_reserve(v,size+size/2+1);                    \
        v->size++;                                              \
    }                                                           \
    _vector_push_back_none_helper(vptr);\
})

#define vector_push_back(vptr,e_in) \
({\
    inline void vector_push_back_helper(typeof(vptr) v,typeof(v->a[0]) e)   \
    {                                                                       \
        _vector_push_back_none(v);                                          \
        v->a[v->size-1]=e;                                                  \
    }                                                                       \
    vector_push_back_helper(vptr,e_in);                                     \
})

#define vector_insert(vptr,index_in,e_in) \
({\
    inline void vector_insert_helper(typeof(vptr) v,ssize_t index,typeof(v->a[0]) e)        \
    {                                                                                       \
        vector_assert(index>=0 && index<=vector_size(v),"vector insert out of range\n");    \
        _vector_push_back_none(v);                                                          \
        for(ssize_t i=vector_size(v)-1;i>index;i--)                                         \
            *vector_at(v,i)=*vector_at(v,i-1);                                              \
        *vector_at(v,index)=e;                                                              \
    }                                                                                       \
    vector_insert_helper(vptr,index_in,e_in);                                               \
})

#define vector_erase(vptr,index_in) \
({\
    inline void vector_erase_helper(typeof(vptr) v,ssize_t index)                      \
    {                                                                                  \
        vector_assert(index>=0 && index<vector_size(v),"vector erase out of range\n"); \
        for(ssize_t i=index;i<vector_size(v)-1;i++)                                    \
            *vector_at(v,i)=*vector_at(v,i+1);                                         \
        vector_pop_back(v);                                                            \
    }                                                                                  \
    vector_erase_helper(vptr,index_in);                                                \
})

#endif