#include "Stack.h"
#include <assert.h>

struct Tracker {
  static int constructions;
  static int moves;
  static int copies;

  static void reset() noexcept
  {
    constructions = moves = copies = 0;
  }

  int m_Value;

  Tracker(int value = 0) :
    m_Value(value)
  {
    ++constructions;
  }
  
  Tracker(const Tracker& rhs) :
    m_Value(rhs.m_Value)
  {
    ++copies;
  }

  Tracker(Tracker&& rhs) noexcept :
    m_Value(rhs.m_Value)
  {
    ++moves;
  }

  Tracker& operator=(const Tracker& rhs) 
  {
    if(this == &rhs) 
    {
      return *this; 
    }

    ++copies;
    m_Value = rhs.m_Value;

    return *this;
  }

  Tracker& operator=(Tracker&& rhs)
  {
    if(this == &rhs) 
    {
      return *this; 
    }

    ++moves;
    m_Value = rhs.m_Value;

    return *this;
  }
};

int Tracker::constructions = 0;
int Tracker::copies = 0;
int Tracker::moves = 0;

void test_basic_operations() {
    std::cout << "Testing basic operations...\n";
    
    Stack<int> s;
    assert(s.isEmpty());
    assert(s.size() == 0);
    
    s.push_back(1);
    assert(!s.isEmpty());
    assert(s.size() == 1);
    assert(s.top() == 1);
    
    s.push_back(2);
    s.push_back(3);
    assert(s.size() == 3);
    assert(s.top() == 3);
    
    s.pop();
    assert(s.top() == 2);
    assert(s.size() == 2);
    
    std::cout << "✓ Basic operations passed\n";
}

void test_copy_semantics() {
    std::cout << "Testing copy semantics...\n";
    
    Stack<int> s1;
    s1.push_back(10);
    s1.push_back(20);
    
    Stack<int> s2 = s1;  // Copy constructor
    assert(s2.size() == 2);
    assert(s2.top() == 20);
    
    s2.pop();
    assert(s1.top() == 20);  // s1 unchanged
    assert(s2.top() == 10);
    
    Stack<int> s3;
    s3 = s1;  // Copy assignment
    assert(s3.size() == 2);
    assert(s3.top() == 20);
    
    std::cout << "✓ Copy semantics passed\n";
}

void test_move_semantics() {
    std::cout << "Testing move semantics...\n";
    
    Stack<std::string> s1;
    s1.push_back("hello");
    s1.push_back("world");
    
    Stack<std::string> s2 = std::move(s1);  // Move constructor
    assert(s2.size() == 2);
    assert(s2.top() == "world");
    assert(s1.size() == 0);  // s1 moved-from
    
    Stack<std::string> s3;
    s3 = std::move(s2);  // Move assignment
    assert(s3.size() == 2);
    assert(s3.top() == "world");
    
    std::cout << "✓ Move semantics passed\n";
}

void test_emplace_vs_push() {
    std::cout << "Testing emplace vs push...\n";
    
    Tracker::reset();
    Stack<Tracker> s1;
    s1.push_back(Tracker(42));  // Construct temporary, then move
    std::cout << "  push_back: " << Tracker::constructions << " constructions, "
              << Tracker::moves << " moves\n";
    
    Tracker::reset();
    Stack<Tracker> s2;
    s2.emplace_back(42);  // Construct in-place
    std::cout << "  emplace_back: " << Tracker::constructions << " constructions, "
              << Tracker::moves << " moves\n";
    
    // emplace should have fewer operations
    std::cout << "✓ Emplace test completed\n";
}

void test_grow_shrink() {
    std::cout << "Testing grow/shrink...\n";
    
    Stack<int> s(2);  // Small initial capacity
    
    s.push_back(1);
    s.push_back(2);
    s.push_back(3);  // Should trigger grow
    s.push_back(4);
    
    assert(s.size() == 4);
    
    s.pop();
    s.pop();
    s.pop();  // Should trigger shrink at 1/4 capacity
    
    assert(s.size() == 1);
    assert(s.top() == 1);
    
    std::cout << "✓ Grow/shrink passed\n";
}

void test_move_only_type() {
    std::cout << "Testing move-only types...\n";
    
    Stack<std::unique_ptr<int>> s;
    s.emplace_back(std::make_unique<int>(42));
    s.push_back(std::make_unique<int>(100));
    
    assert(*s.top() == 100);
    s.pop();
    assert(*s.top() == 42);
    
    std::cout << "✓ Move-only types passed\n";
}

void test_exceptions() {
    std::cout << "Testing exceptions...\n";
    
    Stack<int> s;
    
    try {
        s.top();
        assert(false);  // Should not reach
    } catch (const std::out_of_range&) {
        // Expected
    }
    
    try {
        s.pop();
        assert(false);
    } catch (const std::out_of_range&) {
        // Expected
    }
    
    std::cout << "✓ Exception handling passed\n";
}

int main() {
    test_basic_operations();
    test_copy_semantics();
    test_move_semantics();
    test_emplace_vs_push();
    test_grow_shrink();
    test_move_only_type();
    test_exceptions();
    
    std::cout << "\n✅ All tests passed!\n";
    return 0;
}
