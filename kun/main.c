#include <stdio.h>
#include <stdlib.h>
#include "iii.h"

int main() {
    int num;
    while (1) {
        displayMenu();
        scanf("%d", &num);
        switch (num) {
            case 1: addDonation(); break;
            case 2: viewDonations(); break;
            case 3: searchDonation(); break;
            case 4: 
            printf("Program by Sa1i3rii\n");
                return 0; 
            default: printf("Invalid choice.\n");
            
        }
    }
    
    return 0;
}
