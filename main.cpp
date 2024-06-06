#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_BORROWS 100
#define MAX_READERS 100
#define MAX_TITLE_LENGTH 50
#define MAX_AUTHOR_LENGTH 50
#define MAX_PUBLISHER_LENGTH 50
#define MAX_DATE_LENGTH 20
#define MAX_CATEGORY_LENGTH 20
#define MAX_NAME_LENGTH 50

typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    char publisher[MAX_PUBLISHER_LENGTH];
    char pub_date[MAX_DATE_LENGTH];
    double price;
    char category[MAX_CATEGORY_LENGTH];
    int total_quantity;
    int current_quantity;
    int borrowed_quantity;
} Book;

typedef struct {
    int id;
    int book_id;
    int reader_id;
    char borrow_date[MAX_DATE_LENGTH];
    char due_date[MAX_DATE_LENGTH];
    double fine;
    int renew_count; // 新字段用于记录续借次数
} Borrow;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char category[MAX_CATEGORY_LENGTH]; // 教师或学生
    int max_borrow;
} Reader;

Book books[MAX_BOOKS];
Borrow borrows[MAX_BORROWS];
Reader readers[MAX_READERS];
int book_count = 0;
int borrow_count = 0;
int reader_count = 0;

double fine_amount = 0.1; // 罚款金额，每天0.1元

void add_book();
void add_reader();
void borrow_book();
void return_book();
void renew_book();
void display_books();
void display_readers();
void display_borrow_rank();
void display_overdue_books();
void display_stats();
void set_fine_amount();

int main() {
    int choice;
    while (1) {
        printf("图书管理系统\n");
        printf("1. 添加图书\n");
        printf("2. 添加读者\n");
        printf("3. 借书\n");
        printf("4. 还书\n");
        printf("5. 续借\n");
        printf("6. 显示图书\n");
        printf("7. 显示读者\n");
        printf("8. 借阅排行榜\n");
        printf("9. 显示未归还图书\n");
        printf("10. 统计分析\n");
        printf("11. 设置罚款金额\n");
        printf("0. 退出\n");
        printf("请输入您的选择：");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            add_book();
            break;
        case 2:
            add_reader();
            break;
        case 3:
            borrow_book();
            break;
        case 4:
            return_book();
            break;
        case 5:
            renew_book();
            break;
        case 6:
            display_books();
            break;
        case 7:
            display_readers();
            break;
        case 8:
            display_borrow_rank();
            break;
        case 9:
            display_overdue_books();
            break;
        case 10:
            display_stats();
            break;
        case 11:
            set_fine_amount();
            break;
        case 0:
            exit(0);
        default:
            printf("无效选择。\n");
        }
    }
    return 0;
}

void add_book() {
    if (book_count >= MAX_BOOKS) {
        printf("没有更多空间存放图书。\n");
        return;
    }
    Book book;
    book.id = book_count + 1;
    printf("请输入书名：");
    fgets(book.title, sizeof(book.title), stdin);
    book.title[strcspn(book.title, "\n")] = '\0';  // 去掉换行符

    printf("请输入作者：");
    fgets(book.author, sizeof(book.author), stdin);
    book.author[strcspn(book.author, "\n")] = '\0';  // 去掉换行符

    printf("请输入出版社：");
    fgets(book.publisher, sizeof(book.publisher), stdin);
    book.publisher[strcspn(book.publisher, "\n")] = '\0';  // 去掉换行符

    printf("请输入出版日期 (YYYY-MM-DD)：");
    fgets(book.pub_date, sizeof(book.pub_date), stdin);
    book.pub_date[strcspn(book.pub_date, "\n")] = '\0';  // 去掉换行符

    printf("请输入价格：");
    scanf("%lf", &book.price);
    getchar();  // 捕获并丢弃多余的换行符

    printf("请输入类别：");
    fgets(book.category, sizeof(book.category), stdin);
    book.category[strcspn(book.category, "\n")] = '\0';  // 去掉换行符

    printf("请输入总数量：");
    scanf("%d", &book.total_quantity);
    book.current_quantity = book.total_quantity;
    book.borrowed_quantity = 0;

    books[book_count++] = book;
    printf("图书添加成功。\n");
}

void add_reader() {
    if (reader_count >= MAX_READERS) {
        printf("没有更多空间存放读者信息。\n");
        return;
    }
    Reader reader;
    reader.id = reader_count + 1;
    printf("请输入读者姓名：");
    fgets(reader.name, sizeof(reader.name), stdin);
    reader.name[strcspn(reader.name, "\n")] = '\0';  // 去掉换行符

    printf("请输入类别 (教师/学生)：");
    fgets(reader.category, sizeof(reader.category), stdin);
    reader.category[strcspn(reader.category, "\n")] = '\0';  // 去掉换行符

    if (strcmp(reader.category, "教师") == 0) {
        reader.max_borrow = 10;
    }
    else if (strcmp(reader.category, "学生") == 0) {
        reader.max_borrow = 5;
    }
    else {
        printf("无效类别。默认设置为学生。\n");
        strcpy(reader.category, "学生");
        reader.max_borrow = 5;
    }

    readers[reader_count++] = reader;
    printf("读者添加成功。\n");
}

void borrow_book() {
    if (borrow_count >= MAX_BORROWS) {
        printf("没有更多空间存放借阅信息。\n");
        return;
    }
    int book_id, reader_id;
    printf("请输入图书编号：");
    scanf("%d", &book_id);
    getchar();  // 捕获并丢弃多余的换行符

    printf("请输入读者编号：");
    scanf("%d", &reader_id);
    getchar();  // 捕获并丢弃多余的换行符

    if (book_id < 1 || book_id > book_count || reader_id < 1 || reader_id > reader_count) {
        printf("无效的图书或读者编号。\n");
        return;
    }

    Book* book = &books[book_id - 1];
    if (book->current_quantity <= 0) {
        printf("图书不可用。\n");
        return;
    }

    Borrow borrow;
    borrow.id = borrow_count + 1;
    borrow.book_id = book_id;
    borrow.reader_id = reader_id;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(borrow.borrow_date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    tm.tm_mday += 14; // 借期14天
    mktime(&tm);
    sprintf(borrow.due_date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    borrow.fine = 0.0;
    borrow.renew_count = 0; // 初始化续借次数

    book->current_quantity--;
    book->borrowed_quantity++;

    borrows[borrow_count++] = borrow;
    printf("图书借阅成功。\n");
}

void return_book() {
    int borrow_id;
    printf("请输入借阅编号：");
    scanf("%d", &borrow_id);
    getchar();  // 捕获并丢弃多余的换行符

    if (borrow_id < 1 || borrow_id > borrow_count) {
        printf("无效的借阅编号。\n");
        return;
    }

    Borrow* borrow = &borrows[borrow_id - 1];
    Book* book = &books[borrow->book_id - 1];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char current_date[MAX_DATE_LENGTH];
    sprintf(current_date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    if (strcmp(current_date, borrow->due_date) > 0) {
        struct tm due_tm;
        sscanf(borrow->due_date, "%d-%d-%d", &due_tm.tm_year, &due_tm.tm_mon, &due_tm.tm_mday);
        due_tm.tm_year -= 1900;
        due_tm.tm_mon -= 1;
        time_t due_time = mktime(&due_tm);
        borrow->fine = difftime(t, due_time) / (60 * 60 * 24) * fine_amount;
        printf("罚款金额：%.2f\n", borrow->fine);
    }

    book->current_quantity++;
    book->borrowed_quantity--;

    printf("图书归还成功。\n");
}

void renew_book() {
    int borrow_id;
    printf("请输入借阅编号：");
    scanf("%d", &borrow_id);
    getchar();  // 捕获并丢弃多余的换行符

    if (borrow_id < 1 || borrow_id > borrow_count) {
        printf("无效的借阅编号。\n");
        return;
    }

    Borrow* borrow = &borrows[borrow_id - 1];

    if (borrow->renew_count >= 2) { // 限制续借次数为2次
        printf("不能续借超过两次。\n");
        return;
    }

    struct tm due_tm;
    sscanf(borrow->due_date, "%d-%d-%d", &due_tm.tm_year, &due_tm.tm_mon, &due_tm.tm_mday);
    due_tm.tm_year -= 1900; // struct tm 年份从1900开始
    due_tm.tm_mon -= 1; // struct tm 月份从0开始

    due_tm.tm_mday += 14; // 续借14天
    mktime(&due_tm);
    sprintf(borrow->due_date, "%d-%02d-%02d", due_tm.tm_year + 1900, due_tm.tm_mon + 1, due_tm.tm_mday);

    borrow->renew_count++;

    printf("图书续借成功，新的归还日期：%s\n", borrow->due_date);
}

void display_books() {
    printf("图书列表：\n");
    printf("编号\t书名\t作者\t出版社\t出版日期\t价格\t类别\t总数量\t当前库存\t已借出\n");
    for (int i = 0; i < book_count; i++) {
        printf("%d\t%s\t%s\t%s\t%s\t%.2f\t%s\t%d\t%d\t%d\n", books[i].id, books[i].title, books[i].author, books[i].publisher,
            books[i].pub_date, books[i].price, books[i].category, books[i].total_quantity, books[i].current_quantity, books[i].borrowed_quantity);
    }
}

void display_readers() {
    printf("读者列表：\n");
    printf("编号\t姓名\t类别\t最大借阅数\n");
    for (int i = 0; i < reader_count; i++) {
        printf("%d\t%s\t%s\t%d\n", readers[i].id, readers[i].name, readers[i].category, readers[i].max_borrow);
    }
}

void display_borrow_rank() {
    // 基于借阅次数排序并显示
    Borrow temp;
    int i, j;
    for (i = 0; i < borrow_count - 1; i++) {
        for (j = 0; j < borrow_count - i - 1; j++) {
            if (borrows[j].renew_count < borrows[j + 1].renew_count) {
                temp = borrows[j];
                borrows[j] = borrows[j + 1];
                borrows[j + 1] = temp;
            }
        }
    }
    printf("借阅排行榜:\n");
    printf("借阅次数\t书名\t借书证号\t读者姓名\n");
    for (i = 0; i < borrow_count; i++) {
        printf("%d\t%s\t%d\t%s\n", borrows[i].renew_count, books[borrows[i].book_id - 1].title, borrows[i].reader_id, readers[borrows[i].reader_id - 1].name);
    }
}

void display_overdue_books() {
    printf("逾期未还图书列表：\n");
    printf("编号\t书名\t借书证号\t借阅日期\t应归还日期\t罚款金额\n");
    for (int i = 0; i < borrow_count; i++) {
        struct tm due_tm;
        sscanf(borrows[i].due_date, "%d-%d-%d", &due_tm.tm_year, &due_tm.tm_mon, &due_tm.tm_mday);
        due_tm.tm_year -= 1900;
        due_tm.tm_mon -= 1;
        time_t due_time = mktime(&due_tm);

        time_t now = time(NULL);
        double diff_days = difftime(now, due_time) / (60 * 60 * 24);

        if (diff_days > 0) {
            printf("%d\t%s\t%d\t%s\t%s\t%.2f\n", borrows[i].id, books[borrows[i].book_id - 1].title, borrows[i].reader_id,
                borrows[i].borrow_date, borrows[i].due_date, borrows[i].fine);
        }
    }
}

void display_stats() {
    int total_books = 0;
    int total_borrowed = 0;
    int total_overdue = 0;

    for (int i = 0; i < book_count; i++) {
        total_books += books[i].total_quantity;
        total_borrowed += books[i].borrowed_quantity;
    }

    for (int i = 0; i < borrow_count; i++) {
        struct tm due_tm;
        sscanf(borrows[i].due_date, "%d-%d-%d", &due_tm.tm_year, &due_tm.tm_mon, &due_tm.tm_mday);
        due_tm.tm_year -= 1900;
        due_tm.tm_mon -= 1;
        time_t due_time = mktime(&due_tm);

        time_t now = time(NULL);
        double diff_days = difftime(now, due_time) / (60 * 60 * 24);

        if (diff_days > 0) {
            total_overdue++;
        }
    }

    printf("总图书数目：%d\n", total_books);
    printf("总借出数目：%d\n", total_borrowed);
    printf("总逾期数目：%d\n", total_overdue);
}

void set_fine_amount() {
    printf("当前罚款金额为：%.2f元/天\n", fine_amount);
    printf("请输入新的罚款金额：");
    scanf("%lf", &fine_amount);
    printf("罚款金额设置成功。\n");
}
