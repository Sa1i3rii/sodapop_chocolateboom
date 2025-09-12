#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "donations.csv"
#define MAX_LINE 256


// โครงสร้างข้อมูล
typedef struct {
    char donorName[50],donationDate[20],project[100];
    int donationAmount; 
} Donation; 

// add Data
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
    scanf("%d", &d.donationAmount);  
    char dateInput[9];
    printf("Donation Date (YYYYMMDD): ");
    scanf("%8s", dateInput);
    // เติมขีดให้อัตโนมัติ
    snprintf(d.donationDate, sizeof(d.donationDate), "%.4s-%.2s-%.2s", dateInput, dateInput+4, dateInput+6);
    printf("Project: ");
    scanf(" %[^\n]", d.project);

    fprintf(fp, "%s,%d,%s,%s\n", d.donorName, d.donationAmount, d.donationDate, d.project); // %d
    fclose(fp);
    printf("Donation added successfully!\n");
}


// ฟังก์ชันอัปเดตข้อมูล
void updateDonation() {
    char searchName[50];
    int found = 0;
    printf("Enter donor name to update: ");
    scanf(" %[^\n]", searchName);

    FILE *fp = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (!fp || !temp) {
        printf("Error opening file.\n");
        if(fp) fclose(fp);
        if(temp) fclose(temp);
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        Donation d;
        char *token = strtok(line, ",");
        if (token) strncpy(d.donorName, token, sizeof(d.donorName));
        token = strtok(NULL, ",");
        if (token) d.donationAmount = atoi(token);
        token = strtok(NULL, ",");
        if (token) strncpy(d.donationDate, token, sizeof(d.donationDate));
        token = strtok(NULL, ",\n");
        if (token) strncpy(d.project, token, sizeof(d.project));

        if (strcmp(d.donorName, searchName) == 0) {
            found = 1;
            printf("Enter new donation amount: ");
            scanf("%d", &d.donationAmount);
            printf("Enter new donation date (YYYYMMDD): ");
            char dateInput[9];
            scanf("%8s", dateInput);
            snprintf(d.donationDate, sizeof(d.donationDate), "%.4s-%.2s-%.2s", dateInput, dateInput+4, dateInput+6);
            printf("Enter new project: ");
            scanf(" %[^\n]", d.project);
            printf("Record updated.\n");
        }
        fprintf(temp, "%s,%d,%s,%s\n", d.donorName, d.donationAmount, d.donationDate, d.project);
    }
    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.csv", FILENAME);

    if (!found) {
        printf("Donor not found.\n");
    }
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
}// ...existing code...

// ฟังก์ชันลบข้อมูล
void deleteDonation() {
    char keyword[50];
    printf("Enter donor name, amount, date, or project to delete: ");
    scanf(" %[^\n]", keyword);

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No donation records found.\n");
        return;
    }

    Donation matches[100];
    char lines[100][MAX_LINE];
    int matchIndexes[100];
    int count = 0, total = 0;
    char line[MAX_LINE];

    // ค้นหาข้อมูลที่ตรงกับ keyword
    while (fgets(line, sizeof(line), fp)) {
        Donation d;
        char *token = strtok(line, ",");
        if (token) strncpy(d.donorName, token, sizeof(d.donorName));
        token = strtok(NULL, ",");
        if (token) d.donationAmount = atoi(token);
        token = strtok(NULL, ",");
        if (token) strncpy(d.donationDate, token, sizeof(d.donationDate));
        token = strtok(NULL, ",\n");
        if (token) strncpy(d.project, token, sizeof(d.project));

        // ถ้ามีข้อมูลตรง keyword ใด keyword หนึ่ง
        if (strstr(d.donorName, keyword) || 
            strstr(d.donationDate, keyword) || 
            strstr(d.project, keyword) || 
            (atoi(keyword) == d.donationAmount)) {
            matches[count] = d;
            strncpy(lines[count], line, MAX_LINE);
            matchIndexes[count] = total;
            count++;
        }
        total++;
    }
    fclose(fp);

    if (count == 0) {
        printf("No matching records found.\n");
        return;
    }

    // แสดงข้อมูลที่ตรงกัน
    printf("\nMatching records:\n");
    printf("%-5s %-25s | %-10s | %-12s | %-20s\n", "No.", "Donor Name", "Amount", "Date", "Project");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-25s | %-10d | %-12s | %-20s\n", i+1, matches[i].donorName, matches[i].donationAmount, matches[i].donationDate, matches[i].project);
    }

    int delIndex = 0;
    if (count == 1) {
        printf("Deleting the only matching record.\n");
        delIndex = 1;
    } else {
        printf("Enter the number (No.) of the record you want to delete: ");
        scanf("%d", &delIndex);
        if (delIndex < 1 || delIndex > count) {
            printf("Invalid selection. Cancel delete.\n");
            return;
        }
    }

    // ลบข้อมูลที่เลือก
    fp = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.csv", "w");
    if (!fp || !temp) {
        printf("Error opening file.\n");
        if(fp) fclose(fp);
        if(temp) fclose(temp);
        return;
    }

    int current = 0, deleted = 0;
    while (fgets(line, sizeof(line), fp)) {
        // ถ้า current ตรงกับ matchIndexes[delIndex-1] ให้ข้าม
        if (current == matchIndexes[delIndex-1]) {
            deleted = 1;
        } else {
            fputs(line, temp);
        }
        current++;
    }
    fclose(fp);
    fclose(temp);

    remove(FILENAME);
    rename("temp.csv", FILENAME);

    if (deleted) {
        printf("Record deleted successfully.\n");
    } else {
        printf("Delete failed.\n");
    }
}

// Search

// ...existing code...
void searchDonation() {
    char keyword[50];
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No donation records found.\n");
        return;
    }
    printf("\n--- All Donations ---\n");
    printf("%-25s | %-10s | %-12s | %-20s\n", "Donor Name", "Amount", "Date", "Project");
    printf("--------------------------------------------------------------------------\n");
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        Donation d;
        char *token = strtok(line, ",");
        if (token) strncpy(d.donorName, token, sizeof(d.donorName));
        token = strtok(NULL, ",");
        if (token) d.donationAmount = atoi(token);
        token = strtok(NULL, ",");
        if (token) strncpy(d.donationDate, token, sizeof(d.donationDate));
        token = strtok(NULL, ",\n");
        if (token) strncpy(d.project, token, sizeof(d.project));

        printf("%-25s | %-10d | %-12s | %-20s\n", d.donorName, d.donationAmount, d.donationDate, d.project);
    }
    fclose(fp);

    // ค้นหาข้อมูล
    fp = fopen(FILENAME, "r");
    if (!fp) return;
    printf("\nEnter donor name or project to search: ");
    scanf(" %[^\n]", keyword);

    int found = 0;
    printf("\n--- Search Results ---\n");
    printf("%-25s | %-10s | %-12s | %-20s\n", "Donor Name", "Amount", "Date", "Project");
    printf("--------------------------------------------------------------------------\n");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, keyword)) {
            Donation d;
            char *token = strtok(line, ",");
            if (token) strncpy(d.donorName, token, sizeof(d.donorName));
            token = strtok(NULL, ",");
            if (token) d.donationAmount = atoi(token);
            token = strtok(NULL, ",");
            if (token) strncpy(d.donationDate, token, sizeof(d.donationDate));
            token = strtok(NULL, ",\n");
            if (token) strncpy(d.project, token, sizeof(d.project));

            printf("%-25s | %-10d | %-12s | %-20s\n", d.donorName, d.donationAmount, d.donationDate, d.project);
            found = 1;
        }
    }
    if (!found) {
        printf("No matching records found.\n");
    }
    fclose(fp);
}
//

// ...existing code...
// เพิ่มฟังก์ชันเปรียบเทียบวันที่ (YYYY-MM-DD)
int compareDate(const char *date1, const char *date2) {
    return strcmp(date1, date2);
}

// ฟังก์ชันจัดเรียงข้อมูลตามวันที่ (จากเก่าไปใหม่)
void sortDonationsByDate() {
    Donation donations[1000];
    int count = 0;

    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No donation records found.\n");
        return;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
        Donation d;
        char *token = strtok(line, ",");
        if (token) strncpy(d.donorName, token, sizeof(d.donorName));
        token = strtok(NULL, ",");
        if (token) d.donationAmount = atoi(token);
        token = strtok(NULL, ",");
        if (token) strncpy(d.donationDate, token, sizeof(d.donationDate));
        token = strtok(NULL, ",\n");
        if (token) strncpy(d.project, token, sizeof(d.project));
        donations[count++] = d;
    }
    fclose(fp);

    // Bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (compareDate(donations[j].donationDate, donations[j + 1].donationDate) > 0) {
                Donation temp = donations[j];
                donations[j] = donations[j + 1];
                donations[j + 1] = temp;
            }
        }
    }

    // เขียนกลับลงไฟล์
    fp = fopen(FILENAME, "w");
    if (!fp) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%d,%s,%s\n", donations[i].donorName, donations[i].donationAmount, donations[i].donationDate, donations[i].project);
    }
    fclose(fp);
    printf("Sorted by date successfully!\n");
}
// ...existing code...


// Menu
void displayMenu() {
    printf("\n=== Donation Management System ===\n");
    printf("1. Add donation\n");
    printf("2. Update donation\n");
    printf("3. View all donations\n");
    printf("4. Search donation\n");
    printf("5. Exit\n");
    printf("Enter choice: ");
}