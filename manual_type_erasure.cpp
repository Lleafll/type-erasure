#include <string>
#include <string_view>
#include <polymorphic_value.h>

class Logger {
public:
    template<class T>
    Logger(T data) : object_{isocpp_p0201::make_polymorphic_value<Concept, Model < T>>(std::move(data))} {}

    void log(std::string_view const message) {
        object_->log(message);
    }

    [[nodiscard]] std::string const &content() const {
        return object_->content();
    }

    void clear() {
        object_->clear();
    }

private:
    struct Concept {
        virtual ~Concept() = default;

        virtual void log(std::string_view message) = 0;

        [[nodiscard]] virtual std::string const &content() const = 0;

        virtual void clear() = 0;
    };

    template<class T>
    struct Model final : Concept {
        explicit Model(T &&data) : data_{std::move(data)} {}

        ~Model() override = default;

        void log(std::string_view const message) override {
            data_.log(message);
        }

        [[nodiscard]] const std::string &content() const override {
            return data_.content();
        }

        void clear() override {
            data_.clear();
        }

    private:
        T data_;
    };

    isocpp_p0201::polymorphic_value<Concept> object_;
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
