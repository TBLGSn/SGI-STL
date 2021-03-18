# Stack

## 简介
    stack实现了一个 LIFO 结构。
    你可以使用 push() 将任意元素放入到stack中，也可以用 pop() 将任意的元素从容器中移除，也可以通过 top() 取得栈顶的元素。
![ss](stack-1.png)

## 实现方式
    Stack的实现是通过内置一个 deque<T>实现的。之所以选择 deque,而不是vector，是因为:deque移除元素时，会释放内，并且，不需要在重分配时复制全部元素。

    通过容器适配器对deque的接口进行改造。

    只要提供back()、push_back()、pop_back(),你就可以使用任意的sequence容器支持 stack。

    没有提供迭代器，所以算法并不能作用于该容器??
## mini stack
```c++
template <typename T, typename Container = deque<T> >
class Stack{
	public:
		typedef typename Container::value_type value_type;
		//...
		bool empty() { return co.empty(); }
		size_t size() { return co.size(); }
		const value_type& top() { return co.back(); }
		void pop() { 
			if( empty() ) 
				cout <<"Stack is EMPTY!!"<<endl;
			else 
				co.pop_back();
		}
		void push(const value_type& x) { co.push_back(x); } 

	private:
		Container co; //内置容器
};
```
