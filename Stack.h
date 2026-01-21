#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Stack {
  T* m_Data;
  size_t m_Capacity; 
  size_t m_Size;

  void grow() {
    m_Capacity = 2*m_Capacity;
    T* old_data = m_Data;
    m_Data = static_cast<T*>(::operator new(m_Capacity*sizeof(T))); 

    for(size_t ndx = 0; ndx < m_Size; ++ndx) {
      new(&m_Data[ndx]) T(std::move(old_data[ndx]));
      old_data[ndx].~T();
    }
    ::operator delete(old_data);
  }

  void shrink() {
    m_Capacity = m_Capacity/2;
    T* old_data = m_Data;
    m_Data = static_cast<T*>(::operator new(m_Capacity*sizeof(T))); 

    for(size_t ndx = 0; ndx < m_Size; ++ndx) {
      new(&m_Data[ndx]) T(std::move(old_data[ndx]));
      old_data[ndx].~T();
    }
    ::operator delete(old_data);
  }

  public:
  Stack<T>(size_t capacity = 10) : 
    m_Data(static_cast<T*>(::operator new(capacity*sizeof(T)))), 
    m_Capacity(capacity), 
    m_Size(0) 
  {
  }
  
  ~Stack<T>() {
    for(size_t ndx = 0; ndx < m_Size; ++ndx)
    {
      m_Data[ndx].~T();
    }
    ::operator delete(m_Data);
  }

  Stack<T>(const Stack<T>& rhs) : 
    m_Data(static_cast<T*>(::operator new(rhs.m_Capacity*sizeof(T)))), 
    m_Capacity(rhs.m_Capacity), 
    m_Size(rhs.m_Size) 
  {
    for(size_t ndx = 0; ndx < m_Size; ++ndx) {
      new(&m_Data[ndx]) T(rhs.m_Data[ndx]);
    }
  }
  
  Stack<T>& operator=(Stack<T> rhs) noexcept { 
    std::swap(m_Capacity, rhs.m_Capacity);
    std::swap(m_Size, rhs.m_Size);
    std::swap(m_Data, rhs.m_Data);

    return *this;
  }

  Stack<T>(Stack<T>&& rhs) noexcept :
    m_Data(rhs.m_Data), 
    m_Capacity(rhs.m_Capacity), 
    m_Size(rhs.m_Size) 
  {
    rhs.m_Data = nullptr;
    rhs.m_Capacity = 0;
    rhs.m_Size = 0;
  }

  size_t size() const noexcept {
    return m_Size;
  }

  bool isEmpty() const noexcept {
    return m_Size == 0;
  }

  explicit operator bool() const {
    return m_Size != 0;
  }

  const T& top() const {
    if(m_Size == 0) { throw std::out_of_range("Used top on empty stack!\n"); }
    return m_Data[m_Size-1]; 
  }

  T& top() {
    if(m_Size == 0) { throw std::out_of_range("Used top on empty stack!\n"); }
    return m_Data[m_Size-1];
  }

  void pop() {
    if(m_Size == 0) { throw std::out_of_range("Used pop on empty stack!\n"); }
    --m_Size;
    m_Data[m_Size].~T();
    if (m_Size < m_Capacity/4) {shrink();}
  }

  void push_back(const T& item) {
    if (m_Size >=  m_Capacity) { grow(); }
    new(&m_Data[m_Size]) T(item);
    ++m_Size;
  }

  void push_back(T&& item) {
    if (m_Size >= m_Capacity) { grow(); }
    new(&m_Data[m_Size]) T(std::move(item));
    ++m_Size;
  }

  template<typename... Args>
  void emplace_back(Args&&... args) {
      if (m_Size >= m_Capacity) { grow(); }
      new (&m_Data[m_Size]) T(std::forward<Args>(args)...);
      ++m_Size;
  }

};
