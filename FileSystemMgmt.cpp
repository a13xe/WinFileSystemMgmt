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
void FileTimestamps();


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
        puts("\033[1;32;40m 9 \033[0m- File timestamps");
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
            case 9:
                FileTimestamps();
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
        std::cout << "\n";
        std::cout << "Read-only: " << ((fileAttributes & FILE_ATTRIBUTE_READONLY) ? "Yes" : "No") << std::endl;
        std::cout << "Hidden: " << ((fileAttributes & FILE_ATTRIBUTE_HIDDEN) ? "Yes" : "No") << std::endl;
        std::cout << "Archive: " << ((fileAttributes & FILE_ATTRIBUTE_ARCHIVE) ? "Yes" : "No") << std::endl;
        std::cout << "System: " << ((fileAttributes & FILE_ATTRIBUTE_SYSTEM) ? "Yes" : "No") << std::endl;

        // Ask the user if they want to modify attributes
        char choice;
        std::cout << "\nDo you want to modify attributes? (Y/N): ";
        std::cin >> choice;

        if (choice == 'Y' || choice == 'y')
        {
            DWORD newAttributes = 0;

            std::cout << "Set Read-only (Y/N): ";
            std::cin >> choice;
            if (choice == 'Y' || choice == 'y') newAttributes |= FILE_ATTRIBUTE_READONLY;

            std::cout << "Set Hidden (Y/N): ";
            std::cin >> choice;
            if (choice == 'Y' || choice == 'y') newAttributes |= FILE_ATTRIBUTE_HIDDEN;

            std::cout << "Set Archive (Y/N): ";
            std::cin >> choice;
            if (choice == 'Y' || choice == 'y') newAttributes |= FILE_ATTRIBUTE_ARCHIVE;

            std::cout << "Set System (Y/N): ";
            std::cin >> choice;
            if (choice == 'Y' || choice == 'y') newAttributes |= FILE_ATTRIBUTE_SYSTEM;

            if (SetFileAttributes(fileName.c_str(), newAttributes)) std::cout << "File attributes modified successfully.\n";
            else std::cerr << "Error modifying file attributes. Error code: " << GetLastError() << std::endl;
        }
    }
    else std::cerr << "Error getting file attributes. Error code: " << GetLastError() << std::endl;

    // Pause and clear the console screen
    system("pause");
    system("cls");
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File Timestamps:
////////////////////////////////////////////////////////////////////////////////////////////////////////////


void FileTimestamps()
{
    system("cls");
    printf("\033[1;33;40m");
    puts("----------------------------------------------------");
    puts("File Timestamps:");
    puts("----------------------------------------------------");
    printf("\033[0m");
    
    std::string fileName;
    std::cout << "Enter file name: ";
    std::cin >> fileName;

    HANDLE hFile = CreateFile
    (
        fileName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE)
    {
        FILETIME creationTime, lastAccessTime, lastWriteTime;
        if (GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime))
        {
            SYSTEMTIME st;
            FileTimeToSystemTime(&creationTime, &st);
            std::cout << "Creation Time: " << st.wYear << "/" << st.wMonth << "/" << st.wDay << " " << st.wHour << ":" << st.wMinute << std::endl;

            // Ask if the user wants to modify file times
            char choice;
            std::cout << "Do you want to modify file times? (Y/N): ";
            std::cin >> choice;

            if (choice == 'Y' || choice == 'y')
            {
                SYSTEMTIME newTime;
                FILETIME newCreationTime, newAccessTime, newWriteTime;

                std::cout << "Enter new creation time (YYYY MM DD HH mm): ";
                std::cin >> newTime.wYear >> newTime.wMonth >> newTime.wDay >> newTime.wHour >> newTime.wMinute;
                newTime.wSecond = 0;
                newTime.wMilliseconds = 0;
                SystemTimeToFileTime(&newTime, &newCreationTime);

                std::cout << "Enter new modify time (YYYY MM DD HH mm): ";
                std::cin >> newTime.wYear >> newTime.wMonth >> newTime.wDay >> newTime.wHour >> newTime.wMinute;
                newTime.wSecond = 0;
                newTime.wMilliseconds = 0;
                SystemTimeToFileTime(&newTime, &newWriteTime);

                std::cout << "Enter new access time (YYYY MM DD HH mm): ";
                std::cin >> newTime.wYear >> newTime.wMonth >> newTime.wDay >> newTime.wHour >> newTime.wMinute;
                newTime.wSecond = 0;
                newTime.wMilliseconds = 0;
                SystemTimeToFileTime(&newTime, &newAccessTime);

                if (SetFileTime(hFile, &newCreationTime, &newAccessTime, &newWriteTime))
                {
                    std::cout << "File times modified successfully.\n";
                }
                else
                {
                    std::cerr << "Error modifying file times. Error code: " << GetLastError() << std::endl;
                }
            }
        }
        else
        {
            std::cerr << "Error getting file time information. Error code: " << GetLastError() << std::endl;
        }
        CloseHandle(hFile);
    }
    else
    {
        std::cerr << "Error opening file. Error code: " << GetLastError() << std::endl;
    }
}
