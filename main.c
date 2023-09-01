#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_ROWS 7
#define NUM_COLS 16

// Structure to hold movie details
struct Movie
{
    char name[100];
    float price;
    char timing[50];
    struct Movie *next;
};

// Structure to hold customer details
struct Customer
{
    char name[100];
    char phone[20];
};

// Structure to hold booked seats
struct BookedSeat
{
    char movieName[100];
    char timing[50];
    int row;
    int col;
    int numSeats;
    float totalPrice;
    struct Customer customer;
    struct BookedSeat *next;
};

// Global 2D array to represent seats
char cinemaHall[NUM_ROWS][NUM_COLS];

// Function prototypes
void displayMovies(struct Movie *head);
void addNewMovie(struct Movie **head, const char *name, float price, const char *timing);
void bookSeats(struct Movie *head, struct BookedSeat **bookingsHead, const char *movieName, const char *timing, int numSeats, struct Customer customer);
void displaySeats(const char *movieName, const char *timing);
void generateReceipt(struct BookedSeat *booking, float seatPrice);
void saveBookingToFile(struct BookedSeat *booking, float seatPrice);
void saveSeatsToFile();
void loadMoviesFromFile(struct Movie **head);
void showSeatsForMovie(struct Movie *moviesHead, struct BookedSeat *bookingsHead);
struct BookedSeat *getBookingForMovie(struct BookedSeat *head, const char *movieName, const char *timing);

// Function to display the main menu
void displayMainMenu()
{
    printf("\n======== Movie Booking System ========\n");
    printf("1. View available movies\n");
    printf("2. Add new movie\n");
    printf("3. Book seats\n");
    printf("4. Show seats for specific movie\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

// Function to initialize cinema hall with empty seats
void initializeCinemaHall()
{
    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            if (j != 7 && j != 8)
            {
                cinemaHall[i][j] = '0'; // Use '0' to represent empty seats
            }
            else
            {
                cinemaHall[i][j] = ' '; // Use ' ' for empty spaces
            }
        }
    }

    // Add passageways
    for (int i = 1; i < NUM_ROWS; i += 1)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            if (j == 7 || j == 8)
            {
                cinemaHall[i][j] = '|'; // Use '|' to represent passageways
            }
        }
    }
}

// Function to get an integer from the user with validation
int getIntegerInput()
{
    int input;
    while (1)
    {
        int numScanned = scanf("%d", &input);
        if (numScanned != 1)
        {
            printf("Invalid input. Please enter a valid number: ");
            while (getchar() != '\n')
                ; // Clear the input buffer
        }
        else
        {
            break;
        }
    }
    return input;
}

// Function to get a floating-point number from the user with validation
float getFloatInput()
{
    float input;
    while (1)
    {
        int numScanned = scanf("%f", &input);
        if (numScanned != 1)
        {
            printf("Invalid input. Please enter a valid number: ");
            while (getchar() != '\n')
                ; // Clear the input buffer
        }
        else
        {
            break;
        }
    }
    return input;
}

// Function to convert a row number to a character label (A, B, C, ...)
char rowToLabel(int row)
{
    return 'A' + row;
}

// Function to convert a character label (A, B, C, ...) to a row number
int labelToRow(char label)
{
    return label - 'A';
}

int main()
{
    struct Movie *moviesHead = NULL;
    struct BookedSeat *bookingsHead = NULL;
    struct Customer customer;
    char movieName[100], timing[50];
    int numSeats;
    float price;

    // Initialize cinema hall with empty seats and passageways
    initializeCinemaHall();

    // Load movies from Movies.txt and store them in the 'movies' linked list
    loadMoviesFromFile(&moviesHead);

    int choice;
    do
    {
        displayMainMenu();
        choice = getIntegerInput(); // Use the input validation function

        switch (choice)
        {
        case 1:
            printf("\nAvailable Movies:\n");
            displayMovies(moviesHead);
            break;

        case 2:
            printf("\nEnter movie name: ");
            scanf("%s", movieName);
            printf("Enter movie price: ");
            price = getFloatInput(); // Use the input validation function
            printf("Enter movie timing: ");
            scanf("%s", timing);

            addNewMovie(&moviesHead, movieName, price, timing);
            printf("Movie added successfully!\n");
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
            numSeats = getIntegerInput(); // Use the input validation function

            bookSeats(moviesHead, &bookingsHead, movieName, timing, numSeats, customer);
            break;

        case 4:
            showSeatsForMovie(moviesHead, bookingsHead);
            break;

        case 5:
            printf("Exiting...\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 5);

    // Cleanup: Free dynamically allocated memory for movies and bookings linked lists
    while (moviesHead != NULL)
    {
        struct Movie *temp = moviesHead;
        moviesHead = moviesHead->next;
        free(temp);
    }

    while (bookingsHead != NULL)
    {
        struct BookedSeat *temp = bookingsHead;
        bookingsHead = bookingsHead->next;
        free(temp);
    }

    // Save seat reservations and availability to files
    saveSeatsToFile();

    return 0;
}

void displayMovies(struct Movie *head)
{
    struct Movie *current = head;
    int index = 1;
    while (current != NULL)
    {
        printf("%d. %s (%s) - $%.2f\n", index, current->name, current->timing, current->price);
        current = current->next;
        index++;
    }
}

void addNewMovie(struct Movie **head, const char *name, float price, const char *timing)
{
    // Check if the movie already exists in the file
    FILE *moviesFile = fopen("Movies.txt", "r");
    if (moviesFile == NULL)
    {
        printf("Error opening Movies.txt file.\n");
        return;
    }

    char movieName[100];
    float moviePrice;
    char movieTiming[50];
    bool movieExists = false;

    while (fscanf(moviesFile, "%s %f %s", movieName, &moviePrice, movieTiming) != EOF)
    {
        if (strcmp(movieName, name) == 0 && strcmp(movieTiming, timing) == 0)
        {
            // Movie already exists
            movieExists = true;
            break;
        }
    }

    fclose(moviesFile);

    // If the movie already exists, do not add it again
    if (movieExists)
    {
        printf("Movie '%s' (%s) already exists in the file.\n", name, timing);
        return;
    }

    // Movie doesn't exist, proceed to add it
    struct Movie *newMovie = (struct Movie *)malloc(sizeof(struct Movie));
    if (newMovie == NULL)
    {
        printf("Memory allocation error.\n");
        return;
    }

    strcpy(newMovie->name, name);
    newMovie->price = price;
    strcpy(newMovie->timing, timing);
    newMovie->next = NULL;

    // Append the movie details to the "Movies.txt" file
    moviesFile = fopen("Movies.txt", "a");
    if (moviesFile == NULL)
    {
        printf("Error opening Movies.txt file for writing.\n");
        free(newMovie);
        return;
    }

    fprintf(moviesFile, "%s %.2f %s\n", newMovie->name, newMovie->price, newMovie->timing);
    fclose(moviesFile);

    if (*head == NULL)
    {
        *head = newMovie;
    }
    else
    {
        struct Movie *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newMovie;
    }
}
void loadMoviesFromFile(struct Movie **head)
{
    FILE *moviesFile = fopen("Movies.txt", "r");
    if (moviesFile == NULL)
    {
        printf("Error opening Movies.txt file.\n");
        return;
    }

    char name[100];
    float price;
    char timing[50];

    while (fscanf(moviesFile, "%s %f %s", name, &price, timing) == 3)
    {
        // Create a new movie node
        struct Movie *newMovie = (struct Movie *)malloc(sizeof(struct Movie));
        if (newMovie == NULL)
        {
            printf("Memory allocation error.\n");
            fclose(moviesFile);
            return;
        }

        strcpy(newMovie->name, name);
        newMovie->price = price;
        strcpy(newMovie->timing, timing);
        newMovie->next = NULL;

        // Add the new movie to the linked list
        if (*head == NULL)
        {
            *head = newMovie;
        }
        else
        {
            struct Movie *current = *head;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newMovie;
        }
    }

    fclose(moviesFile);
}


void bookSeats(struct Movie *head, struct BookedSeat **bookingsHead, const char *movieName, const char *timing, int numSeats, struct Customer customer)
{
    struct Movie *current = head;
    while (current != NULL)
    {
        if (strcmp(current->name, movieName) == 0 && strcmp(current->timing, timing) == 0)
        {
            // Movie found, book seats
            struct BookedSeat *newBooking = (struct BookedSeat *)malloc(sizeof(struct BookedSeat));
            if (newBooking == NULL)
            {
                printf("Memory allocation error.\n");
                return;
            }

            strcpy(newBooking->movieName, movieName);
            strcpy(newBooking->timing, timing);
            newBooking->numSeats = numSeats;
            newBooking->customer = customer;
            newBooking->next = *bookingsHead;
            *bookingsHead = newBooking;

            // Display the updated seats
            displaySeats(movieName, timing);

            // Prompt the user to select seats
            printf("Select seats by entering the row (A-G) and seat number (1-14) for each seat:\n");

            for (int i = 0; i < numSeats; i++)
            {
                char seatPosition[5];
                printf("Enter seat %d (e.g., A3): ", i + 1);
                scanf("%s", seatPosition);

                // Parse the seat position
                char rowLabel;
                int seatNumber;
                if (sscanf(seatPosition, "%c%d", &rowLabel, &seatNumber) == 2)
                {
                    int row = labelToRow(rowLabel);
                    int col;
                    if (seatNumber <= 7)
                    {
                        col = seatNumber - 1;
                    }
                    else if (seatNumber >= 8)
                    {
                        col = seatNumber + 1;
                    }

                    // Check if the seat is available
                    if (cinemaHall[row][col] == '0')
                    {
                        // Book the seat
                        cinemaHall[row][col] = 'X';
                        newBooking->row = row;
                        newBooking->col = col;
                    }
                    else
                    {
                        // Seat is already booked
                        printf("Seat %s is already booked. Please choose another seat.\n", seatPosition);
                        i--; // Decrement i to re-enter the same seat
                    }
                }
                else
                {
                    // Invalid seat position format
                    printf("Invalid seat position format. Please use the format: A3, B5, etc.\n");
                    i--; // Decrement i to re-enter the same seat
                }
            }

            printf("\n%d seat(s) booked successfully for the movie '%s' (%s).\n", numSeats, movieName, timing);
            // Generate and print the receipt
            generateReceipt(newBooking, current->price);

            // Save the booking to file
            saveBookingToFile(newBooking, current->price);

            return;
        }
        current = current->next;
    }

    // If movie not found
    printf("\nMovie '%s' (%s) not found.\n", movieName, timing);
}

void displaySeats(const char *movieName, const char *timing)
{
    printf("\n========================= Seats for %s (%s) ========================\n", movieName, timing);

    // Display column numbers
    printf("   ");
    for (int j = 1; j <= NUM_COLS; j++)
    {
        if (j == 8 || j == 9)
        {
            printf("|  "); // Display empty spaces for passageways
        }
        else
        {
            if (j <= 7)
            {
                printf("%2d  ", j); // Display seat numbers
            }
            else
            {
                printf("%2d  ", j - 2); // Display seat numbers
            }
        }
    }
    printf("\n");

    // Iterate through the cinema hall and display the seats
    for (int i = 0; i < NUM_ROWS; i++)
    {
        printf("%c  ", rowToLabel(i)); // Display row label (A, B, C, ...)
        for (int j = 0; j < NUM_COLS; j++)
        {
            if (j == 7 || j == 8)
            {
                printf("|  "); // Display empty spaces for passageways
            }
            else
            {
                printf("%2c  ", cinemaHall[i][j]); // Display seat status ('0' or 'X')
            }
        }
        printf("\n");
    }

    printf("==================================================================\n");
}

void generateReceipt(struct BookedSeat *booking, float seatPrice)
{
    printf("\n========== Receipt ==========\n");
    printf("Movie: %s (%s)\n", booking->movieName, booking->timing);
    printf("Customer Name: %s\n", booking->customer.name);
    printf("Phone Number: %s\n", booking->customer.phone);
    printf("Number of seats: %d\n", booking->numSeats);
    printf("Total Price: $%.2f\n", booking->numSeats * seatPrice); // Calculate total price
    printf("=============================\n");
}

void saveBookingToFile(struct BookedSeat *booking, float seatPrice)
{
    FILE *bookedFile = fopen("booked.txt", "a");
    if (bookedFile == NULL)
    {
        printf("Error opening booked.txt file.\n");
        return;
    }

    fprintf(bookedFile, "Movie: %s (%s)\n", booking->movieName, booking->timing);
    fprintf(bookedFile, "Customer Name: %s\n", booking->customer.name);
    fprintf(bookedFile, "Phone Number: %s\n", booking->customer.phone);
    fprintf(bookedFile, "Number of seats: %d\n", booking->numSeats);
    fprintf(bookedFile, "Total Price: $%.2f\n", booking->numSeats * seatPrice); // Assuming $10 per seat
    fprintf(bookedFile, "=============================\n");

    fclose(bookedFile);
}

void saveSeatsToFile()
{
    FILE *seatsFile = fopen("seats.txt", "w");
    if (seatsFile == NULL)
    {
        printf("Error opening seats.txt file.\n");
        return;
    }

    for (int i = 0; i < NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_COLS; j++)
        {
            fputc(cinemaHall[i][j], seatsFile);
        }
        fputc('\n', seatsFile);
    }

    fclose(seatsFile);
}

struct BookedSeat *getBookingForMovie(struct BookedSeat *head, const char *movieName, const char *timing)
{
    struct BookedSeat *current = head;
    while (current != NULL)
    {
        if (strcmp(current->movieName, movieName) == 0 && strcmp(current->timing, timing) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void showSeatsForMovie(struct Movie *moviesHead, struct BookedSeat *bookingsHead)
{
    char movieName[100], timing[50];

    printf("\nAvailable Movies:\n");
    displayMovies(moviesHead);

    printf("Enter the name of the movie to see seats: ");
    scanf("%s", movieName);
    printf("Enter the timing of the movie: ");
    scanf("%s", timing);

    struct BookedSeat *booking = getBookingForMovie(bookingsHead, movieName, timing);

    if (booking != NULL)
    {
        printf("\n========================= Seats for %s (%s) ========================\n", movieName, timing);

        // Display row labels (A, B, C, ...)
        printf("   ");

        for (int j = 1; j <= NUM_COLS; j++)
        {
            if (j == 8 || j == 9)
            {
                printf("|  "); // Display empty spaces for passageways
            }
            else
            {
                if (j <= 7)
                {
                    printf("%2d  ", j); // Display seat numbers
                }
                else
                {
                    printf("%2d  ", j - 2); // Display seat numbers
                }
            }
        }
        printf("\n");

        for (int i = 0; i < NUM_ROWS; i++)
        {
            printf("%c  ", rowToLabel(i)); // Display row label (A, B, C, ...)
            for (int j = 0; j < NUM_COLS; j++)
            {
                if (j == 7 || j == 8)
                {
                    printf("|  "); // Display empty spaces for passageways
                }
                else
                {
                    printf("%2c  ", cinemaHall[i][j]); // Display seat status ('0' or 'X')
                }
            }
            printf("\n");
        }

        printf("==================================================================\n");
    }
    else
    {
        printf("\nMovie '%s' (%s) not found or no seats have been booked for this movie.\n", movieName, timing);
    }
}
