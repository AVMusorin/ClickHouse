#include <Functions/FunctionFactory.h>
#include <Functions/FunctionsComparison.h>
#include <Functions/FunctionsLogical.h>

namespace DB
{

using FunctionGreaterOrEquals = FunctionComparison<GreaterOrEqualsOp, NameGreaterOrEquals>;
using FunctionGreater = FunctionComparison<GreaterOp, NameGreater>;
extern template class FunctionComparison<GreaterOp, NameGreater>;
using FunctionEquals = FunctionComparison<EqualsOp, NameEquals>;
extern template class FunctionComparison<EqualsOp, NameEquals>;

REGISTER_FUNCTION(GreaterOrEquals)
{
    factory.registerFunction<FunctionGreaterOrEquals>();
}

template <>
ColumnPtr FunctionComparison<GreaterOrEqualsOp, NameGreaterOrEquals>::executeTupleImpl(
    const ColumnsWithTypeAndName & x, const ColumnsWithTypeAndName & y, size_t tuple_size, size_t input_rows_count) const
{

    FunctionOverloadResolverPtr greater
        = std::make_unique<FunctionToOverloadResolverAdaptor>(std::make_shared<FunctionGreater>(params));

    FunctionOverloadResolverPtr greater_or_equals
        = std::make_unique<FunctionToOverloadResolverAdaptor>(std::make_shared<FunctionGreaterOrEquals>(params));

    FunctionOverloadResolverPtr func_builder_or
        = std::make_unique<FunctionToOverloadResolverAdaptor>(std::make_shared<FunctionOr>());

    FunctionOverloadResolverPtr func_builder_and
        = std::make_unique<FunctionToOverloadResolverAdaptor>(std::make_shared<FunctionAnd>());

    FunctionOverloadResolverPtr func_builder_equals
        = std::make_unique<FunctionToOverloadResolverAdaptor>(std::make_shared<FunctionEquals>(params));

    return executeTupleLessGreaterImpl(
        greater,
        greater_or_equals,
        func_builder_and,
        func_builder_or,
        func_builder_equals,
        x, y, tuple_size, input_rows_count);
}

}
