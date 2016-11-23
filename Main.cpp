// A basic sample application using Junction.

#include <junction/ConcurrentMap_Grampa.h>
#include <iostream>

class Foo {
  public:
	Foo(int a) : a(a) {}
  int a;
};

struct IntKeyTraits {
    typedef int Key;
    typedef typename turf::util::BestFit<int>::Unsigned Hash;
    static const Key NullKey = Key(0);
    static const Hash NullHash = Hash(0);
    static Hash hash(int key) {
         return 42;
    }
};

void testMap() {
    // Create the map.
    typedef junction::ConcurrentMap_Grampa<turf::u32, Foo*, IntKeyTraits> ConcurrentMap;
    ConcurrentMap myMap;

    // Do a few operations.
    myMap.assign(14, new Foo(1));
    myMap.assign(15, new Foo(2));
    Foo* foo14 = myMap.get(14);
    Foo* foo15 = myMap.get(15);
    std::cout << "Value for key 14 is " << foo14->a << std::endl;
    std::cout << "Value for key 15 is " << foo14->a << std::endl;
}

int main() {
    // Create QSBR context for the main thread.
    junction::QSBR::Context context = junction::DefaultQSBR.createContext();

    // Run a simple map test.
    testMap();

    // Update the QSBR context for this thread.
    // In a larger application, this should be called periodically, for each thread, at a moment
    // when the thread is quiescent – that is, not in the middle of any operation that uses a
    // Junction data structure.
    junction::DefaultQSBR.update(context);

    // Destroy the QSBR context for the main thread.
    junction::DefaultQSBR.destroyContext(context);
    return 0;
}
