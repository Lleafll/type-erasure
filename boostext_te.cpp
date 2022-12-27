#include <string>
#include <string_view>
#include <boost/te.hpp>

namespace te = boost::te;

class Logger final : public te::poly<Logger> {
public:
    using te::poly<Logger>::poly;

    void log(std::string_view const message) {
        te::call([](auto &self, auto const message) { self.log(message); }, *this, message);
    }

    [[nodiscard]] std::string const &content() const {
        return te::call<std::reference_wrapper<std::string const>>(
                [](auto const &self) -> std::reference_wrapper<std::string const> { return self.content(); },
                *this);
    }

    void clear() {
        te::call([](auto &self) { self.clear(); }, *this);
    }
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
    Logger logger;

    int execute() {
        logger.clear();
        logger.log("message");
        return static_cast<int>(std::ssize(logger.content()));
    }
};

int main() {
    return Program{StringLogger{}}.execute();
}
