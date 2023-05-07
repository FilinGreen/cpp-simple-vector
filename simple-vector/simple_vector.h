#pragma once

#include <cassert>
#include <initializer_list>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <algorithm>
#include <utility>

#include "array_ptr.h"

class ReserveProxyObj {
    public:
           ReserveProxyObj(size_t  value):value_(value){}
    
           size_t Get(){
               return value_;
           }
    private:
    size_t value_=0;
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    SimpleVector(SimpleVector&& vec){
    swap(vec);
    }
    
    SimpleVector& operator=(SimpleVector&& vec){
    if(*this!=vec){
    SimpleVector <Type> buf(vec);
    swap(buf);}
    return *this;
    }
    
  

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size):size_(size),capacity_(size),vec_(size){
       std::generate(begin(),begin()+size,[](){return Type{};});
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value):size_(size),capacity_(size){
       if(size>0){
       ArrayPtr<Type> buf (size);
       std::generate(buf.Get(),buf.Get()+size,[value](){return value;});
       vec_.swap(buf);
       
       }
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init):size_(init.size()),capacity_(init.size()),vec_(init.size()) {
        std::copy(init.begin(),init.end(),begin());    
    }
    
    SimpleVector (ReserveProxyObj value){
    Reserve(value.Get());
    
    }
    
     SimpleVector(const SimpleVector& other) {
        Resize(other.size_);
        std::copy(other.begin(), other.end(), begin());
        size_ = other.size_;
        capacity_ = other.capacity_;       
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
    if(*this!=rhs){
    SimpleVector <Type> buf(rhs);
    swap(buf);}
    return *this;
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        // Напишите тело самостоятельно
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_==0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return vec_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return vec_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if(index>=size_){throw std::out_of_range("");}
        return vec_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if(index>=size_){throw std::out_of_range("");}
        return vec_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_=0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
    
        if(new_size>capacity_){
       
             Reserve(std::max(new_size,capacity_*2));
             std::generate(vec_.Get()+size_,vec_.Get()+new_size,[](){return Type{};});
        
        }else if(size_<new_size){
            std::generate(vec_.Get()+size_,vec_.Get()+new_size,[](){return Type{};});   
        }
        
             size_=new_size;
        
       }
    

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return vec_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return vec_.Get()+size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return vec_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return vec_.Get()+size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return vec_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return vec_.Get()+size_;
    }
    
   

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
   void PushBack(const Type& item) { 
        if(size_==capacity_){ 
        
        Reserve(std::max(capacity_*2,size_t {1})); 
        vec_[size_]=item;   
        capacity_=std::max(capacity_*2,size_t {1});
        
        }else{ 
        vec_[size_]=item; 
        } 
        ++size_;
    } 
     
    void PushBack(Type&& item){ 
     if(size_==capacity_){ 
        Reserve(std::max(capacity_*2,size_t {1}));
        vec_[size_]=std::move(item);  
        }else{ 
        vec_[size_]=std::move(item); 
        } 
        ++size_;
    } 

    
  
    
    

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        size_t index=pos-begin();

        if(size_==capacity_){      
          ArrayPtr<Type> new_array(size_+1);
          std::move(begin(),begin()+index,new_array.Get());
          new_array[index]=std::move(value);
          std::move(end(),begin()+index,new_array.Get()+size_+1);
          vec_.swap(new_array); 
          capacity_=std::max(capacity_*2,size_t{1}); 
        }else{
          std::move(end(),begin()+index,end()+1);
          vec_[index]=value;
        }
        ++size_;
        return begin()+index;
    }


Iterator Insert(ConstIterator pos, Type&& value) {
   size_t index=pos-begin();

        if(size_==capacity_){      
          ArrayPtr<Type> new_array(size_+1);
          std::move(begin(),begin()+index,new_array.Get());
          new_array[index]=std::move(value);
          std::move(end(),begin()+index,new_array.Get()+size_+1);
          vec_.swap(new_array); 
          capacity_=std::max(capacity_*2,size_t{1}); 
        }else{
          std::move(end(),begin()+index,end()+1);
          vec_[index]=std::move(value);
        }
        ++size_;
        return begin()+index;
    }
       
    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
      assert(size_>0);
        --size_;
    }


    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
       assert(size_>0);
       size_t index=0;
       index=pos-begin();
       std::move(begin()+index+1,end(),begin()+index);
        --size_;
        return begin()+index;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        vec_.swap(other.vec_);
        std::swap(size_,other.size_);
        std::swap(capacity_,other.capacity_);
    }
    
    
    
   void Reserve(size_t new_capacity){
    
    if(new_capacity>capacity_){
      ArrayPtr<Type> new_array(new_capacity);
      std::move(begin(),end(),new_array.Get());
      vec_.swap(new_array);
      capacity_=new_capacity;
   }
   }
   
   
   private:
    size_t size_=0;
    size_t capacity_=0;
    ArrayPtr<Type> vec_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(),rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    
    return !(std::equal(lhs.begin(), lhs.end(), rhs.begin(),rhs.end()));
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return!(rhs<lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
   return!(lhs<rhs);
}