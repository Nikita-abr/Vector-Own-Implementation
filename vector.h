#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <span>
#include <stdexcept>
#include <utility>

class Vector {
   public:
    Vector()
        : data_(new int[0]), size_(0), capacity_(0) {  // NOLINT(cppcoreguidelines-owning-memory)
    }

    explicit Vector(size_t size) : size_(size), capacity_(size) {
        data_ = new int[size_];  // NOLINT(cppcoreguidelines-owning-memory)
        for (size_t i = 0; i < size; ++i) {
            std::span(data_, size_)[i] = 0;
        }
    }

    Vector(std::initializer_list<int> a) : size_(a.size()), capacity_(a.size()) {
        data_ = new int[size_];  // NOLINT(cppcoreguidelines-owning-memory)
        size_t i = 0;
        for (auto el : a) {
            std::span(data_, size_)[i] = el;
            ++i;
        }
    }

    Vector(Vector& other) : size_(other.size_), capacity_(other.capacity_) {
        data_ = new int[capacity_];  // NOLINT(cppcoreguidelines-owning-memory)
        for (size_t i = 0; i < other.size_; ++i) {
            std::span(data_, size_)[i] = std::span(other.data_, other.size_)[i];
        }
    }

    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }

        delete[] data_;

        data_ = new int[other.capacity_];  // NOLINT(cppcoreguidelines-owning-memory)
        size_ = other.size_;
        capacity_ = other.capacity_;
        for (size_t i = 0; i < size_; ++i) {
            std::span(data_, size_)[i] = std::span(other.data_, other.size_)[i];
        }

        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    ~Vector() {
        delete[] data_;
    }

    Vector& Swap(Vector& other) {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        return *this;
    }

    const int& operator[](size_t i) const {
        return std::span(data_, size_)[i];
    }

    int& operator[](size_t i) {
        return std::span(data_, size_)[i];
    }

    [[nodiscard]] size_t Size() const {
        return size_;
    }

    [[nodiscard]] size_t Capacity() const {
        return capacity_;
    }

    void PushBack(int x) {
        if (size_ < capacity_) {
            std::span(data_, size_)[size_] = x;
            ++size_;
        } else {
            capacity_ *= 2;
            if (capacity_ == 0) {
                ++capacity_;
            }
            int* a = new int[capacity_];  // NOLINT(cppcoreguidelines-owning-memory)
            for (size_t i = 0; i < size_; ++i) {
                std::span(a, size_)[i] = std::span(data_, size_)[i];
                ;
            }
            std::span(a, size_)[size_] = x;
            ++size_;

            delete[] data_;
            data_ = a;
            a = nullptr;
        }
    }

    void PopBack() {
        if (size_ == 0) {
            throw std::out_of_range("PopBack in 0 elements");
        }
        --size_;
    }

    void Clear() {
        size_ = 0;
    }

    void Reserve(size_t capacity) {
        if (capacity > capacity_) {
            int* a = new int[capacity];  // NOLINT(cppcoreguidelines-owning-memory)
            for (size_t i = 0; i < capacity_; ++i) {
                std::span(a, size_)[i] = std::span(data_, size_)[i];
            }
            delete[] data_;
            data_ = a;
            capacity_ = capacity;
        }
    }

    class Iterator {
       public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = int;
        using difference_type = ptrdiff_t;
        using pointer = int*;
        using reference = int&;

        Iterator() : ptr_(nullptr) {
        }

        explicit Iterator(pointer ptr) : ptr_(ptr) {
        }

        reference operator*() const {
            return *ptr_;
        }

        Iterator& operator+=(difference_type n) {
            ptr_ += n;
            return *this;
        }

        Iterator& operator-=(difference_type n) {
            ptr_ -= n;
            return *this;
        }

        Iterator operator+(difference_type n) const {
            Iterator temp = *this;
            temp.ptr_ += n;
            return temp;
        }

        friend Iterator operator+(difference_type n, const Iterator& it) {
            return it + n;
        }

        Iterator operator-(difference_type n) const {
            Iterator temp = *this;
            return temp -= n;
        }

        difference_type operator-(const Iterator& other) const {
            return ptr_ - other.ptr_;
        }

        Iterator& operator++() {
            ptr_++;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ptr_++;
            return temp;
        }

        Iterator& operator--() {
            ptr_--;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            ptr_--;
            return temp;
        }

        reference operator[](difference_type i) {
            return *(ptr_ + i);
        }

        reference operator[](difference_type i) const {
            return *(ptr_ + i);
        }

        pointer operator->() const {
            return ptr_;
        }

        bool operator==(const Iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const Iterator& other) const {
            return ptr_ != other.ptr_;
        }

        bool operator<(const Iterator& other) const {
            return ptr_ < other.ptr_;
        }

        bool operator<=(const Iterator& other) const {
            return ptr_ <= other.ptr_;
        }

        bool operator>(const Iterator& other) const {
            return ptr_ > other.ptr_;
        }

        bool operator>=(const Iterator& other) const {
            return ptr_ >= other.ptr_;
        }

       private:
        pointer ptr_;
    };

    [[nodiscard]] auto begin() const {
        return Iterator(data_);
    }

    [[nodiscard]] auto end() const {
        return Iterator(&std::span(data_, size_)[size_]);
    }

   private:
    int* data_;
    size_t size_, capacity_;
};
