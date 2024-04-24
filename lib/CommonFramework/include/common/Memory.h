#pragma once

#include <memory>
#include <common/Error.h>

template <typename T>
class UPtr : public std::unique_ptr<T>
{
public:
    UPtr(T * ptr) : std::unique_ptr<T>(ptr) {}
    using std::unique_ptr<T>::unique_ptr;
    using std::unique_ptr<T>::operator=;
    using std::unique_ptr<T>::operator*;
};

template <typename T>
class SPtr : public std::shared_ptr<T>
{
public:
    SPtr(T* ptr) : std::shared_ptr<T>(ptr) {}
    using std::shared_ptr<T>::shared_ptr;
    using std::shared_ptr<T>::operator=;
    using std::shared_ptr<T>::operator*;
};