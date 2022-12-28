#include <string>
#include <string_view>
#include <entt/poly/poly.hpp>

class Logger : public entt::type_list<> {
public:
    template<typename Base>
    struct type : Base {
        void log(std::string_view const message) { this->template invoke<0>(*this, message); }

        [[nodiscard]] std::string const &content() const { return this->template invoke<1>(*this); }

        void clear() { this->template invoke<2>(*this); }
    };

    template<typename Type>
    using impl = entt::value_list<&Type::log, &Type::content, &Type::clear>;
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
    entt::poly<Logger> logger;

    int execute() {
        logger->clear();
        logger->log("message");
        return static_cast<int>(std::ssize(logger->content()));
    }
};

int main() {
    return Program{StringLogger{}}.execute();
}
