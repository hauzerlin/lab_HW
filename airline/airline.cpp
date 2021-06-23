#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <conio.h>
#include "check.h"
//#include "passenger.h"

using namespace std;

inline void err_reservation()
{
    cerr<<"No available seats and your reservation is not successfully made\n";
    cout<<"\n\n";
    getch();
    return;
}

//show the main menu and options.
void main_menu()
{
    cout<<"*** Welcome to NCU airline reservation system ***\n";
    for(int i=0;i<49;i++)cout<<"=";
    cout<<endl;
    cout<<"Please type 1 to make a new reservation\n";
    cout<<"Please type 2 to cancel an existing reservation\n";
    cout<<"Please type 3 to show current booking status\n";
    cout<<"Please type 4 to print boarding pass for a record\n";
    cout<<"Please type 5 to save all records in a file\n";
    cout<<"Please type 6 to exit the system\n";
    cout<<"Please type 7 to load data from a file\n";
}

//enter the type number on main menu.
short enter_type()
{
    short type;

    cout<<"Enter your selection Here: ";

    while(cin>>type)
    {
        if(type<=10&&type>=1)
        {
            for(int i=0;i<49;i++)cout<<"=";
            cout<<endl;
            break;
        }
        else
        {
            cout<< "Error type! Please enter your type agian.\n";
        }
    }
    return type;
}

//when select type 1 at main menu, push a new reservation.
void new_reservation(list &ls, bool seats[], bool record[])
{
    //cache variable.
    passenger *psg = new passenger;
    string temp;
    char tmp2[10];
    int dob;
    long tmp;
    short type;

    srand(time(0));

    //enter first name.
    cout<<"Please enter passenger's first name: ";
    cin >> temp;
    psg->set_first(temp);
    
    //enter last name.
    cout<<"Please enter passenger's last name: ";
    cin >> temp;
    psg->set_last(temp);

    //enter "day of birth"
    cout<<"Please enter passenger's DOB: ";
    while(cin >> dob)
    {
        /*int test=dob/10000000;
        if(test<1||test>9)*/
        itoa(dob, tmp2, 10);
        string save(tmp2);
        if(save.length()!= 8)
        {
            cerr<<"Wrong format! \nPlease enter passenger's DOB again: ";
            continue;
        }
        else  break;
    }
    psg->set_DOB(dob);

    //enter passport number.
    cout<<"Please enter passenger's passport number: ";
    while(cin >> tmp)
    {
        ltoa(tmp, tmp2, 10);
        string save(tmp2);
        if(save.length()!=9)
        {
            cerr<<"Wrong format! \nPlease enter passenger's passport number again: ";
            continue;
        }
        else  break;
    }
    psg->set_passport_num(tmp);

    //select type of seat
    cout<<"Please type 1 for First Class\n";
    cout<<"Please type 2 for Economy Class: ";
    cin>>type;

    if(type == 1)//if first class is secelted.
    {
        if(!first_full(seats))assign_first(psg, seats);
        else if(!economy_full(seats))
        {
            //if "yes" is taken, assign a economy class seat.
            if(ask_change(2)==1)assign_economy(psg, seats);
            else {err_reservation();return;}
        }
        else {err_reservation();return;} 
    }
    else if(type == 2)//if economy class is secelted.
    {
        if(!economy_full(seats))assign_economy(psg, seats);
        else if(!first_full(seats)) 
        {
            //if "yes" is taken, assign a first class seat.
            if(ask_change(1)==1)assign_first(psg, seats);
            else {err_reservation();return;}
        }
        else {err_reservation();return;}
    }

    //Set up passenger's record locator.
    if(!record_full(record))assign_record(psg, record);    
    else return;
    
    //prsent list
    ls.Push(psg);

    cout<< "*** The reservation is successfully made! ***\n";
    psg->display_data();
    cout<<"\n\n";

    getch();

}

void cancel_reservation(list &ls, bool seats[], bool record[])
{
    short rd;
    string test;
    passenger *ptr=ls.first;
    cout<<"Please enter record locator: ";
    cin>>rd;
    cout<<"Please enter the first name of the psg:";
    cin>>test;

    ptr= locate_rd(ls, ptr, rd);

    if(ptr->record != rd)//can't find the rd.
    {
        cout<<"“The record locator does not exist\n\n"<<endl;    
        getch();
        return;
    }
    else if(test!=ptr->first_name)//can't match the first name,
    {
        cerr<<"The first name and the record locator is not matched\n\n"<<endl;
        getch();
        return;
    }
    else //all match, check to cancel the reservation.
    {
        cerr<<"*** The reservation is successfully found! ***"<<endl;
        ptr->display_data();
        cerr<<"Are you sure to cancel the above reservation?(Y/N)";
        cin>>test;
        if(test[0]=='y'||test[0]=='Y')
        {
            seats[ptr->return_seat_num()-1]=0;
            record[ptr->record-1]=0;
            ls.POP(ptr);

            cerr<<"The reservation is successfully cancelled...\n\n"<<endl;
            getch();
        }
        else return;
    }
    
    
}

void show_book_status(list &ls, bool seats[], bool record[])
{
    for(int i=0;i<55;i++)cout<<"*";cout<<endl;
    cout<<"Seat capacity:"<<endl;
    cout<<setw(15)<<"Sold";
    cout<<setw(16)<<"Available";
    cout<<setw(12)<<"Total"<<endl;

}


//when select type 4 at main menu, print record on screen.
void print_record(list& ls)
{   
    short rd;
    cout<<"Please enter record locator: ";
    cin>>rd;
    while(!ls.rd_exist(rd))
    {
        cout<<"The record locator you entered is wrong!\n";
        cout<<"Please enter your record locator again: ";
        cin>>rd;
    }
    ls.boarding_pass(rd);
    cout<<"\n\n\n";

}

//when select type 6 at main menu, exit the system.
void exit_system()
{
    cout<<"Thank you for using our system\n";
    cout<<"Have a nice trip~~";
    cout<<"\n\n\n";
}

void read_from_file(list &ls, bool seats[], bool record[])
{
    ifstream psg_in;
    string filename;

    //open the file
    cout<<"Please enter the file name: ";
    cin>>filename;
    psg_in.open(filename, ios::binary | ios::in);
    if(!psg_in.is_open())
    {
        cerr<<"The file "<< filename <<"does not exist.\n";
        cout<<"\n\n";
        return;
    }

    psg_in.seekg(65, ios::beg);
    while(psg_in.peek()!=EOF)
    {
        passenger *in = new passenger;
        ls.Push(in);

        int dob;
        string tmp;
        char name[20];
        long ps_num;
        short type, seat_num, rd;

        psg_in>>rd;
        if(!record[rd-1]){in->set_record(rd);record[rd-1]=1;}
        else if(record[rd-1] && !record_full(record))
        {
            cerr<<"!Warning: The RR "<< rd <<"in file ...\n";
            cerr<<"The record locator"<< rd <<" is already used\n";
            assign_record(in,record);
            cerr<<"The record with RR "<< rd <<" in the file is update to ";
            cerr<< in->record <<".\n";
        }
        else 
        {
            cerr<<"No more seat is available/0\n";
            return;
        }

        psg_in>>tmp;
        in->set_first(tmp);

        psg_in>>tmp;
        in->set_last(tmp);

        psg_in>>dob;
        in->set_DOB(dob);

        psg_in>>ps_num;
        in->set_passport_num(ps_num);

        psg_in>>tmp;
        if(tmp[0]=='F')
        {   
            if(!first_full(seats))
            {
                in->set_type(1);
                psg_in>>seat_num;
                if(seats[seat_num-1])assign_first(in, seats);
                seats[seat_num-1]=1;
                in->set_seat(seat_num);

            }
            else if(ask_change(2)==1)
            {
                in->set_type(2);
                assign_first(in, seats);
            }
            else 
            {
                cerr<<"No more seat is available/0\n";
                return;
            }
        }
        else
        {   
            if(!economy_full(seats))
            {
                in->set_type(2);
                psg_in>>seat_num;
                if(seats[seat_num-1])assign_economy(in, seats);
                seats[seat_num-1]=1;
                in->set_seat(seat_num);
            }
            else if(ask_change(1)==1)
            {
                in->set_type(1);
                assign_economy(in, seats);
            }
            else 
            {
                cerr<<"No more seat is available/0\n";
                return;
            }
        }
        
    }    

    psg_in.close();

    cout<<"The booking record is successfully saved to the file";
    cout<<"\n\n";

}

void check_list(bool seats[], bool record[])
{
    for(int i=0;i<10;i++)
    {
        cout<<"seats["<<i<<"]: "<<seats[i]<<"  ";
        if(i==4)cout<<"\n";
    }

    cout<<"\n\n";

    for(int i=0;i<10;i++)
    {
        cout<<"record["<<i<<"]: "<< record[i] <<"  ";
        if(i==4)cout<<"\n";
    }
    
    cout<<"\n\n";

}