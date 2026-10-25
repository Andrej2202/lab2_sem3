#include "FileStorage.hpp"

template <TriviallyCopyable T>
FileStorage<T>::FileStorage(const std::string& filename) 
    : filename_(filename) {}

template <TriviallyCopyable T>
FileStorage<T>::~FileStorage() {}

template <TriviallyCopyable T>
size_t FileStorage<T>::Write(const T& object) {
    size_t offset = 0;
    {
        std::ifstream sizeFile(filename_, std::ios::binary | std::ios::ate);
        if (sizeFile.is_open()) {
            offset = static_cast<size_t>(sizeFile.tellg());
        }
    }
    
    std::ofstream file(filename_, std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename_);
    }
    
    file.write(reinterpret_cast<const char*>(&object), sizeof(T));
    
    if (!file.good()) {
        throw std::runtime_error("Error writing to file: " + filename_);
    }
    
    return offset;
}

template <TriviallyCopyable T>
T FileStorage<T>::Read(size_t offset) const {
    std::ifstream file(filename_, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename_);
    }
    
    file.seekg(offset);
    T object;
    file.read(reinterpret_cast<char*>(&object), sizeof(T));
    
    if (!file.good()) {
        throw std::runtime_error("Error reading from file: " + filename_);
    }
    
    return object;
}

template <TriviallyCopyable T>
Sequence<T>* FileStorage<T>::ReadAll() const {
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    std::ifstream file(filename_, std::ios::binary);
    if (!file.is_open()) {
        return result;
    }

    T object;
    while (file.read(reinterpret_cast<char*>(&object), sizeof(T))) {
        result->Append(object);
    }
    return result;
}

template <TriviallyCopyable T>
void FileStorage<T>::Clear() {
    std::ofstream file(filename_, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for clearing: " + filename_);
    }
}

template <TriviallyCopyable T>
size_t FileStorage<T>::GetRecordCount() const {
    std::ifstream file(filename_, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return 0;
    }
    
    size_t fileSize = file.tellg();
    return fileSize / sizeof(T);
}