# 迭代器
迭代器 STL 的设计关键所在
STL 的中心思想在于: 将数据容器和算法分开,彼此独立设计,最后再将两者粘连在一起,而如何将两者粘连在一起,便是 iterator 所要完成的事情.

## 迭代器是一种智能指针

## Traits 
我们无法通过参数推导得到我们需要的信息,所以通过内嵌类型的声明是一种好的方法.
```c++
template <class T>
struct MyIter {
    typedef T value_type; //内嵌类型声明
    //...
};
template <typename I>
typename I::value_type/*得到参数对应的类型*/ func( \I ite){
    return * ite;
}
```
但是着还不足以解决所有的问题:并不是所有的迭代器都是 class type. 在 STL 中有时迭代器的本质其实是用 raw 指针实现的.这时我们需要使用偏特化来实现它
```c++
template <typename T>
struct MyIter<T*> {
    //....
}
```
我们再声明一个 "萃取器"来规范一下接口.
```c++
template <typename T>
struct iterator_traits {
    typedef typename I::value_type value_type;
};
```
现在我们可以用以下的方式来重写上面的 func 函数
```c++
template <typename I>
typename iterator_traits<I>::value_type/*得到参数对应的类型*/ func( \I ite){
    return * ite;
}
```
同样,新接口还可以拥有偏特化版本
```c++
template <typename T>
struct iterator_traits <T*> {
    typedef T  value_type;
};
```
> 综上所述,我们只要在迭代器中声明内嵌类型,然后通过萃取器就能够得到我们所需要的信息.

## 迭代器中的类型
根据经验,有五种类型经常会被用到
```c++
template <typename I>
struct iterator_traits {
    //迭代器类型
    typedef typename I::iterator_category iterator_category;
    //对象类型 
    typedef typename I::value_type value_type;
    //两个迭代器之间的距离
    typedef typename I::difference_type difference_type;
    //地址
    typedef typename I::pointer pointer;
    //reference 类型
    typedef typename I::reference reference;
}
```

## 迭代器的类型
![iterator_hypotaxis.png](/image/iterator_hypotaxis.png)
通过迭代器标签来实现的,以 class (struct) 实现,促进重载机制的成功运行. 通过继承关系实现的,我们可以不必再写 "单纯只做传递调用" 的函数.
