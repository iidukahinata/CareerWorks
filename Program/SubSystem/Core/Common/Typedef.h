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
#include <deque>
#include <stack>
#include <queue>
#include <string>
#include <vector>
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>

template<class T>
using List = std::list<T, std::allocator<T>>;

template<class T>
using Deque = std::deque<T, std::allocator<T>>;

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