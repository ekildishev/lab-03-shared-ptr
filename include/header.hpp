// Copyright 2018 Your Name <your_email>
#pragma once
#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#endif // INCLUDE_HEADER_HPP_

#include <iostream>
#include <cstddef>
#include <atomic>

template <typename T>
class SharedPtr{
    std::atomic_size_t *refCounter = nullptr;
    T* elemPointer = nullptr;
public:
    SharedPtr()
    {
        refCounter = new std::atomic_size_t(1);
    }
    SharedPtr(T* ptr)
    {
        elemPointer = ptr;
        refCounter = new std::atomic_size_t(1);
    }
    SharedPtr(const SharedPtr& r)
    {
        *this = r;
    }
    SharedPtr(SharedPtr&& r)
    {
        *this = std::move(r);
    }
    ~SharedPtr()
    {
        if (!refCounter)
        {
            return;
        }
        if(--(*refCounter) == 0)
        {
            delete refCounter;
            delete elemPointer;
        }
    }
    auto operator=(const SharedPtr& r) -> SharedPtr&
    {
        if(this == &r)
        {
            return *this;
        }
        this->~SharedPtr();
        elemPointer = r.elemPointer;
        refCounter = r.refCounter;
        (*refCounter)++;
        return *this;
    }
    auto operator=(SharedPtr&& r) noexcept -> SharedPtr&
    {
        if(this == &r)
        {
            return *this;
        }
        this->~SharedPtr();
        elemPointer = r.elemPointer;
        refCounter = r.refCounter;
        r.refCounter = nullptr;
        r.elemPointer = nullptr;
        (*refCounter)++;
        return *this;
    }

    // проверяет, указывает ли указатель на объект
    operator bool() const
    {
        if(elemPointer != nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    auto operator*() const -> T&
    {
        return *elemPointer;
    }
    auto operator->() const -> T*
    {
        return elemPointer;
    }

    auto get() -> T*
    {
        return elemPointer;
    }
    void reset()
    {
        *this = SharedPtr();
    }
    void reset(T* ptr)
    {
        *this = SharedPtr(ptr);
    }
    void swap(SharedPtr& r)
    {
        std::swap(refCounter, r.refCounter);
        std::swap(elemPointer, r.elemPointer);
    }
    // возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект
    auto use_count() const -> size_t
    {
        return *refCounter;
    };
};

