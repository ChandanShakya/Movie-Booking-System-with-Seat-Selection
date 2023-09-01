#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure to hold movie details
struct Movie {
    char name[100];
    float price;
    char timing[50];
    struct Movie* next;
};

// Structure to hold customer details
struct Customer {
    char name[100];
    char phone[20];
};

// Structure to hold booked seats
struct BookedSeat {
    char movieName[100];
    char timing[50];
    int numSeats;
    float totalPrice;
    struct Customer customer;
    struct BookedSeat* next;
};

// Function prototypes
void displayMovies(struct Movie* head);
void addNewMovie(struct Movie** head, const char* name, float price, const char* timing);
void bookSeats(struct Movie* head, struct BookedSeat** bookingsHead, const char* movieName, const char* timing, int numSeats, struct Customer customer);
void generateReceipt(struct BookedSeat* booking);

int main() {
    struct Movie* moviesHead = NULL;
    struct BookedSeat* bookingsHead = NULL;
    struct Customer customer;
    char movieName[100], timing[50];
    int numSeats;

    // Read movie details from Movies.txt and store them in the 'movies' linked list
    FILE* moviesFile = fopen("Movies.txt", "r");
    if (moviesFile == NULL) {
        printf("Error opening Movies.txt file.\n");
        return 1;
    }

    char name[100];
    float price;
    while (fscanf(moviesFile, "%s %f %s", name, &price, timing) != EOF) {
        addNewMovie(&moviesHead, name, price, timing);
    }
    fclose(moviesFile);

    int choice;
    do {
        printf("\n======== Movie Booking System ========\n");
        printf("1. View available movies\n");
        printf("2. Add new movie\n");
        printf("3. Book seats\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nAvailable Movies:\n");
                displayMovies(moviesHead);
                break;
            case 2:
                if (moviesHead == NULL) {
                    printf("\nEnter movie name: ");
                    scanf("%s", movieName);
                    printf("Enter movie price: ");
                    scanf("%f", &price);
                    printf("Enter movie timing: ");
                    scanf("%s", timing);
                    addNewMovie(&moviesHead, movieName, price, timing);
                    printf("Movie added successfully!\n");
                } else {
                    printf("\nMaximum movie limit reached. Cannot add more movies.\n");
                }
                break;
            case 3:
                printf("\nAvailable Movies:\n");
                displayMovies(moviesHead);
                printf("Enter the name of the movie you want to book: ");
                scanf("%s", movieName);
                printf("Enter the timing of the movie you want to book: ");
                scanf("%s", timing);

                printf("Enter your name: ");
                scanf("%s", customer.name);
                printf("Enter your phone number: ");
                scanf("%s", customer.phone);

                printf("Enter the number of seats you want to book: ");
                scanf("%d", &numSeats);

                bookSeats(moviesHead, &bookingsHead, movieName, timing, numSeats, customer);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 4);

    // Cleanup: Free dynamically allocated memory for movies and bookings linked lists
    while (moviesHead != NULL) {
        struct Movie* temp = moviesHead;
        moviesHead = moviesHead->next;
        free(temp);
    }

    while (bookingsHead != NULL) {
        struct BookedSeat* temp = bookingsHead;
        bookingsHead = bookingsHead->next;
        free(temp);
    }

    return 0;
}

// Function to display available movies
void displayMovies(struct Movie* head) {
    struct Movie* current = head;
    int index = 1;
    while (current != NULL) {
        printf("%d. %s (%s) - $%.2f\n", index, current->name, current->timing, current->price);
        current = current->next;
        index++;
    }
}

// Function to add a new movie to the linked list
void addNewMovie(struct Movie** head, const char* name, float price, const char* timing) {
    struct Movie* newMovie = (struct Movie*)malloc(sizeof(struct Movie));
    if (newMovie == NULL) {
        printf("Memory allocation error.\n");
        return;
    }

    strcpy(newMovie->name, name);
    newMovie->price = price;
    strcpy(newMovie->timing, timing);
    newMovie->next = NULL;

    if (*head == NULL) {
        *head = newMovie;
    } else {
        struct Movie* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newMovie;
    }
}

// Function to book seats for a movie
void bookSeats(struct Movie* head, struct BookedSeat** bookingsHead, const char* movieName, const char* timing, int numSeats, struct Customer customer) {
    struct Movie* current = head;
    while (current != NULL) {
        if (strcmp(current->name, movieName) == 0 && strcmp(current->timing, timing) == 0) {
            // Movie found, book seats
            struct BookedSeat* newBooking = (struct BookedSeat*)malloc(sizeof(struct BookedSeat));
            if (newBooking == NULL) {
                printf("Memory allocation error.\n");
                return;
            }

            strcpy(newBooking->movieName, movieName);
            strcpy(newBooking->timing, timing);
            newBooking->numSeats = numSeats;
            newBooking->totalPrice = numSeats * current->price;
            newBooking->customer = customer;
            newBooking->next = *bookingsHead;
            *bookingsHead = newBooking;

            printf("\n%d seat(s) booked successfully for the movie '%s' (%s).\n", numSeats, movieName, timing);
            return;
        }
        current = current->next;
    }

    // If movie not found
    printf("\nMovie '%s' (%s) not found.\n", movieName, timing);
}

// Function to generate and print the booking receipt
void generateReceipt(struct BookedSeat* booking) {
    printf("\n========== Receipt ==========\n");
    printf("Movie: %s (%s)\n", booking->movieName, booking->timing);
    printf("Customer Name: %s\n", booking->customer.name);
    printf("Phone Number: %s\n", booking->customer.phone);
    printf("Number of seats: %d\n", booking->numSeats);
    printf("Total Price: $%.2f\n", booking->totalPrice);
    printf("=============================\n");
}
