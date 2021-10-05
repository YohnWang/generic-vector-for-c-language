#include"vector.h"
#include<time.h>

vector_def(int)
vector_def(vector(int))
vector_def(vector(vector(int)))

void syntax_test()
{
    vector(int) v,t={};
    vector_init(&v);
    vector_push_back(&v,10);
    vector_pop_back(&v);
    vector_size(&v);
    vector_capacity(&v);
    vector_resize(&v,1);
    vector_at(&v,0);
    vector_get(&v,0);
    vector_data(&v);
    vector_shrink_to_fit(&v);
    vector_clear(&v);
    vector_insert(&v,0,1);
    vector_erase(&v,0);
    vector_shrink_to_fit(&v);
    vector_move(&t,&v);
    vector_assign(&v,&t);
    vector_del_int(&v);
    vector_divert(&v);
}

void normal_test()
{
    vector_raii vector(int) v={};
    vector_push_back(&v,0);
    vector_push_back(&v,1);
    vector_push_back(&v,2);
    vector_push_back(&v,3);
    vector_push_back(&v,4);
    vector_push_back(&v,5);
    vector_push_back(&v,6);
    vector_push_back(&v,7);
    vector_push_back(&v,8);
    vector_push_back(&v,9);
    vector_push_back(&v,10);
    vector_push_back(&v,11);
    for(ssize_t i=0;i<vector_size(&v);i++)
        printf("v[%zd]=%d\n",i,vector_get(&v,i));
    printf("==========\n");
    vector_pop_back(&v);
    vector_pop_back(&v);
    for(ssize_t i=0;i<vector_size(&v);i++)
        printf("v[%zd]=%d\n",i,vector_get(&v,i));
    printf("==========\n");
    vector(int) t={};
    vector_assign(&t,&v);
    for(ssize_t i=0;i<vector_size(&v);i++)
        printf("t[%zd]=%d\n",i,*vector_at(&t,i));
    printf("==========\n");
    printf("v capacity=%zd\n",vector_capacity(&v));
    vector_shrink_to_fit(&v);
    printf("v capacity=%zd\n",vector_capacity(&v));
    printf("==========\n");
    for(ssize_t i=0;i<10000;i++)
        vector_push_back(&v,i);
    printf("v capacity=%zd\n",vector_capacity(&v));
    printf("v size    =%zd\n",vector_size(&v));
    vector_shrink_to_fit(&v);
    printf("v capacity=%zd\n",vector_capacity(&v));
    vector_reserve(&v,100000);
    printf("v capacity=%zd\n",vector_capacity(&v));
    vector_shrink_to_fit(&v);
    printf("v capacity=%zd\n",vector_capacity(&v));
    vector_del(&t);
    printf("==========\n");
 
    vector_clear(&v);
    vector_push_back(&v,0);
    for(ssize_t i=0;i<100;i++)
        vector_insert_int(&v,0,i);
    vector_erase_int(&v,vector_size(&v)-1);
    for(ssize_t i=0;i<vector_size(&v);i++)
        printf("%d ",vector_get(&v,i));
    printf("\n");
    printf("===========\n");
    vector_assign(&v,&v);
    for(ssize_t i=0;i<vector_size(&v);i++)
        printf("%d ",vector_get(&v,i));
    printf("\n");
    vector_del(&v);
    ssize_t cap=0;
    for(ssize_t i=0;i<10000000;i++)
    {
        vector_push_back(&v,i);
        if(vector_capacity(&v)!=cap)
        {
            printf("cap=%zd\n",vector_capacity(&v));
            cap=vector_capacity(&v);
        }
    }
    vector(vector(int)) vv={};
    vector_divert(&v);
    vector_move(&t,&v);
    vector_move(&v,&t);
    vector_clear(&vv);
    vector_resize(&vv,100);
}

void performance_test()
{
    vector(int) v={};
    double average=0;
    srand(time(NULL));
    for(int i=0;i<100;i++)
    {
        __attribute__((cleanup(vector_del)))
        vector(int) v={};
        int cycle=rand()%100000;
        for(ssize_t i=0;i<cycle;i++)
            vector_push_back(&v,i);
        double usage=1.0*vector_size(&v)/vector_capacity(&v);
        average+=usage;
        //printf("[%d]: usage = %f\n",i,usage);
    }
    printf("average usage = %f\n",average/100);
    printf("==========\n");
    average=0;
    for(int i=0;i<100;i++)
    {
        vector_init(&v);
        int copy_times=0;
        int cycle=rand()%100000;
        for(int i=0;i<cycle;i++)
        {
            if(vector_capacity(&v)==vector_size(&v))
            {
                copy_times+=vector_size(&v);
            }
            vector_push_back(&v,i);
        }
        //printf("copy precent :%f\n",1.0*copy_times/cycle);
        average+=1.0*copy_times/cycle;
    }
    printf("average copy precent :%f\n",average/100);
    printf("===========\n");
}

int main(void)
{
    syntax_test();
    normal_test();
    performance_test();
}