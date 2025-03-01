#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_AUTHOR 100
#define FILE_NAME "library.dat"

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int available;
} Book;

void addBook();
void viewBooks();
void issueBook();
void returnBook();
void searchBook();
void removeBook();
void saveBook(Book book);
int getNewBookID();

int main() {
    int choice;
    do {
        printf("\nLibrary Management System\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Search Book\n");
        printf("6. Remove Book\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: searchBook(); break;
            case 6: removeBook(); break;
            case 7: printf("Exiting...\n"); break;
            default: printf("Invalid choice, try again.\n");
        }
    } while (choice != 7);
    return 0;
}

void addBook() {
    Book book;
    FILE *file = fopen(FILE_NAME, "ab");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }
    
    book.id = getNewBookID();
    printf("Enter book title: ");
    fgets(book.title, MAX_TITLE, stdin);
    book.title[strcspn(book.title, "\n")] = 0;
    printf("Enter author: ");
    fgets(book.author, MAX_AUTHOR, stdin);
    book.author[strcspn(book.author, "\n")] = 0;
    book.available = 1;
    
    fwrite(&book, sizeof(Book), 1, file);
    fclose(file);
    printf("Book added successfully!\n");
}

void viewBooks() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("No books found!\n");
        return;
    }
    
    Book book;
    printf("\nBooks in Library:\n");
    printf("ID\tTitle\tAuthor\tAvailable\n");
    while (fread(&book, sizeof(Book), 1, file)) {
        printf("%d\t%s\t%s\t%s\n", book.id, book.title, book.author, book.available ? "Yes" : "No");
    }
    fclose(file);
}

void issueBook() {
    int id, found = 0;
    printf("Enter book ID to issue: ");
    scanf("%d", &id);
    
    FILE *file = fopen(FILE_NAME, "rb+");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }
    
    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id && book.available) {
            book.available = 0;
            fseek(file, -sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);
            printf("Book issued successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found or already issued!\n");
    fclose(file);
}

void returnBook() {
    int id, found = 0;
    printf("Enter book ID to return: ");
    scanf("%d", &id);
    
    FILE *file = fopen(FILE_NAME, "rb+");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }
    
    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id == id && !book.available) {
            book.available = 1;
            fseek(file, -sizeof(Book), SEEK_CUR);
            fwrite(&book, sizeof(Book), 1, file);
            printf("Book returned successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Book not found or already available!\n");
    fclose(file);
}

void searchBook() {
    char title[MAX_TITLE];
    printf("Enter book title to search: ");
    getchar();
    fgets(title, MAX_TITLE, stdin);
    title[strcspn(title, "\n")] = 0;
    
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }
    
    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (strcmp(book.title, title) == 0) {
            printf("Found: ID %d, Author: %s, Available: %s\n", book.id, book.author, book.available ? "Yes" : "No");
            found = 1;
        }
    }
    if (!found) printf("Book not found!\n");
    fclose(file);
}

void removeBook() {
    int id, found = 0;
    printf("Enter book ID to remove: ");
    scanf("%d", &id);
    
    FILE *file = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!file || !temp) {
        printf("Error opening file!\n");
        return;
    }
    
    Book book;
    while (fread(&book, sizeof(Book), 1, file)) {
        if (book.id != id) fwrite(&book, sizeof(Book), 1, temp);
        else found = 1;
    }
    fclose(file);
    fclose(temp);
    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);
    if (found) printf("Book removed successfully!\n");
    else printf("Book not found!\n");
}

int getNewBookID() {
    FILE *file = fopen(FILE_NAME, "rb");
    if (!file) return 1;
    Book book;
    int lastID = 0;
    while (fread(&book, sizeof(Book), 1, file)) lastID = book.id;
    fclose(file);
    return lastID + 1;
}

