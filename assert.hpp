#pragma once
#ifndef DEFINED_ASSERT_HPP
#define DEFINED_ASSERT_HPP

#ifdef NDEBUG
#define assert(...) (void)0
#else
#define assert(...) \
	do { \
		[](const char * file_name, const char* function_name, unsigned int line_number, const char * assertion_string, auto assert_function) -> void{ \
			if constexpr (noexcept(assert_function())) { \
				static_assert(assert_function(), "compile-time assertion failed"); \
			} \
			else { \
				runtime_assert(file_name, function_name, line_number, assertion_string, assert_function()); \
			} \
		}(__FILE__, __FUNCTION__, __LINE__, #__VA_ARGS__, [&]() { return __VA_ARGS__; }); \
	} while (0)
#endif

inline void runtime_assert(const char * file_name, const char* function_name, unsigned int line_number, const char * assertion_string, bool assertion) {
	if (!assertion)
		std::cerr << "runtime assertion failed at " << file_name << "(" << line_number << ")!" << function_name << ": (" << assertion_string << ")" << std::endl;
}

#endif
