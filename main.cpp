#include <iostream>
#include<windows.h>
#include<mysql.h>
#include<string>
#include <sstream>
#include<cstdio>
#include<ctime>



using namespace std;
        MYSQL* conn;
        MYSQL_ROW row;
        MYSQL_RES *res;
        int qstate;

MYSQL* connecting(){
    char *server ="localhost";
   char * user = "root";
    char *password = "";
    char *db = "cooperative";


    conn = mysql_init(NULL);
    conn = mysql_real_connect(conn, server, user, password,db, 0 , NULL,0);
return conn;
 }

 struct Date
{
int d, m, y;
};
// To store number of days in all mont
const int monthDays[12] = {31, 28, 31,
30, 31, 30,31,31,30,31,30,31};
// This function counts number of leap
// given date
int countLeapYears(Date d)
{
int years = d.y;
// Check if the current year needs
// for the count of leap years or
if (d.m <= 2)
years--;
// An year is a leap year if it is
// multiple of 400 and not a multi
return years / 4 - years / 100 + years;
}
// This function returns number of days
// dates
int getDifference(Date dt1, Date dt2)
{
// COUNT TOTAL NUMBER OF DAYS BEFO
// initialize count using years and
long int n1 = dt1.y*365 + dt1.d;
// Add days for months in given da
for ( int i=0; i<dt1.m - 1; i++)
n1 += monthDays[i];
// Since every leap year is of 366
// Add a day for every leap year
n1 += countLeapYears(dt1);
// SIMILARLY, COUNT TOTAL NUMBER O
long int n2 = dt2.y*365 + dt2.d;
for ( int i=0; i<dt2.m - 1; i++)
n2 += monthDays[i];
n2 += countLeapYears(dt2);
// return difference between two co
return (n2 - n1);
}

void Register(){
    cout<<"\n\n************************* REGISTER ************************\n\n";
        //connecting();
        string name;
        string username;
        string password;
        string gender;
        string deposit;
        cout<<"What is your name? \n";
        //getline(cin, name);
        cin>>name;

        cout<<name<<" choose your Username \n";
        cin>>username;
        cout<<name<<" choose your Password \n";
        cin>>password;
        cout<<name<<" choose your Gender \n \n";
        cout<<"\n 1) Male \n 2) Female \n";
        int c;
        cin>>c;
        switch(c){
        case 1: gender = "Male";
        break;
        case 2: gender = "Female";
        break;
        default :gender = "Unknown";
        }
        cout<<"Enter your Initial Deposit\n";
        cin>>deposit;

        string in = "INSERT INTO members(id, name, username, password, gender, deposit ) VALUES('NULL','"+ name + "','"+ username +"','"+ password + "','"+ gender +"','" + deposit+ "' ) ";
        const char* q = in.c_str();
        mysql_query(connecting(), q);
        if(q){
            cout<<"Registered successfully \n";
        }else{
        cout<<"register fail \n";
        }
}
string currentTime(){
          string tim="";
          time_t now = time(0);
     tm *current_time = localtime(&now);
     int year = 1900 + current_time->tm_year;
     int month = 1+ current_time->tm_mon;
     int day = current_time->tm_mday;
//     tim = year"-"+month+"-"+day;
     char buff[11];
     sscanf(tim.c_str(), "%d-%d-%d", &year,&month, &day);
     sprintf(buff, "%02d-%d-%d", year,month,day);
     return buff;
}
void MyAccount(string username){
        cout<<"\n\n************************* MY ACCOUNT  ************************\n\n";
        string  sel = "SELECT* FROM members WHERE username = '"+username+"' ";
        const char* s = sel.c_str();
        qstate = mysql_query(connecting(), s);
        res =  mysql_store_result(conn);
        row = mysql_fetch_row(res);
        cout<<" Serial number :   "<<row[0]<<"\n \n Name :  "<<row[1]<<"\n\n Username :  "<<row[2]<<"\n\n Gender  :  "<<row[4]<<"\n\n My Current Deposit :  "<<row[5];
        cout<<"\n\n My Total Amount : "<<row[6]<<"\n \n My Loan  : "<<row[7]<<"\n\n Date Register : "<<row[8]<<"\n";
}
void loan_duration(string username){
     cout<<"\n\n*************************  ************************\n\n";
        string  sel = "SELECT* FROM members WHERE username = '"+username+"' ";

        const char* s = sel.c_str();

        qstate = mysql_query(connecting(), s);
        res =  mysql_store_result(conn);
        row = mysql_fetch_row(res);
        string loan_date = row[10];

     char buff[11];
     int a,b,c;
     sscanf(loan_date.c_str(), "%d-%d-%d", &a,&b, &c);
     sprintf(buff, "%02d-%d-%d", a,b,c);


     time_t now = time(0);
     tm *current_time = localtime(&now);
     int year = 1900 + current_time->tm_year;
     int month = 1+ current_time->tm_mon;
     int day = current_time->tm_mday;
     Date f = {day, month,year};
     Date g ={c, b,a};
     int da= getDifference(g, f);

     ostringstream st;
        st<<da;
        string days = st.str();

     string update = "UPDATE members SET duration= '"+days+"' WHERE username = '"+username+"'";
        const char* t = update.c_str();
         qstate = mysql_query(connecting(), t);
}



void pay_loan(string username){
     loan_duration(username);

     cout<<"\n\n************************* PAY LOAN  ************************\n\n";
        string  sel = "SELECT* FROM members WHERE username = '"+username+"' ";
        const char* s = sel.c_str();
        qstate = mysql_query(connecting(), s);
        res =  mysql_store_result(conn);
        row = mysql_fetch_row(res);
        const char *init = row[7];

        //amount of current loan
        int current_loan = atoi(init);
        //amount to be paid
        float cur;
         const char *d = row[11];

        //amount of current loan
        int day = atoi(d);
        //amount to be paid

        //six month return
        float loan;

        //loan += loan*0.08;
        //loan += loan*0.05;
        float interest;


        if(day <= 180){
          interest = current_loan*0.08;
          loan +=interest+current_loan;
          cout<<"You have borrow money since "<<day<<"\'s ago and you are to pay an interest of 8%"
          <<"\nInterest : "<<interest<<"\n Your Loan is : "<<current_loan
          <<"\n Your are to pay : "<<loan <<"\n\n";
        }else{
          interest = current_loan*0.05;
          loan +=interest+current_loan;
          cout<<"You have borrow money since "<<day<<"\'s ago and you are to pay an interest of 5%"
          <<"\nInterest : "<<interest<<"\n Your Loan is : "<<current_loan
          <<"\n Your are to pay : "<<loan <<"\n\n";
        }
        cout<<init;
        cout<<"\n\n Enter Amount to Pay Back\n\n";
        cin>>cur;
        float  pay = loan-cur;
        ostringstream st;
        st<<pay;
        string amount = st.str();
        cout<<amount<<"\n";


        string update = "UPDATE members SET loan = '"+amount+"' WHERE username = '"+username+"'";
        const char* t = update.c_str();
         qstate = mysql_query(connecting(), t);
         if(t){
            cout<<username <<" paid "<<amount<<" successfully\n\n";
         }else{
         cout<<"Unable to pay back \n";
         }

}


void deposit(string username){
       cout<<"\n\n************************* DEPOSIT  ************************\n\n";
        string  sel = "SELECT* FROM members WHERE username = '"+username+"' ";
        const char* s = sel.c_str();
        qstate = mysql_query(connecting(), s);
        res =  mysql_store_result(conn);
        row = mysql_fetch_row(res);
        const char *init = row[5];
        int a = atoi(init);
        float cur;
        cout<<init;
        cout<<"\n\n Enter Amount to Deposit\n\n";
        cin>>cur;
        float  amt = cur+a;
        ostringstream st;
        st<<amt;
        string amount = st.str();
        cout<<amount<<"\n";


        string update = "UPDATE members SET amount = '"+amount+"' WHERE username = '"+username+"'";
        const char* t = update.c_str();
         qstate = mysql_query(connecting(), t);
         if(t){
            cout<<"Money added successfully\n\n";
         }else{
         cout<<"Unable to Deposit \n";
         }
}
void register_duration(string username){
     cout<<"\n\n*************************   ************************\n\n";
        string  sel = "SELECT* FROM members WHERE username = '"+username+"' ";

        const char* s = sel.c_str();

        qstate = mysql_query(connecting(), s);
        res =  mysql_store_result(conn);
        row = mysql_fetch_row(res);
        string date_reg = row[8];

     char buff[11];
     int a,b,c;
     sscanf(date_reg.c_str(), "%d-%d-%d", &a,&b, &c);
     sprintf(buff, "%02d-%d-%d", a,b,c);
     cout<<buff<<"\t"<<a<<"\t"<<b<<"\t"<<c;

     time_t now = time(0);
     tm *current_time = localtime(&now);
     int year = 1900 + current_time->tm_year;
     int month = 1+ current_time->tm_mon;
     int day = current_time->tm_mday;
     Date f = {day, month,year};
     Date g ={c, b,a};
     int da= getDifference(g, f);
     ostringstream st;
        st<<da;
        string days = st.str();

     string update = "UPDATE members SET days = '"+days+"' WHERE username = '"+username+"'";
        const char* t = update.c_str();
         qstate = mysql_query(connecting(), t);
}

void Loan(string username){
     register_duration(username);
    cout<<"\n\n************************* LOAN  ************************\n\n";
        string  sel = "SELECT* FROM members WHERE username = '"+username+"' ";

        const char* s = sel.c_str();

        qstate = mysql_query(connecting(), s);
        res =  mysql_store_result(conn);
        row = mysql_fetch_row(res);
//string day = row['DAY'];
        string user = row[1];
        string date_register = row[8];
        const char* amt = row[6];
        float amot = atoi(amt);

        const char* d = row[9];
        int total_day = atoi(d);

        if (total_day >= 180){
        //check if the user have money with us before grant loan
        //user must can only borrow 2x of his total amount

        float qualify = amot*2;



        const char* prev = row[7];
        float previus_loan = atoi(prev);

        cout<<"\nWelcome "<<user<<"\n";
       cout<<"\nYou qualify for Loan because you have be with us "<<total_day <<" days ago\n\n";
       cout<<"Your Previous Loan : "<<previus_loan<<"\n\n";
       cout<<"Your Current Amount : "<<amot<<"\n\n";
       cout<<"You can Loan still Borrow up to : "<<(qualify-previus_loan)<<"\n\n";
        float lo;
        cout<<user <<"  Request for Your Loan \n\n";
        if(previus_loan <= qualify){
        cout<<"Enter the Amount of Loan \n";
        cin>>lo;
        if(lo <= qualify){
        ostringstream str;
        str<<lo;
        string loan = str.str();
        string currentdate = currentTime();
        string l = "UPDATE members SET loan = '"+loan+"',date_loan = '"+currentdate+"'  WHERE username = '"+username+"'";
        const char* ss = l.c_str();
        qstate = mysql_query(connecting(), ss);
        if(ss){
            cout <<"Your Loan is successful Granted \n";
        }else{
        cout<<"Fail to Grant loan to you\n\n";
        }
        }
        else{
         cout<<"You can't borrow more than 2x of your Saving \n";
         cout<<"\n You can only borrow up to "<<(qualify-previus_loan)<<"\n";
        }

}
 else{
            cout<<"You have an Unpaid "<<previus_loan<<"\n";
        }
}else{
     cout<<"You are not qualify yet because you just registered with us "<<total_day<<" days ago\n";
}
}
void Logout();

void dashboard(string username){
    cout<<"\n\n************************* MY DASHBOARD  ************************\n\n";
    cout<<"\n\n Welcome "<<username<<"\n";

    int che ;
    do{
    cout<<"\n 1) Loan \n\n 2) Deposit \n\n 3) My Account";
    cout<<"\n\n 4) Pay Loan \n\n 5) Check my Register Duration";
    cout<<"\n\n 6) Logout\n\n";
    cout<<"Choose What to do \n";
    cin>> che;
    switch(che){
        case 1: Loan(username);
        break;
        case 2 : deposit(username);
        break;
        case 3: MyAccount(username);
        break;
        case 4: pay_loan(username);
        break;
        case 5: register_duration(username);
        break;
        case 6: Logout();
        default : cout<<"Invalid Input\n";
    }
    }
    while(che != 0);

}
void Login(){
    cout<<"\n\n************************* LOGIN  ************************\n\n";
        string username;
        string password;
        cout<<"\n\n Enter Your Username \n";
        cin>>username;
        cout<<"\nEnter your Password \n";
        cin>>password;
        string sele = "SELECT* FROM members WHERE username = '"+username+"' AND password = '"+password+" '";
        const char* s = sele.c_str();
        qstate = mysql_query(connecting(), s);
        res =  mysql_store_result(conn);
        row = mysql_fetch_row(res);
        string  pass = row[3];
        string user = row[2];
        int i =0;
        do{
        if(password == pass && username ==user){
            dashboard(username);
            cout<<"Login\n";
           // cout<<"\n username = "<<username<<"\n name = "<<row[1]<<"\n password = "<<pw <<"\n Gender = "<<row[4]<<"\n";

        }else{
        cout<<"Incorrect Password \n";
        i++;
        }
        }
        while(i<3);
}
void Members(){
    cout<<"\n\n************************* MEMBERS  ************************\n\n";
    connecting();
        string sele = "SELECT * FROM members"; // WHERE username = '"+username+"' AND password = '"+password+" '";
        const char* s = sele.c_str();
       qstate = mysql_query(connecting(), s);
        res =  mysql_store_result(conn);
        int i =1;
        cout<<"S/N \t Name \t Username \t Gender \t Deposit \t Amount \t Loan \t Date register \n";
        while(row = mysql_fetch_row(res)){
            cout<< i<<"\t"<< row[1]<<"\t"<<row[2]<<"\t"<<row[4]<<"\t"<<row[5]<<"\t"<<row[6]<<"\t"<<row[7]<<"\t"<<row[8]<<"\n";
            i++;
        }
}
int main()
{
    cout<<"\n\n************************* CHOOSE YOUR CHOICE  ************************\n\n";
    int choice;

    do{
       cout<<"\n 1) Login \n\n 2) Register \n\n 3) Chech Register Members\n";
        cout<<"\n Choose Option \n";
        cin >>choice;
        switch(choice){
          case 1 : Login();
          break;
          case 2: Register();
          break;
          case 3: Members();
          break;
          default: cout<<"Invalid choice \n\n";
        }
    }
    while(choice !=0);
//Loan("adesina");
//     Date t1 = (2002,03,03);
//loan_duration("adesina");
//register_duration("adesina");
//pay_loan("adesina");

    return 0;
}

void Logout(){
     Login();
}



