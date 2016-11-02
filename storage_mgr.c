#include<stdio.h>
#include<stdlib.h>
#include"storage_mgr.h"
#include"dberror.h"

FILE *file;
int pagetoberead;

////////////////////////////////////////////////////////////////
//Print if storage manager is initialized
////////////////////////////////////////////////////////////////
void initStorageManager()
{
	printf("Storage Manager Initialized\n");
}
////////////////////////////////////////////////////////////////
//Create new file of one page initialized with '\0'
////////////////////////////////////////////////////////////////
RC createPageFile(char *fname)
{	
	int i;
	char choice;
	file=fopen(fname,"r");
	if(file!=NULL)//Check if file is opened or not.
	{
		//if file opened, ask whether to overwrite or not
		printf("File already exists. Would you like to overwrite it?(y/n)");
		scanf("%c",&choice);
		fclose(file);
	}
	if(file==NULL || choice=='y')
	{	
		//if file is null or if choose to overwrite, fill the page wih '\0' bytes
		file=fopen(fname,"w");
		char *ptr=(char *)calloc(PAGE_SIZE,sizeof(char));//this ptr pointer will be of size 4096 with '\0' in it.
		fwrite(ptr,PAGE_SIZE,sizeof(char),file);//writing this page to the file.
		fseek(file,0,SEEK_END);//seeking to end of the file.
		free(ptr);//free the pointer ensuring no memory leak.
		fclose(file); //closing the file
		RC_message="File created successfully";
		return RC_OK;
	}
	else
	{
		//else return file already present and terminate.
		RC_message="File Already Present";
		return RC_FILE_ALREADY_PRESENT;
		
	}
	
} 
////////////////////////////////////////////////////////////////
//Open existing page file
////////////////////////////////////////////////////////////////
RC openPageFile(char *fname,SM_FileHandle *fHandle)
{
	
	file=fopen(fname,"r");
	int fPageCount,fSize;
	if(file == NULL)//Check if file is opened or not.
	{
		RC_message="File Not Found";
		return RC_FILE_NOT_FOUND;//display file not found if it doesn't exists
	}
	else
	{
		//else initialize file handle with the information of opened file
		file=fopen(fname,"r");
		fHandle->fileName=fname;
		fHandle->curPagePos=ftell(file)/PAGE_SIZE;//updating fhandle current page position to the beginning of file.
		fseek(file,0,SEEK_END);//seeking to end of file
		fSize=ftell(file);//getting file size 
		fPageCount=fSize/PAGE_SIZE;//counting the number of pages.
		fHandle->totalNumPages=fPageCount;
		fHandle->mgmtInfo=file;//updating file pointer in fhandle.
		RC_message="File Opened Successfully.";
		return RC_OK;
		
	}
}
////////////////////////////////////////////////////////////////
//Delete the page file
////////////////////////////////////////////////////////////////
RC destroyPageFile(char *fileName)
{
	if(fopen(fileName,"r") != NULL)//check if file is opened
	{
		remove(fileName);//if file is there, remove the file
		RC_message="File removed successfully.";
		return RC_OK;
	}	
	else
	{
		RC_message="File Not Found";
		return RC_FILE_NOT_FOUND;//else display file not found
	}
} 
////////////////////////////////////////////////////////////////
//Close the page file
////////////////////////////////////////////////////////////////
RC closePageFile(SM_FileHandle *fHandle)
{
	if(fHandle != NULL)//check if file handle is initialized
	{
		if(fopen(fHandle->fileName,"r") != NULL)//if file handle is initialized, close the file
		{
			if(fclose(fHandle->mgmtInfo) == 0)//check whether file is closed successfully.
			{
				RC_message="File closed successfully";
				return RC_OK;
			}
			else
			{
				RC_message="File Not Found.";
				return RC_FILE_NOT_FOUND;//return file not found
			}
		}
		else 
		{	
			RC_message="File Not Found.";
			return RC_FILE_NOT_FOUND;
		}
	}
	else 
	{
		RC_message="requested file related data is not initialized";
		return RC_FILE_HANDLE_NOT_INIT;//else display file handle not initialized
	}
}

////////////////////////////////////////////////////////////////
//Get current position of the block in the file
////////////////////////////////////////////////////////////////
int getBlockPos (SM_FileHandle *fHandle)
{
	if(fHandle != NULL)//check if the file handle is null
	{
		//if file handle is not null, open the file and then print and return the current position
		if((fopen(fHandle->fileName,"r")) != NULL)
		{
			printf("%d ",fHandle->curPagePos);
			return fHandle->curPagePos;
		}
		else
		{
			RC_message="File Not Found"; 
			return RC_FILE_NOT_FOUND;
		}
	}
	else
	{
		//if file handle is null return that file handle is not initialized with file info
		RC_message="desired file related data is not initialized";
		return RC_FILE_HANDLE_NOT_INIT;
	}
}

////////////////////////////////////////////////////////////////
// Read the blocks of page file
////////////////////////////////////////////////////////////////
RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle !=NULL)//check if the file handle is null
	{
		if(fopen(fHandle->fileName,"r") != NULL)
		{
			//if file handle is not null, start reading the file
			if((pageNum)>-1 && (pageNum+1)<=fHandle->totalNumPages)//check whether requested pageNum is smaller than total number of pages
			{
				if(memPage !=NULL)
				{
					fseek(fHandle->mgmtInfo,(((pageNum)*PAGE_SIZE)),SEEK_SET);//seeking to requested page number.
					fread(memPage,sizeof(char),PAGE_SIZE,fHandle->mgmtInfo);//reading to requested page.
					fHandle->curPagePos=pageNum;//updating current page position
					RC_message="File Read Successfull.";
					return RC_OK;
				}
			}
			else
			{
				RC_message="Requested page to be read doesn't exist";
				return RC_READ_NON_EXISTING_PAGE;
			}
		}
		else
		{
			RC_message="File Not Found";
			return RC_FILE_NOT_FOUND;
		}
	}
	else
	{
		RC_message="requested file related data is not initialized";
		return RC_FILE_HANDLE_NOT_INIT;
	}	
}

////////////////////////////////////////////////////////////////
//Read the current block of the file based on the current position of the block
////////////////////////////////////////////////////////////////
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=getBlockPos(fHandle);
	readBlock (pagetoberead,fHandle, memPage);
}

////////////////////////////////////////////////////////////////
//Read next block of the file based on the current position of the block
////////////////////////////////////////////////////////////////
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=getBlockPos(fHandle)+1;
	readBlock (pagetoberead,fHandle, memPage);
}
////////////////////////////////////////////////////////////////
//Read the previous block of the file based on the current position of the block
////////////////////////////////////////////////////////////////
RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=getBlockPos(fHandle)-1;//fetching current page position and then subtracting 1 to get previous page number.
	readBlock (pagetoberead,fHandle, memPage);
}

////////////////////////////////////////////////////////////////
//Read the first block of the file
////////////////////////////////////////////////////////////////
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=0;//setting first page number value to be read.
	readBlock(pagetoberead,fHandle, memPage);
}

////////////////////////////////////////////////////////////////
//Read last block of the file based on the current position of the block
////////////////////////////////////////////////////////////////
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	pagetoberead=(fHandle->totalNumPages)-1;
	readBlock (pagetoberead,fHandle, memPage);
}



////////////////////////////////////////////////////////////////
//Check if number of pages in file is less than the number of pages requested. if less than append pages.
//////////////////////////////////////////////////////////////// 
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
	//check if file handle is null and file can be opened or not
	if ((fHandle != NULL) && (fopen(fHandle->fileName,"r") != NULL))
	{
		//if number of pages passed less than number of pages in file handle, append required empty pages at the end of the file
		if (fHandle->totalNumPages < numberOfPages)
		{
			while (fHandle->totalNumPages != numberOfPages) 
			{
				appendEmptyBlock(fHandle);
			}
		}
		else
		{
			return RC_NUMBER_OF_PAGES_EXISTS;//else return number of pages exist in the file
		}
	}
	else
	{
		if (fHandle == NULL) 
		{		
			RC_message="Required file data not initialized";
			return RC_FILE_HANDLE_NOT_INIT;
		}
		else
		{
			RC_message="File not found;";
			return RC_FILE_NOT_FOUND;
		}
	}
		
}

////////////////////////////////////////////////////////////////
//Append empty page to the end of the file
////////////////////////////////////////////////////////////////
RC appendEmptyBlock (SM_FileHandle *fHandle)
{
	
	if ((fHandle != NULL) && (fopen(fHandle->fileName,"r") != NULL))//check if file handle is empty and if file can be opened
	{
		//if file handle is not null and file is there, append new page to the end of the file
		file = fopen(fHandle->fileName,"a");
		fHandle->totalNumPages+=1;//incrementing page number.
		fseek(file, (((fHandle->totalNumPages)*PAGE_SIZE)), SEEK_END);//seeking to particular page number.
		char *stream = (char *)calloc(PAGE_SIZE, sizeof(char));//creating a pointer of PAGE_SIZE
		fwrite(stream, PAGE_SIZE, sizeof(char), file);//writing it file
		free(stream);//releasing memory
		fclose(file);//closing the file
		RC_message="Block appended successfully";
		return RC_OK;
	}
	else
	{
		if (fHandle == NULL)  
		{
			RC_message="Requested file related data not initialized";
			return RC_FILE_HANDLE_NOT_INIT;//if the handle is null return that file handle is not initialized with file info
		}
		else
		{	
			RC_message="File not found.";
			return RC_FILE_NOT_FOUND;//if the file is not there, return that file can not be found
		}
	}
}

////////////////////////////////////////////////////////////////
//Update the current position and info of the file in the file handle when the block gets written
////////////////////////////////////////////////////////////////
RC UpdateFileStats(SM_FileHandle *fHandle)
{

    file = fopen(fHandle->fileName,"r");
    fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE);
    fseek(file, (fHandle->curPagePos)*PAGE_SIZE , SEEK_SET);
    fHandle->mgmtInfo = file;
    RC_message="Data write successful.";
	return RC_OK;
}

////////////////////////////////////////////////////////////////
//Write the current block of the file based on the current position of the block
////////////////////////////////////////////////////////////////
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//check if file handle is null
    if(fHandle != NULL)
    {
        if((fopen(fHandle->fileName,"r")) != NULL)
        {
                if((fHandle->curPagePos <= (fHandle->totalNumPages)))//check the current position of the block
                {
                    if(memPage != NULL)
                    {
						//if current position of the block is fetched and memPage is not null, write the memPage in to the file
                        writeBlock(fHandle->curPagePos, fHandle, memPage);
                    }

                }
                else { 
					RC_message="Requested page doesnt exists in file";
					return RC_WRITE_OUT_OF_BOUND_INDEX;
				}
        }
         else 
		{
			RC_message="File not found";
			return RC_FILE_NOT_FOUND;
		}
    }
    else 
	{
		RC_message="Requested File data not found;";
		return RC_FILE_HANDLE_NOT_INIT;
	}
}

////////////////////////////////////////////////////////////////
//Write the block of the file
////////////////////////////////////////////////////////////////
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//check if file handle is null
    if(fHandle != NULL)
    {
		//check if fill is there
        if((fopen(fHandle->fileName,"r")) != NULL)
        {
                if((pageNum <= (fHandle->totalNumPages)))//check if page number passed less than or equal to pages in the file handle
                {
                    if(memPage != NULL)
                    {
                            file = fopen(fHandle->fileName,"r+");
                            fseek(file, pageNum*PAGE_SIZE , SEEK_SET);
							//check if file is successfuly written with the memPage
                            if(fwrite(memPage,PAGE_SIZE,1,file) != 1)
                            { 
								RC_message="Write Request Failed";
								return RC_WRITE_FAILED;
							}
                            else
							{
								UpdateFileStats(fHandle);//if file is written, update the file handle
							}
                    }

                }
                else 
				{
					RC_message="Requested page doesnt exists.";
					return RC_WRITE_OUT_OF_BOUND_INDEX;
				}
        }
        else 
		{
			RC_message="File not found";
			return RC_FILE_NOT_FOUND;
		}
    }
    else 
	{
		RC_message="Requested File data not found;";
		return RC_FILE_HANDLE_NOT_INIT;
	}
}

