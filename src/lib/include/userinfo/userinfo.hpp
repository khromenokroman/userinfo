#pragma once

#include <ctime>
#include <optional>
#include <string>
#include <sys/types.h>

namespace userinfo {

struct CurrentUser {
    std::string m_name;
    uid_t       m_uid;
    gid_t       m_gid;
    std::string m_home;
    std::string m_shell;
    std::string m_display_name;
};

struct Terminal {
    std::string m_full_name;   // /dev/pts/0
    std::string m_short_name;  // pts/0
    int         m_number;      // 0, или -1 если не определён
};

struct LastLogin {
    std::string m_host;      // IP или имя хоста (пусто — локальный вход)
    std::time_t m_time;      // unix-время
    std::string m_terminal;  // терминал входа
};

// Бросает std::runtime_error если данные недоступны.
CurrentUser              current_user();

// nullopt если процесс не связан с терминалом.
std::optional<Terminal>  current_terminal();

// nullopt если /var/log/lastlog недоступен или пользователь ещё не входил.
std::optional<LastLogin> last_login();

} // namespace userinfo
