/*
DATA STRUCTURES USED:
1. Singly Linked List (Dynamic Flight Database)
2. Queue (Runway FIFO Management)
3. Stack (Undo Operations - LIFO)
4. Array (Fixed Size Hangar Parking)
*/

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;
void printHeader(string title)
{
    cout << "\n==========================================================\n";
    cout << "  " << title << "\n";
    cout << "==========================================================\n";
}

void printDivider()
{
    cout << "----------------------------------------------------------\n";
}
// 1. DATA STRUCTURE: SINGLY LINKED LIST (Flight Database)
// Purpose: Stores an unlimited number of registered flights.
// Why: Dynamic memory allocation means we don't waste memory.
struct FlightNode
{
    string flightID;
    string destination;
    string airline;
    FlightNode *next;
};

class FlightLinkedList
{
private:
    FlightNode *head;

public:
    FlightLinkedList()
    {
        head = NULL;
    }

    // Insert a new flight at the beginning
    void insertFlight(string id, string dest, string air)
    {
        FlightNode *newNode = new FlightNode();
        newNode->flightID = id;
        newNode->destination = dest;
        newNode->airline = air;

        newNode->next = head;
        head = newNode;

        cout << "[SUCCESS] Flight " << id << " registered in database.\n";
    }

    // Delete a flight by ID (Used by the Undo feature)
    bool deleteFlight(string id)
    {
        if (head == NULL)
            return false;

        // If the head is the node to be deleted
        if (head->flightID == id)
        {
            FlightNode *temp = head;
            head = head->next;
            delete temp;
            return true;
        }

        // Search for the node to delete
        FlightNode *current = head;
        FlightNode *previous = NULL;

        while (current != NULL && current->flightID != id)
        {
            previous = current;
            current = current->next;
        }

        // If flight ID was not found
        if (current == NULL)
            return false;

        // Unlink the node and delete it
        previous->next = current->next;
        delete current;
        return true;
    }

    // Display all registered flights in a table
    void displayFlights()
    {
        if (head == NULL)
        {
            cout << "  [Database Empty] No flights currently registered.\n";
            return;
        }

        cout << left << setw(15) << "FLIGHT ID"
             << setw(20) << "AIRLINE"
             << setw(20) << "DESTINATION" << endl;
        printDivider();

        FlightNode *temp = head;
        while (temp != NULL)
        {
            cout << left << setw(15) << temp->flightID
                 << setw(20) << temp->airline
                 << setw(20) << temp->destination << endl;
            temp = temp->next;
        }
    }

    // Check if a flight exists before allowing it on the runway
    bool flightExists(string id)
    {
        FlightNode *temp = head;
        while (temp != NULL)
        {
            if (temp->flightID == id)
                return true;
            temp = temp->next;
        }
        return false;
    }
};
// 2. DATA STRUCTURE: QUEUE (Runway Management)
// Purpose: Manages planes waiting to takeoff or land.
// Why: First-In, First-Out (FIFO) ensures planes are handled fairly.
struct QueueNode
{
    string flightID;
    QueueNode *next;
};

class RunwayQueue
{
private:
    QueueNode *front;
    QueueNode *rear;

public:
    RunwayQueue()
    {
        front = NULL;
        rear = NULL;
    }

    // Add plane to the end of the queue
    void enqueue(string id)
    {
        QueueNode *newNode = new QueueNode();
        newNode->flightID = id;
        newNode->next = NULL;

        if (rear == NULL)
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "[RUNWAY] Flight " << id << " added to waiting queue.\n";
    }

    // Remove the first plane from the queue (Takeoff)
    string dequeue()
    {
        if (front == NULL)
        {
            return "";
        }

        QueueNode *temp = front;
        string takeoffID = temp->flightID;

        front = front->next;
        if (front == NULL)
        {
            rear = NULL;
        }

        delete temp;
        return takeoffID;
    }

    // View planes waiting
    void displayQueue()
    {
        if (front == NULL)
        {
            cout << "[Runway Clear] No planes waiting.\n";
            return;
        }

        cout << "  Waiting to Takeoff: ";
        QueueNode *temp = front;
        while (temp != NULL)
        {
            cout << "[" << temp->flightID << "] -> ";
            temp = temp->next;
        }
        cout << "RUNWAY\n";
    }
};

// 3. DATA STRUCTURE: STACK (Undo Operations)
// Purpose: Keeps track of recent actions to undo mistakes.
// Why: Last-In, First-Out (LIFO) is the standard model for "Undo" buttons.
struct StackNode
{
    string actionType;
    string flightID;
    StackNode *next;
};

class ActionStack
{
private:
    StackNode *top;

public:
    ActionStack()
    {
        top = NULL;
    }

    // Save an action
    void push(string action, string id)
    {
        StackNode *newNode = new StackNode();
        newNode->actionType = action;
        newNode->flightID = id;

        newNode->next = top;
        top = newNode;
    }

    // Retrieve and remove the last action
    bool pop(string &action, string &id)
    {
        if (top == NULL)
            return false;

        StackNode *temp = top;
        action = temp->actionType;
        id = temp->flightID;

        top = top->next;
        delete temp;
        return true;
    }
};

// ==============================================================================
// 4. DATA STRUCTURE: ARRAY (Hangar Parking Management)
// Purpose: Manages VIP/Maintenance parking.
// Why: Arrays have a fixed size, perfectly mimicking physical parking spots.
// ==============================================================================
class HangarArray
{
private:
    static const int MAX_SPOTS = 5;
    string spots[MAX_SPOTS];

public:
    HangarArray()
    {
        // Initialize array with empty strings
        for (int i = 0; i < MAX_SPOTS; i++)
        {
            spots[i] = "EMPTY";
        }
    }

    // Find first empty index and assign it
    void parkPlane(string id)
    {
        for (int i = 0; i < MAX_SPOTS; i++)
        {
            if (spots[i] == "EMPTY")
            {
                spots[i] = id;
                cout << "[PARKED] Flight " << id << " parked at Hangar Spot " << (i + 1) << ".\n";
                return;
            }
        }
        cout << "[WARNING] Hangar is FULL! Cannot park " << id << ".\n";
    }

    // Remove plane from a specific spot
    void removePlane(int spotNumber)
    {
        int index = spotNumber - 1;
        if (index < 0 || index >= MAX_SPOTS)
        {
            cout << "[ERROR] Invalid spot number.\n";
            return;
        }
        if (spots[index] == "EMPTY")
        {
            cout << "[INFO] Spot " << spotNumber << " is already empty.\n";
        }
        else
        {
            cout << "[UNPARKED] Flight " << spots[index] << " removed from Hangar Spot " << spotNumber << ".\n";
            spots[index] = "EMPTY";
        }
    }
    void displayHangar()
    {
        cout << "--- HANGAR PARKING STATUS ---\n";
        for (int i = 0; i < MAX_SPOTS; i++)
        {
            cout << "Spot " << (i + 1) << ": ";
            if (spots[i] == "EMPTY")
                cout << "[   EMPTY   ]\n";
            else
                cout << "[" << setw(11) << spots[i] << "]\n";
        }
    }
};

int main()
{
    FlightLinkedList database;
    RunwayQueue runway;
    ActionStack undoSystem;
    HangarArray hangar;

    int choice;
    string id, dest, airline;

    while (true)
    {
        printHeader("*** AEROMANAGER: AIRPORT CONTROL SYSTEM ***");
        cout << "1. Register New Flight       (Linked List + Stack)\n";
        cout << "2. View All Flights          (Linked List Traversal)\n";
        cout << "3. Request Runway Takeoff    (Queue Enqueue)\n";
        cout << "4. Allow Next Takeoff        (Queue Dequeue)\n";
        cout << "5. Park Plane in Hangar      (Array Insert)\n";
        cout << "6. Unpark Plane from Hangar  (Array Remove)\n";
        cout << "7. Undo Last Registration    (Stack Pop)\n";
        cout << "8. System Overview           (View All Data Structures)\n";
        cout << "9. Exit System\n";
        printDivider();
        cout << "Enter operation (1-9): ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[ERROR] Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            cout << "\n--- REGISTER FLIGHT ---\n";
            cout << "Enter Flight ID (e.g. BG101): ";
            cin >> id;
            cout << "Enter Airline Name: ";
            cin >> airline;
            cout << "Enter Destination: ";
            cin >> dest;

            // Add to Linked List
            database.insertFlight(id, dest, airline);
            // Push to Stack for Undo capability
            undoSystem.push("REGISTER", id);
            break;
        }
        case 2:
        {
            printHeader("REGISTERED FLIGHT DATABASE");
            database.displayFlights();
            break;
        }
        case 3:
        {
            cout << "\n--- REQUEST RUNWAY ---\n";
            cout << "Enter Flight ID requesting takeoff: ";
            cin >> id;

            // Check if flight actually exists in linked list first
            if (database.flightExists(id))
            {
                runway.enqueue(id);
            }
            else
            {
                cout << "[ERROR] Flight " << id << " is not registered in the system.\n";
            }
            break;
        }
        case 4:
        {
            cout << "\n--- CLEAR RUNWAY ---\n";
            string clearedFlight = runway.dequeue();
            if (clearedFlight == "")
            {
                cout << "[EMPTY] The runway is clear. No flights waiting in queue.\n";
            }
            else
            {
                cout << "[DEPARTURE] Takeoff Approved! Flight " << clearedFlight << " has departed.\n";
            }
            break;
        }
        case 5:
        {
            cout << "\n--- PARK IN HANGAR ---\n";
            cout << "Enter Flight ID to park: ";
            cin >> id;
            hangar.parkPlane(id);
            break;
        }
        case 6:
        {
            cout << "\n--- UNPARK FROM HANGAR ---\n";
            hangar.displayHangar();
            int spot;
            cout << "Enter Spot Number to clear (1-5): ";
            cin >> spot;
            hangar.removePlane(spot);
            break;
        }
        case 7:
        {
            cout << "\n--- UNDO LAST ACTION ---\n";
            string action, undoID;
            if (undoSystem.pop(action, undoID))
            {
                if (action == "REGISTER")
                {
                    database.deleteFlight(undoID);
                    cout << "[UNDO SUCCESS] Removed Flight " << undoID << " from database.\n";
                }
            }
            else
            {
                cout << "[WARNING] No recent actions available to undo.\n";
            }
            break;
        }
        case 8:
        {
            // Feature to view everything at once
            printHeader("SYSTEM OVERVIEW");
            cout << "[1] DATABASE STATUS (Linked List):\n";
            database.displayFlights();

            cout << "\n[2] RUNWAY STATUS (Queue):\n";
            runway.displayQueue();

            cout << "\n[3] HANGAR STATUS (Array):\n";
            hangar.displayHangar();
            break;
        }
        case 9:
        {
            printHeader("SHUTTING DOWN SYSTEM. GOODBYE!");
            return 0;
        }
        default:
        {
            cout << "[ERROR] Invalid choice! Please select between 1 and 9.\n";
            break;
        }
        }
    }
    return 0;
}