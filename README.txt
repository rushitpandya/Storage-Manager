________________

Storage Manager
________________

Team Members
_____________

Rushit Pandya (Leader) - A20381916 - rpandya4@hawk.iit.edu
Subhdeep Roy - A20358508 - sroy7@hawk.iit.edu
Dutt Patel - A20380571 - dpatel106@hawk.iit.edu
Rumin Shah - A20369998 - rshah90@hawk.iit.edu
__________

File List
__________

README.txt
dberror.c
dberror.h
storage_mgr.c
storage_mgr.h
test_assign1_1.c
test_assign1_2.c
test_helper.h
Makefile
____

Aim
____

To implement a simple storage manager that consist of various functions that are capable of reading and writing blocks from file on disk to memory and from memory to file on disk respectively. There is fixed size (referred to as PAGE_SIZE) of blocks or pages for this implementation. In addition to reading and writing pages from a file, it provides methods for creating, opening, closing and destroying files. The storage manager has to maintain several types of information for an open file: The number of total pages in the file, the current page position (for reading and writing), the file name, and a POSIX file descriptor or FILE pointer.
_________________________

Installation Instruction
_________________________

1) Go to the path where the extracted files are present.
2) Run the below command: make
3) Run the below command for testing test_assign1.c: make run 
4) To remove object files, run following command: make clean 
5) We have provided additional test cases in file "test_assign1_2.c To run those test cases use the below command: ./test_assign1_2
_______________________

Functions Descriptions
_______________________

initStorageManager()
Indicates start or initialization of the storage manager.
_______________________

File Related Functions
_______________________

createPageFile()
This function asks user to overwrite a file if it already exists. If no file exists then a new file opens in write mode and write the first page with NULLs or if user choose to overwrite the file then the existing file will overwrite. The calloc function is used to fill the first page of the file with NULLs.
 
openPageFile()
This functions opens the file if exists or else throws an approriate error. This function assigns all the file information to an existing file handle used to maintain the file information.

destroyPageFile()
This function destroy or removes the existing file if it exists or else throws an appropriate error.

closePageFile()
This function closes an opened file if it exists or throws appropriate error.
_______________________

Read Related Functions
_______________________

readBlock()
This function reads a particular given block in the file and stores the content of the page or block into memory. It also check whether the given page number to be read is less than the total number of pages or else it throws an error. We used malloc function to assign the memory space for the read content. In case of any errors, appropriate errors are returned.

getBlockPos()
This function makes use of the file handle and gives the current block position using curPagePos member of fileHandle structure or else throws an appropriate error.

readFirstBlock()
This function gives the page number to be read as an input to readBlock() function. It then reads that block.

readPreviosBlock()
This function gives the page number to be read by getting current block position using getBlockPos() function and decrementing it by 1 as an input to readBlock() function. It then reads that block.

readCurrentBlock()
This function gives the page number to be read by getting current block position using getBlockPos() function as an input to readBlock() function. It then reads that block.

readNextBlock()
This function gives the page number to be read by getting current block position using getBlockPos() function and incrementing it by 1 as an input to readBlock() function. It then reads that block.

readLastBlock()
This function gives the page number to be read by getting total number of  pages using the fileHandle structure and decrementing it by 1 as an input to readBlock() function. It then reads that block.
________________________

Write Related Functions
________________________

writeBlock()
This function is used to write a given block or page by providing its page number. First it check whethers the file handle is initialized or not, then it checks if the given page number lies within the total number of pages, then it writes the content from the memory to the file and updates the file stats using the UpdateFileStats() function that we created additionally. In case of any errors, appropriate errors are returned.

writeCurrentBlock
This function is used to write the current block with the content stored in memory (memPage). It makes use of writeBlock() function to write the current block by passing the current position, the file handle structure and the content to be written. In case of any errors, appropriate errors are returned.

appendEmptyBlock()
This function is used to append one empty block to the file with NULLs and incrementing the total number of pages by 1. The calloc function is used in order to allocate a block equal to PAGE_SIZE in the file and filling it with NULLs. In case of any errors, appropriate errors are returned.
 
ensureCapacity()
This function is used to check if the file is able to accomodate the number of pages. If the total number of pages is less than the nuber of pages then appendEmptyBlock() function is called which will append required number of pages that can accomodate the desired number of pages. Note that the blocks are appended and are filled with NULLs. In case of any errors, appropriate errors are returned.
_____________________

Additional Functions
_____________________

UpdateFileStats()
This function is used to update the file related information in file handle.
_______________________

Additional Error Codes
_______________________

#define RC_NUMBER_OF_PAGES_EXISTS 5
When the total number of pages in the file is equal to the number of pages required than this error code will be thrown stating that the number of pages required already exists.

#define RC_WRITE_OUT_OF_BOUND_INDEX 6

When the fwrite() function is used to write a block that has its page number more than that of its total number of pages then this error is thrown stating that the block you are trying to write is out of the bounds of the file.

#define RC_FILE_ALREADY_PRESENT 7
When any function checks whether the file exists or not and if the file exists then this error is thrown.
_____________

Extra Credit
_____________

We have implemented an extra test case that is available in the file named "test_assign1_2.c". The description of that test case can be found in the Test Cases section below.
_________________

Additional Files
_________________

test_assign1_2.c
This file contains additional test cases for testing.
________________

No Memory Leaks
________________

All the test cases are tested with Valgrind tool and no memory leaks exists. 
___________

Test Cases
___________

testCreateOpenClose()
This test case is used to test the functions that creates a file, opens a file, destroys a file and close the file. It should return an error if a file that has already been destroyed is tried to be opened.

testSinglePageContent()
This test case is used to test the functions that are used to read first block and write a block. Firstly, a file is created and then opened. The first block is read and is expected to be empty since it is full of NULLs. Then a block is written in the file and it is read again. At last, the file is destroyed.

testAppendWriteRead()
This test case reads first block and then writes current block, reads last block and then reads next block. It should return an error since there is no block after the last block that is already read.
A page is appended and then the next block is read, the current block is written and the previous block is read and then a page is appended and the last page is read and capacity is ensured to any given number of pages. After this, a block is written and the file is destroyed and opened again. Now, it should throw an error since a file destroyed cannot be opened. 
