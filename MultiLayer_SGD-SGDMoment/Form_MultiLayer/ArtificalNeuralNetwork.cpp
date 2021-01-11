#include "ArtificalNeuralNetwork.h"
#include <stdlib.h>
#include "Process.h"
#include <math.h>
#include <iostream>

#define SIGMOIDLAMDA 2.0

void Layer::CreateLayer(int inputs) {
	this->inputs = inputs;
	this->weightSize = this->neurons * inputs;
	weight = new double[weightSize];
}

int Layer::GetNeurons() {
	return this->neurons;
}

int Layer::GetInputs() {
	return this->inputs;
}

int Layer::GetWeightSize() {
	return this->weightSize;
}

double Layer::GetWeight(int neuronIndex, int inputIndex) {
	return weight[neuronIndex * this->inputs + inputIndex];
}

double Layer::GetWeight(int index) {
	return weight[index];
}

void Layer::SetWeight(int index, double value) {
	weight[index] = value;
}

void Layer::WeightsUpdateSGD(int neuronIndex, double value, const double* input) {

	int startIndex = neuronIndex * this->inputs;

	for (int i = 0; i < this->inputs; i++)
		weight[i + startIndex] += (value * input[i]);
}

void Layer::WeightsUpdateSGDMoment(int neuronIndex, double value, const double* input, double a, double* lastDeltaWeights) {

	double deltaError = 0.0;
	int startIndex = neuronIndex * this->inputs;

	for (int i = 0; i < this->inputs; i++) {

		deltaError = value * input[i];
		weight[i + startIndex] += (deltaError + a * lastDeltaWeights[i + startIndex]);
		lastDeltaWeights[i + startIndex] = deltaError;
	}
}

double ANeuralNetwork::Activation(double net) {

	switch (this->activationType)
	{
	case Activations::Sigmoid:									// Bipolar Sigmoid Activation
		return (2.0 / (1.0 + exp(-SIGMOIDLAMDA * net)) - 1.0);

	case Activations::TanH:
		return tanh(net);
	
	//case Activations::ReLU:
	//	return net > 0.0 ? net : 0.0;
	//
	//case Activations::LReLU:
	//	return net > 0.0 ? net : 0.01 * net;
	//
	//case Activations::Swish:
	//	return net * (1.0 / (1.0 + exp(-net)));

	}
}

double ANeuralNetwork::ActivationDerivative(double net) {

	switch (this->activationType)
	{
	case Activations::Sigmoid:									// Bipolar Sigmoid Activation Gradient
		return 0.5 * (1.0 - pow(Activation(net), 2));

	case Activations::TanH:
		return (1.0 - pow(tanh(net), 2));

	//case Activations::ReLU:
	//	return net > 0.0 ? 1.0 : 0.0;

	//case Activations::LReLU:
	//	return net > 0.0 ? 1.0 : 0.01;

	//case Activations::Swish:
	//	double swish = Activation(net);
	//	return swish + (1.0 / (1.0 + exp(-net))) * (1.0 - swish);

	}
}

ANeuralNetwork::ANeuralNetwork(int inputNeurons, int hiddenLayers, int outputNeurons) {

	this->networkInputs = inputNeurons;
	this->networkOutputs = outputNeurons;
	this->totalLayers = hiddenLayers + 1;						// total layer = hidden layers + output layer

	layers = new Layer[totalLayers];

}


void ANeuralNetwork::SetLayerNeurons(int layerIndex, int layerNeurons) {

	layers[layerIndex].neurons = layerNeurons;
	totalNeurons += layerNeurons;
}


void ANeuralNetwork::SetTrainParameters(double learnRate, double momentRate, TrainType trainType, Activations activationType) {

	learningRate = new double[totalLayers];

	learningRate[0] = learnRate;
	for (int i = 1; i < totalLayers; i++)
	{
		learningRate[i] = learningRate[i - 1] + 0.01;
	}

	this->momentRate = momentRate;
	this->trainType = trainType;
	this->activationType = activationType;
}


double ANeuralNetwork::GetNetworkOutputs() {

	return networkOutputs;
}


void ANeuralNetwork::Create() {

	layers[0].CreateLayer(this->networkInputs);

	for (int layerIndex = 1; layerIndex < this->totalLayers; layerIndex++)
	{
		layers[layerIndex].CreateLayer(layers[layerIndex - 1].GetNeurons() + 1);		// layer[n].inputs = layer[n-1].neurons + 1 bias
	}


	nets = new double* [totalLayers];

	for (int i = 0; i < totalLayers; i++)
	{
		nets[i] = new double[layers[i].GetNeurons()];
	}

	outputs = new double* [totalLayers];

	for (int i = 0; i < totalLayers - 1; i++)
	{
		outputs[i] = new double[layers[i].GetNeurons() + 1];				// neurons + 1(Bias) 
		outputs[i][layers[i].GetNeurons()] = 1.0;							// Bias value setted
	}

	outputs[totalLayers - 1] = new double[layers[totalLayers - 1].GetNeurons()];


	S_ok = new double* [totalLayers];

	for (int i = 0; i < totalLayers; i++)
	{
		S_ok[i] = new double[layers[i].GetNeurons()];
	}


	if (trainType == TrainType::SGD_Moment) {

		weightsMoment = new double* [totalLayers];

		for (int i = 0; i < totalLayers; i++)
		{
			weightsMoment[i] = (double*)calloc(layers[i].GetWeightSize(), sizeof(double));
		}
	}

	endIndex = totalLayers - 1;
}


double ANeuralNetwork::Train(const double* inputs, const int* destinations) {

	this->inputSamples = inputs;
	this->destinations = destinations;

	FeedForward();
	BackPropagation();

	return this->totalError;
}


void ANeuralNetwork::FeedForward() {


	totalError = 0.0;

	MatrixMultiplication(layers[0].weight, inputSamples, nets[0], layers[0].GetNeurons(), networkInputs, 1);

	for (int outputIndex = 0; outputIndex < layers[0].GetNeurons(); outputIndex++) {

		outputs[0][outputIndex] = Activation(nets[0][outputIndex]);		// f(net)
	}


	for (int layerIndex = 1; layerIndex < totalLayers - 1; layerIndex++)
	{
		MatrixMultiplication(layers[layerIndex].weight, outputs[layerIndex - 1], nets[layerIndex], layers[layerIndex].GetNeurons(), layers[layerIndex].GetInputs(), 1);

		for (int outputIndex = 0; outputIndex < layers[layerIndex].GetNeurons(); outputIndex++) {

			outputs[layerIndex][outputIndex] = Activation(nets[layerIndex][outputIndex]);	// f(net)
		}
	}


	MatrixMultiplication(layers[endIndex].weight, outputs[endIndex - 1], nets[endIndex], layers[endIndex].GetNeurons(), layers[endIndex].GetInputs(), 1);

	for (int outputIndex = 0; outputIndex < layers[endIndex].GetNeurons(); outputIndex++) {

		outputs[endIndex][outputIndex] = Activation(nets[endIndex][outputIndex]);			// f(net)
		totalError += pow(((double)destinations[outputIndex] - outputs[endIndex][outputIndex]), 2);
	}

}


void ANeuralNetwork::BackPropagation() {


	// SON KATMAN	//////////////////////////////////////////////////////////////

	for (int k = 0; k < layers[endIndex].GetNeurons(); k++) {

		S_ok[endIndex][k] = ((double)destinations[k] - outputs[endIndex][k]) * ActivationDerivative(nets[endIndex][k]);		// (d - O) * f'(net)
	}

	// SON KATMANIN AGIRLIKLARI GUNCELLE
	for (int neuronIndex = 0; neuronIndex < layers[endIndex].GetNeurons(); neuronIndex++) {

		if (trainType == TrainType::SGD)
			layers[endIndex].WeightsUpdateSGD(neuronIndex, learningRate[endIndex] * S_ok[endIndex][neuronIndex], outputs[endIndex - 1]);

		else if (trainType == TrainType::SGD_Moment)
			layers[endIndex].WeightsUpdateSGDMoment(neuronIndex, learningRate[endIndex] * S_ok[endIndex][neuronIndex], outputs[endIndex - 1], momentRate, weightsMoment[endIndex]);

	}


	for (int layerIndex = endIndex - 1; layerIndex > 0; layerIndex--)
	{

		// SONDAN ONCEKI KATMANLAR	//////////////////////////////////////////////////////////////
		for (int neuronIndex = 0; neuronIndex < layers[layerIndex].GetNeurons(); neuronIndex++) {

			S_ok_W_kj = 0.0;

			for (int k = 0; k < layers[layerIndex + 1].GetNeurons(); k++) {

				S_ok_W_kj += (S_ok[layerIndex + 1][k] * layers[layerIndex + 1].GetWeight(k, neuronIndex));
			}

			S_ok[layerIndex][neuronIndex] = ActivationDerivative(nets[layerIndex][neuronIndex]) * S_ok_W_kj;

			if (trainType == TrainType::SGD)
				layers[layerIndex].WeightsUpdateSGD( neuronIndex, learningRate[layerIndex] * S_ok[layerIndex][neuronIndex], outputs[layerIndex - 1]);


			else if (trainType == TrainType::SGD_Moment)
				layers[layerIndex].WeightsUpdateSGDMoment( neuronIndex, learningRate[layerIndex] * S_ok[layerIndex][neuronIndex], outputs[layerIndex - 1], momentRate, weightsMoment[layerIndex]);

		}
	}


	// ILK KATMAN	//////////////////////////////////////////////////////////////
	for (int neuronIndex = 0; neuronIndex < layers[0].GetNeurons(); neuronIndex++) {

		S_ok_W_kj = 0.0;

		for (int k = 0; k < layers[1].GetNeurons(); k++) {

			S_ok_W_kj += (S_ok[1][k] * layers[1].GetWeight(k, neuronIndex));
		}

		S_ok[0][neuronIndex] = ActivationDerivative(nets[0][neuronIndex]) * S_ok_W_kj;

		if (trainType == TrainType::SGD)
			layers[0].WeightsUpdateSGD( neuronIndex, learningRate[0] * S_ok[0][neuronIndex], inputSamples);


		else if (trainType == TrainType::SGD_Moment)
			layers[0].WeightsUpdateSGDMoment( neuronIndex, learningRate[0] * S_ok[0][neuronIndex], inputSamples, momentRate, weightsMoment[0]);

	}
}


double* ANeuralNetwork::Predict(const double* inputSamples) {


	MatrixMultiplication(layers[0].weight, inputSamples, nets[0], layers[0].GetNeurons(), networkInputs, 1);

	for (int outputIndex = 0; outputIndex < layers[0].GetNeurons(); outputIndex++) {

		outputs[0][outputIndex] = Activation(nets[0][outputIndex]);							// f(net)
	}


	for (int layerIndex = 1; layerIndex < totalLayers - 1; layerIndex++)
	{
		MatrixMultiplication(layers[layerIndex].weight, outputs[layerIndex - 1], nets[layerIndex], layers[layerIndex].GetNeurons(), layers[layerIndex].GetInputs(), 1);

		for (int outputIndex = 0; outputIndex < layers[layerIndex].GetNeurons(); outputIndex++) {

			outputs[layerIndex][outputIndex] = Activation(nets[layerIndex][outputIndex]);	// f(net)
		}
	}


	MatrixMultiplication(layers[endIndex].weight, outputs[endIndex - 1], nets[endIndex], layers[endIndex].GetNeurons(), layers[endIndex].GetInputs(), 1);

	for (int outputIndex = 0; outputIndex < layers[endIndex].GetNeurons(); outputIndex++) {

		outputs[endIndex][outputIndex] = Activation(nets[endIndex][outputIndex]);			// f(net)
	}

	return outputs[endIndex];
}


double ANeuralNetwork::RMSE(double totalError, int totalSample) {

	return (double)(sqrt(0.5 * totalError) / (totalSample * this->totalNeurons));
}

