require 'nn'
torch.setdefaulttensortype('torch.FloatTensor')

-- If mean and stdv not given, normalize the data so that the mean is 0 and the standard deviation is 1
-- Otherwise, shift and scale the data according to the given mean and standard deviation
function normalize(data, mean, stdv)
	mean = mean or {}
	stdv = stdv or {}
	for i = 1, 3 do -- over each image channel
		if mean[i] == nil then
			mean[i] = data[{ {}, {i}, {}, {}  }]:mean()    -- mean estimation
			print('Channel ' .. i .. ', Mean: ' .. mean[i])
		end
		data[{ {}, {i}, {}, {} }]:add(-mean[i]) -- mean subtraction
		
		if stdv[i] == nil then
			stdv[i] = data[{ {}, {i}, {}, {}  }]:std()  -- std estimation
			print('Channel ' .. i .. ', Standard Deviation: ' .. stdv[i])
		end
		data[{ {}, {i}, {}, {} }]:div(stdv[i])  -- std scaling
	end
	return mean, stdv
end

-- Prepare the dataset so that it can be used by nn.StochasticGradient trainer
function prepare(dataset) 
	-- Define the index of the dataset
    setmetatable(dataset, {
		__index = function(t, i) 
			return {t.data[i], t.label[i]} 
		end
	});

    -- Define the size of the dataset
	function dataset:size() 
		return self.data:size(1) 
	end
	
	return dataset
end

-- Build a LeNet convolutional neural network based on a set of parameters
function makeNetwork(params)
    -- Calculate the length of the second output layer
    local outputBLength = math.floor((math.floor((params.input - params.kernelA + 1) / params.maxPoolA) - params.kernelB + 1) / params.maxPoolB)

    -- Build the convolutional neural network
    local net = nn.Sequential()
    net:add(nn.SpatialConvolution(3, params.outputA, params.kernelA, params.kernelA))                    -- 3 input image channels, 6 output channels, 21x21 convolution kernel
    net:add(nn.ReLU())                                                                                   -- non-linearity 
    net:add(nn.SpatialMaxPooling(params.maxPoolA, params.maxPoolA, params.maxPoolA, params.maxPoolA))    -- A max-pooling operation that looks at 3x3 windows and finds the max.
    net:add(nn.SpatialConvolution(params.outputA, params.outputB, params.kernelB, params.kernelB))       -- 6 input image channels, 16 output channels, 21x21 convolution kernel
    net:add(nn.ReLU())                                                                                   -- non-linearity 
    net:add(nn.SpatialMaxPooling(params.maxPoolB, params.maxPoolB, params.maxPoolB, params.maxPoolB))    -- A max-pooling operation that looks at 4x4 windows and finds the max.
    net:add(nn.View(params.outputB * outputBLength * outputBLength))                                     -- reshapes from a 3D tensor of 16x10x10 into 1D tensor of 16*10*10
    net:add(nn.Linear(params.outputB * outputBLength * outputBLength, params.fullyA))                    -- fully connected layer (matrix multiplication between input and weights)
    net:add(nn.ReLU())                                                                                   -- non-linearity 
    net:add(nn.Linear(params.fullyA, params.fullyB))                                                     -- fully connected layer (matrix multiplication between input and weights)
    net:add(nn.ReLU())                                                                                   -- non-linearity 
    net:add(nn.Linear(params.fullyB, params.finalOutput))                                                -- 2 is the number of outputs of the network
    net:add(nn.LogSoftMax())                                                                             -- converts the output to a log-probability. Useful for classification problems

    return net
end

-- Build a LeNet convolutional neural network based on a set of parameters
function train(trainData, networkParams, alpha, iterations)
    -- Build a convolutional neural network
    local net = makeNetwork(networkParams)

    -- Set up a log-likelihood classification loss
    local criterion = nn.ClassNLLCriterion()

    local trainer = nn.StochasticGradient(net, criterion)
    trainer.learningRate = alpha
    trainer.maxIteration = iterations

    trainer:train(trainData)

    net:clearState()
    return net
end

function evaluate(testData, net)
    local correct = 0
    local dataLength = testData.data:size(1)

    for i = 1, dataLength do
        local groundTruth = testData.label[i]
        local prediction = net:forward(testData.data[i])
        local confidences, indices = torch.sort(prediction, true)  -- true means sort in descending order
        if groundTruth == indices[1] then
            correct = correct + 1
        end
    end

    net:clearState()
    return correct / dataLength
end

function main()
	local trainset = torch.load('data/tensors/binaryTrain.t7')
	local testset = torch.load('data/tensors/binaryTest.t7')

	-- Normalize the training data
	local mean, stdv = normalize(trainset.data)
	-- Normalize the testing data according to the normalizing paramaters used in the training data
	normalize(testset.data, mean, stdv)
	
	-- Prepare the training data
	local trainData = prepare(trainset)
	local testData = prepare(testset)

	-- Build a convolutional neural network
	local net = train(trainData, {
        input = 200,
        outputA = 6,
        kernelA = 21,
        maxPoolA = 3,
        outputB = 16,
        kernelB = 21,
        maxPoolB = 4,
        fullyA = 120,
        fullyB = 84,
        finalOutput = 2
    }, 0.01, 1)

    local accuracy = evaluate(testData, net)
    print(accuracy)
	
	-- Save the model
    savePath = 'data/models'
    os.execute("mkdir -p "..savePath)
	torch.save(savePath..'/net.t7', net)
end

main()
