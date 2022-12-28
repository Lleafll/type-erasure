#include <string>
#include <string_view>
#include <anyany.hpp>

template<class T>
struct Log {
    static void do_invoke(T &self, std::string_view const message) {
        self.log(message);
    }
};

template<class T>
struct Content {
    static std::string const &do_invoke(T const &self) {
        return self.content();
    }
};

template<class T>
struct Clear {
    static void do_invoke(T &self) {
        self.clear();
    }
};

using Logger = aa::any_with<Log, Content, Clear>;

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
        aa::invoke<Clear>(logger);
        aa::invoke<Log>(logger, "message");
        return static_cast<int>(std::ssize(aa::invoke<Content>(logger)));
    }
};

int main() {
    return Program{StringLogger{}}.execute();
}
