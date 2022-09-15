/**
* @file  Typedeg.h
* @brief 
*
* @date	 2022/08/02 2022îNìxèâî≈
*/
#pragma once


#include <set>
#include <map>
#include <list>
#include <stack>
#include <queue>
#include <string>
#include <vector>
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>

using String = std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

using Wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;

#ifdef __cpp_lib_char8_t
using u8String = std::basic_string<char8_t, std::char_traits<char8_t>, std::allocator<char8_t>>;
#endif // __cpp_lib_char8_t

template<class T>
using List = std::list<T, std::allocator<T>>;

template<class T>
using Stack = std::stack<T, std::deque<T, std::allocator<T>>>;

template<class T>
using Queue = std::queue<T, std::deque<T, std::allocator<T>>>;

template<class T>
using Vector = std::vector<T, std::allocator<T>>;

template<class T, class U>
using Map = std::map<T, U, std::less<T>, std::allocator<std::pair<const T, U>>>;

template<class T, class U>
using Unordered_Map = std::unordered_map<T, U, std::hash<T>, std::equal_to<T>, std::allocator<std::pair<const T, U>>>;

template<class T>
using Set = std::set<T, std::less<T>, std::allocator<T>>;

template<class T>
using Unordered_Set = std::unordered_set<T, std::hash<T>, std::equal_to<T>, std::allocator<T>>;