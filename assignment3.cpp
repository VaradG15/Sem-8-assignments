#include<iostream>
#include<vector>
#include<omp.h>

using namespace std;

long long arr_max(vector<long long> arr)
{
    long long maximum = arr[0];
    double start_time = omp_get_wtime();
    #pragma omp parallel for reduction(max : maximum)
    for(int i=0; i<arr.size(); i++) 
    {
        maximum = max(maximum, arr[i]);
    }
    double end_time = omp_get_wtime();

    cout<<"Maximum: " << maximum<<endl;
    cout<<"Time taken to calculate: " << (end_time - start_time) <<endl;
    return maximum;
}

long long arr_max_sequential(vector<long long> arr) 
{
    long long maximum = arr[0];
    double start_time = omp_get_wtime();
    for(int i=0; i<arr.size(); i++) 
    {
        maximum = max(maximum, arr[i]);
    }
    double end_time = omp_get_wtime();

    cout<<"Maximum: " << maximum << endl;
    cout<<"Time taken to calculate: " << (end_time - start_time) << endl;
    return maximum;
}

long long arr_min_sequential(vector<long long> arr)
{

    long long minimum = arr[0];
    double start = omp_get_wtime();
    for(int i=0; i<arr.size(); i++)
    {
        minimum = min(minimum, arr[i]);
    }
    double end = omp_get_wtime();

    cout<< "Minimum: " << minimum <<endl;
    cout<< "Time taken to calculate: " << end-start <<endl;
    return minimum;
}

long long arr_min_parallel(vector<long long> arr)
{
    long long minimum = arr[0];
    double start = omp_get_wtime();
    #pragma omp parallel for reduction(min : minimum)
    for(int i=0; i<arr.size(); i++)
    {
        minimum = min(minimum, arr[i]);
    }
    double end = omp_get_wtime();

    cout<< "Minimum: " << minimum <<endl;
    cout<< "Time taken to calculate: " << end-start <<endl;
    return minimum;
}

double avg_sequential(vector<long long> arr)
{
    long long sum = 0;
    double start = omp_get_wtime();
    for(int i=0; i<arr.size(); i++)
    {
        sum += arr[i];
    }
    double avg = sum/arr.size();
    double end = omp_get_wtime();

    cout<< "Average: " << avg <<endl;
    cout<< "Time taken to calculate: " << end-start <<endl;
    return avg;
}


double avg_parallel(vector<long long> arr)
{
    long long sum = 0;
    double start = omp_get_wtime();
    #pragma omp parallel for reduction(+ : sum)
    for(int i=0; i<arr.size(); i++)
    {
        sum += arr[i];
    }
    double avg = sum/arr.size();
    double end = omp_get_wtime();

    cout<< "Average: " << avg <<endl;
    cout<< "Time taken to calculate: " << end-start <<endl;
    return avg;
}

long long sum_sequential(vector<long long> arr)
{
    long long sum = 0;
    double start = omp_get_wtime();
    for(int i=0; i<arr.size(); i++)
    {
        sum += arr[i];
    }
    double end = omp_get_wtime();

    cout<< "Sum: " << sum <<endl;
    cout<< "Time taken to calculate: " << end-start <<endl;
    return sum;
}

long long sum_parallel(vector<long long> arr)
{
    long long sum = 0;
    double start = omp_get_wtime();
    #pragma omp parallel for reduction(+ : sum)
    for(int i=0; i<arr.size(); i++)
    {
        sum += arr[i];
    }
    double end = omp_get_wtime();

    cout<< "Sum: " << sum <<endl;
    cout<< "Time taken to calculate: " << end-start <<endl;
    return sum;
}

int main()
{
    long long N = 90000000;
    vector<long long> arr = {1, 2, 4, 5, 7, 2, 6, 0, 9, 76, 89, 78, 45, 56, 100, 56, 75, 13, 145, 900};

    for (long long i = 0; i < N; i++)
    {
        arr.push_back(rand() % 1000);
    }

    cout<<"Parallel Max:"<<endl;
    arr_max(arr);
    cout<<"Sequential Max:"<<endl;
    arr_max_sequential(arr);
    cout << endl;

    cout<<"Sequential Min:"<<endl;
    arr_min_sequential(arr);

    cout<<"Parallel Min:"<<endl;
    arr_min_parallel(arr);
    cout<<endl;

    cout<<"Sequential Sum:"<<endl;
    sum_sequential(arr);

    cout<<"Parallel Sum:"<<endl;
    sum_parallel(arr);
    cout<<endl;

    cout<<"Sequential Average:"<<endl;
    avg_sequential(arr);

    cout<<"Parallel Average:"<<endl;
    avg_parallel(arr);
    cout<<endl;


}