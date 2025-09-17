#include <stdio.h>
#include <string.h>

#define FILENAME "Data.csv"
#define MAX_DONATIONS 100

char names[MAX_DONATIONS][50],dates[MAX_DONATIONS][20],projects[MAX_DONATIONS][100],line[100];
int amounts[MAX_DONATIONS],count = 0;


// ฟังก์ชันเพิ่มข้อมูล
void add() {
    if (count >= MAX_DONATIONS) {
        printf("Data is full.\n");
        return;
    }
    printf("Donor Name: ");
    scanf(" %[^\n]", names[count]);
    printf("Donation Amount: ");
    scanf("%d", &amounts[count]);
    printf("Donation Date (YYYY-MM-DD): ");
    scanf("%s", dates[count]);
    printf("Project: ");
    scanf(" %[^\n]", projects[count]);
    //count++;
    printf("Successfully:)\n");

    // CSV
    FILE *fp = fopen(FILENAME, "a");
    if (fp != NULL) {
        fprintf(fp, "%s,%d,%s,%s\n",
            names[count],
            amounts[count],
            dates[count],
            projects[count]);
        fclose(fp);
    } else {
        printf("Error\n");
    }
    count++;
}

// แสดงข้อมูลทั้งหมด
void viewall() {
    
   FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No donation records found.\n");
        return;
    }
    //char line[100];
    printf("\n--- All Donations ---\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

// ค้นหาข้อมูล
void search() {
    char keyword[50];
    int isHeader = 1;
    
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("No donation records found.\n");
        return;
    }
    printf("Enter donor name or project to search: ");
    scanf(" %[^\n]", keyword);

    while (fgets(line, sizeof(line), fp)) 
        if (isHeader) {
            isHeader = 0;
            continue; 
        }

    
    printf("\n--- Search Results ---\n");
    int found = 0;
    for (int i = 0; i < count; i++) { 
        if (strstr(names[i], keyword)) { 
            printf("%d. Name: %s, Amount: %d, Date: %s, Project: %s\n",
               i + 1,
               names[i],    
               amounts[i],  
               dates[i],    
               projects[i]); 

            found = 1;          
        }
    }
    if (!found){
        printf("Error\n");
    }
    /*
    for (int i = 0; i < count; i++) {
        if (strstr(names[i], keyword) || strstr(projects[i], keyword)) {
            printf("%d. %s, %d, %s, %s\n", i + 1,
                names[i],
                amounts[i],
                dates[i],
                projects[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No matching records found.\n");
    }
        */
       fclose(fp);
}

// เมนู
void Menu() {
    printf("\n=== Donation Management System ===\n");
    printf("1. Add \n");
    printf("2. View all\n");
    printf("3. Search\n");
    printf("4. Exit\n");
    printf("Enter choice: ");
}