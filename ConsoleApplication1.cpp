#include <iostream>

using namespace std;

int field[100][100];
//-1 - mine
//0-2 - cell
int field_p[100][100];

const int N = 10, M = 10, K = 5; //строк, столбцов, мин

//функция вывода игрового поля
//-3 отобразить мину
//-2 не открыто - closed
//-1- флаг      - flag
//0-2 - цифры   - opened
void print_field() {
    system("cls");
    //system("clear"); //для Mac OS!

    //координаты столбца
    cout << "  ";
    for (int i = 0; i < M; ++i) {
        cout << " " << i + 1;
        if (i + 1 < 10) cout << " ";
    }
    cout << endl;

    //гровое поле    
    for (int i = 0; i < N; i++) {
        cout << i + 1 << " ";
        if (i + 1 < 10) cout << ' ';
        for (int j = 0; j < M; j++) {
            if (field_p[i][j] == -2)      cout << "_  ";
            else if (field_p[i][j] == -1)    cout << "f  ";
            else if (field_p[i][j] == -3)    cout << "*  ";
            else cout << field_p[i][j] << "  ";
        }
        cout << endl;
    }

    cout << endl << endl;
}

//обход в глубину - dfs
void dfs(int x, int y) {
    if (x < 0 || x >= N || y < 0 || y >= M || field_p[x][y] > -2) return;

    field_p[x][y] = field[x][y];
    if (field[x][y] > 0) return;

    for (int i2 = x - 1; i2 <= x + 1; ++i2) {
        for (int j2 = y - 1; j2 <= y + 1; ++j2) {
            if (i2 != x || j2 != y) dfs(i2, j2);
        }
    }
}

//true - играем дальше
//false - проиграли
bool open_cell(int x, int y) {
    if (field[x][y] == -1) return false; //мина

    if (field[x][y] > 0) {  //значение
        field_p[x][y] = field[x][y];
        return true;
    }

    dfs(x, y); //значение 0
}


bool is_win() {
    int opened = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (field_p[i][j] >= 0) opened++;
        }
    }

    return ((N * M - K) == opened); //кол-во открытых клеток  = кол-вл клеток без мин
}


int main()
{
    while (true) {
        srand(time(0)); //ранбомайзер стартует от текущего времени

        //обнулить
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                field[i][j] = 0; //-1 - клетка с миной, 0- нет вокруг мин, 8 - все мины вокруг
                field_p[i][j] = -2;
            }
        }

        //генерация координат мин
        for (int i = 0; i < K; i++) {
            while (true) {
                int x = rand() % N; //случайное число 0 .. N
                int y = rand() % M; //случайное число 0 .. M

                if (field[x][y] != -1) {
                    field[x][y] = -1;
                    // cout << x << ' ' << y << endl;
                    break;
                }
            }
        }

        //клетки вокруг - проставить кол-во мин
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (field[i][j] != -1) {
                    field[i][j] = 0;

                    for (int i2 = i - 1; i2 <= i + 1; i2++) {
                        for (int j2 = j - 1; j2 <= j + 1; j2++) {
                            if ((i2 >= 0) && (i2 < N) && (j2 >= 0) && (j2 < M) && (field[i2][j2] == -1))
                                ++field[i][j];
                        }
                    }
                }
            }
        }


        //тестовый вывод
        //for (int i = 0; i <N; i++) {
        //    for (int j = 0; j <M; j++) {
        //        if (field[i][j] == -1)
        //            cout << '*' << ' ';
        //        else
        //            cout << field[i][j] << ' ';
        //    } 
        //     cout << endl;
        //}  

        //ходы
        while (true) {
            print_field();

            cout << "Enter command:";
            /*
              \o x y  - открыть клетку
              \f x y - установить/снять флаг
              \n - начать заново
              \q - выход
            */
            string command = "";
            cin >> command;

            if (command == "\\o") { //экранирование "\"
                int x, y;
                cin >> x >> y;
                --x; --y;
                if (x < 0 || x >= N || y < 0 || y >= M || field_p[x][y] >= 0) continue;

                //проверка на проигрыш
                if (!open_cell(x, y)) {
                    //вывести все мины
                    for (int i = 0; i < N; i++) {
                        for (int j = 0; j < M; j++) {
                            if (field_p[i][j] == -1) field_p[i][j] = -2; //открыть флаги
                            if (field[i][j] == -1) field_p[i][j] = -3;  //отобразить мины
                        }
                    }
                    print_field();
                    cout << "You loose! \n For begin new enter any";
                    string s;
                    cin >> s;
                    break;
                }
                //проверка на проигрыш
                if (is_win()) {
                    //вывести все мины
                    for (int i = 0; i < N; i++) {
                        for (int j = 0; j < M; j++) {
                            if (field_p[i][j] == -1) field_p[i][j] = -2; //открыть флаги
                            if (field[i][j] == -1) field_p[i][j] = -3;  //отобразить мины
                        }
                    }
                    print_field();
                    cout << "You win! \n For begin new enter any";
                    string s;
                    cin >> s;
                    break;
                }

            }
            else if (command == "\\f") {
                int x, y;
                cin >> x >> y;
                --x; --y;
                if (x < 0 || x >= N || y < 0 || y >= M || field_p[x][y] >= 0) continue;

                //установить флаг
                if (field_p[x][y] == -1) field_p[x][y] = -2;
                else field_p[x][y] = -1;

            }
            else if (command == "\\n") {
                break;
            }
            else if (command == "\\q") {
                //выход
                return 0;
            }

        }

    }
}










