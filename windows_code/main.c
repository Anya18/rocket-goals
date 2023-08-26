#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>

typedef enum
{
    false = 0,
    true = 1
} bool;

struct coord
{
    short int x;
    short int y;
    unsigned int neibour_count;
};

struct input_info
{
    int radius;
    struct coord *goals;
    unsigned int goals_count;
};


// Функция парсинга файла с координатами
void readfile(wchar_t *filePath, struct input_info *info)
{
    FILE *file;
    if ((file = _wfopen(filePath, L"r")) != NULL)
    {
        fseek(file, 0, SEEK_END);
        int fileLength = ftell(file);
        rewind(file);

        wchar_t *outString = (wchar_t *) malloc(sizeof(wchar_t) * fileLength);

        for (int i = 0; i < fileLength; i++)
            outString[i] = fgetc(file);
        info->goals = (struct coord *) malloc(sizeof(struct coord) * (fileLength / 4));

        wchar_t *state1, *state2;
        wchar_t *token = wcstok_s(outString, L"\n", &state1);
        info->goals_count = 0;
        while (token)
        {
            wchar_t *current_string = wcsdup(token);

            info->goals[info->goals_count].x = _wtoi(wcstok_s(current_string, L" ", &state2));
            info->goals[info->goals_count].y = _wtoi(wcstok_s(NULL, L" ", &state2));

            free(current_string);
            token = wcstok_s(NULL, L"\n", &state1);
            info->goals_count++;
        }

        info->goals_count--;
        fclose(file);
        return;
    }

    printf("ERROR: File (%s) cannot be opened.\n", filePath);
}


// функция вычисления расстояния между двумя точками и сравнения их с заданным радиусом
bool is_in_area(int x1, int y1, int x2, int y2, int r)
{
    float dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    if (dist <= r)
    {
        return true;
    } else
    {
        return false;
    }
}


// функция подсчёта количества соседей в зоне заданного радиуса для каждой цели (функция засчитывает саму точку за соседа)
// и нахождения цели с максимальным количеством соседей
struct coord count_goals_nearby(struct input_info *info)
{
    struct coord point;
    point.x = info->goals[0].x;
    point.y = info->goals[0].x;
    point.neibour_count = info->goals[0].neibour_count = 0;
    for (int i = 0; i < info->goals_count; i++)
    {
        info->goals[i].neibour_count = 0;
        for (int j = 0; j < info->goals_count; j++)
        {
            if (is_in_area(info->goals[i].x, info->goals[i].y, info->goals[j].x, info->goals[j].y, info->radius))
            {
                info->goals[i].neibour_count++;
            }
        }
        if (point.neibour_count < info->goals[i].neibour_count)
        {
            point.x = info->goals[i].x;
            point.y = info->goals[i].y;
            point.neibour_count = info->goals[i].neibour_count;
        }
    }
    return point;
}

const wchar_t *getWc(const char *c)
{
    const size_t cSize = strlen(c)+1;
    wchar_t* wc = malloc(sizeof (wchar_t) * cSize);
    mbstowcs(wc, c, cSize);

    return wc;
}


int main(int argc, char *argv[])
{
    setlocale(LC_CTYPE, ".1251");

    if (argc < 3)
    {
        return 0;
    }

    struct input_info info;

    const wchar_t *file_name = getWc(argv[1]);
    const wchar_t *num = getWc(argv[2]);

    swscanf(num, L"%d", &info.radius);

    readfile(file_name, &info);

    if (!info.goals_count)
    {
        return 0;
    }

    struct coord final_point = count_goals_nearby(&info);

    printf("Выбранные координаты: %d %d\nБудет поражено целей: %d \n", final_point.x, final_point.y, final_point.neibour_count);

    free(file_name);
    free(num);

    return 0;
}