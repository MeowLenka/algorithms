#include <ncurses.h>
#include <form.h>
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

    int height = 12;
    int width = 50;
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

    mvwprintw(win, 2, 2, "Category: ");
    wgetstr(win, category);

    mvwprintw(win, 4, 2, "Product name: ");
    wgetstr(win, good);

    mvwprintw(win, 6, 2, "Cost (rub): ");
    wgetstr(win, price_str);

    mvwprintw(win, 8, 2, "Amount: ");
    wgetstr(win, num_str);

    double price = atof(price_str);
    unsigned int num = atoi(num_str);


    items = (PRICE *)realloc(items, (item_count + 1) * sizeof(PRICE));
    if (items)
    {
        items[item_count].category = strdup(category);
        items[item_count].good = strdup(good);
        items[item_count].price = price;
        items[item_count].num = num;
        item_count++;

        mvwprintw(win, 10, 2, "Product added successfully!");
        wrefresh(win);
        getch();
    }

    delwin(win);
    endwin();

    return items ? 1 : 0;
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


int main()
{
    char *signature = "TASH";
    int choice;
    char category[256];

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
        // инициализация загловка
        memset(&header, 0, sizeof(DB_HEADER));
        strncpy(header.signature, signature, 4);
        header.tr_num = 0;
        header.st_count = 0;
        header.crc32 = 0;
    }

    while (1)
    {
        printf("PRODUCT DATABASE MANAGEMENT:\n");
        printf("1. Add new product\n");
        printf("2. Search by category\n");
        printf("3. Show all products\n");
        printf("4. Save database\n");
        printf("5. Database info\n");
        printf("6. Exit\n");
        printf("Select action: ");

        scanf("%d", &choice);
        getchar();

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
            else
            {
                printf("\nProduct addition cancelled.\n");
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
            if (item_count == 0)
            {
                printf("\nDatabase is empty.\n");
            }
            else
            {
                printf("\nALL PRODUCTS : \n");
                for (unsigned int i = 0; i < item_count; i++)
                {
                    print_price(&items[i], i + 1);
                }
                printf("Total: %u products\n", item_count);
            }
            break;

        case 4:
            if (item_count == 0)
            {
                printf("\nNo data to save.\n");
                break;
            }
            header.st_count = item_count;
            header = create_header(signature, header.tr_num, item_count, items);
            save_database(filename, &header, items, item_count);
            break;

        case 5:
            printf("\nDATABASE INFO : \n");
            printf("Signature: %.4s\n", header.signature);
            printf("Transaction: %u\n", header.tr_num);
            printf("Records: %u\n", header.st_count);
            printf("CRC-32: 0x%08X\n", header.crc32);
            printf("File: %s\n", filename);
            break;

        case 6:
            if (item_count > 0)
            {
                printf("\nSaving database.\n");
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
            printf("\nInvalid number.\n");
            break;
        }
    }

    return 0;
}
