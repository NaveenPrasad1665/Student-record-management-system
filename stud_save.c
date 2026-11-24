#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

/* student.dat is a plain text file, one record per line:
 *     <roll> <percentage> <name>
 * Example:  1 85.50 Arun
 */

void stud_save(void)
{
    FILE *fp = fopen(DATA_FILE, "w");
    Student *p;
    int count = 0;
    if (!fp) { perror("Cannot open " DATA_FILE " for writing"); return; }

    for (p = head; p; p = p->next) {
        fprintf(fp, "%d %.2f %s\n", p->roll, p->percentage, p->name);
        count++;
    }
    fclose(fp);
    printf("%d record(s) saved to " DATA_FILE ".\n", count);
}

void stud_load(void)
{
    FILE *fp = fopen(DATA_FILE, "r");
    Student *tail = NULL, *n;
    char line[128];
    int count = 0;
    if (!fp) return;   /* first run: no file yet */

    while (fgets(line, sizeof line, fp)) {
        int roll;
        float pct;
        char name[NAME_LEN];

        /* %49[^\r\n] reads the rest of the line (name, spaces allowed) */
        if (sscanf(line, "%d %f %49[^\r\n]", &roll, &pct, name) != 3)
            continue;                       /* skip blank/bad lines */

        n = (Student *)malloc(sizeof(Student));
        if (!n) break;
        n->roll = roll;
        n->percentage = pct;
        strcpy(n->name, name);
        n->next = NULL;

        if (tail) tail->next = n; else head = n;
        tail = n;
        count++;
    }
    fclose(fp);
    if (count) printf("Loaded %d record(s) from " DATA_FILE ".\n", count);
}
