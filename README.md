# 🔍 C++ Search Server

Поисковый движок на C++ с поддержкой статуса документов, TF-IDF ранжирования, фильтрации, пагинации и учётом стоп-слов.

## 📑 Содержание

- [Возможности](#возможности)
- [Структура проекта](#структура-проекта)
- [Пример использования](#пример-использования)
- [Сборка](#сборка)
- [Поведение main.cpp](#поведение-maincpp)
- [Методы SearchServer](#методы-searchserver)
- [Компоненты](#компоненты)
- [Требования](#требования)

---

## Возможности

- Добавление документов с рейтингами и статусами
- Поиск с учётом минус-слов и стоп-слов
- Поддержка фильтрации: по статусу или произвольной функции
- Ранжирование результатов по TF-IDF + сортировка по рейтингу
- Учёт «пустых» запросов через RequestQueue
- Поддержка пагинации результата

---

## Структура проекта

| Файл | Назначение |
|------|------------|
| `main.cpp` | Демонстрация работы сервера |
| `search_server.h` | Класс `SearchServer` |
| `document.h` | Структура `Document`, перечисление `DocumentStatus` |
| `request_queue.h` | Очередь запросов и подсчёт неудачных |
| `paginator.h` | Пагинация (`Paginator`, `IteratorRange`) |
| `read_input_functions.h` | Ввод строки/числа |
| `string_processing.h` | Разделение текста на слова, обработка строк |

---

## Пример использования

```cpp
#include "search_server.h"

SearchServer server("and in at"s);
server.AddDocument(1, "big dog and fancy collar"s, DocumentStatus::ACTUAL, {8, 3, 7});
server.AddDocument(2, "fluffy cat with long tail"s, DocumentStatus::ACTUAL, {5, 5, 5});

for (const auto& doc : server.FindTopDocuments("cat -dog")) {
    std::cout << doc.id << ": " << doc.relevance << " (" << doc.rating << ")" << std::endl;
}
```

---

## Сборка

Требуется компилятор с поддержкой C++17 (g++ ≥ 9, clang++ ≥ 10, MSVC 2019+):

```bash
g++ -std=c++17 -o search_server main.cpp
./search_server
```

---

## Поведение `main.cpp`

- Создаёт `SearchServer` со стоп-словами
- Добавляет 5 документов
- Выполняет 1440+ запросов через `RequestQueue`, часть из них пустые
- Подсчитывает число неудачных запросов (результат: `Total empty requests: 1437`)

---

## Методы `SearchServer`

| Метод | Описание |
|-------|----------|
| `explicit SearchServer(const StringContainer& stop_words)` | Создаёт сервер с заданными стоп-словами |
| `void AddDocument(int id, const std::string& text, DocumentStatus status, const std::vector<int>& ratings)` | Добавляет документ |
| `FindTopDocuments(query)` | Поиск без фильтров |
| `FindTopDocuments(query, status)` | Поиск с фильтрацией по статусу |
| `FindTopDocuments(query, predicate)` | Поиск с предикатом |
| `MatchDocument(query, doc_id)` | Возвращает совпавшие плюс-слова и статус |
| `GetDocumentCount()` | Кол-во документов |
| `GetDocumentId(index)` | ID по индексу |

---

## Компоненты

- **`RequestQueue`**
  - Хранит результаты 1440 последних запросов
  - `GetNoResultRequests()` — сколько из них не дали результатов

- **`Paginator`**
  - Делит любой контейнер (например, `vector<Document>`) на страницы
  - Удобно использовать для вывода результатов порциями

- **`string_processing.h`**
  - `SplitIntoWords(text)` — разбивает строку на слова
  - `MakeUniqueNonEmptyStrings(container)` — фильтрует пустые строки и создаёт `set<string>`

---

## Требования

- C++17
- Компилятор: GCC, Clang, MSVC
- Без внешних библиотек

---

> Разработано в учебных целях: система индексирует документы и поддерживает фильтрацию/поиск с хорошей производительностью.