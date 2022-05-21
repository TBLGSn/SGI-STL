# 配接器
adapter 将原来的接口改变成另一个接口,而不实际改变内部实际的功能
## 分类
    
- container adapter
    queue 和 stack 的本质其实只不过是一种 Adapter.
- iterator adapter
    STL 提供了很多应用于 迭代器上的适配器,例如 insert iterator, reverse iterator, stream iterator. 详见 Annotation/iterator/stl_iterator.h 文件
	- insert iterator
	每一个 insert iterator 内部都维护一个容器,但客户端调用 insert iterator 进行赋值操作时,就转化为调用容器中的方法进行插入操作.而对于其他 iterator 的一些其他接口(如 operator++) 等都被关闭了.
	- reverse iterator
	所谓的 reverse iterator 就是将迭代器的移动行为倒转. 为了配合迭代器“前闭后开”的习惯,“正向迭代器”和“与其对应的反向迭代器”取出的元素并不相同.
	- stream iterator
	所谓的 stream iterators,可以将迭代器迭代器绑定到一个 stream(输入数据流 istream,输出ostream)对象身上。例如,绑定 istream object 对象的过程,其实就是在 istream iterator 内部维护一个istream member,客户端对于这个迭代器所做的 operator ++ 操作会被导引调用迭代器内部包含的那个 istream member 的输入操作 operator >>。
- function adapter
	SGI STL 将这部分的实现定义于 <stl_function.h>
	通过 function adapters 之间的绑定、组合、修饰能力，几乎可以构建出一个各种无限可能的表达式.
	区别于“一般函数”和“成员函数”, STL 为我们提供了众多的适配器,使得“函数”能够不加区分的和其他适配器和算法结合起来.
