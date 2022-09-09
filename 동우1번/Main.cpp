#define MainNumber 1

#if MainNumber == 1
#include "Matrix.h"
#include <conio.h>

void PrintResult(Matrix& A, Matrix& B, Matrix& sum, char symbol);
void PrintMatrix(Matrix& matrix, int x, int y, string name);

int main()
{
    Matrix A, B;
    A.Init();
    B.Init();

    Matrix sum;
    char cmd;
    while (true)
    {
        system("cls");
        {
            gotoxy(0, 0);
            cout << "A";
            A.print(0, 1);
        }
        {
            gotoxy(A.matrix_X + 2, 0);
            cout << "B";
            B.print(A.matrix_X + 2, 1);
        }
        gotoxy(0, A.matrix_Y + 3);
        cout << "a : ����� ����" << endl;
        cout << "d : ����� ����" << endl;
        cout << "m : ����� ����" << endl;
        cout << "r : ��Ľ��� ��" << endl;
        cout << "t : ��ġ��� " << endl;
        cout << "h : ��� ũ�� �� ����" << endl;
        cout << "s : ��İ� ���� �缳��" << endl;
        cout << "q : ���α׷� ����" << endl;

        cmd = _getch();

        if (cmd == 'a' || cmd == 'A') // ���ϱ�
        {
            sum.ReSize(A.matrix_X, A.matrix_Y);
            sum.matrix_Index = A.Plus(B);
            PrintResult(A, B, sum, '+');

            gotoxy(0, A.matrix_Y + 5);
            cout << "�ƹ�Ű�� ������ �� ȭ������ �ǵ��� �ɴϴ�." << endl;
            system("pause");
        }
        else if (cmd == 'd' || cmd == 'D')
        {
            sum.ReSize(A.matrix_X, A.matrix_Y);
            sum.matrix_Index = A.Minus(B);
            PrintResult(A, B, sum, '-');

            gotoxy(0, A.matrix_Y + 5);
            cout << "�ƹ�Ű�� ������ �� ȭ������ �ǵ��� �ɴϴ�." << endl;
            system("pause");
        }
        else if (cmd == 'm' || cmd == 'M')
        {
            sum.ReSize(A.matrix_X, A.matrix_Y);
            sum.matrix_Index = A.Multiplication(B);
            PrintResult(A, B, sum, '*');

            gotoxy(0, A.matrix_Y + 5);
            cout << "�ƹ�Ű�� ������ �� ȭ������ �ǵ��� �ɴϴ�." << endl;
            system("pause");
        }
        else if (cmd == 'r' || cmd == 'R')
        {
            system("cls");
            PrintMatrix(A, 0, 0, "A");
            gotoxy(A.matrix_X + 1, A.matrix_Y / 2 + 1);
            cout << "= " << A.determinant(A.matrix_Index, A.matrix_X);

            PrintMatrix(B, A.matrix_X + B.matrix_X + 2, 0, "B");
            gotoxy(A.matrix_X + B.matrix_X + 5, B.matrix_Y / 2 + 1);
            cout << "= " << B.determinant(B.matrix_Index, B.matrix_X);

            gotoxy(0, A.matrix_Y + B.matrix_Y + 5);
            cout << "�ƹ�Ű�� ������ �� ȭ������ �ǵ��� �ɴϴ�." << endl;
            system("pause");
        }
        else if (cmd == 't' || cmd == 'T')
        {
            system("cls");

            PrintMatrix(A, 0, 0, "A");
            A.Transposed();
            PrintMatrix(A, A.matrix_X + 1, 0, "A(T)");
            gotoxy(A.matrix_X * 2 + 1, A.matrix_Y / 2 + 1);
            cout << "= " << A.determinant(A.matrix_Index, A.matrix_X);

            PrintMatrix(B, 0, A.matrix_Y + 2, "B");
            B.Transposed();
            PrintMatrix(B, B.matrix_X + 1, A.matrix_Y + 2, "B(T)");
            gotoxy(A.matrix_X * 2 + 1, A.matrix_Y / 2 + 3);
            cout << "= " << B.determinant(B.matrix_Index, B.matrix_X);

            gotoxy(0, A.matrix_Y + B.matrix_Y + 5);
            cout << "�ƹ�Ű�� ������ �� ȭ������ �ǵ��� �ɴϴ�." << endl;
            system("pause");
        }
        else if (cmd == 'h' || cmd == 'H')
        {
            system("cls");
            int sizeX, sizeY;
            char matrix_Name;
            cout << "�ٲ� size�� �����ֽÿ�" << endl;
            cout << "���� : ";
            cin >> sizeX;
            cout << "���� : ";
            cin >> sizeY;
            cout << endl;
            cout << "�ٲ� Matrix�� �����ֽÿ�" << endl;
            cout << "A, B �ܿ� �ٸ� Ű �Է½� A,B �� ������ ������" << endl;
            cin >> matrix_Name;
            if (matrix_Name == 'a' || matrix_Name == 'A')
                A.ReSize(sizeX, sizeY);
            else if (matrix_Name == 'b' || matrix_Name == 'B')
                B.ReSize(sizeX, sizeY);
            else
            {
                A.ReSize(sizeX, sizeY);
                B.ReSize(sizeX, sizeY);
            }

            system("pause");
        }
        else if (cmd == 's' || cmd == 'S')
        {
            system("cls");
            cout << "���� �� ������ Matrix�� ����ֽÿ�" << endl;
            cout << "A, B �ܿ� �ٸ� Ű �Է½� A,B �� �� ������" << endl;

            char matrix_Name;
            cin >> matrix_Name;
            if (matrix_Name == 'a' || matrix_Name == 'A')
                A.Init();
            else if (matrix_Name == 'b' || matrix_Name == 'B')
                B.Init();
            else
            {
                A.Init();
                B.Init();
            }
            system("pause");
        }
        
        if (cmd == 'q' || cmd == 'Q')
            break;
    }
}

void PrintMatrix(Matrix& matrix, int x, int y, string name)
{
    gotoxy(x, y);
    cout << name;
    matrix.print(x, y + 1);
}

void PrintResult(Matrix& A, Matrix& B, Matrix& sum, char symbol)
{
    system("cls");
    PrintMatrix(A, 0, 0, "A");

    gotoxy(A.matrix_X + 1, A.matrix_Y/2 + 1);
    cout << symbol;

    PrintMatrix(B, A.matrix_X + 2, 0, "B");

    gotoxy(A.matrix_X + B.matrix_X + 3, B.matrix_Y / 2 + 1);
    cout << "=";

    PrintMatrix(sum, A.matrix_X + B.matrix_X + 4, 0, "sum");
}



#endif // 0