#ifndef RANGECHECKED_MULTIDARRAY_H
#define RANGECHECKED_MULTIDARRAY_H

#include <vector>
#include <stdexcept>
#include <string>
#include <algorithm>

namespace GridTools {

    // Exception class for range errors
    class RangeException : public std::out_of_range {
    public:
        RangeException(size_t index, size_t size, int dim_level, int total_dims) 
            : std::out_of_range("Range Error: Index [" + std::to_string(index) + 
                                "] exceeds size (" + std::to_string(size) + 
                                ") at Dimension " + std::to_string(dim_level) + 
                                " of " + std::to_string(total_dims)) {}
    };

    // Internal implementation with dimension tracking
    namespace detail {
        template <typename T, int CurrentDim, int TotalDims, size_t S, size_t... Rest>
        class RangedArrayImpl;

        // PRIMARY TEMPLATE (Recursive/Multi-D Case)
        template <typename T, int CurrentDim, int TotalDims, size_t S, size_t... Rest>
        class RangedArrayImpl {
        private:
            std::vector<RangedArrayImpl<T, CurrentDim + 1, TotalDims, Rest...>> data;
        public:
            RangedArrayImpl() noexcept : data(S) {}

            // Move constructor
            RangedArrayImpl(RangedArrayImpl&&) noexcept = default;

            // Move assignment operator
            RangedArrayImpl& operator=(RangedArrayImpl&&) noexcept = default;

            // Copy constructor
            RangedArrayImpl(const RangedArrayImpl&) = default;

            // Copy assignment operator
            RangedArrayImpl& operator=(const RangedArrayImpl&) = default;

            void fill(const T& value) {
                for (auto& sub : data) sub.fill(value);
            }

            RangedArrayImpl<T, CurrentDim + 1, TotalDims, Rest...>& operator[](size_t i) {
                if (i >= S) throw RangeException(i, S, CurrentDim, TotalDims);
                return data[i];
            }

            const RangedArrayImpl<T, CurrentDim + 1, TotalDims, Rest...>& operator[](size_t i) const {
                if (i >= S) throw RangeException(i, S, CurrentDim, TotalDims);
                return data[i];
            }

            constexpr size_t size() const noexcept { return S; }
        };

        // PARTIAL SPECIALIZATION (Base/1D Case)
        template <typename T, int CurrentDim, int TotalDims, size_t S>
        class RangedArrayImpl<T, CurrentDim, TotalDims, S> {
        private:
            std::vector<T> data;
        public:
            RangedArrayImpl() noexcept : data(S) {}

            // Move constructor
            RangedArrayImpl(RangedArrayImpl&&) noexcept = default;

            // Move assignment operator
            RangedArrayImpl& operator=(RangedArrayImpl&&) noexcept = default;

            // Copy constructor
            RangedArrayImpl(const RangedArrayImpl&) = default;

            // Copy assignment operator
            RangedArrayImpl& operator=(const RangedArrayImpl&) = default;

            void fill(const T& value) {
                std::fill(data.begin(), data.end(), value);
            }

            T& operator[](size_t i) {
                if (i >= S) throw RangeException(i, S, CurrentDim, TotalDims);
                return data[i];
            }

            const T& operator[](size_t i) const {
                if (i >= S) throw RangeException(i, S, CurrentDim, TotalDims);
                return data[i];
            }

            constexpr size_t size() const noexcept { return S; }
        };
    } // namespace detail

    // 1. PUBLIC TEMPLATE - wraps the implementation with proper dimension tracking
    template <typename T, size_t S, size_t... Rest>
    class RangedArray : public detail::RangedArrayImpl<T, 1, sizeof...(Rest) + 1, S, Rest...> {
    public:
        // Expose total dimensions for public API
        static constexpr int TotalDims = sizeof...(Rest) + 1;

        // Inherit constructors from base implementation
        using detail::RangedArrayImpl<T, 1, sizeof...(Rest) + 1, S, Rest...>::RangedArrayImpl;
    };

    // 2. PARTIAL SPECIALIZATION (Base/1D Case) - also wraps implementation
    template <typename T, size_t S>
    class RangedArray<T, S> : public detail::RangedArrayImpl<T, 1, 1, S> {
    public:
        // Expose total dimensions for public API
        static constexpr int TotalDims = 1;

        // Inherit constructors from base implementation
        using detail::RangedArrayImpl<T, 1, 1, S>::RangedArrayImpl;
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
    using Array4D = RangedArray<T, d1, d2, d3, d4>;

} // namespace GridTools

#endif // RANGECHECKED_MULTIDARRAY_H