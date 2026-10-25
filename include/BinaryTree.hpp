#pragma once
#include "base/Sequence.hpp"
#include "base/MutableArraySequence.hpp"
#include "base/Exceptions.hpp"
#include "Pair.hpp"
#include <concepts>

template <typename T>
concept Comparable = requires(T a, T b) {
    { a < b } -> std::convertible_to<bool>;
};

template <Comparable TKey, typename TValue>
class BinaryTree {
private:
    
    struct Node {
        Pair<TKey, TValue> data;
        Node* left;
        Node* right;
        Node(const Pair<TKey, TValue>& val) : data(val), left(nullptr), right(nullptr) {}
    };
    
    Node* root_;
    size_t size_;
    
    void InsertRecursive(Node*& node, const Pair<TKey, TValue>& value);
    void DestroyRecursive(Node* node);
    void GetRangeRecursive(Node* node, const Pair<TKey, TValue>& min, const Pair<TKey, TValue>& max, 
                          MutableArraySequence<Pair<TKey, TValue>>* result) const;
    
public:
    BinaryTree();
    ~BinaryTree();
    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;
    
    void Insert(const Pair<TKey, TValue>& value);
    size_t GetSize() const;
    bool IsEmpty() const;
    Sequence<Pair<TKey, TValue>>* GetRange(const Pair<TKey, TValue>& min, const Pair<TKey, TValue>& max) const;
    
    class Iterator {
    private:
        MutableArraySequence<Node*> stack_;
        void pushLeftBranch(Node* node);
    public:
        Iterator();
        Iterator(Node* root);
        Pair<TKey, TValue> operator*() const;
        Iterator& operator++();
        bool operator!=(const Iterator& other) const;
        bool operator==(const Iterator& other) const;
    };
    
    Iterator begin() const;
    Iterator end() const;
};

#include "BinaryTree.tpp"