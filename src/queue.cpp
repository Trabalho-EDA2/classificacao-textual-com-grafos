#include "queue.hpp"

Queue::Queue() : head(nullptr), tail(nullptr) {}

Queue::~Queue()
{
    while (!is_empty())
        dequeue();
}

void Queue::enqueue(int value)
{
    queue_node *new_node = new queue_node();
    new_node->data = value;
    new_node->prox = nullptr;

    if (tail == nullptr)
        head = tail = new_node;
    else
    {
        tail->prox = new_node;
        tail = new_node;
    }
}

int Queue::dequeue()
{
    if (head == nullptr)
        return -1;

    queue_node *removed = head;
    int value = removed->data;
    head = head->prox;

    if (head == nullptr)
        tail = nullptr;

    delete removed;
    return value;
}

bool Queue::is_empty() const
{
    return head == nullptr;
}
