/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */
/*
STL 标准中，并没有区分 基本算法或复杂算法，然而 SGI 将一些常见的算法定义于 <stl_algobase.h>
之中,其它算法定义于 <stl_base.h> 中
*/

#ifndef __SGI_STL_INTERNAL_ALGOBASE_H
#define __SGI_STL_INTERNAL_ALGOBASE_H

#ifndef __STL_CONFIG_H
#include <stl_config.h>
#endif
#ifndef __SGI_STL_INTERNAL_RELOPS
#include <stl_relops.h>
#endif
#ifndef __SGI_STL_INTERNAL_PAIR_H
#include <stl_pair.h>
#endif
#ifndef __TYPE_TRAITS_H_
#include <type_traits.h>
#endif

#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <new.h>
#include <iostream.h>

#ifndef __SGI_STL_INTERNAL_ITERATOR_H
#include <stl_iterator.h>
#endif

__STL_BEGIN_NAMESPACE
/* 将 两个 ForwardIterator 所指对象对调，第三参数并没有定义参数名称,使用了
  利用了 C++ 的参数推导机制
*/
template <class ForwardIterator1, class ForwardIterator2, class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
  T tmp = *a;
  *a = *b;
  *b = tmp;
}
/*
* 不借助 _iter_swap 辅助函数的话,iter_swap 就应该这样写
* typename iterator_traits<ForwardIterator>::value_type tmp = *a;
* *a = *b;
* *b = tmp;
*/
template <class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
  __iter_swap(a, b, value_type(a)); //第三参数的接受端是 T*
}
//交换两个对象,是通过 copy 的方式实现的,在某些情况下很慢
template <class T>
inline void swap(T& a, T& b) {
  T tmp = a;
  a = b;
  b = tmp;
}

#ifndef __BORLANDC__

#undef min
#undef max
//返回较小值
template <class T>
inline const T& min(const T& a, const T& b) {
  return b < a ? b : a;
}
//返回较大值
template <class T>
inline const T& max(const T& a, const T& b) {
  return  a < b ? b : a;
}

#endif /* __BORLANDC__ */

template <class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
  return comp(b, a) ? b : a;
}

template <class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
  return comp(a, b) ? b : a;
}
/*
* 因为 copy 函数，常常被调用,所以 STL 对其做了特别优化
*/

//1. copy 函数的对外接口,完全泛化版本
template <class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
                           OutputIterator result)
{
  return __copy_dispatch<InputIterator,OutputIterator>()(first, last, result);
}
//2. 重载,对于 原生指针 const char* 版本
inline char* copy(const char* first, const char* last, char* result) {
  memmove(result, first, last - first);
  return result + (last - first);
}
//3. 重载,对于 原生版本 const wchar_t* 版本
inline wchar_t* copy(const wchar_t* first, const wchar_t* last,
                     wchar_t* result) {
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}

/*
*  __copy_dispatch 共有三个版本
*/
//1. 完全泛化版本,针对不同种类的迭代器所使用的 _copy 版本也不一样
template <class InputIterator, class OutputIterator>
struct __copy_dispatch
{
  OutputIterator operator()(InputIterator first, InputIterator last,
                            OutputIterator result) {
    return __copy(first, last, result, iterator_category(first));
  }
};
//偏特化版本二
template <class T>
struct __copy_dispatch<T*, T*>
{
  T* operator()(T* first, T* last, T* result) {
    typedef typename __type_traits<T>::has_trivial_assignment_operator t; 
    return __copy_t(first, last, result, t());
  }
};
//偏特化版本三
template <class T>
struct __copy_dispatch<const T*, T*>
{
  T* operator()(const T* first, const T* last, T* result) {
    typedef typename __type_traits<T>::has_trivial_assignment_operator t; 
    return __copy_t(first, last, result, t());
  }
};

//__copy 的 InputIterator 版本
template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last,
                             OutputIterator result, input_iterator_tag)
{
  for ( ; first != last; ++result, ++first)
    *result = *first;
  return result;
}
//__copy 的 RandomAccessIterator 版本,调用_copy_d
template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator 
__copy(RandomAccessIterator first, RandomAccessIterator last,
       OutputIterator result, random_access_iterator_tag)
{
  return __copy_d(first, last, result, distance_type(first));
}



//__copy_dispatch 的偏特化版本三 对应的函数
//以下版本适用于 “指针所指对象具有 trivial assignment operator”
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
  memmove(result, first, sizeof(T) * (last - first));
  return result + (last - first);
}
//__copy_dispatch 的偏特化版本三 对应的函数
//以下版本适用于 “指针所指对象具有 non-trivial assignment operator”
template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
  //原生指针 毕竟是一种 RandomAccessIterator,所以交给 _copy_d() 完成
  return __copy_d(first, last, result, (ptrdiff_t*) 0);
}

template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator
__copy_d(RandomAccessIterator first, RandomAccessIterator last,
         OutputIterator result, Distance*)
{
  // 速度比 InputIterator 版本快
  for (Distance n = last - first; n > 0; --n, ++result, ++first) 
    *result = *first;
  return result;
}

/* 
*  将[first, last) 逆向复制到 result -1 为起点逆向进行复制的区间上
*  算法实现的技巧上与 copy 类似
*/
template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, 
                                              BidirectionalIterator1 last, 
                                              BidirectionalIterator2 result) {
  while (first != last) *--result = *--last;
  return result;
}


template <class BidirectionalIterator1, class BidirectionalIterator2>
struct __copy_backward_dispatch
{
  BidirectionalIterator2 operator()(BidirectionalIterator1 first, 
                                    BidirectionalIterator1 last, 
                                    BidirectionalIterator2 result) {
    return __copy_backward(first, last, result);
  }
};

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION 

template <class T>
inline T* __copy_backward_t(const T* first, const T* last, T* result,
                            __true_type) {
  const ptrdiff_t N = last - first;
  memmove(result - N, first, sizeof(T) * N);
  return result - N;
}

template <class T>
inline T* __copy_backward_t(const T* first, const T* last, T* result,
                            __false_type) {
  return __copy_backward(first, last, result);
}

template <class T>
struct __copy_backward_dispatch<T*, T*>
{
  T* operator()(T* first, T* last, T* result) {
    typedef typename __type_traits<T>::has_trivial_assignment_operator t; 
    return __copy_backward_t(first, last, result, t());
  }
};

template <class T>
struct __copy_backward_dispatch<const T*, T*>
{
  T* operator()(const T* first, const T* last, T* result) {
    typedef typename __type_traits<T>::has_trivial_assignment_operator t; 
    return __copy_backward_t(first, last, result, t());
  }
};

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, 
                                            BidirectionalIterator1 last, 
                                            BidirectionalIterator2 result) {
  return __copy_backward_dispatch<BidirectionalIterator1, 
                                  BidirectionalIterator2>()(first, last, 
                                                            result);
}

template <class InputIterator, class Size, class OutputIterator>
pair<InputIterator, OutputIterator> __copy_n(InputIterator first, Size count,
                                             OutputIterator result,
                                             input_iterator_tag) {
  for ( ; count > 0; --count, ++first, ++result)
    *result = *first;
  return pair<InputIterator, OutputIterator>(first, result);
}

template <class RandomAccessIterator, class Size, class OutputIterator>
inline pair<RandomAccessIterator, OutputIterator>
__copy_n(RandomAccessIterator first, Size count,
         OutputIterator result,
         random_access_iterator_tag) {
  RandomAccessIterator last = first + count;
  return pair<RandomAccessIterator, OutputIterator>(last,
                                                    copy(first, last, result));
}

template <class InputIterator, class Size, class OutputIterator>
inline pair<InputIterator, OutputIterator>
copy_n(InputIterator first, Size count,
       OutputIterator result) {
  return __copy_n(first, count, result, iterator_category(first));
}
/*
  将[first, last)内所有元素赋为新值
*/
template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value) {
  for ( ; first != last; ++first)
    *first = value;
}
/*
 * 前 n 个元素被设置为新值
 * 但是循环的判断条件并没有 first < last, 所以如果 n > 容器的大小那么这个函数会直接覆盖掉容器之后的元素
 * 避免条件是 利用 inserter 迭代器产生一个插入功能而非覆写操作的适配器(使用 = 时，会调用容器的 insert 函数)
 */ 
template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
  for ( ; n > 0; --n, ++first)
    *first = value;
  return first;
}
/*
* 用来平行比较两个序列，指出两者之间的第一个不匹配点，
* 返回 不匹配时，两个序列中位置
*/
template <class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
					      InputIterator1 last1,
					      InputIterator2 first2) {
  while (first1 != last1 && *first1 == *first2) {
    ++first1;
    ++first2;
  }
  return pair<InputIterator1, InputIterator2>(first1, first2);
}

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
					      InputIterator1 last1,
					      InputIterator2 first2,
					      BinaryPredicate binary_pred) {
  while (first1 != last1 && binary_pred(*first1, *first2)) {
    ++first1;
    ++first2;
  }
  return pair<InputIterator1, InputIterator2>(first1, first2);
}
/*
*  如果两个序列在 [first, last)区间相等，则返回true(使用的默认比较依据是 operator!=)
*  但 第二个序列长出来的部分不在考虑范围内
*/
//版本一
template <class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
		  InputIterator2 first2) {
  for ( ; first1 != last1; ++first1, ++first2)
    if (*first1 != *first2)
      return false;
  return true;
}
//版本二，可以指定 比较依据
template <class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
		  InputIterator2 first2, BinaryPredicate binary_pred) {
  for ( ; first1 != last1; ++first1, ++first2)
    if (!binary_pred(*first1, *first2))
      return false;
  return true;
}
/*
*  以字典排序的方式，对两个序列[first1, last1) 和 [first2, last2) 进行比较
*/
template <class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
			     InputIterator2 first2, InputIterator2 last2) {
  for ( ; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 < *first2)
      return true;
    if (*first2 < *first1)
      return false;
  }
  return first1 == last1 && first2 != last2;
}

template <class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
			     InputIterator2 first2, InputIterator2 last2,
			     Compare comp) {
  for ( ; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (comp(*first1, *first2))
      return true;
    if (comp(*first2, *first1))
      return false;
  }
  return first1 == last1 && first2 != last2;
}
//原生指针 const unsigned char* 类型
inline bool 
lexicographical_compare(const unsigned char* first1,
                        const unsigned char* last1,
                        const unsigned char* first2,
                        const unsigned char* last2)
{
  const size_t len1 = last1 - first1;
  const size_t len2 = last2 - first2;
  //比较相同长度的部分
  const int result = memcmp(first1, first2, min(len1, len2));
  //如果相同部分相同，则长度更长的元素较大
  return result != 0 ? result < 0 : len1 < len2;
}
// const char* 版本
inline bool lexicographical_compare(const char* first1, const char* last1,
                                    const char* first2, const char* last2)
{
#if CHAR_MAX == SCHAR_MAX
  return lexicographical_compare((const signed char*) first1,
                                 (const signed char*) last1,
                                 (const signed char*) first2,
                                 (const signed char*) last2);
#else
  return lexicographical_compare((const unsigned char*) first1,
                                 (const unsigned char*) last1,
                                 (const unsigned char*) first2,
                                 (const unsigned char*) last2);
#endif
}

template <class InputIterator1, class InputIterator2>
int lexicographical_compare_3way(InputIterator1 first1, InputIterator1 last1,
                                 InputIterator2 first2, InputIterator2 last2)
{
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) return -1;
    if (*first2 < *first1) return 1;
    ++first1; ++first2;
  }
  if (first2 == last2) {
    return !(first1 == last1);
  } else {
    return -1;
  }
}

inline int
lexicographical_compare_3way(const unsigned char* first1,
                             const unsigned char* last1,
                             const unsigned char* first2,
                             const unsigned char* last2)
{
  const ptrdiff_t len1 = last1 - first1;
  const ptrdiff_t len2 = last2 - first2;
  const int result = memcmp(first1, first2, min(len1, len2));
  return result != 0 ? result : (len1 == len2 ? 0 : (len1 < len2 ? -1 : 1));
}

inline int lexicographical_compare_3way(const char* first1, const char* last1,
                                        const char* first2, const char* last2)
{
#if CHAR_MAX == SCHAR_MAX
  return lexicographical_compare_3way(
				(const signed char*) first1,
                                (const signed char*) last1,
                                (const signed char*) first2,
                                (const signed char*) last2);
#else
  return lexicographical_compare_3way((const unsigned char*) first1,
                                      (const unsigned char*) last1,
                                      (const unsigned char*) first2,
                                      (const unsigned char*) last2);
#endif
}

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_ALGOBASE_H */

// Local Variables:
// mode:C++
// End:
