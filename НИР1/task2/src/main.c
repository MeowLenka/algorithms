// main.c
#include <ncurses.h>
#include <ctype.h>
#include "price.h"

PRICE *items = NULL;
unsigned int item_count = 0;
DB_HEADER header;
char filename[256] = "database.bin";

// Прототипы функций
int input_data_ncurses();
void search_by_category(const char *category);
void show_all_products();
void show_database_info();
void save_with_message();
void draw_main_menu(WINDOW *win, const char *status_msg);
int get_menu_choice(WINDOW *win);

// Функция отрисовки главного меню
void draw_main_menu(WINDOW *win, const char *status_msg)
{
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " PRODUCT DATABASE MANAGEMENT ");

    mvwprintw(win, 2, 4, "1. Add new product");
    mvwprintw(win, 3, 4, "2. Search by category");
    mvwprintw(win, 4, 4, "3. Show all products");
    mvwprintw(win, 5, 4, "4. Save database");
    mvwprintw(win, 6, 4, "5. Database info");
    mvwprintw(win, 7, 4, "6. Exit");

    mvwprintw(win, 9, 4, "Records in memory: %u", item_count);
    mvwprintw(win, 10, 4, "Transaction #: %u", header.tr_num);

    if (status_msg && strlen(status_msg) > 0)
    {
        mvwprintw(win, 12, 4, "%s", status_msg);
    }

    mvwprintw(win, 14, 4, "Select action (1-6): ");
    wrefresh(win);
}

// Функция получения выбора с валидацией
int get_menu_choice(WINDOW *win)
{
    char input[256];
    int choice;

    while (1)
    {
        mvwprintw(win, 14, 24, "                ");
        wmove(win, 14, 24);
        wrefresh(win);

        echo();
        wgetnstr(win, input, sizeof(input) - 1);
        noecho();

        input[strcspn(input, "\n")] = '\0';

        int valid_input = 1;
        if (input[0] == '\0')
        {
            valid_input = 0;
        }
        else
        {
            for (int i = 0; input[i] != '\0'; i++)
            {
                if (!isdigit(input[i]))
                {
                    valid_input = 0;
                    break;
                }
            }
        }

        if (valid_input)
        {
            choice = atoi(input);
            if (choice >= 1 && choice <= 6)
            {
                return choice;
            }
        }

        mvwprintw(win, 15, 4, "Invalid input! Please enter number 1-6.");
        wrefresh(win);
        napms(1500);
        mvwprintw(win, 15, 4, "                                        ");
        wrefresh(win);
    }
}

int input_data_ncurses()
{
    initscr();
    cbreak();
    echo();
    keypad(stdscr, TRUE);
    clear();
    refresh();

    int height = 14;
    int width = 55;
    int start_y = 2;
    int start_x = 10;

    WINDOW *win = newwin(height, width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " ENTER PRODUCT DATA ");
    wrefresh(win);

    char category[256] = "";
    char good[256] = "";
    char price_str[256] = "";
    char num_str[256] = "";

    int valid = 0;
    double price = 0;
    unsigned int num = 0;

    while (!valid)
    {
        // Очищаем поля для ввода
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, " ENTER PRODUCT DATA ");
        wrefresh(win);

        mvwprintw(win, 2, 2, "Category: ");
        wgetstr(win, category);

        mvwprintw(win, 4, 2, "Product name: ");
        wgetstr(win, good);

        mvwprintw(win, 6, 2, "Cost (rub): ");
        wgetstr(win, price_str);

        mvwprintw(win, 8, 2, "Amount: ");
        wgetstr(win, num_str);

        // Валидация данных
        int valid_price = 1;
        int valid_num = 1;
        int has_dot = 0;

        // Проверка категории и названия (не пустые)
        if (strlen(category) == 0 || strlen(good) == 0)
        {
            valid = 0;
            // Показываем сообщение об ошибке
            mvwprintw(win, 11, 2, "ERROR: Category and Product name cannot be empty!");
            wrefresh(win);
            getch();
            continue;
        }

        // Проверка цены
        if (strlen(price_str) == 0)
        {
            valid_price = 0;
        }
        else
        {
            for (int i = 0; price_str[i] != '\0'; i++)
            {
                if (price_str[i] == '.')
                {
                    if (has_dot)
                    {
                        valid_price = 0;
                        break;
                    }
                    has_dot = 1;
                }
                else if (!isdigit(price_str[i]))
                {
                    valid_price = 0;
                    break;
                }
            }
        }

        // Проверка количества
        if (strlen(num_str) == 0)
        {
            valid_num = 0;
        }
        else
        {
            for (int i = 0; num_str[i] != '\0'; i++)
            {
                if (!isdigit(num_str[i]))
                {
                    valid_num = 0;
                    break;
                }
            }
        }

        // Преобразование
        if (valid_price && valid_num)
        {
            price = atof(price_str);
            num = atoi(num_str);

            if (price <= 0 || num == 0)
            {
                valid = 0;
                // Показываем сообщение об ошибке
                if (price <= 0)
                {
                    mvwprintw(win, 11, 2, "ERROR: Price must be greater than 0!");
                }
                else
                {
                    mvwprintw(win, 11, 2, "ERROR: Amount must be greater than 0!");
                }
                wrefresh(win);
                getch();
                continue;
            }
            valid = 1;
        }
        else
        {
            valid = 0;
            // Показываем сообщение об ошибке
            if (!valid_price)
            {
                mvwprintw(win, 11, 2, "ERROR: Price must be a positive number (e.g. 99.50)");
            }
            else if (!valid_num)
            {
                mvwprintw(win, 11, 2, "ERROR: Amount must be a positive integer (e.g. 10)");
            }
            wrefresh(win);
            getch();
        }
    }

    // Добавление товара
    items = (PRICE *)realloc(items, (item_count + 1) * sizeof(PRICE));
    if (items)
    {
        items[item_count].category = strdup(category);
        items[item_count].good = strdup(good);
        items[item_count].price = price;
        items[item_count].num = num;
        item_count++;

        // Очищаем окно и показываем сообщение об успехе
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, " ENTER PRODUCT DATA ");
        mvwprintw(win, 5, 2, "Product added successfully!");
        mvwprintw(win, 7, 2, "Category: %s", category);
        mvwprintw(win, 8, 2, "Name: %s", good);
        mvwprintw(win, 9, 2, "Price: %.2f rub.", price);
        mvwprintw(win, 10, 2, "Amount: %u", num);
        mvwprintw(win, 12, 2, "Press any key to continue...");
        wrefresh(win);
        getch();

        delwin(win);
        endwin();
        return 1;
    }

    delwin(win);
    endwin();
    return 0;
}

void search_by_category(const char *category)
{
    bool found = false;
    unsigned int max_num = 0;
    unsigned int count_found = 0;
    double total_price = 0.0;
    int indices[100] = {0};

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
                indices[count_found] = i;
                total_price += items[i].price;
                count_found++;
            }
        }
    }

    initscr();
    clear();
    refresh();
    box(stdscr, 0, 0);

    if (!found)
    {
        mvprintw(5, 10, "No products found in category: %s", category);
    }
    else
    {
        mvprintw(2, 10, "SEARCH RESULTS ");
        mvprintw(3, 10, "Category: %s", category);
        mvprintw(5, 10, "Maximum quantity: %u", max_num);
        mvprintw(6, 10, "Number of products: %u", count_found);
        mvprintw(7, 10, "Average price: %.2f rub.", total_price / count_found);

        mvprintw(9, 10, "Products with maximum quantity:");
        for (unsigned int i = 0; i < count_found; i++)
        {
            int idx = indices[i];
            mvprintw(11 + i, 15, "%s - %.2f rub.", items[idx].good, items[idx].price);
        }
    }

    mvprintw(20, 10, "Press any key to continue...");
    refresh();
    getch();
    endwin();
}

void show_all_products()
{
    initscr();
    clear();
    refresh();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    if (item_count == 0)
    {
        mvprintw(rows / 2, (cols - 20) / 2, "Database is empty!");
        getch();
        endwin();
        return;
    }

    box(stdscr, 0, 0);
    mvprintw(0, 2, " ALL PRODUCTS ");

    int line = 2;
    for (unsigned int i = 0; i < item_count && line < rows - 2; i++)
    {
        mvprintw(line++, 2, "[%u] Category: %s", i + 1, items[i].category);
        mvprintw(line++, 4, "Name: %s", items[i].good);
        mvprintw(line++, 4, "Price: %.2f rub.", items[i].price);
        mvprintw(line++, 4, "Amount: %u", items[i].num);
        line++;
    }

    mvprintw(rows - 2, 2, "Total: %u products. Press any key...", item_count);
    refresh();
    getch();
    endwin();
}

void show_database_info()
{
    initscr();
    clear();
    refresh();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    box(stdscr, 0, 0);
    mvprintw(0, 2, " DATABASE INFO ");

    mvprintw(2, 4, "Signature: %.4s", header.signature);
    mvprintw(3, 4, "Transaction: %u", header.tr_num);
    mvprintw(4, 4, "Records in file: %u", header.st_count);
    mvprintw(5, 4, "Records in memory: %u", item_count);
    mvprintw(6, 4, "CRC-32: 0x%08X", header.crc32);
    mvprintw(7, 4, "File: %s", filename);

    mvprintw(rows - 2, 4, "Press any key to continue...");
    refresh();
    getch();
    endwin();
}

void save_with_message()
{
    char *signature = "TASH";

    if (item_count == 0)
    {
        initscr();
        clear();
        refresh();
        box(stdscr, 0, 0);
        mvprintw(5, 10, "No data to save!");
        mvprintw(7, 10, "Press any key...");
        refresh();
        getch();
        endwin();
        return;
    }

    header.st_count = item_count;
    header = create_header(signature, header.tr_num, item_count, items);
    save_database(filename, &header, items, item_count);

    initscr();
    clear();
    refresh();
    box(stdscr, 0, 0);
    mvprintw(3, 10, "Database saved successfully!");
    mvprintw(4, 10, "Records: %u", item_count);
    mvprintw(5, 10, "CRC-32: 0x%08X", header.crc32);
    mvprintw(7, 10, "Press any key...");
    refresh();
    getch();
    endwin();
}

int main()
{
    char *signature = "TASH";
    int choice;
    char category[256];
    char status_msg[256] = "";
    WINDOW *main_win;
    int rows, cols;

    // Загрузка базы данных
    PRICE *loaded_data = NULL;
    header = load_database(filename, &loaded_data);

    if (loaded_data != NULL && header.st_count > 0)
    {
        items = loaded_data;
        item_count = header.st_count;
        snprintf(status_msg, sizeof(status_msg), "Loaded %u records.", item_count);
    }
    else
    {
        memset(&header, 0, sizeof(DB_HEADER));
        strncpy(header.signature, signature, 4);
        header.tr_num = 0;
        header.st_count = 0;
        header.crc32 = 0;
        snprintf(status_msg, sizeof(status_msg), "No database found. Creating new.");
    }

    // Основной цикл
    while (1)
    {
        // Инициализация ncurses для главного меню
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);
        clear(); // Очищаем весь экран перед отрисовкой главного меню
        refresh();

        getmaxyx(stdscr, rows, cols);

        main_win = newwin(17, 55, (rows - 17) / 2, (cols - 55) / 2);
        draw_main_menu(main_win, status_msg);

        curs_set(1);
        choice = get_menu_choice(main_win);

        delwin(main_win);
        endwin();

        // Обработка выбора
        switch (choice)
        {
        case 1:
        {
            int added = input_data_ncurses();
            if (added)
            {
                header.tr_num++;
                snprintf(status_msg, sizeof(status_msg),
                         "Product added! Transaction #%u", header.tr_num);
            }
            else
            {
                snprintf(status_msg, sizeof(status_msg),
                         "Product addition cancelled.");
            }
            break;
        }

        case 2:
            if (item_count == 0)
            {
                snprintf(status_msg, sizeof(status_msg),
                         "Database is empty! Add products first.");
                break;
            }
            printf("Enter category to search: ");
            fgets(category, sizeof(category), stdin);
            category[strcspn(category, "\n")] = '\0';
            search_by_category(category);
            header.tr_num++;
            snprintf(status_msg, sizeof(status_msg),
                     "Search completed. Transaction #%u", header.tr_num);
            break;

        case 3:
            show_all_products();
            snprintf(status_msg, sizeof(status_msg),
                     "Displayed all %u products.", item_count);
            break;

        case 4:
            save_with_message();
            snprintf(status_msg, sizeof(status_msg),
                     "Database saved successfully!");
            break;

        case 5:
            show_database_info();
            snprintf(status_msg, sizeof(status_msg),
                     "Database info displayed.");
            break;

        case 6:
            if (item_count > 0)
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

            printf("\nGoodbye!\n");
            return 0;

        default:
            snprintf(status_msg, sizeof(status_msg),
                     "Invalid choice! Please select 1-6.");
            break;
        }
    }

    return 0;
}
