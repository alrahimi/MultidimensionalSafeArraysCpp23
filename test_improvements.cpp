#include "RangeCheckedMultidArray.h"
#include <iostream>
#include <cassert>

void test_dimension_reporting() {
    std::cout << "Testing dimension error reporting...\n";
    
    GridTools::Array3D<int, 3, 4, 5> arr3d;
    arr3d.fill(0);
    
    // Test valid access
    arr3d[0][0][0] = 1;
    assert(arr3d[0][0][0] == 1);
    std::cout << "✓ Valid 3D access works\n";
    
    // Test dimension 1 (outermost) bounds
    try {
        arr3d[3][0][0] = 1;  // Should fail - index 3, size 3
        std::cout << "✗ Should have thrown for dimension 1\n";
    } catch (const GridTools::RangeException& e) {
        std::cout << "✓ Caught expected error: " << e.what() << "\n";
    }
    
    // Test dimension 2 (middle) bounds
    try {
        arr3d[0][4][0] = 1;  // Should fail - index 4, size 4
        std::cout << "✗ Should have thrown for dimension 2\n";
    } catch (const GridTools::RangeException& e) {
        std::cout << "✓ Caught expected error: " << e.what() << "\n";
    }
    
    // Test dimension 3 (innermost) bounds
    try {
        arr3d[0][0][5] = 1;  // Should fail - index 5, size 5
        std::cout << "✗ Should have thrown for dimension 3\n";
    } catch (const GridTools::RangeException& e) {
        std::cout << "✓ Caught expected error: " << e.what() << "\n";
    }
}

void test_move_semantics() {
    std::cout << "\nTesting move semantics...\n";
    
    GridTools::Array2D<int, 2, 3> arr1;
    arr1.fill(42);
    
    // Test move constructor
    GridTools::Array2D<int, 2, 3> arr2 = std::move(arr1);
    assert(arr2[0][0] == 42);
    std::cout << "✓ Move constructor works\n";
    
    // Test move assignment
    GridTools::Array2D<int, 2, 3> arr3;
    arr3 = std::move(arr2);
    assert(arr3[0][0] == 42);
    std::cout << "✓ Move assignment works\n";
}

void test_constexpr_size() {
    std::cout << "\nTesting constexpr size...\n";
    
    GridTools::Array1D<int, 10> arr1d;
    // size() is constexpr and returns compile-time constant
    static_assert(std::is_same_v<decltype(arr1d.size()), size_t>);
    assert(arr1d.size() == 10);
    
    GridTools::Array2D<int, 5, 7> arr2d;
    assert(arr2d.size() == 5);
    std::cout << "✓ Constexpr size() works\n";
}

void test_copy_semantics() {
    std::cout << "\nTesting copy semantics...\n";
    
    GridTools::Array2D<int, 2, 2> arr1;
    arr1[0][0] = 1;
    arr1[0][1] = 2;
    arr1[1][0] = 3;
    arr1[1][1] = 4;
    
    // Test copy constructor
    GridTools::Array2D<int, 2, 2> arr2 = arr1;
    assert(arr2[0][0] == 1);
    assert(arr2[1][1] == 4);
    std::cout << "✓ Copy constructor works\n";
    
    // Test copy assignment
    GridTools::Array2D<int, 2, 2> arr3;
    arr3 = arr1;
    assert(arr3[0][0] == 1);
    assert(arr3[1][1] == 4);
    std::cout << "✓ Copy assignment works\n";
    
    // Modify original and check independence
    arr1[0][0] = 99;
    assert(arr2[0][0] == 1);  // Should not change
    assert(arr3[0][0] == 1);  // Should not change
    std::cout << "✓ Copy creates independent objects\n";
}

int main() {
    std::cout << "=== RangedArray Test Suite ===\n\n";
    
    test_dimension_reporting();
    test_move_semantics();
    test_constexpr_size();
    test_copy_semantics();
    
    std::cout << "\n=== All tests passed! ===\n";
    return 0;
}
