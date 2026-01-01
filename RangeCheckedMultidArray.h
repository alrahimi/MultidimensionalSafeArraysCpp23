#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm>

namespace GridTools {

    // Exception remains the same
    class RangeException : public std::out_of_range {
    public:
        RangeException(size_t index, size_t size, int dim_level) 
            : std::out_of_range("Range Error: Index [" + std::to_string(index) + 
                                "] exceeds size (" + std::to_string(size) + 
                                ") at Dimension Level " + std::to_string(dim_level)) {}
    };

    // 1. PRIMARY TEMPLATE (Recursive/Multi-D Case)
    // This is now the default "many-argument" version.
    template <typename T, size_t S, size_t... Rest>
    class RangedArray {
    private:
        std::vector<RangedArray<T, Rest...>> data;
    public:
        static constexpr int TotalDims = sizeof...(Rest) + 1;

        RangedArray() : data(S) {}

        void fill(const T& value) {
            for (auto& sub : data) sub.fill(value);
        }

        RangedArray<T, Rest...>& operator[](size_t i) {
            if (i >= S) throw RangeException(i, S, TotalDims);
            return data[i];
        }

        const RangedArray<T, Rest...>& operator[](size_t i) const {
            if (i >= S) throw RangeException(i, S, TotalDims);
            return data[i];
        }

        size_t size() const { return S; }
    };

    // 2. PARTIAL SPECIALIZATION (Base/1D Case)
    // This "specializes" the template for when only one size is provided.
    template <typename T, size_t S>
    class RangedArray<T, S> {
    private:
        std::vector<T> data;
    public:
        static constexpr int TotalDims = 1;
        RangedArray() : data(S) {}

        void fill(const T& value) {
            std::fill(data.begin(), data.end(), value);
        }

        T& operator[](size_t i) {
            if (i >= S) throw RangeException(i, S, 1);
            return data[i];
        }

        const T& operator[](size_t i) const {
            if (i >= S) throw RangeException(i, S, 1);
            return data[i];
        }

        size_t size() const { return S; }
    };
    // --- Convenience Type Aliases ---
    template <typename T, size_t R>
    using Array1D = RangedArray<T, R>;
    
    
    
    
    template <typename T, size_t R, size_t C>
    using Array2D = RangedArray<T, R, C>;
    
    
    template <typename T, size_t D, size_t R, size_t C>
    using Array3D = RangedArray<T, D, R, C>;

    // 4D Alias: 
    template <typename T, size_t d1, size_t d2, size_t d3, size_t d4>
    using Array4D = RangedArray<T, d1, d2, d3,d4>;

} // namespace GridTools