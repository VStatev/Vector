 #ifndef _VECTOR_H_
#define _VECTOR_H_
#include <iostream>
#include <stdexcept>
#include <cstddef>
#include <initializer_list>
#include <utility>

using namespace std;

template<typename T> 
class Vector {
public:
	class ConstIterator;
	class Iterator;
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector::Iterator;
	using const_iterator = Vector::ConstIterator;
private:
	pointer values;
	size_type sz;
	size_type max_sz;
	static constexpr size_type min_sz {5};
public:

	class Iterator {
	 public:
		using value_type = Vector::value_type;
		using reference = Vector::reference;
		using pointer = Vector::pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = std::forward_iterator_tag;
	 private:
		pointer ptr;
	 public:
		Iterator() {ptr = nullptr;}
		Iterator(pointer d) { ptr = d;}
		reference operator*() { return *ptr;}
		pointer operator->()  { return ptr;}
		bool operator==(const const_iterator c) const {
		ConstIterator temp {static_cast<ConstIterator>(*this)};
		return temp == c;
		}
		bool operator!=(const const_iterator c) const {
		ConstIterator temp {static_cast<ConstIterator>(*this)};
		return temp != c;
		}
		iterator& operator++() { 
			ptr++;
			return *this; 
		}
		iterator operator++(int junk){ 
		    iterator old(ptr); 
		    ++ptr; 
		    return old; 
		}
		operator const_iterator() const{
		  ConstIterator tmpr (ptr) ;
			return tmpr;
		}
	};

	class ConstIterator {
	 public:
		using value_type = Vector::value_type;
		using reference = Vector::const_reference;
		using pointer = Vector::const_pointer;
		using difference_type = Vector::difference_type;
		using iterator_category = forward_iterator_tag;
	 private:
		pointer ptr;
	 public:
		ConstIterator() { ptr = nullptr;}
		ConstIterator(pointer d) { ptr = d;}
		reference operator*() const {return *ptr;}
		pointer operator->() const {return ptr;}
		const bool operator==(const const_iterator c) const {return ptr == c.ptr;}
		const bool operator!=(const const_iterator c) const {return ptr != c.ptr;}
		const_iterator& operator++() {
			ptr++;
			return *this;
		}
		const_iterator operator++(int junk) {
		    const_iterator old (ptr);
		    ptr++;
			return old;
		}
		 friend Vector::difference_type operator-(const Vector::ConstIterator& lop, const Vector::ConstIterator& rop) {
        return lop.ptr-rop.ptr;
    }
};
	Vector(){
	sz = 0;
	max_sz = min_sz;
	values = new value_type[max_sz];
	}

	Vector(size_type new_size) {
		if (min_sz > new_size)
			max_sz = min_sz;
		else 
			max_sz = new_size;
		sz = 0;
		values = new value_type[max_sz];
	}

	Vector(initializer_list<value_type> l){
		max_sz = l.size();
		sz = max_sz;
		values = new value_type [max_sz];
		size_type a{0};
		for (auto i:l){
			values[a] = i;
			a++;
		}
	}

	Vector(const Vector& v) {
		max_sz = v.max_sz;
		sz = v.sz;
		values = new value_type[max_sz];
		for (size_type i {0}; i < sz; i++)
			values[i] = v.values[i];
	}

	~Vector(){ delete[] values;}

	Vector operator=(const Vector& v) {
		max_sz = v.max_sz;
        	sz = v.sz;
		values = new value_type[max_sz];
		for (size_type i {0}; i < sz; i++)
			values[i] = v.values[i];
		return *this;
	}

	size_type size() const { return sz;}

	bool empty() const { return !sz;}

	void clear() { sz = 0;}

	void reserve(size_type n) {
		if (max_sz >= n)
			return;
		max_sz = n;
		pointer buf = new value_type[n];
		for (size_type i{0}; i < sz; i++)
			buf[i] = values[i];
		delete[] values;
		values = buf;
	}

	void shrink_to_fit() {
		if (max_sz == sz)
			return;
		max_sz = sz;
	}

	void push_back(value_type d) {
		if (max_sz == sz)
			reserve(max_sz*2);
		values[sz++] = d;
	}

	void pop_back() {
		if(empty())
			throw runtime_error("Vector is empty!");
		sz--;
	}

	iterator begin() {
		return values;
	}

	iterator end() {
		return values+sz;
	}

	const_iterator begin() const {
		return values;
	}

	const_iterator end() const {
		return values+sz;
	}

	iterator insert(const_iterator pos, const_reference val) {
		auto diff = pos-begin();
		if (diff<0 || static_cast<size_type>(diff)>sz)
			throw runtime_error("Iterator out of bounds");
		size_type current{static_cast<size_type>(diff)};
		if (sz>=max_sz)
			reserve(max_sz*2);
		for (size_t i {sz}; i-->current;)
			values[i+1]=values[i];
		values[current]=val;
		++sz;
		return iterator{values+current};
	}

	iterator erase(const_iterator pos) {
		auto diff = pos-begin();
		if (diff<0 || static_cast<size_type>(diff)>=sz)
			throw runtime_error("Iterator out of bounds");
		size_type current{static_cast<size_type>(diff)};
		for (size_type i{current}; i<sz-1; ++i)
			values[i]=values[i+1];
		--sz;
		return iterator{values+current};
	}

	reference operator[](size_type index){
		if(index < 0 || index >= sz)
			throw runtime_error("Out of Bounds!");
		return values[index];
	}

	const_reference operator[](size_type index) const{
		if(index < 0 || index >= sz)
			throw runtime_error("Out of Bounds!");
		return values[index];
	}

	 ostream& print(ostream& o) const{
		o << "[" ;
		for (size_t i {0}; i < sz; i++){
			o << values[i];
			if (i < sz-1)
				o << ", ";
			else
				o << "]";
		}
		return o;
	}

};

	template<typename T>
	ostream& operator<<(ostream& o, const Vector<T> &v) {
	return v.print(o);
	}

#endif
