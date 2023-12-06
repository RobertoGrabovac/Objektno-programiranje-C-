#include "log.h"

Log::Log(std::string sender, int priority, std::array<char, 3> code, std::string message) : 
_sender{sender}, _priority{priority}, _code(code), _message(message) {}

