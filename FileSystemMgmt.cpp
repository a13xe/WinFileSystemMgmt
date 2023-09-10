#include <iostream>
#include <windows.h>


// Create function prototypes
void ListDrives();
void DiskInfo();
void CreateDirectory();
void DeleteDirectory();
void CreateFile();
void CopyFile();
void MoveFile();
void FileAttributes();


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main menu
////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
    int choice;
    while (true)
    {
        system("cls");
        printf("\033[1;33;40m");
        puts("====================================================");
        puts(" File System Mgmt ");
        puts("====================================================");
        printf("\033[0m");
        puts("\033[1;34;40mEnter the number of the action: \033[0m");
        puts("\033[1;32;40m 1 \033[0m- List drives");
        puts("\033[1;32;40m 2 \033[0m- Disk information");
        puts("\033[1;32;40m 3 \033[0m- Create directory");
        puts("\033[1;32;40m 4 \033[0m- Delete directory");
        puts("\033[1;32;40m 5 \033[0m- Create file");
        puts("\033[1;32;40m 6 \033[0m- Copy file");
        puts("\033[1;32;40m 7 \033[0m- Move file");
        puts("\033[1;32;40m 8 \033[0m- File attributes");
        puts("----------------------------------------------------");
        puts("\033[1;32;40m 0 \033[0m- to EXIT the program");
        puts("----------------------------------------------------");

        printf("\033[1;34;40mThe number: \033[0m");
        std::cin >> choice;
        switch (choice)
        {
            case 1:
                ListDrives();
                break;
            case 2:
                DiskInfo();
                break;
            case 3:
                CreateDirectory();
                break;
            case 4:
                DeleteDirectory();
                break;
            case 5:
                CreateFile();
                break;
            case 6:
                CopyFile();
                break;
            case 7:
                MoveFile();
                break;
            case 8:
                FileAttributes();
                break;
            case 0:
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// List Drives:
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void ListDrives()
{
    system("cls");
    printf("\033[1;33;40m");
    puts("----------------------------------------------------");
    puts("List Drives:");
    puts("----------------------------------------------------");
    printf("\033[0m\n");

    DWORD drives = GetLogicalDrives();
    for (char letter = 'A'; letter <= 'Z'; ++letter)
    {
        if (drives & 1)
        {
            std::cout << letter << " ";
        }
        drives >>= 1;
    }
    std::cout << "\n";
    std::cout << std::endl;
    system("pause");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Disk Information:
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DiskInfo()
{
    system("cls");
    printf("\033[1;33;40m");
    puts("----------------------------------------------------");
    puts("Disk Information:");
    puts("----------------------------------------------------");
    printf("\033[0m");

    char driveLetter;
    std::cout << "\nEnter drive letter (e.g., C): ";
    std::cin >> driveLetter;
    std::string driveRoot = std::string(1, driveLetter) + ":\\";
    DWORD driveType = GetDriveType(driveRoot.c_str());

    if (driveType == DRIVE_FIXED)
    {
        char volumeName[MAX_PATH];
        DWORD serialNumber;
        DWORD maxComponentLength;
        DWORD fileSystemFlags;
        std::cout << "\n";

        if (GetVolumeInformation(driveRoot.c_str(), volumeName, MAX_PATH, &serialNumber, &maxComponentLength, &fileSystemFlags, NULL, 0))
        {
            ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
            if (GetDiskFreeSpaceEx(driveRoot.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes))
            {
                std::cout << "Volume Name: " << volumeName << std::endl;
                std::cout << "Serial Number: " << serialNumber << std::endl;
                std::cout << "File System: " << ((fileSystemFlags & FILE_SUPPORTS_SPARSE_FILES) ? "NTFS" : "FAT32") << std::endl;
                std::cout << "Total Space: " << totalNumberOfBytes.QuadPart << " bytes\n";
                std::cout << "Free Space: " << totalNumberOfFreeBytes.QuadPart << " bytes\n";
            }
            else std::cerr << "Error getting disk free space information.\n";
        }
        else std::cerr << "Error getting volume information.\n";
    }
    else std::cerr << "The selected drive is not a fixed drive.\n";
    std::cout << "\n";
    system("pause");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create Directory:
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CreateDirectory()
{
    system("cls");
    printf("\033[1;33;40m");
    puts("----------------------------------------------------");
    puts("Create Directory:");
    puts("----------------------------------------------------");
    printf("\033[0m");

    std::string dirName;
    std::cout << "\nEnter directory name: ";
    std::cin >> dirName;

    if (CreateDirectory(dirName.c_str(), NULL)) std::cout << "\nDirectory created successfully.\n";
    else std::cerr << "\nError creating directory. Error code: " << GetLastError() << std::endl;
    system("pause");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Delete Directory:
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DeleteDirectory()
{
    system("cls");
    printf("\033[1;33;40m");
    puts("----------------------------------------------------");
    puts("Delete Directory:");
    puts("----------------------------------------------------");
    printf("\033[0m");

    std::string dirName;
    std::cout << "\nEnter directory name: ";
    std::cin >> dirName;

    if (RemoveDirectory(dirName.c_str())) std::cout << "Directory deleted successfully.\n";
    else std::cerr << "Error deleting directory. Error code: " << GetLastError() << std::endl;
    system("pause");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create File:
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CreateFile()
{
    system("cls");
    printf("\033[1;33;40m");
    puts("----------------------------------------------------");
    puts("Create File:");
    puts("----------------------------------------------------");
    printf("\033[0m");

    std::string fileName;
    std::cout << "\nEnter file name: ";
    std::cin >> fileName;

    HANDLE hFile = CreateFile(
        fileName.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE)
    {
        std::cout << "File created successfully.\n";
        CloseHandle(hFile);
    }
    else std::cerr << "Error creating file. Error code: " << GetLastError() << std::endl;
    system("pause");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copy File:
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CopyFile()
{
    system("cls");
    printf("\033[1;33;40m");
    puts("----------------------------------------------------");
    puts("Copy File:");
    puts("----------------------------------------------------");
    printf("\033[0m");

    std::string sourceFile, destFile;
    std::cout << "\nEnter source file name: ";
    std::cin >> sourceFile;
    std::cout << "\nEnter destination file name: ";
    std::cin >> destFile;

    if (CopyFile(sourceFile.c_str(), destFile.c_str(), FALSE)) std::cout << "File copied successfully.\n";
    else std::cerr << "Error copying file. Error code: " << GetLastError() << std::endl;
    system("pause");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Move File:
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MoveFile()
{
    system("cls");
    printf("\033[1;33;40m");
    puts("----------------------------------------------------");
    puts("Move File:");
    puts("----------------------------------------------------");
    printf("\033[0m");

    std::string sourceFile, destFile;
    std::cout << "\nEnter source file name: ";
    std::cin >> sourceFile;
    std::cout << "\nEnter destination file name: ";
    std::cin >> destFile;

    if (MoveFileEx(sourceFile.c_str(), destFile.c_str(), MOVEFILE_REPLACE_EXISTING)) std::cout << "File moved successfully.\n";
    else std::cerr << "Error moving file. Error code: " << GetLastError() << std::endl;
    system("pause");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File Attributes:
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FileAttributes()
{
    system("cls");
    printf("\033[1;33;40m");
    puts("----------------------------------------------------");
    puts("File Attributes:");
    puts("----------------------------------------------------");
    printf("\033[0m");

    std::string fileName;
    std::cout << "\nEnter file name: ";
    std::cin >> fileName;

    // Get file attributes
    DWORD fileAttributes = GetFileAttributes(fileName.c_str());

    if (fileAttributes != INVALID_FILE_ATTRIBUTES)
    {
        std::cout << "File Attributes for " << fileName << ":\n";

        // Check and display file attributes
        if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) std::cout << "Directory\n";
        else std::cout << "File\n";

        if (fileAttributes & FILE_ATTRIBUTE_READONLY) std::cout << "Read-only\n";
        else std::cout << "Not Read-only\n";

        if (fileAttributes & FILE_ATTRIBUTE_HIDDEN)std::cout << "Hidden\n";
        else std::cout << "Not Hidden\n";

        if (fileAttributes & FILE_ATTRIBUTE_SYSTEM) std::cout << "System\n";
        else std::cout << "Not System\n";

        // Modify file attributes if desired
        char choice;
        printf("\033[1;31;40m");
        puts("Do you want to modify file attributes (Y/N)?");
        printf("\033[0m");
        std::cin >> choice;

        if (choice == 'Y' || choice == 'y')
        {
            std::cout << "Enter new attributes (e.g., R for read-only, H for hidden, S for system):\n";
            std::string newAttributes;
            std::cin >> newAttributes;

            // Calculate new file attributes
            DWORD newFileAttributes = 0;
            if (newAttributes.find('R') != std::string::npos) newFileAttributes |= FILE_ATTRIBUTE_READONLY;
            if (newAttributes.find('H') != std::string::npos) newFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
            if (newAttributes.find('S') != std::string::npos) newFileAttributes |= FILE_ATTRIBUTE_SYSTEM;

            // Set the new attributes
            if (SetFileAttributes(fileName.c_str(), newFileAttributes)) std::cout << "File attributes modified successfully.\n";
            else std::cerr << "Error modifying file attributes. Error code: " << GetLastError() << std::endl;
        }
    }
    else std::cerr << "Error getting file attributes. Error code: " << GetLastError() << std::endl;

    // Pause and clear the console screen
    system("pause");
    system("cls");
}
