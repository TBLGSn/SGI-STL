/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_CONSTRUCT_H
#define __SGI_STL_INTERNAL_CONSTRUCT_H

#include <new.h>

__STL_BEGIN_NAMESPACE

template <class T1, class T2>
inline void construct(T1* p, const T2& value) {
  new (p) T1(value); // 调用T1::T1(value)
}
// destroy 的第一个版本
template <class T>
inline void destroy(T* pointer) {
    pointer->~T(); // 调用dctor ~T()
}

// destroy 的第二个版本
template <class ForwardIterator> 
inline void __destroy_aux(ForwardIterator, ForwardIterator, __true_type) {}

template <class ForwardIterator>
inline void
__destroy_aux(ForwardIterator first, ForwardIterator last, __false_type) {
  for ( ; first < last; ++first)
    destroy(&*first); //调用的是版本一
}

template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last, T*) {
  // 根据元素 value type有无 trivial_destructor (通过 __false_type 和 __true_type 标示出来).
  //如果有则要调用对应的析构函数,才能真正完成内存释放 
  typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
  __destroy_aux(first, last, trivial_destructor());
}

template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
  // 利用 类型的 value_type   调用最佳版本
  __destroy(first, last, value_type(first));
}
// 第二版本对于 char* 和 wchar_t* 的特化版本
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_CONSTRUCT_H */

// Local Variables:
// mode:C++
// End:
