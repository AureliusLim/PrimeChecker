#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <cmath>

#define LIMIT 10000000

/*
This function checks if an integer n is prime.

Parameters:
n : int - integer to check

Returns true if n is prime, and false otherwise.
*/
bool check_prime(const int &n);

void findPrimes(int start, int end, std::vector<int>& primes, std::mutex& mutex) {
    for (int i = start; i <= end; ++i) {
        if (check_prime(i)) {
            std::lock_guard<std::mutex> lock(mutex);
            primes.push_back(i);
        }
    }
}

int main() {
  int limit, numThreads = 1;

  std::vector<int> primes;
  std::cout<< "Enter Upper Bound: ";
  std::cin >> limit;

  std::cout<< "Enter Number of Threads: ";
  std::cin >> numThreads;


  if (numThreads <= 0) {
      numThreads = 1;
  }

  std::mutex primesMutex;

  auto start_time = std::chrono::high_resolution_clock::now();

  int chunkSize = limit / numThreads;
  std::vector<std::thread> threads;

  for (int i = 0; i < numThreads; ++i) {
      int startRange = (i==0 ? 2 : i * chunkSize + 1);
      int endRange = (i == numThreads - 1 ? limit : (i + 1) * chunkSize);

      threads.emplace_back(findPrimes, startRange, endRange, std::ref(primes), std::ref(primesMutex));
  }

  for (auto& thread : threads) {
      thread.join();
  }


  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  std::cout << primes.size() << " primes were found." << std::endl;
  std::cout << "Runtime: " << duration.count() << std::endl;
  return 0;
}

bool check_prime(const int &n) {
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}