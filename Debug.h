#include <iostream>
enum debug_option
{
	DEBUG_DISABLE,
	DEBUG_ENABLE
};

class debug
{
public:
	debug_option debug_state;

	debug() : debug_state(DEBUG_ENABLE) {} // constr
	debug(debug_option state) : debug_state(state) {} // constr

	template<typename T>
	debug & operator<< (T input)
	{
		if (this->debug_state == DEBUG_ENABLE)
			std::cout << input;
		return *this;
	}
};