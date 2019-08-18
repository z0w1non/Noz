#pragma once
#ifndef NIL_HPP
#define NIL_HPP
#include <optional>

/**
 * nil represents an invalid valuu that assignable to any pointer type and std::optional<T> class.
 * std::optional<int> opt = nil; assert(opt == std::nullopt);
 * int * ptr = nil;              assert(ptr == nullptr);
 *
 */
const class {
public:
	template<typename T>
	operator T* () const {
			return 0;
		}

	template<class C, class T>
	operator T C::* () const {
		return 0;
	}

	operator std::nullopt_t() const {
		return std::nullopt;
	}

private:
	void operator &() const;
} nil;

#endif
