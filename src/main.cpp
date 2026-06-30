#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Deque.hpp"
#include "LinearForm.hpp"
#include "ListSequence.hpp"
#include "PriorityQueue.hpp"
#include "Queue.hpp"
#include "Stack.hpp"

int ReadInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            throw std::invalid_argument("Input stream was closed");
        }

        std::istringstream input(line);
        int value;
        char extra;
        if ((input >> value) && !(input >> extra)) {
            return value;
        }

        std::cout << "Input error: enter an integer number\n";
    }
}

double ReadDouble(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            throw std::invalid_argument("Input stream was closed");
        }

        std::istringstream input(line);
        double value;
        char extra;
        if ((input >> value) && !(input >> extra)) {
            return value;
        }

        std::cout << "Input error: enter a number\n";
    }
}

template <class T>
void PrintStack(Stack<T> stack) {
    std::cout << "[";
    bool first = true;
    while (!stack.IsEmpty()) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << stack.Pop();
        first = false;
    }
    std::cout << "]";
}

template <class T>
void PrintQueue(Queue<T> queue) {
    std::cout << "[";
    bool first = true;
    while (!queue.IsEmpty()) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << queue.Dequeue();
        first = false;
    }
    std::cout << "]";
}

template <class T>
void PrintDeque(Deque<T> deque) {
    std::cout << "[";
    bool first = true;
    while (!deque.IsEmpty()) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << deque.PopFront();
        first = false;
    }
    std::cout << "]";
}

template <class T>
void PrintPriorityQueue(PriorityQueue<T> queue) {
    std::cout << "[";
    bool first = true;
    while (!queue.IsEmpty()) {
        if (!first) {
            std::cout << ", ";
        }
        int priority = queue.PeekPriority();
        T value = queue.Dequeue();
        std::cout << value << "(p=" << priority << ")";
        first = false;
    }
    std::cout << "]";
}

void PrintLinearForm(const LinearForm<double>& form) {
    std::cout << form.GetCoefficient(0);
    for (int i = 1; i < form.GetCoefficientsCount(); ++i) {
        std::cout << " + " << form.GetCoefficient(i) << "*x" << i;
    }
}

Stack<int> ReadStack() {
    int count = ReadInt("Count: ");
    if (count < 0) {
        throw std::invalid_argument("Count cannot be negative");
    }

    Stack<int> stack;
    for (int i = 0; i < count; ++i) {
        std::ostringstream prompt;
        prompt << "item[" << i << "]: ";
        stack.Push(ReadInt(prompt.str()));
    }
    return stack;
}

Queue<int> ReadQueue() {
    int count = ReadInt("Count: ");
    if (count < 0) {
        throw std::invalid_argument("Count cannot be negative");
    }

    Queue<int> queue;
    for (int i = 0; i < count; ++i) {
        std::ostringstream prompt;
        prompt << "item[" << i << "]: ";
        queue.Enqueue(ReadInt(prompt.str()));
    }
    return queue;
}

Deque<int> ReadDeque() {
    int count = ReadInt("Count: ");
    if (count < 0) {
        throw std::invalid_argument("Count cannot be negative");
    }

    Deque<int> deque;
    for (int i = 0; i < count; ++i) {
        std::ostringstream prompt;
        prompt << "item[" << i << "]: ";
        deque.PushBack(ReadInt(prompt.str()));
    }
    return deque;
}

PriorityQueue<int> ReadPriorityQueue() {
    int count = ReadInt("Count: ");
    if (count < 0) {
        throw std::invalid_argument("Count cannot be negative");
    }

    PriorityQueue<int> queue;
    for (int i = 0; i < count; ++i) {
        std::ostringstream valuePrompt;
        valuePrompt << "item[" << i << "]: ";
        int value = ReadInt(valuePrompt.str());

        std::ostringstream priorityPrompt;
        priorityPrompt << "priority[" << i << "]: ";
        int priority = ReadInt(priorityPrompt.str());

        queue.Enqueue(value, priority);
    }
    return queue;
}

LinearForm<double> ReadLinearForm() {
    int variablesCount = ReadInt("Variables count: ");
    if (variablesCount < 0) {
        throw std::invalid_argument("Variables count cannot be negative");
    }

    MutableListSequence<double> coefficients;
    for (int i = 0; i <= variablesCount; ++i) {
        std::ostringstream prompt;
        prompt << "a" << i << ": ";
        coefficients.Append(ReadDouble(prompt.str()));
    }
    return LinearForm<double>(coefficients);
}

MutableListSequence<double> ReadVariables(int variablesCount) {
    MutableListSequence<double> variables;
    for (int i = 1; i <= variablesCount; ++i) {
        std::ostringstream prompt;
        prompt << "x" << i << ": ";
        variables.Append(ReadDouble(prompt.str()));
    }
    return variables;
}

void WorkWithStack(Stack<int>& stack) {
    bool running = true;
    while (running) {
        std::cout << "\nStack menu\n";
        std::cout << "1. Print\n";
        std::cout << "2. Push\n";
        std::cout << "3. Pop\n";
        std::cout << "4. Peek\n";
        std::cout << "5. Concat with another stack\n";
        std::cout << "0. Back\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1) {
                PrintStack(stack);
                std::cout << "\n";
            } else if (choice == 2) {
                stack.Push(ReadInt("Value: "));
            } else if (choice == 3) {
                std::cout << "Popped: " << stack.Pop() << "\n";
            } else if (choice == 4) {
                std::cout << "Top: " << stack.Peek() << "\n";
            } else if (choice == 5) {
                std::cout << "Enter second stack\n";
                Stack<int> other = ReadStack();
                Stack<int> result = stack.Concat(other);
                PrintStack(result);
                std::cout << "\n";
            } else if (choice == 0) {
                running = false;
            } else {
                std::cout << "Unknown command\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void WorkWithQueue(Queue<int>& queue) {
    bool running = true;
    while (running) {
        std::cout << "\nQueue menu\n";
        std::cout << "1. Print\n";
        std::cout << "2. Enqueue\n";
        std::cout << "3. Dequeue\n";
        std::cout << "4. Peek\n";
        std::cout << "5. Concat with another queue\n";
        std::cout << "0. Back\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1) {
                PrintQueue(queue);
                std::cout << "\n";
            } else if (choice == 2) {
                queue.Enqueue(ReadInt("Value: "));
            } else if (choice == 3) {
                std::cout << "Dequeued: " << queue.Dequeue() << "\n";
            } else if (choice == 4) {
                std::cout << "Front: " << queue.Peek() << "\n";
            } else if (choice == 5) {
                std::cout << "Enter second queue\n";
                Queue<int> other = ReadQueue();
                Queue<int> result = queue.Concat(other);
                PrintQueue(result);
                std::cout << "\n";
            } else if (choice == 0) {
                running = false;
            } else {
                std::cout << "Unknown command\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void WorkWithDeque(Deque<int>& deque) {
    bool running = true;
    while (running) {
        std::cout << "\nDeque menu\n";
        std::cout << "1. Print\n";
        std::cout << "2. PushFront\n";
        std::cout << "3. PushBack\n";
        std::cout << "4. PopFront\n";
        std::cout << "5. PopBack\n";
        std::cout << "6. PeekFront\n";
        std::cout << "7. PeekBack\n";
        std::cout << "8. Concat with another deque\n";
        std::cout << "0. Back\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1) {
                PrintDeque(deque);
                std::cout << "\n";
            } else if (choice == 2) {
                deque.PushFront(ReadInt("Value: "));
            } else if (choice == 3) {
                deque.PushBack(ReadInt("Value: "));
            } else if (choice == 4) {
                std::cout << "Popped front: " << deque.PopFront() << "\n";
            } else if (choice == 5) {
                std::cout << "Popped back: " << deque.PopBack() << "\n";
            } else if (choice == 6) {
                std::cout << "Front: " << deque.PeekFront() << "\n";
            } else if (choice == 7) {
                std::cout << "Back: " << deque.PeekBack() << "\n";
            } else if (choice == 8) {
                std::cout << "Enter second deque\n";
                Deque<int> other = ReadDeque();
                Deque<int> result = deque.Concat(other);
                PrintDeque(result);
                std::cout << "\n";
            } else if (choice == 0) {
                running = false;
            } else {
                std::cout << "Unknown command\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void WorkWithPriorityQueue(PriorityQueue<int>& queue) {
    bool running = true;
    while (running) {
        std::cout << "\nPriority queue menu\n";
        std::cout << "1. Print\n";
        std::cout << "2. Enqueue\n";
        std::cout << "3. Dequeue\n";
        std::cout << "4. Peek\n";
        std::cout << "5. Peek priority\n";
        std::cout << "6. Concat with another priority queue\n";
        std::cout << "0. Back\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1) {
                PrintPriorityQueue(queue);
                std::cout << "\n";
            } else if (choice == 2) {
                int value = ReadInt("Value: ");
                int priority = ReadInt("Priority: ");
                queue.Enqueue(value, priority);
            } else if (choice == 3) {
                std::cout << "Dequeued: " << queue.Dequeue() << "\n";
            } else if (choice == 4) {
                std::cout << "Front: " << queue.Peek() << "\n";
            } else if (choice == 5) {
                std::cout << "Priority: " << queue.PeekPriority() << "\n";
            } else if (choice == 6) {
                std::cout << "Enter second priority queue\n";
                PriorityQueue<int> other = ReadPriorityQueue();
                PriorityQueue<int> result = queue.Concat(other);
                PrintPriorityQueue(result);
                std::cout << "\n";
            } else if (choice == 0) {
                running = false;
            } else {
                std::cout << "Unknown command\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void WorkWithLinearForm(LinearForm<double>& form) {
    bool running = true;
    while (running) {
        std::cout << "\nLinear form menu\n";
        std::cout << "1. Print\n";
        std::cout << "2. Evaluate\n";
        std::cout << "3. Set coefficient\n";
        std::cout << "4. Add another form\n";
        std::cout << "5. Subtract another form\n";
        std::cout << "6. Multiply by scalar\n";
        std::cout << "0. Back\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1) {
                PrintLinearForm(form);
                std::cout << "\n";
            } else if (choice == 2) {
                MutableListSequence<double> variables = ReadVariables(form.GetVariablesCount());
                std::cout << "Value: " << form.Evaluate(variables) << "\n";
            } else if (choice == 3) {
                int index = ReadInt("Coefficient index: ");
                double value = ReadDouble("Value: ");
                form.SetCoefficient(index, value);
            } else if (choice == 4) {
                std::cout << "Enter second linear form\n";
                LinearForm<double> other = ReadLinearForm();
                LinearForm<double> result = form.Add(other);
                PrintLinearForm(result);
                std::cout << "\n";
            } else if (choice == 5) {
                std::cout << "Enter second linear form\n";
                LinearForm<double> other = ReadLinearForm();
                LinearForm<double> result = form.Subtract(other);
                PrintLinearForm(result);
                std::cout << "\n";
            } else if (choice == 6) {
                double scalar = ReadDouble("Scalar: ");
                LinearForm<double> result = form.Multiply(scalar);
                PrintLinearForm(result);
                std::cout << "\n";
            } else if (choice == 0) {
                running = false;
            } else {
                std::cout << "Unknown command\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

int main() {
    bool running = true;
    while (running) {
        std::cout << "\nLab 3\n";
        std::cout << "1. Stack<int>\n";
        std::cout << "2. Queue<int>\n";
        std::cout << "3. Deque<int>\n";
        std::cout << "4. PriorityQueue<int>\n";
        std::cout << "5. LinearForm\n";
        std::cout << "0. Exit\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1) {
                Stack<int> stack = ReadStack();
                WorkWithStack(stack);
            } else if (choice == 2) {
                Queue<int> queue = ReadQueue();
                WorkWithQueue(queue);
            } else if (choice == 3) {
                Deque<int> deque = ReadDeque();
                WorkWithDeque(deque);
            } else if (choice == 4) {
                PriorityQueue<int> priorityQueue = ReadPriorityQueue();
                WorkWithPriorityQueue(priorityQueue);
            } else if (choice == 5) {
                LinearForm<double> form = ReadLinearForm();
                WorkWithLinearForm(form);
            } else if (choice == 0) {
                running = false;
            } else {
                std::cout << "Unknown command\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }
    }

    return 0;
}
