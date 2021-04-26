# 空间配置器
只为容器提供底层支持, 这使得容器不在面对底层编程,容器要做的只是使用适合自己结构的 **不同空间配置器**就可以了.

## 组织方式
一般而言,C++ 内存配置操作和释放是这样的:
```c++
class Foo{};
Foo* pf = new Foo; //配置内存,然后构造对象
delete pf; //析构对象,释放内存
```
因此,STL 中的 allocator 将这两个阶段操作区分开来. 内存配置操作由 alloc::allocate() 负责,内存的释放由 alloc::deallocate()负责.
对象构造工作由 ::construct() 负责, 对象析构操作由 ::destroy() 负责. 
> STL 的 <memory> 内包含着两个文件,一个是 <stl_alloc.h> 负责内存空间的释放和配置,一个是 <stl_construct.h> 负责内存对象的构造和析构.

![allocator_structure.png](/image/allocator_structure.png?raw=true) 
## 使用方式
```c++
template <class T, class Alloc = alloc> // 指定缺省 空间配置器
class vector {
    typedef simple_alloc<value_type, Alloc> data_allocator;
    //...
}
//指定自己的空间配置器
vector<int, std::allocator<int> >// vc or CB
vector<int, std::alloc> iv; // GCC

// 或者调用上面说明的全局函数
std::destroy(...);
std::construct(...)
uninitialized_copy()
```