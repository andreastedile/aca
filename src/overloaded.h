#ifndef ACA_OVERLOADED_H
#define ACA_OVERLOADED_H

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

#endif // ACA_OVERLOADED_H
