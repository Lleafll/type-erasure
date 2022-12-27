#include <string>
#include <string_view>
#include <dyno.hpp>

DYNO_INTERFACE(Logger,
               (log, void(std::string_view)),
               (content, std::string const&() const),
               (clear, void())
);

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
