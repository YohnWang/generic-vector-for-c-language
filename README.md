# vector
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

`vector_def()`接受的参数必须是具有**单独单词的类型**，不能是复合类型，或者具有修饰的类型，例如`unsigned int`，这些由多个单词或符号拼接的类型都是非法的，在使用这些类型作为参数前，需要`typedef`定义一个类型别名

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

vector在使用其他所有操作之前，都必须初始化

*1. 这种写法可以使得使用了该vector的代码移植到c++变得更容易，因为标准c的结构体初始化与c++不兼容，所有的宏都可以替换成c++的std::vector，但c++的vector定义时如果是`vector<int> v={0}` ，它将会认为vector中有一个值为0的元素。不过目前并没有将其设计成与c++兼容的形式，vector是一个c项目*

# 向vector添加元素

**1. vector_push_back()**

`vector_push_back()`向vector的尾部添加一个元素，不需要关心vector的空间是否足够，空间不足会自动重新申请一块更大的空间，并复制原有的元素

**2. vector_insert()**

`vector_insert()`向vector的指定位置添加一个元素，添加位置的下标不能是任意的，范围是`[0,vector_size(&v)]`，允许在最后一个元素的下一个位置处添加元素

```c
vector(int) v={};
vector_push_back(&v,0); //向尾部添加元素0
vector_insert(&v,1,0);  //向位置1添加元素0
```

**扩容机制**

vector的扩容公式为`size=size+size/2+1` ，即每次将会增加一半（向零取整）加1的元素，不采用2倍增长方式的主要考虑是第三次扩容时的容量应该小于前两次的容量之和，这样具有更好的局部性

