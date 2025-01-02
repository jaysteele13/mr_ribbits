template <typename T, size_t Size>
class SimpleQueue {
  private:
    T data[Size];         // Array to store queue elements
    size_t front = 0;     // Index of the front element
    size_t rear = 0;      // Index of the next insertion
    size_t count = 0;     // Number of elements in the queue

  public:
    bool isEmpty() const {
        return count == 0;
    }

    bool isFull() const {
        return count == Size;
    }

    bool enqueue(const T& item) {
        if (isFull()) return false; // Queue is full

        data[rear] = item;
        rear = (rear + 1) % Size;
        ++count;
        return true;
    }

    bool dequeue(T& item) {
        if (isEmpty()) return false; // Queue is empty

        item = data[front];
        front = (front + 1) % Size;
        --count;
        return true;
    }

    size_t size() const {
        return count;
    }
};
