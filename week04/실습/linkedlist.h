#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <iostream>
using namespace std;

//	Linked List Node
template <class T>
class Node {
public:
	T data;
	Node<T>* link;
	Node(T element) {
		data = element;
		link = NULL;				
	}				
};

template <class T>
class LinkedList {
protected:
	Node<T>* first;
	int current_size;
public:
	LinkedList();					
	int GetSize() const;		//	리스트의 노드의 개수를 리턴
	void Insert(T element);	//	맨 앞에 원소를 삽입
	virtual bool Delete(T& element);	//	맨 뒤의 원소를 삭제
	void Print() const;		//	리스트를 출력
};

template <class T>
LinkedList<T>::LinkedList() {
	first = 0;
	current_size = 0;				
}

template <class T>
int LinkedList<T>::GetSize() const {
	return current_size;				
}

template <class T>
void LinkedList<T>::Insert(T element) {
	Node<T>* newNode = new Node<T>(element);
	newNode->link = first;
	first = newNode;
	current_size++;				
}

template <class T>
bool LinkedList<T>::Delete(T& element) {
	if(first == 0)
		return false;
	
	Node<T>* current = first;
	Node<T>* previous = NULL;
	
	while(1) {
		if(current->link == 0) {
			if(previous) previous->link = current->link;
			else first = first->link;
			break;				
		}				
		previous = current;
		current = current->link;
	}				
	
	element = current->data;
	delete current;

	current_size--;

	return true;
}

template <class T>
void LinkedList<T>::Print() const {
	int i = 1;
	Node<T>* current = first;
	if(!current)	//	LinkedList가 비었을 경우
		return;
					 
	for(; current; current = current->link) {
		cout<<"["<<(i++)<<"|"<<current->data<<"]";
		if(current->link) cout<<"->";				
	}
	cout<<endl;
}

#endif
