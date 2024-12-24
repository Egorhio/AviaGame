// ShipTable inline

#include "Table.h"

namespace acg {

    template<class T>
    ShipTable<T>::HashNode::HashNode(std::string call_sign, T *value)
    : call_sign(std::move(call_sign)), value(value), next(nullptr) {}

    template <typename T>
    ShipTable<T>::ShipTable() : capacity(DEFAULT_CAPACITY), size(0) {
        buckets = new HashNode*[capacity]();
    }

    template <typename T>
    ShipTable<T>::~ShipTable() {
        for (size_t i = 0; i < capacity; ++i) {
            HashNode* node = buckets[i];
            while (node != nullptr) {
                HashNode* next = node->next;
                delete node->value;
                delete node;
                node = next;
            }
        }
        delete[] buckets;
    }

    template <typename T>
    size_t ShipTable<T>::hashFunction(const std::string& key) const {
        std::hash<std::string> hash_fn;
        return hash_fn(key) % capacity;
    }

    template <typename T>
    void ShipTable<T>::rehash() {
        size_t oldCapacity = capacity;
        capacity *= 2;
        auto** newBuckets = new HashNode*[capacity]();

        for (size_t i = 0; i < oldCapacity; ++i) {
            HashNode* node = buckets[i];
            while (node != nullptr) {
                HashNode* nextNode = node->next;
                size_t newIndex = hashFunction(node->call_sign);
                node->next = newBuckets[newIndex];
                newBuckets[newIndex] = node;
                node = nextNode;
            }
        }

        delete[] buckets;
        buckets = newBuckets;
    }

    template <typename T>
    void ShipTable<T>::addShip(const std::string& call_sign, T* ship) {
        size_t index = hashFunction(call_sign);
        HashNode* node = buckets[index];

        while (node != nullptr) {
            if (node->call_sign == call_sign) {
                delete node->value;
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

    template <typename T>
    T* ShipTable<T>::getShip(const std::string& call_sign) const {
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

    template <typename T>
    void ShipTable<T>::removeShip(const std::string& call_sign) {
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
                delete node->value;
                delete node;
                size--;
                return;
            }
            prev = node;
            node = node->next;
        }
    }

    template <typename T>
    size_t ShipTable<T>::getShipCount() const {
        return size;
    }


    template<class T>
    ShipTable<T>::Iterator::Iterator(ShipTable::HashNode **buckets, size_t capacity)
            : buckets(buckets), capacity(capacity), currentIndex(0), currentNode(nullptr)
            {
        advanceToNext();
    }

    template<class T>
    void ShipTable<T>::Iterator::advanceToNext() {
        while (currentIndex < capacity && buckets[currentIndex] == nullptr) {
            currentIndex++;
        }
        currentNode = (currentIndex < capacity) ? buckets[currentIndex] : nullptr;
    }

    template<class T>
    std::pair<std::string, T*> ShipTable<T>::Iterator::get() const  {
        if (currentNode == nullptr) {
            throw std::out_of_range("Iterator at end");
        }
        return {currentNode->call_sign, currentNode->value};
    }

    template<class T>
    void ShipTable<T>::Iterator::next() {
        currentNode = currentNode->next;
        if (currentNode == nullptr) {
            currentIndex++;
            advanceToNext();
        }
    }

    template<class T>
    bool ShipTable<T>::Iterator::hasNext() const {
        return currentNode != nullptr;
    }

    template<class T>
    ShipTable<T>::Iterator ShipTable<T>::getIterator() {
        return Iterator(buckets, capacity);
    }

    template <typename T>
    void ShipTable<T>::freeBuckets() {
        for (size_t i = 0; i < capacity; ++i) {
            HashNode* node = buckets[i];
            while (node != nullptr) {
                HashNode* next = node->next;
                delete node->value;
                delete node;
                node = next;
            }
            buckets[i] = nullptr;
        }
        size = 0;
    }

    template <typename T>
    void ShipTable<T>::clear() {
        freeBuckets();
    }

    template <typename T>
    bool ShipTable<T>::empty() const {
        return size == 0;
    }

} // namespace acg