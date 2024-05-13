#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

// Random floating-point number generation for testing
double fRand(double min, double max)
{
    double ratio = (double)(rand()) / RAND_MAX;
    return min + ratio * (max - min);
}

// Roounds down a given floating-point number with n decimal places
double fRoundDown(double x, int n)
{
    if (n <= 0) return (int)x;
    double rounded = (int)(x * pow(10, n)) / pow(10, n);
    if (abs(x - rounded) > 1.0) return x;
    return rounded;
}


// Recursive function to calculate the determinant of any given NxN matrix
double det(vector<vector<double>> mat, int n)
{
    if (n == 1) return mat[0][0];
    if (n == 2) return (mat[0][0] * mat[1][1]) - (mat[0][1] * mat[1][0]);

    double determ = 0;

    for (int i = 0; i < n; i++)
    {
        vector<vector<double>> reduced(n - 1);
        for (int j = 1; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                if (k == i) continue;
                reduced[j - 1].push_back(mat[j][k]);
            }
        }
        double curr = mat[0][i] * det(reduced, n - 1);
        if (i % 2 != 0) curr *= -1;
        determ += curr;
    }

    return determ;
}

// Prompts the user to type the size of their square matrix; must be between 2 and 100 (inclusive).
int get_size()
{
    int size = -1;

    while (size < 2 || size > 9)
    {
        cout << "Please type the size of your NxN system of equations (2-9): ";
        cin >> size;
    }
        
    return size;
}

// struct to encode a system of equations. Consists of an NxN coefficient matrix and a Nx1 constants vector.
struct System
{
    System(int size, vector<vector<double>> coefficients, vector<double> constants) : size(size), coefficients(coefficients), constants(constants) { };
    int size;
    vector<vector<double>> coefficients;
    vector<double> constants;
};

// Prompts the user to type their system of equations
System get_system(int size)
{
    cout << "\nNote: All numbers will be rounded down to 3 decimal places to minimize floating-point arithmetic error.\n"
        << "Please type the coefficients of each row of your system, followed by the constant:\n\n";

    vector<vector<double>> coefficients(size);
    vector<double> constants(size);
    double curr;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cin >> curr;
            curr = fRoundDown(curr, 3);
            coefficients[i].push_back(curr);
        }

        cin >> constants[i];
        constants[i] = fRoundDown(constants[i], 3);
    }
    
    return System(size, coefficients, constants);
}

// Prints the system of equations to the screen
void print_sys(System sys)
{
    int max_len = -1, width(-1);
    for (int i = 0; i < sys.size; i++)
        for (int j = 0; j < sys.size; j++)
        {
            width = to_string((int)sys.coefficients[i][j]).length();
            if (width > max_len) max_len = width;
        }
    width += 5;
    
    for (int i = 0; i < sys.size; i++)
    {
        for (int j = 0; j < sys.size; j++)
        {
            if (j == 0 && sys.coefficients[i][j] < 0)
                cout << '-';

            cout << setw(width - (j == 0 && sys.coefficients[i][j] < 0)) << abs(sys.coefficients[i][j]) << (char)(97 + j);
            string sign;
            if (j != sys.size - 1)
                sign = sys.coefficients[i][j + 1] < 0 ? " - " : " + ";
            else
                sign = " = ";
            cout << sign;
        }
        cout << sys.constants[i] << endl;
    }
}

// Uses Cramer's rule to solve the given system of equations
vector<double> cramer_solve(System sys)
{
    vector<double> solutions;
    double coeff_det = det(sys.coefficients, sys.size);
    if (abs(coeff_det) < 1e-9) return solutions;

    for (int i = 0; i < sys.size; i++)
    {
        vector<vector<double>> curr = sys.coefficients;
        for (int j = 0; j < sys.size; j++)
        {
            curr[j][i] = sys.constants[j];
        }
        double curr_sol = det(curr, sys.size) / coeff_det;
        if (abs(curr_sol) < 1e-9) curr_sol = 0;
        solutions.push_back(fRoundDown(curr_sol, 5));
    }

    return solutions;
}

// Prints the solution to the given system of equations
void print_solution(System sys)
{
    vector<double> solutions = cramer_solve(sys);

    if (!solutions.size())
    {
        cout << "The system has no unique soluton.\n";
        return;
    }

    for (int i = 0; i < solutions.size(); i++)
        cout << (char)(97 + i) << " = " << solutions[i] << endl;
}

int main()
{
    cout.precision(3);
    cout.setf(ios::showpoint);
    cout.setf(ios::fixed);

    int size = get_size();
    System sys = get_system(size);
    
    // Uncomment the next segment for random testing

    //int size = 6;
    //vector<vector<double>> coeff(size);
    //vector<double> constants;

    //for (int i = 0; i < size; i++)
    //{
    //    for (int j = 0; j < size; j++) coeff[i].push_back(fRand(-50, 50));
    //    constants.push_back(fRand(-50, 50));
    //}

    //System sys(size, coeff, constants);
        
    

    cout << "\nYour system is:\n\n";
    print_sys(sys);

    cout.precision(5);
    cout << "\nThe solution of your system is as follows:\n\n";
    print_solution(sys);
    cout << endl;

    return 0;
}

// Exercise #25
// Please type the size of your NxN system of equations(2 - 9) : 4
//
// Note : All numbers will be rounded down to 3 decimal places to minimize floating - point arithmetic error.
// Please type the coefficients of each row of your system, followed by the constant :
//
// 3 -2 9 4 35 <-- input
// -1 0 -9 -6 -17 <-- input
// 0 0 3 1 5 <-- input
// 2 2 0 8 -4 <-- input
//
// Your system is :
//
// 3.000a - 2.000b + 9.000c + 4.000d = 35.000
// -1.000a + 0.000b - 9.000c - 6.000d = -17.000
// 0.000a + 0.000b + 3.000c + 1.000d = 5.000
// 2.000a + 2.000b + 0.000c + 8.000d = -4.000
//
// The solution of your system is as follows :
//
// a = 5.00000
// b = -3.00000
// c = 2.00000
// d = -1.00000

// Exercise #26
// Please type the size of your NxN system of equations(2 - 9) : 4
//
// Note : All numbers will be rounded down to 3 decimal places to minimize floating - point arithmetic error.
// Please type the coefficients of each row of your system, followed by the constant :
//
// -1 -1 0 1 -8
// 3 5 5 0 24
// 0 0 2 1 -6
// -2 -3 -3 0 -15
// 
// Your system is :
// 
// -1.000a - 1.000b + 0.000c + 1.000d = -8.000
// 3.000a + 5.000b + 5.000c + 0.000d = 24.000
// 0.000a + 0.000b + 2.000c + 1.000d = -6.000
// -2.000a - 3.000b - 3.000c + 0.000d = -15.000
// 
// The solution of your system is as follows :
// 
// a = 3.00000
// b = 7.00000
// c = -4.00000
// d = 2.00000