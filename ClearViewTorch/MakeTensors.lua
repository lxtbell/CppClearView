require 'image'
torch.setdefaulttensortype('torch.FloatTensor')

function scanFolder(folder)
    local fileList, popen = {}, io.popen
    local pfile = popen('ls -a "'..folder..'"')
    for file in pfile:lines() do
		if file ~= '.' and file ~= '..' then
			table.insert(fileList, file)
		end
    end
    pfile:close()
    return fileList
end

function getDataset(folder, filter)
	local dataset = torch.Tensor()
	for i, file in ipairs(scanFolder(folder)) do
		if filter(i, file) then
			local img = image.load(folder..'/'..file)
			local data = img:view(1, img:size(1), img:size(2), img:size(3)):float()
			dataset = torch.cat(dataset, data, 1)
		end
	end
	return dataset
end

function main()
	local merged = getDataset('data/normalized/merged', function(i, file)
		return string.byte(file:sub(1, 1)) % 2 == 0
	end)
	local transmissions = getDataset('data/normalized/transmissions', function(i, file)
		return string.byte(file:sub(1, 1)) % 2 == 1
	end)

	local dataset = torch.cat(merged, transmissions, 1)
	local labels = torch.cat(torch.Tensor(merged:size(1)):fill(1), torch.Tensor(transmissions:size(1)):fill(2))
	local datasetLength = dataset:size(1)

	local shuffle = torch.randperm(datasetLength)
	local datasetShuffled, labelsShuffled = torch.Tensor(), torch.Tensor()
	for i = 1, datasetLength do
		local data = dataset[shuffle[i]]
		local label = torch.Tensor(1):fill(labels[shuffle[i]])
		data = data:view(1, data:size(1), data:size(2), data:size(3))
		
		datasetShuffled = torch.cat(datasetShuffled, data, 1)
		labelsShuffled = torch.cat(labelsShuffled, label, 1)
	end

	testLength = datasetLength / 5
	torch.save('data/tensors/binaryTrain.t7', {
		data = datasetShuffled[{ {testLength + 1, datasetLength}, {}, {}, {} }], 
		label = labelsShuffled[{ {testLength + 1, datasetLength} }]
	})
	torch.save('data/tensors/binaryTest.t7', {
		data = datasetShuffled[{ {1, testLength}, {}, {}, {} }],
		label = labelsShuffled[{ {1, testLength} }]
	})
end

main()
