#include <string>
#include <string_view>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/mpl/vector.hpp>

namespace bte = boost::type_erasure;
namespace mpl = boost::mpl;

BOOST_TYPE_ERASURE_MEMBER(log);
BOOST_TYPE_ERASURE_MEMBER(content);
BOOST_TYPE_ERASURE_MEMBER(clear);

using Logger = bte::any<mpl::vector<
        bte::copy_constructible<>,
        has_log<void(std::string_view)>,
        has_content<std::string const &()>,
        has_clear<void()>>>;

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
