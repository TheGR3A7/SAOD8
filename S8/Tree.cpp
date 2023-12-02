#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//� ������� n-������ ������ ������� ��������� �����, � �� ���������� �������� � ����� �������������� ��������.
//�������� ����������� �������� ���� '+' ��� '-'.� ���� ������ ��������� ������ ���� �������. ���������� �������� ���������, �������������� ��
//������ ������ ��� ��������.

struct Node
{
    char operation[10];
    struct Node** children;
    int num_children;
};

struct Node* CreateNode(const char* operation)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    strcpy(newNode->operation, operation);
    newNode->num_children = 0;
    newNode->children = NULL;
    return newNode;
}

void PushNode(struct Node* parent, struct Node* child)
{
    parent->num_children++;
    parent->children = (struct Node**)realloc(parent->children, parent->num_children * sizeof(struct Node*));
    parent->children[parent->num_children - 1] = child;
}

void InsertChildElement(struct Node* parent, const char* operation, int childIndex)
{
    if (parent == NULL)
    {
        printf("������������ ���� �� ����������.\n");
        return;
    }

    if (childIndex >= 0 && childIndex <= parent->num_children)
    {
        struct Node* newNode = CreateNode(operation);

        parent->num_children++;
        parent->children = (struct Node**)realloc(parent->children, parent->num_children * sizeof(struct Node*));

        if (childIndex < parent->num_children - 1)
        {
            for (int i = parent->num_children - 1; i > childIndex; i--)
            {
                parent->children[i] = parent->children[i - 1];
            }
        }

        parent->children[childIndex] = newNode;

        printf("������� �������� � �������� ����.\n");
    }
    else
    {
        printf("�������� ������ ��������� ����.\n");
    }
}

void InsertElement(struct Node* root, const char* operation, int childIndex)
{
    if (root == NULL)
    {
        printf("������ �� �������.\n");
        return;
    }

    // ���� �������� ��������� ���, ������� ������
    if (root->children == NULL)
    {
        root->children = (struct Node**)malloc(sizeof(struct Node*));
        root->num_children = 0;
    }

    // ���� ������ ��� �� ������ ��� ������ � �������� ����������� ���������
    if (root->children == NULL || (childIndex >= 0 && childIndex <= root->num_children))
    {
        if (childIndex < root->num_children)
        {
            InsertChildElement(root->children[childIndex], operation, 0);
        }
        else
        {
            struct Node* newNode = CreateNode(operation);

            root->num_children++;
            root->children = (struct Node**)realloc(root->children, root->num_children * sizeof(struct Node*));

            if (childIndex < root->num_children - 1)
            {
                for (int i = root->num_children - 1; i > childIndex; i--)
                {
                    root->children[i] = root->children[i - 1];
                }
            }

            root->children[childIndex] = newNode;

            printf("������� �������� � ������.\n");
        }
    }
    else
    {
        printf("�������� ������ ��������� ����.\n");
    }
}



int IsEmpty(struct Node* root)
{
    return root == NULL;
}

void DeleteTree(struct Node* root)
{
    if (root == NULL)
        return;

    for (int i = 0; i < root->num_children; i++)
    {
        DeleteTree(root->children[i]);
    }

    free(root->children);
    free(root);
}

void DeleteNode(struct Node* parent, int index)
{
    if (parent == NULL || index < 0 || index >= parent->num_children)
    {
        printf("�������� ������ ��� ��������.\n");
        return;
    }

    struct Node* deletedNode = parent->children[index];

    // �������� ���������� �������� ����
    for (int i = index; i < parent->num_children - 1; i++)
    {
        parent->children[i] = parent->children[i + 1];
    }

    parent->num_children--;

    // ���������, ���� �� ������ �������� ����� ������
    if (parent->num_children == 0)
    {
        free(parent->children);
        parent->children = NULL;
    }
    else
    {
        // ��������� ������ �������
        parent->children = (struct Node**)realloc(parent->children, parent->num_children * sizeof(struct Node*));
        if (parent->children == NULL)
        {
            printf("������ realloc. ������ �� ��������.\n");
            exit(EXIT_FAILURE);
        }
    }

    DeleteTree(deletedNode);
    printf("������� ������.\n");
}

void DeleteChildNode(struct Node* parent, int childIndex)
{
    if (parent == NULL || childIndex < 0 || childIndex >= parent->num_children)
    {
        printf("�������� ������ ��������� ���� ��� ��������.\n");
        return;
    }

    struct Node* deletedNode = parent->children[childIndex];

    // �������� ���������� �������� ����
    for (int i = childIndex; i < parent->num_children - 1; i++)
    {
        parent->children[i] = parent->children[i + 1];
    }

    parent->num_children--;

    // ���������, ���� �� ������ �������� ����� ������
    if (parent->num_children == 0)
    {
        free(parent->children);
        parent->children = NULL;
    }
    else
    {
        // ��������� ������ �������
        parent->children = (struct Node**)realloc(parent->children, parent->num_children * sizeof(struct Node*));
        if (parent->children == NULL)
        {
            printf("������ realloc. ������ �� ��������.\n");
            exit(EXIT_FAILURE);
        }
    }

    DeleteTree(deletedNode);
    printf("�������� ���� ������.\n");
}


double EvaluateTree(struct Node* root)
{
    if (root == NULL)
        return 0;

    if (root->num_children == 0)
    {
        return atoi(root->operation);
    }

    double result = EvaluateTree(root->children[0]);

    for (int i = 1; i < root->num_children; i++)
    {
        if (root->operation[0] == '+')
        {
            result += EvaluateTree(root->children[i]);
        }
        else if (root->operation[0] == '-')
        {
            result -= EvaluateTree(root->children[i]);
        }
    }

    return result;
}

void PrintTree(struct Node* root, int level)
{
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }

    printf("%s\n", root->operation);

    for (int i = 0; i < root->num_children; i++)
    {
        PrintTree(root->children[i], level + 1);
    }
}


int main()
{
    system("chcp 1251");

    struct Node* root = NULL;

    int choice;
    char value[10];
    int index = 0;
    int childIndex = 0;

    do {
        printf("1 - ������� ������\n");
        printf("2 - ������� ��������\n");
        printf("3 - ������� �������� � �������� ����\n");
        printf("4 - ���������� �������� ������\n");
        printf("5 - �������� ��������\n");
        printf("6 - �������� �������� � �������� ����\n");
        printf("7 - �������� �� �������\n");
        printf("8 - ������ ������\n");
        printf("0 - �����\n");
        scanf("%d", &choice);
        system("cls");

        switch (choice) 
        {
        case 1:
            // �������� ������
            if (root == NULL)
            {
                printf("������� �������� ��������� ��������: ");
                scanf("%s", value);
                root = CreateNode(value);
                printf("������ �������.\n");
            }
            else
            {
                printf("������ ��� ����������.\n");
            }
            break;
        case 2:
            printf("������� �������� ��� �������: ");
            scanf("%s", value);
            printf("������� ������ ��� �������: ");
            scanf("%d", &index);
            InsertElement(root, value, index);
            break;
        case 3:
            // ������� �������� � �������� ����
            printf("������� �������� ��� �������: ");
            scanf("%s", value);
            printf("������� ������ ������������� ����: ");
            scanf("%d", &index);
            printf("������� ������ ��� �������: ");
            scanf("%d", &childIndex);
            InsertChildElement(root->children[index], value, childIndex);
            break;
        case 4:
            // ���������� �������� ������
            if (IsEmpty(root)) {
                printf("������ �����. ���������� ���������.\n");
            }
            else {
                double result = EvaluateTree(root);
                printf("��������� ����������: %.2f\n", result);
            }
            break;
        case 5:
            // �������� ��������
            printf("������� ������ �������� ��� ��������: ");
            int index;
            scanf("%d", &index);
            DeleteNode(root, index);
            break;
        case 6:
            printf("������� ������ ������������� ���� ��� �������� ��������� ����: ");
            scanf("%d", &index);
            printf("������� ������ ��������� ���� ��� ��������: ");
            scanf("%d", &childIndex);
            if (root->num_children > index && root->children[index]->num_children > childIndex) {
                DeleteChildNode(root->children[index], childIndex);
            }
            else {
                printf("�������� ������� ��� �������� ��������� ����.\n");
            }
            break;
        case 7:
            // �������� �� �������
            if (IsEmpty(root)) {
                printf("������ �����.\n");
            }
            else {
                printf("������ �� �����.\n");
            }
            break;
        case 8:
            // ������ ������
            printf("\n������ ���������:\n");
            PrintTree(root, 0);
            break;
        case 0:
            break; // ����� �� �����
        default:
            printf("�������� ����. ����������, ������� ����� �� 0 �� 7.\n");
            break;
        }

    } while (choice != 0);


    DeleteTree(root);

    return 0;
}
