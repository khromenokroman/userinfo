#include <userinfo/userinfo.hpp>

#include <ctime>
#include <iostream>

int main() {
    try {
        auto user = userinfo::current_user();
        std::cout << "Пользователь    : " << user.m_name << "\n";
        std::cout << "UID             : " << user.m_uid << "\n";
        std::cout << "GID             : " << user.m_gid << "\n";
        std::cout << "Домашний каталог: " << user.m_home << "\n";
        std::cout << "Оболочка        : " << user.m_shell << "\n";
        if (!user.m_display_name.empty())
            std::cout << "Имя             : " << user.m_display_name << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }

    std::cout << "\n";

    if (auto term = userinfo::current_terminal()) {
        std::cout << "Терминал        : " << term->m_full_name << "\n";
        if (term->m_number >= 0)
            std::cout << "Номер           : " << term->m_number << "\n";
    } else {
        std::cout << "Терминал        : нет tty\n";
    }

    std::cout << "\n";

    if (auto login = userinfo::last_login()) {
        char buf[64];
        std::tm* tm = std::localtime(&login->m_time);
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
        std::cout << "Последний вход\n";
        std::cout << "  IP / хост     : " << (login->m_host.empty() ? "(локально)" : login->m_host) << "\n";
        std::cout << "  Время         : " << buf << "\n";
        std::cout << "  Терминал      : " << login->m_terminal << "\n";
    } else {
        std::cout << "Данные о последнем входе недоступны\n";
    }

    return 0;
}
