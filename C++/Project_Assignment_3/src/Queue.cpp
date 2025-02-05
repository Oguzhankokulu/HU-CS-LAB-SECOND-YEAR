#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    // TODO: Your code here

    front = -1;
    rear = -1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // TODO: Your code here
    // Implement circular structure 
    // Add the province

    if(front == ((rear+1)% MAX_QUEUE_SIZE)){
        return; //queue full
    }
    rear=(rear+1)% MAX_QUEUE_SIZE;
    if(front==-1)
        front++;
    data[rear]=province;
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    // TODO: Your code here
    if(front == -1)
        return -1; //empty
    if(front == rear){
        int temp = data[front];
        front = rear = -1;
        return temp;
    }
    int x = data[front];
    front=(front+1)% MAX_QUEUE_SIZE;
    return x;
}

// Returns the front province without removing it
int Queue::peek() const {
    // TODO: Your code here
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    // TODO: Your code here
    return front == -1;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    // TODO: Your code here

    if(front == ((rear+1)% MAX_QUEUE_SIZE)){
        return; //queue full
    }
    if(front==-1) {
        front++;
        rear++;
    } else if (front == 0) {
        front = MAX_QUEUE_SIZE - 1;
    } else {
        front -= 1;
    }

    data[front] = province;
    
}

bool Queue::isInQueue(int province) const {
    int frontNum = front;
    bool isFound = false;
    while (frontNum != rear + 1) {
        if (data[frontNum] == province) {
            isFound = true;
        }
        frontNum = (frontNum + 1) % MAX_QUEUE_SIZE;
    }
    return isFound;
}