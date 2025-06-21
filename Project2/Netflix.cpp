#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>
#include <sstream>
#include <vector>

#define NMOVIES 100
#define NUSERS 4

using namespace std;

void printString(string s, unsigned int length) {
    if (s.length() < length) {
        cout << s;
        for (unsigned int i = 0; i < length - s.length(); i++) {
            cout << ' ';
        }
    }
    else {
        cout << s.substr(0, length - 5);
        cout << "...  ";
    }
}
void printColumns() {
    printString("NUMBER", 11);
    printString("DIRECTOR", 20);
    printString("TITLE", 35);
    printString("GENRE", 25);
    printString("DURATION", 15);
    printString("RATING", 15);
    cout << endl;
}
void clear_screen(int sleep = 10, bool overwritten = true) {
    if (overwritten == true) {
        COORD pos = { 0, 0 };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        Sleep(sleep);
    }
    else {
        system("cls");
    }
}

void initializing_screen(int loading) {
    clear_screen(10, false);
    cout << "===================================" << endl;
    cout << "||                               ||" << endl;
    cout << "||                               ||" << endl;
    cout << "||       - N E T F L I X -       ||" << endl;
    cout << "||                               ||" << endl;
    cout << "||                               ||" << endl;
    cout << "===================================" << endl;
    cout << "Loading DB... (" << loading << "%)"; 
    //DB 한줄을 읽어올때마다 로딩 화면을 띄운다.
}



class Movie {
private:
    int number;
    string director;
    string title;
    string genre;
    int duration;
    double rating; //Movie가 갖는 variable

public:
    Movie();
    Movie(int c, string d, string t, string g, int dur, double rat);
    int getNumber() { return number; }
    string getDirector() { return director; }
    string getTitle() { return title; }
    string getGenre() { return genre; }
    int getDuration() { return duration; }
    double getRating() { return rating; }
    void play();
    bool operator == (Movie& movie) { return (number == movie.number); }
    bool operator <(Movie& movie) {
        if (movie.rating > rating) {
            return true;
        }
        else {
            return false;
        } /*입력한 movie의 rating이 더 큰 경우 true를 return한다. 이는 sort를 할 때 따로 함수를 정의하지 않고
        operator overloading으로 하기 위함이다.*/
    }
    friend ostream& operator <<(ostream& outputStream, Movie& movie);

    void progress_bar(int number, int duration, int current_time, string title); 
    //로딩창을 표시하는 함수
};

class User {
private:
    string name;
    int user_num;
    Movie myList[NMOVIES];
    int list_size; //현재 myList에 저장된 영화의 개수

public:
    User();
    User(string name, int user_num);
    string getName() { return name; }
    int getUsernum() { return user_num; }
    int getListsize() { return list_size; } /*myList를 효율적으로 탐색하기 위해 list_size에도 접근하는 함수를 만들었다.*/
    void setName(string s) { name = s; }
    void addMovie(Movie movie);
    void deleteMovie(Movie movie);
    friend ostream& operator <<(ostream& outputStream, User& user); /*혹시 모를 cout << x << y경우를 대비하여 void대신
                                                                    ostream을 사용하였다. 이번 project에서는 안쓰인 듯 하다.*/
};

class Netflix {
private:
    User userList[NUSERS];
    User current_user;
    Movie movieList[NMOVIES];

public:
    Netflix(fstream& f);
    User& getUser(int i) { return userList[i - 1]; }
    User& getCurrentuser() { return current_user; }
    Movie& getMovie(int i) { return movieList[i]; }
    void setUser(int i, User user) { userList[i - 1] = user; }
    void setCurrentuser();
    void setMovielist(int i, Movie movie) { movieList[i] = movie; }
    int screenMain();
    int screenSearch();
    int screenResults(string keyword, int cmd);
    int screenProfile();
    void scrollPage(int i = 0);
    void showUsers();
    void searchMovie(string keyword, int cmd);
    void playMovie();
    void addMovie_to_user();
    void deleteMovie_of_user();
    void change_user_name();
    void log_off();
};

/*
#######################################################################
######################            MAIN             ####################
#######################################################################
*/

int main(void) {
    
    fstream f("database.txt");

    

    Netflix nf = Netflix(f);

    clear_screen(0, false);
    
    cout << "Which user? (1 ~ 4): " << endl;
    nf.showUsers();
    nf.setCurrentuser();           /*log_off 함수와 유사하게, 먼저 맨 처음 로그인할 유저를 입력받기 위해 showUsers와 set
    Currentuser를 이용하여 current_user를 initialize 해준다.*/

    

    while (true) {
        int cmd;
        string keyword;
        cmd = nf.screenMain();
        switch (cmd) {
        case 3:
            nf.playMovie();
            break;
        case 4:
            nf.addMovie_to_user();
            break;
        case 5:
            cmd = nf.screenSearch();
            if (cmd == 1)
                nf.playMovie();
            else if (cmd == 2)
                nf.addMovie_to_user();
            break;
        case 6:
            cmd = nf.screenProfile();
            if (cmd == 1)
                nf.change_user_name();
            else if (cmd == 2)
                nf.deleteMovie_of_user();
            break;
        case 7:
            nf.log_off();
            break;
        case 8:
            cout << "SEE YOU LATER~!" << endl;
            Sleep(1000);
            exit(0); //main함수로, 각 screen에서 cmd 값을 입력받으면 그에 맞는 창을 띄워준다. 
        }
    }

    return 0;
}
/*
#######################################################################
#######    BELOW, WRITE THE CODE RELATED TO THE CLASS 'MOVIE'   #######
#######################################################################
*/

Movie::Movie() : number(0), director("a"), title("a"), genre("a"), duration(0), rating(0.0) {};
Movie::Movie(int c, string d, string t, string g, int dur, double rat) : number(c), director(d), title(t), genre(g), duration(dur), rating(rat) {};
/*Movie class의 constructor이다. 모든 movie에는 각각의 정보가 누락되지 않았기 때문에 initialize는 임의로 해주었다.
각 변수를 입력받아서 Movie class의 object들을 생성한다.*/

void Movie::progress_bar(int number, int duration, int current_time, string title) {
    cout << "<" << title << "> is playing..." << endl;
    cout << "|";
    for (int i = 0; i < number; i++) {
        cout << "=";
    }
    for (int j = number; j < 10; j++) {
        cout << " ";
    } /*number는 (현재시간)/(총시간)의 값을 10단계로 나눈 것으로, play 함수에서 받는다.
      이 함수는 number 개수만큼 =를 print하여 progress_bar를 출력하는 함수이다.*/
    cout << "|";
    cout << current_time << "mins / " << duration << "mins";

}

ostream& operator<<(ostream& outputStream, Movie& movie) {
    int number = movie.getNumber();
    string director = movie.getDirector();
    string title = movie.getTitle();
    string genre = movie.getGenre();
    int duration = movie.getDuration();
    double rating = movie.getRating(); /*각 private variable를 get함수로 받아온다.*/
    string num, dur, rat; /*string type이 아닌 3개의 변수는 stringstream 라이브러리를 이용하여 string으로 바꿔준다.
                          printString함수를 이용하기 위함이다.*/
    stringstream convert1, convert2, convert3;
    convert1 << number;
    convert2 << duration;
    convert3 << rating;
    convert1 >> num;
    convert2 >> dur;
    convert3 >> rat; /* convert1~convert3을 이용하여 num, dur, rat이라는 string을 생성한다.*/
    if (rat.size() == 1) {
        rat += ".0";
    }     /*4.0과 같이 소수점이 없는 숫자의 경우 이전에 double type으로 저장할 때 정수값만 남게되었다. 표기의 일관성을 위해
          숫자 뒤에 .0을 표시해준다.*/
    printString(num, 11);
    printString(director, 20);
    printString(title, 35);
    printString(genre, 25);
    printString(dur, 15);
    printString(rat, 15); /*printColumn과 같은 간격으로 각각의 요소를 출력한다.*/

    return outputStream; /*Movie class의 << operator overloading이 되었다.*/
}

void Movie::play() {
    int duration = getDuration();
    const int scale = 200; /*scale 값은 실제 시간과 영상 재생 시간의 비율을 결정한다. */
    clear_screen(10, false);
    bool is_playing = true;
    int started_time = clock();
    int time_watched = 0; 
    while (is_playing) {
        int end_time = clock();
        time_watched = end_time - started_time; /*영상 재생 시작시간을 clock으로 불러오고, 현재 시간에서 빼서 시청시간을 구한다.*/
        if (time_watched % scale == 0) {
            clear_screen();
            progress_bar(time_watched / (duration * scale / 10), duration, time_watched / scale, title);
            /*시간이 전체 duration의 1/10을 넘을 때마다, progress bar를 갱신한다.*/
        }
        if (time_watched / scale >= duration) {
            is_playing = false; /*시청시간이 영상 길이와 같아지면 is_playing을 false로 바꿔 영상을 끝낸다.*/
        }

    }
}



/*
#######################################################################
#######    BELOW, WRITE THE CODE RELATED TO THE CLASS 'USER'    #######
#######################################################################
*/

User::User() : name(""), user_num(0), list_size(0) { }
User::User(string name, int user_num) : name(name), user_num(user_num),list_size(0) {  }
/* User class의 constructor이다. list_size는 기본적으로 0으로 설정하였다.(처음에는 관심목록에 영화가 없기 때문)*/
void User::addMovie(Movie movie) {
    bool is_exist = false;
    for (int i = 0; i < list_size; i++) {
        if (myList[i] == movie) {
            cout << "The movie already exists in your list" << endl; /*myList에서 movie들을 쭉 훑는다.
                                                                     만약 같은 movie가 있으면 이미 있다는 오류창을 띄운다.*/
            is_exist = true;
            Sleep(500); /*유저가 메시지를 볼 수 있게 Sleep time을 설정하였다.*/
            cin.ignore();
            break; /*이미 있으면 Add를 종료한다.*/
        }
    }
    if (!is_exist) {
        myList[list_size] = movie;
        list_size++;
    } /*만약 존재하지 않으면, movie가 할당된 lsit 끝 쪽에 movie를 새로 할당하고, list_size를 늘린다.
      이는 vector와 유사한 역할을 하는데, 이 경우 myList의 크기 수정이 불가능하다.
      그러나 최대 영화 개수가 100개로 고정이기 때문에 해당 database에는 list를 써도 무방할 것이다.*/
}

void User::deleteMovie(Movie movie) {
    bool not_exist = true;
    for (int i = 0; i < list_size; i++) {
        if (myList[i] == movie) {
            for (int j = i; j < list_size - 1; j++) {
                myList[j] = myList[j + 1];
            } 
       
            list_size--;
            not_exist = false;

            /* Add와 유사하지만, 반대로 myList를 훑으면서 동일한 movie를 발견하면 그 index를 기준으로 뒤쪽의 movie들을
              앞으로 한칸씩 당겨준 뒤, list_size를 하나 줄인다. 이는 그 movie가 삭제된 것과 같은 역할을 한다.*/
           
        }
    }
    if (not_exist) {
        cout << "The movie does not exist in your list" << endl;
        /*모든 list에 다 없으면 오류 메시지를 출력한다.*/
        cin.ignore();
        Sleep(500); /*유저가 메시지를 보기 위한 시간 설정*/
    }
}

ostream& operator<<(ostream& outputStream, User& user) {

    for (int i = 0; i < user.getListsize(); i++) {
        cout << user.myList[i] << endl;
    }
    return outputStream;
} /*User의 << operator overloading으로, User class를 출력하면 myList를 출력해준다.*/


/*
#######################################################################
######    BELOW, WRITE THE CODE RELATED TO THE CLASS 'NETFLIX'   ######
#######################################################################
*/

Netflix::Netflix(fstream& f) { /*netflix는 각 database를 fstream으로 입력받아 객체를 생성한다.*/
    int data_loaded = 0;
    string director;
    string title;
    string genre;
    string dur;
    string rat;
    int duration = 0;
    double rating = 0.0;
    Movie initial_movie_list[NMOVIES]; /* Netflix의 첫 initialize할 때 함수 실행으로, fstream을 통해 각 요소를 저장할 변수를 설정한다.*/

    setUser(1, User("user1", 1));
    setUser(2, User("user2", 2));
    setUser(3, User("user3", 3));
    setUser(4, User("user4", 4)); /* setUser함수를 통해 user 4명을 user1~4로 초기화한다.*/
  
    for (int i = 0; i < NMOVIES; i++) {
        
        initializing_screen(data_loaded);
        getline(f, director, '|');
        getline(f, title, '|');
        getline(f, genre, '|');
        getline(f, dur, '|');
        getline(f, rat); /*각 line을 "|"를 기준으로 getline을 이용해 읽은 뒤, 각 변수에 저장한다.*/
        

        stringstream convert(dur);
        convert >> duration;
        rating = std::stod(rat); /*int와 double type으로 변환해야하는 duration과 rating은 stringstream을 이용해 변환시켜주었다.
                                 getline();은 기본적으로 string으로 자료를 읽어오기 때문이다.*/

        Movie movie = Movie(i + 1, director, title, genre, duration, rating); /*각 줄마다 Movie object를 생성한다.*/
        movieList[i] = movie; /*movieList에 하나씩 Movie object를 저장한다.*/
        data_loaded++; /*data_loaded를 추가해 유저가 화면으로 얼마나 loading됐는지 확인할 수 있게 한다.*/
    }
}




int Netflix::screenMain() {
    clear_screen(10, false);
    
    unsigned long long int command=0;
    cout << "==================================================================================" << endl;
    cout << "Main                                                                  (" << current_user.getName() << ')' << endl;
    cout << "----------------------------------------------------------------------------------" << endl;
    cout << "1.Up    2.Down    3.Play    4.Add    5.Search    6.Profile    7.Logout    8.Exit  " << endl;
    cout << "==================================================================================" << endl << endl;
    printColumns();
    cout << endl;
    
    for (int i = 0; i < 7; i++) {
        cout << getMovie(i);
    }
    cout << endl; /*movieList의 첫 7개를 get함수를 이용해 불러오고 출력한다.*/
    

    while (command < 1 || command>8) {
        cout << "Command: "; /*1~8의 정수가 아니면 계속 command입력을 받는다.*/
        cin >> command;
    }
    if (command == 1 || command==2) {
        scrollPage(command); /*1또는 2이면 scrollPage 함수를 각 커맨드에 맞게 실행시킨다.*/
    }

    else {
        return command; /*3~8 정수이면 main함수로 해당 command를 return시켜 각 임무를 수행하도록 한다.*/
    }
}

void Netflix::setCurrentuser() {
    int user_num=0;
    while (user_num > 4 || user_num < 1) {
        cout << "=>";
        cin >> user_num;
    }
    current_user = getUser(user_num);

}/*현재 사용하고 있는 user가 누군지 설정한다. 1~4를 입력받을 때까지 입력을 받으며, 해당하는 number의 user를 current_user에 할당한다.*/

int Netflix::screenSearch() {
    clear_screen(10, false);
    cout << "==================================================================================" << endl;
    cout << "Search                                                                (" << current_user.getName() << ')' << endl;
    cout << "----------------------------------------------------------------------------------" << endl;
    cout << "1.Director    2.Title    3.Genre    4.Exit" << endl;
    cout << "==================================================================================" << endl << endl;
    printColumns();
    int command=0;
    string keyword;
    while (command != 1 && command != 2 && command != 3 && command != 4) {
        cout << endl << "Command: ";      
        cin >> command; /*1~4 입력이 아니면 계속 입력을 받는다.*/
    }
    if (command == 4) {
        cin.ignore(); /*혹시 입력된 값이 있을까봐 ignore을 해주었다.*/
        return command; /* command가 4, 즉 end이면 다시 main화면으로 돌아가야하므로 main함수로 command값을 바로 return한다.*/
        
    }


    cin.ignore();
    cout << "Keyboard: ";
    getline(cin, keyword); /*getline을 이용해 keyword를 불러온다. getline을 이용한 것은 띄어쓰기도 포함해야하기 때문이다.*/

    return screenResults(keyword, command); /*result screen에 변수를 보낸다.*/


}


void Netflix::searchMovie(string keyword, int cmd) {
    vector<Movie> search_list;
   /*search한 movie들을 저장할 vector를 생성한다. vector를 쓴 이유는 몇개의 영화가 결과로 나올지 몰라서
   동적으로 크기를 조절하기 위함이다. myList처럼 크기 100인 array로 해도 무방할 것이다.*/
    if (cmd == 1) {
        for (int i = 0; i < NMOVIES; i++) {
            if (movieList[i].getDirector().find(keyword) != string::npos) {
                search_list.push_back(movieList[i]);
                
            } /*string의 find를 이용하여 만약 npos가 나오지 않는다면, 즉 해당 keyword가 포함되어있다면
              그 movie를 search_list에 push_back한다. find와 npos는 string에 문자열이 포함되는지의 여부를
              판단하기 용이하기 때문에 사용하였다.*/
        }
    }
    if (cmd == 2) {
        for (int i = 0; i < NMOVIES; i++) {
            if (movieList[i].getTitle().find(keyword) != string::npos) {
                search_list.push_back(movieList[i]);
            }
        }
    }
    if (cmd == 3) {
        for (int i = 0; i < NMOVIES; i++) {
            if (movieList[i].getGenre().find(keyword) != string::npos) {
                search_list.push_back(movieList[i]);
            }
        }
    }
     /* 입력된 cmd에 따라 getter로 얻어지는 변수종류를 다르게 하여 벡터에 push_back한다.*/

    sort(search_list.begin(), search_list.end()); /*algorithm의 sort로 search_list를 정렬한다.
                                                  이때 따로 compare 함수는 지정하지 않았는데, 이는 부등호 연산자를 overloading하였기 때문에
                                                  rating에 따라 자동으로 sorting된다.*/
    for (int j = 0; j < search_list.size(); j++) {
        cout << search_list[j];
    }                                               /*sorting된 결과를 줄 별로 출력한다.*/

    cout << "You have " << search_list.size() << " matched results!!!" << endl; /*size는 검색 결과의 총 개수를 의미한다.*/
    cout << endl;
}

int Netflix::screenResults(string keyword, int cmd) {
    int command=0;
    clear_screen(10, false);
    cout << "==================================================================================" << endl;
    cout << "Results                                                               (" << current_user.getName() << ')' << endl;
    cout << "----------------------------------------------------------------------------------" << endl;
    cout << "1.Play a movie    2.Add   3.Exit       " << endl;
    cout << "==================================================================================" << endl << endl;
    printColumns();
    searchMovie(keyword,cmd);
    while (command != 1 && command != 2 && command != 3) {
        cout << "Command: ";
        cin >> command;
    }
    return command;

}

int Netflix::screenProfile() {
    clear_screen(10,false);
    cout << "==================================================================================" << endl;
    cout << "Profile                                                               (" << current_user.getName() << ')' << endl;
    cout << "----------------------------------------------------------------------------------" << endl;
    cout << "1.Change name    2.Delete    3.Exit" << endl;
    cout << "==================================================================================" << endl << endl;
    printColumns();
    cout << current_user;
    
    int command=0;
    while (command != 1 && command != 2 && command != 3) {
        cout << "Command: ";
        cin >> command;
    }

    return command;

}

void Netflix::scrollPage(int i) {
    if (i == 1) {
        
        Movie temp=movieList[NMOVIES-1]; /* 맨 뒤의 데이터 삭제를 방지하기 위해 temp에 저장한다.*/
        if (temp.getNumber() != 100) { /*만약 temp가 100이면, 즉 더 이상 위로 scroll이 안될때는 제외한다. */
            for (int j = NMOVIES - 2; j >= 0; j--) {
                movieList[j + 1] = movieList[j]; /*movieList자체를 한칸씩 뒤로 밀어준다. mainscreen에서는 movieList의 맨
                                                 앞 7개만 출력하기 때문에 scroll이 한 칸 올라간 것과 같은 효과가 있다.*/
            }
            movieList[0] = temp; /*temp에 저장한 movie를 맨 앞에 놓는다.*/
        }
   }
    else if (i == 2) { 
        Movie temp = movieList[0];
        if (temp.getNumber() != 94) {
            for (int j = 0; j < NMOVIES - 1; j++) {
                movieList[j] = movieList[j + 1];
            }
            movieList[NMOVIES - 1] = temp; /*scroll UP에서 방향을 반대로 바꾸어 scroll down을 실행하도록 한다.*/
        }
    }
}

void Netflix::showUsers() {
    for (int i = 1; i < NUSERS + 1; i++) {
        cout << i << ": " << getUser(i).getName() << endl;
    } /*netflix class의 user list를 한줄에 하나씩 출력한다. User 자체의 print는 myList에서 썼기때문에 getUser을 이용하여
      1~4 까지의 유저를 불러오고 각각 getName함수를 이용하여 이름을 불러왔다.*/
}

void Netflix::log_off() {
    clear_screen(10, false);
    setUser(current_user.getUsernum(), current_user); /*log out하기 전, user의 my_list들을 확실히 저장하여
    다음에 로그인했을 때 상태를 유지하기 위하여 setUser를 사용한다.*/

    cout << "Which user? (1 ~ 4): " << endl;
    showUsers();
    setCurrentuser();
    
    /*log out을 하면 화면을 clear하고 다시 user_num을 입력받을 준비를 한다.
    맨 처음 main함수와 동작방식이 같다. */
}

void Netflix::playMovie() {
    cout << "Enter movie number you want to watch: ";
    int movie_num;
    cin >> movie_num;
    for (int i = 0; i < NMOVIES; i++) {
        if (movieList[i].getNumber() == movie_num) {
            movieList[i].play();
        } /*movie number를 입력받으면 movie list를 훑으면서 해당하는 number의 movie를 실행한다.
          이때 1~100을 제외한 정수 input은 실행되지 않고 그냥 종료된다.*/
    }
}

void Netflix::addMovie_to_user() {
    
    
    int movie_num=0;
    while (movie_num<1 || movie_num>NMOVIES) { /*movie number가 1~100사이일 때까지 계속 입력받는다.*/
        cout << "Enter movie number you want to add: ";
        cin >> movie_num;
        
    }
    for (int i = 0; i < NMOVIES; i++) {
        if (movie_num == movieList[i].getNumber()) {
            current_user.addMovie(movieList[i]); /*movieList movie의 번호를 get함수로 훑으면서 해당하는 movie를 current_user의
                                                 list에 저장하는 함수를 실행한다. 이때 그냥 movieList[i]를 쓰지 않은 이유는 scroll
                                                 하여 movie List 순서를 바꿔주도록 이전에 코딩했기 때문이다.*/
        }
    }
   
}

void Netflix::deleteMovie_of_user() {
    int movie_num = 0;
    while (movie_num<1 || movie_num>NMOVIES) {
        cout << "Enter movie number you want to delete: ";/*movie number가 1~100사이일 때까지 계속 입력받는다.*/
        cin >> movie_num;
        
    }
    for (int i = 0; i < NMOVIES; i++) {
        if (movie_num == movieList[i].getNumber()) {
            current_user.deleteMovie(movieList[i]); /*movieList를 add와 마찬가지로 훑으면서 해당 번호의 movie를 delete하는
                                                    함수를 실행한다.*/
        }
    }
}

void Netflix::change_user_name() {
    cout << "New name: ";
    string new_name;
    cin.ignore(); /*혹시 모를 오류 방지를 위한 buffer 삭제*/
    getline(cin, new_name); /*띄어쓰기도 받기 위하여 getline을 이용하였다.*/
    current_user.setName(new_name); /*new_name을 입력받아 setter를 이용해 이름을 바꾼다. */
}

/*
########################################################################
######   YOU CAN FREELY USE THE SCREEN HEADERS PROVIDED BELOW   ########
########################################################################


cout << "===================================" << endl;
cout << "||                               ||" << endl;
cout << "||                               ||" << endl;
cout << "||       - N E T F L I X -       ||" << endl;
cout << "||                               ||" << endl;
cout << "||                               ||" << endl;
cout << "===================================" << endl;


cout << "==================================================================================" << endl;
cout << "Main                                                                  (" << current_user.getName() << ')' << endl;
cout << "----------------------------------------------------------------------------------" << endl;
cout << "1.Up    2.Down    3.Play    4.Add    5.Search    6.Profile    7.Logout    8.Exit  " << endl;
cout << "==================================================================================" << endl << endl;


cout << "==================================================================================" << endl;
cout << "Search                                                                (" << current_user.getName() << ')' << endl;
cout << "----------------------------------------------------------------------------------" << endl;
cout << "1.Director    2.Title    3.Genre    4.Exit" << endl;
cout << "==================================================================================" << endl << endl;


cout << "==================================================================================" << endl;
cout << "Results                                                               (" << current_user.getName() << ')' << endl;
cout << "----------------------------------------------------------------------------------" << endl;
cout << "1.Play a movie    2.Add   3.Exit       " << endl;
cout << "==================================================================================" << endl << endl;


cout << "==================================================================================" << endl;
cout << "Profile                                                               (" << current_user.getName() << ')' << endl;
cout << "----------------------------------------------------------------------------------" << endl;
cout << "1.Change name    2.Delete    3.Exit" << endl;
cout << "==================================================================================" << endl << endl;


*/