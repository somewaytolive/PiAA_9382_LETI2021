#include <iostream>
#include <vector>
#include <ctime>

// промежуточный вывод
#define OUTPUT

// подсчет операций
#define COUNTOPERATIONS

#ifdef COUNTOPERATIONS
int countOfOperations = 0;
#endif

class SmallSquare // класс для содержания информации о конкретных обрезках
{
public:
    int x;
    int y;
    int size;
};

bool tryAbilityToAddSquare(const std::vector<std::vector<int>>& mainSquare, int x, int y, int sizeOfSmallSqr) 
{
    #ifdef COUNTOPERATIONS
    ++countOfOperations;
    #endif

    // базовая проверка на адекватность
    if ((x + sizeOfSmallSqr) > mainSquare.size() || (y + sizeOfSmallSqr) > mainSquare.size())
    {
        return false;
    }

    for (int i = y; i < y + sizeOfSmallSqr; i++) 
    {
        for (int j = x; j < x + sizeOfSmallSqr; j++) 
        {
            if (mainSquare[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

void addSmallSqrToMainSqr(std::vector<std::vector<int>>& mainSquare, int x, int y, int sizeOfSmallSqr)
{
    #ifdef COUNTOPERATIONS
    ++countOfOperations;
    #endif

    // просто закрашиваем указанные клетки без проверки
    for (int i = y; i < y + sizeOfSmallSqr; i++) 
    {
        for (int j = x; j < x + sizeOfSmallSqr; j++) 
        {
            mainSquare[i][j] = sizeOfSmallSqr;
        }
    }
}

const void printMainSquare(const std::vector <std::vector <int>>& mainSquare, int compr, int sizeOfMainSquare) 
{
    for (int i = 0; i < sizeOfMainSquare * compr; i++) 
    {
        for (int j = 0; j < sizeOfMainSquare * compr; j++) // просто печатаем все клетки по порядку
        {
            std::cout.width(3); // чтобы получился почти квадратик, а не прямоугольник
            std::cout << mainSquare[i][j];
        }
        std::cout << std::endl;
    }
}

void initMainSqrWithZeros(std::vector<std::vector<int>>& mainSquare, int sizeOfMainSquare) // заполнен нулями == не поставлено ни одного квадрата
// здесь ссылку константной сделать не можем, т.к. в 85 и 88 строках делается resize
{
    #ifdef COUNTOPERATIONS
    ++countOfOperations;
    #endif

    // просто заполняем все нулями
    mainSquare.resize(sizeOfMainSquare);
    for (int i = 0; i < sizeOfMainSquare; i++) 
    {
        mainSquare[i].resize(sizeOfMainSquare);

        for (int j = 0; j < sizeOfMainSquare; j++) 
        {
            mainSquare[i][j] = 0;
        }
    }
}

void deleteLastSmallSqr(std::vector<std::vector<int>>& mainSquare, std::vector<SmallSquare>& currArrayOfSmallSqrs, int recursionDepth) 
{
    #ifdef COUNTOPERATIONS
    ++countOfOperations;
    #endif

    SmallSquare lastSqr = currArrayOfSmallSqrs.back(); 
    // очистка из массива
    currArrayOfSmallSqrs.pop_back();

    // очистка из основного квадрата
    for (int i = lastSqr.y; i < lastSqr.y + lastSqr.size; i++) 
    {
        for (int j = lastSqr.x; j < lastSqr.x + lastSqr.size; j++) 
        {
            mainSquare[i][j] = 0;
        }
    }

    #ifdef OUTPUT
    for(int l = 0; l < recursionDepth; l++)
    {
        std::cout << " ";
    }
    std::cout << "Удаляем квадрат (l = " << lastSqr.size << ", x = " <<  lastSqr.x + 1 << ", y = " << lastSqr.y + 1 << ')' << std::endl;
    #endif
}


void preworkWithMainSqr(int &compr, std::vector<std::vector<int>>& mainSquare, std::vector<SmallSquare>& currArrayOfSmallSqrs, int& freeAreaOfMainSquare, int& sizeOfMainSquare, int& bestCountOfSquares) 
{
    #ifdef COUNTOPERATIONS
    ++countOfOperations;
    #endif

    initMainSqrWithZeros(mainSquare, sizeOfMainSquare); // инициализация пустого "видимого" квадрата

    // оптимизации для простейших кратностей
    if (sizeOfMainSquare % 2 == 0) 
    {
        compr = sizeOfMainSquare / 2;
        sizeOfMainSquare = 2;
    }
    else if (sizeOfMainSquare % 3 == 0) 
    {
        compr = sizeOfMainSquare / 3;
        sizeOfMainSquare = 3;
    }
    else if (sizeOfMainSquare % 5 == 0) 
    {
        compr = sizeOfMainSquare / 5;
        sizeOfMainSquare = 5;
    }
    // для 7 и дальше нет смысла, т.к. следующее место, где оно поможет слишком далеко


    bestCountOfSquares = 2 * sizeOfMainSquare + 1;

    // первые 3 квадрата
    currArrayOfSmallSqrs.push_back({ 0, 0, (sizeOfMainSquare + 1) / 2 });
    currArrayOfSmallSqrs.push_back({ 0, (sizeOfMainSquare + 1) / 2, sizeOfMainSquare / 2 });
    currArrayOfSmallSqrs.push_back({ (sizeOfMainSquare + 1) / 2, 0, sizeOfMainSquare / 2 });

    addSmallSqrToMainSqr(mainSquare, 0, 0, (sizeOfMainSquare + 1) / 2);
    addSmallSqrToMainSqr(mainSquare, 0, (sizeOfMainSquare + 1) / 2, sizeOfMainSquare / 2);
    addSmallSqrToMainSqr(mainSquare, (sizeOfMainSquare + 1) / 2, 0, sizeOfMainSquare / 2);

    #ifdef OUTPUT
    std::cout << "Вставляем квадрат (l = " << (sizeOfMainSquare + 1) / 2 << ", x = " <<  1 << ", y = " << 1 << ")" << std::endl;
    std::cout << "Вставляем квадрат (l = " << sizeOfMainSquare / 2 << ", x = " <<  1 << ", y = " << (sizeOfMainSquare + 1) / 2 + 1 << ")" << std::endl;
    std::cout << "Вставляем квадрат (l = " << sizeOfMainSquare / 2 << ", x = " <<  (sizeOfMainSquare + 1) / 2 + 1 << ", y = " << 1 << ")" << std::endl;
    #endif

    // обновление к-ва пустого пространства
    freeAreaOfMainSquare = sizeOfMainSquare * sizeOfMainSquare - ((sizeOfMainSquare + 1) / 2) * ((sizeOfMainSquare + 1) / 2) - 2 * (sizeOfMainSquare / 2) * (sizeOfMainSquare / 2);
}

void doTaskRecursive(std::vector<std::vector<int>>& mainSquare, int freeAreaOfMainSquare, int sizeOfSmallSqr, int currCountOfSmallSqrs, std::vector<SmallSquare>& currArrayOfSmallSqrs, int recursionDepth, int sizeOfMainSquare, int& bestCountOfSquares, std::vector<SmallSquare>& bestArrayOfSmallSqrs) 
{
    #ifdef COUNTOPERATIONS
    ++countOfOperations;
    #endif

    // даже если квадрат заполнит все пустоты - результат будет не лучше нынешнего
    if ( currCountOfSmallSqrs == (bestCountOfSquares - 1) && freeAreaOfMainSquare ) 
    {
        #ifdef OUTPUT
        for(int l = 0; l < recursionDepth; l++)
        {
            std::cout << " ";
        }
        std::cout << "Количество квадратов не меньше лучшего, выход из рекурсии" << std::endl;
        #endif

        return;
    }

    // первый добавленный квадрат (после начальных 3)
    if ( (sizeOfSmallSqr + 1) <= (sizeOfMainSquare / 2) && currCountOfSmallSqrs == 3 ) 
    {
        // рекурсивный вызов этой же функции
        doTaskRecursive(mainSquare, freeAreaOfMainSquare, (sizeOfSmallSqr+1), currArrayOfSmallSqrs.size(), currArrayOfSmallSqrs, 0, sizeOfMainSquare, bestCountOfSquares, bestArrayOfSmallSqrs);
    }


    bool ableToAddSmallSqr = false;
    for (int y = 0; y < sizeOfMainSquare; y++) 
    {
        for (int x = 0; x < sizeOfMainSquare; x++) 
        {
            // для каждой пустой клетки происходит попытка вставить квадрат текущего размера
            if (mainSquare[y][x] == 0) 
            {
                if (tryAbilityToAddSquare(mainSquare, x, y, sizeOfSmallSqr))
                {
                    ableToAddSmallSqr = true;
                    addSmallSqrToMainSqr(mainSquare, x, y, sizeOfSmallSqr);
                    freeAreaOfMainSquare -= sizeOfSmallSqr * sizeOfSmallSqr;
                    currArrayOfSmallSqrs.push_back({ x, y, sizeOfSmallSqr });

                    #ifdef OUTPUT
                    for(int l=0; l < recursionDepth; l++)
                    {
                        std::cout << " ";
                    }
                    std::cout << "Вставляем квадрат (l = " << sizeOfSmallSqr<< ", x = " <<  x + 1 << ", y = " << y + 1 << ")" << std::endl;
                    #endif

                    break;
                }
                else
                {
                    #ifdef OUTPUT
                    for(int l=0; l < recursionDepth; l++)
                    {
                        std::cout << " ";
                    }
                    std::cout << "Не можем поставить квадрат (l = " << sizeOfSmallSqr<< ", x = " <<  x + 1 << ", y = " << y + 1 << ")" << std::endl;
                    #endif

                    return;
                }
            }
            else 
            {
                x += (mainSquare[y][x] - 1);
            }
        }

        // выход из цикла
        if(ableToAddSmallSqr) 
        {
            break;
        }
    }

    // нет смысла обновлять показатели лучшего результата при таком же количестве обрезков
    if ( currCountOfSmallSqrs + 1 == bestCountOfSquares) 
    {
        #ifdef OUTPUT
        for(int l = 0; l < recursionDepth; l++)
        {
            std::cout << " ";
        }
        std::cout << "Количество квадратов не меньше лучшего, выход из рекурсии" << '\n';
        #endif

        deleteLastSmallSqr(mainSquare, currArrayOfSmallSqrs, recursionDepth);
        return;
    }

    // минимальное заполнение
    if ( ((currCountOfSmallSqrs + 1) < bestCountOfSquares) && (freeAreaOfMainSquare == 0)) 
    {
        bestCountOfSquares = currCountOfSmallSqrs + 1;
        bestArrayOfSmallSqrs.assign(currArrayOfSmallSqrs.begin(), currArrayOfSmallSqrs.end());

        #ifdef OUTPUT
        std::cout << "!!!Нашли новое лучшее количество квадратов: " << bestCountOfSquares << std::endl;
        #endif

        deleteLastSmallSqr(mainSquare, currArrayOfSmallSqrs, recursionDepth);
        return;
    }

    // рекурсивный вызов этой же функции
    for (int i = sizeOfMainSquare / 2; i > 0; i--) 
    {
        if (i * i <= freeAreaOfMainSquare) 
        {
            #ifdef OUTPUT
            for (int l=0; l < recursionDepth+2; l++)
            {
                std::cout << " ";
            }
            std::cout << "Вызываем рекурсию для квадрата со стороной " << i << '\n';
            #endif

            doTaskRecursive(mainSquare, freeAreaOfMainSquare, i, currCountOfSmallSqrs + 1, currArrayOfSmallSqrs, recursionDepth+2, sizeOfMainSquare, bestCountOfSquares, bestArrayOfSmallSqrs);
        }
    }

    deleteLastSmallSqr(mainSquare, currArrayOfSmallSqrs, recursionDepth);
}

int main() 
{
    setlocale(LC_ALL, "Russian");

    std::vector <std::vector<int>> mainSquare; // главный квадрат (0 = не занято)
    std::vector <SmallSquare> currArrayOfSmallSqrs;
    std::vector <SmallSquare> bestArrayOfSmallSqrs;

    int compr = 1; // во сколько раз сжали квадрат
    int freeAreaOfMainSquare;
    int sizeOfMainSquare;
    int bestCountOfSquares; // лучшее количество маленьких квадратов, покрывающих основной

    #ifdef OUTPUT
    std::cout << "Введите размер главного квадрата:" << std::endl;
    #endif
    std::cin >> sizeOfMainSquare;


    preworkWithMainSqr(compr, mainSquare, currArrayOfSmallSqrs, freeAreaOfMainSquare, sizeOfMainSquare, bestCountOfSquares);

    #ifdef OUTPUT
    clock_t start = clock();
    #endif

    doTaskRecursive(mainSquare, freeAreaOfMainSquare, 1, currArrayOfSmallSqrs.size(), currArrayOfSmallSqrs, 0, sizeOfMainSquare, bestCountOfSquares, bestArrayOfSmallSqrs);

    #ifdef OUTPUT
    clock_t end = clock();

    std::cout << "\nВремя выполнения: " << (double) (end - start) / CLOCKS_PER_SEC << "\n\n";
    #endif

    #ifdef COUNTOPERATIONS
    std::cout << "Количество операций = " << ::countOfOperations << '\n' << std::endl;
    #endif
    
    std::cout << bestCountOfSquares << std::endl;
    
    for (int i = 0; i < bestArrayOfSmallSqrs.size(); i++)
    {
        std::cout << bestArrayOfSmallSqrs[i].x * compr + 1 << " " << bestArrayOfSmallSqrs[i].y * compr + 1 << " " << bestArrayOfSmallSqrs[i].size * compr << std::endl;

        #ifdef OUTPUT
        addSmallSqrToMainSqr(mainSquare, bestArrayOfSmallSqrs[i].x * compr, bestArrayOfSmallSqrs[i].y * compr, bestArrayOfSmallSqrs[i].size * compr);
        #endif
    }

    #ifdef OUTPUT
    std::cout << std::endl;
    printMainSquare(mainSquare, compr, sizeOfMainSquare);
    #endif

    return 0;
}