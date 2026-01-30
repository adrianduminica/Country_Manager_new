#ifndef UTILS_H
#define UTILS_H

namespace GameUtils {
    template<typename T>
    T ensureRange(T value, T min, T max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
}

#endif
