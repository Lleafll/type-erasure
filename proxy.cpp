#include <string>
#include <string_view>
#include <memory>
#include <proxy/proxy.h>

struct Log final : pro::dispatch<void(std::string_view)> {
    void operator()(auto &self, std::string_view const message) {
        self.log(message);
    }
};

struct Content final : pro::dispatch<std::string const &()> {
    std::string const &operator()(auto const &self) {
        return self.content();
    }
};

struct Clear final : pro::dispatch<void()> {
    void operator()(auto &self) {
        self.clear();
    }
};

struct LoggerFacade final : pro::facade<Log, Content, Clear> {
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
    pro::proxy<LoggerFacade> logger;

    int execute() {
        logger.invoke<Clear>();
        logger.invoke<Log>("message");
        return static_cast<int>(std::ssize(logger.invoke<Content>()));
    }
};

int main() {
    return Program{pro::make_proxy<LoggerFacade>(StringLogger{})}.execute();
}
