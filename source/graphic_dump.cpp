#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "classic_list.h"
#include "graphic_dump.h"

const int DumpFileNameAddedLength = 64;
const int CommandSize             = 28;

int listGraphicDump(List *list)
{
    assert(list);

    static int dumpNumber   = 1;
    int fileNameLength      = 0;

    char *fileName = createDumpFileName(dumpNumber, &fileNameLength);
    dumpNumber++;

    FILE* dumpFile  = fopen(fileName,  "w");

    writeListToDotFileArrangedNext(list, dumpFile);
    
    fclose(dumpFile);

    createListGraphicDumpPng(fileName,  fileNameLength);
    free(fileName);

    return EXIT_SUCCESS;
}

char *createDumpFileName(int fileNumber, int *fileNameLength)
{
    int numberLength = 0;
    int number = fileNumber;

    for (  ; number > 0; number /= 10, numberLength++) {}

    *fileNameLength = DumpFileNameAddedLength + numberLength;
    char *fileName  = (char *)calloc(*fileNameLength, sizeof(char));

    sprintf(fileName, "gr_dump/dump%d.dot", fileNumber);

    return fileName;
}

int createListGraphicDumpPng(char *fileName, int fileNameLength)
{
    assert(fileName);

    char *command = (char *)calloc(CommandSize + fileNameLength * 2, sizeof(char));
    sprintf(command, "dot %s -T png -o %s.png", fileName, fileName);

    system(command);
    free  (command);

    return EXIT_SUCCESS;
}

int writeListToDotFileArrangedNext(List *list, FILE *dumpFile)
{
    assert(list);
    assert(dumpFile);

    fprintf(dumpFile, "digraph\n{\n");
    fprintf(dumpFile, "rankdir = LR;\n\n");

    fprintf(dumpFile, "node [shape = Mrecord, color  = \"navy\", style = \"filled\"];\n");

    fprintf(dumpFile, "next [fillcolor = \"cornFlowerBlue\"];\n");
    fprintf(dumpFile, "prev [fillcolor = \"salmon\"];\n");

    fprintf(dumpFile, "node01 [label = \"list | head = %p | tail = %p\",", 
            listHead(list), listTail(list));
    fprintf(dumpFile, "fillcolor = \"#afeeee\", ];\n");

    fprintf(dumpFile, "node02 [label = \"list | head = %p | tail = %p\",", 
            listHead(list), listTail(list));
    fprintf(dumpFile, "fillcolor = \"#afeeee\", ];\n");

    int size = listSize(list);

    Node *current = listHead(list);
    for (int i = 0; i < size && current; i++)
    {
        const char *color = "#fff3e0";

        fprintf(dumpFile, 
                "node%p[label = \"%d | %p | data = %d | next = %p | prev = %p\", fillcolor = \"%s\"];\n",
                current, i, current, 
                listData(list, current), listNext(list, current), listPrev(list, current),
                color);

        current = listNext(list, current);
    }

    fprintf(dumpFile, "\nedge [color = \"cornFlowerBlue\"];\n\n");

    Node *head = listHead(list);
    if (head) fprintf(dumpFile, "node01 -> node%p;\n", head);
    else      fprintf(dumpFile, "node01 -> node02;\n");

    current = head;
    for (int i = 0; i < size && current; i++)
    {
        if (listNext(list, current) == 0)
        {
            fprintf(dumpFile, "node%p -> node02;\n", current);
        }
        else
        {
            fprintf(dumpFile, "node%p -> node%p;\n", current, listNext(list, current));
        }

        current = listNext(list, current);
    }

    fprintf(dumpFile, "\nedge [color = \"salmon\"];\n\n");

    Node *tail = listTail(list);
    if (tail) fprintf(dumpFile, "node02 -> node%p;\n", tail);
    else      fprintf(dumpFile, "node02 -> node01;\n"); 

    current = head;
    for (int i = 0; i < size && current; i++)
    {
        if (listPrev(list, current) == NULL)
        {
            fprintf(dumpFile, "node%p -> node01;\n", current);
        }
        else
        {
            fprintf(dumpFile, "node%p -> node%p;\n", current, listPrev(list, current));
        }

        current = listNext(list, current);
    }

    fprintf(dumpFile, "}\n");

    return EXIT_SUCCESS;
}
