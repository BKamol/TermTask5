#include <fstream>
#include <iostream>
#include <string>
#include <assert.h>
using namespace std;

struct elem
{
    int i;
    int j;
    int data;
    elem* next;
};

typedef elem* Matr;

Matr create_mat(string filename)
{
    ifstream in(filename);

    Matr mat = new elem;
    in >> mat->i;
    in >> mat->j;
    mat->data = 0;

    Matr p = mat;
    int data;
    for (int i = 0; i < mat->i; i++)
    {
        for (int j = 0; j < mat->j; j++)
        {
            in >> data;
            if (data != 0)
            {
                p->next = new elem;
                p = p->next;
                p->i = i;
                p->j = j;
                p->data = data;
            }
        }
    }
    p->next = NULL;
    in.close();

    return mat;
}

void show_mat(Matr mat)
{
    int rows = mat->i;
    int columns = mat->j;
    int i = 0; int j = 0;
    mat = mat->next;
    cout << "------------" << endl;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            if (mat != NULL && mat->i == i && mat->j == j)
            {
                cout << mat->data << ' ';
                mat = mat->next;
            }
            else
            {
                cout << 0 << ' ';
            }
        }
        cout << endl;
    }
    cout << "------------" << endl;
}

void print_mat(Matr mat, string filename)
{
    ofstream out(filename);

    int rows = mat->i;
    int columns = mat->j;
    int i = 0; int j = 0;
    out << rows << ' ' << columns << endl;
    mat = mat->next;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns; j++)
        {
            if (mat != NULL && mat->i == i && mat->j == j)
            {
                out << mat->data << ' ';
                mat = mat->next;
            }
            else
            {
                out << 0 << ' ';
            }
        }
        out << endl;
    }
}

Matr min_in_row(Matr mat, int row)
{
    if (row >= mat->i) cout << "Wrong row" << endl;
    Matr p = mat->next;

    Matr min = new elem;
    min->data = 0;
    min->i = row;
    min->j = 0;

    int count_i = 0;

    while (p != NULL && p->i != row) p = p->next; //пропускаем все строки перед row

    if (p == NULL) return min; //если в строке одни нули

    min->data = p->data; //предполагаем, что первый ненулевой элемент строки минимальный
    min->j = p->j;
    while (p != NULL && p->i == row)
    {
        if (p->data < min->data)
        {
            min->j = p->j;
            min->data = p->data;
        }
        count_i++;
        p = p->next;
    }

    if (count_i < mat->j && min->data > 0) //если в строке есть хотя бы 1 ноль, и он минимальный
    {
        int j = 0;
        min->data = 0;
        min->j = -1;

        p = mat->next;
        while (p != NULL && min->j == -1) //поиск столбца, в котором ноль - минимальный элемент
        {
            if (p->i == row && p->j != j)
            {
                min->j = j;
            }
            if (p->i == row && p->j == j)
            {
                j++;
            }
            p = p->next;
        }
        if (p == NULL && min->j == -1)
        {
            min->j = j;
        }
    }

    return min;
}

Matr max_in_column(Matr mat, int column)
{
    if (column >= mat->j) cout << "Wrong column" << endl;
    int count_j = 0;
    Matr max_elem = new elem;
    max_elem->j = column;
    Matr p = mat->next;
    while (p != NULL && p->j != column) //поиск первого элемента столбца column
    {
        p = p->next;
    }
    if (p == NULL) //Если в j-ом столбце одни нули
    {
        max_elem->data = 0;
        max_elem->i = 0;
        return max_elem;
    }

    max_elem->i = p->i; //препдолагаем, что первый элемент столбца максимальынй
    count_j++;
    while (p != NULL)
    {
        if (p->j == column) //если встретился элемент нужного столбца
        {
            if (p->data > max_elem->data)
            {
                max_elem->data = p->data;
                max_elem->i == p->i;
            }
            count_j++;
        }
        p = p->next;
    }

    if (count_j < mat->i && max_elem->data < 0) //Если в столбце есть хотя бы один ноль и он максимальный
    {
        int i = 0;
        max_elem->data = 0;
        max_elem->i = -1;
        p = mat->next;
        while (p != NULL && max_elem->i == -1) //поиск строки, в которой ноль - максимальный элемент
        {
            if (p->j == column && p->i != i)
            {
                max_elem->i = i;
            }
            if (p->j == column && p->i == i)
            {
                i++;
            }
            p = p->next;
        }
    }
    return max_elem;
}

void delete_mat(Matr& mat)
{
    Matr p;
    while (mat != NULL)
    {
        p = mat;
        mat = mat->next;
        delete p;
    }
}

void replace_elem_after_min(Matr mat) //
{
    Matr min;
    Matr p;
    Matr q;
    
    for (int k = 0; k < mat->i; k++)
    {
        min = min_in_row(mat, k);
        if (min->j == mat->j-1) continue; 
        p = mat->next;
        for (int i = 0; i < min->i; i++) //пропускаю строки до минимального элемента (или последнего)
        {
            for (int j = 0; j < mat->j; j++)
            {
                if (p->i == i && p->j == j) p = p->next;
            }
        }
        for (int j = 0; j < min->j; j++) //перехожу к минимальному элементу или первому существующему до него
        {
            if (p->i == min->i && p->j == j && p->next != NULL && p->next->i == min->i) p = p->next;
        }

        if (min->data == 0 && min->j == 0) //маргинальный случай
        {
            q = mat;
            while (q->next != p) q = q->next;
            p = q;
        }

        cout << "min: " << min->data << ' ' << min->i << ' ' << min->j << endl;
        cout << "p: " << p->data << ' ' << p->i << ' ' << p->j << endl;

        if (p->next != NULL && p->next->i == min->i && p->next->j == min->j + 1) //следующий после минимального элемент ненулевой
        {
            p = p->next;
            p->data = p->j;
        }
        else //следующий элемент нулевой
        {
            q = p->next;
            p->next = new elem;
            p = p->next;
            p->data = min->j + 1;
            p->i = min->i;
            p->j = min->j + 1;
            p->next = q;
        }

        //show_mat(mat);
    }
}

void min_in_row_max_in_column(Matr mat) //минимальный в строке и максимальный в столбце элемент
{
    Matr min_ir; //минимальный в строке
    Matr max_ic; //максимальный в столбце
    Matr p = mat->next;
    while (p != NULL)
    {
        min_ir = min_in_row(mat, p->i);
        max_ic = max_in_column(mat, p->j);
        if (p->data == min_ir->data && p->data == max_ic->data) //минимальность в строке и максимальность в столбце
        {
            cout << "row=" << p->i << ' ' << "elem=" << ' ' << p->data << endl;
        }
        p = p->next;
    }
}

int main()
{
    Matr mat = create_mat("matrix.txt");
    show_mat(mat);

    //Matr min = min_in_row(mat, 2);
    //cout << "Min=" << min->data << " " << "i=" << min->i << " " << "j=" << min->j << endl;

    //Matr max = max_in_column(mat, 3);
    //cout << "Max=" << max->data << " " << "i=" << max->i << " " << "j=" << max->j << endl;

    //print_mat(mat, "printed_matrix.txt");

    //min_in_row_max_in_column(mat);

    replace_elem_after_min(mat);
    show_mat(mat);
}

