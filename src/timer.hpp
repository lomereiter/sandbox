#ifndef TIMER_HPP
#define TIMER_HPP

#include <sys/time.h>
#include <functional>

void timeit(const std::function<void(void)>& foo, const char* description="") {
	timeval t1, t2, t;
	gettimeofday(&t1, NULL);

	foo();

	gettimeofday(&t2, NULL);
	timersub(&t2, &t1, &t);
 
	std::cout << "TIMEIT: " << description << ": " << 
			 	t.tv_sec + t.tv_usec / 1000000.0 << " s" << std::endl;
}
#endif
