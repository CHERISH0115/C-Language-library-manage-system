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
    int renew_count; // ���ֶ����ڼ�¼�������
} Borrow;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char category[MAX_CATEGORY_LENGTH]; // ��ʦ��ѧ��
    int max_borrow;
} Reader;

Book books[MAX_BOOKS];
Borrow borrows[MAX_BORROWS];
Reader readers[MAX_READERS];
int book_count = 0;
int borrow_count = 0;
int reader_count = 0;

double fine_amount = 0.1; // �����ÿ��0.1Ԫ

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
        printf("ͼ�����ϵͳ\n");
        printf("1. ���ͼ��\n");
        printf("2. ��Ӷ���\n");
        printf("3. ����\n");
        printf("4. ����\n");
        printf("5. ����\n");
        printf("6. ��ʾͼ��\n");
        printf("7. ��ʾ����\n");
        printf("8. �������а�\n");
        printf("9. ��ʾδ�黹ͼ��\n");
        printf("10. ͳ�Ʒ���\n");
        printf("11. ���÷�����\n");
        printf("0. �˳�\n");
        printf("����������ѡ��");
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
            printf("��Чѡ��\n");
        }
    }
    return 0;
}

void add_book() {
    if (book_count >= MAX_BOOKS) {
        printf("û�и���ռ���ͼ�顣\n");
        return;
    }
    Book book;
    book.id = book_count + 1;
    printf("������������");
    fgets(book.title, sizeof(book.title), stdin);
    book.title[strcspn(book.title, "\n")] = '\0';  // ȥ�����з�

    printf("���������ߣ�");
    fgets(book.author, sizeof(book.author), stdin);
    book.author[strcspn(book.author, "\n")] = '\0';  // ȥ�����з�

    printf("����������磺");
    fgets(book.publisher, sizeof(book.publisher), stdin);
    book.publisher[strcspn(book.publisher, "\n")] = '\0';  // ȥ�����з�

    printf("������������� (YYYY-MM-DD)��");
    fgets(book.pub_date, sizeof(book.pub_date), stdin);
    book.pub_date[strcspn(book.pub_date, "\n")] = '\0';  // ȥ�����з�

    printf("������۸�");
    scanf("%lf", &book.price);
    getchar();  // ���񲢶�������Ļ��з�

    printf("���������");
    fgets(book.category, sizeof(book.category), stdin);
    book.category[strcspn(book.category, "\n")] = '\0';  // ȥ�����з�

    printf("��������������");
    scanf("%d", &book.total_quantity);
    book.current_quantity = book.total_quantity;
    book.borrowed_quantity = 0;

    books[book_count++] = book;
    printf("ͼ����ӳɹ���\n");
}

void add_reader() {
    if (reader_count >= MAX_READERS) {
        printf("û�и���ռ��Ŷ�����Ϣ��\n");
        return;
    }
    Reader reader;
    reader.id = reader_count + 1;
    printf("���������������");
    fgets(reader.name, sizeof(reader.name), stdin);
    reader.name[strcspn(reader.name, "\n")] = '\0';  // ȥ�����з�

    printf("��������� (��ʦ/ѧ��)��");
    fgets(reader.category, sizeof(reader.category), stdin);
    reader.category[strcspn(reader.category, "\n")] = '\0';  // ȥ�����з�

    if (strcmp(reader.category, "��ʦ") == 0) {
        reader.max_borrow = 10;
    }
    else if (strcmp(reader.category, "ѧ��") == 0) {
        reader.max_borrow = 5;
    }
    else {
        printf("��Ч���Ĭ������Ϊѧ����\n");
        strcpy(reader.category, "ѧ��");
        reader.max_borrow = 5;
    }

    readers[reader_count++] = reader;
    printf("������ӳɹ���\n");
}

void borrow_book() {
    if (borrow_count >= MAX_BORROWS) {
        printf("û�и���ռ��Ž�����Ϣ��\n");
        return;
    }
    int book_id, reader_id;
    printf("������ͼ���ţ�");
    scanf("%d", &book_id);
    getchar();  // ���񲢶�������Ļ��з�

    printf("��������߱�ţ�");
    scanf("%d", &reader_id);
    getchar();  // ���񲢶�������Ļ��з�

    if (book_id < 1 || book_id > book_count || reader_id < 1 || reader_id > reader_count) {
        printf("��Ч��ͼ�����߱�š�\n");
        return;
    }

    Book* book = &books[book_id - 1];
    if (book->current_quantity <= 0) {
        printf("ͼ�鲻���á�\n");
        return;
    }

    Borrow borrow;
    borrow.id = borrow_count + 1;
    borrow.book_id = book_id;
    borrow.reader_id = reader_id;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(borrow.borrow_date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    tm.tm_mday += 14; // ����14��
    mktime(&tm);
    sprintf(borrow.due_date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    borrow.fine = 0.0;
    borrow.renew_count = 0; // ��ʼ���������

    book->current_quantity--;
    book->borrowed_quantity++;

    borrows[borrow_count++] = borrow;
    printf("ͼ����ĳɹ���\n");
}

void return_book() {
    int borrow_id;
    printf("��������ı�ţ�");
    scanf("%d", &borrow_id);
    getchar();  // ���񲢶�������Ļ��з�

    if (borrow_id < 1 || borrow_id > borrow_count) {
        printf("��Ч�Ľ��ı�š�\n");
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
        printf("�����%.2f\n", borrow->fine);
    }

    book->current_quantity++;
    book->borrowed_quantity--;

    printf("ͼ��黹�ɹ���\n");
}

void renew_book() {
    int borrow_id;
    printf("��������ı�ţ�");
    scanf("%d", &borrow_id);
    getchar();  // ���񲢶�������Ļ��з�

    if (borrow_id < 1 || borrow_id > borrow_count) {
        printf("��Ч�Ľ��ı�š�\n");
        return;
    }

    Borrow* borrow = &borrows[borrow_id - 1];

    if (borrow->renew_count >= 2) { // �����������Ϊ2��
        printf("�������賬�����Ρ�\n");
        return;
    }

    struct tm due_tm;
    sscanf(borrow->due_date, "%d-%d-%d", &due_tm.tm_year, &due_tm.tm_mon, &due_tm.tm_mday);
    due_tm.tm_year -= 1900; // struct tm ��ݴ�1900��ʼ
    due_tm.tm_mon -= 1; // struct tm �·ݴ�0��ʼ

    due_tm.tm_mday += 14; // ����14��
    mktime(&due_tm);
    sprintf(borrow->due_date, "%d-%02d-%02d", due_tm.tm_year + 1900, due_tm.tm_mon + 1, due_tm.tm_mday);

    borrow->renew_count++;

    printf("ͼ������ɹ����µĹ黹���ڣ�%s\n", borrow->due_date);
}

void display_books() {
    printf("ͼ���б�\n");
    printf("���\t����\t����\t������\t��������\t�۸�\t���\t������\t��ǰ���\t�ѽ��\n");
    for (int i = 0; i < book_count; i++) {
        printf("%d\t%s\t%s\t%s\t%s\t%.2f\t%s\t%d\t%d\t%d\n", books[i].id, books[i].title, books[i].author, books[i].publisher,
            books[i].pub_date, books[i].price, books[i].category, books[i].total_quantity, books[i].current_quantity, books[i].borrowed_quantity);
    }
}

void display_readers() {
    printf("�����б�\n");
    printf("���\t����\t���\t��������\n");
    for (int i = 0; i < reader_count; i++) {
        printf("%d\t%s\t%s\t%d\n", readers[i].id, readers[i].name, readers[i].category, readers[i].max_borrow);
    }
}

void display_borrow_rank() {
    // ���ڽ��Ĵ���������ʾ
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
    printf("�������а�:\n");
    printf("���Ĵ���\t����\t����֤��\t��������\n");
    for (i = 0; i < borrow_count; i++) {
        printf("%d\t%s\t%d\t%s\n", borrows[i].renew_count, books[borrows[i].book_id - 1].title, borrows[i].reader_id, readers[borrows[i].reader_id - 1].name);
    }
}

void display_overdue_books() {
    printf("����δ��ͼ���б�\n");
    printf("���\t����\t����֤��\t��������\tӦ�黹����\t������\n");
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

    printf("��ͼ����Ŀ��%d\n", total_books);
    printf("�ܽ����Ŀ��%d\n", total_borrowed);
    printf("��������Ŀ��%d\n", total_overdue);
}

void set_fine_amount() {
    printf("��ǰ������Ϊ��%.2fԪ/��\n", fine_amount);
    printf("�������µķ����");
    scanf("%lf", &fine_amount);
    printf("���������óɹ���\n");
}
