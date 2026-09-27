#include<iostream>
#include<conio.h>
#include<windows.h>
#include<fstream>
#include<ctime>
#include<cstdlib>

using namespace std;

string chart[8][8];
string white = "\u25CF";
string black = "\u25CB";
string blank = "\u25A1";
string turn; //current player's disc
int n; //size of board
bool unfinnished; //true if loading an unfinished game
bool advanced;
int gameCount=0;
int x=0;
int y=0;

struct gameInfo {
    string player1;
    string player2;
    string winner;
    int countBlack;
    int countWhite;
    string date="";
    time_t endTime;
};
gameInfo* game = new gameInfo [20];
int capacity = 20;

void enterData(int player_number);
void newGame();
void menu();
void help();
void loadGame();
void gameHistory();
void board();
void print();
void choose();
void possible();
void twoPlayer();
void humanMove();
bool isPossible();
void winner();
void addGameHistory();
void sortAndSave();
void singlePlayer();
void robotMove();
void saveGame();
void setColor(int color);
void placeAndFlip();
void resize();


int main() {

    srand(static_cast<unsigned int>(time(nullptr)));
    SetConsoleOutputCP(CP_UTF8);
    addGameHistory();
    menu();
}

//displays a menu and handles user's selection
void menu() {

    system("cls");
    cout<<"~~~~~~~~~~~~~~ MENU ~~~~~~~~~~~~~~"<<endl;
    cout<<"1) New Game\n2) Load Game\n3) Help\n4) Game History\n5) Exit\n";

    while(true) {

        //using retuen instead of break so that we don't stuck in the loop
        char choice=getch();
        switch(choice) {
            case '1' : newGame(); return;
            case '2' : loadGame(); return;
            case '3' : help(); return;
            case '4' : gameHistory(); return;
            case '5' : {
                system("cls");
                delete [] game;
                exit(0);
            }
        }
    }
}

// starts a new game by selecting board size and player count
void newGame() {

    unfinnished=false;
    x=0;
    y=0;
    system("cls");
    cout<<"\nEnter your desired size : \n\n";
    cout<<"1) 4×4\n2) 6×6\n3) 8×8\n";
    
    while(true) {
        char choice = getch();
        if(choice=='1') {
            n=4;
            break;
        }
        else if(choice=='2') {
            n=6;
            break;
        }
        else if(choice=='3') {
            n=8;
            break;
        }
    }
    
    system("cls");
    cout<<"\nChoose game mode : \n\n";
    cout<<"1) Single Player\n2) Two Player\n";
    
    while(true) {
        char choice = getch();
        if (choice=='1') {
            system("cls");
            cout<<"\nChoose level... \n\n1) simple\n2) advanced\n";

            while(true) {
                char choice = getch();
                if(choice=='1') {
                    advanced=false;
                    break;
                }
                else if(choice=='2') {
                    advanced=true;
                    break;
                }
            }
            enterData(1);
            singlePlayer();
            return;
        } 
        
        else if (choice=='2') {
            enterData(2);
            twoPlayer();
            return;
        }
    }  
}

// Reads player names based on selected mode
void enterData(int player_number) {

    if(gameCount==capacity) resize();
    system("cls");
    if(player_number==1) {
        cout<<"\nEnter your name : \n";
        cin>>game[gameCount].player1;
        game[gameCount].player2="robot";
    }

    else if(player_number==2) {
        cout<<"\nEnter the name of first player : \n";
        cin>>game[gameCount].player1;
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout<<"Enter the name of second player : \n";
        cin>>game[gameCount].player2;
    }
}

// creats the board with blank cells and places the 4 starting discs in the center
void board() {

    for (int i=0 ; i<n ; i++) {
        for (int j=0 ; j<n ; j++) {
            
            if((i==n/2-1 && j==n/2-1) || (i==n/2 && j==n/2)) {
                chart[i][j]=white;
            }
            else if((i==n/2-1 && j==n/2) || (i==n/2 && j==n/2-1)) {
                chart[i][j]=black;
            }
            else chart[i][j]=blank;
        }
    }
}

// Handles keyboard input (WASD, Enter, Ctrl+S, Q)
void choose() {
    
    char move;
    while (true) {
        print();
        move = getch();

        if(move=='w' && y>0) y--;
        else if(move=='a' && x>0) x--;
        else if(move=='s' && y<n-1) y++;
        else if(move=='d' && x<n-1) x++;
        else if(move==13 && chart[y][x]=="*") {
            humanMove();
            return;
        }
        else if(move=='q') {
            saveGame();
            menu();
            return;
        }
    }
    return;
}

//Marks all the valid moves with '*'
void possible() {

    string opp=(turn==black ? white : black);

    // clear previous possible moves
    for(int i=0 ; i<n ; i++) {
        for(int j=0 ; j<n ; j++) {
            if(chart[i][j]=="*") chart[i][j]=blank;
        }
    }

    int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
    int dy[8] = {1, -1, 0, 0, 1, -1, 1, -1};

    // Scan the board for current player's discs
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {

            if(chart[i][j]==turn) {

                //check all directions
                for(int k=0; k<8; k++) {

                    int ny=i+dy[k];
                    int nx=j+dx[k];
                    bool foundOpp=false;

                    //move forward while we see opp discs
                    while(ny>=0 && ny<n && nx>=0 && nx<n && chart[ny][nx]==opp) {
                        foundOpp = true;
                        ny+=dy[k];
                        nx+=dx[k];
                    }

                    //put '*' if we reach a blank cell 
                    if(foundOpp && ny>=0 && ny<n && nx>=0 && nx<n && chart[ny][nx]==blank) {
                        chart[ny][nx] = "*";
                    }
                }
            }
        }
    }
}


// Clears the screen and prints the board, showing the cursor and the current player
void print() {

    system("cls");

    string currentPlayer;
    if(turn==black) currentPlayer=game[gameCount].player1;
    else currentPlayer=game[gameCount].player2;

    cout<< "Current Player : "<<currentPlayer<<endl;

    for(int i=0 ; i<n ; i++) {
        for (int j=0 ; j<n ; j++) {
            if(i==y && j==x) {
                cout<<"["<<chart[i][j]<<"]";
            }
            else cout<<" "<<chart[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"\nPress Q to go to menu...";
}

// Applies the human player's move by calling placeAndFlip()
void humanMove() {
    placeAndFlip();
    return;
}

//checks if there is any possible move
bool isPossible() {
    for(int i=0 ; i<n ; i++) {
        for(int j=0 ; j<n ; j++) {
            if(chart[i][j]=="*") return true;
        }
    }
    return false;
}

// Runs the full two-player game loop, switching turns and ending the game
void twoPlayer() {

    //checks if it's a new game or continuing a unfinished one
    if(!unfinnished) {
        board();
        turn=black;
    }
    possible();

    while(true) {
        choose();
        possible();

        //passes the turn if there is no possible move
        if(!isPossible()) {
            turn = (turn==black) ? (white) : (black);
            possible();

            if(!isPossible()) {
                print();
                winner();
                break;
            }
        }
    }
}

// Counts discs, finds the winner, saves game results, and displays the final result
void winner() {

    int countWhite=0;
    int countBlack=0;

    for(int i=0 ; i<n ; i++) {
        for(int j=0 ; j<n ; j++) {

            if(chart[i][j]==white) countWhite++;
            else if(chart[i][j]==black) countBlack++;
        }
    }

    system("cls");
    setColor(14);
    cout<<"Game Over!"<<endl;
    setColor(7);
    cout<<game[gameCount].player1<<" : "<<countBlack<<" scores"<<endl;
    cout<<game[gameCount].player2<<" : "<<countWhite<<" scores"<<endl;

    if(countBlack==countWhite) game[gameCount].winner="equal";
    else if(countBlack>countWhite) game[gameCount].winner = game[gameCount].player1;
    else if(countBlack<countWhite) game[gameCount].winner = game[gameCount].player2;

    if(game[gameCount].winner=="equal") {
        setColor(10);
        cout<<"It's a draw!"<<endl;
        setColor(7);
    }
    else cout<<game[gameCount].winner<<" is the winner \U0001F3C6\n\n";

    game[gameCount].countBlack = countBlack;
    game[gameCount].countWhite = countWhite;
    time_t now = time(0);
    string endTime = ctime(&now);
    endTime.erase(endTime.size()-1); //erasing the \n at the end of string
    game[gameCount].date = endTime;
    game[gameCount].endTime = now;

    //clears the loadGame.txt
    ofstream file("loadGame.txt");
    file.close();

    if(gameCount==capacity) resize();
    gameCount++;

    sortAndSave();

    cout<<"Press Q to return to menu...";
    while(true) {
        char input=getch();
        if(input=='q') {
            menu();
            return;
        }
    }
}

// Loads previously saved finished games from gameHistory.txt
void addGameHistory() {
    ifstream file("gameHistory.txt");

    if(!file.is_open()) {
        cout<<"file isn't open"<<endl;
        return;
    }

    string line;
    while(getline(file, line)) {
        if(line.empty()) continue;

        gameInfo temp;
        temp.player1 = line.substr(line.find(":")+2);

        getline(file, line);
        temp.player2 = line.substr(line.find(":")+2);

        getline(file, line);
        temp.countBlack = stoi(line.substr(line.find(":")+2));

        getline(file, line);
        temp.countWhite = stoi(line.substr(line.find(":")+2));

        getline(file, line);
        temp.winner = line.substr(line.find(":")+2);

        getline(file, line);
        temp.date = line.substr(line.find(":")+2);

        getline(file, line);
        temp.endTime = stoll(line.substr(line.find(":")+2));

        getline(file, line);

        game[gameCount]=temp;
        gameCount++;
        if(gameCount==capacity) resize();
       
    }
    file.close();
    return;
}

// Sorts all finished games by end time (newest first) and writes them in gameHistory.txt.
void sortAndSave() {

    for(int i=0 ; i<gameCount-1 ; i++) {
        for (int j=0 ; j<gameCount-i-1 ; j++) {

            if(game[j].endTime<game[j+1].endTime) {
                swap(game[j], game[j+1]);
            }
        }
        
    }


    ofstream file("gameHistory.txt");

    if(!file.is_open()) {
        cout<<"file isn't open"<<endl;
        return;
    }

    for(int i=0 ; i<gameCount ; i++) {

        file<<"Name of player1 : "<<game[i].player1<<endl;
        file<<"Name of player2 : "<<game[i].player2<<endl;
        file<<game[i].player1<<"'s scores : "<<game[i].countBlack<<endl;
        file<<game[i].player2<<"'s scores : "<<game[i].countWhite<<endl;
        file<<"The winner : "<<game[i].winner<<endl;
        file<<"date : "<<game[i].date<<endl;
        file<<"End time : "<<game[i].endTime<<endl;
        file<<"~~~~~~~~~~~~~~ END GAME ~~~~~~~~~~~~~~"<<endl;
    }
    file.close();
}

// robot chooses the move with max fliped discs
void robotMove() {
        
    int countPossible=0;
    string opp = (turn == black) ? white : black;
    int cx[64];
    int cy[64];

    //stores possible moves in cx[] and cy[]
    for(int i=0 ; i<n ; i++) {
        for(int j=0 ; j<n ; j++) {

            if(chart[i][j]=="*") {
                cx[countPossible]=j;
                cy[countPossible]=i;
                countPossible++;
            }
        }
    }

    if(countPossible==0) return;

    if(advanced==false) {
        int move = rand() % countPossible;
        x = cx[move];
        y = cy[move]; 
    }

    else if(advanced==true) {
        int countScore[64];
        for(int i=0 ; i<countPossible ; i++) {
            countScore[i]=0;
        }
        
        int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
        int dy[8] = {1, -1, 0, 0, 1, -1, 1, -1};

        //saves the count of fliped discs for each possible move in countScore[]
        for(int i=0 ; i<countPossible ; i++) {
            
            for(int k=0 ; k<8 ; k++) {
                int nx=cx[i]+dx[k];
                int ny=cy[i]+dy[k];
                
                int temp=0;
                while(ny>=0 && ny<n && nx>=0 && nx<n && chart[ny][nx]==opp) {
                    ny+=dy[k];
                    nx+=dx[k];
                    temp++;
                }

                if(ny>=0 && ny<n && nx>=0 && nx<n && chart[ny][nx]==turn && temp>0) {
                    countScore[i]+=temp;
                }
            }
        }
        
        //finds the max fliped discs
        int max=countScore[0];
        for (int i=1 ; i<countPossible ; i++) {
            if(countScore[i]>max) max=countScore[i];
        }
        
        //chooses the first max 
        for(int i=0 ; i<countPossible ; i++) {
            if(countScore[i]==max) {
                x=cx[i];
                y=cy[i];
                break;
            }
        }
    }
    placeAndFlip();
}

// Runs the single-player game loop, switching between human and robot turns.
void singlePlayer() {

    //checks if it's a new game or continuing a unfinished one
    if(!unfinnished) {
        board();
        turn = black;
    }
    possible();

    while(true) {
        if(isPossible()) {
            if(turn==black) {
                choose();
            }
            else {
                robotMove();
            }
            possible();
        }
        else {
            turn=(turn==black) ? white : black;
            possible();

            if (!isPossible()) {
                print();
                winner();
                break;
            }
        }
    }
}

//saves the game to continue later
void saveGame() {

    ofstream file("loadGame.txt");

    if(!file.is_open()) {
        cout<<"file isn't open"<<endl;
        return;
    }

    file<<turn<<endl;
    if(advanced==true) file<<"1"<<endl;
    else if(advanced==false) file<<"0"<<endl;
    file<<n<<endl;
    file<<game[gameCount].player1<<endl;
    file<<game[gameCount].player2<<endl;
    for(int i=0 ; i<n ; i++) {
        for(int j=0 ; j<n ; j++) {
            file<<chart[i][j]<<endl;
        }
    }
    file.close();

}

//continues the saved game
void loadGame() {

    x=0;
    y=0;
    ifstream file("loadGame.txt");

    if(!file.is_open()) {
        cout<<"file isn't open"<<endl;
        return;
    }

    string line;
    // checks if the loadGame.txt is empty
    // if it's empty, no game has been saved
    if(!getline(file, line)) {
        menu();
        return;
    }

    turn=line;

    getline(file, line);
    int temp = stoi(line);
    if(temp==1) advanced=true;
    else if(temp==0) advanced=false;

    getline(file, line);
    n=stoi(line);

    getline(file, line);
    game[gameCount].player1 = line;

    getline(file, line);
    game[gameCount].player2 = line;

    for(int i=0 ; i<n ; i++) {
        for(int j=0 ; j<n ; j++) {
            getline(file, line);
            chart[i][j]=line;
        }
    }
    file.close();

    unfinnished=true;
    if(game[gameCount].player2=="robot") singlePlayer();
    else twoPlayer();
}

// Changes console text color
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Displays guidelines 
void help() {
    system("cls");

    setColor(14);
    cout<<"\n\n\tWelcome to othello!\U0001F3AE\n\n\n";

    setColor(10);
    cout<<"Controls: \n\n";
    setColor(7);
    cout<<"- Use W(\u2191), A(\u2190), S(\u2193), D(\u2192) to move on the board\n";
    cout<<"- Press Enter to apply your move\n";
    cout<<"- Use Q to pause the game and continue it later(before you start another game) on loadGame !\n\n\n";

    setColor(13);
    cout<<"Symbols: \n\n";
    setColor(7);
    cout<<black<<" \u2192 Black piece\n";
    cout<<white<<" \u2192 White piece\n";
    cout<<blank<<" \u2192 Blank cell\n";
    cout<<"* \u2192 Possible move\n\n\n";

    setColor(12);
    cout<<"Game rules:\n\n";
    setColor(7);
    cout<<"- Players take turns placing discs.\n";
    cout<<"- A move is valid if it flips at least one opponent’s disc.\n";
    cout<<"- To flip, trap opponent’s discs between two of yours.\n";
    cout<<"- If no valid move, you pass automatically.\n";
    cout<<"- The game ends when both players have no valid move.\n";
    cout<<"- Winner is the player with more discs \U0001F3C6\n\n\n";

    setColor(7);

    cout<<"Press Q to return to menu...";
    while(true) {
        char input=getch();
        if(input=='q') {
            menu();
            return;
        }
    }
}

// displays all the finished games
void gameHistory() {

    system("cls");
    if(gameCount==0) {
        cout<<"No game has been finished !\n\n";
    }
    for(int i=0 ; i<gameCount ; i++) {
        if(game[i].date=="") continue;

        setColor(5);
        cout<<"----------------- GAME "<<i+1<<" -----------------\n";
        setColor(7);
        cout<<"Player 1 : "<<game[i].player1<<"\t player 2 : "<<game[i].player2<<endl;
        cout<<game[i].player1<<"'s scores : "<<game[i].countBlack<<"\t";
        cout<<game[i].player2<<"'s scores : "<<game[i].countWhite<<endl;
        cout<<"End time : "<<game[i].date<<endl;
        cout<<game[i].winner<<" was the winner \U0001F3C6\n\n";
    }

    cout<<"Press Q to return to menu...";
    while(true) {
        char input=getch();
        if(input=='q') {
            menu();
            return;
        }
    }
}

//flips the opp discs and switches the turn
void placeAndFlip() {

    string opp = (turn == black) ? white : black;
    int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
    int dy[8] = {1, -1, 0, 0, 1, -1, 1, -1};

    if(chart[y][x]=="*") {
        chart[y][x]=turn;
            
        for(int k=0 ; k<8 ; k++) {
            int ny=y+dy[k];
            int nx=x+dx[k];
            if(ny>=0 && ny<n && nx>=0 && nx<n && chart[ny][nx]==opp) {
                    
                while(ny>=0 && ny<n && nx>=0 && nx<n && chart[ny][nx]==opp) {
                    ny+=dy[k];
                    nx+=dx[k];
                }
                    
                if(ny>=0 && ny<n && nx>=0 && nx<n && chart[ny][nx]==turn) {
                    ny-=dy[k];
                    nx-=dx[k];
                    while(ny>=0 && ny<n && nx>=0 && nx<n && chart[ny][nx]==opp) {
                        chart[ny][nx]=turn;
                        ny-=dy[k];
                        nx-=dx[k];
                    }
                }
            }
        }
    }
    turn=opp;
}

void resize() {

    int newCapacity = capacity*2;
    gameInfo* newGame = new gameInfo[newCapacity];

    for(int i=0; i<capacity; i++) {
        newGame[i] = game[i];
    }

    delete[] game;
    game = newGame;
    capacity = newCapacity;
}