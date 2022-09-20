# vector

该项目已合并至[gcc-magic-lib](https://github.com/YohnWang/gcc-magic-lib)

a generic vector container for c like c++

# 概述

该vector实现基于部分gcc拓展，并且可以不使用必须依赖gcc拓展的功能，以实现标准c可以使用的目的

使用该vector，可以实现任意类型（内置数组除外）的动态数组，用于自动管理数组容量

# 如何使用

vector是基于头文件的，所有的实现都只包含在头文件中，因此，使用前必须先`#include"vector.h"`

由于vector实现是基于宏的，在未指定类型定义前，没有任何关于vector的定义（因为宏还未展开）

因此，在使用指定类型的vector前，需要定义类型，使用`vector_def()`定义vector类型，例如

```c
#include"vector.h"
vector_def(int)          //正确的定义
vector_def(vector(int)); //不正确但能工作的，尾部的';'是多余的
vector_def(int8_t*)      //错误的定义，vector的实现本质是字符拼接，'*'是不合法的标识符定义
typedef int8_t* int8_ptr;
vector_def(int8_ptr)     //正确的，int8_ptr是合法的标识符
```

`vector_def()`接受的参数必须是具有**单独单词的类型**，不能是复合类型，或者具有修饰的类型，例如`unsigned int`，这些由多个单词或符号拼接的类型都是非法的，在使用这些类型作为参数前，需要`typedef`定义一个类型别名（对于宏定义的类型，在预处理阶段单词会被替换掉，所以实际生成的类型名与函数名将会使用替换后的名字，因此不建议使用宏定义的类型）

`vector_def()`必须位于该vector使用之前的位置，并且应该处于全局作用域

# 定义vector变量

有两种方法定义vector变量，分别为

```c
vector(int) v1;
vector_int  v2;
```

实际上`vector()`宏会将接收到的参数进行字符拼接，得到第二种形式

vector也支持嵌套定义，例如`vector(vector(int)) vv;`也是合法的，它等价于`vector_vector_int vv;`

# vector初始化

vector具有两种初始化形式，一种调用初始化函数，另一种只需要保证vector的所有成员变量为0

```c
vector(int) v1;
vector_init(&v1);   // 调用初始化函数
vector(int) v2={0}; // 标准c的结构体初始化，vector实际上就是一个结构体
vector(int) v3={};  // gcc支持的初始化，这是c++的标准初始化写法之一 //推荐写法:[1]
```

vector在使用其他所有操作之前，都**必须初始化**

*1. 这种写法可以使得使用了该vector的代码移植到c++变得更容易，因为标准c的结构体初始化与c++不兼容，所有的宏都可以替换成c++的std::vector，但c++的vector定义时如果是`vector<int> v={0}` ，它将会认为vector中有一个值为0的元素。不过目前并没有将其设计成与c++兼容的形式，vector是一个c项目*

# 向vector添加元素

### 1. vector_push_back(vector *v, T e)

`vector_push_back()`向vector的尾部添加一个元素，不需要关心vector的空间是否足够，空间不足会自动重新申请一块更大的空间，并复制原有的元素。该操作具有摊还的O(1)时间复杂度。

### 2. vector_insert(vector *v, ssize_t index, T e)

`vector_insert()`向vector的指定位置添加一个元素，添加位置的下标不能是任意的，范围是`[0,vector_size(&v)]`，允许在最后一个元素的下一个位置处添加元素。该操作的时间复杂度为O(n)。

```c
vector(int) v={};
vector_push_back(&v,0); //向尾部添加元素0
vector_insert(&v,1,0);  //向位置1添加元素0
```

**扩容机制**

vector的扩容公式为`size=size+size/2+1`，即每次将会增加一半（向零取整）加1的元素，不采用2倍增长方式的主要考虑是第三次扩容时的容量应该小于前两次的容量之和，这样具有更好的局部性，降低内存碎片的问题

# 删除vector中的元素

### 1. vector_pop_back(vector *v)

`vector_pop_back()`会删除尾部的元素，实际的操作是将`size`成员减1，因此该操作的时间复杂度为O(1)

### 2. vector_erase(vector *v, ssize_t index)

`vector_erase()`会删除指定下标的元素，该操作的时间复杂度为O(n)

### 3. vector_clear(vector *v)

`vector_clear()`将清空所有元素，实际的操作是将`size`成员置0，该操作的时间复杂度为O(1)

# 访存vector中的元素

### 1. T* vector_at(vector *v, ssize_t index)

`vector_at()`会返回指定下标元素的指针，该操作可用于引用或修改指定位置的元素，具有O(1)的时间复杂度

`vector_at()`保证会检查当前的下标

### 2. T vector_get(vector *v, ssize_t index)

`vector_get()`会返回指定下标的元素值，用于访问该元素，它是存储元素的一份拷贝，具有O(1)的时间复杂度

`vector_get()`是`vector_at()`的便利写法

### 3. void vector_set(vector *v, ssize_t index, T e)

`vector_set()`用于设置指定下标的元素值，具有O(1)的时间复杂度

`vector_set()`是`vector_at()`的便利写法

# 获取vector的容量与元素个数

### 1. ssize_t vector_capacity(vector *v)

`vector_capacity()`用于返回vector当前持有的内存能容纳元素的个数

### 2. ssize_t vector_size(vector *v)

`vector_size()`用于返回vector当前存储的元素个数

# 资源管理

### 1. vector_reserve(vector *v, ssize_t new_capacity)

`vector_reserve()`用于指定vector持有的内存必须至少能容纳`new_capacity`个元素。该操作用于预先分配指定长度的内存，以减少添加元素时自动扩容带来的拷贝开销

### 2. vector_resize(vector *v, ssize_t new_size)

`vector_resize()`用于指定vector当前的元素个数，如果`new_size`大于当前元素个数，将会用0元素值填充，如果`new_size`小于当前元素个数，则后面的多余元素将会被删除

### 3. T* vector_data(vector *v)

`vector_data()`返回当前vector所持有内存的地址，它的目的是提供一个可以优化的接口访问数据，而避免`vector_at()`带来的下标检查开销

### 4. T* vector_divert(vector *v)

`vector_divert()`用于转移当前vector所持有的内存，它将返回持有内存的地址，并且不在拥有其控制权，vector中的所有成员将会被置0

### 5. vector_shrink_to_fit(vector *v)

`vector_shrink_to_fit()`将使得vector持有的内存刚好容纳目前的所有元素，它用于提高内存利用率

**除非有必要，否则不建议使用这些功能，性能瓶颈往往不在这里，资源的所有权也不应该向外部泄露**

# 拷贝与移动

### 1. vector_assign(vector *t, vector *v)

`vector_assign()`用于拷贝`v`至`t`，因为拷贝过程中会先释放`t`的资源，因此`t`必须初始化后才能使用

### 2. vector_move(vector *t, vector *v)

`vector_move()`用于模拟c++中的移动赋值，将`v`的资源所有权转移给`t`。目前的实现是交换`t`和`v`的成员（这也是实现定义，可能会更改），因此，不能假设所有权转移后`t`的资源被释放，仍然需要保留`v`和`t`的`vector_del()`操作。**在不了解c++的移动语义时，不要使用该函数**

# vector删除

### vector_del(vector *v)

`vector_del()`用于删除并释放vector的资源，它等价于c++的析构函数，由于c中必须手动管理，因此在使用完vector后需要执行该操作。执行该操作之后的vector等价于刚初始化后的状态。

gcc具有cleanup拓展，使得vector可以在离开作用域时自动执行`vector_del()`，写法如下

```c
void f()
{
    __attribute__((cleanup(vector_del))) vector(int) v={};
    //do something
    if(...)
    {
        //do something
        return;
    }
    //do something
    return;
}
```

`v`在函数`f`的作用域内，当函数退出时，将自动执行`vector_del(&v)`，这样我们就能将vector看做是普通的值，不在需要关系资源的释放问题

`vector_raii`宏用于`__attribute__((cleanup(vector_del)))`的便利写法，需要注意，该属性不能用于描述函数参数（vector的拷贝赋值需要调用`vector_assign()`，因此，vector的所有传参应该都是指针传递方式，因此不需要在函数参数中提供cleanup）

# 指定函数类型

在`vector_def`时，会定义指定类型的函数，这些函数不同于自动推导类型的宏，它们具有全局作用域的函数实体（宏没有函数实体，但在实现中，会定义一个局部的辅助内嵌函数，离开作用域后将访问不到这些函数，`vector_del()`比较特殊，它不是宏实现的，而是一个函数实体，主要考虑是出于对cleanup属性的支持，它必须接受一个函数）

指定函数的写法为在上述的所有宏函数后添加`_T`，`T`是`vector_def()`中填入的参数，例如

```c
vector_def(int)
void f()
{
    vector(int) v={};
    vector_push_back_int(&v,0);
    vector_reserve_int(&v,10);
    vector_del_int(&v);
}
```

这些函数符合标准c的定义，没有使用gcc拓展，因此它可以在不支持gcc拓展的编译器上工作，并且由于是类型明确的，错误提示将会更友好

# 异常处理机制

vector将会面临两种异常

1. 内存申请失败
2. 越界访问（只在DEBUG模式下检查是否越界）

这两种实际上都是不可恢复的错误，从这种错误中恢复过来是不可能的，因此目前遇到这两种异常将会打印错误报告，并直接退出程序。

TODO：提供一种方式，在内存申请失败以及访问越界时提供一个默认的无效操作，保证程序可以运行下去（某些场景可以容忍程序错误，但不能崩溃）

