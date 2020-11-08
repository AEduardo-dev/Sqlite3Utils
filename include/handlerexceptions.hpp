#include <iostream>
#include <string>

using namespace std;

class Exception {

public:
   Exception(const string& msg) : msg_(msg) {}
  ~Exception() {}

   string getMessage() const {return(msg_);}
private:
   string msg_;
};
