// 11. Сгенерировать массив из 300 строк, содержащих случайные 200000 цифр.
// Посчитать число вхождений четных и нечетных цифр в каждой строке, используя N потоков.
// Измерить время программы для N = 1, 2, 4, 10. Измерить работы время программы в каждом случае.

#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>

// Bad practice 2, no access restriction, doesnt matter =]
std::array<std::string, 300> custom_array;

// Function, that counts evens
void counter(int start, int stop, int *even_array) {
	std::string s;
	int even = 0, x = 0;
	for (int i = start; i <= stop; i++) {
		s = custom_array.at(i);
		for (auto c : s) {
			x = c - '0';
			if (x % 2 == 0) {
				even++;
			}
		}
		even_array[i] = even;
		even = 0;
	}
}

// Function, that calls threads
float create_threads(int number_threads, int string_lenght) {
	int threads_piece = (int)(custom_array.size() / number_threads),
		start = 0,
	    stop = threads_piece - 1;
	std::array<int, custom_array.size()> even_array;
	std::vector<std::thread> threads;
	auto begin = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < number_threads; i++)
	{
		threads.push_back(std::thread(counter, start, stop, &even_array[0]));
		start = stop + 1;
		stop += threads_piece;
	}
	for (auto& th : threads)
	{
		th.join();
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - begin;
	for (int i = 0; i < even_array.size() ; i++) {
		std::cout << std::left << "String: " << std::setw(8) << i << " Even: " << std::setw(7) << even_array.at(i) << " Odd: " << std::setw(7) << string_lenght - even_array.at(i) << "\n";
	}
	//threads.clear();
	return (float)duration.count();
}

int main()
{
	// Variables
	int number_threads = 1, 
		string_lenght = 200000;
	srand((unsigned int)time(0));

	// Filling Array. Also need parallelisation for better performance
	float progress = 0, 
		increment = 100.0 / custom_array.size();
	std::cout << "Start of filling array\n";
 	for (int i = 0; i < custom_array.size(); i++) {
		for (int j = 0; j < string_lenght; j++) {
			custom_array.at(i) += std::to_string((rand() % 9));
		}
		progress += increment;
		std::cout << std::left << std::setprecision(3) << std::setw(6) << progress << "%\n";
	}
	std::cout << "Finish of filling array\n\n";

	// Function call & time output
	std::cout << "\nThreads: " << number_threads << " Total time: " << create_threads(number_threads, string_lenght) << "\n\n\n";
	number_threads++;
	std::cout << "\nThreads: " << number_threads << " Total time: " << create_threads(number_threads, string_lenght) << "\n\n\n";
	number_threads += 2;
	std::cout << "\nThreads: " << number_threads << " Total time: " << create_threads(number_threads, string_lenght) << "\n\n\n";
	number_threads += 6;
	std::cout << "\nThreads: " << number_threads << " Total time: " << create_threads(number_threads, string_lenght) << "\n\n\n";

	system("pause");
	return EXIT_SUCCESS;
}