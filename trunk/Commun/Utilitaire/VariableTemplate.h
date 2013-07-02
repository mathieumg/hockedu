// Structure that can hold a variable number of attributes
// credit : http://stackoverflow.com/questions/672843/can-templates-be-used-to-access-struct-variables-by-name/672913#672913
// example
/*
struct age { typedef int value_type; };
struct last_name { typedef std::string value_type; };

ctmap<last_name, age> person;

person[last_name()] = "Smith";
person[age()] = 104;

template <class TMember>
void print_member(ctmap<last_name, age> &person)
{
    std::cout << person[TMember()] << std::endl;
}

print_member<age>(person);

*/
#pragma once

struct None { struct value_type {}; };

template <
    class T0 = None, class T1 = None,
    class T2 = None, class T3 = None,
    class T4 = None, class T5 = None,
    class T6 = None, class T7 = None
>
struct ctmap;

template <>
struct ctmap<
    None, None, None, None,
    None, None, None, None
>
{
    void operator[](const int &) {};
    static const int TemplateSize = 0;
};

template <
    class T0, class T1, class T2, class T3,
    class T4, class T5, class T6, class T7
>
struct ctmap : public ctmap<T1, T2, T3, T4, T5, T6, T7, None>
{
    typedef ctmap<T1, T2, T3, T4, T5, T6, T7, None> base_type;

    using base_type::operator[];
    typename T0::value_type storage;

    typename T0::value_type &operator[](const T0 &c)
    { return storage; }

    typename const T0::value_type &operator[](const T0 &c) const 
    { return storage; }

    static const int TemplateSize = base_type::TemplateSize+1;
    

};