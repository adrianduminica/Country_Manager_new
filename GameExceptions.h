#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class AssetLoadException : public GameException {
public:
    explicit AssetLoadException(const std::string& msg)
        : GameException(msg) {}
};

class InvalidProvinceIndexException : public GameException {
    int invalidIndex;
public:
    InvalidProvinceIndexException(const std::string& msg, int index)
        : GameException(msg + " index=" + std::to_string(index)),
          invalidIndex(index) {}

    int getIndex() const { return invalidIndex; }
};

class InvalidConstructionCountException : public GameException {
    int invalidCount;
public:
    InvalidConstructionCountException(const std::string& msg, int count)
        : GameException(msg + " count=" + std::to_string(count)),
          invalidCount(count) {}

    int getCount() const { return invalidCount; }
};

#endif
