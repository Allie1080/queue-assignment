#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>

const std::chrono::duration<double> timeLimit{10.0};

void delayFor (int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

class Person {
private:
    std::string name;
    int ticketNumber;
    // starts at 0, but display() starts at 1
    std::chrono::time_point<std::chrono::system_clock> ticketCreationTime;

public:
    
    Person () {};

    Person (std::string nameInput, int ticketNumberInput) {
        name = nameInput;
        ticketNumber = ticketNumberInput;
        ticketCreationTime = std::chrono::system_clock::now();

    }

    bool isDue () {
        return (std::chrono::system_clock::now() - ticketCreationTime) >= timeLimit;
    }

    std::string getName () {
        return name;
    }

    int getRawTicketNumber () {
        return ticketNumber;
    }

    std::chrono::time_point<std::chrono::system_clock> getTicketCreationTime () {
        return ticketCreationTime;
    }

    std::string getFormattedTicketNumber () {
        std::ostringstream ticket;
        int ticketNumberDigits{0};
        int ticketNumberCopy = ticketNumber + 1;

        while (ticketNumberCopy != 0) {
            ticketNumberDigits++;
            ticketNumberCopy /= 10;
        }

        ticket << "(Ticket #";
        
        if (ticketNumberDigits == 1) {
            ticket << "00" << (ticketNumber + 1);

        } else if (ticketNumberDigits == 2) {
            ticket << "0" << (ticketNumber + 1);

        } else if (ticketNumberDigits == 1) {
            ticket << (ticketNumber + 1);

        }

        ticket << ")";

        return ticket.str();

    }

    std::string getTicket () {
        // sample output: "Alice (#001)"
        std::ostringstream personInformation;

        personInformation << name << " " << getFormattedTicketNumber();

        return personInformation.str();
    }
    
};

class Queue {
private:
    const int front{0};
    int rear;
    int capacity;
    
    Person* queue;

    int lastTicketNumber;

public:
    Queue (int cap) {
        rear  = -1;
        capacity = cap;

        queue = new Person[capacity];

        lastTicketNumber = 0;
    }   
    
    ~Queue () {
        delete[] queue;
    }

    bool isEmpty () {
        return rear == -1;
    }

    bool isFull () {
        return rear == capacity - 1;
    }

    int enqueue (std::string name) {
        if (isFull()) {
            return -1;

        }

        queue[++rear] = Person(name, lastTicketNumber++);
        return 0;
    }

    int dequeue () {
        if (isEmpty()) {
            return -1;

        }

        for (int index{0}; index < rear; index++) {
            queue[index] = queue[index + 1];
        }

        rear--;
        return 0;
    }

    std::string peek () {
        if (isEmpty()) {
            return "-1";

        } 
        
        return queue[front].getTicket();
    }

    void displaySize () {
        if (isEmpty()) {
            std::cout << "QUEUE EMPTY" << '\n';
        
        } else {
            std::cout << "QUEUE SIZE: " << (rear + 1) << '\n';

        } 
    }

    void displayTickets () {
        if (!isEmpty()) {
            for (int index = front; index <= rear; index++) {
                std::cout << (index + 1) << ". " << queue[index].getTicket() << '\n';
                delayFor(150);
            }

        }

        displaySize();
        std::cout << '\n';
    }
    
    int findPosition (int ticketNumber) {
        for (int index = front; index <= rear; index++) {
            if ((ticketNumber - 1) == queue[index].getRawTicketNumber()) {
                return index + 1;

            }
        }

        return -1;
        // not found
    }

    int findPosition (std::string name) {
        for (int index = front; index <= rear; index++) {
            if (name == queue[index].getName()) {
                return index + 1;
            }
        }

        return -1;
        // not found
    }

    void updateQueue () {
        bool thereIsUpdate{false};

        for (int index{0}; index <= rear; index++) {
            if (queue[index].isDue()) {
                thereIsUpdate = true;

                std::cout << "DEQUEUE: " << queue[index].getName() << " has received a ticket " << queue[index].getFormattedTicketNumber() << '\n';
                dequeue();
                delayFor(300);
            
            } else {
                break;

            }

        }

        if (thereIsUpdate) {
            displaySize();
            std::cout << '\n';

        }
    }
        
};


int main () {
    Person Alice("Alice", 100);
    int wait;
    std::chrono::time_point<std::chrono::system_clock> currentTime;

    Queue ticketLine(999);

    ticketLine.enqueue("Lisa");
    ticketLine.enqueue("Allaine");
    ticketLine.enqueue("Lua");
    ticketLine.enqueue("Ruby");
    ticketLine.displayTickets();

    ticketLine.dequeue();
    ticketLine.displayTickets();


    std::cin >> wait;
    currentTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedTime = currentTime - Alice.getTicketCreationTime();

    ticketLine.updateQueue();

    std::cout << "Elapsed time: " << elapsedTime.count() << '\n';
    // std::cout << "Position of Ticket Number 3: " << ticketLine.findPosition(3) << '\n';
    // std::cout << "Position of Ruby: " << ticketLine.findPosition("Ruby") << '\n';
    // std::cout << "Position of Lua: " << ticketLine.findPosition("Lua") << '\n';
    // std::cout << "Position of Lisa: " << ticketLine.findPosition("Lisa") << '\n';
    // std::cout << "First in Line: " << ticketLine.peek() << '\n';

    return 0;
}
