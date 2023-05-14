#include <fstream>
#include <iostream>
#include <string>
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
    for (i = 0; i < rows && mat != NULL; i++)
    {
        for (j = 0; j < columns && mat != NULL; j++)
        {
            if (mat->i == i && mat->j == j)
            {
                cout << mat->data << ' ';
                mat = mat->next;
            }
            else
            {
                cout << 0 << ' ';
            }
        }
        if(j == columns) cout << endl;
    }

    while (j < columns) //если в конце строки нули
    {
        cout << 0 << ' ';
        j++;
    }
    cout << endl;
    
    while (i < rows) //если последние строки содержат одни нули
    {
        for (j = 0; j < columns; j++) cout << 0 << ' ';
        cout << endl;
        i++;
    }

    cout << "------------" << endl;
}

void print_mat(Matr mat, string filename)
{
    ofstream out(filename);

    int rows = mat->i;
    int columns = mat->j;
    int i = 0; int j = 0;
    mat = mat->next;
    for (i = 0; i < rows && mat != NULL; i++)
    {
        for (j = 0; j < columns && mat != NULL; j++)
        {
            if (mat->i == i && mat->j == j)
            {
                out << mat->data << ' ';
                mat = mat->next;
            }
            else
            {
                out << 0 << ' ';
            }
        }
        if (j == columns) out << endl;
    }

    while (j < columns) //если в конце строки нули
    {
        out << 0 << ' ';
        j++;
    }
    out << endl;

    while (i < rows) //если последние строки содержат одни нули
    {
        for (j = 0; j < columns; j++) out << 0 << ' ';
        out << endl;
        i++;
    }

    out.close();
}

Matr min_in_row(Matr mat, int row)
{
    if (row >= mat->i) cout << "Wrong row" << endl;
    Matr p = mat->next;
    for (int i = 0; i < row && p != NULL; i++) //пропускаю первые row-1 строк
    {
        for (int j = 0; j < mat->j && p !=NULL; j++)
        {
            if (p->i == i && p->j == j) p = p->next;
        }
    }

    Matr min = new elem;
    min->data = 0;
    min->i = row;
    min->j = 0;
    int j = 0;
    for (j = 0; j < mat->j && p!=NULL && p->i == row; j++)
    {
        if (p->data < min->data) min = p;
        if (p->j == j)
        {
            p = p->next;
        }
    }
    if (j != row && min->data == 0) min->j = j;
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
        while (p != NULL && max_elem->i == -1) //поиск строки, в которой ноль как максимальный элемент
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

int main()
{
    Matr mat = create_mat("matrix.txt");
    show_mat(mat);

    //Matr min = min_in_row(mat, 3);
    //cout << "Min=" << min->data << " " << "i=" << min->i << " " << "j=" << min->j << endl;

    //Matr max = max_in_column(mat, 3);
    //cout << "Max=" << max->data << " " << "i=" << max->i << " " << "j=" << max->j << endl;

    //print_mat(mat, "printed_matrix.txt");

}

