#include <string>
#include <string_view>
#include <dyn/dyn.h>

DYN_MAKE_INTERFACE(Logger,
                   (void, log, (std::string_view)),
                   (std::string const &, content, () const),
                   (void, clear, ()));

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
    dyn_Logger<> logger;

    int execute() {
        logger.clear();
        logger.log("message");
        return static_cast<int>(std::ssize(logger.content()));
    }
};

int main() {
    return Program{StringLogger{}}.execute();
}
