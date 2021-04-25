// Inclusion of this file is DEPRECATED.  This is the original HP
// default allocator.  It is provided only for backward compatibility.
// 


// DO NOT USE THIS FILE unless you have an old container implementation
// that requires an allocator with the HP-style interface.  SGI STL
// uses a different allocator interface.  SGI-style allocators are not
// parametrized with respect to the object type; they traffic in void *
// pointers.  This file is not included by any other SGI STL header.

/*
    这个版本，并没有使用这个文件里定义的allocator,而是使用的其他配置器
    实际使用是下面的默认配置器如下:
        template <class T, class Alloc = alloc>
        class vector{ ... } 
    本质上，这个allocator是通过operator new 和 operator delete来申请函数，代价较大

*/
#ifndef DEFALLOC_H
#define DEFALLOC_H

#include <new.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream.h>
#include <algobase.h>


template <class T>
inline T* allocate(ptrdiff_t size, T*) {
    set_new_handler(0);
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (tmp == 0) {
	cerr << "out of memory" << endl; 
	exit(1);
    }
    return tmp;
}


template <class T>
inline void deallocate(T* buffer) {
    ::operator delete(buffer);
}

/* 分配器的基类，模板类
    最重要的是 allocate 和 deallocate 这两个函数
*/
template <class T>
class allocator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
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

class allocator<void> {
public:
    typedef void* pointer;
};



#endif
