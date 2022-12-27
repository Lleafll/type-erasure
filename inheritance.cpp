#include <string>
#include <string_view>
#include <polymorphic_value.h>

class Logger {
public:
    virtual ~Logger() = default;

    virtual void log(std::string_view message) = 0;

    [[nodiscard]] virtual std::string const &content() const = 0;

    virtual void clear() = 0;
};

class StringLogger final : public Logger {
public:
    ~StringLogger() override = default;

    void log(std::string_view const message) override {
        content_.append(message);
        content_.push_back('\n');
    }

    [[nodiscard]] std::string const &content() const override {
        return content_;
    }

    void clear() override {
        content_.clear();
    }

private:
    std::string content_;
};

struct Program {
    isocpp_p0201::polymorphic_value<Logger> logger;

    int execute() {
        logger->clear();
        logger->log("message");
        return static_cast<int>(std::ssize(logger->content()));
    }
};

int main() {
    return Program{isocpp_p0201::make_polymorphic_value<Logger, StringLogger>()}.execute();
}
