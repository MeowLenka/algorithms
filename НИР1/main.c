#include <ncurses.h>
#include <panel.h>
#include <form.h>
#include <ctype.h>
#include "price.h"

#define MAX_FIELDS 4
#define MAX_ITEMS 100

PRICE *items = NULL;
unsigned int item_count = 0;
DB_HEADER header;
char filename[256] = "database.bin";

void input_data_ncurses()
{
    FIELD *fields[MAX_FIELDS + 1];
    FORM *form;
    int ch;
    int rows, cols;
    WINDOW *win;
    char *field_labels[] = {
        "Категория: ",
        "Название: ",
        "Цена: ",
        "Количество: "};
    char field_values[4][256] = {""};

    // инициализация ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    getmaxyx(stdscr, rows, cols);

    // создание полей, формы, окна
    for (int i = 0; i < MAX_FIELDS; i++)
    {
        fields[i] = new_field(1, 30, i + 1, 20, 0, 0);
        set_field_back(fields[i], A_UNDERLINE);
        field_opts_off(fields[i], O_AUTOSKIP);
    }
    fields[MAX_FIELDS] = NULL;
    form = new_form(fields);

    win = newwin(10, 60, (rows - 10) / 2, (cols - 60) / 2);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " ВВОД ДАННЫХ О ТОВАРЕ ");

    // отображение меток
    for (int i = 0; i < MAX_FIELDS; i++)
    {
        mvwprintw(win, i + 1, 2, "%s", field_labels[i]);
    }

    // свзяать формы с окном
    set_form_win(form, win);
    set_form_sub(form, derwin(win, 6, 30, 1, 20));

    post_form(form);
    wrefresh(win);

    // ввод данных
    int field_index = 0;
    while (field_index < MAX_FIELDS)
    {
        form_driver(form, REQ_END_LINE);
        ch = wgetch(win);

        switch (ch)
        {
        case KEY_UP:
        case KEY_LEFT:
            form_driver(form, REQ_PREV_FIELD);
            break;
        case KEY_DOWN:
        case KEY_RIGHT:
            form_driver(form, REQ_NEXT_FIELD);
            break;
        case 10: // Enter
            form_driver(form, REQ_NEXT_FIELD);
            field_index++;
            break;
        case 9: // Tab
            form_driver(form, REQ_NEXT_FIELD);
            break;
        case 27: // ESC
            goto exit_input;
        default:
            form_driver(form, ch);
            break;
        }
        wrefresh(win);
    }

    for (int i = 0; i < MAX_FIELDS; i++)
    {
        field_buffer(fields[i], 0);
        strcpy(field_values[i], field_buffer(fields[i], 0));
    }

    items = (PRICE *)realloc(items, (item_count + 1) * sizeof(PRICE));
    items[item_count].category = strdup(field_values[0]);
    items[item_count].good = strdup(field_values[1]);
    items[item_count].price = atof(field_values[2]);
    items[item_count].num = atoi(field_values[3]);
    item_count++;

exit_input:
    unpost_form(form);
    free_form(form);
    for (int i = 0; i < MAX_FIELDS; i++)
    {
        free_field(fields[i]);
    }
    delwin(win);
    endwin();
}

// Функция поиска по категории
void search_by_category(const char *category)
{
    bool found = false;
    unsigned int max_num = 0;
    unsigned int count_found = 0;
    double total_price = 0.0;
    int indices[MAX_ITEMS] = {0};

    for (unsigned int i = 0; i < item_count; i++)
    {
        if (strcmp(items[i].category, category) == 0)
        {
            found = true;
            if (items[i].num > max_num)
            {
                max_num = items[i].num;
                count_found = 1;
                total_price = items[i].price;
                indices[0] = i;
            }
            else if (items[i].num == max_num)
            {
                indices[count_found++] = i;
                total_price += items[i].price;
            }
        }
    }

    // вывод результатов ncurses
    initscr();
    clear();

    if (!found)
    {
        mvprintw(5, 10, "Товаров в категории '%s' не найдено.", category);
    }
    else
    {
        mvprintw(2, 10, "Результаты поиска по категории: %s", category);
        mvprintw(3, 10, "========================================");
        mvprintw(5, 10, "Максимальное количество: %u", max_num);
        mvprintw(6, 10, "Количество товаров: %u", count_found);
        mvprintw(7, 10, "Средняя цена: %.2f руб.", total_price / count_found);

        mvprintw(9, 10, "Товары с максимальным количеством:");
        for (unsigned int i = 0; i < count_found; i++)
        {
            int idx = indices[i];
            mvprintw(11 + i, 15, "%s - %.2f руб.", items[idx].good, items[idx].price);
        }
    }

    mvprintw(20, 10, "Нажмите любую клавишу для продолжения...");
    getch();
    endwin();
}


int main()
{
    char *signature = "TASH";
    int choice;
    char category[256];
    char input_str[256];

    PRICE *loaded_data = NULL;
    header = load_database(filename, &loaded_data);

    if (loaded_data != NULL && header.st_count > 0)
    {
        items = loaded_data;
        item_count = header.st_count;
        printf("Загружено %u записей из базы данных.\n", item_count);
    }
    else
    {
        printf("Базы данных не существует.\n");
    }

    while (1)
    {
        printf("УПРАВЛЕНИЕ БАЗОЙ ДАННЫХ ТОВАРОВ\n");
        printf("----------------------------------------\n");
        printf("1. Добавить товар\n");
        printf("2. Поиск по категории\n");
        printf("3. Показать все товары\n");
        printf("4. Сохранить базу данных\n");
        printf("5. Информация о базе данных\n");
        printf("6. Выход\n");
        printf("----------------------------------------\n");
        printf("Выберите действие: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            input_data_ncurses();
            header.tr_num++;
            printf("\nТовар добавлен! Транзакция #%u\n", header.tr_num);
            break;

        case 2:
            printf("Введите категорию для поиска: ");
            fgets(category, sizeof(category), stdin);
            category[strcspn(category, "\n")] = '\0';
            search_by_category(category);
            header.tr_num++;
            break;

        case 3:
            if (item_count == 0)
            {
                printf("База данных пуста.\n");
            }
            else
            {
                printf("Все товары в базе данных:\n");
                printf("----------------------------\n");
                for (unsigned int i = 0; i < item_count; i++)
                {
                    print_price(&items[i], i + 1);
                }
                printf("Всего: %u товаров\n", item_count);
            }
            break;

        case 4:
            header.st_count = item_count;
            header = create_header(signature, header.tr_num, item_count, items);
            save_database(filename, &header, items, item_count);
            break;

        case 5:
            printf("БАЗА ДАННЫХ:\n");
            printf("Сигнатура: %.4s\n", header.signature);
            printf("Номер транзакции: %u\n", header.tr_num);
            printf("Количество записей: %u\n", header.st_count);
            printf("CRC-32: 0x%08X\n", header.crc32);
            printf("Размер файла: %s\n", filename);
            break;

        case 6:
            if (item_count > 0) // save database
            {
                header.st_count = item_count;
                header = create_header(signature, header.tr_num, item_count, items);
                save_database(filename, &header, items, item_count);
            }
            for (unsigned int i = 0; i < item_count; i++)
            {
                free_price(&items[i]);
            }
            if (items)
                free(items);
            return 0;

        default:
            printf("Некорректный выбор.\n");
            break;
        }
    }

    return 0;
}