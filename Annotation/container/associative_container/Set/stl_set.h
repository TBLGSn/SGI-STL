/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef __SGI_STL_INTERNAL_SET_H
#define __SGI_STL_INTERNAL_SET_H

__STL_BEGIN_NAMESPACE

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1174
#endif

/*
  set 和 multiset以rb-tree为底层结构,因此有自动排序的特性
  set 和 multiset 的 key 就是value
  虽然 br-tree能够通过iterator改变元素的值，然而set和multiset并不支持
  什么时候用 内置 空间配置器.
*/
#ifndef __STL_LIMITED_DEFAULT_TEMPLATES
template <class Key, class Compare = less<Key>, class Alloc = alloc>
#else
template <class Key, class Compare, class Alloc = alloc>
#endif
class set {
public:
  // typedefs:
  // key_type 和 value_type都是 Key
  typedef Key key_type;
  typedef Key value_type;
  
  typedef Compare key_compare;
  typedef Compare value_compare;
private:
  /*

    红黑树 为 底层结构,大部分操作通过适配 红黑树实现,其中key_type和value指定为同一个
    keyOfvalue 指定为 identity<value_type>
  */
  typedef rb_tree<key_type, value_type, 
                  identity<value_type>, key_compare, Alloc> rep_type;
  rep_type t;  //以红黑树为底层架构

public:
  typedef typename rep_type::const_pointer pointer;
  typedef typename rep_type::const_pointer const_pointer;
  typedef typename rep_type::const_reference reference;
  typedef typename rep_type::const_reference const_reference;
  //set的迭代器是红黑树的 const迭代器,所以我们不能通过迭代器更改元素
  typedef typename rep_type::const_iterator iterator;
  typedef typename rep_type::const_iterator const_iterator;
  typedef typename rep_type::const_reverse_iterator reverse_iterator;
  typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
  typedef typename rep_type::size_type size_type;
  typedef typename rep_type::difference_type difference_type;

  // allocation/deallocation

  set() : t(Compare()) {}
  //为什么是 eplict ?
  explicit set(const Compare& comp) : t(comp) {}

#ifdef __STL_MEMBER_TEMPLATES
  template <class InputIterator>
  set(InputIterator first, InputIterator last)
    : t(Compare()) { t.insert_unique(first, last); }

  template <class InputIterator>
  set(InputIterator first, InputIterator last, const Compare& comp)
    : t(comp) { t.insert_unique(first, last); }
#else
  //使用 insert_unique() 而非 insert_equal()
  set(const value_type* first, const value_type* last) 
    : t(Compare()) { t.insert_unique(first, last); }
  set(const value_type* first, const value_type* last, const Compare& comp)
    : t(comp) { t.insert_unique(first, last); }

  set(const_iterator first, const_iterator last)
    : t(Compare()) { t.insert_unique(first, last); }
  set(const_iterator first, const_iterator last, const Compare& comp)
    : t(comp) { t.insert_unique(first, last); }
#endif /* __STL_MEMBER_TEMPLATES */
  //copy 构造符
  set(const set<Key, Compare, Alloc>& x) : t(x.t) {}
  //copy assignment 操作符，未处理自我赋值的情况，但 返回了 * this
  set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) { 
    t = x.t; 
    return *this;
  }

  // accessors:

  key_compare key_comp() const { return t.key_comp(); }
  value_compare value_comp() const { return t.key_comp(); }
  iterator begin() const { return t.begin(); }
  iterator end() const { return t.end(); }
  reverse_iterator rbegin() const { return t.rbegin(); } 
  reverse_iterator rend() const { return t.rend(); }
  bool empty() const { return t.empty(); }
  size_type size() const { return t.size(); }
  size_type max_size() const { return t.max_size(); }
  void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }

  // insert/erase
  //通过调用 红黑树Insert_unique()实现,而multi_set通过调用insert_equal()实现
  typedef  pair<iterator, bool> pair_iterator_bool; 
  pair<iterator,bool> insert(const value_type& x) { 
    pair<typename rep_type::iterator, bool> p = t.insert_unique(x); 
    return pair<iterator, bool>(p.first, p.second);
  }
  iterator insert(iterator position, const value_type& x) {
    typedef typename rep_type::iterator rep_iterator;
    return t.insert_unique((rep_iterator&)position, x);
  }
#ifdef __STL_MEMBER_TEMPLATES
  template <class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    t.insert_unique(first, last);
  }
#else
  void insert(const_iterator first, const_iterator last) {
    t.insert_unique(first, last);
  }
  void insert(const value_type* first, const value_type* last) {
    t.insert_unique(first, last);
  }
#endif /* __STL_MEMBER_TEMPLATES */
  void erase(iterator position) { 
    typedef typename rep_type::iterator rep_iterator;
    t.erase((rep_iterator&)position); 
  }
  size_type erase(const key_type& x) { 
    return t.erase(x); 
  }
  void erase(iterator first, iterator last) { 
    typedef typename rep_type::iterator rep_iterator;
    t.erase((rep_iterator&)first, (rep_iterator&)last); 
  }
  void clear() { t.clear(); }

  // set operations:

  iterator find(const key_type& x) const { return t.find(x); }
  size_type count(const key_type& x) const { return t.count(x); }
  iterator lower_bound(const key_type& x) const {
    return t.lower_bound(x);
  }
  iterator upper_bound(const key_type& x) const {
    return t.upper_bound(x); 
  }
  pair<iterator,iterator> equal_range(const key_type& x) const {
    return t.equal_range(x);
  }
  friend bool operator== __STL_NULL_TMPL_ARGS (const set&, const set&);
  friend bool operator< __STL_NULL_TMPL_ARGS (const set&, const set&);
};

template <class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& x, 
                       const set<Key, Compare, Alloc>& y) {
  return x.t == y.t;
}

template <class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& x, 
                      const set<Key, Compare, Alloc>& y) {
  return x.t < y.t;
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class Key, class Compare, class Alloc>
inline void swap(set<Key, Compare, Alloc>& x, 
                 set<Key, Compare, Alloc>& y) {
  x.swap(y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1174
#endif

__STL_END_NAMESPACE

#endif /* __SGI_STL_INTERNAL_SET_H */

// Local Variables:
// mode:C++
// End:
