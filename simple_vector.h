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
    explicit SimpleVector(size_t size){
    if(size>0){
    ArrayPtr<Type> buf (size);
    std::fill(buf.Get(),buf.Get()+size,value_);
    vec_.swap(buf);
    size_=size;
    capacity_=size;
    }
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value){
       if(size>0){
       value_=value;
       ArrayPtr<Type> buf (size);
       std::fill(buf.Get(),buf.Get()+size,value_);
       vec_.swap(buf);
       size_=size;
       capacity_=size;
       }
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        size_t size=init.size();
        ArrayPtr<Type> buf (size);
        std::fill(buf.Get(),buf.Get()+size,value_);
        vec_.swap(buf);
        size_=size;
        capacity_=size;
        size_t i=0;
        for(const Type& value:init){
        vec_[i]=value;
        ++i;
        }
        
    }
    
    SimpleVector (ReserveProxyObj value){
    Reserve(value.Get());
    
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
             ArrayPtr<Type> new_vec(std::max(new_size,capacity_*2));
             for(size_t i=0;i<size_;++i){
             new_vec[i]=std::move(vec_[i]);      
             }
             vec_.swap(new_vec);
             std::generate(vec_.Get()+size_,vec_.Get()+new_size,[](){return Type{};});
             size_=new_size;
             
             capacity_=std::max(new_size,capacity_*2);
        
        }else if(size_<new_size){
            std::generate(vec_.Get()+size_,vec_.Get()+new_size,[](){return Type{};});   
        }else{
             size_=new_size;
        }
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
    
    SimpleVector(const SimpleVector& other) {
        Resize(other.size_);
        std::copy(other.begin(), other.end(), begin());
        size_ = other.size_;
        capacity_ = other.capacity_;
        
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
    size_t size = rhs.GetSize();
    size_t capacity=rhs.GetCapacity();
        if(size>capacity_){
          Resize(std::max(capacity_*2,size));
          std::copy(rhs.begin(), rhs.end(), begin());
          size_=size;
          capacity_=std::max(capacity_*2,size);
        
        
        }else{
        
          std::copy(rhs.begin(), rhs.end(), begin());
          size_=size;
          capacity_=capacity;
          }
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if(size_==capacity_){
        ArrayPtr<Type> new_array(std::max(capacity_*2,size_t {1}));
        for(size_t i=0;i<size_;++i){
        new_array[i]=std::move(vec_[i]);
        }
        vec_.swap(new_array);
        vec_[size_]=item;
        ++size_;
        capacity_=std::max(capacity_*2,size_t {1});
        
        }else{
        vec_[size_]=item;
        ++size_;
        }
    }
    
    void PushBack(Type&& item){
     if(size_==capacity_){
        ArrayPtr<Type> new_array(std::max(capacity_*2,size_t {1}));
        for(size_t i=0;i<size_;++i){
        new_array[i]=std::move(vec_[i]);
        }
        vec_.swap(new_array);
        vec_[size_]=std::move(item);
        ++size_;
        capacity_=std::max(capacity_*2,size_t {1});
        
        }else{
        vec_[size_]=std::move(item);
        ++size_;
        }
    }
    
  
    
    

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        size_t index=pos-begin();
        
         if(capacity_==0){
          Resize(1);
          vec_[0]=value;
          size_=1;
          capacity_=1;
          
        }else if(size_==capacity_){
          size_t size=size_;
          ArrayPtr<Type> new_array(size_+1);
          for(size_t i=0;i<index;++i){
            new_array[i]=std::move(vec_[i]);
          }
          new_array[index]=value;
          std::copy_backward(begin()+index,end(),new_array.Get()+size_+1);
          vec_.swap(new_array);
          size_=size+1;
          capacity_=capacity_*2;
          
          
        }else{
          std::copy_backward(begin()+index,end(),end()+1);
          vec_[index]=value;
          ++size_;
        }
        return begin()+index;
    }


Iterator Insert(ConstIterator pos, Type&& value) {
        size_t index=pos-begin();
        
         if(capacity_==0){
          Resize(1);
          vec_[0]=std::move(value);
          size_=1;
          capacity_=1;
          
        }else if(size_==capacity_){
          size_t size=size_;
          ArrayPtr<Type> new_array(size_+1);
          for(size_t i=0;i<index;++i){
            new_array[i]=std::move(vec_[i]);
          }
          new_array[index]=std::move(value);
          
          for(auto it=begin()+index;it!=end();++it){
            ++index;
            new_array[index]=std::move(*it);
          }
          
          vec_.swap(new_array);
          size_=size+1;
          capacity_=capacity_*2;
          
          
        }else{
          size_t indexx=index;
          for(auto it=begin()+index;it!=end();++it){
            ++indexx;
            vec_[indexx]=std::move(*it);
          }
          vec_[index]=std::move(value);
          ++size_;
        }
        return begin()+index;
    }
    
   
    
    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if(size_>0){
        --size_;
        }
    }


    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
       size_t index=0;
       if(size_>0){
       index=pos-begin();
       
       std::move(begin()+index+1,end(),begin()+index);
        --size_;
        }
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
   std::copy(begin(),end(),new_array.Get());
   vec_.swap(new_array);
   capacity_=new_capacity;
   }
   }
   
   
   private:
    size_t size_=0;
    size_t capacity_=0;
    ArrayPtr<Type> vec_;
    Type value_=Type();
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
    // Заглушка. Напишите тело самостоятельно
    return(rhs==lhs||lhs<rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
   return(lhs==rhs||lhs>rhs);
}