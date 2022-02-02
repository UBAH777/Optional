#pragma once
#include <exception>

namespace BMSTU {
    class BadOptionalAccess : public std::exception {
    public:
        using std::exception::exception;

        virtual const char *what() const noexcept {
            return "Bad optional access";
        }
    };

    template<typename T>
    class Optional {
    public:
        Optional() = default;

        Optional(const T &value) {
            is_initialized_ = true;
            T *val = new(&data_[0])T{value};
            (void) (&val);
        }

        Optional(T &&value) {
            is_initialized_ = true;
            T *val = new(&data_[0])T{std::move(value)};
            (void) (&val);
        }

        Optional(const BMSTU::Optional<T> &other) {
            if (is_initialized_ && other.is_initialized_) {
                this->value() = other.value();
            } else if (!is_initialized_ && other.is_initialized_) {
                T *val = new(&data_[0])T{other.value()};
                is_initialized_ = true;
                (void) (&val);
            } else if (is_initialized_ && !other.is_initialized_) {
                reset();
            } else if (!is_initialized_ && !other.is_initialized_) {
                (void) (&data_);
            }
        }

        bool has_value() const {
            return is_initialized_;
        };

        void reset() {
            if (is_initialized_) {
                static_cast<T *>((void *) &data_[0])->~T();
                is_initialized_ = false;
            }
        }

        T &value() &{
            if (!is_initialized_) {
                throw BMSTU::BadOptionalAccess();
            }
            return reinterpret_cast<T &>(*(T *) &data_[0]);
        }

        const T &value() const &{
            if (!is_initialized_) {
                throw BMSTU::BadOptionalAccess();
            }
            return reinterpret_cast<T &>(*(T *) &data_[0]);
        }

        Optional &operator=(const T &outer_value) {
            if (is_initialized_) {
                this->value() = outer_value;
            } else {
                T *val = new(&data_[0])T{outer_value};
                is_initialized_ = true;
                (void) (&val);
            }
            return *this;
        }

        Optional &operator=(T &&outer_value) {
            if (is_initialized_) {
                this->value() = std::move(outer_value);
            } else {
                T *val = new(&data_[0])T{outer_value};
                is_initialized_ = true;
                (void) (&val);
            }
            return *this;
        }

        Optional &operator=(const Optional &rhs) {
            if (this == &rhs) {
                return *this;
            }
            if (is_initialized_ && rhs.is_initialized_) {
                this->value() = rhs.value();
            } else if (!is_initialized_ && rhs.is_initialized_) {
                T *val = new(&data_[0])T{rhs.value()};
                is_initialized_ = true;
                (void) (&val);
            } else if (is_initialized_ && !rhs.is_initialized_) {
                reset();
            } else if (!is_initialized_ && !rhs.is_initialized_) {
                return *this;
            }
            return *this;
        }

        Optional &operator=(Optional &&rhs) {
            if (this == &rhs) {
                return *this;
            }
            if (is_initialized_ && rhs.is_initialized_) {
                this->value() = std::move(rhs.value());
            } else if (!is_initialized_ && rhs.is_initialized_) {
                T *val = new(&data_[0])T{std::move(rhs.value())};
                is_initialized_ = true;
                (void) (&val);
            } else if (is_initialized_ && !rhs.is_initialized_) {
                reset();
            } else if (!is_initialized_ && !rhs.is_initialized_) {
                return *this;
            }
            return *this;
        }

        ~Optional() {
            if (is_initialized_) {
                T *ptr = static_cast<T *>((void *) &data_[0]);
                ptr->~T();
            }
        }

        T &operator*() &{
            return *(static_cast<T *>((void *) &data_[0]));
        }

        const T &operator*() const &{
            return *(static_cast<const T *>((void *) &data_[0]));
        }

        T *operator->() {
            return static_cast<T *>((void *) &data_[0]);
        }

        const T *operator->() const {
            return static_cast<const T *>((void *) &data_[0]);
        }
        T &&operator*() &&{
            return std::move(*(static_cast<T *>((void *) &data_[0])));
        }

        T &&value() &&{
            if (!is_initialized_) {
                throw BadOptionalAccess();
            }
            return std::move(reinterpret_cast<T &>(*(T *) &data_[0]));
        }

        friend void swap(BMSTU::Optional<T> &first, BMSTU::Optional<T> &second) {
            std::swap(first.data_, second.data_);
            std::swap(first.is_initialized_, second.is_initialized_);
        }

        template<typename ... Args>
        void Emplace(Args &&... args) {
            if (is_initialized_) {
                reset();
            }
            is_initialized_ = true;
            T *val = new(&data_[0])T(std::forward<Args>(args)...);
            (void) (&val);
        }

    private:
        alignas(T) char data_[sizeof(T)];
        bool is_initialized_ = false;
    };
}