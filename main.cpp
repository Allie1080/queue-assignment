#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>

const std::chrono::duration<double> timeLimit{60.0};
// you can change this if you want

void delayFor (int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

bool isdigit (std::string text) {
    for (int index{0}; index < text.size(); index++) {
        if (!isdigit(text[index])) {
            return false;
        
        }
    }

    return true;
}

int convertStringToInt(std::string text) {
    int number{0};

    if (!isdigit(text)) {
        return -1;
    
    }

    for (int index{0}; index < text.size(); index++) {
        number = number*10 + (static_cast<int>(text[index]) - 48);
    }

    return number;
}

class Person {
private:
    std::string name;
    int ticketNumber;
    // starts at 0, but display() starts at 1
    std::chrono::time_point<std::chrono::system_clock> ticketCreationTime;
    // i thouught tickets were to be dequeued 1 minute after creation, not every minute
    // whatever, its cool
    // you can toggle it in the .updateQueue somewhere around line 336

public:
    
    Person () {};

    Person (std::string nameInput, int ticketNumberInput) {
        name = nameInput;
        ticketNumber = ticketNumberInput;
        ticketCreationTime = std::chrono::system_clock::now();

    }

    bool isDue () {
        // std::cout << name << ": " << (std::chrono::system_clock::now() - ticketCreationTime).count() << '\n';
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

        ticket << "Ticket #";
        
        if (ticketNumberDigits == 1) {
            ticket << "00" << (ticketNumber + 1);

        } else if (ticketNumberDigits == 2) {
            ticket << "0" << (ticketNumber + 1);

        } else if (ticketNumberDigits == 1) {
            ticket << (ticketNumber + 1);

        }

        return ticket.str();

    }

    std::string getTicket () {
        // sample output: "Alice (#001)"
        std::ostringstream personInformation;

        personInformation << name << " (" << getFormattedTicketNumber() << ")";

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

    std::chrono::time_point<std::chrono::system_clock> creationTime;
    std::chrono::time_point<std::chrono::system_clock> timeOfLastDequeue;
    int totalMinutesPassed;

public:
    Queue (int cap) {
        rear  = -1;
        capacity = cap;

        queue = new Person[capacity];

        lastTicketNumber = 0;

        creationTime = std::chrono::system_clock::now();
        timeOfLastDequeue = creationTime;
        totalMinutesPassed = 0;
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

    void peek () {
        if (isEmpty()) {
            std::cout << "[QUEUE EMPTY]" << '\n';

        } else {
            std::cout << queue[front].getName() << " is at the front of the line with " << queue[front].getFormattedTicketNumber() << '\n';  

        }
    }

    void displayEnqueueMessage (int enqueueExitCode) {
        if (enqueueExitCode == 0) {
        std::cout << queue[rear].getName() << " has been added to the queue with " << queue[rear].getFormattedTicketNumber() << '\n';  

        } else {
            std::cout << "QUEUE IS FULL" << '\n';
        
        }
    }

    int size () {
        return rear + 1;
    }

    void displaySize () {
        if (isEmpty()) {
            std::cout << "[QUEUE EMPTY]" << '\n';
        
        } else {
            std::cout << "[QUEUE SIZE: " << size() << "]" << '\n';

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
    }
    
    int findPosition (int ticketNumber) {
        for (int index = front; index <= rear; index++) {
            // std::cout << ticketNumber << " == " << queue[index].getRawTicketNumber() << " is " << (ticketNumber == queue[index].getRawTicketNumber()) << '\n';

            if ((ticketNumber - 1) == queue[index].getRawTicketNumber()) {
                return index + 1;

            }
        }

        return -1;
        // not found
    }

    int findPosition (std::string name) {
        for (int index = front; index <= rear; index++) {
            // std::cout << name << " == " << queue[index].getName() << " is " << (name == queue[index].getName());
            
            if (name == queue[index].getName()) {
                return index + 1;
            }
        }

        return -1;
        // not found
    }

    std::string findTicket (int position) {
        if (position < 0) {
            return "\0";
        }

        return queue[position - 1].getTicket();
    }

    int countDequeueAndAnnounceEveryMinute () {
        int minutesPassed = ((std::chrono::system_clock::now() - timeOfLastDequeue) / timeLimit);
        int amountToDequeue = minutesPassed;

        amountToDequeue = (amountToDequeue > size()) ? size() : amountToDequeue;
        // since nothing is actually being dequed at this stage, we need to manually match amountToDequeue to size()
        // otherwise, the program would announce the dequeuement of the final item multiple times 

        if (amountToDequeue == 0) {
            return amountToDequeue;
        }

        std::cout << minutesPassed << ((minutesPassed > 1) ? " minutes" : " minute") << " has passed..." << '\n';
        delayFor(300);

        for (int index{0}; index < amountToDequeue; index++) {
            if (isEmpty()) {
                break;
            
            }
            
            std::cout << "DEQUEUE: " << queue[index].getName() << " has received a ticket (" << queue[index].getFormattedTicketNumber() << ")" << '\n';
            delayFor(300);
        }

        
        timeOfLastDequeue = std::chrono::system_clock::now();
        return amountToDequeue;
    }

    int countDequeueAndAnnouncePerIndividualTicketClock () {
        int amountToDequeue{0};

        for (int index{0}; index <= rear; index++) {
            if (queue[index].isDue()) {
                std::cout << "DEQUEUE: " << queue[index].getName() << " has received a ticket (" << queue[index].getFormattedTicketNumber() << ")" << '\n';
                amountToDequeue++;
                delayFor(300);
            
            } else {
                break;

            }

        }
        
        return amountToDequeue;
    }

    void updateQueue (bool useOriginalDequeueingCounter=false) {
        bool thereIsUpdate{false};
        int amountToDequeue = (!useOriginalDequeueingCounter) ? countDequeueAndAnnounceEveryMinute() : countDequeueAndAnnouncePerIndividualTicketClock() ;

        if (amountToDequeue == 0) {
            return;
        }

        for (int counter{1}; counter <= amountToDequeue; counter++) {
            if (isEmpty()) {
                break;
            }
            dequeue();
            // shouldnt dequeue iterating over queue
        }
        displaySize();
        std::cout << '\n';
    }
        
};

int main () {
    std::string prompt;
    Queue ticketQueue(999);
    std::string name;
    std::string searchRequest;
    int ticketSearchRequest;
    int position;
    
    std::cout << "WELCOME TO OLIVIA RODRIGO'S CONCERT TICKETING SYSTEM" << '\n';
    std::cout << "====================================================" << '\n';
    std::cout << '\n';
    delayFor(500);
    std::cout << "1. Enqueue a person" << '\n';
    delayFor(50);
    std::cout << "2. Check your position in the queue" << '\n';
    delayFor(50);
    std::cout << "3. Peek entire line" << '\n';
    delayFor(50);
    std::cout << "4. Peek the front of the line" << '\n';
    delayFor(50);
    std::cout << "5. Exit" << '\n';
    delayFor(50);
    std::cout << '\n';
    std::cout << '\n';

    while (prompt[0] != '5') {
        std::cout << "Choose an option: " << '\n' << ">> ";
        getline(std::cin, prompt);
        std::cout << '\n';
        ticketQueue.updateQueue();

        if (prompt.size() > 1) {
            std::cout << "ERROR" << '\n';
            std::cout << "Not a valid option!" << '\n';
            std::cout << '\n';

            continue;
        }

        switch(prompt[0]) {
            case '1':
                std::cout << "Enter the name: " << '\n' << ">> ";
                getline(std::cin, name);
                std::cout << '\n';

                delayFor(150);
                ticketQueue.displayEnqueueMessage(ticketQueue.enqueue(name));
                break;

            case '2':
                std::cout << "Enter your name or ticket number: " << '\n' << ">> ";
                getline(std::cin, searchRequest);
                std::cout << '\n';

                ticketSearchRequest = (isdigit(searchRequest)) ? convertStringToInt(searchRequest) : -1;  
                position = (ticketSearchRequest != -1) ? ticketQueue.findPosition(ticketSearchRequest) : ticketQueue.findPosition(searchRequest);
                
                delayFor(150);
                if (position != -1) {
                    std::cout << ticketQueue.findTicket(position) << " is currently at position " << position << " of the queue." << '\n';

                } else {
                    std::cout << "ERROR" << '\n';
                    std::cout << "Unable to find ticket!" << '\n';

                }

                break;
            
            case '3':
                delayFor(150);
                ticketQueue.peek();
                break;

            case '4':
                delayFor(150);
                std::cout << "CURRENT QUEUE:" << '\n';
                delayFor(300);
                ticketQueue.displayTickets();
                break;

            case '5': 
                break;

            default:
                std::cout << "ERROR" << '\n';
                delayFor(150);
                std::cout << "Not a valid option!" << '\n';
                break;

        }

        std::cout << '\n';

    }

    // Person Alice("Alice", 100);
    // int wait;
    // std::chrono::time_point<std::chrono::system_clock> currentTime;

    // Queue ticketLine(999);

    // ticketLine.enqueue("Lisa");
    // ticketLine.enqueue("Allaine");
    // ticketLine.enqueue("Lua");
    // ticketLine.enqueue("Ruby");
    // ticketLine.displayTickets();

    // ticketLine.dequeue();
    // ticketLine.displayTickets();


    // std::cin >> wait;
    // currentTime = std::chrono::system_clock::now();
    // std::chrono::duration<double> elapsedTime = currentTime - Alice.getTicketCreationTime();

    // ticketLine.updateQueue();

    // std::cout << "Elapsed time: " << elapsedTime.count() << '\n';
    // // std::cout << "Position of Ticket Number 3: " << ticketLine.findPosition(3) << '\n';
    // std::cout << "Position of Ruby: " << ticketLine.findPosition("Ruby") << '\n';
    // std::cout << "Position of Lua: " << ticketLine.findPosition("Lua") << '\n';
    // std::cout << "Position of Lisa: " << ticketLine.findPosition("Lisa") << '\n';
    // std::cout << "First in Line: " << ticketLine.peek() << '\n';

    return 0;
}
