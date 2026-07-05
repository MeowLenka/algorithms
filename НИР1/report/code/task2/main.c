#include <ncurses.h>
#include <ctype.h>
#include "price.h"

PRICE *items = NULL;
unsigned int item_count = 0;
DB_HEADER header;
char filename[256] = "databasetest.bin";

int do_main = 1;

int input_data_ncurses();
int search_category_ncurses();
void search_by_category(const char *category);
void show_all_products();
void show_database_info();
void save_with_message(char *signature);
void draw_main_menu(WINDOW *win, const char *status_msg);
int get_menu_choice(WINDOW *win);

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
        mvwprintw(win, 12, 4, "%s CRC-32: 0x%08X", status_msg, header.crc32);
    }
    mvwprintw(win, 14, 4, "Select action (1-6): ");
    wrefresh(win);
}

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
            valid_input = 0;
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
                return choice;
        }

        mvwprintw(win, 15, 4, "Invalid input! Please enter number 1-6.");
        wrefresh(win);
        napms(1000);
        mvwprintw(win, 15, 4, "                                        ");
        wrefresh(win);
    }
}

int input_data_ncurses()
{
    echo();
    curs_set(1); 

    int height = 14;
    int width = 55;
    int start_y = 2;
    int start_x = 10;

    WINDOW *win = newwin(height, width, start_y, start_x);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " ENTER PRODUCT DATA ");
    mvwprintw(win, 13, 2, "Press ESC to cancel");
    wrefresh(win);

    char category[256] = "";
    char good[256] = "";
    char price_str[256] = "";
    char num_str[256] = "";

    int valid = 0;
    int cancelled = 0;
    double price = 0;
    unsigned int num = 0;

    while (!valid && !cancelled)
    {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 0, 2, " ENTER PRODUCT DATA ");
        mvwprintw(win, 13, 2, "Press ESC to cancel");
        wrefresh(win);

        mvwprintw(win, 2, 2, "Category: ");
        wmove(win, 2, 14);
        wclrtoeol(win);
        wrefresh(win);

        int ch;
        int pos = 0;
        while (1)
        {
            ch = wgetch(win);
            if (ch == 27) // esc
            {
                cancelled = 1;
                break;
            }
            if (ch == 10) // enter
            {
                category[pos] = '\0';
                break;
            }
            if (ch == KEY_BACKSPACE || ch == 127) // backspace or delete
            {
                if (pos > 0)
                {
                    pos--;
                    category[pos] = '\0';
                    mvwprintw(win, 2, 14, "%-30s", category);
                    wmove(win, 2, 14 + pos);
                    wrefresh(win);
                }
                continue;
            }
            if (pos < 255 && ch >= 32 && ch <= 126) // разрешены только цифры и точка
            {
                category[pos++] = ch;
                category[pos] = '\0';
                mvwprintw(win, 2, 14, "%s", category);
                wrefresh(win);
            }
        }
        if (cancelled)
            break;

        mvwprintw(win, 4, 2, "Product name: ");
        wmove(win, 4, 16);
        wclrtoeol(win);
        wrefresh(win);

        pos = 0;
        while (1)
        {
            ch = wgetch(win);
            if (ch == 27)
            {
                cancelled = 1;
                break;
            }
            if (ch == 10)
            {
                good[pos] = '\0';
                break;
            }
            if (ch == KEY_BACKSPACE || ch == 127)
            {
                if (pos > 0)
                {
                    pos--;
                    good[pos] = '\0';
                    mvwprintw(win, 4, 16, "%-30s", good);
                    wmove(win, 4, 16 + pos);
                    wrefresh(win);
                }
                continue;
            }
            if (pos < 255 && ch >= 32 && ch <= 126)
            {
                good[pos++] = ch;
                good[pos] = '\0';
                mvwprintw(win, 4, 16, "%s", good);
                wrefresh(win);
            }
        }
        if (cancelled)
            break;

        mvwprintw(win, 6, 2, "Cost (rub): ");
        wmove(win, 6, 14);
        wclrtoeol(win);
        wrefresh(win);

        pos = 0;
        while (1)
        {
            ch = wgetch(win);
            if (ch == 27)
            {
                cancelled = 1;
                break;
            }
            if (ch == 10)
            {
                price_str[pos] = '\0';
                break;
            }
            if (ch == KEY_BACKSPACE || ch == 127)
            {
                if (pos > 0)
                {
                    pos--;
                    price_str[pos] = '\0';
                    mvwprintw(win, 6, 14, "%-30s", price_str);
                    wmove(win, 6, 14 + pos);
                    wrefresh(win);
                }
                continue;
            }
            if (pos < 255 && ((ch >= '0' && ch <= '9') || ch == '.'))
            {
                price_str[pos++] = ch;
                price_str[pos] = '\0';
                mvwprintw(win, 6, 14, "%s", price_str);
                wrefresh(win);
            }
        }
        if (cancelled)
            break;

        mvwprintw(win, 8, 2, "Amount: ");
        wmove(win, 8, 10);
        wclrtoeol(win);
        wrefresh(win);

        pos = 0;
        while (1)
        {
            ch = wgetch(win);
            if (ch == 27)
            {
                cancelled = 1;
                break;
            }
            if (ch == 10)
            {
                num_str[pos] = '\0';
                break;
            }
            if (ch == KEY_BACKSPACE || ch == 127)
            {
                if (pos > 0)
                {
                    pos--;
                    num_str[pos] = '\0';
                    mvwprintw(win, 8, 10, "%-30s", num_str);
                    wmove(win, 8, 10 + pos);
                    wrefresh(win);
                }
                continue;
            }
            if (pos < 255 && ch >= '0' && ch <= '9')
            {
                num_str[pos++] = ch;
                num_str[pos] = '\0';
                mvwprintw(win, 8, 10, "%s", num_str);
                wrefresh(win);
            }
        }
        if (cancelled)
            break;

        // валидация
        int valid_price = 1;
        int valid_num = 1;
        int has_dot = 0;

        if (strlen(category) == 0 || strlen(good) == 0)
        {
            valid = 0;
            mvwprintw(win, 11, 2, "ERROR: Category and Product name cannot be empty!");
            wrefresh(win);
            getch(); // ожидание нажатия клавиши
            continue;
        }

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

        if (strlen(num_str) == 0)
            valid_num = 0;
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

        if (valid_price && valid_num)
        {
            price = atof(price_str);
            num = atoi(num_str);

            if (price <= 0 || num == 0)
            {
                valid = 0;
                if (price <= 0)
                    mvwprintw(win, 11, 2, "ERROR: Price must be greater than 0!");

                else
                    mvwprintw(win, 11, 2, "ERROR: Amount must be greater than 0!");
                wrefresh(win);
                getch();
                continue;
            }
            valid = 1;
        }
        else
        {
            valid = 0;
            if (!valid_price)
                mvwprintw(win, 11, 2, "ERROR: Price must be a positive number (e.g. 99.50)");
            else if (!valid_num)
                mvwprintw(win, 11, 2, "ERROR: Amount must be a positive integer (e.g. 10)");
            wrefresh(win);
            getch();
        }
    }

    if (cancelled)
    {
        delwin(win);
        noecho();
        curs_set(0);
        return 0;
    }

    items = (PRICE *)realloc(items, (item_count + 1) * sizeof(PRICE));
    if (items)
    {
        items[item_count].category = strdup(category);
        items[item_count].good = strdup(good);
        items[item_count].price = price;
        items[item_count].num = num;
        item_count++;

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
        noecho();
        curs_set(0);
        return 1;
    }

    delwin(win);
    noecho();
    curs_set(0);
    return 0;
}

int search_category_ncurses()
{
    char category[256] = "";
    int rows, cols;
    WINDOW *win;
    int ch;
    int pos = 0;
    int cancelled = 0;

    curs_set(1);
    echo();

    getmaxyx(stdscr, rows, cols);

    win = newwin(7, 50, (rows - 7) / 2, (cols - 50) / 2);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " SEARCH BY CATEGORY ");
    mvwprintw(win, 2, 2, "Enter category: ");
    mvwprintw(win, 5, 2, "Press ESC to cancel");
    wrefresh(win);

    mvwprintw(win, 2, 18, "                                    ");
    wmove(win, 2, 18);
    wrefresh(win);

    while (1)
    {
        ch = wgetch(win);
        if (ch == 27)
        { // esc
            cancelled = 1;
            break;
        }
        if (ch == 10)
        { // enter
            category[pos] = '\0';
            break;
        }
        if (ch == KEY_BACKSPACE || ch == 127)
        {
            if (pos > 0)
            {
                pos--;
                category[pos] = '\0';
                mvwprintw(win, 2, 18, "%-30s", category);
                wmove(win, 2, 18 + pos);
                wrefresh(win);
            }
            continue;
        }
        if (pos < 255 && ch >= 32 && ch <= 126)
        {
            category[pos++] = ch;
            category[pos] = '\0';
            mvwprintw(win, 2, 18, "%s", category);
            wrefresh(win);
        }
    }

    delwin(win);
    noecho();
    curs_set(0);

    if (cancelled || strlen(category) == 0)
    {
        return 0; // отмена
    }

    search_by_category(category);
    return 1; // успешно
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
                indices[count_found++] = i;
                total_price += items[i].price;
            }
        }
    }

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
        mvprintw(6, 10, "Number of products with max num: %u", count_found);
        mvprintw(7, 10, "Average price: %.2f rub.", total_price / count_found);

        mvprintw(9, 10, "Products with max quantity:");
        for (unsigned int i = 0; i < count_found; i++)
        {
            int idx = indices[i];
            mvprintw(11 + i, 15, "%s - %.2f rub.", items[idx].good, items[idx].price);
        }
    }

    mvprintw(20, 10, "Press any key to continue...");
    refresh();
    getch();

}

void show_all_products()
{
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
}

void show_database_info()
{
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
}

void save_with_message(char *signature)
{
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

    clear();
    refresh();
    box(stdscr, 0, 0);
    mvprintw(3, 10, "Database saved successfully!");
    mvprintw(4, 10, "Records: %u", item_count);
    mvprintw(5, 10, "CRC-32: 0x%08X", header.crc32);
    mvprintw(7, 10, "Press any key...");
    refresh();
    getch();
}

int main()
{
    char *signature = "TASH";
    int choice;
    char status_msg[256] = "";
    WINDOW *main_win;
    int rows, cols;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    // загрузка базы данных
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

    while (do_main)
    {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);
        clear();
        refresh();

        getmaxyx(stdscr, rows, cols);

        main_win = newwin(17, 55, (rows - 17) / 2, (cols - 55) / 2);
        draw_main_menu(main_win, status_msg);

        curs_set(1);
        choice = get_menu_choice(main_win);

        delwin(main_win);
        endwin();

        switch (choice)
        {
        case 1:
        {
            int added = input_data_ncurses();
            if (added)
            {
                header.tr_num++;
                snprintf(status_msg, sizeof(status_msg),
                         "Product added. Transaction #%u", header.tr_num);
                printf("Product added. Transaction #%u, CRC-32: 0x%08X", header.tr_num, header.crc32);
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
                         "Database is empty.");
                break;
            }

            int search_result = search_category_ncurses();
            if (search_result == 1)
            {
                header.tr_num++;
                snprintf(status_msg, sizeof(status_msg),
                         "Search completed. Transaction #%u", header.tr_num);
                printf("Search completed. Transaction #%u, CRC-32: 0x%08X", header.tr_num, header.crc32);
            }
            else
            {
                snprintf(status_msg, sizeof(status_msg),
                         "Search cancelled.");
            }
            break;

        case 3:
            show_all_products();
            snprintf(status_msg, sizeof(status_msg),
                     "Displayed all %u products.", item_count);
            break;
            printf("Displayed all %u products, CRC-32: 0x%08X", item_count, header.crc32);

        case 4:
            save_with_message(signature);
            snprintf(status_msg, sizeof(status_msg),
                     "Database saved successfully.");
            printf("Database saved successfully, CRC-32: 0x%08X", header.crc32);
            break;

        case 5:
            show_database_info();
            snprintf(status_msg, sizeof(status_msg),
                     "Database info displayed.");
            printf("Database info displayed, CRC-32: 0x%08X", header.crc32);

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

            return 0;

        default:
            snprintf(status_msg, sizeof(status_msg),
                     "Invalid choice. Please select 1-6.");
            break;
        }
    }

    endwin();
    return 0;
}
