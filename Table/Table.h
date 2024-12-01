#ifndef SHIP_TABLE_H
#define SHIP_TABLE_H

#include "Ship.h"
#include <stdexcept>

namespace acg {

    template <class T>
    class ShipTable {
    private:
        static const size_t DEFAULT_CAPACITY = 16; // начальный размер таблицы
        static const double LOAD_FACTOR;          // коэффициент загрузки для увеличения размера таблицы

        // **Вложенная структура HashNode**
        struct HashNode {
            std::string call_sign; // позывной корабля
            T* value;        // указатель на объект корабля
            HashNode* next;  // указатель на следующий узел в бакете

            HashNode(std::string call_sign, T* value) : call_sign(std::move(call_sign)), value(value), next(nullptr) {}
        };

        HashNode** buckets; // массив бакетов (указателей на узлы)
        size_t capacity;    // емкость таблицы (количество бакетов)
        size_t size;        // текущее количество элементов

        // Функция хэширования для получения индекса бакета
        [[nodiscard]] size_t hashFunction(const std::string& key) const {
            size_t hash = 0;
            for (char c : key) {
                hash = hash * 31 + c; // простое хэширование
            }
            return hash % capacity;
        }

        void rehash() {
            size_t oldCapacity = capacity;
            capacity *= 2;
            auto** newBuckets = new HashNode*[capacity](); // Новый массив бакетов

            for (size_t i = 0; i < oldCapacity; ++i) {
                HashNode* node = buckets[i];
                while (node != nullptr) {
                    HashNode* nextNode = node->next; // Сохраняем указатель на следующий узел
                    size_t newIndex = hashFunction(node->call_sign);
                    node->next = newBuckets[newIndex];
                    newBuckets[newIndex] = node; // Перемещаем узел в новый бакет
                    node = nextNode;
                }
            }

            delete[] buckets;
            buckets = newBuckets;
        }

    public:
        ShipTable() : capacity(DEFAULT_CAPACITY), size(0) {
            buckets = new HashNode*[capacity]();
        }

        ~ShipTable() {
            for (size_t i = 0; i < capacity; ++i) {
                HashNode* node = buckets[i];
                while (node != nullptr) {
                    HashNode* next = node->next;
                    delete node;
                    node = next;
                }
            }
            delete[] buckets;
        }

        void addShip(const std::string& call_sign, T* ship) {
            size_t index = hashFunction(call_sign);
            HashNode* node = buckets[index];

            while (node != nullptr) {
                if (node->call_sign == call_sign) {
                    node->value = ship;
                    return;
                }
                node = node->next;
            }

            auto* newNode = new HashNode(call_sign, ship);
            newNode->next = buckets[index];
            buckets[index] = newNode;
            size++;

            if ((double)size / capacity > LOAD_FACTOR) {
                rehash();
            }
        }

        T* getShip(const std::string& call_sign) const {
            size_t index = hashFunction(call_sign);
            HashNode* node = buckets[index];

            while (node != nullptr) {
                if (node->call_sign == call_sign) {
                    return node->value;
                }
                node = node->next;
            }

            return nullptr;
        }

        [[nodiscard]] size_t getShipCount() const {
            return size;
        }

        void removeShip(const std::string& call_sign) {
            size_t index = hashFunction(call_sign);
            HashNode* node = buckets[index];
            HashNode* prev = nullptr;

            while (node != nullptr) {
                if (node->call_sign == call_sign) {
                    if (prev == nullptr) {
                        buckets[index] = node->next;
                    } else {
                        prev->next = node->next;
                    }
                    delete node;
                    size--;
                    return;
                }
                prev = node;
                node = node->next;
            }
        }

        class Iterator {
        private:
            HashNode** buckets;
            size_t capacity;
            size_t currentIndex;
            HashNode* currentNode;

        public:
            Iterator(HashNode** buckets, size_t capacity)
                    : buckets(buckets), capacity(capacity), currentIndex(0), currentNode(nullptr) {
                advanceToNext();
            }

            [[nodiscard]] bool hasNext() const {
                return currentNode != nullptr;
            }

            void next() {
                if (currentNode == nullptr && currentIndex >= capacity) {
                    throw std::out_of_range("No more elements in iterator");
                }
                if (currentNode != nullptr) {
                    currentNode = currentNode->next;
                }
                if (currentNode == nullptr) {
                    currentIndex++;
                    advanceToNext();
                }
                if (currentNode == nullptr && currentIndex >= capacity) {
                    throw std::out_of_range("No more elements in iterator");
                }
            }

            std::pair<std::string, T*> get() const {
                if (currentNode == nullptr) {
                    throw std::out_of_range("Iterator at end");
                }
                return {currentNode->call_sign, currentNode->value};
            }

        private:
            void advanceToNext() {
                while (currentIndex < capacity && buckets[currentIndex] == nullptr) {
                    currentIndex++;
                }
                currentNode = (currentIndex < capacity) ? buckets[currentIndex] : nullptr;
            }
        };

        Iterator getIterator() {
            return Iterator(buckets, capacity);
        }

    };

    template <typename T>
    const double ShipTable<T>::LOAD_FACTOR = 0.75;

} // namespace acg


#endif // SHIP_TABLE_H
