#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//В листьях n-арного дерева указаны некоторые числа, а во внутренних вершинах – знаки арифметических операций.
//Возможны одноместные операции типа '+' или '-'.В этом случае требуется только один операнд. Необходимо написать программу, рассчитывающую по
//такому дереву его значение.

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
        printf("Родительский узел не существует.\n");
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

        printf("Элемент добавлен в дочерний узел.\n");
    }
    else
    {
        printf("Неверный индекс дочернего узла.\n");
    }
}

void InsertElement(struct Node* root, const char* operation, int childIndex)
{
    if (root == NULL)
    {
        printf("Дерево не создано.\n");
        return;
    }

    // Если дочерних элементов нет, создаем массив
    if (root->children == NULL)
    {
        root->children = (struct Node**)malloc(sizeof(struct Node*));
        root->num_children = 0;
    }

    // Если массив еще не создан или индекс в пределах допустимого диапазона
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

            printf("Элемент добавлен в дерево.\n");
        }
    }
    else
    {
        printf("Неверный индекс дочернего узла.\n");
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
        printf("Неверный индекс для удаления.\n");
        return;
    }

    struct Node* deletedNode = parent->children[index];

    // Сдвигаем оставшиеся дочерние узлы
    for (int i = index; i < parent->num_children - 1; i++)
    {
        parent->children[i] = parent->children[i + 1];
    }

    parent->num_children--;

    // Проверяем, стал ли массив дочерних узлов пустым
    if (parent->num_children == 0)
    {
        free(parent->children);
        parent->children = NULL;
    }
    else
    {
        // Уменьшаем размер массива
        parent->children = (struct Node**)realloc(parent->children, parent->num_children * sizeof(struct Node*));
        if (parent->children == NULL)
        {
            printf("Ошибка realloc. Память не выделена.\n");
            exit(EXIT_FAILURE);
        }
    }

    DeleteTree(deletedNode);
    printf("Элемент удален.\n");
}

void DeleteChildNode(struct Node* parent, int childIndex)
{
    if (parent == NULL || childIndex < 0 || childIndex >= parent->num_children)
    {
        printf("Неверный индекс дочернего узла для удаления.\n");
        return;
    }

    struct Node* deletedNode = parent->children[childIndex];

    // Сдвигаем оставшиеся дочерние узлы
    for (int i = childIndex; i < parent->num_children - 1; i++)
    {
        parent->children[i] = parent->children[i + 1];
    }

    parent->num_children--;

    // Проверяем, стал ли массив дочерних узлов пустым
    if (parent->num_children == 0)
    {
        free(parent->children);
        parent->children = NULL;
    }
    else
    {
        // Уменьшаем размер массива
        parent->children = (struct Node**)realloc(parent->children, parent->num_children * sizeof(struct Node*));
        if (parent->children == NULL)
        {
            printf("Ошибка realloc. Память не выделена.\n");
            exit(EXIT_FAILURE);
        }
    }

    DeleteTree(deletedNode);
    printf("Дочерний узел удален.\n");
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
        printf("1 - Создать дерево\n");
        printf("2 - Вставка элемента\n");
        printf("3 - Вставка элемента в дочерний узел\n");
        printf("4 - Вычисление значения дерева\n");
        printf("5 - Удаление элемента\n");
        printf("6 - Удаление элемента в дочернем узле\n");
        printf("7 - Проверка на пустоту\n");
        printf("8 - Печать дерева\n");
        printf("0 - Выход\n");
        scanf("%d", &choice);
        system("cls");

        switch (choice) 
        {
        case 1:
            // Создание дерева
            if (root == NULL)
            {
                printf("Введите значение корневого элемента: ");
                scanf("%s", value);
                root = CreateNode(value);
                printf("Дерево создано.\n");
            }
            else
            {
                printf("Дерево уже существует.\n");
            }
            break;
        case 2:
            printf("Введите значение для вставки: ");
            scanf("%s", value);
            printf("Введите индекс для вставки: ");
            scanf("%d", &index);
            InsertElement(root, value, index);
            break;
        case 3:
            // Вставка элемента в дочерний узел
            printf("Введите значение для вставки: ");
            scanf("%s", value);
            printf("Введите индекс родительского узла: ");
            scanf("%d", &index);
            printf("Введите индекс для вставки: ");
            scanf("%d", &childIndex);
            InsertChildElement(root->children[index], value, childIndex);
            break;
        case 4:
            // Вычисление значения дерева
            if (IsEmpty(root)) {
                printf("Дерево пусто. Невозможно вычислить.\n");
            }
            else {
                double result = EvaluateTree(root);
                printf("Результат вычисления: %.2f\n", result);
            }
            break;
        case 5:
            // Удаление элемента
            printf("Введите индекс элемента для удаления: ");
            int index;
            scanf("%d", &index);
            DeleteNode(root, index);
            break;
        case 6:
            printf("Введите индекс родительского узла для удаления дочернего узла: ");
            scanf("%d", &index);
            printf("Введите индекс дочернего узла для удаления: ");
            scanf("%d", &childIndex);
            if (root->num_children > index && root->children[index]->num_children > childIndex) {
                DeleteChildNode(root->children[index], childIndex);
            }
            else {
                printf("Неверные индексы для удаления дочернего узла.\n");
            }
            break;
        case 7:
            // Проверка на пустоту
            if (IsEmpty(root)) {
                printf("Дерево пусто.\n");
            }
            else {
                printf("Дерево не пусто.\n");
            }
            break;
        case 8:
            // Печать дерева
            printf("\nДерево выражения:\n");
            PrintTree(root, 0);
            break;
        case 0:
            break; // Выход из цикла
        default:
            printf("Неверный ввод. Пожалуйста, введите число от 0 до 7.\n");
            break;
        }

    } while (choice != 0);


    DeleteTree(root);

    return 0;
}
