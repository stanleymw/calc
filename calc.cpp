// calc -> Simple calculator written in C++ by stanleymw
#include <iostream>
#include <limits>
#include <iomanip>     
#include <stack>
#include <unordered_map>
#include <string>
#include <cmath>
using namespace std;
typedef std::numeric_limits<double> doubleLim;

string version = "0.3";

bool debugMode = false;

char closers[3] = {']', ')', '}'};

unordered_map<char, int> priority = {
  {'+', 1},
  {'-', 1},
  {'*', 2},
  {'/', 2},
  {'^', 3},
  {'(', 0},
  {')', 0},
  {'#', -1},
  };



bool isCloser(char inp) {
  for (int i = 0; i < 3; i++) {
    if (inp == closers[i]) {
      return true;
    }
  }
  return false;
}

double charToNum(char inp) {
  return (double) inp - '0';
}

bool isOperand(char inp) {
  if (priority.find(inp) != priority.end()) {
    return true;
  }
  return false;
}

double performOperation(double operand1, double operand2, char operation) {
  //printf("PERFORMING OPERATION %c on OPERAND1: %f, OPERAND2: %f\n", operation, operand1, operand2);
  switch (operation) {
    case '*':
      return operand1*operand2;
    case '/':
      return  operand1/operand2;
    case '^':
      return pow(operand1, operand2);
    case '+':
      return operand1+operand2;
    case '-':
      return operand1-operand2;
  }
  return 31415926;
}

template <typename chosentype>
void printStack(stack<chosentype> s) {
  stack<chosentype> copy = s;
  while (!copy.empty()) {
    cout << copy.top() << " ";
    copy.pop();
  }
  printf("\n");
}
void printStackChar(stack<char> s) {
  stack<char> copy = s;
  while (!copy.empty()) {
    printf("%c ", copy.top());
    copy.pop();
  }
  printf("\n");
}


int runCalc() {
  // prompt
  if (debugMode) {
    printf("[DEBUG] ");
  }
  printf("calc> ");
  string expr;
  cin >> expr;

  stack<char> operators;
  stack<double> nums;

  // 2^3+5/3*(231-130)+10000
  // 2^3+5/3*101.123456789
  // 2^3+4/3*2

  // 1+2+3*4^2+1
  // 1 + 2 + 3 * 16 + 1
  // 1 + 2 + 48 + 1
  
  //int parenstart = 0;

  int decimalplacesdown = 1;
  bool decimalmode = false;
  //bool parenmode = true;
  double currentnum = -5555;
  bool isminus = true;
  bool curminus = false;
	
  
  for(char i: expr) {
    if (isOperand(i)) {
      // reset number info
      decimalmode = false;
      decimalplacesdown = 1;
      if (currentnum != -5555) {
        if (curminus) {
          currentnum *= -1;
        }
        nums.push(currentnum);
        isminus = false;
      }
      currentnum = -5555;
      curminus=false;

      if (i == '-' && isminus) {
        // thjis is minus sihgn
        curminus=true;
        continue;
      }
      
      if (i == '(') {
        // this is start parenthese
        isminus=true;
        //parenstart = operators.size();
        //parenmode = true;
        operators.push(i);
        continue;
      }
      else if (i == ')') {
        // this is an end parenthese
        //printf("HIT END PARENTHESE, PARENSTART: %d\n", parenstart);
        //parenmode = false;
        isminus = false;
        
        // while (operators.size() > parenstart && operators.size() > 0) {
        while (operators.top() != '(') {
          char op = operators.top();
          operators.pop();

          if (op == '(') {
            break;
          }
  
          double operand2 = nums.top();
          nums.pop();
          double operand1 = nums.top();
          nums.pop();
  
          double result = performOperation(operand1, operand2, op);
  
          nums.push(result);
        }
        //parenstart = 0;
        operators.pop();
        
        continue;
      } // end of left paren check
      // this is a normal operator
      while (operators.size() > 0 && priority[i] <= priority[operators.top()]) {
          char op = operators.top();
          operators.pop();
  
          double operand2 = nums.top();
          nums.pop();
          double operand1 = nums.top();
          nums.pop();
  
          double result = performOperation(operand1, operand2, op);
  
          nums.push(result);
        }
      operators.push(i);
    } else {
      // this is a number
      isminus = false; 
      if (currentnum == -5555) {
        currentnum = 0;
      }
      if (i == '.') {
        decimalmode = true;
	continue;
      }
      if (decimalmode) {
        //printf("ADDED DECIMAL: %f\n", charToNum(i) * (pow(0.1,decimalplacesdown)));
        currentnum += charToNum(i) * (pow(0.1,decimalplacesdown));
        decimalplacesdown+=1;
      }
      else {
        //printf("SET CURRENT NUM\n");
        currentnum *= 10;
        currentnum += charToNum(i);
      }
      
    }
    if (debugMode) {
      printf("DECIMAL PLACES DOWN: %d\nCURRENT NUM: %f\n DECIMALMODE: %d\n", decimalplacesdown, currentnum, decimalmode);
      cout << "NUMS: ";
      printStack<double>(nums);
      cout << "OPERATORS: ";
      printStackChar(operators);
      cout << "---------------------\n";
    }
  }

  if (currentnum != -5555) {
    nums.push(currentnum);
  }

  if (debugMode) {
    cout << "\n\n\n FINAL---------------------\n";
    printf("DECIMAL PLACES DOWN: %d\nCURRENT NUM: %f\n DECIMALMODE: %d\n", decimalplacesdown, currentnum, decimalmode);
      cout << "NUMS: ";
      printStack<double>(nums);
      cout << "OPERATORS: ";
      printStackChar(operators);
      cout << "---------------------\n";
  }


  stack<double> currentlowest;
  stack<char> currentoperators;
  while (!operators.empty()) {
    char op = operators.top();
    operators.pop();

    double operand2 = nums.top();
    nums.pop();
    double operand1 = nums.top();
    nums.pop();

    double result = performOperation(operand1, operand2, op);

    nums.push(result);
  }

  double last = nums.top();
  //printf("%lf\n", last);
  cout.precision(doubleLim::max_digits10);
  cout << last << "\n";

  return 0;
}

int main() {
  cout << "Welcome to calc " << version << "\n";
  while (true) {
  runCalc();
  }
return 0;
}
