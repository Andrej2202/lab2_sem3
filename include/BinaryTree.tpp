template <Comparable TKey, typename TValue>
BinaryTree<TKey, TValue>::BinaryTree() : root_(nullptr), size_(0) {}

template <Comparable TKey, typename TValue>
BinaryTree<TKey, TValue>::~BinaryTree() {
    DestroyRecursive(root_);
}

template <Comparable TKey, typename TValue>
void BinaryTree<TKey, TValue>::DestroyRecursive(Node* node) {
    if (node != nullptr) {
        DestroyRecursive(node->left);
        DestroyRecursive(node->right);
        delete node;
    }
}

template <Comparable TKey, typename TValue>
void BinaryTree<TKey, TValue>::InsertRecursive(Node*& node, const Pair<TKey, TValue>& value) {
    if (node == nullptr) {
        node = new Node(value);
        size_++;
    } else if (value.first < node->data.first) { 
        InsertRecursive(node->left, value);
    } else {
        InsertRecursive(node->right, value);
    }
}

template <Comparable TKey, typename TValue>
void BinaryTree<TKey, TValue>::Insert(const Pair<TKey, TValue>& value) {
    InsertRecursive(root_, value);
}

template <Comparable TKey, typename TValue>
size_t BinaryTree<TKey, TValue>::GetSize() const {
    return size_;
}

template <Comparable TKey, typename TValue>
bool BinaryTree<TKey, TValue>::IsEmpty() const {
    return size_ == 0;
}

template <Comparable TKey, typename TValue>
Sequence<Pair<TKey, TValue>>* BinaryTree<TKey, TValue>::GetRange(const Pair<TKey, TValue>& min, const Pair<TKey, TValue>& max) const {
    MutableArraySequence<Pair<TKey, TValue>>* result = new MutableArraySequence<Pair<TKey, TValue>>();
    GetRangeRecursive(root_, min, max, result);
    return result;
}

template <Comparable TKey, typename TValue>
void BinaryTree<TKey, TValue>::GetRangeRecursive(Node* node, const Pair<TKey, TValue>& min, const Pair<TKey, TValue>& max,
                                                  MutableArraySequence<Pair<TKey, TValue>>* result) const {
    if (node == nullptr) return;

    if (node->data.first > min.first) {
        GetRangeRecursive(node->left, min, max, result);
    }

    if (node->data.first >= min.first && node->data.first <= max.first) {
        result->Append(node->data);
    }

    if (node->data.first <= max.first) {
        GetRangeRecursive(node->right, min, max, result);
    }
}

template <Comparable TKey, typename TValue>
void BinaryTree<TKey, TValue>::Iterator::pushLeftBranch(Node* node) {
    while (node != nullptr) {
        stack_.Append(node);
        node = node->left;
    }
}

template <Comparable TKey, typename TValue>
BinaryTree<TKey, TValue>::Iterator::Iterator() {}

template <Comparable TKey, typename TValue>
BinaryTree<TKey, TValue>::Iterator::Iterator(Node* root) {
    pushLeftBranch(root);
}

template <Comparable TKey, typename TValue>
Pair<TKey, TValue> BinaryTree<TKey, TValue>::Iterator::operator*() const {
    if (stack_.GetLength() == 0) {
        throw InvalidArgumentException("BinaryTree::Iterator", "operator*", "Dereferencing end iterator");
    }
    return stack_.GetLast()->data;
}

template <Comparable TKey, typename TValue>
typename BinaryTree<TKey, TValue>::Iterator& BinaryTree<TKey, TValue>::Iterator::operator++() {
    if (stack_.GetLength() == 0) {
        throw InvalidArgumentException("BinaryTree::Iterator", "operator++", "Incrementing end iterator");
    }
    
    Node* current = stack_.GetLast();
    stack_.RemoveLast();
    
    if (current->right != nullptr) {
        pushLeftBranch(current->right);
    }
    
    return *this;
}

template <Comparable TKey, typename TValue>
bool BinaryTree<TKey, TValue>::Iterator::operator!=(const Iterator& other) const {
    return stack_.GetLength() != other.stack_.GetLength();
}

template <Comparable TKey, typename TValue>
bool BinaryTree<TKey, TValue>::Iterator::operator==(const Iterator& other) const {
    return stack_.GetLength() == other.stack_.GetLength();
}

template <Comparable TKey, typename TValue>
typename BinaryTree<TKey, TValue>::Iterator BinaryTree<TKey, TValue>::begin() const {
    return Iterator(root_);
}

template <Comparable TKey, typename TValue>
typename BinaryTree<TKey, TValue>::Iterator BinaryTree<TKey, TValue>::end() const {
    return Iterator();
}