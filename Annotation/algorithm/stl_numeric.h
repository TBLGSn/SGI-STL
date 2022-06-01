/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

/*
   数值算法
*/

#ifndef __SGI_STL_INTERNAL_NUMERIC_H
#define __SGI_STL_INTERNAL_NUMERIC_H

__STL_BEGIN_NAMESPACE
/* 
 * 将元素"累计"到初值 init 上 ，得益于容器实现时的抽象，例如对于 operator ++ 的重写，
 * 我们只需要使用 ++ first 便能使遍历指针前进到下一个元素的位置
 * 注意 init 的值没有默认实现，你一定需要指定 init 的值,最重要的是 init 的类型T有溢出的风险,在实现该
 * 函数的时候,你应该考虑这一点: long long sum = accumulate(nums.begin(), nums.end(), 0LL);
 */ 
template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init) {
  for ( ; first != last; ++first)
    init = init + *first;
  return init;
}

template <class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init,
             BinaryOperation binary_op) {
  for ( ; first != last; ++first)
    init = binary_op(init, *first);
  return init;
}
/* 
* 计算 [first1, last1) 和 [first2, first2 + (last1-first1)) 的一般内积
*/
template <class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init) {
  for ( ; first1 != last1; ++first1, ++first2)
    init = init + (*first1 * *first2);
  return init;
}

template <class InputIterator1, class InputIterator2, class T,
          class BinaryOperation1, class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init, BinaryOperation1 binary_op1,
                BinaryOperation2 binary_op2) {
  for ( ; first1 != last1; ++first1, ++first2)
    init = binary_op1(init, binary_op2(*first1, *first2));
  return init;
}
/*
*   计算局部总和
*/
template <class InputIterator, class OutputIterator, class T>
OutputIterator __partial_sum(InputIterator first, InputIterator last,
                             OutputIterator result, T*) {
  T value = *first;
  while (++first != last) {
    value = value + *first; //计算 n 个元素的总和
    *++result = value;
  }
  return ++result;
}

template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result) {
  if (first == last) return result;
  *result = *first;
  return __partial_sum(first, last, result, value_type(first));
}

template <class InputIterator, class OutputIterator, class T,
          class BinaryOperation>
OutputIterator __partial_sum(InputIterator first, InputIterator last,
                             OutputIterator result, T*,
                             BinaryOperation binary_op) {
  T value = *first;
  while (++first != last) {
    value = binary_op(value, *first);
    *++result = value;
  }
  return ++result;
}

template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryOperation binary_op) {
  if (first == last) return result;
  *result = *first;
  return __partial_sum(first, last, result, value_type(first), binary_op);
}
/*
*  用来 计算[first, last) 中响铃元素的差额(并不是总和)
*  当 result == first 时,该算法在原区间上直接操作，是更易型算法
*/
//版本一
template <class InputIterator, class OutputIterator, class T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, 
                                     OutputIterator result, T*) {
  T value = *first;
  while (++first != last) {  //遍历整个区间
    T tmp = *first;
    *++result = tmp - value;//当前元素与之前元素的差值,赋给 目的端
    value = tmp;
  }
  return ++result;
}

template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, 
                                   OutputIterator result) {
  if (first == last) return result;
  *result = *first;
  return __adjacent_difference(first, last, result, value_type(first));
}
// 版本二: 可以指定 BinaryOperation 版本
template <class InputIterator, class OutputIterator, class T, 
          class BinaryOperation>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, 
                                     OutputIterator result, T*,
                                     BinaryOperation binary_op) {
  T value = *first;
  while (++first != last) {
    T tmp = *first;
    *++result = binary_op(tmp, value);
    value = tmp;
  }
  return ++result;
}

template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result,
                                   BinaryOperation binary_op) {
  if (first == last) return result;
  *result = *first;
  return __adjacent_difference(first, last, result, value_type(first),
                               binary_op);
}

/*
*  并不属于 STL 标准
*/
/*
  版本一中 MonoidOperation 需要满足结合律，但不需要满足交换律 
*/
template <class T, class Integer, class MonoidOperation>
T power(T x, Integer n, MonoidOperation op) {
  if (n == 0)
    return identity_element(op);
  else {
    // 循环 {n 转换为二进制尾部连续为 0 个数] 次
    while ((n & 1) == 0) {
      n >>= 1;
      x = op(x, x);
    }
    // 循环完 x = 2^t [t:n 转换为二进制尾部连续为 0 个数]
    T result = x;
    n >>= 1;
    while (n != 0) {
      x = op(x, x);
      // n 的二进制元素为 1 时
      if ((n & 1) != 0)
        result = op(result, x);
      n >>= 1;
    }
    return result;
  }
}
//版本二 指定运算型为 乘法
template <class T, class Integer>
inline T power(T x, Integer n) {
  return power(x, n, multiplies<T>());
}
/*
  不属于 STL 标准,更易型算法,从指定的 value开始,呈现递增状态
*/

template <class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T value) {
  while (first != last) *first++ = value++;
}

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_NUMERIC_H */

// Local Variables:
// mode:C++
// End:
