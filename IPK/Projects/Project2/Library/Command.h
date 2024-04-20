
#include <string>
#include <cmath>
#include <exception>
#include <stdexcept>

typedef enum {
    ADD,
    SUB,
    DIV,
    MUL,
    INVALID
} Operation;

class Command
{
public:
    static Command *parse(std::string data);
    double execute(bool* error);
private:
    double leftOperand;
    double rightOperand;
    Operation operation;
};