#include"vector.h"

vector_def(int)
vector_def(vector(int))
vector_def(vector(vector(int)))

int main()
{
    vector_debug("\n");
    vector(int) v;
    vector_init(&v);
    vector_resize_int(&v,10);
    vector_at_int(&v,1);
    vector_debug("\n");
    vector_del(&v);
    vector_debug("\n");
    vector_resize_int(&v,100000000);
    vector_debug("\n");
    printf("%zu\n",vector_size_int(&v));
    vector_debug("\n");
    vector(int) t={0};
    vector_assign_int(&t,&v);
    vector_del(&v);
    vector_shrink_to_fit_int(&v);
    vector_del(&v);
    vector_del(&t);
    for(int i=0;i<1000;i++)
    {
        vector_push_back(&v,i);
        printf("size=%zd capacity=%zd\n",vector_size(&v),vector_capacity(&v));
    }
    vector_reserve(&v,10000);
    vector_resize(&t,5000);
    vector_divert(&v);
    vector_shrink_to_fit(&t);
    vector_push_back(&t,1);
}