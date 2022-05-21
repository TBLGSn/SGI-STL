# 仿函数functor
STL 中的 functor 为 function  adapter 提供支持.
> 
![](/image/STL.png)

## 仿函数与 STL 算法之间的关系
STL 仿函数除了能够搭配 Algorithms 使用之外，还应该能够被 function adapter 修饰.
![](/image/functor_algorithm.png)
# 分类
    - 算术运算
    - 关系运算
    - 逻辑运算
## stl_function 中声明的类型
functor 的相应类型只要用来表现函数参数类型和传回值类别,因此 STL 定义了两个 classes,分别表示 一元仿函数和二元仿函数
```c++
// 一元仿函数
template <class Arg, class Result>
struct unary_function {
    typedef Arg argument_type; //参数类型
    typedef Result result_type;//返回值类型
};
//二元仿函数
template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};
```
> 实现 STL functor 的关键在于必须继承上面两个两个类之一,因为 function adapter 将会对其"提问"(例如,你的返回类型是什么)
现在我们只需要(必须)继承这两者其中一个就可以实现上面的仿函数三大类型了,在 stl_function 文件中声明的 functor 类型都形如:
```c++
template <class T>
class plus : public binary_function<T, T, T>{
    T operator()(const T& x, const T& y) const{return x + y;}
};
```
## 用法
```c++
greater<int> tmp; //生成对象
tmp(3, 5);
greater<int>(3, 5); // 匿名临时对象
sort(vec.begin(), vec.end(), greater<int>()); // 搭配 STL 算法
```

## function adapters
每一个 function adapters 都内置了一个 member object,这样每一个 function adapters
都有一个内置被修饰的对象。我们可以对这个对象进行“加工”，达到 adaption 的功能.
![function_adapter_memberobject.png](/image/function_adapter_memberobject.png)

