#include <string>
#include <string_view>
#include <folly/Poly.h>

class Logger {
public:
    template<class Base>
    struct Interface : Base {
        void log(std::string_view const message) {
            folly::poly_call<0>(*this, message);
        }

        [[nodiscard]] std::string const &content() const {
            return folly::poly_call<1>(*this);
        }

        void clear() {
            folly::poly_call<2>(*this);
        }
    };

    template<class T>
    using Members = folly::PolyMembers<&T::log, &T::content, &T::clear>;
};

class StringLogger {
public:
    void log(std::string_view const message) {
        content_.append(message);
        content_.push_back('\n');
    }

    [[nodiscard]] std::string const &content() const {
        return content_;
    }

    void clear() {
        content_.clear();
    }

private:
    std::string content_;
};

struct Program {
    folly::Poly<Logger> logger;

    int execute() {
        logger.clear();
        logger.log("message");
        return static_cast<int>(std::ssize(logger.content()));
    }
};

int main() {
    return Program{StringLogger{}}.execute();
}
