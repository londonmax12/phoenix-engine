#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <sstream>


#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#ifdef PHX_PLATFORM_WINDOWS
	#include<Windows.h>
#endif 


#ifdef PHX_DEBUG_MODE
	#include "Phoenix/Log.h"
#endif // PHX_DEBUG_MODE