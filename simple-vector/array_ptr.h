#pragma once

#include <cassert>
#include <cstdlib>

template <typename Type>
class ArrayPtr {
public:
    
    ArrayPtr() = default;
    ArrayPtr(ArrayPtr&&) = default;
    ArrayPtr& operator=(ArrayPtr&&) = default;
   
    explicit ArrayPtr(size_t size):raw_ptr_(size==0 ? nullptr : new Type[size]) {}

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept :raw_ptr_(raw_ptr==nullptr ? nullptr : raw_ptr) {}   

    ~ArrayPtr() {
        delete[] raw_ptr_;
    }

   

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept { 
        return std::exchange(raw_ptr_,nullptr);
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
       std::swap(raw_ptr_,other.raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};