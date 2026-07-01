#include "userinfo/userinfo.hpp"

#include <fcntl.h>
#include <lastlog.h>
#include <paths.h>
#include <pwd.h>
#include <unistd.h>

#include <cstring>
#include <stdexcept>
#include <string_view>

namespace userinfo {

CurrentUser current_user() {
    uid_t uid = getuid();
    struct passwd* pw = getpwuid(uid);
    if (!pw)
        throw std::runtime_error("getpwuid failed for uid " + std::to_string(uid));
    return CurrentUser{
        .m_name         = pw->pw_name,
        .m_home         = pw->pw_dir,
        .m_shell        = pw->pw_shell,
        .m_display_name = pw->pw_gecos,
        .m_uid          = pw->pw_uid,
        .m_gid          = pw->pw_gid,
    };
}

std::optional<Terminal> current_terminal() {
    const char* tty = ttyname(STDIN_FILENO);
    if (!tty) tty = ttyname(STDOUT_FILENO);
    if (!tty) tty = ttyname(STDERR_FILENO);
    if (!tty) return std::nullopt;

    std::string full(tty);

    constexpr std::string_view dev = "/dev/";
    std::string short_name = full.starts_with(dev) ? full.substr(dev.size()) : full;

    int number = -1;
    auto pos = short_name.find_last_not_of("0123456789");
    if (pos != std::string::npos && pos + 1 < short_name.size()) {
        try { number = std::stoi(short_name.substr(pos + 1)); } catch (...) {}
    }

    return Terminal{
        .m_full_name  = std::move(full),
        .m_short_name = std::move(short_name),
        .m_number     = number,
    };
}

std::optional<LastLogin> last_login() {
    uid_t uid = getuid();

    int fd = open(_PATH_LASTLOG, O_RDONLY);
    if (fd < 0) return std::nullopt;

    struct lastlog ll{};
    off_t offset = static_cast<off_t>(uid) * static_cast<off_t>(sizeof(struct lastlog));

    if (lseek(fd, offset, SEEK_SET) < 0) {
        close(fd);
        return std::nullopt;
    }

    ssize_t n = read(fd, &ll, sizeof(ll));
    close(fd);

    if (n < static_cast<ssize_t>(sizeof(ll))) return std::nullopt;
    if (ll.ll_time == 0) return std::nullopt;

    return LastLogin{
        .m_host     = std::string(ll.ll_host, strnlen(ll.ll_host, sizeof(ll.ll_host))),
        .m_terminal = std::string(ll.ll_line, strnlen(ll.ll_line, sizeof(ll.ll_line))),
        .m_time     = static_cast<std::time_t>(ll.ll_time),
    };
}

} // namespace userinfo
