# MultidimensionalSafeArraysCpp23
We will use Recursive Compositiont using C++ Templates to create a generic Multidimensional Safe Array class (Range checked) 
This implementation uses variadic templates (C++11 and later) to allow any number of dimensions.
This is a powerful template technique where each dimension wraps the one below it. 
This removes the need for separate A2D, A3D, and A4D classes, replacing them with a single, elegant Array<Type, Size...> structure.

The advantages:
Zero-Based Efficiency: No more i - low math. The index is checked directly against the size.

Infinite Dimensions: You aren't limited to A4D. You can define an Array<int, 2, 2, 2, 2, 2, 2, 2> if you need to.

Heap Safety: By using std::vector, we avoid the stack overflow issues of the original Type element[size] approach.
Sine it is stored on the Heap there is no practical size limit (can be many GBs) and much more flexible.

Cleaner Syntax: The operator[] automatically returns a reference to the next dimension level.
It provide a fill() method to this recursive template so you can initialize all values across all dimensions at once

--------------
Advantages of using this Recursive Composition model:

Maintenance: You only have one class to maintain, not separate A1D, A2D, etc.

Safety: Range checking is enforced at every bracket level.

Flexibility: It supports any depth of dimension automatically.

Memory: Since each level is a std::vector, it handles memory on the heap, preventing the stack overflow issues seen in your original Listing 1.

It uses a custom namespace to prevent collisions, adopts the more descriptive name RangedArray, 
It supports :  zero-based indexing, heap-safety, recursive fill, and multi-level range checking.
-----------------------------------------------

Memory Consideration
This approach uses a "Vector of Vectors" style memory layout. While highly flexible and safe, 
it is slightly less performant than a single "Flattened" block of memory because the data is fragmented across the heap.

---------------------
Usage Example
-------------------
int main() {
    using namespace GridTools;

    // Much cleaner syntax using aliases
    Matrix<int, 10, 10> table;
    Volume<double, 5, 5, 5> cube;

    table.fill(0);
    cube.fill(1.1);

    try {
        table[9][9] = 100;
        cube[4][4][4] = 5.5;
        
        std::cout << "Table Access Success!" << std::endl;
        
        // This will throw a specific error for 'Dimension Level 3'
        cube[10][0][0] = 0.0; 
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
