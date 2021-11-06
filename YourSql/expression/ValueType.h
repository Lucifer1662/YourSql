#pragma once

enum class ValueType {
	Invalid,Int, Float, Long, Double, String, Date, DateTime
};


template<typename T> ValueType getType();


template<> ValueType inline getType<int>() { return ValueType::Int; };
template<> ValueType inline getType<float>() { return ValueType::Float; };
template<> ValueType inline getType<long>() { return ValueType::Long; };
template<> ValueType inline getType<double>() { return ValueType::Double; };
