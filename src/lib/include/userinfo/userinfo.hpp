#pragma once

#include <ctime>
#include <optional>
#include <string>
#include <sys/types.h>

/// @file userinfo.hpp
/// @brief Получение информации о текущем пользователе, терминале и последнем входе.

namespace userinfo {

/// @brief Информация о текущем пользователе системы.
struct CurrentUser {
    std::string m_name;         ///< Логин пользователя.
    std::string m_home;         ///< Путь к домашнему каталогу.
    std::string m_shell;        ///< Путь к командной оболочке.
    std::string m_display_name; ///< Полное имя из поля gecos.
    uid_t       m_uid;          ///< Идентификатор пользователя (UID).
    gid_t       m_gid;          ///< Идентификатор группы (GID).
};

/// @brief Информация о терминале, связанном с процессом.
struct Terminal {
    std::string m_full_name;  ///< Полный путь к устройству, например @c /dev/pts/0.
    std::string m_short_name; ///< Короткое имя без префикса @c /dev/, например @c pts/0.
    int         m_number;     ///< Числовой номер терминала, или @c -1 если не определён.
};

/// @brief Данные о последнем входе пользователя в систему.
struct LastLogin {
    std::string m_host;     ///< IP-адрес или имя хоста. Пустая строка означает локальный вход.
    std::string m_terminal; ///< Имя терминала, с которого выполнялся вход.
    std::time_t m_time;     ///< Время входа в формате Unix timestamp.
};

/// @brief Возвращает информацию о текущем пользователе.
///
/// Использует @c getuid() и @c getpwuid() для получения записи из базы паролей.
///
/// @return Заполненная структура @ref CurrentUser.
/// @throws std::runtime_error Если запись пользователя недоступна.
CurrentUser current_user();

/// @brief Возвращает информацию о терминале текущего процесса.
///
/// Последовательно проверяет дескрипторы @c stdin, @c stdout, @c stderr
/// через @c ttyname(). Возвращает данные первого дескриптора, связанного с tty.
///
/// @return Структура @ref Terminal, или @c std::nullopt если ни один
///         из стандартных дескрипторов не связан с терминалом.
std::optional<Terminal> current_terminal();

/// @brief Возвращает данные о последнем входе текущего пользователя.
///
/// Читает запись из @c /var/log/lastlog по смещению, соответствующему UID.
///
/// @return Структура @ref LastLogin, или @c std::nullopt если файл недоступен
///         либо пользователь ещё не входил в систему.
std::optional<LastLogin> last_login();

} // namespace userinfo
