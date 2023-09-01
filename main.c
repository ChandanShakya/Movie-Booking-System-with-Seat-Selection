#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define NUM_ROWS 7
#define NUM_COLS 16
#define MAX_MOVIES 100
#define MAX_BOOKINGS 100

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
};

// Global 2D array to represent seats
char cinemaHall[NUM_ROWS][NUM_COLS];

// Global arrays to store movies and bookings
struct BookedSeat bookings[MAX_BOOKINGS];
int numBookings = 0;

struct Movie
{
    char name[100];
    float price;
    char timing[50];
};

struct Movie movies[MAX_MOVIES];
int numMovies = 0;

// Function prototypes
void displayMovies();
void addNewMovie(const char *name, float price, const char *timing);
void bookSeats(const char *movieName, const char *timing, int numSeats, struct Customer customer);
void displaySeats(const char *movieName, const char *timing);
void generateReceipt(int bookingIndex);
void saveBookingToFile(int bookingIndex);
void saveSeatsToFile();
void loadMoviesFromFile();
int findMovieIndex(const char *movieName, const char *timing);
void showSeatsForMovie(const char *movieName, const char *timing);

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
    char movieName[100], timing[50];
    int numSeats;
    float price;

    // Initialize cinema hall with empty seats and passageways
    initializeCinemaHall();

    // Load movies from Movies.txt
    loadMoviesFromFile();
    system("cls");

    int choice;
    do
    {
        displayMainMenu();
        choice = getIntegerInput(); // Use the input validation function

        switch (choice)
        {
        case 1:
            system("cls");
            printf("\nAvailable Movies:\n");
            displayMovies();
            getch();
            system("cls");
            break;

        case 2:
            system("cls");
            printf("\nEnter movie name: ");
            scanf("%s", movieName);
            printf("Enter movie price: ");
            price = getFloatInput(); // Use the input validation function
            printf("Enter movie timing: ");
            scanf("%s", timing);
            system("cls");

            addNewMovie(movieName, price, timing);
            getch();
            system("cls");

            break;

        case 3:
            system("cls");

            printf("\nAvailable Movies:\n");
            displayMovies();
            printf("Enter the name of the movie you want to book: ");
            scanf("%s", movieName);
            printf("Enter the timing of the movie you want to book: ");
            scanf("%s", timing);

            struct Customer customer;
            printf("Enter your name: ");
            scanf("%s", customer.name);
            printf("Enter your phone number: ");
            scanf("%s", customer.phone);

            printf("Enter the number of seats you want to book: ");
            numSeats = getIntegerInput(); // Use the input validation function
            system("cls");

            bookSeats(movieName, timing, numSeats, customer);
            getch();
            system("cls");

            break;

        case 4:
            system("cls");

            printf("\nEnter movie name: ");
            scanf("%s", movieName);
            printf("Enter movie timing: ");
            scanf("%s", timing);
            system("cls");

            showSeatsForMovie(movieName, timing);
            getch();
            system("cls");

            break;
        case 5:
            printf("Exiting...\n");
            break;

        default:
            system("cls");

            printf("Invalid choice. Please try again.\n");
            getch();
            system("cls");

            break;
        }
    } while (choice != 5);

    // Save seat reservations and availability to files
    saveSeatsToFile();

    return 0;
}

void displayMovies()
{
    for (int i = 0; i < numMovies; i++)
    {
        printf("%d. %s (%s) - $%.2f\n", i + 1, movies[i].name, movies[i].timing, movies[i].price);
    }
}

void addNewMovie(const char *name, float price, const char *timing)
{
    // Check if the movie already exists
    for (int i = 0; i < numMovies; i++)
    {
        if (strcmp(movies[i].name, name) == 0 && strcmp(movies[i].timing, timing) == 0)
        {
            printf("Movie '%s' (%s) already exists.\n", name, timing);
            return;
        }
    }

    // If not, add the movie
    if (numMovies < MAX_MOVIES)
    {
        strcpy(movies[numMovies].name, name);
        movies[numMovies].price = price;
        strcpy(movies[numMovies].timing, timing);
        numMovies++;
        printf("Movie added successfully!\n");
    }
    else
    {
        printf("Maximum number of movies reached.\n");
    }
}

void loadMoviesFromFile()
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
        addNewMovie(name, price, timing);
    }

    fclose(moviesFile);
}

void bookSeats(const char *movieName, const char *timing, int numSeats, struct Customer customer)
{
    int movieIndex = findMovieIndex(movieName, timing);

    if (movieIndex == -1)
    {
        printf("\nMovie '%s' (%s) not found.\n", movieName, timing);
        return;
    }

    if (numSeats <= 0)
    {
        printf("Invalid number of seats.\n");
        return;
    }

    printf("Booking %d seats for the movie: %s (%s)\n", numSeats, movieName, timing);

    // Loop to book each seat
    for (int i = 0; i < numSeats; i++)
    {
        printf("Enter seat position for seat %d (e.g., A3): ", i + 1);
        char seatPosition[5];
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
            if (row >= 0 && row < NUM_ROWS && col >= 0 && col < NUM_COLS && cinemaHall[row][col] == '0')
            {
                cinemaHall[row][col] = 'X';

                // Create a booking record
                strcpy(bookings[numBookings].movieName, movieName);
                strcpy(bookings[numBookings].timing, timing);
                bookings[numBookings].row = row;
                bookings[numBookings].col = col;
                bookings[numBookings].numSeats = 1; // Book one seat at a time
                bookings[numBookings].customer = customer;
                bookings[numBookings].totalPrice = movies[movieIndex].price; // Price for one seat

                numBookings++;

                printf("Seat booked: Row %c, Seat %d\n", rowToLabel(row), col + 1);
            }
            else
            {
                printf("Seat %s is already booked or invalid. Please choose another seat.\n", seatPosition);
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
}


void displaySeats(const char *movieName, const char *timing)
{
    int movieIndex = findMovieIndex(movieName, timing);

    if (movieIndex == -1)
    {
        printf("\nMovie '%s' (%s) not found.\n", movieName, timing);
        return;
    }

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

void generateReceipt(int bookingIndex)
{
    printf("\n========== Receipt ==========\n");
    printf("Movie: %s (%s)\n", bookings[bookingIndex].movieName, bookings[bookingIndex].timing);
    printf("Customer Name: %s\n", bookings[bookingIndex].customer.name);
    printf("Phone Number: %s\n", bookings[bookingIndex].customer.phone);
    printf("Number of seats: %d\n", bookings[bookingIndex].numSeats);
    printf("Total Price: $%.2f\n", bookings[bookingIndex].totalPrice); // Calculate total price
    printf("=============================\n");
}

void saveBookingToFile(int bookingIndex)
{
    FILE *bookedFile = fopen("booked.txt", "a");
    if (bookedFile == NULL)
    {
        printf("Error opening booked.txt file.\n");
        return;
    }

    fprintf(bookedFile, "Movie: %s (%s)\n", bookings[bookingIndex].movieName, bookings[bookingIndex].timing);
    fprintf(bookedFile, "Customer Name: %s\n", bookings[bookingIndex].customer.name);
    fprintf(bookedFile, "Phone Number: %s\n", bookings[bookingIndex].customer.phone);
    fprintf(bookedFile, "Number of seats: %d\n", bookings[bookingIndex].numSeats);
    fprintf(bookedFile, "Total Price: $%.2f\n", bookings[bookingIndex].totalPrice);
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

int findMovieIndex(const char *movieName, const char *timing)
{
    for (int i = 0; i < numMovies; i++)
    {
        if (strcmp(movies[i].name, movieName) == 0 && strcmp(movies[i].timing, timing) == 0)
        {
            return i;
        }
    }
    return -1;
}

void showSeatsForMovie(const char *movieName, const char *timing)
{
    int movieIndex = findMovieIndex(movieName, timing);

    if (movieIndex == -1)
    {
        printf("\nMovie '%s' (%s) not found or no seats have been booked for this movie.\n", movieName, timing);
        return;
    }

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
