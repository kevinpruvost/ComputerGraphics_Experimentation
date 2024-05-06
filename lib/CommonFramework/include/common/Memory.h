#pragma once

#include <memory>
#include <common/Error.h>

template <typename T>
class Ptr
{
public:
    Ptr(T * ptr = nullptr) : __ptr(ptr) {}
    // Assignment operator
    Ptr& operator=(const Ptr& other) {
        __ptr = other.__ptr;
        return *this;
    }

    // Dereference operator
    T& operator*() const {
        return *__ptr;
    }

    // Member access operator
    T* operator->() const {
        return __ptr;
    }

    // Conversion to bool operator
    explicit operator bool() const {
        return __ptr != nullptr;
    }

    operator T* () const {
        return __ptr;
    }
private:
    T * __ptr;
};

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

// Define specialization of std::hash for SPtr<T>
namespace std {
    template<typename T>
    struct hash<SPtr<T>> {
        std::size_t operator()(const SPtr<T>& sptr) const noexcept {
            return std::hash<T*>{}(sptr.get());
        }
    };
}