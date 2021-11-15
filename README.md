## STL 源码解析

* g++  文件夹是 Cygnus C++ 2.91 for windows 的源代码

* Annotation 文件夹 对 g++ 文件夹中的文件进行分类整理且代码部分有我个人所添加的注释,详细组织如下:
    * [空间配置器(allocator)](Annotation/allocator)
    * [迭代器(iterator)](Annotation/iterator)
    * 容器(container) 
        - [关联性容器](Annotation/container/associative_container)
        - [序列性容器](Annotation/container/sequence_container)
        - [其他容器](Annotation/container/other_container)        
    * [算法(algorithm)](Annotation/algorithm)
    * [仿函数(functor)](Annotation/functor)
    <!-- 没提供字符串 functor -->
    * [适配器(adapter)](Annotation/adapter)
## 代码组织形式
- 容器:
    我们以 [vector](/Annotation/container/sequence_container/vector/)容器为例，介绍这个版本的 STL 中容器的代码组织方式: 
    用户在编程时直接包含的是 [vecotr](/Annotation/container/sequence_container/vector/vector) 头文件,而 vector 的具体实现部分实际上是放在 [stl_vector.h](/Annotation/container/sequence_container/vector/stl_vector.h) 中的. 其他的容器也按照这一格式进行着组织。

## 学习路线
推荐先 看 YOUTUBE 上 [STL 源码侯捷的视频](https://www.youtube.com/watch?v=Edcwv38c7d4&list=PLTcwR9j5y6W2Bf4S-qi0HBQlHXQVFoJrP) , 再对 STL 有了一定的了解的基础上, 再结合书籍阅读源码.

## STL 学习的三种境界
 - 运用 
     [STL STLC++标准库(第2版)——自修教程与参考手册]
 - 了解泛型技术内涵和 STL 实现方式 
     STL源码剖析简体中文完整版(清晰扫描带目录)
 - 扩充 STL
     ing....
