#ifndef DBMANAGER_H
#define DBMANAGER_H

#define OPERATION_SUCCESS 1
#define OPERATION_FAIL 0

/* Represents a book, contains the useful data of a book.
    All the data are strings except for the number of pages
    "numPages" which is an integer.*/
typedef struct {
    char* title;
    char* author;
    char* publisher;
    char* publicationDate;
    char* ISBN;
    char* genre;
    char* lang;
    int numPages;
} BookData;

/* Holds books and the number of books.*/
typedef struct {
    /* Holds the books and their associated data*/
    BookData** books;
    /* Holds the amount of books.
        @note count is used when calling freeBooks(books, count) so it is
            necessary to keep track of this number. */ 
    int count;
} BookArray;

/**
 * Creates a connection to the sqlite database. Also ensures the
 * default tables are created.
 * @returns OPERATION_SUCCESS if connection was successful and
 *          returns OPERATION_FAIL if failed to connect.
*/
int makeConnection(void);

/**
 * Inserts book data into the database.
 * @param data The BookData to be inserted
 * @returns OPERATION_SUCCESS if data was inserted successfully,
 *          else returns OPERATION_FAIL.
*/
int addBook(BookData data);

/**
 * Deletes a book from the database with a given id.
 * @param id The id of the book to be deleted from the database
 * @returns OPERATION_SUCCESS if the book was deleted else
 *          return OPERATOIN_FAIL. If id < 1 returns OPERATION_FAIL.
*/
int deleteBookById(int id);

/**
 * Gets all the books inside of the database. A row inside the database
 * table is represented by BookData, think of BookData as just a book.
 * Each book aka BookData has fields to represent it's data, this would
 * be the data inside the columns of each row. Examples of the data
 * a book would contain are the title, author, ISBN number, number of
 * pages, etc.
 * 
 * This function allocates memory for each book and it's fields, then puts
 * them into an array of books. Remember books are represented by the
 * BookData struct. This array is then stored inside the BookArray struct,
 * along with the number of books as an integer named count.
 * 
 * @returns A BookArray struct that contains an array of BookData which is retrieved
 *          from each row inside the database table. The struct also contains the
 *          count of books retrieved. If an error accures or memory cannot be
 *          allocated to store the books, the BookData array will be set to NULL
 *          and the count will be set to -1.
 * 
 * @note The memory allocated to the BookData array must be freed by calling
 *          freeBooks and passing the array along with the book count.
*/
BookArray getBooks(void);

/**
 * Frees all the memeory allocated to BookData and it's fields.
 * @param books The array of BookData that is created when calling getBooks().
 * @param numBooks The number of books that need to be freed.
*/
void freeBooks(BookData** books, int numBooks);

/**
 * Closes the connection to the sqlite database.
 * @returns OPERATION_SUCCESS if close was successful else
 *          returns OPERATION_FAIL if the database connection is NULL
 *          or if the database is unable to deallocate.
*/
int closeConnection(void);

#endif