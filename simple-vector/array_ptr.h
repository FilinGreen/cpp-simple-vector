#pragma once

#include <cassert>
#include <cstdlib>

template <typename Type>
class ArrayPtr {
public:
    
    ArrayPtr() = default;
    ArrayPtr(ArrayPtr&&) = default;
    ArrayPtr& operator=(ArrayPtr&&) = default;
   
    explicit ArrayPtr(size_t size) {
    if(size<1){
    raw_ptr_=nullptr;
    }else{raw_ptr_=new Type[size];}
       
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept {
    if(raw_ptr==nullptr){
    raw_ptr_=nullptr;
    }else{
    raw_ptr_=raw_ptr;}
    }

    ~ArrayPtr() {
        delete[] raw_ptr_;
    }

   

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        auto adres=raw_ptr_;
        raw_ptr_=nullptr;
        return adres;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
        return raw_ptr_[index];
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
         return raw_ptr_[index];
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        if(raw_ptr_!=nullptr){return true;}
        return false;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        return raw_ptr_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
       Type* copy = other.Get();
        other.raw_ptr_ = raw_ptr_;
        raw_ptr_ = copy;
    }

private:
    Type* raw_ptr_ = nullptr;
};