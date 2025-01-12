/*
    Author -  Archit Raj Jain
    Feature - Exception.h class containing all the declaration required for throwing Events and its custom exceptions

*/

#include <iostream>
#include <string>
#include <exception>

namespace CustomException{

class InvalidAssignment : public std::exception 
{
    public:
        explicit InvalidAssignment(const std::string& message) : _message(message) {}

        const char* what() const noexcept override {
            return _message.c_str();
        }

    private:
        std::string _message;
};

class InvalidFetching : public std::exception 
{
    public:
        explicit InvalidFetching(const std::string& message) : _message(message) {}

        const char* what() const noexcept override {
            return _message.c_str();
        }

    private:
        std::string _message;
};



class ObjectLock : public std::exception 
{
    public:
        explicit ObjectLock(const std::string& message) : _message(message) {}

        const char* what() const noexcept override {
            return _message.c_str();
        }

    private:
        std::string _message;
};




}