#include <iostream>
#include <stdexcept>


template <typename T>
class Stack {
  T* m_Data;
  size_t m_Capacity; 
  size_t m_Size;

  void grow() {
    m_Capacity = 2*m_Capacity;
    T* old_data = m_Data;
    m_Data = new T[m_Capacity];

    for(size_t ndx = 0; ndx < m_Size; ++ndx) {
      m_Data[ndx] = old_data[ndx];
    }

    delete[] old_data;
  }

  void shrink() {
    m_Capacity = m_Capacity/2;
    T* old_data = m_Data;
    m_Data = new T[m_Capacity];

    for(size_t ndx = 0; ndx < m_Size; ++ndx) {
      m_Data[ndx] = old_data[ndx];
    }

    delete[] old_data;
  }

  public:
  // regular ass constructor
  Stack<T>(size_t capacity = 10) : m_Data(new T[capacity]), m_Capacity(capacity), m_Size(0) {}
  
  // destructor (no need to set member variables to 0, they're gonna go out of scope.)
  ~Stack<T>() {
    delete[] m_Data;
  }

  // copy constructor
  Stack<T>(const Stack<T>& rhs) : m_Data(new T[rhs.m_Capacity]), m_Capacity(rhs.m_Capacity), m_Size(rhs.m_Size) {
    for(size_t ndx = 0; ndx < m_Size; ++ndx) {
      m_Data[ndx] = rhs.m_Data[ndx];
    }
  }
  
  // assignment operator
  const Stack<T>& operator=(const Stack<T>& rhs) { 
    // check for self assignment
    if (*this == &rhs) { return *this; }

    // make way for new data
    delete[] m_Data;
    m_Data = new T[rhs.m_Capacity];

    // copy over member variables 
    m_Capacity = rhs.m_Capacity;
    m_Size = rhs.m_Size;

    // copy over items
    for(size_t ndx = 0; ndx < m_Size; ++ndx) {
      m_Data[ndx] = rhs.m_Data[ndx];
    }

    return *this;
  }

  // move assignment operator
  const Stack<T>& operator=(const Stack<T>&& rhs) { 
    // check for self assignment
    if (*this == &rhs) { return *this; }

    // make way for new data
    delete[] m_Data;

    // copy over member variables 
    m_Capacity = rhs.m_Capacity;
    m_Size = rhs.m_Size;
    
    // steal pointer
    m_Data = rhs.m_Data;

    return *this;
  }

  // move copy constructor
  Stack<T>(const Stack<T>&& rhs) : m_Data(rhs.m_Data), m_Capacity(rhs.m_Capacity), m_Size(rhs.m_Size) {}

  size_t size() const {
    return m_Size;
  }

  const T& top() const {
    if(m_Size == 0) { throw std::out_of_range("Used top on empty stack!\n"); }
    return m_Data[m_Size];
  }

  T& top() {
    if(m_Size == 0) { throw std::out_of_range("Used top on empty stack!\n"); }
    return m_Data[m_Size];
  }

  T& pop() {
    if(m_Size == 0) { throw std::out_of_range("Used pop on empty stack!\n"); }

    if (m_Size < m_Capacity/2) {shrink();}
    
    return m_Data[m_Size--];
  }

  void push_back(const T& item) {
    if (m_Size >=  m_Capacity/2) { grow(); }

    m_Data[m_Size++] = item;
  }

};
