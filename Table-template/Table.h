#ifndef SHIP_TABLE_H
#define SHIP_TABLE_H

#include <stdexcept>
#include <cassert> // TODO Потом убрать

namespace acg {

    template <class T>
    class ShipTable {
    private:
        constexpr static const size_t DEFAULT_CAPACITY = 16;
        constexpr static const double LOAD_FACTOR = 0.75;

        struct HashNode {
            std::string call_sign;
            T* value;
            HashNode* next;

            HashNode(std::string call_sign, T* value);
        };

        HashNode** buckets;
        size_t capacity;
        size_t size;

        [[nodiscard]] size_t hashFunction(const std::string& key) const;
        void rehash();

    public:
        ShipTable();
        ~ShipTable();

        void addShip(const std::string& call_sign, T* ship);
        T* getShip(const std::string& call_sign) const;
        [[nodiscard]] size_t getShipCount() const;
        void removeShip(const std::string& call_sign);

        class Iterator {
        private:
            HashNode** buckets;
            size_t capacity;
            size_t currentIndex;
            HashNode* currentNode;

        public:
            Iterator(HashNode** buckets, size_t capacity);
            [[nodiscard]] bool hasNext() const;
            void next();
            std::pair<std::string, T*> get() const;

        private:
            void advanceToNext();
        }; // class Iterator

        Iterator getIterator();
    }; // class ShipTable

} // namespace acg

#include "Table.inl"

#endif // SHIP_TABLE_H