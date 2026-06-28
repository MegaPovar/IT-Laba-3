#include <iostream>
#include <sstream>
#include <string>

#include "Deque.hpp"
#include "Queue.hpp"
#include "Stack.hpp"
#include "stdexcept"

int SquareInt(int value) {
    return value * value;
}

bool IsEvenInt(int value) {
    return value % 2 == 0;
}

int SumInt(int sum, int value) {
    return sum + value;
}

int ReadInt(const std::string& prompt) { // безопасное чтение целого числа
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

template <class T>
void PrintStack(const Stack<T>& stack) { // стек печатаем через внутреннюю sequence
    typename Stack<T>::Iterator iterator = stack.Begin();
    std::cout << "[";
    bool first = true;
    while (iterator.HasValue()) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << iterator.Get();
        first = false;
        iterator.MoveNext();
    }
    std::cout << "]";
}

template <class T>
void PrintQueue(const Queue<T>& queue) {
    typename Queue<T>::Iterator iterator = queue.Begin();
    std::cout << "[";
    bool first = true;
    while (iterator.HasValue()) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << iterator.Get();
        first = false;
        iterator.MoveNext();
    }
    std::cout << "]";
}

template <class T>
void PrintDeque(const Deque<T>& deque) {
    typename Deque<T>::Iterator iterator = deque.Begin();
    std::cout << "[";
    bool first = true;
    while (iterator.HasValue()) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << iterator.Get();
        first = false;
        iterator.MoveNext();
    }
    std::cout << "]";
}

Stack<int> ReadStack() { // ввод стека с клавиатуры
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

void WorkWithStack(Stack<int>& stack) { // основное меню работы со стеком
    bool running = true;
    while (running) {
        std::cout << "\nStack menu\n";
        std::cout << "1. Print\n";
        std::cout << "2. Push\n";
        std::cout << "3. Pop\n";
        std::cout << "4. Peek\n";
        std::cout << "5. Map x*x\n";
        std::cout << "6. Where even\n";
        std::cout << "7. Reduce sum\n";
        std::cout << "8. Concat with another stack\n";
        std::cout << "9. Substack by indexes\n";
        std::cout << "10. Contains subsequence\n";
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
                Stack<int> result = stack.Map<int>(SquareInt); // пример map
                PrintStack(result);
                std::cout << "\n";
            } else if (choice == 6) {
                Stack<int> result = stack.Where(IsEvenInt); // пример where
                PrintStack(result);
                std::cout << "\n";
            } else if (choice == 7) {
                int sum = stack.Reduce<int>(SumInt, 0); // пример reduce
                std::cout << "Sum: " << sum << "\n";
            } else if (choice == 8) {
                std::cout << "Enter second stack\n";
                Stack<int> other = ReadStack();
                Stack<int> result = stack + other;
                PrintStack(result);
                std::cout << "\n";
            } else if (choice == 9) {
                int start = ReadInt("Start index: ");
                int end = ReadInt("End index: ");
                Stack<int> result = stack.GetSubstack(start, end);
                PrintStack(result);
                std::cout << "\n";
            } else if (choice == 10) {
                std::cout << "Enter subsequence\n";
                Stack<int> candidate = ReadStack();
                std::cout << (stack.ContainsSubsequence(candidate.AsSequence()) ? "Found" : "Not found") << "\n";
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
        std::cout << "5. Map x*x\n";
        std::cout << "6. Where even\n";
        std::cout << "7. Reduce sum\n";
        std::cout << "8. Concat with another queue\n";
        std::cout << "9. Subqueue by indexes\n";
        std::cout << "10. Contains subsequence\n";
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
                Queue<int> result = queue.Map<int>(SquareInt);
                PrintQueue(result);
                std::cout << "\n";
            } else if (choice == 6) {
                Queue<int> result = queue.Where(IsEvenInt);
                PrintQueue(result);
                std::cout << "\n";
            } else if (choice == 7) {
                int sum = queue.Reduce<int>(SumInt, 0);
                std::cout << "Sum: " << sum << "\n";
            } else if (choice == 8) {
                std::cout << "Enter second queue\n";
                Queue<int> other = ReadQueue();
                Queue<int> result = queue + other;
                PrintQueue(result);
                std::cout << "\n";
            } else if (choice == 9) {
                int start = ReadInt("Start index: ");
                int end = ReadInt("End index: ");
                Queue<int> result = queue.GetSubqueue(start, end);
                PrintQueue(result);
                std::cout << "\n";
            } else if (choice == 10) {
                std::cout << "Enter subsequence\n";
                Queue<int> candidate = ReadQueue();
                std::cout << (queue.ContainsSubsequence(candidate.AsSequence()) ? "Found" : "Not found") << "\n";
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
        std::cout << "8. Map x*x\n";
        std::cout << "9. Where even\n";
        std::cout << "10. Reduce sum\n";
        std::cout << "11. Concat with another deque\n";
        std::cout << "12. Subdeque by indexes\n";
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
                Deque<int> result = deque.Map<int>(SquareInt);
                PrintDeque(result);
                std::cout << "\n";
            } else if (choice == 9) {
                Deque<int> result = deque.Where(IsEvenInt);
                PrintDeque(result);
                std::cout << "\n";
            } else if (choice == 10) {
                int sum = deque.Reduce<int>(SumInt, 0);
                std::cout << "Sum: " << sum << "\n";
            } else if (choice == 11) {
                std::cout << "Enter second deque\n";
                Deque<int> other = ReadDeque();
                Deque<int> result = deque + other;
                PrintDeque(result);
                std::cout << "\n";
            } else if (choice == 12) {
                int start = ReadInt("Start index: ");
                int end = ReadInt("End index: ");
                Deque<int> result = deque.GetSubdeque(start, end);
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

void DemoScenario() { // автоматический показ основных возможностей
    int source[] = {3, 1, 2, 5, 4};
    Stack<int> stack(source, 5);
    Queue<int> queue(source, 5);
    Deque<int> deque(source, 5);

    std::cout << "\nAutomatic demo scenario\n";
    std::cout << "Initial stack: ";
    PrintStack(stack);
    std::cout << "\n";

    stack.Push(8);
    std::cout << "After Push(8): ";
    PrintStack(stack);
    std::cout << "\n";

    std::cout << "Peek: " << stack.Peek() << "\n";
    std::cout << "Pop: " << stack.Pop() << "\n";
    std::cout << "After Pop: ";
    PrintStack(stack);
    std::cout << "\n";

    Stack<int> squares = stack.Map<int>(SquareInt); // квадраты элементов
    std::cout << "Map x*x: ";
    PrintStack(squares);
    std::cout << "\n";

    Stack<int> even = stack.Where(IsEvenInt); // оставляем четные
    std::cout << "Where even: ";
    PrintStack(even);
    std::cout << "\n";

    int sum = stack.Reduce<int>(SumInt, 0); // сумма всех элементов
    std::cout << "Reduce sum: " << sum << "\n";

    Stack<int> substack = stack.GetSubstack(1, 3);
    std::cout << "Substack [1..3]: ";
    PrintStack(substack);
    std::cout << "\n";
    std::cout << "Contains this substack: "
              << (stack.ContainsSubsequence(substack.AsSequence()) ? "yes" : "no") << "\n";

    std::cout << "Queue demo, Dequeue: " << queue.Dequeue() << ", Peek: " << queue.Peek() << "\n";
    deque.PushFront(0);
    deque.PushBack(6);
    std::cout << "Deque demo: ";
    PrintDeque(deque);
    std::cout << "\n";
}

int main() { // главное меню программы
    bool running = true;
    while (running) {
        std::cout << "\nLab 3: Linear containers based on Sequence\n";
        std::cout << "1. Create and test Stack<int>\n";
        std::cout << "2. Create and test Queue<int>\n";
        std::cout << "3. Create and test Deque<int>\n";
        std::cout << "4. Automatic demo\n";
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
                DemoScenario();
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
