#pragma once
#include <stdexcept>
/**
 * @namespace acg
 * @brief Пространство имен для компонентов морского боя
 */
namespace acg {

    /**
     * @class ShipTable
     * @brief Шаблонный класс для хранения кораблей в хеш-таблице
     * @tparam T Тип данных корабля
     */
    template <class T>
    class ShipTable {
    private:

        /// @brief Размер таблицы по умолчанию
        constexpr static const size_t DEFAULT_CAPACITY = 16;

        /// @brief Коэффициент загрузки таблицы
        constexpr static const double LOAD_FACTOR = 0.75;

        /**
         * @struct HashNode
         * @brief Узел хеш-таблицы
         */
        struct HashNode {
            std::string call_sign;  ///< Позывной корабля
            T* value;              ///< Указатель на корабль
            HashNode* next;        ///< Указатель на следующий узел

            /**
             * @brief Конструктор узла
             * @param call_sign Позывной корабля
             * @param value Указатель на корабль
             */
            HashNode(std::string call_sign, T* value);
        };

        HashNode** buckets;     ///< Массив корзин хеш-таблицы
        size_t capacity;        ///< Текущая емкость таблицы
        size_t size;           ///< Текущий размер таблицы

        /**
         * @brief Вычисляет хеш для ключа
         * @param key Ключ для хеширования
         * @return Индекс корзины
         */
        [[nodiscard]] size_t hashFunction(const std::string& key) const;

        /**
         * @brief Перехеширует таблицу при достижении коэффициента загрузки
         */
        void rehash();

        /**
         * @brief Освобождает память, занятую корзинами
         */
        void freeBuckets();

    public:
        /**
         * @brief Конструктор по умолчанию
         */
        ShipTable();

        /**
         * @brief Деструктор
         */
        ~ShipTable();

        /**
         * @brief Добавляет корабль в таблицу
         * @param call_sign Позывной корабля
         * @param ship Указатель на корабль
         */
        void addShip(const std::string& call_sign, T* ship);

        /**
         * @brief Получает корабль по позывному
         * @param call_sign Позывной корабля
         * @return Указатель на корабль или nullptr
         */
        T* getShip(const std::string& call_sign) const;

        /**
         * @brief Возвращает количество кораблей в таблице
         * @return Количество кораблей
         */
        [[nodiscard]] size_t getShipCount() const;

        /**
         * @brief Удаляет корабль из таблицы
         * @param call_sign Позывной корабля
         */
        void removeShip(const std::string& call_sign);

        /**
         * @brief Очищает таблицу
         */
        void clear();

        /**
         * @brief Проверяет, пуста ли таблица
         * @return true если таблица пуста
         */
        [[nodiscard]] bool empty() const;

        /**
         * @class Iterator
         * @brief Forward Iterator для обхода хеш-таблицы
         */
        class Iterator {
        private:
            HashNode** buckets;     ///< Указатель на массив корзин
            size_t capacity;        ///< Емкость таблицы
            size_t currentIndex;    ///< Текущий индекс
            HashNode* currentNode;  ///< Текущий узел

        public:
            /**
             * @brief Конструктор по умолчанию для итератора
             * Инициализирует итератор с нулевыми значениями
             */
            Iterator() : buckets(nullptr), capacity(0), currentIndex(0), currentNode(nullptr) {}

            /**
             * @brief Определяет категорию итератора как forward_iterator
             * Позволяет использовать итератор в стандартных алгоритмах STL
             */
            using iterator_category = std::forward_iterator_tag;

            /**
             * @brief Определяет тип значения, возвращаемого итератором
             * Пара из строки (позывной) и указателя на корабль
             */
            using value_type = std::pair<std::string, T*>;

            /**
             * @brief Определяет тип для разницы между итераторами
             * Стандартный тип для представления разницы между указателями
             */
            using difference_type = std::ptrdiff_t;

            /**
             * @brief Определяет тип указателя на значение
             * Указатель на пару строка-корабль
             */
            using pointer = value_type*;

            /**
             * @brief Определяет тип ссылки на значение
             * Ссылка на пару строка-корабль
             */
            using reference = value_type&;

            /**
             * @brief Конструктор итератора
             * @param buckets Указатель на массив корзин
             * @param capacity Емкость таблицы
             */
            Iterator(HashNode** buckets, size_t capacity);

            /**
             * @brief Проверяет наличие следующего элемента
             * @return true если есть следующий элемент
             */
            [[nodiscard]] bool hasNext() const;

            /**
             * @brief Переходит к следующему элементу
             */
            void next();

            /**
             * @brief Получает текущий элемент
             * @return Пара из позывного и указателя на корабль
             */
            std::pair<std::string, T*> get() const;

            /**
             * @brief Префиксный инкремент итератора
             * @return Ссылка на обновленный итератор
             */
            Iterator& operator++();

            /**
             * @brief Постфиксный инкремент итератора
             * @param Фиктивный параметр для различения постфиксной версии
             * @return Копия итератора до инкремента
             */
            Iterator operator++(int);

            /**
             * @brief Оператор сравнения итераторов на равенство
             * @param other Другой итератор для сравнения
             * @return true если итераторы указывают на один элемент
             */
            bool operator==(const Iterator& other) const;

            /**
             * @brief Оператор сравнения итераторов на неравенство
             * @param other Другой итератор для сравнения
             * @return true если итераторы указывают на разные элементы
             */
            bool operator!=(const Iterator& other) const;

            /**
             * @brief Оператор разыменования итератора
             * @return Значение текущего элемента
             */
            value_type operator*() const;

        private:
            /**
             * @brief Перемещает итератор к следующему валидному элементу
             */
            void advanceToNext();
        };

        /**
         * @brief Создает итератор для таблицы
         * @return Объект итератора
         */
        Iterator getIterator();
    };

} // namespace acg

#include "Table.inl"