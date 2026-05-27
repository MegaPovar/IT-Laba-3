#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "ArraySequence.hpp"
#include "InversionAlgorithms.hpp"
#include "ListSequence.hpp"
#include "NumberAlgorithms.hpp"
#include "Stack.hpp"

int ReadInt(const std::string& prompt) {
    while (true) {
        std::cout << prompt;

        std::string line;
        if (!std::getline(std::cin, line)) {
            throw InvalidArgument("Input stream was closed");
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
void PrintSequence(const Sequence<T>* sequence) {
    std::cout << "[";
    for (int i = 0; i < sequence->GetLength(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << sequence->Get(i);
    }
    std::cout << "]";
}

template <class T>
void PrintStack(const Stack<T>& stack) {
    PrintSequence(&stack.AsSequence());
}

Stack<int> ReadStack() {
    int count = ReadInt("Count: ");
    if (count < 0) {
        throw InvalidArgument("Count cannot be negative");
    }

    Stack<int> stack;
    for (int i = 0; i < count; ++i) {
        std::ostringstream prompt;
        prompt << "item[" << i << "]: ";
        stack.Push(ReadInt(prompt.str()));
    }
    return stack;
}

void WorkWithStack(Stack<int>& stack) {
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
        std::cout << "11. Count inversions\n";
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
                Stack<int> result = stack.Map<int>([](int value) { return value * value; });
                PrintStack(result);
                std::cout << "\n";
            } else if (choice == 6) {
                Stack<int> result = stack.Where([](int value) { return value % 2 == 0; });
                PrintStack(result);
                std::cout << "\n";
            } else if (choice == 7) {
                int sum = stack.Reduce<int>([](int sum, int value) { return sum + value; }, 0);
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
            } else if (choice == 11) {
                long long loops = CountInversionsLoops<int>(stack.AsSequence(), [](int left, int right) {
                    return left < right;
                });
                long long mapReduce = CountInversionsMapReduce<int>(stack.AsSequence(), [](int left, int right) {
                    return left < right;
                });
                std::cout << "Inversions by loops: " << loops << "\n";
                std::cout << "Inversions by map-reduce: " << mapReduce << "\n";
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

void NumberAlgorithmsMenu() {
    bool running = true;
    while (running) {
        std::cout << "\nNumber algorithms\n";
        std::cout << "1. Range\n";
        std::cout << "2. Primes in range\n";
        std::cout << "3. Divisors\n";
        std::cout << "4. Prime factorization\n";
        std::cout << "0. Back\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1 || choice == 2) {
                int low = ReadInt("Low: ");
                int high = ReadInt("High: ");
                Sequence<int>* result = choice == 1 ? Range(low, high) : PrimesInRange(low, high);
                PrintSequence(result);
                std::cout << "\n";
                delete result;
            } else if (choice == 3 || choice == 4) {
                int value = ReadInt("Value: ");
                Sequence<int>* result = choice == 3 ? Factorize(value) : PrimeFactorization(value);
                PrintSequence(result);
                std::cout << "\n";
                delete result;
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

long long MillisecondsSince(std::chrono::high_resolution_clock::time_point start,
                            std::chrono::high_resolution_clock::time_point end) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void DemoScenario() {
    int source[] = {3, 1, 2, 5, 4};
    Stack<int> stack(source, 5);

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

    Stack<int> squares = stack.Map<int>([](int value) { return value * value; });
    std::cout << "Map x*x: ";
    PrintStack(squares);
    std::cout << "\n";

    Stack<int> even = stack.Where([](int value) { return value % 2 == 0; });
    std::cout << "Where even: ";
    PrintStack(even);
    std::cout << "\n";

    int sum = stack.Reduce<int>([](int acc, int value) { return acc + value; }, 0);
    std::cout << "Reduce sum: " << sum << "\n";

    Stack<int> substack = stack.GetSubstack(1, 3);
    std::cout << "Substack [1..3]: ";
    PrintStack(substack);
    std::cout << "\n";
    std::cout << "Contains this substack: "
              << (stack.ContainsSubsequence(substack.AsSequence()) ? "yes" : "no") << "\n";

    long long loops = CountInversionsLoops<int>(stack.AsSequence(), [](int left, int right) {
        return left < right;
    });
    long long mapReduce = CountInversionsMapReduce<int>(stack.AsSequence(), [](int left, int right) {
        return left < right;
    });
    std::cout << "Inversions by loops: " << loops << "\n";
    std::cout << "Inversions by map-reduce: " << mapReduce << "\n";

    Sequence<int>* primes = PrimesInRange(1, 20);
    std::cout << "Primes in range [1..20]: ";
    PrintSequence(primes);
    std::cout << "\n";
    delete primes;

    Sequence<int>* factors = PrimeFactorization(84);
    std::cout << "Prime factorization of 84: ";
    PrintSequence(factors);
    std::cout << "\n";
    delete factors;
}

template <class TSequence>
long long BenchmarkAppend(int count) {
    TSequence sequence;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        sequence.Append(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return MillisecondsSince(start, end);
}

template <class TSequence>
long long BenchmarkInsertMiddle(int initialCount, int insertCount) {
    TSequence sequence;
    for (int i = 0; i < initialCount; ++i) {
        sequence.Append(i);
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < insertCount; ++i) {
        sequence.InsertAt(-1, sequence.GetLength() / 2);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return MillisecondsSince(start, end);
}

template <class TSequence>
long long BenchmarkIndexedRead(int count) {
    TSequence sequence;
    for (int i = 0; i < count; ++i) {
        sequence.Append(i);
    }

    long long checksum = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; ++i) {
        checksum += sequence.Get(i);
    }
    auto end = std::chrono::high_resolution_clock::now();

    if (checksum == -1) {
        std::cout << "";
    }
    return MillisecondsSince(start, end);
}

void Benchmark() {
    const int stackCount = 500;
    Stack<int> stack;

    auto startPush = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < stackCount; ++i) {
        stack.Push(i);
    }
    auto endPush = std::chrono::high_resolution_clock::now();

    auto startPop = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < stackCount; ++i) {
        stack.Pop();
    }
    auto endPop = std::chrono::high_resolution_clock::now();

    const int appendCount = 12000;
    const int readCount = 5000;
    const int initialCount = 5000;
    const int insertCount = 1500;

    std::cout << "\nStack ADT benchmark\n";
    std::cout << "Push " << stackCount << " items: " << MillisecondsSince(startPush, endPush) << " ms\n";
    std::cout << "Pop " << stackCount << " items: " << MillisecondsSince(startPop, endPop) << " ms\n";

    std::cout << "\nSequence storage implementations benchmark\n";
    std::cout << "Append " << appendCount << " items\n";
    std::cout << "ArraySequence: " << BenchmarkAppend<MutableArraySequence<int> >(appendCount) << " ms\n";
    std::cout << "ListSequence: " << BenchmarkAppend<MutableListSequence<int> >(appendCount) << " ms\n";

    std::cout << "\nIndexed read " << readCount << " items\n";
    std::cout << "ArraySequence: " << BenchmarkIndexedRead<MutableArraySequence<int> >(readCount) << " ms\n";
    std::cout << "ListSequence: " << BenchmarkIndexedRead<MutableListSequence<int> >(readCount) << " ms\n";

    std::cout << "\nInsertAt middle " << insertCount
              << " times, initial length " << initialCount << "\n";
    std::cout << "ArraySequence: "
              << BenchmarkInsertMiddle<MutableArraySequence<int> >(initialCount, insertCount) << " ms\n";
    std::cout << "ListSequence: "
              << BenchmarkInsertMiddle<MutableListSequence<int> >(initialCount, insertCount) << " ms\n";
}

int main() {
    bool running = true;
    while (running) {
        std::cout << "\nLab 3: Stack ADT based on Sequence\n";
        std::cout << "1. Create and test Stack<int>\n";
        std::cout << "2. Number algorithms\n";
        std::cout << "3. Automatic demo\n";
        std::cout << "4. Benchmark\n";
        std::cout << "0. Exit\n";
        int choice = ReadInt("Choice: ");

        try {
            if (choice == 1) {
                Stack<int> stack = ReadStack();
                WorkWithStack(stack);
            } else if (choice == 2) {
                NumberAlgorithmsMenu();
            } else if (choice == 3) {
                DemoScenario();
            } else if (choice == 4) {
                Benchmark();
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
