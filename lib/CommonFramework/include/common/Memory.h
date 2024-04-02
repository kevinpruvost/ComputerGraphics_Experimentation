#pragma once

#include <memory>

template <typename T>
class UPtr : public std::unique_ptr<T>
{
public:
    UPtr(T * ptr) : std::unique_ptr<T>(ptr) {}
    using std::unique_ptr<T>::unique_ptr;
    using std::unique_ptr<T>::operator=;
    using std::unique_ptr<T>::operator*;
};