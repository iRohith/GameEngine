#pragma once

#pragma warning( push, 0 )
#define FMT_HEADER_ONLY
#include "../ThirdParty/fmt/format.h"
#include "../ThirdParty/fmt/ranges.h"
#include "../ThirdParty/fmt/chrono.h"
#include "../ThirdParty/fmt/color.h"
#pragma warning( pop )
#include <filesystem>
#include <string_view>

#define ToLambda(fun, ...) [&](){ fun(__VA_ARGS__); }

template<typename S, typename ...Args> inline auto format_str(const S& str, Args&&... args){
	return fmt::format(str, std::forward<Args>(args)...);
}
template<typename S> inline auto format_str(const S& s){
	return fmt::format("{}", s);
}


template<typename T, typename F> using UnaryFunRetType = decltype((std::declval<F>())(std::declval<T>()));

namespace GameEngine {
	using Runnable = std::function<void()>;

	template<typename _Runnable, typename... Args>
	static inline Runnable* CreateRunnable(_Runnable r, Args&&... args) {
		return new Runnable([&]() { r(std::forward<Args>(args)...); });
	}

	template<class T> class AnyTemp {
	private:
		T ptr;
	public:
		inline constexpr AnyTemp(T& t) : ptr(t) {}
		inline constexpr AnyTemp(T&& t) : ptr(t) {}

		template<typename F, std::enable_if_t<!std::is_same<void, decltype(std::declval<F>()(ptr))>::value, bool> = true>
		inline constexpr auto apply(const F& f) -> AnyTemp<decltype(f(ptr))>{
			return std::move(f(ptr));
		}

		template<typename F, std::enable_if_t<std::is_same<void, decltype(std::declval<F>()(ptr))>::value, bool> = true>
		inline constexpr AnyTemp<T>& apply(const F& f){
			f(ptr);
			return *this;
		}

		inline constexpr operator T&(){ return ptr; }
		inline constexpr T& operator()(){ return ptr; }
		inline constexpr T& operator* (){ return ptr; }
		inline constexpr T* operator-> (){ return &ptr; }
	};

	template<class T> class Any {
	private:
		T* ptr;
	public:
		inline constexpr Any(T& t) : ptr(&t) {}
		inline constexpr Any(T&& t) : ptr(&t) {}

		template<typename F, std::enable_if_t<!std::is_same<void, decltype(std::declval<F>()(*ptr))>::value, bool> = true>
		inline constexpr auto apply(const F& f) -> AnyTemp<decltype(f(*ptr))>{
			return std::move(f(*ptr));
		}

		template<typename F, std::enable_if_t<std::is_same<void, decltype(std::declval<F>()(*ptr))>::value, bool> = true>
		inline constexpr Any<T>& apply(const F& f){
			f(*ptr);
			return *this;
		}

		inline constexpr  operator T& (){ return *ptr; }
		inline constexpr T& operator()(){ return *ptr; }
		inline constexpr T& operator* (){ return *ptr; }
		inline constexpr T* operator->(){ return  ptr; }
	};
}

template<class T> GameEngine::Any<T> any(T& t) { return t; }
template<class T> GameEngine::Any<T> any(T&& t) { return t; }
template<class T> GameEngine::Any<T*> any(T*& t) { return t; }
template<class T> GameEngine::Any<T*> any(T*&& t) { return t; }

#define _apply_(name, code) apply([](auto& name){ code; })

#define with(X, code) [&](){ auto&& P = X; struct WITH : std::decay<decltype(X)>::type { inline void operator()() { code; }}; static_cast<WITH&>(P)(); return P; }()
#define run(X, code) [&](){ struct WITH : std::decay<decltype(X)>::type { inline auto operator()() { code; }}; return static_cast<WITH&>(X)(); }()


#define __M1(zero, a1, macro, ...) macro

#define __lambda2(name) [&](auto&& name)
#define __lambda1 __lambda2(it)
#define lambda(...) __M1(0, ##__VA_ARGS__, __lambda2(__VA_ARGS__), __lambda1)


template<bool, typename T> struct _MakeSignedSafe_Internal {};
template<typename T> struct _MakeSignedSafe_Internal<true, T> { using type = typename std::make_signed<T>::type; };
template<typename T> struct _MakeSignedSafe_Internal<false, T> { using type = T; };
template<typename T> using MakeSignedSafe = typename _MakeSignedSafe_Internal<std::is_unsigned<T>::value, T>::type;

template<bool, typename T> struct _MakeUnSignedSafe_Internal {};
template<typename T> struct _MakeUnSignedSafe_Internal<true, T> { using type = typename std::make_unsigned<T>::type; };
template<typename T> struct _MakeUnSignedSafe_Internal<false, T> { using type = T; };
template<typename T> using MakeUnSignedSafe = typename _MakeSignedSafe_Internal<!std::is_floating_point<T>::value && std::is_signed<T>::value, T>::type;

template<typename T1, typename T2, bool first> struct select_if;
template<typename T1, typename T2> struct select_if<T1, T2, true> { using type = T1; };
template<typename T1, typename T2> struct select_if<T1, T2, false> { using type = T2; };

template<typename T, typename F> using UnaryFunRetType = decltype((std::declval<F>())(std::declval<T>()));

namespace GameEngine {
	inline std::string asset_file(const std::string_view& str) {
		auto pth = std::filesystem::current_path();
		pth += "/assets/";
		pth += str;
		return pth;
	}
}