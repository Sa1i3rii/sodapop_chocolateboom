#include <stdio.h>
#include <stdlib.h>
#include "donation.h"

int main() {
    int choice;
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1: addDonation(); break;
            case 2: updateDonation(); break;
            case 3: deleteDonation(); break;
            case 4: searchDonation(); break;
            case 5: exit(0);
            default: printf("Invalid choice.\n");
        }
        sortDonationsByDate();
    }
    return 0;
}