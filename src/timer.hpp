#ifndef TIMER_HPP
#define TIMER_HPP

#include <sys/time.h>
#include <functional>
#include <iostream>

double timeit(const std::function<void(void)>& foo, const char* description="", bool print=true) {
	timeval t1, t2, t;
	gettimeofday(&t1, 0);

	foo();

	gettimeofday(&t2, 0);
	timersub(&t2, &t1, &t);

    double result = t.tv_sec + t.tv_usec / 1000000.0;

    if (print)
        std::cout << "TIMEIT: " << description << ": " << result << " s" << std::endl;

    return result;
}
#endif
