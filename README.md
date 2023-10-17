
# CLManager

C Library Manager is a application to manager a users favorite books. The application allows users to save, view, and delete books from a database. Furthermore the user can search an online api to save more detailed information about their favorite books. 




## Requirements
There are a few libraries that are required for the application to function properly. The first is the [sqlite3 C library](https://www.sqlite.org/download.html), the second is the [libcurl library](https://curl.se/windows/).

On the *sqlite* page under the **Precompiled Binaries for Windows** download the 64x bit dll. Also you will need the sqlite amalgamation under **Source Code**

On the *curl* page download the **CURL for 64-bit**
## Installation

Clone this project with Github CLI

```bash
  git clone https://github.com/IssiahB/CLManager.git
  cd CLManager
```
    
## Setup
With the libraries downloaded and the the project cloned, if you would like to work on the project do the following:s
#### Sqlite:
Extract the files in the sqlite amalgamation zip and copy the header files into the include/ folder of the project. Extract the files in the sqlite binaries zip and copy the .dll file into the bin/ folder of the project.
#### libcurl:
Extract the files in the curl zip, navigate to the bin/ folder of the extracted files, copy the .dll file into where your compiler is located this should be something like C:/MinGW/bin. Also create a folder in your MinGW/ directory called cert/ and place the .crt certificate inside. 

Next create a curl/ folder inside the include/ directory so that the path will look like: C:/MinGW/include/curl/ or whatever is equivalent. Now copy the header files from your extracted directory they should be located in a folder named include/, once copied place them in the curl/ directory created earlier.
## Author

- [@Issiah Banda](https://www.github.com/IssiahB)

