// needed to allow Visual Studio to work with scanf()
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

extern int errno;
#define CSV_FILE "data.csv"
#define FILE_OPENED 0
#define FILE_CLOSED 1
#define FILE_ERROR 2

#define bool int
#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 25
#define MIN_PURCHASE_AGE 18
#define MAX_PURCHASE_AGE 100
#define MAX_CAR_SALES 10
#define MAX_ATTEMPTS 3            // max attempts of user entries
#define DISCOUNT_PERCENTAGE 0.05f // this means 5%
#define MENU_OPTION_BUY_CAR 'a'
#define MENU_OPTION_VIEW_SALES 'b'
#define MENU_OPTION_VIEW_STOCKS 'c'
#define MENU_OPTION_EXIT 'x'
#define TRUE 1
#define FALSE 0
#define MAX_SALES 100
#define YES 'y'
#define NO 'n'

FILE *file;
unsigned char fileStatus = FILE_CLOSED;
/* Variables */
// don't need negative values; these are generally related to one sale, used when buying cars
unsigned short attempt = 0, customerAge = 0;

bool giveDiscount = FALSE;
char userInputChoice;
unsigned short numberOfSales = 0;
// this array will hold information about each model of car;
char carModels[][10] = {"FIAT", "BMW", "MASERATI"};

typedef struct SalesItem
{
    bool isDiscounted;      // check if the item is discounted
    char carModelName[20];  //
    char carType[20];       //
    char customerName[201]; //
    float carPrice;         //
    float totalPrice;       //
    int saleId;
    short customerAge;                //
    unsigned short purchasedQuantity; //
} SalesItem;

// tracks all sales data
SalesItem *salesData[MAX_SALES];

typedef struct CarStock
{
    char type[20];
    float price;
    short quantity;
} CarStock;

typedef struct CarModel
{
    char name[20];
    CarStock carStock[10];
} CarModel;

// carModel with their stats
CarModel bmwModel = {
    name : "BMW",
    carStock : {
        {
            type : "SEDAN",
            price : 4500.00,
            quantity : 2
        },
        {
            type : "COUPE",
            price : 4500.00,
            quantity : 2
        },
        {
            type : "SPORT",
            price : 4500.00,
            quantity : 2
        }}
};

CarModel fiatModel = {
    name : "Fiat",
    carStock : {
        {
            type : "HatchBack",
            price : 4500.00,
            quantity : 2
        },
        {
            type : "Suv",
            price : 4500.00,
            quantity : 2
        },
        {
            type : "Van",
            price : 4500.00,
            quantity : 2
        }}
};
CarModel maseratiModel = {
    name : "Maserati",
    carStock : {
        {
            type : "Suv",
            price : 4500.00,
            quantity : 2
        },
        {
            type : "Coupes",
            price : 4500.00,
            quantity : 2
        },
        {
            type : "Convertibles",
            price : 4500.00,
            quantity : 2
        }}
};

int getSumOfArrayElements(struct CarModel *carModel)
{
    int sum = 0;
    int length = sizeof(carModel->carStock) / sizeof(carModel->carStock[0]);

    // Loop through the array to calculate sum of elements
    for (int i = 0; i < length; i++)
    {
        sum = sum + carModel->carStock[i].quantity;
    }
    return sum;
}

// calculate the total number of cars together
// print it to the customer later
int getTotalNumberOfCarsAvailable()
{
    return (
        getSumOfArrayElements(&bmwModel) +
        getSumOfArrayElements(&maseratiModel) +
        getSumOfArrayElements(&fiatModel));
}

// takes the quantity the use have chosen
// if desiredQuantity is greater than the Quantity of carModelTypeQuantity then prints an error
// get the CarModel from the structs by using a pointer to pinpoint the address
// create a carTypePosition
// pinpoint the type of car, the user has chosen
int handleCarModelAvailability(CarModel *carModel, int carTypePosition, short userDesiredQuantity)
{
    short carModelTypeQuantity = (short)carModel->carStock[carTypePosition].quantity;

    if (carModelTypeQuantity < userDesiredQuantity)
    {
        printf("\nQuantity selected of %s %s is greater than current stock of: %hd",
               carModel->name, carModel->carStock[carTypePosition].type, carModelTypeQuantity);

        return 0;
    }
    else
    {
        carModel->carStock[carTypePosition].quantity = carModelTypeQuantity - userDesiredQuantity;
    }
    return 1;
}

// print the CarModel
void printCarModelNames()
{
    printf("\nThese are the car models we have: ");

    int numberOfCarModels = sizeof(carModels) / sizeof(carModels[0]);
    printf("\nCar Models:\n");
    for (int i = 0; i < numberOfCarModels; i++)
    {
        printf("%d.\t%s\n", i + 1, carModels[i]);
    }
}

// Customer select the car model they want
int selectCarModel()
{
    if (attempt == MAX_ATTEMPTS)
    {
        return -1;
    }

    int carModelPosition = 0;
    printf("\nselect a car model above and press enter\n");
    scanf("%d", &carModelPosition);

    if (carModelPosition < 4)
    {
        return carModelPosition;
    }

    else
    {
        attempt++;
        return selectCarModel();
    }
}

// show the type of carModel
void showCarModelTypes(CarModel *carModel)
{

    printf("\n========================%s===========================\n", carModel->name);
    int length = (sizeof(carModel->carStock) / sizeof(carModel->carStock[0]));
    for (int i = 0; i < length; i++)
    {
        if (!strcmp(carModel->carStock[i].type, ""))
        {
            continue;
        }

        printf("No %d.| Type: %s | Price: %.2f | Quantity: %hd\n",
               i + 1, carModel->carStock[i].type,
               carModel->carStock[i].price, carModel->carStock[i].quantity);
    }
}

/*show car Model Types by position   */
CarModel *showCarModelTypesByPosition(int carModelPosition)
{
    switch (carModelPosition)
    {
    case 1:
        showCarModelTypes(&fiatModel);
        return &fiatModel;
    case 2:
        showCarModelTypes(&bmwModel);
        return &bmwModel;
    case 3:
        showCarModelTypes(&maseratiModel);
        return &maseratiModel;

    default:
        break;
    }
}

// Ask the customer to select a car type
int selectCarModelTypePosition()
{
    if (attempt == MAX_ATTEMPTS)
    {
        return -1;
    }

    int carModelTypePosition = 0;
    printf("\nselect one of the car types above and press enter\n");
    scanf("%d", &carModelTypePosition);

    if (carModelTypePosition < 4)
    {
        return carModelTypePosition - 1;
    }

    else
    {
        attempt++;
        return selectCarModelTypePosition();
    }
}

// with the idea of quantity
// ask the customer to specifer how many car of that model they want(within the specified range)
short selectDesiredPurchaseQuantity()
{
    short desiredPurchaseQuantity = 0;
    printf("\nselect desired quantity\n");
    scanf("%hd", &desiredPurchaseQuantity);
    return desiredPurchaseQuantity;
}

bool shouldApplyDiscount()
{
    printf("\nHow old are you? Age: ");
    // TODO: REMOVE TO ANOTHER FUNCTION so no global variable is accessed
    scanf("%hd", &customerAge);

    if (customerAge < MIN_PURCHASE_AGE || customerAge > MAX_PURCHASE_AGE)
    {
        printf("Purchase cannot proceed because you do not meet our eligibility criteria\n"
               "Must be within %d and %d years of age",
               MIN_PURCHASE_AGE, MAX_PURCHASE_AGE);
        return FALSE;
    }

    giveDiscount = FALSE;

    if (customerAge >= DISCOUNT_MIN_AGE && customerAge <= DISCOUNT_MAX_AGE)
    {
        giveDiscount = TRUE;
    }
    else
    {
        attempt = 0;
        char driversLicence;
        do
        {
            printf("\nDo you have a drivers licence? Answer 'y' or 'n': ");
            scanf("\n%c", &driversLicence);

            if (driversLicence == YES || driversLicence == NO)
            {
                break;
            }
            else
            {
                printf("A valid menu option was not selected, please try again.\n\n");
                attempt++;
            }
        } while (attempt < MAX_ATTEMPTS);

        if (attempt == MAX_ATTEMPTS)
        {
            return FALSE;
        }
    }

    return giveDiscount;
}

// calculate the discount with the finalPrice (if the customer gets a discount)
float calculateFinalTotalPrice(bool canGiveDiscount, float preDiscountTotPrice)
{
    float totalPrice = preDiscountTotPrice;

    switch (canGiveDiscount)
    {
    case TRUE:
        return totalPrice *= (1 - DISCOUNT_PERCENTAGE);
        printf("\nYou get a discount!");
        break;
    case FALSE:
        printf("\nSorry, No discount given.");
        break;
    }

    return totalPrice;
}

// Reset attempts each time the customer completes the right input
void resetAttempts()
{
    attempt = 0;
}

int getSaleId()
{
    time_t seconds;
    return time(&seconds);
}

void pauseProgram(char userChoice)
{
    if (userChoice == MENU_OPTION_EXIT)
    {
        printf("\n\nPress Enter to Exit...");
    }
    else
    {
        printf("\n\nPress Enter to return to the Menu...");
    }

    if (userChoice != '_')
    {
        getchar();
    }

    getchar();
}

// File starts
FILE *createFile(char fileName[201])
{
    file = fopen(fileName, "w");
    if (file != NULL)
    {
        fclose(file);
    }
    else
    {
        return file;
    }
}

void openFile(char fileName[201], char mode[4])
{
    file = fopen(fileName, mode);

    if (file == NULL)
    {

        if (createFile(fileName) == NULL)
        {
            fileStatus = FILE_ERROR;

            printf("Error opening %s: %s", fileName, strerror(errno));
        }
        else
        {

            openFile(fileName, mode);
        }
    }
    else
    {
        fileStatus = FILE_OPENED;
    }
}

// should be called once the operations on
// the currently opened file have concluded
void closeFile()
{
    // only attempt to close the file if it's already open
    if (fileStatus == FILE_OPENED)
    {
        fclose(file);
        fileStatus = FILE_CLOSED;
    }
}

// will return an array of strings representing the
// customer names stored in the file
void readDataFromFile()
{
    SalesItem *salesItemPtr, salesItem;
    salesItemPtr = &salesItem;
    int lineCounter = 0;

    while (1)
    {

        int scanResult = fscanf(
            file,
            "%s,%s,%s,%f,%f,%d,%hd,%hd,%d",
            &salesItemPtr->carModelName,
            &salesItemPtr->carType,
            &salesItemPtr->customerName,
            &salesItemPtr->carPrice,
            &salesItemPtr->totalPrice,
            &salesItemPtr->saleId,
            &salesItemPtr->customerAge,
            &salesItemPtr->purchasedQuantity,
            &salesItemPtr->isDiscounted);

        salesData[numberOfSales] = salesItemPtr;
        // if we reached the end of the file
        if (scanResult == EOF)
        {
            // then, stop the loop
            break;
        }

        lineCounter++;
    }

    numberOfSales = lineCounter;
}

void getDataFromFile()
{
    openFile(CSV_FILE, "r");

    if (fileStatus == FILE_OPENED)
    {
        readDataFromFile();
    }
    else if (fileStatus == FILE_ERROR)
    {
        printf("There was an error trying to read from the file %s.", CSV_FILE);
        // this function requires a char value, so we give it one
        // that'll tell it we're using it because of a file error
        // see the function body, it's been updated to check for
        // this underscore character
        pauseProgram('_');
    }

    closeFile();
}

void writeDataToFile()
{
    // loop through every sale
    for (int i = 0; i < numberOfSales; i++)
    {

        // this string will be built up bit by bit before being written to the opened file
        char line[500] = "";
        // this string will contain the converted int value
        char data[50];

        strcat(line, salesData[i]->carModelName);
        strcat(line, ",");

        strcat(line, salesData[i]->carType);
        strcat(line, ",");

        strcat(line, salesData[i]->customerName);
        strcat(line, ",");

        sprintf(data, "%f", salesData[i]->carPrice);
        strcat(line, data);
        strcat(line, ",");

        sprintf(data, "%f", salesData[i]->totalPrice);
        strcat(line, data);
        strcat(line, ",");

        sprintf(data, "%d", salesData[i]->saleId);
        strcat(line, data);
        strcat(line, ",");

        sprintf(data, "%hd", salesData[i]->customerAge);
        strcat(line, data);
        strcat(line, ",");

        sprintf(data, "%d", (int)salesData[i]->purchasedQuantity);
        strcat(line, data);
        strcat(line, ",");

        sprintf(data, "%d", salesData[i]->isDiscounted);
        strcat(line, data);
        strcat(line, ",");

        fprintf(file, line);

        // only add a newline character if we're not yet writing the very last
        // line in the file
        if (i < numberOfSales - 1)
        {
            fprintf(file, "\n");
        }
    }
}

void saveDataToFile()
{
    openFile(CSV_FILE, "w+");

    if (fileStatus == FILE_OPENED)
    {
        writeDataToFile();
    }
    else if (fileStatus == FILE_ERROR)
    {
        printf("There was an error trying to write to the file %s.", CSV_FILE);
        // this function requires a char value, so we give it one
        // that'll tell it we're using it because of a file error
        // see the function body, it's been updated to check for
        // this underscore character
        pauseProgram('_');
    }

    closeFile();
}
// File End

void menu_exit()
{
    printf("Thank You for using Ciao Car Shop. CIAO");

    saveDataToFile();
}

int main()
{

    getDataFromFile();
    do
    {
        system("cls");
        printf("================Welcome to Ciao Car Shop!==========================\n\n");
        printf("Menu:\n");
        printf("a. Buy Car\n");
        printf("b. View Sales\n");
        printf("c. View Stocks\n");
        printf("x. Exit\n\n");
        printf("Please choose one: ");

        userInputChoice = getchar();
        system("cls");

        switch (userInputChoice)
        {
        case MENU_OPTION_BUY_CAR:
        {
            resetAttempts();

            SalesItem *salesItemPtr, salesItem;
            salesItemPtr = &salesItem;
            int carsAvailable = getTotalNumberOfCarsAvailable();

            if (carsAvailable == 0)
            {
                printf("Sorry, there are no more cars available.");
                break;
            }

            printf("There are %d cars available.\n\n", carsAvailable);

            printf("What is your name? Name: ");
            scanf("\n%[^\n]s", &salesItemPtr->customerName);
            printCarModelNames();

            resetAttempts();
            int userSelectedCarModelPosition = selectCarModel();

            if (userSelectedCarModelPosition < -1)
            {
                break;
            }

            resetAttempts();
            CarModel *carModel = showCarModelTypesByPosition(userSelectedCarModelPosition);

            strcpy(salesItemPtr->carModelName, carModel->name);

            resetAttempts();
            int modelTypePosition = selectCarModelTypePosition();
            short desiredPurchaseQuantity = selectDesiredPurchaseQuantity();
            handleCarModelAvailability(carModel, modelTypePosition, desiredPurchaseQuantity);

            strcpy(salesItemPtr->carType, carModel->carStock[modelTypePosition].type);
            salesItemPtr->purchasedQuantity = desiredPurchaseQuantity;
            salesItemPtr->carPrice = carModel->carStock[modelTypePosition].price;

            float preDisCountotalPrice = carModel->carStock[modelTypePosition].price * desiredPurchaseQuantity;

            resetAttempts();
            bool isDiscounted = shouldApplyDiscount();
            salesItemPtr->totalPrice = calculateFinalTotalPrice(isDiscounted, preDisCountotalPrice);
            salesItemPtr->isDiscounted = isDiscounted;
            salesItemPtr->customerAge = customerAge;
            salesItemPtr->saleId = getSaleId();

            salesData[numberOfSales] = salesItemPtr;

            printf("\n\nThank you.\n");
            printf("You have bought %d cars.\n", salesItemPtr->purchasedQuantity);
            printf("Total cost is %.2f GBP.\n", salesItemPtr->totalPrice);

            numberOfSales++;
            break;
        }

        case MENU_OPTION_VIEW_SALES:
        {

            printf("All Sales Data:\n\n");

            float totalCarSalesValue = 0;
            unsigned short totalCarSold = 0;

            for (int i = 0; i < numberOfSales; i++)
            {
                totalCarSalesValue += salesData[i]->totalPrice;
                totalCarSold += salesData[i]->purchasedQuantity;
                char discountGivenText[4] = "YES";

                if (salesData[i]->isDiscounted == FALSE)
                {
                    strcpy(discountGivenText, "NO");
                }

                printf("Sale Index: %d | Sale ID: %d | Sale Amount: %f | Model: %s | Type: %s | "
                       "Car Price: %f | Number of Cars: %d | "
                       "Discount Given: %s | Customer Name: %s\n",
                       i, salesData[i]->saleId, salesData[i]->totalPrice,
                       salesData[i]->carModelName, salesData[i]->carType,
                       salesData[i]->carPrice, salesData[i]->purchasedQuantity,
                       discountGivenText, salesData[i]->customerName);
            }

            printf("\n%d cars have been sold with a total value of %f GBP.\n",
                   totalCarSold, totalCarSalesValue);

            break;
        }

        case MENU_OPTION_VIEW_STOCKS:
        {
            printCarModelNames();
        }

        case MENU_OPTION_EXIT:
        {

            menu_exit();
            break;
        }
        }

        if (userInputChoice == MENU_OPTION_EXIT)
        {
            printf("\n\nPress Enter to Exit...");
        }
        else
        {
            printf("\n\nPress Enter to return to the Menu...");
        }

        getchar();
        getchar();

    } while (userInputChoice != MENU_OPTION_EXIT);

    system("cls");

    printf("\n\nHave a good day!\n\n");
}