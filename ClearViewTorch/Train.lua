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
	local net = nn.Sequential()
	net:add(nn.SpatialConvolution(3, 6, 21, 21)) -- 3 input image channels, 6 output channels, 21x21 convolution kernel
	net:add(nn.ReLU())                           -- non-linearity 
	net:add(nn.SpatialMaxPooling(3, 3, 3, 3))    -- A max-pooling operation that looks at 3x3 windows and finds the max.
	net:add(nn.SpatialConvolution(6, 16, 21, 21))-- 6 input image channels, 6 output channels, 21x21 convolution kernel
	net:add(nn.ReLU())                           -- non-linearity 
	net:add(nn.SpatialMaxPooling(4, 4, 4, 4))    -- A max-pooling operation that looks at 4x4 windows and finds the max.
	net:add(nn.View(16 * 10 * 10))               -- reshapes from a 3D tensor of 16x10x10 into 1D tensor of 16*10*10
	net:add(nn.Linear(16 * 10 * 10, 120))        -- fully connected layer (matrix multiplication between input and weights)
	net:add(nn.ReLU())                           -- non-linearity 
	net:add(nn.Linear(120, 84))                  -- fully connected layer (matrix multiplication between input and weights)
	net:add(nn.ReLU())                           -- non-linearity 
	net:add(nn.Linear(84, 2))                    -- 2 is the number of outputs of the network
	net:add(nn.LogSoftMax())                     -- converts the output to a log-probability. Useful for classification problems

	-- Set up a log-likelihood classification loss
	local criterion = nn.ClassNLLCriterion()

	local trainer = nn.StochasticGradient(net, criterion)
	trainer.learningRate = 0.01
	trainer.maxIteration = 100
	
	-- Train with the nn.StochasticGradient trainer
	trainer:train(trainData)
	
	-- Save the model
	net:clearState()
	torch.save('data/models/net.t7', net)
end

main()
