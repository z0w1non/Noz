#ifndef PRIMITIVE_TYPE_HPP
#define PRIMITIVE_TYPE_HPP

namespace Noz {

template<typename primitive_type>
struct get_primitive_value_impl {
    static const primitive_type & get(const primitive_type & value) { return value; }
    static       primitive_type & get(      primitive_type & value) { return value; }
};
template<typename primitive_type>
struct get_primitive_value_impl<let<primitive_type>> {
    static const primitive_type & get(const let<primitive_type> & instance) { return instance.value; }
    static       primitive_type & get(      let<primitive_type> & instance) { return instance.value; }
};
template<typename type>
auto get_primitive_value(type && value) {
    return get_primitive_value_impl<primitive_type_t<std::decay_t<type>>>::get(value);
}

}

#endif // PRIMITIVE_TYPE_HPP
