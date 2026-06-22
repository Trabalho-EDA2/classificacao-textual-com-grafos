#ifndef QUEUE_HPP
#define QUEUE_HPP

struct queue_node
{
    int data;
    queue_node *prox;
};

class Queue
{
    queue_node *head;
    queue_node *tail;

public:
    Queue();
    ~Queue();
    void enqueue(int value);
    int dequeue();
    bool is_empty() const;
};

#endif
