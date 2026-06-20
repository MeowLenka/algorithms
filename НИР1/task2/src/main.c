#include <ncurses.h>
#include <ctype.h>
#include "price.h"

PRICE *items = NULL;
unsigned int item_count = 0;
DB_HEADER header;
char filename[256] = "database.bin";

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

    while (!valid) {
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
        if (strlen(category) == 0 || strlen(good) == 0) {
            valid = 0;
            // Показываем сообщение об ошибке
            mvwprintw(win, 11, 2, "ERROR: Category and Product name cannot be empty!");
            wrefresh(win);
            getch();
            continue;
        }

        // Проверка цены
        if (strlen(price_str) == 0) {
            valid_price = 0;
        } else {
            for (int i = 0; price_str[i] != '\0'; i++) {
                if (price_str[i] == '.') {
                    if (has_dot) {
                        valid_price = 0;
                        break;
                    }
                    has_dot = 1;
                } else if (!isdigit(price_str[i])) {
                    valid_price = 0;
                    break;
                }
            }
        }

        // Проверка количества
        if (strlen(num_str) == 0) {
            valid_num = 0;
        } else {
            for (int i = 0; num_str[i] != '\0'; i++) {
                if (!isdigit(num_str[i])) {
                    valid_num = 0;
                    break;
                }
            }
        }

        // Преобразование
        if (valid_price && valid_num) {
            price = atof(price_str);
            num = atoi(num_str);
            
            if (price <= 0 || num == 0) {
                valid = 0;
                // Показываем сообщение об ошибке
                if (price <= 0) {
                    mvwprintw(win, 11, 2, "ERROR: Price must be greater than 0!");
                } else {
                    mvwprintw(win, 11, 2, "ERROR: Amount must be greater than 0!");
                }
                wrefresh(win);
                getch();
                continue;
            }
            valid = 1;
        } else {
            valid = 0;
            // Показываем сообщение об ошибке
            if (!valid_price) {
                mvwprintw(win, 11, 2, "ERROR: Price must be a positive number (e.g. 99.50)");
            } else if (!valid_num) {
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
        mvprintw(rows/2, (cols - 20)/2, "Database is empty!");
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
        line++; // пустая строка между записями
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
    char input[256];

    PRICE *loaded_data = NULL;
    header = load_database(filename, &loaded_data);

    if (loaded_data != NULL && header.st_count > 0)
    {
        items = loaded_data;
        item_count = header.st_count;
        printf("Loaded %u records.\n", item_count);
    }
    else
    {
        printf("No database found. Creating new database.\n");
        memset(&header, 0, sizeof(DB_HEADER));
        strncpy(header.signature, signature, 4);
        header.tr_num = 0;
        header.st_count = 0;
        header.crc32 = 0;
    }

    while (1)
    {
        printf("\n========================================\n");
        printf("   PRODUCT DATABASE MANAGEMENT\n");
        printf("========================================\n");
        printf("1. Add new product\n");
        printf("2. Search by category\n");
        printf("3. Show all products\n");
        printf("4. Save database\n");
        printf("5. Database info\n");
        printf("6. Exit\n");
        printf("----------------------------------------\n");
        printf("Records: %u\n", item_count);
        printf("Select action (1-6): ");
        
        // Ввод с проверкой
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        
        // Проверка, что введено число
        int valid_input = 1;
        for (int i = 0; input[i] != '\0'; i++)
        {
            if (!isdigit(input[i]))
            {
                valid_input = 0;
                break;
            }
        }
        
        if (!valid_input || input[0] == '\0')
        {
            printf("Invalid input! Please enter a number 1-6.\n");
            continue;
        }
        
        choice = atoi(input);
        
        if (choice < 1 || choice > 6)
        {
            printf("Invalid choice! Please enter 1-6.\n");
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            int added = input_data_ncurses();
            if (added)
            {
                header.tr_num++;
                printf("\nProduct added! Transaction #%u\n", header.tr_num);
            }
            break;
        }

        case 2:
            if (item_count == 0)
            {
                printf("\nDatabase is empty.\n");
                break;
            }
            printf("Enter category to search: ");
            fgets(category, sizeof(category), stdin);
            category[strcspn(category, "\n")] = '\0';
            search_by_category(category);
            header.tr_num++;
            break;

        case 3:
            show_all_products();
            break;

        case 4:
            save_with_message();
            break;

        case 5:
            show_database_info();
            break;

        case 6:
            if (item_count > 0)
            {
                printf("\nSaving database...\n");
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
            printf("\nInvalid choice.\n");
            break;
        }
    }

    return 0;
}