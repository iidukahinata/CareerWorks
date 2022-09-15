/**
* @file    Memory.h
* @brief
*
* @date	   2022/08/02 2022”N“x‰”Å
*/
#pragma once


#include <memory>

template<class T>
using UniquePtr = std::unique_ptr<T>;

template<class T>
using SharedPtr = std::shared_ptr<T>;

template<class T>
using WeakPtr = std::weak_ptr <T>;