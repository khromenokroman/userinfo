# userinfo

Небольшая C++20-библиотека для получения информации о текущем пользователе Linux:
имя и идентификаторы, номер терминала, IP-адрес и время последнего входа.

## Требования

| Компонент | Версия |
|---|---|
| CMake | ≥ 3.20 |
| GCC / Clang | с поддержкой C++20 |
| Linux | `/var/log/lastlog` должен существовать |

## Структура проекта

```
├── CMakeLists.txt
├── cmake/
│   └── userinfoConfig.cmake.in
└── src/
    ├── lib/                         — библиотека
    │   ├── CMakeLists.txt
    │   ├── include/
    │   │   └── userinfo/
    │   │       └── userinfo.hpp
    │   └── userinfo.cpp
    └── app/                         — пример использования
        ├── CMakeLists.txt
        └── main.cpp
```

## Сборка

### Только библиотека

```bash
cmake -S . -B build -DUSERINFO_BUILD_APP=OFF
cmake --build build
```

### Библиотека и пример

```bash
cmake -S . -B build
cmake --build build
./build/src/app/userinfo_example
```

### Установка в систему

```bash
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build
cmake --install build
```

После установки библиотека доступна через `find_package(userinfo)`.

## API

Заголовочный файл: `<userinfo/userinfo.hpp>`, пространство имён `userinfo`.

---

### `current_user()`

```cpp
userinfo::CurrentUser current_user();
```

Возвращает информацию о текущем пользователе. Бросает `std::runtime_error` если данные недоступны.

```cpp
struct CurrentUser {
    std::string m_name;          // логин
    uid_t       m_uid;           // идентификатор пользователя
    gid_t       m_gid;           // идентификатор группы
    std::string m_home;          // домашний каталог
    std::string m_shell;         // оболочка
    std::string m_display_name;  // поле gecos (полное имя)
};
```

---

### `current_terminal()`

```cpp
std::optional<userinfo::Terminal> current_terminal();
```

Возвращает `nullopt` если процесс не связан с терминалом.

```cpp
struct Terminal {
    std::string m_full_name;   // /dev/pts/0
    std::string m_short_name;  // pts/0
    int         m_number;      // 0  (или -1 если не определён)
};
```

---

### `last_login()`

```cpp
std::optional<userinfo::LastLogin> last_login();
```

Читает `/var/log/lastlog`. Возвращает `nullopt` если файл недоступен или пользователь ещё не входил в систему.

```cpp
struct LastLogin {
    std::string m_host;      // IP или имя хоста (пусто — локальный вход)
    std::time_t m_time;      // unix-время входа
    std::string m_terminal;  // терминал входа
};
```

## Подключение в другом проекте

### Вариант 1 — `add_subdirectory`

Скопируйте или склонируйте репозиторий в свой проект:

```cmake
add_subdirectory(path/to/userinfo)
target_link_libraries(my_target PRIVATE userinfo::userinfo)
```

При таком подключении пример (`userinfo_example`) не собирается автоматически.

### Вариант 2 — FetchContent

```cmake
include(FetchContent)

FetchContent_Declare(
    userinfo
    GIT_REPOSITORY https://github.com/khromenokroman/userinfo.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(userinfo)

target_link_libraries(my_target PRIVATE userinfo::userinfo)
```

### Вариант 3 — `find_package` (после установки)

```cmake
find_package(userinfo REQUIRED)
target_link_libraries(my_target PRIVATE userinfo::userinfo)
```

## Пример

```cpp
#include <userinfo/userinfo.hpp>
#include <ctime>
#include <iostream>

int main() {
    auto user = userinfo::current_user();
    std::cout << "Пользователь: " << user.m_name
              << "  uid=" << user.m_uid << "\n";

    if (auto term = userinfo::current_terminal())
        std::cout << "Терминал: " << term->m_full_name << "\n";

    if (auto login = userinfo::last_login()) {
        char buf[32];
        std::tm* tm = std::localtime(&login->m_time);
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
        std::cout << "Последний вход: "
                  << (login->m_host.empty() ? "локально" : login->m_host)
                  << " в " << buf << "\n";
    }
}
```
