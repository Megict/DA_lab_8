#include <iostream>
#include <vector>
#include <map>

#define DEBUG false 

struct TMatrix {
    std::vector<std::vector<double>> Body;
    int M, N;

    TMatrix(int m, int n) : M(m), N(n)
    {
        Body = std::vector<std::vector<double>>(m);

        for (int i = 0; i < m; ++i) {
            Body[i] = std::vector<double>(n , 0);
        }
    }

    void FillMatrix(std::map<double, int> &inter/*заполняется между заполнениями рядов*/) {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cin >> Body[i][j];
            }

            double intV;
            std::cin >> intV;
            inter.emplace(std::pair<double, int>(intV, i));
        }
    }

    int Echelonise() {
        int startCounter = 0;
        for (int k = 0; k < N; ++k) {

            if (Body[startCounter][k] == 0) {//замена строки, если начинается на 0
                for (int j = startCounter; j < M; ++j) { 
                    if (Body[j][k] != 0) {
                        std::vector<double> f = Body[startCounter];
                        Body[startCounter] = Body[j];
                        Body[j] = f;
                        break;
                    }
                }
            }

            if (Body[startCounter][k] == 0) {
                continue;
            }


            for (int i = startCounter + 1; i < M; i++) { //основной цикл
                double factor = Body[i][k] / Body[startCounter][k];
                if(DEBUG) std::cout << "[i|k|sc: " << i << " | " << k << " | " << startCounter << " factor: " << factor << " sizes: " << M << " " << N << std::endl;
                for (int j = k; j < N; ++j) {
                    Body[i][j] -= Body[startCounter][j] * factor;
                }
            }
            startCounter++;

            if (startCounter >= M) {
                break;
            }
        }

        int lastRow = M - 1;
        for (int i = 0; i < N; ++i) {
            if (Body[lastRow][i] != 0) {
                return 0;
            }
        }

        return 1;
    }
};

int main() {
    int m, n;
    std::cin >> m >> n;

    std::map<double, int> costs;
    std::map<int, int> res;
    TMatrix test(m,n);

    test.FillMatrix(costs);

    /*
        1. выбрали строку с мин. стоимостью
        2. выбираем строку ЛНЗ с выбранными ранее строками с мин. стоимостью
        3. пока не останется строк для выбора.
        4. если система имеет одно решение, то нашли подходящий набор. Иначе – решений нет.
    */

    long long cost = 0;
    TMatrix resMatr = TMatrix(0, test.N);

    for (std::pair<int,double> iter : costs) {
        //берем следующую по цене добавку

        resMatr.Body.push_back(test.Body[iter.second]);
        resMatr.M++;
        //добавляем ее в матрицу

        if (DEBUG) {
            printf("attempting to add: \n");
            for (int i = 0; i < test.Body[iter.second].size(); ++i) {
                printf("%lf ", test.Body[iter.second][i]);
            }
            printf("\n\n");
            for (int i = 0; i < resMatr.M; ++i) {
                for (int j = 0; j < resMatr.N; ++j) {
                    printf("%lf ", resMatr.Body[i][j]);
                }
                printf("\n");
            }
        }

        TMatrix checkMatr = resMatr;
        int LinDep = checkMatr.Echelonise();

        if (DEBUG) {
            printf("echelonized:\n");
            for (int i = 0; i < checkMatr.M; ++i) {
                for (int j = 0; j < checkMatr.N; ++j) {
                    printf("%lf ", checkMatr.Body[i][j]);
                }
                printf("\n");
            }
        }

        if (LinDep == 0) {
            //если получилась линейно независимая система, то оставляем строку
            cost += iter.first;
            res.emplace(std::pair<int, int>(iter.second, iter.second));
        }
        else {
            //если нет, то убираем
            resMatr.M--;
            resMatr.Body.pop_back();
        }


        if (resMatr.M == resMatr.N) {
            //если собрали решаемую систему лин. уравнений, то останавливаемся.
            break;
        }
    }

    if (DEBUG) {
        printf("resulted matr: \n");
        for (int i = 0; i < resMatr.M; ++i) {
            for (int j = 0; j < resMatr.N; ++j) {
                printf("%lf ", resMatr.Body[i][j]);
            }
            printf("\n");
        }
        printf("\nsumm\n");
    }

    if (resMatr.M != resMatr.N) {
        std::cout << "-1" << std::endl;
        return 0;
    }

    for (auto iter : res) {
        std::cout << iter.first + 1 << " ";
    }
    std::cout << std::endl;

}