#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char name[50];
    float marks;
};

void addStudent(FILE *file, struct Student *student) {
    printf("Enter student name: ");
    scanf("%s", student->name);

    printf("Enter student ID: ");
    scanf("%d", &student->id);

    printf("Enter student marks: ");
    scanf("%f", &student->marks);

    fwrite(student, sizeof(struct Student), 1, file);
    printf("Student added successfully.\n");
}

void displayStudents(FILE *file) {
    fseek(file, 0, SEEK_SET); // Move the file pointer to the beginning

    struct Student student;

    printf("\n%-5s %-20s %-10s\n", "ID", "Name", "Marks");
    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        printf("%-5d %-20s %-10.2f\n", student.id, student.name, student.marks);
    }
}

int searchStudent(FILE *file, int searchId) {
    fseek(file, 0, SEEK_SET); // Move the file pointer to the beginning

    struct Student student;

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.id == searchId) {
            return 1; // Student found
        }
    }

    return 0; // Student not found
}

void modifyStudent(FILE *file, int searchId) {
    fseek(file, 0, SEEK_SET); // Move the file pointer to the beginning

    struct Student student;

    int found = 0;
    long pos;

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.id == searchId) {
            found = 1;
            pos = ftell(file) - sizeof(struct Student);
            break;
        }
    }

    if (found) {
        printf("Enter new student name: ");
        scanf("%s", student.name);

        printf("Enter new student marks: ");
        scanf("%f", &student.marks);

        fseek(file, pos, SEEK_SET); // Move the file pointer to the position of the found student
        fwrite(&student, sizeof(struct Student), 1, file);

        printf("Student information modified successfully.\n");
    } else {
        printf("Student not found.\n");
    }
}

void deleteStudent(FILE *file, int searchId) {
    FILE *tempFile = fopen("temp.dat", "wb");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        exit(1);
    }

    fseek(file, 0, SEEK_SET); // Move the file pointer to the beginning

    struct Student student;

    int found = 0;

    while (fread(&student, sizeof(struct Student), 1, file) == 1) {
        if (student.id == searchId) {
            found = 1;
        } else {
            fwrite(&student, sizeof(struct Student), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove("students.dat");
    rename("temp.dat", "students.dat");

    if (found) {
        printf("Student deleted successfully.\n");
    } else {
        printf("Student not found.\n");
    }
}

int main() {
    FILE *file = fopen("students.dat", "ab+");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    int choice;
    int searchId;

    do {
        printf("\nStudent Management System\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Modify Student Information\n");
        printf("4. Delete Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                struct Student newStudent;
                addStudent(file, &newStudent);
                break;
            }
            case 2:
                displayStudents(file);
                break;
            case 3:
                printf("Enter student ID to modify: ");
                scanf("%d", &searchId);
                modifyStudent(file, searchId);
                break;
            case 4:
                printf("Enter student ID to delete: ");
                scanf("%d", &searchId);
                deleteStudent(file, searchId);
                break;
            case 5:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);

    fclose(file);

    return 0;
}
