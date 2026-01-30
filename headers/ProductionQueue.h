#ifndef PRODUCTION_QUEUE_H
#define PRODUCTION_QUEUE_H

#include <vector>


template <typename T>
class ProductionQueue {
    std::vector<T> queue;

public:
    // Adăugare element la final
    void add(const T& item) {
        queue.push_back(item);
    }

    // Verificare dacă e goală
    bool isEmpty() const {
        return queue.empty();
    }

    // Acces la primul element
    T& front() {
        return queue.front();
    }
    
    const T& front() const {
        return queue.front();
    }

    // Eliminare primul element
    void removeFirst() {
        if (!queue.empty()) {
            queue.erase(queue.begin());
        }
    }

    // Returnează dimensiunea
    size_t size() const { return queue.size(); }

    // Metode pentru range-based for loops (iterare)
    auto begin() { return queue.begin(); }
    auto end() { return queue.end(); }
    auto begin() const { return queue.begin(); }
    auto end() const { return queue.end(); }
};

#endif