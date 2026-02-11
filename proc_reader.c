#include "proc_reader.h"

int list_process_directories(void) {
    // TODO: Open the /proc directory using opendir()
    // TODO: Check if opendir() failed and print error message
	DIR *dir = opendir("/proc");
	
	if(dir == NULL){
	perror("Error opening\n");
	return -1;
	}

    // TODO: Declare a struct dirent pointer for directory entries
    // TODO: Initialize process counter to 0
	
	struct dirent *entry;
	int processCounter = 0;

    printf("Process directories in /proc:\n");
    printf("%-8s %-20s\n", "PID", "Type");
    printf("%-8s %-20s\n", "---", "----");

    // TODO: Read directory entries using readdir() in a loop
    // TODO: For each entry, check if the name is a number using is_number()
    // TODO: If it's a number, print it as a PID and increment counter
	
	while((entry = readdir(dir)) != NULL){
	if(is_number(entry->d_name)){
	printf("PID: %-8s\n", entry->d_name);
	processCounter++;
       	}

	}

    // TODO: Close the directory using closedir()
    // TODO: Check if closedir() failed
	
	if(closedir(dir) == -1){
        perror("Error closing\n");
        return -1;
        }


    // TODO: Print the total count of process directories found
	printf("Directories found: %d\n",processCounter);

    return processCounter;
}

int read_process_info(const char* pid) {
    char filepath[256];

    // TODO: Create the path to /proc/[pid]/status using snprintf()

    printf("\n--- Process Information for PID %s ---\n", pid);

	snprintf(filepath, sizeof(filepath), "/proc/%s/status", pid);

    // TODO: Call read_file_with_syscalls() to read the status file
    // TODO: Check if the function succeeded
	
	if(read_file_with_syscalls(filepath) == -1){
	perror("Error reading.");
	return -1;
	}

    // TODO: Create the path to /proc/[pid]/cmdline using snprintf()
	
	snprintf(filepath, sizeof(filepath), "proc/%s/cmdline", pid);

    printf("\n--- Command Line ---\n");

    // TODO: Call read_file_with_syscalls() to read the cmdline file
    // TODO: Check if the function succeeded

	if(read_file_with_syscalls(filepath) == -1){
	perror("Error reading");
	return -1;
	}

    printf("\n"); // Add extra newline for readability
	printf("\n");

    return 0; // Replace with proper error handling
}

int show_system_info(void) {
    int line_count = 0;
    const int MAX_LINES = 10;

    printf("\n--- CPU Information (first %d lines) ---\n", MAX_LINES);

    // TODO: Open /proc/cpuinfo using fopen() with "r" mode
    // TODO: Check if fopen() failed
	
	FILE *file; 
	file = fopen("/proc/cpuinfo","r");
	
	if(file == NULL){
	perror("Failed opening");
	return -1;
	}

    // TODO: Declare a char array for reading lines
    // TODO: Read lines using fgets() in a loop, limit to MAX_LINES
    // TODO: Print each line
    // TODO: Close the file using fclose()

	char line[256];
	while (fgets(line, sizeof(line), file) != NULL){
	printf("%s", line);
	line_count++;
	}
	fclose(file);

    printf("\n--- Memory Information (first %d lines) ---\n", MAX_LINES);

    // TODO: Open /proc/meminfo using fopen() with "r" mode
    // TODO: Check if fopen() failed
	
	 file = fopen("/proc/meminfo", "r");

	if(file == NULL){
	perror("Error opening");
	return -1;
	}

    // TODO: Read lines using fgets() in a loop, limit to MAX_LINES
    // TODO: Print each line
    // TODO: Close the file using fclose()

	while(fgets(line, sizeof(line), file) != NULL){
        printf("%s", line);
	line_count++;
        }
        fclose(file);


    return 0; // Replace with proper error handling
}

void compare_file_methods(void) {
    const char* test_file = "/proc/version";

    printf("Comparing file reading methods for: %s\n\n", test_file);

    printf("=== Method 1: Using System Calls ===\n");
    read_file_with_syscalls(test_file);

    printf("\n=== Method 2: Using Library Functions ===\n");
    read_file_with_library(test_file);

    printf("\nNOTE: Run this program with strace to see the difference!\n");
    printf("Example: strace -e trace=openat,read,write,close ./lab2\n");
}

int read_file_with_syscalls(const char* filename) {
    // TODO: Declare variables: file descriptor (int), buffer (char array), bytes_read (ssize_t)
	int fd;
	char buffer[1024];
	ssize_t bytes_read;


    // TODO: Open the file using open() with O_RDONLY flag
    // TODO: Check if open() failed (fd == -1) and return -1

	fd = open(filename, O_RDONLY);
	
	if(fd == -1){
	perror("Error opening");
	return -1;
	}

    // TODO: Read the file in a loop using read()
    // TODO: Use sizeof(buffer) - 1 for buffer size to leave space for null terminator
    // TODO: Check if read() returns > 0 (data was read)
    // TODO: Null-terminate the buffer after each read
    // TODO: Print each chunk of data read

	while((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0){
	buffer[bytes_read] = '\0';
	printf("%s", buffer);
	}
	close(fd);

    // TODO: Handle read() errors (return value -1)
    // TODO: If read() fails, close the file and return -1

	if (bytes_read  == -1){
	perror("Error reading");
	return -1;
	}

	
    // TODO: Close the file using close()
    // TODO: Check if close() failed

	if(close(fd) == -1){
	perror("Error closing");
	return -1;
	}

    return 0; // Replace with proper error handling
}

int read_file_with_library(const char* filename) {
    // TODO: Declare variables: FILE pointer, buffer (char array)

	FILE *pointer;
	char buffer[1024];

    // TODO: Open the file using fopen() with "r" mode
    // TODO: Check if fopen() failed and return -1

	FILE *file = fopen(filename, "r");
	
	if(file == NULL){
	perror("Failed opening");
	return -1;
	}

    // TODO: Read the file using fgets() in a loop
    // TODO: Continue until fgets() returns NULL
    // TODO: Print each line read
	char line[256];
	while (fgets(line, sizeof(line), file) != NULL){
	printf("%s", line);
	}
	fclose(file);


    // TODO: Close the file using fclose()
    // TODO: Check if fclose() failed
	
	if(fclose(file) == -1){
	perror("Error closing file");
	return -1;
	}

    return 0; // Replace with proper error handling
}

int is_number(const char* str) {
    // TODO: Handle empty strings - check if str is NULL or empty
    // TODO: Return 0 for empty strings

	if (str == NULL || *str == '\0'){
	return 0;

    // TODO: Check if the string contains only digits
    // TODO: Loop through each character using a while loop
    // TODO: Use isdigit() function to check each character
    // TODO: If any character is not a digit, return 0
	
	while(*str !='\0'){
	
	if(!isdigit(*str)){
	return 0;;
	}
	str++;
	}
	

	return 1;
	}  
}
