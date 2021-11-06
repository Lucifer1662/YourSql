#include "INumber.h"
#include "Number.h"



//std::unique_ptr<Value> add(const Value& left, const Value& right) {
//
//	switch (left.type) {
//	case ValueType::Int: {
//		auto newValue = ((INumber<int>&)left).value() + ((INumber<int>&)right).value();
//
//	}
//					   /*case ValueType::Double: return std::make_unique<Number>(left.value() + ((INumber<double>&)right).value());
//					   case ValueType::Float: return std::make_unique<Number>(left.value() + ((INumber<float>&)right).value());
//					   case ValueType::Long: return std::make_unique<Number>(left.value() + ((INumber<long>&)right).value());*/
//	}
//
//	throw std::exception("Invalid operation");
//
//}
