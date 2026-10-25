#pragma once
#include "base/Sequence.hpp"
#include "base/MutableArraySequence.hpp"
#include "base/Exceptions.hpp"
#include <fstream>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <type_traits>

template <typename T>
concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

template <TriviallyCopyable T>
class FileStorage {
private:
    std::string filename_;

public:
    FileStorage(const std::string& filename);
    ~FileStorage();

    size_t Write(const T& object);
    T Read(size_t offset) const;
    Sequence<T>* ReadAll() const;
    void Clear();
    size_t GetRecordCount() const;
};

#include "FileStorage.tpp"