//------------------------------------------------------------------------------
//! @file LValue.h
//! @brief Compile-time lvalue representation (for constant evaluation)
//
// File is under the MIT license; see LICENSE for details
//------------------------------------------------------------------------------
#pragma once

#include "slang/numeric/ConstantValue.h"

namespace slang {

/// An lvalue is anything that can appear on the left hand side of an assignment
/// expression. It represents some storage location in memory that can be read
/// from and written to.
///
class LValue {
public:
    /// A concatenation of lvalues is also an lvalue and can be assigned to.
    using Concat = std::vector<LValue>;

    LValue() = default;
    LValue(nullptr_t) {}
    LValue(const LValue&) = delete;
    LValue(LValue&& other) = default;

    explicit LValue(Concat&& concat) : value(std::move(concat)) {}
    explicit LValue(ConstantValue& base) : value(Path{ &base }) {}

    bool bad() const { return std::holds_alternative<std::monostate>(value); }
    explicit operator bool() const { return !bad(); }

    ConstantValue load() const;
    void store(const ConstantValue& value);

    void addBitSlice(ConstantRange range);
    void addIndex(int32_t index, ConstantValue&& defaultValue);
    void addArraySlice(ConstantRange range, ConstantValue&& defaultValue);
    void addArrayLookup(ConstantValue&& index, ConstantValue&& defaultValue);

private:
    // A selection of a range of bits from an integral value.
    struct BitSlice {
        ConstantRange range;
    };

    // A selection of a single element from an array, string, or struct.
    struct ElementIndex {
        int32_t index = 0;
        ConstantValue defaultValue;
    };

    // A selection of a range of elements from an array.
    struct ArraySlice {
        ConstantRange range;
        ConstantValue defaultValue;
    };

    // A lookup of an element from an associative array.
    struct ArrayLookup {
        ConstantValue index;
        ConstantValue defaultValue;
    };

    using PathElement = std::variant<BitSlice, ElementIndex, ArraySlice, ArrayLookup>;
    struct Path {
        ConstantValue* base = nullptr;
        SmallVectorSized<PathElement, 4> elements;
    };

    // Every LValue is either a pointer to some variable plus a path of selections,
    // or it's a concatenation of other lvalues.
    std::variant<std::monostate, Path, Concat> value;
};

} // namespace slang