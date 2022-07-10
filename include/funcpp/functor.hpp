#pragma once

#include <optional>
#include <functional>

#ifndef __FUNCPP_FUNCTOR_HPP__
#define __FUNCPP_FUNCTOR_HPP__

#define _FUNCPP_MAPR(O, U, T) O<U>(O<T>)
#define _FUNCPP_WRAP(O, U, T) O<O<T>>

namespace funcpp {
    template<class FP> using func = std::function<FP>;

    template<class T>
    struct remove_cvref { typedef std::remove_cv_t<std::remove_reference_t<T>> type; };
    template< class T > using remove_cvref_t = typename remove_cvref<T>::type;

    template <typename T, typename = void>
    struct monad {
        using type = T;
        using origin = remove_cvref_t<T>;
        constexpr static bool value = false;
    };
    template <typename T> using monad_e = typename monad<T>::origin;
    template<typename T> [[maybe_unused]] constexpr static auto monad_v = monad<T>::value;

    template<typename T>
    struct monad<std::optional<T>> {
        using type = std::optional<T>;
        using origin = remove_cvref_t<T>;
        constexpr static bool value = true;

        template<typename U=T>
        constexpr static auto lift(func<U(T)> functor) -> func<_FUNCPP_MAPR(std::optional, U, T)> {
            return [functor](std::optional<T> opt_t) -> std::optional<U> {
                return opt_t ? std::optional<U>(functor(opt_t.value())) : std::nullopt;
            };
        }

        constexpr static auto flat(std::optional<T> opt2) -> origin {
            return opt2 ? opt2.value() : origin{};
        }
    };

    template<typename T>
    struct monad<std::vector<T>> {
        using type = std::vector<T>;
        using origin = remove_cvref_t<T>;
        constexpr static bool value = true;

        template<typename U=T>
        constexpr static auto lift(func<U(T)> functor) -> func<_FUNCPP_MAPR(std::vector, U, T)> {
            return [functor](std::vector<T> vec_t) -> std::vector<U> {
                std::vector<U> new_vec;
                new_vec.reserve(vec_t.size());
                for(auto& it : vec_t) { new_vec.push_back(functor(it)); }
                return new_vec;
            };
        }

        constexpr static auto flat(std::vector<T> vec)
            -> std::vector<monad_e<T>> {
            std::vector<monad_e<T>> result{};
            for(auto&it : vec) {for(auto& child : it) {result.push_back(child);}}
            return result;
        }
    };

    template<typename M, typename = void>
    class Monad {
        using OM = remove_cvref_t<M>;
    public:
        constexpr Monad(M m) : m_(m) {}
        auto to() -> OM { return m_; }
        operator OM() { return to(); }
    private:
        M m_;
    };

    
    template<typename M>
    class Monad<M, std::enable_if_t<monad_v<remove_cvref_t<M>>>> {
    public:
        using OM = remove_cvref_t<M>;
        using E  = monad_e<OM>;
        constexpr Monad(M m) : m_(m) {}

        template<typename U, typename T>
        auto lift(func<U(T)> functor) {
            using lift_t = remove_cvref_t<decltype(monad<OM>::lift(functor)(m_))>;
            return Monad<lift_t>(monad<OM>::lift(functor)(m_));
        }

        auto flat() { return Monad<E>(monad<OM>::flat(m_)); }

        template<typename U, typename T>
        auto flatlift(func<U(T)> functor) { return lift(functor).flat(); }

        auto to() -> OM { return m_; }
        operator OM() { return to(); }
    private:
        M m_;
    };

    template<typename M> Monad(M) -> Monad<M>;

    //template<class T, class U, class V, class Functor>
    //auto mlift2d(func<V(T,U)> f)  {
    //    return lift<T,V,Functor>([f](T t) {
    //        return lift<U,V,Functor>([f,t](U u){
    //            return f(t, u);
    //        });
    //    });
    //}

    //template<class Functor>
    //struct functor {
    //    template <class T, class U>
    //    auto _lift(func<U(T)> f) {
    //        return lift<T,U,Functor>(f);
    //    }

    //    template<class T, class U, class V>
    //    auto _lift2d(func<V(T,U)> f)  {
    //        return lift<T,V>([f](T t) {
    //            return lift<U,V,Functor>([f,t](U u){
    //                return f(t, u);
    //            });
    //        });
    //    }
    //};

} // namespace funcpp
#endif
