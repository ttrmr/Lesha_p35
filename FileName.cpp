#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <Windows.h>
#include <CoreWindow.h> 
#include <ctime>
#include <fstream>
using namespace std;

const string red = "\33[31m";
const string green = "\33[32m";
const string zero = "\33[0m";
const string clear = "\033[2J\033[1;1H";

void set_Board(char** board, int rows, int cols, int pairs) {
    vector<char> sym;
    for (int i = 0; i < pairs; ++i) {
        sym.push_back('A' + i);
        sym.push_back('A' + i);
    }

    random_shuffle(sym.begin(), sym.end());

    int index = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i][j] = sym[index++];
        }
    }
}

void cout_Board(char** board, bool** rev, int rows, int cols) {
    cout << "   ";
    for (int j = 0; j < cols; ++j) cout << j + 1 << " ";
    cout << endl << "   ";
    for (int j = 0; j < cols - 1; ++j) cout << "--";
    cout << "-" << endl;
    for (int i = 0; i < rows; ++i) {
        cout << char('A' + i) << "| ";
        for (int j = 0; j < cols; ++j) {
            if (rev[i][j]) cout << green << board[i][j] << zero << " ";
            else cout << "X ";
        }
        cout << endl;
    }
}

bool is_Val(char r, int c, int rows, int cols) {
    r = toupper(r);
    return (r >= 'A' && r < 'A' + rows && c >= 1 && c <= cols);
}

bool all_Cell(bool** rev, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!rev[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void clear_B() {
    cin.clear();
    cin.ignore(32767, '\n');
}

void save_StatFile(int gameNumber, double timePlayer, int flips) {
    ofstream outFile("result.txt", ios::app);
    outFile << "��� #" << gameNumber << endl;
    outFile << "��� ���: " << timePlayer << " ������" << endl;
    outFile << "ʳ������ ���������� ����: " << flips << endl;
    outFile << endl;
    outFile.close();
}

int main() {
    srand(time(0));
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int rows, cols, gameNumber = 1;
    while (true) {
        cout << clear;
        cout << "\t\t\t��� ��� ��� \33[43m\33[35m���'���\33[0m\33[0m\n\n";
        cout << "��� ������� ��� ������� ����� ����\n1 - 4x4 (normal) / 2 - 5x6 (hard): ";
        int choice;
        cin >> choice;
        cout << endl;
        while (choice != 1 && choice != 2) {
            cout << "\n�������!!!\n" << red << "�� ��������� ������ ����� ����!\n" << zero << "������ ����� ����� ���� " << green << "���������: " << zero;
            cin >> choice;
        }
        if (choice == 1) {
            cout << "�� ������� ����� ��� " << green << "normal" << zero << endl;
            rows = 4;
            cols = 4;
        }
        if (choice == 2) {
            cout << "�� ������� ����� ��� " << red << "hard" << zero << endl;
            rows = 5;
            cols = 6;
        }

        char** board = new char* [rows];
        bool** rev = new bool* [rows];

        for (int i = 0; i < rows; ++i) {
            board[i] = new char[cols];
            rev[i] = new bool[cols];
            for (int j = 0; j < cols; ++j) {
                rev[i][j] = false;
            }
        }

        int pairs = (rows * cols) / 2;
        set_Board(board, rows, cols, pairs);

        cout << "�����'�������� ������������ ���\n";
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                rev[i][j] = true;
            }
        }

        cout_Board(board, rev, rows, cols);
        Sleep(5000);
        cout << clear;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                rev[i][j] = false;
            }
        }

        int flips = 0;
        auto startPlayer = chrono::steady_clock::now();

        while (true) {
            cout << clear;
            cout_Board(board, rev, rows, cols);

            char r1, r2;
            int c1, c2;
            cout << "������ ���������� ����� ����� (���������, A 1): ";
            if (!(cin >> r1) || !(cin >> c1)) {
                cout << red << "������� � �������! ���� �����, ��������� �� ���." << zero << endl;
                Sleep(3000);
                clear_B();
                continue;
            }
            cout << "������ ���������� ����� ����� (���������, B 2): ";
            if (!(cin >> r2) || !(cin >> c2)) {
                cout << red << "������� � �������! ���� �����, ��������� �� ���." << zero << endl;
                Sleep(3000);
                clear_B();
                continue;
            }

            r1 = toupper(r1);
            r2 = toupper(r2);
            int row1 = r1 - 'A', row2 = r2 - 'A';
            int col1 = c1 - 1, col2 = c2 - 1;

            if (!is_Val(r1, c1, rows, cols) || !is_Val(r2, c2, rows, cols) || rev[row1][col1] || rev[row2][col2] || (row1 == row2 && col1 == col2)) {
                cout << red << "���������� ���������� ��� ����� ��� �������!" << zero << endl;
                Sleep(3000);
                continue;
            }

            flips++;
            rev[row1][col1] = true;
            rev[row2][col2] = true;
            cout << clear;
            cout_Board(board, rev, rows, cols);

            if (board[row1][col1] == board[row2][col2]) {
                cout << green << "���� �������!" << zero << endl;
                Sleep(3000);
            }
            else {
                cout << red << "���� �� �������!" << zero << endl;
                Sleep(3000);
                rev[row1][col1] = false;
                rev[row2][col2] = false;
            }

            if (all_Cell(rev, rows, cols)) {
                auto endPlayer = chrono::steady_clock::now();
                chrono::duration<double> elapsedPlayer = endPlayer - startPlayer;
                double timePlayer = elapsedPlayer.count();
                cout << green << "\n�� ���������!\n" << zero << endl;
                cout << "��� ���: " << timePlayer << " ������" << endl;
                cout << "ʳ������ ���������� ����: " << flips << endl;
                break;
                save_StatFile(gameNumber, timePlayer, flips);
                gameNumber++;
                break;
            }
        }

        for (int i = 0; i < rows; ++i) {
            delete[] board[i];
            delete[] rev[i];
        }
        delete[] board;
        delete[] rev;
        cout << "\n������ ������ �� ���? (y/n): ";
        char newGame;
        cin >> newGame;
        if (tolower(newGame) == 'n') {
            break;
        }
    }
    return 0;
}
