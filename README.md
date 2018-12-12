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
The type conversion in C++ as described below require developers to write long code.
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

`Noz::generic_cast` can be uniformly applied to the type conversion from `std::shared_ptr<T>` to `std::shared_ptr<U>`, and that is resolved with `std::static_pointer_cast<U, T>` or `std::dynamic_pointer_cast<U, T>` as described below.
```
struct base {};
struct derived : base { int value{1}; };
std::shared_ptr<base> ptr = std::make_shared<derived>();
assert(Noz::generic_cast<std::shared_ptr<derived>>(ptr)->value == 1);
```

## Implicit convetible type
Noz provides several implicit convertible types that have upper camel case naming such as `Bool`, `Int`, `String`, etc.. These types are implicitly call `Noz::generic_cast<T>(...)` at the own constructor and `operator T() const` only when these are not explicitly overloaded.
```
Noz::String s1 = "1.5";
Noz::Double d  = s;

Noz::String s2 = 1;
```

| Primitive type | Alias type | Entity type |
|---|---|---|
| bool | Noz::Bool | Noz::let&lt;bool&gt; |
| char | Noz::Char | Noz::let&lt;char&gt; |
| signed char | Noz::SignedChar | Noz::let&lt;signed char&gt; |
| unsigned char | Noz::UnsignedChar | Noz::let&lt;unsigned char&gt; |
| short | Noz::Short | Noz::let&lt;short&gt; |
| unsigned short | Noz::UnsignedShort | Noz::let&lt;unsigned short&gt; |
| int | Noz::Int | Noz::let&lt;int&gt; |
| unsigned int | Noz::UnsignedInt | Noz::let&lt;unsigned int&gt; |
| long | Noz::Long | Noz::let&lt;long&gt; |
| unsigned long | Noz::UnsignedLong | Noz::let&lt;unsigned long&gt; |
| long long | Noz::LongLong | Noz::let&lt;long long&gt; |
| unsigned long long | Noz::UnsignedLongLong | Noz::let&lt;unsigned long long&gt; |
| float | Noz::Float | Noz::let&lt;float&gt; |
| double | Noz::Double | Noz::let&lt;double&gt; |
|std::string | Noz::String | Noz::let&lt;Noz::string_tag&lt;char&gt;&gt; |
|std::wstring | Noz::WString | Noz::let&lt;Noz::string_tag&lt;wchar_t&gt;&gt; |

## Universal initialization
Developers be able to declare variables of implicit convertible type with `Noz::let` as described below.
```
Noz::let str   = "foo"; // means String str   = "foo";
Noz::let value = 1;     // means Int    value = 1;
```

This `let` syntax is implemented by the class template argument deduction (since C++17).

## String concatenation
Using Noz::String as the left-hand-side or right-hand-side operand of `operator +` , you can stringize any types and concatenate them. The constuctor of `Noz::String` can be used to convert other types to the strings.
```
Noz::String a = "foo";
Noz::String b = a + 1;
Noz::String c = 1 + a;
```

## String behaves like a value type
`Noz::String` has `std::shared_ptr<const std::string>`, which is the pointer to the **constant** string, and provides the value semantics such as the `String` type in Java.
In many cases, developers do not need to use the `const` qualifier to the arguments of type `Noz::String`.
`Noz::String` can be used in the same way as primitive types such as int or bool. The copy constructor and the copy assignment operator of `Noz::String` are executed quickly because it is implemented with the copy assignment operator of the `std::shared_ptr<const std::string>`.
```
void print(Noz::String b) {
    b = "bar";
    std::cout << b << std::endl; // print "bar"
}

void main() {
    Noz::String a = "foo";
    print(a);
    std::cout << a << std::endl; // print "foo", not "bar"
}
```

## License
This software is released under the MIT License, see LICENSE.md.
