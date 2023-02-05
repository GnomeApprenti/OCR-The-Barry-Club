#include <stdlib.h>
#include <stdio.h>
#include <math.h>


//définition des différentes constantes

#define nb_inputs 2
#define nb_hidden_nodes 2
#define nb_outputs 1
#define nb_training_sets 4


//fonction sigmoide et sa dérivée

double sigmoide(double x)
{
    return 1 / (1 + exp(-x));
}
double dsigmoide(double x)
{
    return x * (1 - x);
}

//initialise les poids de manère aléatoire

double init_weights()
{
    return ((double)rand()) / ((double)RAND_MAX);
}


//mélange in tableau

void shuffle(int* array, size_t n)
{
    for (size_t i = 0; i < n-1; i++){
        size_t j = (i + rand() / (RAND_MAX / (n - 1) + 1))%nb_training_sets;

        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

//fonction main

int main(void)
{
    const double lr = 0.1f; //learning rate

    //initialisation de tous les poids

    double hidden_layer[nb_hidden_nodes];
    double output_layer[nb_outputs];

    double hidden_layer_bias[nb_hidden_nodes];
    double output_layer_bias[nb_outputs];

    double hidden_weights[nb_inputs][nb_hidden_nodes];
    double output_weights[nb_hidden_nodes][nb_outputs];


    double training_inputs[nb_training_sets][nb_inputs] =
    {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f}
    };

    double training_outputs[nb_training_sets][nb_outputs] =
    {
        {0.0f},
        {1.0f},
        {1.0f},
        {0.0f},
    };

    for(int i = 0; i<nb_inputs; i++){
        for(int j = 0; j < nb_hidden_nodes; j++){
            hidden_weights[i][j] = init_weights();
        }
    }


    for(int i = 0; i<nb_hidden_nodes; i++){
        for(int j = 0; j < nb_outputs; j++){
            output_weights[i][j] = init_weights();
        }
    }

    for(int i = 0; i<nb_outputs; i++){
        output_layer_bias[i] = init_weights();
    }

    int training_set_order[] = {0,1,2,3};

    int nb_epochs = 100000;


    //entrainement du reseau

    for (int epoch = 0; epoch < nb_epochs; epoch++){

        shuffle(training_set_order, nb_training_sets);

        for (int x = 0; x < nb_training_sets; x++){
            int i = training_set_order[x];

            //passage vers le neurone suivant

            for (int j = 0; j < nb_hidden_nodes; j++){
                double activation = hidden_layer_bias[j];

                for (int k = 0; k < nb_inputs; k++){
                    activation += training_inputs[i][k] * hidden_weights[k][j];
                }

                hidden_layer[j] = sigmoide(activation);
            }

            //passage vers l'output

            for (int j = 0; j < nb_outputs; j++){
                double activation = output_layer_bias[j];

                for (int k = 0; k < nb_hidden_nodes; k++){
                    activation += hidden_layer[k] * output_weights[k][j];
                }

                output_layer[j] = sigmoide(activation);
            }

            printf("Input: %g %g  Output: %g   Predicted Output: %g \n",
                    training_inputs[i][0], training_inputs[i][1],
                    output_layer[0], training_outputs[i][0]);


            //propagation arrière

            double delta_output[nb_outputs];
            for (int j = 0; j<nb_outputs; j++){
                double error = (training_outputs[i][j] - output_layer[j]);
                delta_output[j] = error * dsigmoide(output_layer[j]);
            }

            double delta_hidden[nb_hidden_nodes];
            for (int j = 0; j < nb_hidden_nodes; j++){
                double error = 0.0f;
                for(int k = 0; k<nb_outputs; k++){
                    error += delta_output[k] * output_weights[j][k];
                }

                delta_hidden[j] = error * dsigmoide(hidden_layer[j]);
            }

            //application des changements dans les poids de l'output

            for(int j = 0; j<nb_outputs; j++){
                output_layer_bias[j] += delta_output[j] * lr;
                for(int k = 0; k < nb_hidden_nodes; k++){
                    output_weights[k][j] += hidden_layer[k] * delta_output[j] * lr;
                }
            }

            //application des changements dans les poids du hidden layer

            for(int j = 0; j<nb_hidden_nodes; j++){
                hidden_layer_bias[j] += delta_hidden[j] * lr;
                for(int k = 0; k < nb_inputs; k++){
                    hidden_weights[k][j] += training_inputs[i][k] * delta_hidden[j] * lr;
                }
            }

        }
    }

    printf("\n");

    printf("Poids finaux des neurones cachés : \n");
    for (int j=0; j<nb_hidden_nodes; j++) {
        printf("[ ");
        for(int k=0; k<nb_inputs; k++) {
            printf ("%f ", hidden_weights[k][j]);
        }
        printf("]\n");
    }

    printf("\n");
    printf("Biais finaux des neurones cachés : \n");
    for (int j=0; j<nb_hidden_nodes; j++) {
        printf ("%f ", hidden_layer_bias[j]);
    }

    printf("\n");
    printf("\nPoids finaux de l'output : \n");
    for (int j=0; j<nb_outputs; j++) {
        printf("[ ");
        for (int k=0; k<nb_hidden_nodes; k++) {
            printf ("%f ", output_weights[k][j]);
        }
        printf("]\n");
    }

    printf("\n");
    printf("Biais finaux de l'output : \n");
    for (int j=0; j<nb_outputs; j++) {
        printf ("%f ", output_layer_bias[j]);

    }

    printf("\n");
    printf("\n");
    return 1;
}
