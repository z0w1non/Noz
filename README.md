# Noz
![Noz](https://github.com/z0w1non/Noz/blob/master/Noz.png)

## Overview
**Noz provides the generic type conversion.**
```
let s = "100";
int i = s; //100
```
```
let d = 0.5;
string s = d; //"0.5"
```

## Feature
- Concise type conversion
- Downward compatibility with primitive types (int, std::string, bool, etc.)
- Header only

## Usage
Only include "Noz.hpp" in your header file or source file.

## Motivation
The type converting in C++ as described below require developers to write long code.
```
double d = 1.5;
int i = static_cast<int>(d);
```
```
struct base {};
struct derived : base {};
base * b = new derived;
derived * d = dynamic_cast<derived *>(b);
```
```
std::string s = "123";
int    i = std::atoi(s.c_str());
double d = std::atod(s.c_str());
```
```
int i = 0;
std::string s = std::to_string(i);  //since C++11
```
Developers required to use difference functions or cast operators such as `static_cast`, `dynamic_cast`, `atoi`, `atof`, `to_string` depending on a source type and a destination type of the type casting, and these are named asymmetrically. However, what should be used for type casting can be deduced from a source type and a destination type at compile-time.

## Generic cast
Template function `Noz::generic_cast` can be used to cast a any type to another type.
```
int         i = Noz::generic_cast<int>("1");
std::string s = Noz::generic_cast<std::string>(1);
double      d = Noz::generic_cast<double>(1);
```
This function provides optimized generic type castings. If there is a code as described in the following
```
T t;
auto u = Noz::generic_cast<U>(t);
```
then
1. If `Noz::is_static_castalbe<T, U>::value == true` then convert with `static_cast<U>(t)`.
1. Else if `Noz::is_static_pointer_castalbe<T, U>::value == true` then convert with `static_pointer_cast<U>(t)`.
1. Else if `Noz::is_dynamic_castable<T, U>::value == true` then convert with `dynamic_cast<U>(t)`.
1. Else if `Noz::is_dynamic_pointer_castable<T, U>::value == true` then convert with `dynamic_pointer_cast<U>(t)`.
1. Else if `Noz::is_constructor_castable<T, U>::value == true` then convert with `U(t)`.
1. Else convert with `Noz::lexical_cast<U>(t)` that implemented by `operator <<(std::ostream &, const T &)` and `operator >>(std::istream &, U &)`.

A few destructive type castings as described in the following are not used at implementation of  `Noz::generic_cast` to prevent developers from unexpected problems.
- `const_cast`
- `reinterpret_cast`

Noz::generic_cast can be uniformly applied to the type conversion from `std::shared_ptr<T>` to `std::shared_ptr<U>`, and that is resolved with `std::static_pointer_cast<U, T>` or `std::dynamic_pointer_cast<U, T>` as described below.
```
struct base {};
struct derived : base { int value{1}; };
std::shared_ptr<base> ptr = std::make_shared<derived>();
assert(Noz::generic_cast<std::shared_ptr<derived>>(ptr)->value == 1);
```

## Implicit conversion
Noz provides few types Int, Bool, String and so on has an upper camel case naming.
These types provide the compatibility with the primitive types such as int, bool, etc. or the standard library types such as std::string and std::string_view.

The types provided by Noz is constructible from the primitive types or the types of string representations.
Noz::Int is constructible from int, and Noz::String is constructible from std::string, std::string_view, const char [N], and const char * .

The types provided by Noz are easily convertible to the other types.

```
Noz::String s1 = "1.5";
Noz::Double d  = s;

Noz::Int    i  = 1;
Noz::String s2 = i;
```

## Universal initialization
You can declare variables of type provided by Noz with `let` as described below.
```
Noz::let str   = "foo"; // means String str   = "foo";
Noz::let value = 1;     // means Int    value = 1;
```

This `let` syntax is implemented by the class template argument deduction (since C++17).

## Stringize
Using Noz::String as the left-hand-side or right-hand-side operand of `operator +` , you can stringize any types and concatenate them. The constuctor of Noz::String is used to stringize other types.
```
Noz::String a = "foo";
Noz::String b = a + 1;
Noz::String c = 1 + a;
Noz::String d = Noz::String(1);
```

## String behaves like a value type
Noz::String references a constant string and provides the value semantics like the String type in Java.
In many cases, you do not need to use the `const` qualifier to the arguments of type Noz::String.
Noz::String can be used in the same way as premitive types such as int or bool. The copy constructor and the copy assignment operator of Noz::String is executed quickly because it is implemented with copying of a std::shared_ptr.
```
void print(Noz::String b) {
    b = "bar";
    std::cout << b << std::endl; // print "bar"
}

void main() {
    Noz::String a = "foo";
    print(a);
    std::cout << a << std::endl; // print "foo"
}
```

## License
This software is released under the MIT License, see LICENSE.md.
