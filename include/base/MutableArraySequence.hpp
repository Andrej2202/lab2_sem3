

#pragma once
#include "ArraySequence.hpp"

template<class T>
class MutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;

    ArraySequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
    
    Sequence<T>* Instance() override { 
        return this;
    }

    Sequence<T>* CreateEmpty() const override { 
        return new MutableArraySequence<T>(); 
    }

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override;

    void Set(size_t index, T item) override {
        if (index > this->GetLength()) {
            throw IndexOutOfRangeException("ArraySequence", "InsertAtInternal", "index > size.", this->GetLength(), index);
        }
        this->data_->Set(index, item);
    }

    void RemoveLast();
};

template<class T>
Sequence<T>* MutableArraySequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) const{
    if(endIndex >= this->data_->GetSize()){
        throw IndexOutOfRangeException("ArraySequence", "GetSubList", "size of new container out of range.", this->data_->GetSize(), endIndex);
    }
    if(startIndex > endIndex){
        throw IndexOutOfRangeException("ArraySequence", "GetSubList", "size of new container out of range.", endIndex, startIndex);
    }
    size_t buffer_length = endIndex - startIndex + 1;
    DynamicArray<T> buffer{buffer_length};

    for(size_t i = 0; i < buffer_length; i++){
        buffer.Set(i, this->data_->Get(startIndex + i));
    } 
    return new MutableArraySequence<T>(std::move(buffer));
}

template <typename T>
void MutableArraySequence<T>::RemoveLast() {
    size_t current_size = this->data_->GetSize();
    
    if (current_size == 0) {
        throw InvalidArgumentException("MutableArraySequence", "RemoveLast", 
            "Попытка удалить элемент из пустой последовательности");
    }
    this->data_->Resize(current_size - 1);
}