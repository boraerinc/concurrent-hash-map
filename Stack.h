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
  Stack<T>(size_t capacity = 10) : m_Data(new T[capacity]), m_Capacity(capacity), m_Size(0) {
    std::cout << "Constructor!\n";
  }
  
  // destructor (no need to set member variables to 0, they're gonna go out of scope.)
  ~Stack<T>() {
    std::cout << "Destructor!\n";
    delete[] m_Data;
    m_Capacity = 0;
    m_Size = 0;
  }

  // copy constructor
  Stack<T>(const Stack<T>& rhs) : m_Data(new T[rhs.m_Capacity]), m_Capacity(rhs.m_Capacity), m_Size(rhs.m_Size) {
    std::cout << "Copy Constr!\n";
    for(size_t ndx = 0; ndx < m_Size; ++ndx) {
      m_Data[ndx] = rhs.m_Data[ndx];
    }
  }
  
  // assignment operator
  const Stack<T>& operator=(const Stack<T>& rhs) { 
    std::cout << "Assignment Oprtr!\n";
    // check for self assignment
    // if (*this == &rhs) { return *this; } // oops, need to compare addresses
    if (this == &rhs) { return *this; }

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
  // r-values should be const (duh), and noexcept
  
  // OLD:
  // const Stack<T>& operator=(const Stack<T>&& rhs) { 
  
  // NEW:
  const Stack<T>& operator=(Stack<T>&& rhs) noexcept { 
    std::cout << "Move Assignment!\n";
    // check for self assignment
    // if (*this == &rhs) { return *this; } // oops, need to compare addresses
    if (this == &rhs) { return *this; }

    // make way for new data
    delete[] m_Data;

    // copy over member variables 
    m_Capacity = rhs.m_Capacity;
    m_Size = rhs.m_Size;
    
    // steal pointer
    m_Data = rhs.m_Data;

    // NEW:
    rhs.m_Data = nullptr;
    rhs.m_Capacity = 0;
    rhs.m_Size = 0;

    // As the r-value is going out of scope, a destructor will be called, so we
    // need to point it to nullptr, otherwise it will cause our resource to be 
    // freed.

    return *this;
  }

  // move copy constructor
  //Stack<T>(const Stack<T>&& rhs) : m_Data(rhs.m_Data), m_Capacity(rhs.m_Capacity), m_Size(rhs.m_Size) {}
  Stack<T>(Stack<T>&& rhs) noexcept : m_Data(rhs.m_Data), m_Capacity(rhs.m_Capacity), m_Size(rhs.m_Size) {
    std::cout << "Move Copy Constr!\n";
    // Always leave rhs in a usable state!
    rhs.m_Data = nullptr;
    rhs.m_Capacity = 0;
    rhs.m_Size = 0;
  }


  // HELPERS
  size_t size() const noexcept {
    return m_Size;
  }
  // NEW:
  bool isEmpty() const noexcept {
    return m_Size == 0;
  }

  explicit operator bool() const {
    return m_Size != 0;
  }

  const T& top() const {
    if(m_Size == 0) { throw std::out_of_range("Used top on empty stack!\n"); }
    // return m_Data[m_Size];
    return m_Data[m_Size-1]; // fix off by one error
  }

  T& top() {
    if(m_Size == 0) { throw std::out_of_range("Used top on empty stack!\n"); }
    // return m_Data[m_Size];
    return m_Data[m_Size-1]; // fix off by one error
  }

  T& pop() {
    if(m_Size == 0) { throw std::out_of_range("Used pop on empty stack!\n"); }

    if (m_Size < m_Capacity/2) {shrink();}
    
    return m_Data[(m_Size--)-1];
  }

  void push_back(const T& item) {
    if (m_Size >=  m_Capacity/2) { grow(); }

    m_Data[m_Size++] = item;
  }

};
