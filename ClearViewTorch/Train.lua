require 'nn'
torch.setdefaulttensortype('torch.FloatTensor')

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

function prepare(dataset) 
    setmetatable(dataset, {
		__index = function(t, i) 
			return {t.data[i], t.label[i]} 
		end
	});

	function dataset:size() 
		return self.data:size(1) 
	end
	
	return dataset
end

function main()
	-- local trainset = torch.load('data/tensors/cifar10-train.t7')
	-- local testset = torch.load('data/tensors/cifar10-test.t7')
	
	-- print(testset.label)

	local trainset = torch.load('data/tensors/binaryTrain.t7')
	local testset = torch.load('data/tensors/binaryTest.t7')

	local mean, stdv = normalize(trainset.data)
	normalize(testset.data, mean, stdv)
	
	local trainData = prepare(trainset)
	local testData = prepare(testset)

	local net = nn.Sequential()
	net:add(nn.SpatialConvolution(3, 6, 21, 21)) -- 3 input image channels, 6 output channels, 5x5 convolution kernel
	net:add(nn.ReLU())                           -- non-linearity 
	net:add(nn.SpatialMaxPooling(3, 3, 3, 3))    -- A max-pooling operation that looks at 2x2 windows and finds the max.
	net:add(nn.SpatialConvolution(6, 16, 21, 21))  -- 3 input image channels, 6 output channels, 5x5 convolution kernel
	net:add(nn.ReLU())                           -- non-linearity 
	net:add(nn.SpatialMaxPooling(4, 4, 4, 4))
	net:add(nn.View(16 * 10 * 10))                 -- reshapes from a 3D tensor of 16x5x5 into 1D tensor of 16*5*5
	net:add(nn.Linear(16 * 10 * 10, 120))          -- fully connected layer (matrix multiplication between input and weights)
	net:add(nn.ReLU())                           -- non-linearity 
	net:add(nn.Linear(120, 84))
	net:add(nn.ReLU())                           -- non-linearity 
	net:add(nn.Linear(84, 2))                    -- 10 is the number of outputs of the network (in this case, 10 digits)
	net:add(nn.LogSoftMax())                     -- converts the output to a log-probability. Useful for classification problems

	local criterion = nn.ClassNLLCriterion()

	local trainer = nn.StochasticGradient(net, criterion)
	trainer.learningRate = 0.01
	trainer.maxIteration = 100
	
	trainer:train(trainData)
	
	net:clearState()
	torch.save('data/models/net.t7', net)
end

main()
