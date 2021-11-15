/*
 * @Author: tblgsn
 * @Date: 2021-10-05 09:58:18
 * @Description: HP 默认的空间分配器(实际上并没有使用分配器),因为本质上这个 allocator
 *               只是对于 operator new 和 operator delete 的简单封装。
 * @FilePath: /SGI-STL/Annotation/allocator/defalloc.h
 */

#ifndef DEFALLOC_H
#define DEFALLOC_H

#include <new.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream.h>
#include <algobase.h>

/**
 * @description: 用于申请内存空间,实际上是通过调用 operator new 来实现的
 * @param { size 的类型等价于 long int, 通过 sizeof(T) 得到类型的大小}
 * @return {一个指针，指向申请的空间}
 */
template <class T>
inline T* allocate(ptrdiff_t size, T*) {
    /* 声明在 new.h 中的标准程序库函数。参数实际上是一个函数指针， 当 operator new 
    无法分配足够的内存时，就会调用这个函数。这里被定义为 0(nullptr)*/
    set_new_handler(0); 
    //申请空间，可能会抛出 bad_alloc 异常
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {z
        cerr << "out of memory" << endl; 
        exit(1);
    }
    return tmp;
}


template <class T>
inline void deallocate(T* buffer) {
    // 调用 operator delete 释放指针指向的内存空间
    ::operator delete(buffer);
}

/* 分配器的基类，模板类
    最重要的是 allocate 和 deallocate 这两个函数
*/
template <class T>
class allocator {
public:
    //各种type
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    // 成员函数
    pointer allocate(size_type n) { 
	return ::allocate((difference_type)n, (pointer)0); // 相当于调用::operator new()
    }
    void deallocate(pointer p) { ::deallocate(p); } //调用::operator delete()
    pointer address(reference x) { return (pointer)&x; }
    const_pointer const_address(const_reference x) { 
	return (const_pointer)&x; 
    }
    size_type init_page_size() { 
	return max(size_type(1), size_type(4096/sizeof(T))); 
    }
    size_type max_size() const { 
	return max(size_type(1), size_type(UINT_MAX/sizeof(T))); 
    }
};
//void 类型的特化版本
class allocator<void> {
public:
    typedef void* pointer;
};



#endif
