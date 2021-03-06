/*
    Nathan Gibson
    March 9, 2016
    
    Version of the traveling salesman that uses a stack to manage
    candidates to visit. 
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
            Move the top_pointer down to remove the top stack entry
        */
        void pop(){
            if(!empty()){
                top_pointer--;
            }else{
                throw runtime_error("Stack Underflow");   
            }
        }
    
        /*
            Puts an element on the top of the stack
        */
        void push(T t){
            if(!full()){
                stack[++top_pointer] = t;    
            }else{
                throw runtime_error("Stack Overflow");  
            }
        }
    
        /*
            Returns the element on the top of the stack
        */
        T top(){
            return stack[top_pointer];    
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
        int picked_up;
    public:
        /*
            Default constructor for a Card, compiler dislikes no default
        */
        Card(){
            received = 0;
            x = 0;
            y = 0;
            duration = 0;
            picked_up = -1;
        }
    
        /*
            Custom constructor intializes elements
        */
        Card(int _received, int _x, int _y, int _duration){
            received = _received;
            x = _x;
            y = _y;
            duration = _duration;
            picked_up = -1;
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
            Setter for picked_up
        */
        void set_picked_up(int time){
            picked_up = time;
        }
        
        /*
            Getter for picked_up
        */
        int get_picked_up(){
           return picked_up;
        }
    
        /*
            Returns -1 as an error code if not set
        */
        int get_wait_time(){
            if(picked_up == -1){
                return -1;
            }else{
                return picked_up - received;
            }
        }
    
        /*
            Overloads the output operator, outputs each element of a Card 
            - space seperated
        */
        friend ostream& operator<<(ostream& os, const Card& s){
            os << setw(10) << s.received << setw(10) << s.x <<
                setw(10) << s.y << setw(10) << s.duration << setw(10) << s.picked_up;
            return os;
        }
        
};

Card get_next(Stack<Card>& s, int currentx, int currenty);
void wait(int& time_in_bookstore);
int distance(int x1, int x2, int y1, int y2);
Stack<Card> get_input();

int main(int argc, char* argv[]){
    
    //manage simulation events
    int currentx = 0;
    int currenty = 0;
    int time = 1;
    int next_meet = 0;

    //statistic tracking variables 
    int time_in_bookstore = 0;
    int total_number_calls = 0;
    int time_on_road = 0;
    int time_meeting_clients = 0;
    
    //places to go, places we haven't been, places we've already been
    Stack<Card> places_to_visit;
    Stack<Card> input = get_input();
    Stack<Card> used;
    
    while(true){
            
        //if done visiting and nowhere else to go, stop
        if(input.empty() && places_to_visit.empty() && next_meet == 0){
            break;
        }
        
        //if done with a meeting, try to get a new one -- if cannot, wait
        if(next_meet == 0){
            
            try{
                Card next = get_next(places_to_visit, currentx, currenty);
                
                //figure out how far it is to the new location
                int distance_traveled = distance(currentx, currenty, 
                                               next.get_x(), next.get_y());
                
                //drive to location
                for(int i = 0; i < distance_traveled - 1; i++){
                    //check for calls while traveling 
                    Card call = input.top();
                    if(call.get_received() == time){
                        total_number_calls++;
                        places_to_visit.push(call);
                        input.pop();
                    }
                    time++;
                }

                next.set_picked_up(time);
                used.push(next);
                
                //update how far we have gone
                time_on_road += distance_traveled;
                
                //set location to where we traveled
                currentx = next.get_x();
                currenty = next.get_y();
                
                //meeting with a client for this duration
                next_meet = next.get_duration();
                
                //update time spent meeting with clients
                time_meeting_clients += next.get_duration();

            }catch(...){
                wait(time_in_bookstore);
            }
        }else{
            //if not done with a meeting, decrement time spent in the current meeting
            next_meet--;   
        }
        
        //if a call is received, add it to the places to visit
        Card call = input.top();
        if(call.get_received() == time){
            total_number_calls++;
            places_to_visit.push(call);
            input.pop();
        }
        
        time++; 
    }
    
    //calculate wait time statistics
    int client_waiting_max = 0;
    int total = 0;
    
    while(!used.empty()){
        int current = used.top().get_wait_time();
        total += current;
        used.pop();
        if(current > client_waiting_max){
            client_waiting_max = current;
        }
    }
    
    int client_waiting_average = total / total_number_calls;
    
    //output
    cout << "It took " << time << " minutes for the salesman to process " 
        << total_number_calls << " calls. " << endl;
    
    cout << "The salesman spent " << time_in_bookstore 
        << " minutes in bookstores, " 
        << time_on_road << " minutes on the road, and" << endl;
    
    cout << time_meeting_clients << " minutes in meetings with clients." 
        << endl;
    
    cout << "Clients spent an average of " << client_waiting_average 
        << " minutes waiting for the salesperson to see them." << endl;
    
    cout << "The maximum amount of time any client spent waiting was " 
        << client_waiting_max << "." << endl;
        
}

/*
    Reads input into a stack, then reverses it so it appears
    in the original order when being popped off
*/    
Stack<Card> get_input(){
    Stack<Card> input;
    Stack<Card> flipped;
    int received;
    int x;
    int y;
    int duration;
    
    while(cin >> received && cin >> x &&
           cin >> y && cin >> duration){
        input.push(Card(received, x, y, duration));
    }
    
    while(!input.empty()){
        flipped.push(input.top());
        input.pop();
    }
    
    return flipped;
}

/*
    Given a stack, and a location, this function takes the
    first two entries off the stack and determines which is the 
    closest to the current location. The closest is returned, the
    other is put back on the stack. If the stack is empty an 
    exception is thrown, if there is only one card-- it is returned.
    If they are equal distance the oldest card is returned.
*/
Card get_next(Stack<Card>& s, int currentx, int currenty){
    
    if(s.empty()){
        throw runtime_error("No cards");
    }
    
    Card first = s.top();
    s.pop();
    
    if(s.empty()){
        return first;
    }
    
    Card second = s.top();
    s.pop();
    
    int first_dist = distance(currentx, first.get_x(),
                                currenty, first.get_y());
    
    int second_dist = distance(currentx, second.get_x(),
                                 currenty, second.get_y());
    
    if(first_dist < second_dist){
        s.push(second);
        return first;
    }else{
        s.push(first);
        return second;
    }
}

/*
    returns distance between two points on a 2d grid
*/
int distance(int x1, int y1, int x2, int y2){
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1),2));
}

/*
    function that has the side effect of increasing the time_in_bookstore
    by one
*/
void wait(int& time_in_bookstore){
    time_in_bookstore++;
}
