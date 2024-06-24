#include <iostream>
#include <fstream>
#include <conio.h>
#include <Windows.h>
#include <string>

using namespace std;

void HideCursor() {
    HANDLE cursor = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO idk;
    idk.dwSize = 1;
    idk.bVisible = FALSE;
    SetConsoleCursorInfo(cursor, &idk);
}

void gotoxy(int x, int y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD axis;
    axis.X = x;
    axis.Y = y;
    SetConsoleCursorPosition(handle, axis);
}

void DisplayWelcomeMessage() {
    system("Color 0E");
    gotoxy(30, 5);
    cout << "Hotel Room Reservation System" << endl;
    gotoxy(30, 7);
    cout << "For First Class=1, For Economy Class=2, Exit=-1" << endl;
}

void CollectCredentials(const string& roomType) {
    string name, id;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter your ID: ";
    getline(cin, id);

    ofstream fout;
    if (roomType == "First Class") {
        fout.open("firstclass.txt", ios::app);
    }
    else {
        fout.open("economyclass.txt", ios::app);
    }

    fout << "Name: " << name << ", ID: " << id << ", Room Type: " << roomType << endl;
    fout.close();
}

void ReserveRoom(char data[][6], int ans, bool& found) {
    int firstClassCount = 0, economyClassCount = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            if (data[i][j] == 'X') firstClassCount++;
        }
    }
    for (int i = 2; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            if (data[i][j] == 'X') economyClassCount++;
        }
    }

    if (ans == 1) {  // Reserve room in First Class section
        if (firstClassCount >= 10) {
            gotoxy(30, 11);
            cout << "First Class rooms are fully booked. Please visit next time.";
            return;
        }
        for (int i = 0; i < 2 && !found; i++) {
            for (int j = 0; j < 6 && !found; j++) {
                if (data[i][j] != 'X') {
                    data[i][j] = 'X';
                    found = true;
                    CollectCredentials("First Class");
                }
            }
        }
    }
    else if (ans == 2) {  // Reserve room in Economy Class section
        if (economyClassCount >= 10) {
            gotoxy(30, 11);
            cout << "Economy Class rooms are fully booked. Please visit next time.";
            return;
        }
        int ans2;
        gotoxy(30, 11);
        cout << "For Non-Smoking=1, For Smoking=2: ";
        cin >> ans2;
        int startRow = (ans2 == 1) ? 2 : 5;  // Starting row based on Non-Smoking/Smoking
        int endRow = (ans2 == 1) ? 5 : 8;    // Ending row based on Non-Smoking/Smoking
        for (int i = startRow; i < endRow && !found; i++) {
            for (int j = 0; j < 6 && !found; j++) {
                if (data[i][j] != 'X') {
                    data[i][j] = 'X';
                    found = true;
                    string roomType = (ans2 == 1) ? "Economy Class Non-Smoking" : "Economy Class Smoking";
                    CollectCredentials(roomType);
                }
            }
        }
    }
}

void DisplayRooms(char data[][6]) {
    gotoxy(30, 11);
    cout << "Room Status:" << endl;
    for (int i = 0; i < 8; i++) {
        gotoxy(30, 13 + i);
        cout << "Floor " << i + 1 << "   ";
        for (int j = 0; j < 6; j++) {
            if (data[i][j] != 'X') {
                cout << "* ";
            }
            else {
                cout << "X ";
            }
        }
        cout << endl;
    }
}

int main() {
    HideCursor();
    char data[8][6] = { 0 };  // Initialize room data
    int ans;
    bool found;

    do {
        system("cls");
        DisplayWelcomeMessage();
        gotoxy(30, 9);
        cout << "Enter your choice: ";
        cin >> ans;

        while (ans != -1) {
            found = false;  // Reset found flag
            system("cls");
            DisplayWelcomeMessage();
            gotoxy(30, 9);

            if (ans == 1 || ans == 2) {
                ReserveRoom(data, ans, found);  // Reserve room based on user input
                if (!found && ans != 1) {
                    gotoxy(30, 11);
                    cout << "No rooms available in the selected section\n";
                }
                else if (found) {
                    gotoxy(30, 11);
                    cout << "Action Successful. Please Come Again...";
                }
            }
            else {
                gotoxy(30, 11);
                cout << "Invalid Input. Please Enter 1 or 2" << endl;
            }

            gotoxy(30, 13);
            cout << "Press -1 to return to the main menu or enter your choice: ";
            cin >> ans;
        }

        system("cls");
        DisplayRooms(data);
        gotoxy(30, 21);
        cout << "Press any key to return to the main menu...";
        _getch();
    } while (ans == -1);

    return 0;
}
