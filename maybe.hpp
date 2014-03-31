#ifndef MAYBE_MAYBE_HPP
#define MAYBE_MAYBE_HPP

#include <exception>
#include <iostream>

class MaybeException : public std::exception {
    virtual const char* what() const throw() {
        return "Attempted to get value from None";

    } // virtual const char* what() const throw();

}; // class MaybeException : public std::exception;

template<typename T>
class Maybe;

template<typename T>
Maybe<T> Just(T data);

template<typename T>
Maybe<T> None();

template<typename T>
class Maybe {
public:
    template<typename _T>
    friend Maybe<_T> Just(_T data);

    template<typename _T>
    friend Maybe<_T> None();

    T get() {
        if(none) {
            throw MaybeException();

        } // if(none);

        return data;

    } // T get();

    bool isNone() { return none; }
    explicit operator bool() { return !none; }

    template<typename F>
    auto operator>>(F f) -> Maybe<decltype(f(this->get()))> {
        return none ? None<decltype(f(data))>() : Just(f(data));

    } // auto operator>>(F f) -> Maybe<decltype(f(this->get()))>;

    template<typename F>
    void call(F f) {
        if(!none) { f(data); }
    }

    template<typename F1, typename F2>
    void callElse(F1 f1, F2 f2) {
        if(none) { f2(); } else { f1(data); }
    }

private:
    T data;
    bool none;

    Maybe() : none(true) {};
    Maybe(T data) : data(data), none(false) {};

}; // class Maybe;

template<typename T>
Maybe<T> Just(T data) {
    return Maybe<T>(data);

} // Maybe<T> Just(T data);

template<typename T>
Maybe<T> None() {
    return Maybe<T>();

} // Maybe<T> None();

template<typename T>
std::ostream& operator<<( std::ostream& o, Maybe<T> maybe) {
    if(maybe) {
        return o << maybe.get();

    } // if(maybe);

    return o << "None";

} // std::ostream& operator<<( std::ostream& o, Maybe<T> maybe);

#endif // MAYBE_MAYBE_HPP
