## STL 源码解析

* g++  文件夹包含 Cygnus C++ 2.91 for windows 的源代码

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

## 学习路线
推荐先 看 STL 源码侯捷的视频[YOUTUBE](https://www.youtube.com/watch?v=Edcwv38c7d4&list=PLTcwR9j5y6W2Bf4S-qi0HBQlHXQVFoJrP) , 再对 STL 有了一定的了解的基础上,再结合书籍阅读源码.

## STL 学习的三种境界
 - 运用 
     [STL STLC++标准库(第2版)——自修教程与参考手册]
 - 了解泛型技术内涵和 STL 实现方式 
     STL源码剖析简体中文完整版(清晰扫描带目录)
 - 扩充 STL
     ing....
