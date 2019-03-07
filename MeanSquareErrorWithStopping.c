#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
int i = 16, j = 8, k = 10;
//calculating euclidian form of both errors
int calculateError(double ew1[][i+1], double ew2[][j+1])
{
    double error1 = 0, error2 = 0;
    int l, m;
    for(l = 1; l <= j; l++)
    {
        for(m = 1; m <= i; m++)
        {
            error1 += ew1[l][m]*ew1[l][m];
        }
    }
    for(l = 1; l <= k; l++)
    {
        for(m = 1; m <= j; m++)
        {
            error2 += ew2[l][m]*ew2[l][m];
        }
    }
    error1 = sqrt(error1);
    error2 = sqrt(error2);
    return (error1>error2)?(error1):error2;
}
//finding out sigmoid function
double sigmoid(double a)
{
    return (1/(1+exp(-a)));
}
//finding out derivative of sigmoid function
double derivSigmoid(double a)
{
    return(sigmoid(a)*(1 - sigmoid(a)));
}
//initiating backpropagation procedure
void backpropagation(int train[][17], int test[][17])
{
    int l, m, p, n, count = 0;
    double w1[i+1][j+1], w2[j+1][k+1], del1[j+1], del2[k+1], ew2[k+1][j+1], ew1[j+1][i+1], net1[j+1], net2[k+1], x[i+1], y[j+1], z[k+1], eeta=0.001;
    srand(time(NULL));
    //initialization of random weights for first layer weights
    for(l = 1; l <= i; l++)
    {
        for(m = 1; m <= j; m++)
        {
            w1[l][m] = (rand()%100 + 1)/100000.0;
        }
    }
    //initialization of random weights for second layer weights
    for(l = 1; l <= j; l++)
    {
        for(m = 1; m <= k; m++)
        {
            w2[l][m] = (rand()%100 + 1)/100000.0;
        }
    }
    //for all training sets
    for(l = 0;l < 2216;l++)
    {
        //no limited no. of epochs, so infinite loop
        for(;;)
        {
            //finding the net value after dot product with weights from input to hidden layer and applying activation function(sigmoid) on it
            for(m = 1; m <= j; m++)
            {
                net1[m] = 1.5;
                for(n = 1; n <= i; n++)
                {
                    net1[m] += train[l][n]*w1[m][n];
                }
                y[m] = sigmoid(net1[m]);
            }
            //finding the net value after dot product with weights from hidden to output layer and applying activation function(sigmoid) on it
            for(m = 1; m <= k; m++)
            {
                net2[m] = 0.5;
                for(n = 1; n <= j; n++)
                {
                    net2[m] += y[n]*w2[m][n];
                }
                z[m] = sigmoid(net2[m]);
            }
            //delta assignment with derivative of mean square loss
            for(m = 1; m <= k; m++)
            {
                if(train[l][0]==m)
                {
                    del2[m] = (z[m]-1)*derivSigmoid(net2[m]);
                }
                else
                {
                    del2[m] = (z[m])*derivSigmoid(net2[m]);
                }
            }
            //error for hidden to output layer weights
            for(m = 1; m <= k; m++)
            {
                for(n = 1; n <= j; n++)
                {
                    ew2[m][n] = eeta*del2[m]*y[n];
                }
            }
            //delta assignment for first to hidden layer
            for(m = 1; m <= j; m++)
            {
                del1[m] = 0;
                for(n = 1; n <= k; n++)
                {
                    del1[m] += w2[m][n]*del2[n]; 
                }
                del1[m] *= derivSigmoid(net1[m]);
            }
            //error for first to hidden layer weights
            for(m = 1; m <= j; m++)
            {
                for(n = 1; n <= i; n++)
                {
                    ew1[m][n] = eeta*train[l][n]*del1[m];
                }
            }
            //changing weights for hidden to output layer weights
            for(m = 1; m <= j; m++)
            {
                for(n = 1; n <= k; n++)
                {
                    w2[m][n] += ew2[n][m];
                }
            }
            //changing weights for first to hidden layer weights
            for(m = 1; m <= i; m++)
            {
                for(n = 1; n <= j; n++)
                {
                    w1[m][n] += ew1[n][m];
                }
            }
            //stopping criteria
            if(calculateError(ew1, ew2) < 0.01)
            {
                break;
            }
        }
    }
    //for all testing sets
    for(l = 0; l <= 998; l++)
    {
        //testing with first layer weights and applying activation(sigmoid) function
        for(m = 1; m <= j; m++)
        {
            net1[m] = 0;
            for(n = 1; n <= i; n++)
            {
                net1[m] += test[l][n]*w1[m][n];
            }
            y[m] = sigmoid(net1[m]);
        }
        //testing with second layer weights and applying activation(sigmoid) function
        for(m = 1; m <= k; m++)
        {
            net2[m] = 0;
            for(n = 1; n <= j; n++)
            {
                net2[m] += y[n]*w2[m][n];
            }
            z[m] = sigmoid(net2[m]);
        }
        //finding the class to with it belongs based on output
        double v = 0, w = 0;
        for(m = 1; m <= k; m++)
        {
            if(z[m] > v)
            {
                v = z[m];
                w = m;
            }
        }
        //increasing accuracy count
        if(w == test[l][0])
        {
            count++;
        }
    }
    printf("ACCURACY ON TEST FILE:%f%%\n", count*100/998.0);
}

int main()
{
    FILE *ftr = fopen("train1.csv", "r+");
    int ch[17];
    char str[100000];
    fscanf(ftr, "%[^\n]s",str);
    int train[2217][17];
    int c=0;
    while(fscanf(ftr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &ch[0], &ch[1], &ch[2], &ch[3], &ch[4], &ch[5], &ch[6], &ch[7], &ch[8], &ch[9], &ch[10], &ch[11], &ch[12], &ch[13], &ch[14], &ch[15], &ch[16]) >= 1)
    {
        memcpy(train[c], ch, sizeof(ch));
        //printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", train[c][0], train[c][1], train[c][2], train[c][3], train[c][4], train[c][5], train[c][6], train[c][7], train[c][8], train[c][9], train[c][10], train[c][11], train[c][12], train[c][13], train[c][14], train[c][15], train[c][16]);
        c++;
    }
    fclose(ftr);
    FILE *ttr = fopen("test.csv", "r+");
    fscanf(ttr, "%[^\n]s",str);
    int test[998][17];
    c = 0;
    while(fscanf(ttr, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &ch[0], &ch[1], &ch[2], &ch[3], &ch[4], &ch[5], &ch[6], &ch[7], &ch[8], &ch[9], &ch[10], &ch[11], &ch[12], &ch[13], &ch[14], &ch[15], &ch[16]) >= 1)
    {
        memcpy(test[c], ch, sizeof(ch));
        //printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", test[c][0], test[c][1], test[c][2], test[c][3], test[c][4], test[c][5], test[c][6], test[c][7], test[c][8], test[c][9], test[c][10], test[c][11], test[c][12], test[c][13], test[c][14], test[c][15], test[c][16]);
        c++;
    }
    backpropagation(train, test);
    return 0;
}