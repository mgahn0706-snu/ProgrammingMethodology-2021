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
    //DB ������ �о�ö����� �ε� ȭ���� ����.
}



class Movie {
private:
    int number;
    string director;
    string title;
    string genre;
    int duration;
    double rating; //Movie�� ���� variable

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
        } /*�Է��� movie�� rating�� �� ū ��� true�� return�Ѵ�. �̴� sort�� �� �� ���� �Լ��� �������� �ʰ�
        operator overloading���� �ϱ� �����̴�.*/
    }
    friend ostream& operator <<(ostream& outputStream, Movie& movie);

    void progress_bar(int number, int duration, int current_time, string title); 
    //�ε�â�� ǥ���ϴ� �Լ�
};

class User {
private:
    string name;
    int user_num;
    Movie myList[NMOVIES];
    int list_size; //���� myList�� ����� ��ȭ�� ����

public:
    User();
    User(string name, int user_num);
    string getName() { return name; }
    int getUsernum() { return user_num; }
    int getListsize() { return list_size; } /*myList�� ȿ�������� Ž���ϱ� ���� list_size���� �����ϴ� �Լ��� �������.*/
    void setName(string s) { name = s; }
    void addMovie(Movie movie);
    void deleteMovie(Movie movie);
    friend ostream& operator <<(ostream& outputStream, User& user); /*Ȥ�� �� cout << x << y��츦 ����Ͽ� void���
                                                                    ostream�� ����Ͽ���. �̹� project������ �Ⱦ��� �� �ϴ�.*/
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
    nf.setCurrentuser();           /*log_off �Լ��� �����ϰ�, ���� �� ó�� �α����� ������ �Է¹ޱ� ���� showUsers�� set
    Currentuser�� �̿��Ͽ� current_user�� initialize ���ش�.*/

    

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
            exit(0); //main�Լ���, �� screen���� cmd ���� �Է¹����� �׿� �´� â�� ����ش�. 
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
/*Movie class�� constructor�̴�. ��� movie���� ������ ������ �������� �ʾұ� ������ initialize�� ���Ƿ� ���־���.
�� ������ �Է¹޾Ƽ� Movie class�� object���� �����Ѵ�.*/

void Movie::progress_bar(int number, int duration, int current_time, string title) {
    cout << "<" << title << "> is playing..." << endl;
    cout << "|";
    for (int i = 0; i < number; i++) {
        cout << "=";
    }
    for (int j = number; j < 10; j++) {
        cout << " ";
    } /*number�� (����ð�)/(�ѽð�)�� ���� 10�ܰ�� ���� ������, play �Լ����� �޴´�.
      �� �Լ��� number ������ŭ =�� print�Ͽ� progress_bar�� ����ϴ� �Լ��̴�.*/
    cout << "|";
    cout << current_time << "mins / " << duration << "mins";

}

ostream& operator<<(ostream& outputStream, Movie& movie) {
    int number = movie.getNumber();
    string director = movie.getDirector();
    string title = movie.getTitle();
    string genre = movie.getGenre();
    int duration = movie.getDuration();
    double rating = movie.getRating(); /*�� private variable�� get�Լ��� �޾ƿ´�.*/
    string num, dur, rat; /*string type�� �ƴ� 3���� ������ stringstream ���̺귯���� �̿��Ͽ� string���� �ٲ��ش�.
                          printString�Լ��� �̿��ϱ� �����̴�.*/
    stringstream convert1, convert2, convert3;
    convert1 << number;
    convert2 << duration;
    convert3 << rating;
    convert1 >> num;
    convert2 >> dur;
    convert3 >> rat; /* convert1~convert3�� �̿��Ͽ� num, dur, rat�̶�� string�� �����Ѵ�.*/
    if (rat.size() == 1) {
        rat += ".0";
    }     /*4.0�� ���� �Ҽ����� ���� ������ ��� ������ double type���� ������ �� �������� ���ԵǾ���. ǥ���� �ϰ����� ����
          ���� �ڿ� .0�� ǥ�����ش�.*/
    printString(num, 11);
    printString(director, 20);
    printString(title, 35);
    printString(genre, 25);
    printString(dur, 15);
    printString(rat, 15); /*printColumn�� ���� �������� ������ ��Ҹ� ����Ѵ�.*/

    return outputStream; /*Movie class�� << operator overloading�� �Ǿ���.*/
}

void Movie::play() {
    int duration = getDuration();
    const int scale = 200; /*scale ���� ���� �ð��� ���� ��� �ð��� ������ �����Ѵ�. */
    clear_screen(10, false);
    bool is_playing = true;
    int started_time = clock();
    int time_watched = 0; 
    while (is_playing) {
        int end_time = clock();
        time_watched = end_time - started_time; /*���� ��� ���۽ð��� clock���� �ҷ�����, ���� �ð����� ���� ��û�ð��� ���Ѵ�.*/
        if (time_watched % scale == 0) {
            clear_screen();
            progress_bar(time_watched / (duration * scale / 10), duration, time_watched / scale, title);
            /*�ð��� ��ü duration�� 1/10�� ���� ������, progress bar�� �����Ѵ�.*/
        }
        if (time_watched / scale >= duration) {
            is_playing = false; /*��û�ð��� ���� ���̿� �������� is_playing�� false�� �ٲ� ������ ������.*/
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
/* User class�� constructor�̴�. list_size�� �⺻������ 0���� �����Ͽ���.(ó������ ���ɸ�Ͽ� ��ȭ�� ���� ����)*/
void User::addMovie(Movie movie) {
    bool is_exist = false;
    for (int i = 0; i < list_size; i++) {
        if (myList[i] == movie) {
            cout << "The movie already exists in your list" << endl; /*myList���� movie���� �� �ȴ´�.
                                                                     ���� ���� movie�� ������ �̹� �ִٴ� ����â�� ����.*/
            is_exist = true;
            Sleep(500); /*������ �޽����� �� �� �ְ� Sleep time�� �����Ͽ���.*/
            cin.ignore();
            break; /*�̹� ������ Add�� �����Ѵ�.*/
        }
    }
    if (!is_exist) {
        myList[list_size] = movie;
        list_size++;
    } /*���� �������� ������, movie�� �Ҵ�� lsit �� �ʿ� movie�� ���� �Ҵ��ϰ�, list_size�� �ø���.
      �̴� vector�� ������ ������ �ϴµ�, �� ��� myList�� ũ�� ������ �Ұ����ϴ�.
      �׷��� �ִ� ��ȭ ������ 100���� �����̱� ������ �ش� database���� list�� �ᵵ ������ ���̴�.*/
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

            /* Add�� ����������, �ݴ�� myList�� �����鼭 ������ movie�� �߰��ϸ� �� index�� �������� ������ movie����
              ������ ��ĭ�� ����� ��, list_size�� �ϳ� ���δ�. �̴� �� movie�� ������ �Ͱ� ���� ������ �Ѵ�.*/
           
        }
    }
    if (not_exist) {
        cout << "The movie does not exist in your list" << endl;
        /*��� list�� �� ������ ���� �޽����� ����Ѵ�.*/
        cin.ignore();
        Sleep(500); /*������ �޽����� ���� ���� �ð� ����*/
    }
}

ostream& operator<<(ostream& outputStream, User& user) {

    for (int i = 0; i < user.getListsize(); i++) {
        cout << user.myList[i] << endl;
    }
    return outputStream;
} /*User�� << operator overloading����, User class�� ����ϸ� myList�� ������ش�.*/


/*
#######################################################################
######    BELOW, WRITE THE CODE RELATED TO THE CLASS 'NETFLIX'   ######
#######################################################################
*/

Netflix::Netflix(fstream& f) { /*netflix�� �� database�� fstream���� �Է¹޾� ��ü�� �����Ѵ�.*/
    int data_loaded = 0;
    string director;
    string title;
    string genre;
    string dur;
    string rat;
    int duration = 0;
    double rating = 0.0;
    Movie initial_movie_list[NMOVIES]; /* Netflix�� ù initialize�� �� �Լ� ��������, fstream�� ���� �� ��Ҹ� ������ ������ �����Ѵ�.*/

    setUser(1, User("user1", 1));
    setUser(2, User("user2", 2));
    setUser(3, User("user3", 3));
    setUser(4, User("user4", 4)); /* setUser�Լ��� ���� user 4���� user1~4�� �ʱ�ȭ�Ѵ�.*/
  
    for (int i = 0; i < NMOVIES; i++) {
        
        initializing_screen(data_loaded);
        getline(f, director, '|');
        getline(f, title, '|');
        getline(f, genre, '|');
        getline(f, dur, '|');
        getline(f, rat); /*�� line�� "|"�� �������� getline�� �̿��� ���� ��, �� ������ �����Ѵ�.*/
        

        stringstream convert(dur);
        convert >> duration;
        rating = std::stod(rat); /*int�� double type���� ��ȯ�ؾ��ϴ� duration�� rating�� stringstream�� �̿��� ��ȯ�����־���.
                                 getline();�� �⺻������ string���� �ڷḦ �о���� �����̴�.*/

        Movie movie = Movie(i + 1, director, title, genre, duration, rating); /*�� �ٸ��� Movie object�� �����Ѵ�.*/
        movieList[i] = movie; /*movieList�� �ϳ��� Movie object�� �����Ѵ�.*/
        data_loaded++; /*data_loaded�� �߰��� ������ ȭ������ �󸶳� loading�ƴ��� Ȯ���� �� �ְ� �Ѵ�.*/
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
    cout << endl; /*movieList�� ù 7���� get�Լ��� �̿��� �ҷ����� ����Ѵ�.*/
    

    while (command < 1 || command>8) {
        cout << "Command: "; /*1~8�� ������ �ƴϸ� ��� command�Է��� �޴´�.*/
        cin >> command;
    }
    if (command == 1 || command==2) {
        scrollPage(command); /*1�Ǵ� 2�̸� scrollPage �Լ��� �� Ŀ�ǵ忡 �°� �����Ų��.*/
    }

    else {
        return command; /*3~8 �����̸� main�Լ��� �ش� command�� return���� �� �ӹ��� �����ϵ��� �Ѵ�.*/
    }
}

void Netflix::setCurrentuser() {
    int user_num=0;
    while (user_num > 4 || user_num < 1) {
        cout << "=>";
        cin >> user_num;
    }
    current_user = getUser(user_num);

}/*���� ����ϰ� �ִ� user�� ������ �����Ѵ�. 1~4�� �Է¹��� ������ �Է��� ������, �ش��ϴ� number�� user�� current_user�� �Ҵ��Ѵ�.*/

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
        cin >> command; /*1~4 �Է��� �ƴϸ� ��� �Է��� �޴´�.*/
    }
    if (command == 4) {
        cin.ignore(); /*Ȥ�� �Էµ� ���� ������� ignore�� ���־���.*/
        return command; /* command�� 4, �� end�̸� �ٽ� mainȭ������ ���ư����ϹǷ� main�Լ��� command���� �ٷ� return�Ѵ�.*/
        
    }


    cin.ignore();
    cout << "Keyboard: ";
    getline(cin, keyword); /*getline�� �̿��� keyword�� �ҷ��´�. getline�� �̿��� ���� ���⵵ �����ؾ��ϱ� �����̴�.*/

    return screenResults(keyword, command); /*result screen�� ������ ������.*/


}


void Netflix::searchMovie(string keyword, int cmd) {
    vector<Movie> search_list;
   /*search�� movie���� ������ vector�� �����Ѵ�. vector�� �� ������ ��� ��ȭ�� ����� ������ ����
   �������� ũ�⸦ �����ϱ� �����̴�. myListó�� ũ�� 100�� array�� �ص� ������ ���̴�.*/
    if (cmd == 1) {
        for (int i = 0; i < NMOVIES; i++) {
            if (movieList[i].getDirector().find(keyword) != string::npos) {
                search_list.push_back(movieList[i]);
                
            } /*string�� find�� �̿��Ͽ� ���� npos�� ������ �ʴ´ٸ�, �� �ش� keyword�� ���ԵǾ��ִٸ�
              �� movie�� search_list�� push_back�Ѵ�. find�� npos�� string�� ���ڿ��� ���ԵǴ����� ���θ�
              �Ǵ��ϱ� �����ϱ� ������ ����Ͽ���.*/
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
     /* �Էµ� cmd�� ���� getter�� ������� ���������� �ٸ��� �Ͽ� ���Ϳ� push_back�Ѵ�.*/

    sort(search_list.begin(), search_list.end()); /*algorithm�� sort�� search_list�� �����Ѵ�.
                                                  �̶� ���� compare �Լ��� �������� �ʾҴµ�, �̴� �ε�ȣ �����ڸ� overloading�Ͽ��� ������
                                                  rating�� ���� �ڵ����� sorting�ȴ�.*/
    for (int j = 0; j < search_list.size(); j++) {
        cout << search_list[j];
    }                                               /*sorting�� ����� �� ���� ����Ѵ�.*/

    cout << "You have " << search_list.size() << " matched results!!!" << endl; /*size�� �˻� ����� �� ������ �ǹ��Ѵ�.*/
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
        
        Movie temp=movieList[NMOVIES-1]; /* �� ���� ������ ������ �����ϱ� ���� temp�� �����Ѵ�.*/
        if (temp.getNumber() != 100) { /*���� temp�� 100�̸�, �� �� �̻� ���� scroll�� �ȵɶ��� �����Ѵ�. */
            for (int j = NMOVIES - 2; j >= 0; j--) {
                movieList[j + 1] = movieList[j]; /*movieList��ü�� ��ĭ�� �ڷ� �о��ش�. mainscreen������ movieList�� ��
                                                 �� 7���� ����ϱ� ������ scroll�� �� ĭ �ö� �Ͱ� ���� ȿ���� �ִ�.*/
            }
            movieList[0] = temp; /*temp�� ������ movie�� �� �տ� ���´�.*/
        }
   }
    else if (i == 2) { 
        Movie temp = movieList[0];
        if (temp.getNumber() != 94) {
            for (int j = 0; j < NMOVIES - 1; j++) {
                movieList[j] = movieList[j + 1];
            }
            movieList[NMOVIES - 1] = temp; /*scroll UP���� ������ �ݴ�� �ٲپ� scroll down�� �����ϵ��� �Ѵ�.*/
        }
    }
}

void Netflix::showUsers() {
    for (int i = 1; i < NUSERS + 1; i++) {
        cout << i << ": " << getUser(i).getName() << endl;
    } /*netflix class�� user list�� ���ٿ� �ϳ��� ����Ѵ�. User ��ü�� print�� myList���� ��⶧���� getUser�� �̿��Ͽ�
      1~4 ������ ������ �ҷ����� ���� getName�Լ��� �̿��Ͽ� �̸��� �ҷ��Դ�.*/
}

void Netflix::log_off() {
    clear_screen(10, false);
    setUser(current_user.getUsernum(), current_user); /*log out�ϱ� ��, user�� my_list���� Ȯ���� �����Ͽ�
    ������ �α������� �� ���¸� �����ϱ� ���Ͽ� setUser�� ����Ѵ�.*/

    cout << "Which user? (1 ~ 4): " << endl;
    showUsers();
    setCurrentuser();
    
    /*log out�� �ϸ� ȭ���� clear�ϰ� �ٽ� user_num�� �Է¹��� �غ� �Ѵ�.
    �� ó�� main�Լ��� ���۹���� ����. */
}

void Netflix::playMovie() {
    cout << "Enter movie number you want to watch: ";
    int movie_num;
    cin >> movie_num;
    for (int i = 0; i < NMOVIES; i++) {
        if (movieList[i].getNumber() == movie_num) {
            movieList[i].play();
        } /*movie number�� �Է¹����� movie list�� �����鼭 �ش��ϴ� number�� movie�� �����Ѵ�.
          �̶� 1~100�� ������ ���� input�� ������� �ʰ� �׳� ����ȴ�.*/
    }
}

void Netflix::addMovie_to_user() {
    
    
    int movie_num=0;
    while (movie_num<1 || movie_num>NMOVIES) { /*movie number�� 1~100������ ������ ��� �Է¹޴´�.*/
        cout << "Enter movie number you want to add: ";
        cin >> movie_num;
        
    }
    for (int i = 0; i < NMOVIES; i++) {
        if (movie_num == movieList[i].getNumber()) {
            current_user.addMovie(movieList[i]); /*movieList movie�� ��ȣ�� get�Լ��� �����鼭 �ش��ϴ� movie�� current_user��
                                                 list�� �����ϴ� �Լ��� �����Ѵ�. �̶� �׳� movieList[i]�� ���� ���� ������ scroll
                                                 �Ͽ� movie List ������ �ٲ��ֵ��� ������ �ڵ��߱� �����̴�.*/
        }
    }
   
}

void Netflix::deleteMovie_of_user() {
    int movie_num = 0;
    while (movie_num<1 || movie_num>NMOVIES) {
        cout << "Enter movie number you want to delete: ";/*movie number�� 1~100������ ������ ��� �Է¹޴´�.*/
        cin >> movie_num;
        
    }
    for (int i = 0; i < NMOVIES; i++) {
        if (movie_num == movieList[i].getNumber()) {
            current_user.deleteMovie(movieList[i]); /*movieList�� add�� ���������� �����鼭 �ش� ��ȣ�� movie�� delete�ϴ�
                                                    �Լ��� �����Ѵ�.*/
        }
    }
}

void Netflix::change_user_name() {
    cout << "New name: ";
    string new_name;
    cin.ignore(); /*Ȥ�� �� ���� ������ ���� buffer ����*/
    getline(cin, new_name); /*���⵵ �ޱ� ���Ͽ� getline�� �̿��Ͽ���.*/
    current_user.setName(new_name); /*new_name�� �Է¹޾� setter�� �̿��� �̸��� �ٲ۴�. */
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