#pragma once

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

bool should_merge(float detail_threshold, const RGB<float>& std) {
    return std.r <= detail_threshold &&
           std.g <= detail_threshold &&
           std.b <= detail_threshold;
}
