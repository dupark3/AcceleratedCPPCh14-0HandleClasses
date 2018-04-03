#ifndef GUARD_Ref_handle_h
#define GUARD_Ref_handle_h

#include <cstddef> // size_t
#include <stdexcept> // runtime_error

template <class T>
class Ref_handle{
public:
    Ref_handle() : p(0), refptr(new std::size_t(1)) { }
    Ref_handle(T* t) : p(t), refptr(new std::size_t(1)) { }
    Ref_handle(const Ref_handle& h) : p(h.p), refptr(h.refptr) { ++*refptr; }
    Ref_handle& operator=(const Ref_handle&);
    ~Ref_handle();

    operator bool() const { return p; }
    T& operator*() const {
        if(p) return *p;
        throw std::runtime_error("unbound Ref_handle");
    }
    T* operator->() const {
        if(p) return p;
        throw std::runtime_error("unbound Ref_handle");
    }

private:
    T* p;
    std::size_t* refptr; // points to a size_t that stores how many Ref_handle objects point to the same T object
};

template <class T>
Ref_handle<T>& Ref_handle<T>::operator=(const Ref_handle& rhs){
    ++*rhs.refptr;
    if (--*refptr == 0){
        delete refptr;
        delete p;
    }
    p = rhs.p;
    refptr = rhs.refptr;
    return *this;
}

template <class T>
Ref_handle<T>::~Ref_handle<T>(){
    if (--*refptr == 0){
        delete refptr;
        delete p;
    }
}

#endif // GUARD_Ref_handle_h
