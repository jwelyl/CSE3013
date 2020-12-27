#include "linkedlist.h"

template <class T>
class Stack : public LinkedList<T> {
public:
	virtual bool Delete(T& element);	//	LinkedList의 Delete 함수를 재정의				
};

template <class T>
bool Stack<T>::Delete(T& element) {
	if(LinkedList<T>::first == 0) return false;
	
	Node<T>* delNode = LinkedList<T>::first;
	element = delNode->data;
	
	LinkedList<T>::first = LinkedList<T>::first->link;
	delete delNode;
	LinkedList<T>::current_size--;
	
	return true;				
}
