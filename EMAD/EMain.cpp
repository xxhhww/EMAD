#include "Renderer.h"
#include "Exception.h"

#include <iostream>

int main() {
	try {
		return App{ 800, 600 }.run();
	}
    catch (const BaseException& e) {
        std::cout << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "UnKnown Error" << std::endl;
    }
    return -1;
}