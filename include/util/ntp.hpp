//=======================================================================
// Copyright (c) 2015 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================
// The below code is modified

#ifndef NTP_HPP
#define NTP_HPP

#include <iostream>
#include <type_traits>

/*
 * NTP (named template parameter) implementation 
 */
namespace P {
using namespace std;

template <typename ID, typename T, T value>
struct value_conf_t : integral_constant<T, value> {
  using type_id = ID;
};

template <typename ID>
struct presence_conf_t {
  using type_id = ID;
};

template <typename ID, typename T>
struct type_conf_t {
  using type_id = ID;
  using type = T;
};

template <typename ID, template <typename...> class T>
struct template_type_conf_t {
  using type_id = ID;

  template <typename... C>
  using type = T<C...>;
};

/*
 * value template parameter
 */
template <typename D, typename... Args>
struct get_value;

template <typename D, typename... Args>
inline constexpr const auto get_v = get_value<D, Args...>::value;

template <typename D, typename T2, typename... Args>
struct get_value<D, T2, Args...> {
  template <typename D2, typename T22, typename Enable = void>
  struct impl : integral_constant<decltype(D::value), get_v<D, Args...>> {};

  template <typename D2, typename T22>
  struct impl<
      D2, T22,
      enable_if_t<is_same_v<typename D2::type_id, typename T22::type_id>>>
      : integral_constant<decltype(D::value), T22::value> {};

  static constexpr const auto value = impl<D, T2>::value;
};

template <typename D>
struct get_value<D> : integral_constant<decltype(D::value), D::value> {};

/*
 * type template parameter
 */
template <typename D, typename... Args>
struct get_type;

template <typename D, typename... Args>
using get_t = typename get_type<D, Args...>::type;

template <typename D, typename T2, typename... Args>
struct get_type<D, T2, Args...> {
  template <typename D2, typename T22, typename Enable = void>
  struct impl {
    using type = typename get_type<D, Args...>::type;
  };

  template <typename D2, typename T22>
  struct impl<
      D2, T22,
      enable_if_t<is_same_v<typename D2::type_id, typename T22::type_id>>> {
    using type = typename T22::type;
  };

  using type = typename impl<D, T2>::type;
};

template <typename D>
struct get_type<D> {
  using type = typename D::type;
};

/*
 * template template parameter
 */

template <typename D, typename... Args>
struct get_template_type;

template <typename D, typename T2, typename... Args>
struct get_template_type<D, T2, Args...> {
  template <typename D2, typename T22, typename Enable = void>
  struct impl {
    template <typename... C>
    using type = typename get_template_type<D, Args...>::template type<C...>;
  };

  template <typename D2, typename T22>
  struct impl<
      D2, T22,
      enable_if_t<is_same_v<typename D2::type_id, typename T22::type_id>>> {
    template <typename... C>
    using type = typename T22::template type<C...>;
  };

  template <typename... C>
  using type = typename impl<D, T2>::template type<C...>;
};

template <typename D>
struct get_template_type<D> {
  template <typename... C>
  using type = typename D::template type<C...>;
};

/*
 * Check template argument exists
 */
template <typename T1, typename... Args>
struct is_present;

template <typename... Args>
inline constexpr auto is_present_v = is_present<Args...>::value;

template <typename T1, typename T2, typename... Args>
struct is_present<T1, T2, Args...>
    : integral_constant<bool, is_same_v<T1, T2> || is_present_v<T1, Args...>> {
};

template <typename T1>
struct is_present<T1> : false_type {};

/*
 * Check template arguments fit the NTP listed
 */
template <typename... Valid>
struct tparam_list {  // template param list
  template <typename T>
  struct contains
      : integral_constant<bool, is_present_v<typename T::type_id, Valid...>> {};
};

template <typename L, typename... Args>
struct is_valid : true_type {};

template <typename L, typename... Args>
inline constexpr const auto is_valid_v = is_valid<L, Args...>::value;

template <typename L, typename T1, typename... Args>
struct is_valid<L, T1, Args...>
    : integral_constant<bool, L::template contains<T1>::value &&
                                  is_valid_v<L, Args...>> {};

/*
 * Declaration of policies
 */
#define NTP_POLICY_VALUE(policy_name, type) \
  struct policy_name##_ID;                  \
  template <type value>                     \
  struct policy_name : value_conf_t<policy_name##_ID, type, value> {}
#define NTP_POLICY_PRESENT(policy_name) \
  struct policy_name##_ID;              \
  struct policy_name : presence_conf_t<policy_name##_ID> {}
#define NTP_POLICY_TYPE(policy_name) \
  struct policy_name##_ID;           \
  template <typename T>              \
  struct policy_name : type_conf_t<policy_name##_ID, T> {}
#define NTP_POLICY_TEMPLATE(policy_name)    \
  struct policy_name##_ID;                  \
  template <template <typename...> class T> \
  struct policy_name : template_type_conf_t<policy_name##_ID, T> {};

/*
 * Extracting template arguments to policies
 *
 * Use in class definition
 */
#define NTP_VALUE(var_name, policy_name, default_arg) \
  static constexpr const auto var_name =              \
      get_v<policy_name<default_arg>, Args...>
#define NTP_PRESENT(var_name, policy_name) \
  static constexpr const auto var_name = is_present_v<policy_name, Args...>
#define NTP_TYPE(var_name, policy_name, default_type) \
  using var_name = get_t<policy_name<default_type>, Args...>
#define NTP_TEMPLATE(var_name, policy_name, default_template)                \
  template <typename... C>                                                   \
  using var_name = typename get_template_type<policy_name<default_template>, \
                                              Args...>::template type<C...>
#define NTP_VALIDATE(...)                                      \
  static_assert(is_valid_v<tparam_list<__VA_ARGS__>, Args...>, \
                "Invalid parameters type");

/*
 * Example usage
 *
 * using configurable_t2 = configurable<a<100>, b<'z'>, c<type::CCC>, d,
 * e<watcher_2>, h<trainer_2a>, h2<trainer_3a>>; cout << configurable_t2::A <<
 * endl; cout << configurable_t2::B << endl; cout <<
 * static_cast<size_t>(configurable_t2::C)<< endl; cout << configurable_t2::D <<
 * endl; cout << configurable_t2::E::name() << endl; cout <<
 * configurable_t2::template H<int>::name() << endl; cout <<
 * configurable_t2::template H2<int, int>::name() << endl;
 */

// struct a_id;
// struct b_id;
// struct c_id;
// struct d_id;
// struct e_id;
// struct h_id;
// struct h2_id;
// template <int value>
// struct a : value_conf_t<a_id, int, value> {};
//
// template <char value>
// struct b : value_conf_t<b_id, char, value> {};
//
// template <type value>
// struct c : value_conf_t<c_id, type, value> {};
// struct d : presence_conf_t<d_id> {};
// template <typename T>
// struct e : type_conf_t<e_id, T> {};
// template <template <typename...> class T>
// struct h : template_type_conf_t<h_id, T> {};
//
// template <template <typename...> class T>
// struct h2 : template_type_conf_t<h2_id, T> {};
//
// enum class type { AAA, BBB, CCC };
//
// struct watcher_1 {
//   static string name() { return "watcher_1"; }
// };
//
// struct watcher_2 {
//   static string name() { return "watcher_2"; }
// };
//
// template <typename C>
// struct trainer_1 {
//   static string name() { return "trainer_1"; }
// };
//
// template <typename C>
// struct trainer_2 {
//   static string name() { return "trainer_2"; }
// };
//
// template <typename C>
// struct trainer_2a {
//   static string name() { return "trainer_2a"; }
// };
//
// template <typename C, typename C2>
// struct trainer_3 {
//   static string name() { return "trainer_3"; }
// };
//
// template <typename C, typename C2>
// struct trainer_3a {
//   static string name() { return "trainer_3a"; }
// };
//
// template <typename... C>
// struct trainer_4 {
//   static string name() { return "trainer_4"; }
// };
//
// template <typename... C>
// struct trainer_4a {
//   static string name() { return "trainer_4a"; }
// };
//
// NTP_POLICY_VALUE(a, int);
// NTP_POLICY_VALUE(b, char);
// NTP_POLICY_VALUE(c, type);
// NTP_POLICY_PRESENT(d);
// NTP_POLICY_TYPE(e);
// NTP_POLICY_TEMPLATE(h);
// NTP_POLICY_TEMPLATE(h2);
//
// template <typename... Args>
// struct configurable {
//   NTP_VALUE(A, a, 1);
//   NTP_VALUE(B, b, 'b');
//   NTP_VALUE(C, c, type::BBB);
//   NTP_PRESENT(D, d);
//   NTP_TYPE(E, e, watcher_1);
//   NTP_TEMPLATE(H, h, trainer_1);
//   NTP_TEMPLATE(H2, h2, trainer_4);
//   NTP_VALIDATE(a_ID, b_ID, c_ID, d_ID, e_ID, h_ID, h2_ID);
//   // static constexpr const auto A = get_v<a<1>, Args...>;
//   // static constexpr const auto B = get_v<b<'b'>, Args...>;
//   // static constexpr const auto C = get_v<c<type::BBB>, Args...>;
//   // static constexpr const auto D = is_present_v<d, Args...>;
//   // using E = get_t<e<watcher_1>, Args...>;
//   // template <typename... C>
//   // using H = typename get_template_type<h<trainer_1>, Args...>::template
//   // type<C...>; template <typename... C> using H2 =
//   //     typename get_template_type<h2<trainer_4>, Args...>::template
//   //     type<C...>;
//   // static_assert(is_valid_v<tparam_list<a_id, b_id, c_id, d_id, e_id, h_id,
//   // h2_id>, Args...>, "Invalid parameters type");
// };

}  // namespace P

#endif /* NTP_HPP */
