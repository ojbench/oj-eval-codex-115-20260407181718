#pragma once
#include <new>
#include <utility>
// You may not include any other headers

namespace sjtu {

/**
 * @brief A pointer that releases memory when leaving scope.
 * unique_ptr<_Tp> should behave almost like a raw pointer _Tp*.
 */
template <typename _Tp>
class unique_ptr {
 private:
  _Tp *ptr_;

 public:
  // Construct a null pointer
  unique_ptr() : ptr_(nullptr) {}

  // Disable copy
  unique_ptr(const unique_ptr &) = delete;
  unique_ptr &operator=(const unique_ptr &) = delete;

  // Move constructor: transfer ownership and null the source
  unique_ptr(unique_ptr &&other) : ptr_(other.ptr_) { other.ptr_ = nullptr; }

  // Destructor: delete managed object (delete nullptr is safe)
  ~unique_ptr() {
    delete ptr_;
    ptr_ = nullptr;
  }

  // Move assignment: release current, take other's, null other
  unique_ptr &operator=(unique_ptr &&other) {
    if (this != &other) {
      if (ptr_ != other.ptr_) {
        delete ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
      }
    }
    return *this;
  }

  // Take ownership of a raw pointer
  explicit unique_ptr(_Tp *p) : ptr_(p) {}

  // Reset to nullptr, deleting the managed object
  void reset() {
    delete ptr_;
    ptr_ = nullptr;
  }

  // Release ownership and return the raw pointer
  _Tp *release() {
    _Tp *tmp = ptr_;
    ptr_ = nullptr;
    return tmp;
  }

  // Get the raw pointer without transferring ownership
  _Tp *get() const { return ptr_; }

  // Dereference operators
  _Tp &operator*() { return *ptr_; }
  _Tp *operator->() { return ptr_; }
};

// Ensure only one pointer-sized member is stored
static_assert(sizeof(unique_ptr<int>) <= sizeof(void *));

// Create a unique_ptr managing a new copy-constructed object
template <typename _Tp>
unique_ptr<_Tp> make_unique(const _Tp &value) {
  return unique_ptr<_Tp>(new _Tp(value));
}

// Bonus perfect-forwarding overload (optional):
// If you implement the bonus, remove the above make_unique and keep this one instead.
// template <typename _Tp, typename... Args>
// unique_ptr<_Tp> make_unique(Args &&... args) {
//   return unique_ptr<_Tp>(new _Tp(std::forward<Args>(args)...));
// }

} // namespace sjtu

