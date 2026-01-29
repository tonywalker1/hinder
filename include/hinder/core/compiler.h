//
// hinder::core
//
// MIT License
//
// Copyright (c) 2019-2021  Tony Walker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef HINDER_CORE_COMPILER_H
#define HINDER_CORE_COMPILER_H

//
// Useful for making tests more readable
//
#define HINDER_CPP_11 201'103L
#define HINDER_CPP_14 201'402L
#define HINDER_CPP_17 201'703L
#define HINDER_CPP_20 202'002L

//
// Branch prediction hints.
//
// NOTE: [[likely]] and [[unlikely]] attributes can only be used with if-statements, not ternary
// operators. These macros use __builtin_expect when C++20 attributes cannot be used directly.
// The project requires C++20, so these macros always expand to the __builtin form for use in
// ternary expressions throughout the codebase.
//
#if defined(__clang__) || defined(__GNUC__)
    #define HINDER_LIKELY(cond)   __builtin_expect((cond), 1)
    #define HINDER_UNLIKELY(cond) __builtin_expect((cond), 0)
#else
    #define HINDER_LIKELY(cond)   (cond)
    #define HINDER_UNLIKELY(cond) (cond)
#endif

//
// DEPRECATED: Use [[nodiscard]] attribute directly.
// This macro is retained for backward compatibility but will be removed in a future version.
// The project requires C++20, so [[nodiscard]] is always available.
//
#define HINDER_NODISCARD [[nodiscard]]

//
// Compiles to nothing (i.e., a no-op)
//
#define HINDER_NOOP static_cast<void>(0)

#endif  // HINDER_CORE_COMPILER_H
