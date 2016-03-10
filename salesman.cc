/*
    Nathan Gibson
    March 9, 2016
    
    
*/

#include <iostream>
#include <string>
#include <cmath>
#include <iomanip> 

using namespace std;

//constants for max and minimum stack size
const int STACK_MAX_SIZE = 32;
const int EMPTY_STACK_SIZE = -1;

/* 
    Class that implements the stack abstract data type
*/
template <class T>
class Stack{
    
    private:
        T stack[STACK_MAX_SIZE];
        int top_pointer;           
    
    public:
        /*
            Initialize the stack to an empty state
        */
        Stack(){
            top_pointer = EMPTY_STACK_SIZE;
        }
        
        /*
            Move the top_pointer down to remove the top stack entry,
            throws a runtime_error if the stack is empty
        */
        void pop(){
            if(!empty()){
                top_pointer--;
            }else{
                throw runtime_error("Stack Underflow");   
            }
        }
    
        /*
            Puts an element on the top of the stack, throws a runtime_error
            if the stack is too full
        */
        void push(T t){
            if(!full()){
                stack[++top_pointer] = t;    
            }else{
                throw runtime_error("Stack Overflow");  
            }
        }
    
        /*
            Returns the element on the top of the stack, throws a runtime_error
            if the stack is empty
        */
        T top(){
            if(!empty()){
                return stack[top_pointer];    
            }else{
                throw runtime_error("Empty Stack");
            }
        }
        
        /*
            returns true iff the stack is empty, otherwise false
        */
        bool empty(){
            return top_pointer == EMPTY_STACK_SIZE ? true : false;
        }
    
        /*
            returns true iff the stack is full, otherwise false
        */
        bool full(){
            return top_pointer == STACK_MAX_SIZE - 1 ? true : false;
        }
    
        /*
            Overloads the output operator, outputs all the elements on the 
            stack, starting at the top and working down. Elements are seperated
            by a newline.
        */
        friend ostream& operator<<(ostream& os, const Stack& s){
            for(int i = s.top_pointer; i >= 0; i--){
                os << s.stack[i] << endl;
            }
            return os;
        }
};

/*
    Represents all of the data that a salesman collects from
    each call
*/
class Card{
    
    private:
        int received;
        int x;
        int y;
        int duration;
    
    public:
        /*
            Default constructor for a Card, compiler dislikes no default
        */
        Card(){
            received = 0;
            x = 0;
            y = 0;
            duration = 0;
        }
    
        /*
            Custom constructor intializes elements
        */
        Card(int _received, int _x, int _y, int _duration){
            received = _received;
            x = _x;
            y = _y;
            duration = _duration;
        }
    
        /*
            Getter for received
        */
        int get_received(){
            return received;
        }
    
        /*
            Getter for x
        */
        int get_x(){
            return x;
        }
    
        /*
            Getter for y
        */
        int get_y(){
            return y;
        }
    
        /*
            Getter for duration
        */
        int get_duration(){
            return duration;
        }
    
        /*
            Overloads the output operator, outputs each element of a Card - space 
            seperated
        */
        friend ostream& operator<<(ostream& os, const Card& s){
            os << setw(10) << s.received << " " << setw(10) << s.x << " " << setw(10) << s.y << " " << setw(10) << s.duration;
            return os;
        }
        
};

int main(int argc, char* argv[]){
    
    Stack<Card> s;
    
    int received;
    int x;
    int y;
    int duration;
    
    int currentx = 0;
    int currenty = 0;
    int time = 0;
    
    
    while(cin >> received && cin >> x && cin >> y && cin >> duration){
        
        for (; true; time++){
            if (time == received){
                Card c(received, x, y, duration);
                cout << time << c << endl;
                s.push(c);
                break;
            }else{
                cout << time << endl;
            }
        }       
    }
}