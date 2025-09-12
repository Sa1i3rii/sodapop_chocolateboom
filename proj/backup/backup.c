#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "donations.csv"
#define MAX_LINE 256
/*
// โครงสร้างข้อมูล
typedef struct {
    char donorName[50];
    int donationAmount;     
    char donationDate[20];
    char project[100];
} Donation; */

typedef struct {
    char donorName[50],donationDate[20],project[100];
    int donationAmount;      
    
} Donation;

// ฟังก์ชันเพิ่มข้อมูล
void addDonation() {
    Donation d;
    FILE *fp = fopen(FILENAME, "a");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }
    printf("Donor Name: ");
    scanf(" %[^\n]", d.donorName);
    printf("Donation Amount: ");
    scanf("%d", &d.donationAmount);  // เปลี่ยนเป็น %d
    printf("Donation Date (YYYY-MM-DD): ");
    scanf("%s", d.donationDate);
    printf("Project: ");
    scanf(" %[^\n]", d.project);

    fprintf(fp, "%s,%d,%s,%s\n", d.donorName, d.donationAmount, d.donationDate, d.project); // %d
    fclose(fp);
    printf("Donation added successfully!\n");
}
// ฟังก์ชันแสดงข้อมูลทั้งหมด
void viewDonations() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No donation records found.\n");
        return;
    }
    char line[MAX_LINE];
    printf("\n--- All Donations ---\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

// ฟังก์ชันค้นหาข้อมูล
void searchDonation() {
    char keyword[50];
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No donation records found.\n");
        return;
    }
    printf("Enter donor name or project to search: ");
    scanf(" %[^\n]", keyword);

    char line[MAX_LINE];
    printf("\n--- Search Results ---\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, keyword)) {
            printf("%s", line);
        }
    }
    fclose(fp);
}

// ฟังก์ชันเมนู
void displayMenu() {
    printf("\n=== Donation Management System ===\n");
    printf("1. Add donation\n");
    printf("2. View all donations\n");
    printf("3. Search donation\n");
    printf("4. Exit\n");
    printf("Enter choice: ");
}

int main() {
    int choice;
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: addDonation(); break;
            case 2: viewDonations(); break;
            case 3: searchDonation(); break;
            case 4: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
