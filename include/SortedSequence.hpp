#pragma once
#include "base/Sequence.hpp"
#include "base/MutableArraySequence.hpp"
#include "base/Exceptions.hpp"
#include <functional>
#include <cmath>

template <typename T>
class SortedSequence : public Sequence<T> {
public:
    using SplitFunc = std::function<size_t(size_t, size_t)>;


    static SplitFunc SplitMid() {
        return [](size_t l, size_t r) { return l + (r - l) / 2; };
    }

    static SplitFunc SplitGolden() {
        return [](size_t l, size_t r) {
            return l + static_cast<size_t>((r - l) * 0.382);
        };
    }

private:
    MutableArraySequence<T>* data_;
    SplitFunc splitFunc_; 

    size_t BinarySearch(const T& item) const;
    size_t FindInsertPosition(const T& item) const;

    static SplitFunc DefaultSplitFunc() {
        return [](size_t l, size_t r) {
            return l + (r - l) / 2;
        };
    }

public:
    SortedSequence(SplitFunc splitFunc = DefaultSplitFunc());
    SortedSequence(const SortedSequence<T>& other);
    ~SortedSequence() override;
    SortedSequence<T>& operator=(const SortedSequence<T>& other);

    void Add(const T& item);
    int IndexOf(const T& item) const;
    Sequence<T>* GetRange(const T& min, const T& max) const;

    void SetSplitFunction(SplitFunc func);

    T GetFirst() const override;
    T GetLast() const override;
    T Get(size_t index) const override;
    size_t GetLength() const override;
    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override;
    Sequence<T>* Append(T item) override;
    Sequence<T>* Prepend(T item) override;
    Sequence<T>* InsertAt(T item, size_t index) override;
    Sequence<T>* Concat(Sequence<T>* list) override;
    void Set(size_t index, T data) override;
    Sequence<T>* CreateEmpty() const override;
    SequenceIterator<T> begin() const override;
    SequenceIterator<T> end() const override;
};

#include "SortedSequence.tpp"