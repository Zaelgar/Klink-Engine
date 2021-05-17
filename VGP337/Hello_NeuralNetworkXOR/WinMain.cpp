#include <Klink/Inc/Klink.h>
#include <iostream>



int main()
{
	std::vector<size_t> topology{ 2, 2, 1 };

	Klink::ML::NeuralNetwork xorNetwork(topology);

	std::vector<std::vector<float>> inputValues(4);
	inputValues[0].push_back(0.0f);
	inputValues[0].push_back(0.0f);

	inputValues[1].push_back(1.0f);
	inputValues[1].push_back(0.0f);

	inputValues[2].push_back(0.0f);
	inputValues[2].push_back(1.0f);

	inputValues[3].push_back(1.0f);
	inputValues[3].push_back(1.0f);

	std::vector<std::vector<float>> targetValues(4);
	targetValues[0].push_back(-1.0f);
	targetValues[1].push_back(1.0f);
	targetValues[2].push_back(1.0f);
	targetValues[3].push_back(-1.0f);


	std::vector<float> outputValues;
	size_t trainingCount = 2000;
	size_t pickedTest = 0;
	float error = 0.0f;
	float threshold = 0.1f;
	for (size_t i = 0; i < trainingCount; ++i)
	{
		// Pick a test (0, 1, 2, 3)
		pickedTest = Klink::JMath::Random::RandomIntUniform(0, 3);

		// Feed forward with that data
		xorNetwork.FeedFoward(inputValues[pickedTest]);

		// Get results of feed from neural net
		outputValues = xorNetwork.GetResults();

		error = outputValues[0] - targetValues[pickedTest][0];

		// Back propagate with real values
		xorNetwork.BackPropagate(targetValues[pickedTest]);

		// Print test
		std::cout << "Training Session # " << i << std::endl
			<< "Inputs: " << inputValues[pickedTest][0] << ", " << inputValues[pickedTest][1] << std::endl
			<< "Result: " << outputValues[0] << std::endl
			<< "Actual: " << targetValues[pickedTest][0] << std::endl
			<< "Error: " << error << std::endl;
		if (outputValues[0] <= 0)
		{
			std::cout << "I think this is NOT exclusive." << std::endl << std::endl;
		}
		else
		{
			std::cout << "I think this is exclusive." << std::endl << std::endl;
		}
	}

	return 0;
}