#pragma once

#include <vector>
#include <memory>
#include "Util.hpp"

#define ic inline constexpr

template<typename T>
class Array {
private:
    struct array_data {
        T *start, *end;
	    ic array_data(T* _data, size_t _size) : start(_data), end(start + _size) {}
        ic array_data(size_t _size) : start(new T[_size]), end(start + _size) {}
        ic array_data() : start(nullptr), end(nullptr) {}
        inline ~array_data(){ delete[] start; }
    };

public:
	ic Array(T* _data, size_t __size, bool copy = true) {
        if (__size <= 0) return;
		if (copy) {
            _arr = std::make_shared<array_data>(__size);
            std::copy(_data, _data + __size, _arr->start);
		} else {
            _arr = std::make_shared<array_data>(_data, __size);
		}
	}

    ic Array(size_t _size) {
        if (_size <= 0) return;
		_arr = std::make_shared<array_data>(_size);
    }

    ic Array(size_t _size, T defVal) {
        if (_size <= 0) return;
		_arr = std::make_shared<array_data>(_size);
        std::fill(begin(), end(), defVal);
    }

    ic Array(std::initializer_list<T> l) {
        if (l.size() <= 0) return;
		_arr = std::make_shared<array_data>(l.size());
		std::move(l.begin(), l.end(), _arr->start);
	}

    template<typename C> ic Array(const C& c) {
        _arr = std::make_shared<array_data>(c.size());
		std::copy(c.begin(), c.end(), _arr->start);
    }

    ic Array(){}
    
    ic T* begin(){ return _arr->start; }
    ic T* end(){ return _arr->end; }
    ic T* begin() const { return _arr->start; }
    ic T* end() const { return _arr->end; }

    ic T& operator[](const size_t& idx){ return _arr->start[idx]; }
	ic T& operator[](const size_t& idx) const { return _arr->start[idx]; }
	
	ic T* data() { return _arr->start; }
	ic T* data() const { return _arr->start; }
	ic size_t size() const { return std::distance(begin(), end()); }
    ic Array<T> clone() const { return Array<T>(_arr->start, size(), true); }
    ic std::vector<T> toStlVector() { return std::vector<T>(begin(), end()); }

    ic bool contains(T e){ return std::find(begin(), end(), e) != end(); }
    ic int indexOf(T e){
        auto&& found = std::find(begin(), end(), e);
        if (found == end()) return -1;
        return std::distance(begin(), found);
    }

    ic void copyTo(Array<T>& arr){
        arr._arr.reset(new array_data(size()));
        std::copy(begin(), end(), arr._arr->start);
    }

    ic void resize(size_t newSize, bool retainData = true){
        if (newSize == size()) return;
        T* tmp = new T[newSize];
        if (retainData)
            if (_arr.unique())
                std::move(begin(), begin() + std::min(newSize, size()), tmp);
            else
                std::copy(begin(), begin() + std::min(newSize, size()), tmp);
        _arr.reset(new array_data(tmp, newSize));
    }

    template<typename F> ic void foreach(const F& f){
        for (T* i = begin(); i != end(); i++) f(*i);
    }

private:
    std::shared_ptr<array_data> _arr;
};

template<typename T>
class ArrayView {
private:
    struct array_data {
        T *start, *end;
	    ic array_data(T* _data, size_t _size) : start(_data), end(start + _size) {}
        ic array_data() : start(nullptr), end(nullptr) {}
    };
public:
    template<size_t N> ic ArrayView(T (&arr)[N]) {
        _arr = array_data(arr, N);
	}

    template<typename C> ic ArrayView(C& arr){
        _arr = array_data(arr.data(), arr.size());
    }

	ic ArrayView(T* _data, size_t __size) {
        if (__size <= 0) return;
        _arr = array_data(_data, __size);
	}

    ic ArrayView(){}
    
    ic T* begin(){ return _arr.start; }
    ic T* end(){ return _arr.end; }
    ic T* begin() const { return _arr.start; }
    ic T* end() const { return _arr.end; }

    ic T& operator[](const size_t& idx){ return _arr.start[idx]; }
	ic T& operator[](const size_t& idx) const { return _arr.start[idx]; }
	
	ic T* data() { return _arr.start; }
	ic T* data() const { return _arr.start; }
	ic size_t size() const { return std::distance(begin(), end()); }
    ic Array<T> toArray() const { return Array<T>(_arr.start, size(), true); }
    ic std::vector<T> toStlVector() { return std::vector<T>(begin(), end()); }

    ic bool contains(T e){ return std::find(begin(), end(), e) != end(); }
    ic int indexOf(T e){
        auto&& found = std::find(begin(), end(), e);
        if (found == end()) return -1;
        return std::distance(begin(), found);
    }

    template<typename F> ic void foreach(const F& f){
        for (T* i = begin(); i != end(); i++) f(*i);
    }

private:
    array_data _arr;
};

template<typename CT>
class ArrayConstView {
private:
    using T = const CT;
    struct array_data {
        T *start, *end;
	    ic array_data(T* _data, size_t _size) : start(_data), end(start + _size) {}
        ic array_data() : start(nullptr), end(nullptr) {}
    };

public:

    template<size_t N> ic ArrayConstView(T (&arr)[N]) {
        _arr = array_data(arr, N);
	}

    template<typename C> ic ArrayConstView(const C& arr){
        _arr = array_data(arr.data(), arr.size());
    }

    ic ArrayConstView(std::initializer_list<T> l){
        _arr = array_data(l.begin(), l.size());
    }

	ic ArrayConstView(T* _data, size_t __size) {
        if (__size <= 0) return;
        _arr = array_data(_data, __size);
	}

    ic ArrayConstView(){}
    
    ic T* begin(){ return _arr.start; }
    ic T* end(){ return _arr.end; }
    ic T* begin() const { return _arr.start; }
    ic T* end() const { return _arr.end; }

	ic T& operator[](const size_t& idx){ return _arr.start[idx]; }
	ic T& operator[](const size_t& idx) const { return _arr.start[idx]; }
	
	ic T* data() { return _arr.start; }
	ic T* data() const { return _arr.start; }
	ic size_t size() const { return std::distance(begin(), end()); }
    ic Array<T> toArray() const { return Array<T>(_arr.start, size(), true); }
    ic std::vector<T> toStlVector() { return std::vector<T>(begin(), end()); }

    ic bool contains(T e){ return std::find(begin(), end(), e) != end(); }
    ic int indexOf(T e){
        auto&& found = std::find(begin(), end(), e);
        if (found == end()) return -1;
        return std::distance(begin(), found);
    }

    template<typename F> ic void foreach(const F& f){
        for (T* i = begin(); i != end(); i++) f(*i);
    }

private:
    array_data _arr;
};

#undef ic