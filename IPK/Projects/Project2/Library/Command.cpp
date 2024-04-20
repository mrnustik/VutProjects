#include "Command.h"

using namespace std;

//function used to truncate number on x Digits
double truncateNumber(double In, unsigned int Digits)
{
    double f=pow(10, Digits);
    return ((int)(In*f))/f;
}

Command* Command::parse(string expression)
{
    long leftOperand, rightOperand;
    string numString = "";
    Operation o;
    unsigned long opPosition = -1;
    if((opPosition = expression.find('+')) != string::npos) o = ADD;
    else if((opPosition = expression.find('-')) != string::npos) o = SUB;
    else if((opPosition = expression.find('*')) != string::npos) o = MUL;
    else if((opPosition = expression.find('/')) != string::npos) o = DIV;
    else {
        return nullptr;
    }
    try {
        numString = expression.substr(0, opPosition - 1);
        leftOperand = stol(numString);

        numString = expression.substr(opPosition + 1);
        rightOperand = stol(numString);
    }
    catch (const out_of_range& exception)
    {
        Command* command = new Command;
        command->leftOperand = 0;
        command->rightOperand = 0;
        command->operation = INVALID;
        return command;
    }
    catch (...)
    {
        return nullptr;
    }

    Command* command = new Command;
    command->leftOperand = leftOperand;
    command->rightOperand = rightOperand;
    command->operation = o;
    return command;
}

double Command::execute(bool* error)
{
    double result = 0.0;
    *error = false;
    switch(this->operation)
    {
        case ADD:
            result = this->leftOperand + this->rightOperand;
            break;
        case SUB:
            result = this->leftOperand - this->rightOperand;
            break;
        case DIV:
            if(this->rightOperand == 0)
            {
                *error = true;
                return NAN;
            }
            result = this->leftOperand / this->rightOperand;
            break;
        case MUL:
            result = this->leftOperand * this->rightOperand;
            break;
        case INVALID:
            *error = true;
            return 0;
            break;
    }
    return truncateNumber(result, 2);
}

