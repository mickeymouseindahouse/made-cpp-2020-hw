#pragma once

#include <vector>
#include <iostream>
#include <exception>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <numeric>

#define THREE 3
#define EPSILON 1e-7

namespace task {

    template< typename T>
    std::vector<T> operator + (const std::vector<T>& v) {
        return v;
    }

    template< typename T>
    std::vector<T> operator - (const std::vector<T>& v) {
        std::vector<T> res;
        res.reserve(v.size());
        std::transform(v.begin(), v.end(), std::back_inserter(res), std::negate<T>());
        return res;
    }

    template< typename T >
    std::vector<T> operator + (const std::vector<T>& lhs, const std::vector<T>& rhs) {
        if(lhs.size() != rhs.size()) {
            throw std::runtime_error("Vectors should be of same size!");
        }
        std::vector<T> res;
        res.reserve(lhs.size());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), std::back_inserter(res), std::plus<T>());
        return res;
    }

    template< typename T >
    std::vector<T> operator - (const std::vector<T>& lhs, const std::vector<T>& rhs) {
        if(lhs.size() != rhs.size()) {
            throw std::runtime_error("Vectors should be of same size!");
        }
        std::vector<T> res;
        res.reserve(lhs.size());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), std::back_inserter(res), std::minus<T>());
        return res;
    }

    template< typename T >
    double operator * (const std::vector<T>& lhs, const std::vector<T>& rhs) {
        if(lhs.size() != rhs.size()) {
            throw std::runtime_error("Vectors should be of same size!");
        }
        return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), 0.0);
    }

    template< typename T >
    std::vector<T> operator % (const std::vector<T>& lhs, const std::vector<T>& rhs) {
        if(lhs.size() != rhs.size()) {
            throw std::runtime_error("Vectors should be of same size!");
        }
        if(lhs.size() != THREE) {
            throw std::runtime_error("Vector product available only for 3D vectors!");
        }
        return std::vector<T> {
            lhs[1] * rhs[2] - lhs[2] * rhs[1],
            lhs[2] * rhs[0] - lhs[0] * rhs[2],
            lhs[0] * rhs[1] - lhs[1] * rhs[0]
        };
    }

    template< typename T >
    bool operator || (const std::vector<T>& lhs, const std::vector<T>& rhs) {
        if(lhs.size() != rhs.size()) {
            throw std::runtime_error("Vectors should be of same size!");
        }
        for(size_t i = 0; i + 1 < lhs.size(); ++i) {
            // if lhs || rhs and lhs[i] == 0 then rhs[i] == 0
            if((lhs[i] == 0.0 || rhs[i] == 0.0) && (lhs[i] + rhs[i] != 0.0)
                || fabs(lhs[i] / rhs[i] - lhs[i + 1] /rhs[i + 1]) > EPSILON) {
                return false;
            }
        }
        return true;
    }

    template< typename T >
    bool operator && (const std::vector<T>& lhs, const std::vector<T>& rhs) {
        if( lhs || rhs) {
            for(size_t i = 0; i < lhs.size(); ++i) {
                if(rhs[i] != 0.0) {
                    return lhs[i] / rhs[i] > 0.0;
                }
            }
            // zero vector is same directed with any vector
            return true;
        } else {
            // non-collinear vectors can't be same directed
            return false;
        }
    }

    template< typename T >
    std::istream& operator >> (std::istream& stream, std::vector<T>& v) {
        size_t size;
        stream >> size;

        v.clear();
        std::copy(std::istream_iterator<T>(stream), std::istream_iterator<T>(), std::back_inserter(v));
        return stream;
    }

    template< typename T >
    std::ostream& operator << (std::ostream& stream, const std::vector<T>& v) {
        for(size_t i = 0; i < v.size(); ++i) {
            stream << v[i] << " ";
        }
        stream << std::endl;
        return stream;
    }

    template< typename T >
    // in tests, std::reverse is called, thus function is never called
    void reverse(std::vector<T>& v) {
        std::reverse(v.begin(), v.end());
    }

    template< typename T >
    std::vector<T> operator | (const std::vector<T>& lhs, const std::vector<T>& rhs) {
        if(!std::is_same<T, int>::value) {
            throw std::runtime_error("Bitwise or can be done only with ints!");
        }
        if(lhs.size() != rhs.size()) {
            throw std::runtime_error("Vectors should be of same size!");
        }
        std::vector<T> res;
        res.reserve(lhs.size());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), std::back_inserter(res), std::bit_or<T>());
        return res;
    }

    template< typename T >
    std::vector<T> operator & (const std::vector<T>& lhs, const std::vector<T>& rhs) {
        if(!std::is_same<T, int>::value) {
            throw std::runtime_error("Bitwise and can be done only with ints!");
        }
        if(lhs.size() != rhs.size()) {
            throw std::runtime_error("Vectors should be of same size!");
        }
        std::vector<T> res;
        res.reserve(lhs.size());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), std::back_inserter(res), std::bit_and<T>());
        return res;
    }


}  // namespace task
